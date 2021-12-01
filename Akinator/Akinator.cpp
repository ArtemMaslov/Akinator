#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "FileIO.h"
#include "Tree.h"
#include "Voice.h"
#include "Akinator.h"


#include "..\Logs\Logs.h"
#include "..\StackLibrary\Stack.h"
#include "..\StringLibrary\StringLibrary.h"


static void ProcessUnknownObject(Tree* tree, Node* node, QuestionAnswer answer);

static void ProcessPredictedObject(Node* node);

static bool ParseText(Akinator* akinator);

static Node* AkinatorNodeConstructor();


Akinator AkinatorConstructor(FILE* file)
{
    //assert(file);

    static bool librariesInited = false;

    if (!librariesInited)
    {
        LogConstructor("Akinator.html", "��� ������ ��������� ��������.");
        VoiceConstructor();
    }

    LogLine("������ AkinatorConstructor()", LOG_DEBUG);

    Akinator akinator = {};
    akinator.file = file;
    akinator.IsTreeEmpty = true;
    bool result = false;

    TreeConstructor(&akinator.tree);

    result = ReadFile(&akinator.text, file);

    if (result)
    {
        result = ParseText(&akinator);
        TreeMeasure(&akinator.tree, akinator.tree.root, 0);

        if (akinator.tree.root && akinator.tree.root->value.ptr)
            akinator.IsTreeEmpty = false;
    }

    return akinator;
}

void AkinatorDestructor(Akinator* akinator)
{
    LogLine("������ AkinatorDestructor()", LOG_DEBUG);
    assert(akinator);

    TextDestructor(&akinator->text);

    TreeDestructor(&akinator->tree);
}

static bool ParseText(Akinator* akinator)
{
    LogLine("������ ParseText()", LOG_DEBUG);
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
            puts("��������� ��������� ������������������.\n"
                 "���������� ����������� ������ '{' ������ ���������� ����������� '}'");
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
            
            unsigned char* strStart = nullptr;

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
                    (char*)strStart,
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

            if (!_parentNode) // ������ ������
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

    StackConstructor(&stk, sizeof(Node*), akinator->tree.treeLength);
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, *str, &stk);
    
    if (!n1)
    {
        LogLine("GetObjectDefinition: ������ �� ������� � ������", LOG_ERROR, true);
        return;
    }

    ClearSpeakBuffer();

    PRINT_NODE_AND_SPEAK(nullptr, str, " - ");

    PrintObjectInformation(0, stk.stackSize, &stk, akinator->tree.root);

    SpeakUpBuffer();

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

    StackConstructor(&stk1, sizeof(Node*), akinator->tree.treeLength);
    StackConstructor(&stk2, sizeof(Node*), akinator->tree.treeLength);
    
    Node* n1 = TreeFindObjectStack(akinator->tree.root, *str1, &stk1);
    Node* n2 = TreeFindObjectStack(akinator->tree.root, *str2, &stk2);

    if (!n1 || !n2)
    {
        LogLine("CompareObjects: ������ �� ������� � ������", LOG_ERROR, true);
        return;
    }

    ClearSpeakBuffer();
    
    PRINT_NODE_AND_SPEAK(nullptr, str1, nullptr);
    PRINT_NODE_AND_SPEAK(" � ", str2, nullptr);

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
        PRINT_AND_SPEAK(" ������ ���, ��� ��� ");
        
        child1 = GetNodeFromStack(&stk1, 0);

        PrintObjectInformation(0, sameCount, &stk1, child1);
        puts(".");
    }
    else
        PRINT_AND_SPEAK(" ����� �� ������.\n");
    
    child1 = GetNodeFromStack(&stk1, sameCount);
    child2 = GetNodeFromStack(&stk2, sameCount);
        
    PRINT_NODE_AND_SPEAK(nullptr, str1, nullptr);
    PRINT_NODE_AND_SPEAK(" � ", str2, nullptr);

    if (stk1.stackSize - sameCount - 1 > 0 || stk2.stackSize - sameCount - 1 > 0)
    {
        PRINT_AND_SPEAK(" ����������� ���, ��� ");

        PRINT_NODE_AND_SPEAK(nullptr, str1, " - ");
    
        PrintObjectInformation(sameCount, stk1.stackSize, &stk1, child1);

        PRINT_NODE_AND_SPEAK(", � ", str2, " ");
    
        PrintObjectInformation(sameCount, stk2.stackSize, &stk2, child2);
        puts(".");
    }
    else
        PRINT_AND_SPEAK(" ��������� ������.\n");
    
    SpeakUpBuffer();

    puts(".");
}

void PlayAkinator(Akinator* akinator)
{
    LogLine("������ PlayAkinator()", LOG_DEBUG);
    assert(akinator);

    puts("��������� �����-������ ������, � � �������� ��� ��������.");

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
        PrintNodeValue("��� ", &ptr->value, "? (��/���)\n", stdout);

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

    puts("������� ������! �� ���� �������� ���������� ���� ������. ��� ����� ���������. ��� �� ��������?");

    Node* newChildNode = AkinatorNodeConstructor();

    ReadString(&newChildNode->value, MAX_STRING_LENGTH, nullptr);

    if (node == nullptr)
    {        
        PrintNodeValue("����� �� ��������� �������� ", &newChildNode->value, "?\n", stdout);

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

    PrintNodeValue("�� �������� ", &node->value, "? (��/���)", stdout);
    
    char buffer[MAX_STRING_LENGTH] = {};

    switch (ReadQuestionAnswer(buffer, MAX_STRING_LENGTH))
    {
        case YES:
            puts("��� ���� ��������!");
            break;
        case NO:
            puts("� � ��������. ��� �� ��� ���?");

            Node* newRightNode = AkinatorNodeConstructor();

            ReadString(&newRightNode->value, MAX_STRING_LENGTH, nullptr);
            
            PrintNodeValue("��� �� ���������� ", &newRightNode->value, " �� ", stdout);
            PrintNodeValue(nullptr, &node->value, "? ", stdout);
            PrintNodeValue(nullptr, &newRightNode->value, " - \n", stdout);

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