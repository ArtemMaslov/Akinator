#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "Config.h"
#include "FileIO.h"
#include "Tree.h"
#include "Akinator.h"
#include "Utilities.h"


#include "..\Logs\Logs.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static const char* menuItems[] =
{
    "Играть",
    "Определить",
    "Сравнить",
    nullptr,
    "Считать из файла",
    "Записать в файл",
#ifdef GRAPHVIZ
    "Построить граф",
#endif
    nullptr,
    nullptr,
    "Выход",
};

static const char menuLetters[]
{
    'и',
    'о',
    'с',
    '\0',
    'ч',
    'з',
#ifdef GRAPHVIZ
    'г',
#endif
    '\0',
    '\0',
    'в'
};

enum menuActions
{
    PLAY_AKINATOR = 'и',
    DEF_OBJECTS   = 'о',
    CMP_OBJECTS   = 'с',
    QUIT          = 'в',
    READ_FILE     = 'ч',
    WRITE_TO_FILE = 'з',
    BUILD_GRAPH   = 'г'
};


static void ChoseAction();

static void ProcessCompareObject(Akinator* akinator);

static void ProcessWriteToFile(Akinator* akinator);

static void ProcessReadFile(Akinator* akinator);

static void ProcessBuilfGraph(Akinator* akinator);

static void ProcessDefineObjects(Akinator* akinator);


bool ShowMenu(Akinator* akinator)
{
    LogLine("Вызван ShowMenu()", LOG_DEBUG);

    assert(akinator);
    
    SetConsoleColor(VIOLET, BLACK);

    puts("Добро пожаловать в игру Акинатор!");
    
    SetConsoleColor(WHITE, BLACK);

    char c = '\0';

    while (true)
    {
        ChoseAction();
        if (scanf(" %c", &c) == 0)
        {
            SetConsoleColor(RED, BLACK);
            puts("Не удаётся прочитать входные данные");
            SetConsoleColor(WHITE, BLACK);
        }

        while (getchar() != '\n')
            break;

        switch (c)
        {
            case menuActions::PLAY_AKINATOR:
                PlayAkinator(akinator);
                break;
            case menuActions::CMP_OBJECTS:
                ProcessCompareObject(akinator);
                break;
            case menuActions::DEF_OBJECTS:
                ProcessDefineObjects(akinator);
                break;
            case menuActions::BUILD_GRAPH:
                ProcessBuilfGraph(akinator);
                break;
            case menuActions::READ_FILE:
                ProcessReadFile(akinator);
                break;
            case menuActions::WRITE_TO_FILE:
                ProcessWriteToFile(akinator);
                break;
            case menuActions::QUIT:
                SetConsoleColor(RED, BLACK);
                puts("Работа программы завершена :(");
                return true;
            default:
                SetConsoleColor(RED, BLACK);
                puts("Ошибка ввода.");
                SetConsoleColor(WHITE, BLACK);
                break;
        }
    }

    return false;
}


static void ProcessDefineObjects(Akinator* akinator)
{
    LogLine("Вызван ProcessDefineObjects()", LOG_DEBUG);

    assert(akinator);
    
    if (!akinator->tree.root)
    {
        SetConsoleColor(YELLOW, BLACK);
        puts("База данных пуста, сначала заполните её.\n"
             "Либо воспользуйтесь готовой, либо создаёте свою, играя в акинатор (пункт играть).");
        SetConsoleColor(WHITE, BLACK);
        return;
    }

    char buffer[MAX_STRING_LENGTH] = "";

    String str = {};

    str.ptr = buffer;
    
    SetConsoleColor(CYAN, BLACK);

    ReadString(&str, MAX_STRING_LENGTH, "Про кого вы хотите узнать все подробности:");
    
    SetConsoleColor(WHITE, BLACK);

    GetObjectDefinition(akinator, &str);
}

