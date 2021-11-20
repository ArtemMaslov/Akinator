#ifndef AKINATOR_H_
#define AKINATOR_H_


#include "..\StringLibrary\StringLibrary.h"
#include "Tree.h"

struct Akinator
{
    Text  text;
    Tree  tree;
    FILE* file;
};


Akinator AkinatorConstructor(FILE* file);

void AkinatorDestructor(Akinator* akinator);

void WriteTreeToFile(Akinator* akinator, FILE* file);

bool CreateGraph(Akinator* akinator, const char* outFileName);


#endif