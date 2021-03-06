#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include "filter.h"
#include "signal.h"
#include "timing.h"

#define MAXWIDTH 40
#define THRESHOLD 2.0
#define ALIENS_LOW  50000.0
#define ALIENS_HIGH 150000.0

int num_bands;
int num_procs;
int num_threads;
pthread_t *tids;

typedef struct {
    double bandwidth;
    double* filter_coeffs;
    double* band_power;
    signal* sig;
    int worker_idx;
    int filter_order;
    int start; // first band to be processed (inclusive)
    int end; // last band to be processed (not inclusive)
  } worker_struct;

void usage()
{
  printf("usage: p_band_scan text|bin|mmap signal_file Fs filter_order num_bands num_processsors num_threads\n");
}

double avg_power(double *data, int num)
{
  double ss = 0;
  for (int i = 0; i < num; i++) {
    ss += data[i] * data[i];
  }

  return ss / num;
}

double max_of(double* data, int num) {

  double m = data[0];
  for (int i = 1; i < num; i++) {
    if (data[i] > m) {
      m = data[i];
    }
  }
  return m;
}

double avg_of(double* data, int num) {

  double s = 0;
  for (int i = 0; i < num; i++) {
    s += data[i];
  }
  return s / num;
}

void remove_dc(double* data, int num) {

  double dc = avg_of(data,num);

  printf("Removing DC component of %lf\n",dc);

  for (int i = 0; i < num; i++) {
    data[i] -= dc;
  }
}

void* worker(void* arg) {
  worker_struct *worker_data = (worker_struct*) arg;

  printf("Starting thread... \n"); ////////////
  int start = worker_data->start;
  int end = worker_data->end;
  signal* sig = worker_data->sig;
  double* filter_coeffs = worker_data->filter_coeffs;
  double* band_power = worker_data->band_power;
  double bandwidth = worker_data->bandwidth;
  int filter_order = worker_data->filter_order;

  printf("Starting loop...\n");
  for(int band = start; band < end; band++) {
    printf("band %d\n", band);/////////
    printf("Generating band pass\n");//////////
    double *curr_coeffs = &filter_coeffs[band * (filter_order + 1)];
    generate_band_pass(sig->Fs,
                      band * bandwidth + 0.0001,
                      (band + 1) * bandwidth - 0.0001,
                      filter_order,
                      curr_coeffs);
    hamming_window(filter_order, curr_coeffs);

    // Convolve
    printf("Convolving\n");/////////
    convolve_and_compute_power(sig->num_samples,
                               sig->data,
                               filter_order,
                               curr_coeffs,
                               &(band_power[band]));
    printf("Convolved\n");
  }
  printf("Finishing thread...\n"); ////////////
  pthread_exit(NULL);
}

