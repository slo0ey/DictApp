#ifndef DICTLOADER_H
#define DICTLOADER_H

#include <stdio.h>
#include "dict.h"

void load(DictNode** dictionary, const char* filename);

void save(DictNode** dictionary, const char* filename);

#endif
