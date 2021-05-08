#include <stdio.h>
#include <stdlib.h>


FILE *infile;

int func(int a, int b){
	return a + b;
}

int main(int argc, char *argv[]){
    infile = stdin;


    char input[256];
    sscanf("%s", &input[0]);

    printf("hello world\n");
    printf("%s\n", input);

    printf("Calling func\n");
    int printing_value = func(1, 2);
    printf("%d\n", printing_value);

    return 0;


}





