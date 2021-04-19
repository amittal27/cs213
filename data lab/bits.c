/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* Bit manipulations */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) { //done - 4 ops
  /* use De Morgan's law for bit and operation */
  return ~(~x | ~y);
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) { //done - 6 ops
  int word = 73;
  int pattern = word << 3 | 1;
  int result = pattern; 

  result |= pattern << 12;
  result |= word << 24;
  return result;
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) { //done - 7 ops
  /* compare to a pattern 0x55555555 and see if the resulting "and" is 0 */
  int one_byte = 85;
  int two_byte = one_byte << 8 | one_byte; 
  int four_byte = two_byte << 16 | two_byte; 
  int matched_pattern = x & four_byte; //will return four_byte only if all evens. 
  int four_byte_difference = matched_pattern ^ four_byte; // will return 0 only if the matched pattern is the same as four_byte (all evens)

  return !four_byte_difference; // matched_pattern ^ four_byte + 1;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {  // x = 80 00 00 00, n = 0 m = 3
  // isolate bits n and m
  int byte1_pattern = 255 << 8 * n; // 00 00 00 FF
  int byte2_pattern = 255 << 8 * m; // FF 00 00 00
  int sum_bytes = byte1_pattern | byte2_pattern; // FF 00 00 FF
  int negated_sum_bytes = ~sum_bytes; // 00 FF FF 00
  int result = negated_sum_bytes & x; // 00 00 00 00; 0s in place of swap bytes
  unsigned int actual1 = byte1_pattern & x; // 00 00 00 00
  unsigned int actual2 = byte2_pattern & x; // 80 00 00 00
  actual1 = actual1 >> 8*n << 8*m; // 00 00 00 00
  actual2 = actual2 >> 8*m << 8*n; // 00 00 00 80
  
  result |= actual1; // 00 00 00 00
  result |= actual2; // 00 00 00 80

  //printf("%d\n", actual1);
  //printf("%d\n", actual2);

  return result;

//byteSwap(-2147483648, 0, 3);
  
  
  // int first_byte_pattern = 255;
  // int first_byte = first_byte_pattern & x; // first actual byte (LSB) (byte 0)
  // int second_byte_pattern = 255 << 8;
  // int second_byte = second_byte_pattern & x; // second actual byte
  // int third_byte_pattern = 255 << 16;
  // int third_byte = third_byte_pattern & x; // third actual byte
  // int fourth_byte_pattern = 255 << 24;
  // int fourth_byte = fourth_byte_pattern & x; // fourth actual byte (MSB) (byte 3)

  // int is_m_1 = !(m ^ 0) << 16 ;

  // !(m ^ 1)

  // int new_word = 
  // //
  // return 2;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* or all of the bits together so that the entire word is filled with 1s if at least one 1 is present */
  // x |= x >> 1;
  // x |= x << 1;
  // x |= x >> 2;
  // x |= x << 2;
  // x |= x >> 4;
  // x |= x << 4;
  // x |= x >> 8;
  // x |= x << 8;
  // x |= x >> 16;
  // x |= x << 16;
  // return (x & y) | (~x & z);
  return ((~(!x >> 1) & ~x) & y) | ((!x >> 1) & z);
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) { //done - 18 ops
  int word = x;
  int pattern_16bit = (255 << 8) + 255; //first 16 bit are zero, last 16 bit are one. 
  word = (word >> 16) ^ (word & pattern_16bit); //symmetric difference, evens cancel out. 
  int pattern_8bit = 255;
  word = (word >> 8) ^ (word & pattern_8bit);
  int pattern_4bit = 15;
  word = (word >> 4) ^ (word & pattern_4bit);
  int pattern_2bit = 3;
  word = (word >> 2) ^ (word & pattern_2bit);
  int pattern_1bit = 1;
  int result = (word >> 1) ^ (word & pattern_1bit); //if it is 1, odd number. otherwise, 0. 
  result = result & 1; //discard leading 1s (on the left) from the arithmetic right shifts. 

  return result;
}
/* Two's complement arithmetic */
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */

