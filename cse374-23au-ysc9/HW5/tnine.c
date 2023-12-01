/* tnine is a program that drives a trie / t9 program.  This code
   will build a trie, according to trienode.  It will also run
   an interactive session where the user can retrieve words using
   t9 sequences.
   CSE374, HW5, 22wi
   Copyright 2023 Vincent Choo
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

// run_session run the on-going decoding interaction with the
// user.  It requires a previously build wordTrie to work.
void run_session(trieNode *wordTrie) {
  char* key = (char *) malloc(MAXLEN);
  char* lastKey = (char *) malloc(MAXLEN);

  if (!key || !lastKey)   {
    fprintf(stderr, "Error: Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  strncpy(lastKey, "#", 2);

  printf("Enter \"exit\" to quit.\n");
  printf("Enter Key Sequence (or \"#\" for next word):\n");
  while (fgets(key, MAXLEN, stdin)) {
    if (strcmp(key, "exit\n") == 0) {
      break;
    }

    if (strcmp(key, "#\n") == 0) {
      strcat(lastKey, key);
      printf("%s\n", get_word(wordTrie, lastKey));
    } else {
      printf("%s\n", get_word(wordTrie, key));
    }
    strncpy(lastKey, key, strlen(key)-1);
    printf("Enter Key Sequence (or \"#\" for next word):\n");
  }
  free(lastKey);
  free(key);
}

int main(int argc, char **argv) {
  FILE *dictionary = NULL;
  trieNode *wordTrie = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }

  // build the trie
  wordTrie = build_tree(dictionary);

  // run interactive session
  run_session(wordTrie);

  // clean up
  free_tree(wordTrie);

  return(EXIT_SUCCESS);
}
