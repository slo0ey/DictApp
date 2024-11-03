#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include "dict.h"
#include "dictloader.h"

#define UI_WIDTH 64
#define UI_CHAR "#"

#define SEARCH         1
#define ADD            2
#define DELETE         3
#define LEVEL_ORDER    4
#define TERMINAL_NODES 5
#define CLEAN          6
#define EXIT           7

using namespace std;

void run_app(); //시작 지점
void on_start();
void on_end();
void print_menu();
void input_line(const char* title, char* buffer, int size);

void search_word();
void add_new_word();
void delete_word();
void view_level_order();
void view_all_terminal_nodes();
void clear_history();

DictNode* dictionary[46];

int main(int argc, char* argv[]) {
  run_app();

  return 0;
}

void run_app() {
  on_start();

  int menu = -1;
  print_menu();
  while(menu != EXIT) {
    printf("Choice: ");
    scanf("%d", &menu);
    printf("\n");
    getchar(); // 입력버퍼에서 개행문자 날림

    switch(menu) {
      case SEARCH:
        search_word();
        break;
      case ADD:
        add_new_word();
        break;
      case DELETE:
        delete_word();
        break;
      case LEVEL_ORDER:
        view_level_order();
        break;
      case TERMINAL_NODES:
        view_all_terminal_nodes();
        break;
      case CLEAN:
        clear_history();
        break;
      case EXIT:
        break;
    }
  }

  on_end();
}

void on_start() {
  load(dictionary, "engkor.dat");
  system("clear");
}

void on_end() {
  save(dictionary, "engkor.dat");
}

void print_menu() {
  printf("########################################################\n");
  printf("#                                                      #\n");
  printf("#                      DictApp v1                      #\n");
  printf("#                   (AVL Tree based)                   #\n");
  printf("#                                                      #\n");
  printf("########################################################\n");
  printf("#                                                      #\n");
  printf("#  1. Search Word                                      #\n");
  printf("#  2. Add New Word                                     #\n");
  printf("#  3. Delete Word                                      #\n");
  printf("#  4. View Level Order                                 #\n");
  printf("#  5. View All Terminal Nodes                          #\n");
  printf("#  6. Clear History                                    #\n");
  printf("#  7. Exit                                             #\n");
  printf("#                                                      #\n");
  printf("########################################################\n\n");
}

void input_line(const char* title, char* buffer, int size) {
  printf("%s", title);
  fgets(buffer, size, stdin);
  buffer[strlen(buffer)-1] = '\0';
}

void search_word() {
  char query[46];
  input_line("Word: ", query, 46);
  printf("\n");

  int len = strlen(query);
  Word* word = dict_search(dictionary[len], query);

  if(word == NULL) {
    printf("> 없는 단어입니다.\n\n");
  } else{
    printf("> 단어: %s\n", word->word);
    printf("> 뜻:   %s\n\n", word->meaning);
  }
}

void add_new_word() {
  char word[46];
  char meaning[100];

  input_line("Word: ", word, 46);
  input_line("Meaning: ", meaning, 100);

  int len = strlen(word);
  dictionary[len] = dict_insert(dictionary[len], new_word(word, meaning));

  printf("\n");
  printf("> 새로운 단어 \"%s\" 이(가) 추가되었습니다.\n\n", word);
}

void delete_word() {
  char word[46];
  input_line("Word: ", word, 46);

  int len = strlen(word);
  dictionary[len] = dict_remove(dictionary[len], word);

  printf("\n");
  printf("> 단어 \"%s\" 이(가) 삭제되었습니다.\n\n", word);
}

void level_order(DictNode* root) {
  if(root == NULL) {
    printf("> EMPTY\n");
    return;
  }

  queue<DictNode*> q;
  q.push(root);

  while(!q.empty()) {
    int len = q.size();
    printf("> ");
    while(len--) {
      DictNode* node = q.front(); q.pop();
      printf("%s//%d  ", node->wordRef->word, node->height);

      if(node->left != NULL)
        q.push(node->left);
      if(node->right != NULL)
        q.push(node->right);
    }
    printf("\n");
  }
}

void view_level_order() {
  int size;
  printf("Target Word Size: ");
  scanf("%d", &size);
  printf("\n");

  level_order(dictionary[size]);
  printf("\n");
}

void inorder(DictNode* root) {
  if(root->left != NULL)
    inorder(root->left);
  if(root->height == 1)
    printf("> %s\n", root->wordRef->word);
  if(root->right != NULL)
    inorder(root->right);
}

void view_all_terminal_nodes() {
  int size;
  printf("Target Word Size: ");
  scanf("%d", &size);
  printf("\n");

  printf("> Terminal Nodes:\n");
  inorder(dictionary[size]);
  printf("\n");
}

void clear_history() {
  system("clear");
  print_menu();
}