static void ProcessBuilfGraph(Akinator* akinator)
{
    LogLine("Вызван ProcessBuilfGraph()", LOG_DEBUG);

    assert(akinator);

#ifdef GRAPHVIZ
    char buffer[MAX_STRING_LENGTH] = "";

    SetConsoleColor(CYAN, BLACK);

    puts("Введите название файла, либо пустую строку для значения по умолчанию:");
    
    SetConsoleColor(WHITE, BLACK);

    char* res = ReadLine(buffer, MAX_STRING_LENGTH);
                
    if (res && *res)
        CreateGraph(akinator, res);
    else
        CreateGraph(akinator, "TreeGraph.png");
#endif
}

static void ProcessReadFile(Akinator* akinator)
{
    LogLine("Вызван ProcessReadFile()", LOG_DEBUG);

    assert(akinator);

    char buffer[MAX_STRING_LENGTH] = "";
    
    SetConsoleColor(CYAN, BLACK);

    puts("Введите название файла, либо пустую строку для значения по умолчанию:");

    SetConsoleColor(WHITE, BLACK);

    char* res = ReadLine(buffer, MAX_STRING_LENGTH);

    FILE* inputFile = nullptr;
    if (res && *res)
        inputFile = fopen(res, "r");
    else
        inputFile = fopen("tree.txt", "r");

    if (inputFile)
    {
        AkinatorDestructor(akinator);

        memset(akinator, 0, sizeof(Akinator));

        AkinatorConstructor(inputFile, akinator);
        fclose(inputFile);
    }
    else
        LogLine("ProcessReadFile: Ошибка открытия файла", LOG_ERROR, true);
}

static void ProcessWriteToFile(Akinator* akinator)
{
    LogLine("Вызван ProcessWriteToFile()", LOG_DEBUG);

    assert(akinator);

    char buffer[MAX_STRING_LENGTH] = "";
    
    SetConsoleColor(CYAN, BLACK);

    puts("Введите название файла, либо пустую строку для значения по умолчанию:");
    
    SetConsoleColor(WHITE, BLACK);

    char* res = ReadLine(buffer, MAX_STRING_LENGTH);

    FILE* outputFile = nullptr;
    if (res && *res)
        outputFile = fopen(res, "w");
    else
        outputFile = fopen("tree.txt", "w");

    if (outputFile)
    {
        WriteTreeToFile(akinator, outputFile);
        fclose(outputFile);
    }
    else
        LogLine("ProcessReadFile: Ошибка открытия файла", LOG_ERROR, true);
}

static void ProcessCompareObject(Akinator* akinator)
{
    LogLine("Вызван ProcessCompareObject()", LOG_DEBUG);

    assert(akinator);

    if (!akinator->tree.root)
    {
        SetConsoleColor(YELLOW, BLACK);
        puts("База данных пуста, сначала заполните её.\n"
             "Либо воспользуйтесь готовой, либо создаёте свою, играя в акинатор (пункт играть).");
        SetConsoleColor(WHITE, BLACK);
        return;
    }

    char buffer1[MAX_STRING_LENGTH] = "";
    char buffer2[MAX_STRING_LENGTH] = "";

    String str1 = {};
    String str2 = {};

    str1.ptr = buffer1;
    str2.ptr = buffer2;

    SetConsoleColor(CYAN, BLACK);

    ReadString(&str1, MAX_STRING_LENGTH, "Введите первый сравниваемый объект:");
    ReadString(&str2, MAX_STRING_LENGTH, "Введите второй сравниваемый объект:");

    SetConsoleColor(WHITE, BLACK);

    CompareObjects(akinator, &str1, &str2);
}

static void ChoseAction()
{
    LogLine("Вызван ChoseAction()", LOG_DEBUG);
    
    SetConsoleColor(DARK_VIOLET, BLACK);

    puts("Выберите действие:");
    
    SetConsoleColor(VIOLET, BLACK);

    for (int st = 0; st < sizeof(menuItems) / sizeof(menuItems[0]); st++)
    {
        if (menuItems[st] == nullptr)
        {
            if (st % 2 == 1)
                puts("");
            else
                printf("%24s", " ");
        }
        else
            printf("%c) %-20s%c", menuLetters[st], menuItems[st], (st % 2 == 1)? '\n': ' ');
    }
    
    SetConsoleColor(WHITE, BLACK);
}