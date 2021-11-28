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
    Node* node = TreeFindObject(akinator->tree.root, *str);

    if (!node)
    {
        puts("Строка не найдена в дереве");
        return;
    }

    PrintNodeValue(nullptr, &node->value, nullptr, stdout);

    while (node->parent && node->parent->parent)
    {
        if (node->parent->nodeRight == node)
        {
            PrintNodeValue(" ", &node->parent->value, node->parent->parent? ", " : " и ", stdout);
        }
        else
        {
            PrintNodeValue(" не ", &node->parent->value, node->parent->parent? ", " : " и ", stdout);
        }

        node = node->parent;
    }
    
    if (node->parent)
    {
        if (node->parent->nodeRight == node)
        {
            PrintNodeValue(" ", &node->parent->value, ".\n", stdout);
        }
        else
        {
            PrintNodeValue(" не ", &node->parent->value, ".\n", stdout);
        }
    }
    else
        PrintNodeValue(". Ни да, ни нет. Просто ", &node->value, ".\n", stdout);
}

void CompareObjects(Akinator* akinator, String str1, String str2)
{
    Stack stk1 = {};
    Stack stk2 = {};

    StackConstructor(&stk1, sizeof(Node*), akinator->tree.treeLength);
    StackConstructor(&stk2, sizeof(Node*), akinator->tree.treeLength);
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, str1, &stk1);
    Node* n2 = TreeFindObjectStack(akinator->tree.root, str2, &stk2);

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
    size_t index = 0;

    PrintNodeValue(nullptr, &str1, nullptr, stdout);
    PrintNodeValue(" и ", &str2, nullptr, stdout);

    Node* parent = GetNodeFromStack(&stk1, index);
    index++;
    Node* child1 = GetNodeFromStack(&stk1, index);
    Node* child2 = GetNodeFromStack(&stk2, index);

    if (child1 == child2)
    {
        puts(" похожи тем, что они ");
        size_t sameCount = 0;
        while (child1 && child1 == child2)
        {
            sameCount++;
        }

        while (child1 && child1 == child2)
        {
            PrintNodeValue(parent->nodeLeft == child1->parent? "не " : "", &parent->value, " и ", stdout);

            index++;
            parent = child1;
            child1 = GetNodeFromStack(&stk1, index);
            child2 = GetNodeFromStack(&stk2, index);
            if (child1 == child2)
                puts("\b\b\b, ");
        }
    }
    
    PrintNodeValue(parent->nodeLeft == child1->parent? "не " : "", &parent->value, ".\n", stdout);

    PrintNodeValue(nullptr, &str1, nullptr, stdout);
    PrintNodeValue(" и ", &str2, " различаются тем, что ", stdout);

    PrintNodeValue(nullptr, &str1, nullptr, stdout);

    parent = child1->parent;
    while (index < stk1.stackSize)
    {
        PrintNodeValue(parent->nodeLeft == child1->parent? "не " : "", &child1->value, ", ", stdout);
        
        parent = child1;
        index++;
        child1 = GetNodeFromStack(&stk1, index);
        
        if (index == stk1.stackSize)
            puts("\b\b и");
    }

    PrintNodeValue(parent->nodeLeft == child1->parent? "не " : "", &child1->value, ", а ", stdout);
    
    PrintNodeValue(nullptr, &str2, nullptr, stdout);
    
    index = minIndex;
    parent = child2->parent;
    while (index < stk2.stackSize)
    {
        PrintNodeValue(parent->nodeLeft == child2->parent? "не " : "", &child2->value, ", ", stdout);
        
        parent = child2;
        index++;
        child2 = GetNodeFromStack(&stk2, index);
        
        if (index == stk2.stackSize)
            puts("\b\b и ");
    }
    PrintNodeValue(nullptr, &child2->value, ".\n", stdout);
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

                CompareObjects(akinator, str1, str2);

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