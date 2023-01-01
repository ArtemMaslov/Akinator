#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "FileIO.h"
#include "Tree.h"
#include "Voice.h"
#include "Akinator.h"
#include "Utilities.h"


#include "..\Logs\Logs.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static void ProcessUnknownObject(Tree* tree, Node* node, QuestionAnswer answer);

static void ProcessPredictedObject(Node* node);

static Node* AkinatorNodeConstructor();

/// ������� ������ � ������� � ��������� � �� ���������� ����� ��� �������.
#define PRINT_AND_SPEAK(ptr)                                                \
        {                                                                   \
            fputs(ptr, stdout);                                             \
            AddStringToSpeakBuffer(ptr);                                    \
        }


bool AkinatorConstructor(FILE* file, Akinator* akinator)
{
    assert(akinator);
    //assert(file);

    static bool librariesInited = false;

    if (!librariesInited)
    {
        LogConstructor("Akinator.html", "��� ������ ��������� ��������.");
        VoiceConstructor();
    }

    LogLine("������ AkinatorConstructor()", LOG_DEBUG);

    akinator->file = file;
    akinator->IsTreeEmpty = true;
    bool result = false;

    TreeConstructor(&akinator->tree);

    result = ReadFile(&akinator->text, file);

    if (!result)
        return result;

    result = ParseText(akinator);
    
    if (!result)
        return result;

    TreeMeasure(&akinator->tree, akinator->tree.root, 0);

    if (akinator->tree.root && akinator->tree.root->value.ptr)
        akinator->IsTreeEmpty = false;
    
    return result;
}

void AkinatorDestructor(Akinator* akinator)
{
    LogLine("������ AkinatorDestructor()", LOG_DEBUG);

    assert(akinator);

    TextDestructor(&akinator->text);

    TreeDestructor(&akinator->tree);
}

bool CreateGraph(Akinator* akinator, const char* outFileName)
{
    LogLine("������ CreateGraph()", LOG_DEBUG);

    assert(akinator);
    assert(outFileName);

    return CreateTreeGraph(outFileName, &akinator->tree);
}

void GetObjectDefinition(Akinator* akinator, String* str)
{
    LogLine("������ GetObjectDefinition()", LOG_DEBUG);

    assert(akinator);
    assert(str);

    Stack stk = {};
    VoiceBuffer buffer = {};
    buffer.capacity = VOICE_BUFFER_SIZE;

    StackConstructor(&stk, sizeof(Node*), akinator->tree.treeLength);
    
    Node* node = TreeFindObjectStack(akinator->tree.root, *str, &stk);
    
    if (!node)
    {
        LogLine("GetObjectDefinition: ������ �� ������� � ������", LOG_ERROR, true);
        return;
    }

    SetConsoleColor(YELLOW, BLACK);

    PrintNodeValue(nullptr, str, " - ", &buffer, stdout);
    
    SetConsoleColor(WHITE, BLACK);

    PrintObjectInformation(0, stk.stackSize, &stk, akinator->tree.root, &buffer);

    SpeakUp(buffer.buffer);

    puts(".");
}

