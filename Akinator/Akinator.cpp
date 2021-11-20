#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "Tree.h"
#include "Akinator.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static bool ParseText(Akinator* akinator);

static void PrintNode(Node* node, FILE* file);


Akinator AkinatorConstructor(FILE* file)
{
    assert(file);

    Akinator akinator = {};
    akinator.file = file;
    bool result = false;

    result = ReadFile(&akinator.text, file);

    if (result)
        result = ParseText(&akinator);

    if (result)
        result = CreateGraph(&akinator, "D:\\Язык C\\Akinator\\Graph.png");

    FILE* out_file = fopen("D:\\Язык C\\Akinator\\out.txt", "w");

    WriteTreeToFile(&akinator, out_file);

    if (!result)
    {
        AkinatorDestructor(&akinator);
    }

    return akinator;
}

void AkinatorDestructor(Akinator* akinator)
{
    assert(akinator);

    TextDestructor(&akinator->text);

    TreeDestructor(&akinator->tree);
}

static bool ParseText(Akinator* akinator)
{
    assert(akinator);

    char* ptr = akinator->text.buffer;
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

    ptr = akinator->text.buffer;

    while (*ptr)
    {
        if (*ptr == '{')
        {
            Node* node = TreeNodeConstructor(nullptr);

            if (!node)
                return false;
            
            char* strStart = nullptr;

            ptr++;
            while (*ptr != '}' && *ptr != '{')
            {
                if (!strStart && !isspace(*ptr))
                    strStart = ptr;
                ptr++;
            }
            ptr--;

            if (strStart)
            {
                String string =
                {
                    strStart,
                    ptr - strStart + 1
                };
                node->value = string;
                strStart = nullptr;
            }
            StackPush(&stk, &node);
        }
        else if (*ptr == '}')
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
            {
                parentNode->nodeRight = childNode;
            }
            else
            {
                parentNode->nodeLeft = childNode;
            }
            StackPush(&stk, &parentNode);
        }
        ptr++;
    }

    return true;
}

bool CreateGraph(Akinator* akinator, const char* outFileName)
{
    return CreateTreeGraph(outFileName, &akinator->tree);
}

void WriteTreeToFile(Akinator* akinator, FILE* file)
{
    assert(akinator);

    PrintNode(akinator->tree.root, file);
}

static void PrintNode(Node* node, FILE* file)
{
    assert(node);

    if (!node)
        return;
    
    fputc('{', file);

    if (node->value.ptr)
    {
        size_t strLen = node->value.length;
        char*  ptr    = node->value.ptr;
        for (size_t st = 0; st < strLen; st++)
            fputc(ptr[st], file);
    }

    if (node->nodeRight)
        PrintNode(node->nodeRight, file);

    if (node->nodeLeft)
        PrintNode(node->nodeLeft, file);
    
    fputc('}', file);
}

void PlayAkinator(Akinator* akinator)
{

}

void GetObjectDefinition(Akinator* akinator, const char* str)
{
    String string = {};
    string.ptr = (char*)str;

    Node* node = TreeFindObject(akinator->tree.root, string);

    if (!node)
    {
        puts("Строка не найдена в дереве");
        return;
    }

    if (node->nodeLeft || node->nodeRight)
    {
        puts("Строка является вопросом, а не объектом.");
        return;
    }

    printf("%s ", node->value.ptr);

    while (node->parent && node->parent->parent)
    {
        if (node->parent->nodeRight == node)
            printf("%s%s", node->parent->value.ptr, node->parent->parent? ", " : " и ");
        else
            printf("не %s%s", node->parent->value.ptr, node->parent->parent? ", " : " и ");

        node = node->parent;
    }
    
    if (node->parent)
    {
        if (node->parent->nodeRight == node)
            printf("%s.", node->parent->value.ptr);
        else
            printf("не %s.", node->parent->value.ptr);
    }
    else
        puts("Ошибка");
}

void CompareObjects(Akinator* akinator, const char* str1, const char* str2)
{
    Stack stk1 = {};
    Stack stk2 = {};
    String s1 = {};
    String s2 = {};

    s1.ptr = (char*)str1;
    s2.ptr = (char*)str2;
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, s1, &stk1);
    Node* n2 = TreeFindObjectStack(akinator->tree.root, s2, &stk2);

    if (!n1)
    {
        puts("Строка не найдена в дереве");
        return;
    }
    
    if (!n2)
    {
        puts("Строка не найдена в дереве");
        return;
    }
    
    if (n1->nodeLeft || n1->nodeRight)
    {
        puts("Строка является вопросом, а не объектом.");
        return;
    }
    
    if (n2->nodeLeft || n2->nodeRight)
    {
        puts("Строка является вопросом, а не объектом.");
        return;
    }


}