#ifndef FILE_IO_H_
#define FILE_IO_H_


#include "Tree.h"
#include "Akinator.h"
#include "..\StringLibrary\StringLibrary.h"


void ReadString(String* str, size_t bufferSize, const char* msg);

char* ReadLine(char* buffer, size_t bufferSize);

bool ReadQuestionAnswer(char* buffer, size_t bufferSize);

void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child);

void PrintNodeValue(const char* prefix, String* str, const char* postfix, FILE* file);

void WriteTreeToFile(Akinator* akinator, FILE* file);


#endif