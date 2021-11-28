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

bool ShowMenu(Akinator* akinator);

void PlayAkinator(Akinator* akinator);

void GetObjectDefinition(Akinator* akinator, String* str);

void CompareObjects(Akinator* akinator, String* str1, String* str2);


#endif