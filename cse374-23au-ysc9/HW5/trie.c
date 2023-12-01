/* trie implements a trie, made of trieNodes. This includes
   code to build, search, and delete a trie
   CSE374, HW5, 22wi
   Copyright 2023 Vincent Choo
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

/* Translates the given character key into the corresponding T9
   Key.
*/
static int charToT9Key(char c) {
    if (c >= 'a' && c <= 'c') {
        return 2;
    }
    if (c >= 'd' && c <= 'f') {
        return 3;
    }
    if (c >= 'g' && c <= 'i') {
        return 4;
    }
    if (c >= 'j' && c <= 'l') {
        return 5;
    }
    if (c >= 'm' && c <= 'o') {
        return 6;
    }
    if (c >= 'p' && c <= 's') {
        return 7;
    }
    if (c >= 't' && c <= 'v') {
        return 8;
    }
    if (c >= 'w' && c <= 'z') {
        return 9;
    }
    return -1;
}

/* Initializes a trieNode by alloacting memory to the nodes.
*/
static trieNode* createTrieNode() {
    trieNode *node = (trieNode*)malloc(sizeof(trieNode));
    if (node) {
       node->word = NULL;
       for (int i = 0; i < BRANCHES; i++) {
            node->branches[i] = NULL;
       }
    }
    return node;
}

// Adds a word to the existing dictionary(root)
void insertWord(trieNode* root, char* word) {
    trieNode* current = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int key = charToT9Key(word[i]);
        if (!current->branches[key]) {
            current->branches[key] = createTrieNode();
        }
        current = current->branches[key];
    }
    while (current->word != NULL) {
        if (current->branches[0] == NULL) {
           current->branches[0] = createTrieNode();
        }
        current = current->branches[0];
    }
    current->word = (char*)malloc(length + 3);
    strncpy(current->word, "'", 2);
    strcat(current->word, word);
    strcat(current->word, "'");
}

// given a word list (dict), build a trie that holds
// each word. Return pointer to the root node.
trieNode* build_tree(FILE *dict) {
    trieNode* root = createTrieNode();
    char* word = (char *) malloc(MAXLEN);
    while (fgets(word, MAXLEN, dict) != NULL) {
        int length = strlen(word);
        if (word[length - 1] == '\n') {
            word[length - 1] = '\0';
        }
        insertWord(root, word);
    }

    fclose(dict);
    free(word);
    return root;
}

// given a pattern, return the word stored in the
// appropriate trieNode
char* get_word(trieNode *root, char *pattern) {
  trieNode* current = root;
  int length = strlen(pattern) - 1;
  for (int i = 0; i < length; i++) {
    if (pattern[i] == '#') {
      if (current->branches[0] == NULL) {
        return "There are no more T9nonyms.";
      }
      current = current->branches[0];
    } else {
      int index = pattern[i] - '0';
      if (current->branches[index] == NULL) {
        return "Not found in current dictionary.";
      } else {
        current = current->branches[index];
      }
    }
  }

  if (current == NULL) {
    return "There are no more T9onyms.";
  } else if (current->word == NULL) {
    return "Not found in current dictionary.";
  } else {
    return current->word;
  }
}

// deletes the entire tree branching from root
void free_tree(trieNode *root) {
  if (!root) {
    return;
  }

  for (int i = 0; i < BRANCHES; i++) {
    free_tree(root->branches[i]);
  }
  if (root->word != NULL) {
    free(root->word);
  }

  free(root);
}