int tmax(void) { //done - 4 ops
  // return 0 and all 1s. FF shift 
  int pattern_16bit = (255 << 8) + 255;

  return (pattern_16bit << 15) | pattern_16bit;
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) { //done - 10 ops
  int number = x;
  int minus_one = ~0; // 32-bit all ones. 
  //printf("Number\t%d\n", number);
  int is_negative = (number >> 31) ^ 1; // by arithmetic shift, this is -2 if negative, 1 if positive or 0
  int is_not_negative = !(is_negative + minus_one); // 0 if negative, 1 if positive or 0. 
  //printf("is_negative\t%d\n", is_negative);
  //printf("is_not_negative\t%d\n", is_not_negative);
  
  int is_0 = !number; // this is 1 if it is 0, and 0 if it is not 0. 
  int is_not_0 = !is_0; // now, this is 0 if it is 0, and 1 if it is not zero. 
  int is_positive = is_not_0 & is_not_negative;
  //printf("is_positive\t%d\n", is_positive);

  return minus_one + is_not_negative + is_positive;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int negative_x = ~x + 1;
  int sum = y + negative_x;
  int is_negative = sum >> 31;
 
  return is_negative + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  return 2;
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  return 2;
}
/* FP operations */
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) { //done - 9 ops
  int exp_mask = 0xff << 23; //mask for the exponent. 
  int exp_is_not_all_ones = (uf & exp_mask) ^ exp_mask; // 0 if it is all ones, number if it is not all ones
  int frac_is_not_all_zeros = uf << 9; // 0 if it is all zeros, number if it is not all 0s
  int exp_is_all_ones = ! exp_is_not_all_ones;

  int isNaN = exp_is_all_ones && frac_is_not_all_zeros;
  if (isNaN){ //if this is a NaN, return the same
    return uf;
  }
  //else
  return (uf) + (1 << 31);
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {  //done - 10 ops
  int exp_mask = 0xff;
  int exp_on_lsb = uf >> 23; //move the exponent to the least significant byte
  int isolated_exp = exp_on_lsb & exp_mask; //discard everything else. 
  int exp_is_not_all_ones = isolated_exp ^ exp_mask; // 0 if all ones, number if not all ones. 
  int frac_is_not_all_zeros = uf << 9; // 0 if it is all zeros, number if it is not all 0s
  int frac_is_all_zeros = ! frac_is_not_all_zeros;
  
  int is_not_NaN = frac_is_all_zeros || exp_is_not_all_ones;
  if (is_not_NaN){
    return (uf) & ~((1 << 31));
  }
  //else
  return uf;

}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  //lets start with the normalized case. 
  int number = uf;
  int minus_one = ~0;
  int sign_mask = (1<<31);
  int negative_sign = number & sign_mask; // 0 if it is positive, 0x80000000 if it is negative. 

  int exp_on_lsb = uf >> 23; //move the exponent to the least significant byte
  int isolated_exp = exp_on_lsb & 0xff; //discard everything else.
  int frac = number & 0x008fffff;
  int actual_exp =  isolated_exp + (~ 126); //subtract the bias (~126 is -127). this follows from ~0 = -1 and ~1 = -2

  //denormalized case or exponent < 0 case. 
  int exp_is_all_zeros = !(isolated_exp^0); // 0 if exp is all zeros, 1 if it is not all zeros
  int exponent_is_negative = actual_exp & sign_mask; // 0 if zero or positive, number if negative
  if (exp_is_all_zeros || exponent_is_negative){
    return 0; //return out of range value (the same as negative_sign if negative...)
  }

  //out of range case
  int greater_than_31 = !((actual_exp + (~30))&(sign_mask)) ; //subtract 31 from actual_exp and check + sign
  int out_of_range = (greater_than_31);
  if (out_of_range){
    return 0x80000000;
  }
  int result = 1 << actual_exp; // leading 1 of the mantissa 

  actual_exp = actual_exp + minus_one;
  int is_actual_exp_positive = !(actual_exp & sign_mask); //check that sign is not negative. 
  while (is_actual_exp_positive){
    result = result + ((frac & 1) << actual_exp); //get last frac digit. 

    frac = frac >> 1;
    actual_exp = actual_exp + minus_one;
    is_actual_exp_positive = !(actual_exp & sign_mask);
  }

  if (negative_sign){
    result = ~result + 1;
    result = result | negative_sign;
  }
  return result | negative_sign;
}
