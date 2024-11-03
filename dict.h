#ifndef DICT_H
#define DICT_H

#include <stdint.h>

typedef struct Word {
  char* word;
  char* meaning;
} Word;

typedef struct DictNode {
  uint8_t height;
  Word* wordRef;
  DictNode* left;
  DictNode* right;
} DictNode;

Word* new_word(char* word, char* meaning);

DictNode* new_dictnode(Word* word);

Word* dict_search(DictNode* root, const char* word);

DictNode* dict_insert(DictNode* root, Word* word);

DictNode* dict_remove(DictNode* root, const char* word);

#endif
