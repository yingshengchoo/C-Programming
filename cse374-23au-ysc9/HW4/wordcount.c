/* wordcount.c
   Author: Vincent Choo
   UWNetID: ysc9
   Homework 4: C Mimic of bash command wc
   This program recreates basic fuctionality of the bash command wc.
   It calculates and prints out the number of lines, total lines words, and
   characters as specified.
   Copyright 2023 Ying Sheng Choo
*/
#define MAXLINE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Params: Takes the pointer of the file being counted, and the pointer to
// pointers that short the words, lines, character, and total line counts.
// Counts the number of words, lines, characters. Also adds to the running count
// of the number of total lines.
void count(FILE *file, int *words, int *lines, int *chars, int *total) {
  char line[MAXLINE];
  while (fgets(line, MAXLINE, file) != NULL) {
    (*lines)++;
    (*total)++;
    (*chars) += strlen(line);
    char *token = strtok(line, " \t\n");
    while (token != NULL) {
      (*words)++;
      token = strtok(NULL, " \t\n");
    }
  }
}

int main(int argc, char *argv[]) {
  // checks to see if there are at least 2 arguments.
  if (argc < 2) {
      fprintf(stderr, "Usage: %s requires an input file.\n", argv[0]);
      return EXIT_FAILURE;
  }

  int tot = 0; int printLines = 0; int printWords = 0; int printChars = 0;

  // checks for option input. If there is no option input, the first file is
  // counted instead.
  if (strcmp(argv[1], "-l") == 0) {
    printLines = 1;
  } else if (strcmp(argv[1], "-w") == 0) {
    printWords = 1;
  } else if (strcmp(argv[1], "-c") == 0) {
    printChars = 1;
  } else {
    char *file_to_open = argv[1];
    FILE *file = fopen(file_to_open, "r");
    int w = 0; int l = 0; int ch = 0;
    if (file) {
      count(file, &w, &l, &ch, &tot);
      fclose(file);
      printf("%d %d %d %s\n", l, w, ch, argv[1]);
    } else {
      fprintf(stderr, "%s will not open.  Skipping.\n", argv[1]);
    }
  }

  // Throws an error if there is an option but no input files.
  if ((printWords == 1 || printChars == 1 || printLines == 1) && argc < 3) {
    fprintf(stderr, "Usage: %s requires an input file.\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Loops through all the input files except for the first one. Skips
  // files that cannot be opened. Print results based on the option input.
  for (int i = 2; i < argc; i ++) {
    char *file_to_open = argv[i];
    FILE *file = fopen(file_to_open, "r");
    int w = 0; int l = 0; int ch = 0;

    if (file) {
      count(file, &w, &l, &ch, &tot);
      fclose(file);
    } else {
      fprintf(stderr, "%s will not open.  Skipping.\n", argv[i]);
      break;
    }

    if (printLines == 1) {
      printf("%d\n", l);
    } else if (printWords == 1) {
      printf("%d\n", w);
    } else if (printChars == 1) {
      printf("%d\n", ch);
    } else {
      printf("%d %d %d %s\n", l, w, ch, argv[i]);
    }
  }

  // prints out the total number of lines if no option was selected
  if (printWords == 0 && printChars == 0 && printLines == 0) {
    printf("Total Lines = %d\n", tot);
  }

  return EXIT_SUCCESS;
}