void CompareObjects(Akinator* akinator, String* str1, String* str2)
{
    LogLine("������ CompareObjects()", LOG_DEBUG);

    assert(akinator);
    assert(str1);
    assert(str2);

    Stack stk1 = {};
    Stack stk2 = {};
    VoiceBuffer buffer = {};
    buffer.capacity = VOICE_BUFFER_SIZE;

    StackConstructor(&stk1, sizeof(Node*), akinator->tree.treeLength);
    StackConstructor(&stk2, sizeof(Node*), akinator->tree.treeLength);
    
    Node* node1 = TreeFindObjectStack(akinator->tree.root, *str1, &stk1);
    Node* node2 = TreeFindObjectStack(akinator->tree.root, *str2, &stk2);

    if (!node1 || !node2)
    {
        LogLine("CompareObjects: ������ �� ������� � ������", LOG_ERROR, true);
        return;
    }
    
    SetConsoleColor(YELLOW, BLACK);

    PrintNodeValue(nullptr, str1, nullptr, &buffer, stdout);
    PrintNodeValue(" � ", str2, nullptr, &buffer, stdout);
    
    SetConsoleColor(WHITE, BLACK);

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
    if (index < sameCount - 1)
    {
        SetConsoleColor(GREEN, BLACK);

        fputs(" ������ ���, ��� ��� ", stdout);
        AddStringToBuffer(&buffer, " ������ ���, ��� ��� ");

        SetConsoleColor(WHITE, BLACK);
        
        child1 = GetNodeFromStack(&stk1, 0);

        PrintObjectInformation(0, sameCount, &stk1, child1, &buffer);
        puts(".");
    }
    else
    {
        SetConsoleColor(DARK_RED, BLACK);
        fputs(" ����� �� ������.\n", stdout);
        AddStringToBuffer(&buffer, " ����� �� ������.");
        SetConsoleColor(WHITE, BLACK);
    }
    
    child1 = GetNodeFromStack(&stk1, sameCount - 1);
    child2 = GetNodeFromStack(&stk2, sameCount - 1);
    
    
    SetConsoleColor(YELLOW, BLACK);

    PrintNodeValue(nullptr, str1, nullptr, &buffer, stdout);
    PrintNodeValue(" � ", str2, nullptr, &buffer, stdout);
    
    SetConsoleColor(WHITE, BLACK);

    if (stk1.stackSize - sameCount > 0 || stk2.stackSize - sameCount > 0)
    {
        SetConsoleColor(DARK_RED, BLACK);
        fputs(" ����������� ���, ��� ", stdout);
        AddStringToBuffer(&buffer, " ����������� ���, ��� ");
        SetConsoleColor(WHITE, BLACK);

        PrintNodeValue(nullptr, str1, " - ", &buffer, stdout);
    
        PrintObjectInformation(sameCount - 1, stk1.stackSize, &stk1, child1, &buffer);

        PrintNodeValue(", � ", str2, " ", &buffer, stdout);
    
        PrintObjectInformation(sameCount - 1, stk2.stackSize, &stk2, child2, &buffer);
        puts(".");
    }
    else
    {
        SetConsoleColor(GREEN, BLACK);
        fputs(" ��������� ������.\n", stdout);
        AddStringToBuffer(&buffer, " ��������� ������.");
        SetConsoleColor(WHITE, BLACK);
    }

    SpeakUp(buffer.buffer);
}

void PlayAkinator(Akinator* akinator)
{
    LogLine("������ PlayAkinator()", LOG_DEBUG);

    assert(akinator);
    
    SetConsoleColor(YELLOW, BLACK);

    puts("��������� �����-������ ������, � � �������� ��� ��������.");
    
    SetConsoleColor(WHITE, BLACK);

    if (akinator->IsTreeEmpty)
    {
        ProcessUnknownObject(&akinator->tree, nullptr, NO);
        akinator->IsTreeEmpty = false;
        return;
    }

    Node* ptr = akinator->tree.root;

    char buffer[MAX_STRING_LENGTH] = {};

    do
    {
        SetConsoleColor(DARK_YELLOW, BLACK);
        PrintNodeValue("��� ", &ptr->value, "? (��/���)\n", nullptr, stdout);
        SetConsoleColor(WHITE, BLACK);

        switch (ReadQuestionAnswer(buffer, MAX_STRING_LENGTH))
        {
            case YES:
                if (ptr->nodeRight == nullptr && ptr->nodeLeft != nullptr)
                {
                    ProcessUnknownObject(&akinator->tree, ptr, YES);
                    return;
                }
                else if (IsLeaf(ptr->nodeRight))
                {
                    ProcessPredictedObject(ptr->nodeRight);
                    return;
                }
                ptr = ptr->nodeRight;
                break;
            case NO:
                if (ptr->nodeLeft == nullptr && ptr->nodeRight != nullptr)
                {
                    ProcessUnknownObject(&akinator->tree, ptr, NO);
                    return;
                }
                else if (IsLeaf(ptr->nodeLeft))
                {
                    ProcessPredictedObject(ptr->nodeLeft);
                    return;
                }
                ptr = ptr->nodeLeft;
                break;
        }
    }
    while (ptr && (ptr->nodeLeft || ptr->nodeRight));
}

