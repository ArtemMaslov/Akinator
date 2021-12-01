#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "Tree.h"
#include "Akinator.h"
#include "FileIO.h"
#include "Voice.h"


#include "..\Logs\Logs.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static void PrintNodeToFile(Node* node, FILE* file);


void ReadString(String* str, size_t bufferSize, const char* msg)
{
    LogLine("Вызван ReadString()", LOG_DEBUG);
    assert(str);
    //assert(msg);

    char* res = nullptr;
    while (res == nullptr)
    {
        if (msg)
            puts(msg);
        res = fgets(str->ptr, bufferSize, stdin);
    }

    res = strchr(str->ptr, '\n');
    if (res)
    {
        *res = '\0';
        str->length = res - str->ptr;
    }
    else
    {
        while (getchar() != '\n')
            ;
    }
}

bool ReadQuestionAnswer(char* buffer, size_t bufferSize)
{
    LogLine("Вызван ReadQuestionAnswer()", LOG_DEBUG);
    assert(buffer);

    char* res = fgets(buffer, bufferSize, stdin);

    do
    {
        while (res == nullptr)
        {
            puts("Ответом на вопрос может быть только: да, нет.");
            res = fgets(buffer, bufferSize, stdin);
        }

        res = strchr(buffer, '\n');
        if (res)
        {
            *res = '\0';
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        if (strncmp(buffer, "да", bufferSize)  == 0)
            return YES;
        if (strncmp(buffer, "нет", bufferSize) == 0)
            return NO;
        res = nullptr;
    }
    while (res == nullptr)
        ;

    return NO;
}

char* ReadLine(char* buffer, size_t bufferSize)
{
    LogLine("Вызван ReadLine()", LOG_DEBUG);
    assert(buffer);

    char* res = fgets(buffer, bufferSize, stdin);

    char* nl = strchr(buffer, '\n');
    if (nl)
    {
        *nl = '\0';
    }
    else
    {
        while (getchar() != '\n')
            ;
    }

    return res;
}

void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child)
{
    LogLine("Вызван PrintObjectInformation()", LOG_DEBUG);
    assert(stk);
    assert(child);

    for (size_t index = indexFrom; index < indexTo - 1; index++)
    {
        Node* next =  GetNodeFromStack(stk, index + 1);

        if (indexTo - index > 3)
        {
            PRINT_NODE_AND_SPEAK(child->nodeLeft == next? "не " : "", &child->value, ", ");
        }
        else if (indexTo - index == 3)
        {
            PRINT_NODE_AND_SPEAK(child->nodeLeft == next? "не " : "", &child->value, " и ");
        }
        else
        {
            PRINT_NODE_AND_SPEAK(child->nodeLeft == next? "не " : "", &child->value, nullptr);
        }
    
        child = next;
    }
}

void WriteTreeToFile(Akinator* akinator, FILE* file)
{
    LogLine("Вызван WriteTreeToFile()", LOG_DEBUG);
    assert(akinator);
    assert(file);

    PrintNodeToFile(akinator->tree.root, file);
}

static void PrintNodeToFile(Node* node, FILE* file)
{
    LogLine("Вызван PrintNodeToFile()", LOG_DEBUG);
    assert(node);
    assert(file);
    
    fputc('{', file);

    if (node->value.ptr)
    {
        size_t strLen = node->value.length;
        char*  ptr    = node->value.ptr;
        for (size_t st = 0; st < strLen; st++)
            fputc(ptr[st], file);
    }

    if (node->nodeRight)
        PrintNodeToFile(node->nodeRight, file);

    if (node->nodeLeft)
        PrintNodeToFile(node->nodeLeft, file);
    
    fputc('}', file);
}

void PrintNodeValue(const char* prefix, String* str, const char* postfix, FILE* file)
{
    LogLine("Вызван PrintNodeValue()", LOG_DEBUG);
    //assert(prefix);
    assert(str);
    //assert(postfix);
    assert(file);

    if (prefix)
        fputs(prefix, file);

    fprintf(file, "%.*s",  str->length, str->ptr);
    
    if (postfix)
        fputs(postfix, file);
}
