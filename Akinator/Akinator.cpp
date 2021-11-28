#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "Tree.h"
#include "Akinator.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


const char* menuItems[] =
{
    "Играть",
    "Определить",
    "Сравнить",
    "Выход",
};

const char menuLetters[]
{
    'a',
    'b',
    'c',
    'd'
};

enum menuActions
{
    PLAY_AKINATOR = 'a',
    DEF_OBJECTS   = 'b',
    CMP_OBJECTS   = 'c',
    QUIT          = 'd'
};


static bool ParseText(Akinator* akinator);

static void PrintNodeToFile(Node* node, FILE* file);

static void ReadLine(String* str, size_t bufferSize, const char* msg);

static void ChoseAction();

static void PrintNodeValue(const char* prefix, String* str, const char* postfix, FILE* file);

static void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child);


Akinator AkinatorConstructor(FILE* file)
{
    assert(file);

    Akinator akinator = {};
    akinator.file = file;
    bool result = false;

    result = ReadFile(&akinator.text, file);

    if (result)
    {
        result = ParseText(&akinator);
        TreeMeasure(&akinator.tree, akinator.tree.root, 0);
    }

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
                    (size_t)(ptr - strStart + 1)
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
                TreeAddRightNode(parentNode, childNode);
            else
                TreeAddLeftNode(parentNode, childNode);

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

    PrintNodeToFile(akinator->tree.root, file);
}

static void PrintNodeToFile(Node* node, FILE* file)
{
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

static void PrintNodeValue(const char* prefix, String* str, const char* postfix, FILE* file)
{
    assert(str);
    assert(file);

    if (prefix)
        fputs(prefix, file);

    size_t strLen = str->length;
    char*  ptr    = str->ptr;
    for (size_t st = 0; st < strLen; st++)
        fputc(ptr[st], file);

    if (postfix)
        fputs(postfix, file);
}

void GetObjectDefinition(Akinator* akinator, String* str)
{
    Stack stk = {};

    StackConstructor(&stk, sizeof(Node*), akinator->tree.treeLength);
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, *str, &stk);
    
    if (!n1)
    {
        puts("Строка не найдена в дереве");
        return;
    }

    PrintNodeValue(nullptr, str, " - ", stdout);

    PrintObjectInformation(0, stk.stackSize, &stk, akinator->tree.root);

    puts(".");
}

void CompareObjects(Akinator* akinator, String* str1, String* str2)
{
    Stack stk1 = {};
    Stack stk2 = {};

    StackConstructor(&stk1, sizeof(Node*), akinator->tree.treeLength);
    StackConstructor(&stk2, sizeof(Node*), akinator->tree.treeLength);
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, *str1, &stk1);
    Node* n2 = TreeFindObjectStack(akinator->tree.root, *str2, &stk2);

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

    PrintNodeValue(nullptr, str1, nullptr, stdout);
    PrintNodeValue(" и ", str2, nullptr, stdout);

    Node* child1 = GetNodeFromStack(&stk1, 0);
    Node* child2 = GetNodeFromStack(&stk2, 0);
    
    size_t sameCount = 0;
    size_t index = 0;

    while (index < stk1.stackSize && index < stk2.stackSize && child1 == child2)
    {
        sameCount++;
        index++;
        child1 = GetNodeFromStack(&stk1, index);
        child2 = GetNodeFromStack(&stk2, index);
    }

    index = 0;
    if (index < sameCount)
    {
        fputs(" похожи тем, что они ", stdout);
        
        child1 = GetNodeFromStack(&stk1, 0);

        PrintObjectInformation(0, sameCount, &stk1, child1);
        puts(".");
    }
    else
        fputs(" ничем не похожи.\n", stdout);
    
    child1 = GetNodeFromStack(&stk1, sameCount);
    child2 = GetNodeFromStack(&stk2, sameCount);
        
    PrintNodeValue(nullptr, str1, nullptr, stdout);
    PrintNodeValue(" и ", str2, nullptr, stdout);

    if (stk1.stackSize - sameCount - 1 > 0 || stk2.stackSize - sameCount - 1 > 0)
    {
        fputs(" различаются тем, что ", stdout);

        PrintNodeValue(nullptr, str1, " - ", stdout);
    
        PrintObjectInformation(sameCount, stk1.stackSize, &stk1, child1);

        PrintNodeValue(", а ", str2, " ", stdout);
    
        PrintObjectInformation(sameCount, stk2.stackSize, &stk2, child2);
        puts(".");
    }
    else
        fputs(" абсолютно похожи.\n", stdout);
}

static void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child)
{
    for (size_t index = indexFrom; index < indexTo - 1; index++)
    {
        Node* next =  GetNodeFromStack(stk, index + 1);

        if (indexTo - index > 3)
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, ", ", stdout);
        }
        else if (indexTo - index == 3)
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, " и ", stdout);
        }
        else
        {
            PrintNodeValue(child->nodeLeft == next? "не " : "", &child->value, nullptr, stdout);
        }
    
        child = next;
    }
}

void PlayAkinator(Akinator* akinator)
{
}

bool ShowMenu(Akinator* akinator)
{
    puts("Добро пожаловать в игру Акинатор!");

    char c = '\0';

    while (true)
    {
        ChoseAction();
        scanf("%c", &c);

        while (getchar() != '\n')
            break;

        switch (c)
        {
            case menuActions::PLAY_AKINATOR:
            {
                PlayAkinator(akinator);
                break;
            }
            case menuActions::CMP_OBJECTS:
            {
                const size_t bufferSize = 100;
                String str1 = {};
                String str2 = {};

                str1.ptr = (char*)calloc(bufferSize, sizeof(char));
                str2.ptr = (char*)calloc(bufferSize, sizeof(char));

                if (!str1.ptr || !str2.ptr)
                {
                    puts("Нехватает памяти");
                    return false;
                }

                ReadLine(&str1, bufferSize, "Введите первый сравниваемый объект:");
                ReadLine(&str2, bufferSize, "Введите второй сравниваемый объект:");

                CompareObjects(akinator, &str1, &str2);

                free(str1.ptr);
                free(str2.ptr);
                break;
            }
            case menuActions::DEF_OBJECTS:
            {
                const size_t bufferSize = 100;
                String str = {};

                str.ptr = (char*)calloc(bufferSize, sizeof(char));

                if (!str.ptr)
                {
                    puts("Нехватает памяти");
                    return false;
                }

                ReadLine(&str, bufferSize, "Про кого вы хотите узнать все подробности:");

                GetObjectDefinition(akinator, &str);

                free(str.ptr);
                break;
            }
            case menuActions::QUIT:
            {
                puts("Работа программы завершена :(");
                return true;
            }
            default:
            {
                puts("Ошибка ввода.");
                break;
            }
        }
    }

    return false;
}

static void ReadLine(String* str, size_t bufferSize, const char* msg)
{
    assert(str);
    assert(msg);

    char* res = nullptr;
    while (res == nullptr)
    {
        puts(msg);
        res = fgets(str->ptr, bufferSize, stdin);
    }

    res = strchr(str->ptr, '\n');
    if (res)
    {
        *res = '\0';
        str->length = res - str->ptr;
    }
}

static void ChoseAction()
{
    puts("Выберите действие:");

    for (int st = 0; st < sizeof(menuItems) / sizeof(menuItems[0]); st++)
        printf("%c) %-20s%c", menuLetters[st], menuItems[st], (st % 2 == 1)? '\n': ' ');
}