static void ProcessUnknownObject(Tree* tree, Node* node, QuestionAnswer answer)
{
    LogLine("������ ProcessUnknownObject()", LOG_DEBUG);

    assert(tree);
    //assert(node);
    
    SetConsoleColor(RED, BLACK);
    puts("������� ������! �� ���� �������� ���������� ���� ������. ��� ����� ���������. ��� �� ��������?");
    SetConsoleColor(WHITE, BLACK);

    Node* newChildNode = AkinatorNodeConstructor();

    ReadString(&newChildNode->value, MAX_STRING_LENGTH, nullptr);

    if (node == nullptr)
    {        
        SetConsoleColor(CYAN, BLACK);
        PrintNodeValue("����� �� ��������� �������� ", &newChildNode->value, "?\n", nullptr, stdout);
        SetConsoleColor(WHITE, BLACK);

        Node* newParentNode = AkinatorNodeConstructor();
            
        ReadString(&newParentNode->value, MAX_STRING_LENGTH, nullptr);

        tree->root = newParentNode;
        TreeAddRightNode(newParentNode, newChildNode);
    }
    else
    {
        switch (answer)
        {
            case YES:
                TreeAddRightNode(node, newChildNode);
                break;
            case NO:
                TreeAddLeftNode(node, newChildNode);
                break;
        }
    }
}

static void ProcessPredictedObject(Node* node)
{
    LogLine("������ ProcessPredictedObject()", LOG_DEBUG);

    assert(node);

    SetConsoleColor(GREEN, BLACK);
    PrintNodeValue("�� �������� ", &node->value, "? (��/���)", nullptr, stdout);
    SetConsoleColor(WHITE, BLACK);
    
    char buffer[MAX_STRING_LENGTH] = {};

    switch (ReadQuestionAnswer(buffer, MAX_STRING_LENGTH))
    {
        case YES:
            SetConsoleColor(YELLOW, BLACK);
            puts("��� ���� ��������!");
            SetConsoleColor(WHITE, BLACK);
            break;
        case NO:
            SetConsoleColor(RED, BLACK);
            puts("� � ��������. ��� �� ��� ���?");
            SetConsoleColor(WHITE, BLACK);

            Node* newRightNode = AkinatorNodeConstructor();

            ReadString(&newRightNode->value, MAX_STRING_LENGTH, nullptr);
            
            SetConsoleColor(CYAN, BLACK);
            PrintNodeValue("��� �� ���������� ", &newRightNode->value, " �� ", nullptr, stdout);
            PrintNodeValue(nullptr, &node->value, "? ", nullptr, stdout);
            PrintNodeValue("����� �������� �������� ���� ������, �������� �� ������: ", &newRightNode->value, " - ���?\n", nullptr, stdout);
            fputs("��������� ����������� � ��������� �����, � ����� �� ������� ���� ����������.\n", stdout);
            fputs("��������, ��� \"�����\" ��������� �������� ����� ��������: \"����� ��������\", \"��������, ������� ���� �� ����\", \"������\", \"��������, ������� �������� �� ������\".\n", stdout);
            SetConsoleColor(WHITE, BLACK);

            Node* newLeftNode = AkinatorNodeConstructor();
            
            ReadString(&newLeftNode->value, MAX_STRING_LENGTH, nullptr);

            String parentStr = node->value;
            node->value = newLeftNode->value;
            newLeftNode->value = parentStr;

            TreeAddLeftNode(node, newLeftNode);
            TreeAddRightNode(node, newRightNode);
            break;
    }
}

static Node* AkinatorNodeConstructor()
{
    LogLine("������ AkinatorNodeConstructor()", LOG_DEBUG);

    Node* newNode = TreeNodeConstructor(nullptr);

    if (!newNode)
        return nullptr;

    newNode->value.ptr = (char*)calloc(MAX_STRING_LENGTH, sizeof(char));
            
    if (!newNode->value.ptr)
    {
        LogLine("ProcessPredictedObject: �� ����� ������ ��� ������ ������ ����.", LOG_ERROR, true);
        return nullptr;
    }

    return newNode;
}