int analyze_signal(signal *sig, int filter_order, int num_bands, int num_procs, int num_threads, double *lb, double *ub)
{
  printf("Starting analyze signal...\n"); //////////////////
  double Fc        = (sig->Fs) / 2;
  double bandwidth = Fc / num_bands;

  tids = (pthread_t *) malloc(sizeof(pthread_t)*num_threads);
  if (!tids) {
      fprintf(stderr, "malloc failed: couldn't find memory space\n");
      exit(-1);
  }

  remove_dc(sig->data,sig->num_samples);

  double signal_power = avg_power(sig->data,sig->num_samples);

  printf("signal average power:     %lf\n", signal_power);
  resources rstart;
  get_resources(&rstart,THIS_PROCESS);
  double start = get_seconds();
  unsigned long long tstart = get_cycle_count();
  double filter_coeffs[(filter_order+1)*(num_bands)];
  double band_power[num_bands];

  int bands_per_thread = num_bands / num_threads;
  int remainder_bands = num_bands % num_threads;

  for (int thread_idx = 0; thread_idx < num_threads; thread_idx++) {
    worker_struct* worker_input = malloc(sizeof(worker_struct));

    worker_input->bandwidth = bandwidth;
    worker_input->filter_order = filter_order;
    worker_input->filter_coeffs = filter_coeffs;
    worker_input->sig = sig;
    worker_input->band_power = band_power;
    worker_input->start = thread_idx * bands_per_thread;
    worker_input->end = (thread_idx + 1) * bands_per_thread;
    if ((thread_idx + 1) == num_threads){
      worker_input->end = thread_idx * bands_per_thread + remainder_bands;
    }

    long code = pthread_create( &(tids[thread_idx]),
                         NULL,
                         worker,
                         worker_input);
    if (code != 0) {
      perror("Error creating thread\n");
      exit(-1);
    }
  }

  printf("Joining threads \n");////////////
  for (int i = 0; i < num_threads; i++) {
    pthread_t curr_tid = tids[i];
    int returncode = pthread_join(curr_tid, NULL);
    if (returncode != 0) {
      perror("join failed");
      exit(-1);
    }
  }

 unsigned long long tend = get_cycle_count();
  double end = get_seconds();

  resources rend;
  get_resources(&rend,THIS_PROCESS);

  resources rdiff;
  get_resources_diff(&rstart, &rend, &rdiff);

  // Pretty print results
  double max_band_power = max_of(band_power,num_bands);
  double avg_band_power = avg_of(band_power,num_bands);
  int wow = 0;
  *lb = -1;
  *ub = -1;

  for (int band = 0; band < num_bands; band++) {
    double band_low  = band * bandwidth + 0.0001;
    double band_high = (band + 1) * bandwidth - 0.0001;

    printf("%5d %20lf to %20lf Hz: %20lf ",
           band, band_low, band_high, band_power[band]);

    for (int i = 0; i < MAXWIDTH * (band_power[band] / max_band_power); i++) {
      printf("*");
    }

    if ((band_low >= ALIENS_LOW && band_low <= ALIENS_HIGH) ||
        (band_high >= ALIENS_LOW && band_high <= ALIENS_HIGH)) {

      // band of interest
      if (band_power[band] > THRESHOLD * avg_band_power) {
        printf("(WOW)");
        wow = 1;
        if (*lb < 0) {
          *lb = band * bandwidth + 0.0001;
        }
        *ub = (band + 1) * bandwidth - 0.0001;
      } else {
        printf("(meh)");
      }
    } else {
      printf("(meh)");
    }

    printf("\n");
  }

  printf("Resource usages:\n"
         "User time        %lf seconds\n"
         "System time      %lf seconds\n"
         "Page faults      %ld\n"
         "Page swaps       %ld\n"
         "Blocks of I/O    %ld\n"
         "Signals caught   %ld\n"
         "Context switches %ld\n",
         rdiff.usertime,
         rdiff.systime,
         rdiff.pagefaults,
         rdiff.pageswaps,
         rdiff.ioblocks,
         rdiff.sigs,
         rdiff.contextswitches);

  printf("Analysis took %llu cycles (%lf seconds) by cycle count, timing overhead=%llu cycles\n"
         "Note that cycle count only makes sense if the thread stayed on one core\n",
         tend - tstart, cycles_to_seconds(tend - tstart), timing_overhead());
  printf("Analysis took %lf seconds by basic timing\n", end - start);

  return wow;
}

int main(int argc, char *argv[])
{
  signal *sig;
  double Fs;
  char sig_type;
  char *sig_file;
  int filter_order;
  int num_bands;
  double start, end;

  if (argc != 8) {
    usage();
    return -1;
  }

  char sig_type    = toupper(argv[1][0]);
  char* sig_file   = argv[2];
  double Fs        = atof(argv[3]);
  int filter_order = atoi(argv[4]);
  int num_bands    = atoi(argv[5]);
  int num_threads = atoi(argv[6]);
  int num_processors = atoi(argv[7]);

  assert(Fs>0.0);
  assert(filter_order>0 && !(filter_order & 0x1));
  assert(num_bands>0);

  printf("type:     %s\n"
         "file:     %s\n"
         "Fs:       %lf Hz\n"
         "order:    %d\n"
         "bands:    %d\n"
         "processes: %d\n"
         "threads: %d\n",
         sig_type=='T' ? "Text" : sig_type=='B' ? "Binary" : sig_type=='M' ? "Mapped Binary" : "UNKNOWN TYPE",
         sig_file,
         Fs,
         filter_order,
         num_bands,
         num_procs,
         num_threads);

  printf("Load or map file\n");

  switch (sig_type) {
    case 'T':
    sig = load_text_format_signal(sig_file);
    break;

    case 'B':
    sig = load_binary_format_signal(sig_file);
    break;

    case 'M':
    sig = map_binary_format_signal(sig_file);
    break;

    default:
    printf("Unknown signal type\n");
    return -1;
  }

  if (!sig) {
    printf("Unable to load or map file\n");
    return -1;
  }

  sig->Fs=Fs;

  if (analyze_signal(sig,filter_order,num_bands, num_procs, num_threads, &start,&end)) {
    printf("POSSIBLE ALIENS %lf-%lf HZ (CENTER %lf HZ)\n",start,end,(end+start)/2.0);
  } else {
    printf("no aliens\n");
  }

  free_signal(sig);

  return 0;
}