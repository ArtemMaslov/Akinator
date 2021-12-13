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


static void PrintNodeToFile(Node* node, FILE* file, size_t recursiveLength);


void ReadString(String* str, size_t bufferSize, const char* msg)
{
    LogLine("Вызван ReadString()", LOG_DEBUG);
    assert(str);
    //assert(msg);
    str->length = 0;

    while (str->length == 0)
    {    
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
        if (str->length == 0)
            puts("Строка не может быть пустой:");
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

void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child, VoiceBuffer* buffer)
{
    LogLine("Вызван PrintObjectInformation()", LOG_DEBUG);
    assert(stk);
    assert(child);

    for (size_t index = indexFrom; index < indexTo - 1; index++)
    {
        Node* next =  GetNodeFromStack(stk, index + 1);

        if (indexTo - index > 3)
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, ", ", buffer, stdout);
        }
        else if (indexTo - index == 3)
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, " и ", buffer, stdout);
        }
        else
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, nullptr, buffer, stdout);
        }
    
        child = next;
    }
}

void WriteTreeToFile(Akinator* akinator, FILE* file)
{
    LogLine("Вызван WriteTreeToFile()", LOG_DEBUG);
    assert(akinator);
    assert(file);

    PrintNodeToFile(akinator->tree.root, file, 0);
}

static void PrintNodeToFile(Node* node, FILE* file, size_t recursiveLength)
{
    LogLine("Вызван PrintNodeToFile()", LOG_DEBUG);
    assert(node);
    assert(file);
    
    fprintf(file, "%*s{\n", recursiveLength * 4, "");

    if (node->value.ptr)
        fprintf(file, "%*s%.*s\n", (recursiveLength + 1) * 4, "", node->value.length, node->value.ptr);

    if (node->nodeRight)
        PrintNodeToFile(node->nodeRight, file, recursiveLength + 1);

    if (node->nodeLeft)
        PrintNodeToFile(node->nodeLeft, file, recursiveLength + 1);
    
    fprintf(file, "%*s}\n", recursiveLength * 4, "");
}

void PrintNodeValue(const char* prefix, String* str, const char* postfix, VoiceBuffer* voiceBuffer, FILE* file)
{
    LogLine("Вызван PrintNodeValue()", LOG_DEBUG);
    //assert(prefix);
    assert(str);
    //assert(voiceBuffer);
    //assert(postfix);
    assert(file);

    if (prefix)
        fputs(prefix, file);

    fprintf(file, "%.*s",  str->length, str->ptr);
    
    if (postfix)
        fputs(postfix, file);

    if (voiceBuffer)
    {
        if (prefix)
            AddStringToBuffer(voiceBuffer, prefix, -1);
        
        AddStringToBuffer(voiceBuffer, str->ptr, str->length);

        if (postfix)
            AddStringToBuffer(voiceBuffer, postfix, -1);
    }
}

bool ParseText(Akinator* akinator)
{
    LogLine("Вызван ParseText()", LOG_DEBUG);

    assert(akinator);

    unsigned char* ptr = (unsigned char*)akinator->text.buffer;

    size_t lBracketsCount = 0;
    size_t rBracketsCount = 0;
    size_t openedBrackets = 0;

    while (*ptr)
    {
        if (*ptr == '{')
            lBracketsCount++;
        else if (*ptr == '}')
            rBracketsCount++;

        if (lBracketsCount < rBracketsCount)
        {
            puts("Ошибочная скобочная последовательность.\n"
                 "Количество закрывающих скобок '{' больше количества открывающих '}'");
            return false;
        }

        ptr++;
    }
    
    Stack stk = {};
    StackConstructor(&stk, sizeof(Node*), lBracketsCount);

    ptr = (unsigned char*)akinator->text.buffer;

    while (*ptr)
    {
        if (*ptr == '{')
        {
            Node* node = TreeNodeConstructor(nullptr);

            if (!node)
                return false;
            
            do
            {
                ptr++;
            }
            while (isspace(*ptr));
            
            unsigned char* strStart = ptr;

            while (*ptr != '}' && *ptr != '{')
                ptr++;
            ptr--;

            String string =
            {
                (char*)strStart,
                (size_t)(ptr - strStart + 1)
            };

            while (string.length > 0 && isspace((unsigned char)string.ptr[string.length - 1]))
                string.length--;

            node->value = string;

            StackPush(&stk, &node);
        }
        if (*ptr == '}')
        {
            Node** _childNode  = (Node**)StackPop(&stk);
            Node** _parentNode = (Node**)StackPop(&stk);

            if (!_childNode)
                return false;

            if (!_parentNode) // Корень дерева
            {
                akinator->tree.root = *_childNode;
                break;
            }

            Node* childNode  = *_childNode;
            Node* parentNode = *_parentNode;

            if (parentNode->nodeRight == nullptr)
                TreeAddRightNode(parentNode, childNode);
            else
                TreeAddLeftNode(parentNode, childNode);

            StackPush(&stk, &parentNode);
        }
        ptr++;
    }

    return true;
}
