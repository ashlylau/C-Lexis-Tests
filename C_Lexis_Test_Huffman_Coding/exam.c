#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exam.h"

/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);
static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
  if (s == NULL) {
    perror("contains");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < strlen(s); i++) {
    if (c == s[i]) {
      return 1;
    }
  }
  return 0;
}
//works

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
  if (s == NULL) {
    perror("frequency");
    exit(EXIT_FAILURE);
  }
  int count = 0;
  for (int i = 0; i < strlen(s); i++) {
    if (c == s[i]) {
      count++;
    }
  }
  return count;
}
//worksss

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
  if (s == NULL) {
    perror("nub");
    exit(EXIT_FAILURE);
  }
  char *result = malloc(MAX_STRING_LENGTH - 1);
  if (result == NULL) {
    perror("nub");
    exit(EXIT_FAILURE);
  }
  int index = 0;
  for (int i = 0; i < strlen(s); i++) {
    if (contains(result, s[i]) == 0) { //char is not added to result yet
      result[strlen(result)] = s[i];
    }
    index++;
  }
  return result;
}
//worksssss

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l,
                                            huffman_tree_t *t) {
  if (l == NULL || t == NULL) {
    perror("add");
    exit(EXIT_FAILURE);
  }
  huffman_tree_t *thisTree = malloc(sizeof(*thisTree));
  thisTree = l->tree;
  huffman_tree_list_t *thisList = malloc(sizeof(*thisList));
  thisList = l;
  huffman_tree_list_t *newList = malloc(sizeof(*newList));
  if (thisTree->count > t->count) { //add to head of list
    newList->tree = t;
    newList->next = l;
    return newList;
  }
  else {
    huffman_tree_list_t *nextList = malloc(sizeof(*nextList));
    nextList = l->next;
    huffman_tree_t *nextTree = malloc(sizeof(*nextTree));
    nextTree = l->next->tree;
    while (nextTree->count < t->count || nextList == NULL) {
      thisList = nextList;
      nextList = nextList->next;
      nextTree = nextList->tree;
    }
    newList->tree = t;
    newList->next = nextList;
    thisList->next = newList;
    return l;
  }
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {
  if (s == NULL || t == NULL) {
    perror("list_build");
    exit(EXIT_FAILURE);
  }

  //create first tree and initiate first list;
  huffman_tree_list_t *list = malloc(sizeof(*list));
  char letter = t[0];
  int freq = frequency(s, letter);
  huffman_tree_t *firstTree = malloc(sizeof(*firstTree));
  firstTree->count = freq;
  firstTree->letter = letter;
  firstTree->left = NULL;
  firstTree->right = NULL;
  list->tree = firstTree;
  list->next = NULL;

  //iterate through to create the rest of trees and add them to the list
  for (int i = 1; i < strlen(t); i++) {
    char letter = t[i];
    int freq = frequency(s, letter);
    huffman_tree_t *newTree = malloc(sizeof(*newTree));
    newTree->count = freq;
    newTree->letter = letter;
    newTree->left = NULL;
    newTree->right = NULL;

    huffman_tree_list_add(list, newTree);
  }

  return list;
}

/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {
  if (l == NULL) {
    perror("list_reduce");
    exit(EXIT_FAILURE);
  }

  while (l->next != NULL) {
    huffman_tree_t *firstT = malloc(sizeof(*firstT));
    firstT = l->tree;
    huffman_tree_t *secondT = malloc(sizeof(*secondT));
    secondT = l->next->tree;
    huffman_tree_list_free(l);
    huffman_tree_list_free(l);

    huffman_tree_t *newTree = malloc(sizeof(*newTree));
    newTree->count = firstT->count + secondT->count;
    newTree->left = firstT;
    newTree->right = secondT;

    huffman_tree_list_add(l, newTree);
  }
  return l;
}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {
  return NULL;
}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  return NULL;
}
//
//
//
// int main(void) {
//   printf("contains test: %d\n", contains("hello", 'f'));
//   printf("freq test: %d\n", frequency("banana", 't'));
//   char *nubb = malloc(MAX_STRING_LENGTH-1);
//   memcpy(nubb, nub("mississiprerepi"), 10);
//   printf("nub test: %s\n", nubb);
//
// }
