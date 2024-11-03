#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include "dict.h"
#include "dictloader.h"

#define END   0
#define START 1

using namespace std;

void level_order(DictNode* root, FILE* f);

void load(DictNode** dictionary, const char* filename) {
  FILE* f = fopen(filename, "r");
  char line[100];
  char line2[100];
  int flag = END;
  int word_size;

  while(NULL != fgets(line, 100, f)) {
    if(line[0] == '#') {
      if(flag == END) {
        flag = START;
        fscanf(f, "%d\n", &word_size);
      } else {
        flag = END;
      }
    } else {
      fgets(line2, 100, f);
      line[word_size] = '\0';
      line2[strlen(line2) - 1] = '\0';
      dictionary[word_size] = dict_insert(dictionary[word_size], new_word(line, line2));
    }
  }

  fclose(f);
}

void save(DictNode** dictionary, const char* filename) {
  FILE* f = fopen(filename, "w");
  for(int word_size = 1; word_size < 46; word_size++) {
    if(dictionary[word_size] == NULL)
      continue;

    fputs("#\n", f);
    fprintf(f, "%d\n", word_size);
    level_order(dictionary[word_size], f);
    fputs("#\n", f);
  } 

  fclose(f);
}

void level_order(DictNode* root, FILE* f) {
  queue<DictNode*> q;
  q.push(root);

  while(!q.empty()) {
    DictNode* node = q.front(); q.pop();
    const char* word = node->wordRef->word;
    const char* meaning = node->wordRef->meaning;
    fputs(word, f);
    fputs("\n", f);
    fputs(meaning, f);
    fputs("\n", f);

    if(node->left != NULL)
      q.push(node->left);
    if(node->right != NULL)
      q.push(node->right);
  }
}
