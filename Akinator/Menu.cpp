#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "Config.h"
#include "FileIO.h"
#include "Tree.h"
#include "Akinator.h"


#include "..\Logs\Logs.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static const char* menuItems[] =
{
    "������",
    "����������",
    "��������",
    nullptr,
    "������� �� �����",
    "�������� � ����",
#ifdef GRAPHVIZ
    "��������� ����",
#endif
    nullptr,
    nullptr,
    "�����",
};

static const char menuLetters[]
{
    '�',
    '�',
    '�',
    '\0',
    '�',
    '�',
#ifdef GRAPHVIZ
    '�',
#endif
    '\0',
    '\0',
    '�'
};

enum menuActions
{
    PLAY_AKINATOR = '�',
    DEF_OBJECTS   = '�',
    CMP_OBJECTS   = '�',
    QUIT          = '�',
    READ_FILE     = '�',
    WRITE_TO_FILE = '�',
    BUILD_GRAPH   = '�'
};


static void ChoseAction();

static void ProcessCompareObject(Akinator* akinator);

static void ProcessWriteToFile(Akinator* akinator);

static void ProcessReadFile(Akinator* akinator);

static void ProcessBuilfGraph(Akinator* akinator);

static void ProcessDefineObjects(Akinator* akinator);


bool ShowMenu(Akinator* akinator)
{
    LogLine("������ ShowMenu()", LOG_DEBUG);
    assert(akinator);

    puts("����� ���������� � ���� ��������!");

    char c = '\0';

    while (true)
    {
        ChoseAction();
        if (scanf("%c", &c) == 0)
            puts("�� ������ ��������� ������� ������");

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
                puts("������ ��������� ��������� :(");
                return true;
            default:
                puts("������ �����.");
                break;
        }
    }

    return false;
}


static void ProcessDefineObjects(Akinator* akinator)
{
    LogLine("������ ProcessDefineObjects()", LOG_DEBUG);
    assert(akinator);

    char buffer[MAX_STRING_LENGTH] = "";

    String str = {};

    str.ptr = buffer;

    ReadString(&str, MAX_STRING_LENGTH, "��� ���� �� ������ ������ ��� �����������:");

    GetObjectDefinition(akinator, &str);
}

static void ProcessBuilfGraph(Akinator* akinator)
{
    LogLine("������ ProcessBuilfGraph()", LOG_DEBUG);
    assert(akinator);

#ifdef GRAPHVIZ
    char buffer[MAX_STRING_LENGTH] = "";

    puts("������� �������� �����, ���� ������ ������ ��� �������� �� ���������:");

    char* res = ReadLine(buffer, MAX_STRING_LENGTH);
                
    if (res && *res)
        CreateGraph(akinator, res);
    else
        CreateGraph(akinator, "TreeGraph.png");
#endif
}

static void ProcessReadFile(Akinator* akinator)
{
    LogLine("������ ProcessReadFile()", LOG_DEBUG);
    assert(akinator);

    char buffer[MAX_STRING_LENGTH] = "";

    puts("������� �������� �����, ���� ������ ������ ��� �������� �� ���������:");

    char* res = ReadLine(buffer, MAX_STRING_LENGTH);

    FILE* inputFile = nullptr;
    if (res && *res)
        inputFile = fopen(res, "r");
    else
        inputFile = fopen("tree.txt", "r");

    if (inputFile)
    {
        AkinatorDestructor(akinator);

        *akinator = AkinatorConstructor(inputFile);
        fclose(inputFile);
    }
    else
        puts("������ �������� �����");
}

static void ProcessWriteToFile(Akinator* akinator)
{
    LogLine("������ ProcessWriteToFile()", LOG_DEBUG);
    assert(akinator);

    char buffer[MAX_STRING_LENGTH] = "";

    puts("������� �������� �����, ���� ������ ������ ��� �������� �� ���������:");

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
        puts("������ �������� �����");
}

static void ProcessCompareObject(Akinator* akinator)
{
    LogLine("������ ProcessCompareObject()", LOG_DEBUG);
    assert(akinator);

    char buffer1[MAX_STRING_LENGTH] = "";
    char buffer2[MAX_STRING_LENGTH] = "";

    String str1 = {};
    String str2 = {};

    str1.ptr = buffer1;
    str2.ptr = buffer2;

    ReadString(&str1, MAX_STRING_LENGTH, "������� ������ ������������ ������:");
    ReadString(&str2, MAX_STRING_LENGTH, "������� ������ ������������ ������:");

    CompareObjects(akinator, &str1, &str2);
}

static void ChoseAction()
{
    LogLine("������ ChoseAction()", LOG_DEBUG);
    puts("�������� ��������:");

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
}