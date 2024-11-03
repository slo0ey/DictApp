#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "dict.h"

uint8_t max(uint8_t a, uint8_t b) {
  return a > b ? a : b;
}

// AVL Tree features

uint8_t is_leaf(DictNode* root);
uint8_t get_height(DictNode* root);
int8_t get_balance_factor(DictNode* root);
DictNode* get_successor_node(DictNode* root);
DictNode* rotate_ll(DictNode* root);
DictNode* rotate_rr(DictNode* root);
DictNode* rotate_lr(DictNode* root);
DictNode* rotate_rl(DictNode* root);
DictNode* rebalance(DictNode* root);
void swap_node(DictNode* n1, DictNode* n2);
void free_node(DictNode* root);

Word* new_word(char* word, char* meaning) {
  Word* wordRef = (Word*) malloc(sizeof(Word));
  wordRef->word = (char*) malloc(sizeof(char) * strlen(word));
  wordRef->meaning = (char*) malloc(sizeof(char) * strlen(meaning));

  strcpy(wordRef->word, word);
  strcpy(wordRef->meaning, meaning);
  return wordRef;
}

DictNode* new_dictnode(Word* word) {
  DictNode* node = (DictNode*) malloc(sizeof(DictNode));
  node->height = 1;
  node->wordRef = word;
  node->left = NULL;
  node->right = NULL;
  return node;
}

Word* dict_search(DictNode* root, const char* word) {
  if(root == NULL)
    return NULL;
  
  const char* current = root->wordRef->word;
  int cmp = strcmp(word, current);

  if(cmp < 0)
    return dict_search(root->left, word);
  else if(cmp > 0)
    return dict_search(root->right, word);
  else
    return root->wordRef;
}

DictNode* dict_insert(DictNode* root, Word* word) {
  if(root == NULL) {
    return new_dictnode(word);
  }

  const char* current = root->wordRef->word;
  const char* target = word->word;

  if(strcmp(target, current) < 0) {
    root->left = dict_insert(root->left, word);
  } else {
    root->right = dict_insert(root->right, word);
  }
  root->height = 1 + max(get_height(root->left), get_height(root->right));
  return rebalance(root);
}

DictNode* dict_remove(DictNode* root, const char* word) {
  if(root == NULL)
    return NULL;

  const char* current = root->wordRef->word;
  int cmp = strcmp(word, current);

  if(cmp < 0) {
    root->left = dict_remove(root->left, word);
  } else if(cmp > 0) {
    root->right = dict_remove(root->right, word);
  } else {
    if(is_leaf(root)) {
      free_node(root);
      return NULL;
    }

    DictNode* new_root;
    if(root->left == NULL) {
      new_root = root->right;
      free_node(root);
    } else if(root->right == NULL) {
      new_root = root->left;
      free_node(root);
    } else {
      new_root = root;
      swap_node(root, get_successor_node(root->right));
      root->right = dict_remove(root->right, word);
    }

    return new_root;
  }

  root = rebalance(root);
  root->height = 1+max(get_height(root->left), get_height(root->right));
  return root;
}

uint8_t is_leaf(DictNode* root) {
  return root->left == NULL && root->right == NULL;
}

uint8_t get_height(DictNode* root) {
  if(root == NULL)
    return 0;
  return root->height;
}

int8_t get_balance_factor(DictNode* root) {
  return get_height(root->left) - get_height(root->right);
}

DictNode* get_successor_node(DictNode* root) {
  if(is_leaf(root))
    return root;

  if(root->left != NULL)
    return get_successor_node(root->left);
  else
    return get_successor_node(root->right);
}

DictNode* rotate_ll(DictNode* root) {
  DictNode* child = root->left;
  root->left = child->right;
  child->right = root;

  root->height = 1+max(get_height(root->left), get_height(root->right));
  child->height = 1+max(get_height(child->left), get_height(child->right));
  return child;
}

DictNode* rotate_rr(DictNode* root) {
  DictNode* child = root->right;
  root->right = child->left;
  child->left = root;

  root->height = 1+max(get_height(root->left), get_height(root->right));
  child->height = 1+max(get_height(child->left), get_height(child->right));
  return child;
}

DictNode* rotate_lr(DictNode* root) {
  DictNode* child = root->left;
  root->left = rotate_rr(child);
  return rotate_ll(root);
}

DictNode* rotate_rl(DictNode* root) {
  DictNode* child = root->right;
  root->right = rotate_ll(child);
  return rotate_rr(root);
}

DictNode* rebalance(DictNode* root) {
  int8_t bf = get_balance_factor(root);

  if(bf > 1) {
    if(get_balance_factor(root->left) > 0)
      return rotate_ll(root);
    else
      return rotate_lr(root);
  } else if(bf < -1) {
    if(get_balance_factor(root->right) < 0)
      return rotate_rr(root);
    else
      return rotate_rl(root);
  }
  return root;
}

void swap_node(DictNode* n1, DictNode* n2) {
  Word* temp = n1->wordRef;
  n1->wordRef = n2->wordRef;
  n2->wordRef = temp;
}

void free_node(DictNode* root) {
  Word* wordRef = root->wordRef;
  free(wordRef->word);
  free(wordRef->meaning);
  free(wordRef);
  free(root);
}
