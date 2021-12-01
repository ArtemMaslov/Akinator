#ifndef AKINATOR_H_
#define AKINATOR_H_


#include "..\StringLibrary\StringLibrary.h"
#include "Tree.h"

struct Akinator
{
    Text  text;
    Tree  tree;
    FILE* file;

    bool IsTreeEmpty;
};

enum QuestionAnswer
{
    YES = 1,
    NO  = 0
};

const size_t MAX_STRING_LENGTH = 100;

/**
 * @brief      ������������ ��������� ��������.
 * @param file ��������� �� ����, �� �������� ����� ��������� ������.
 * @return     ��������� akinator.
*/
Akinator AkinatorConstructor(FILE* file);

/**
 * @brief          ����������� ��������� ��������.
 * @param akinator ��������� �� ��������� akinator.
*/
void AkinatorDestructor(Akinator* akinator);

/**
 * @brief             ������ ���� �� ������.
 * @param akinator    ��������� �� ��������� akinator.
 * @param outFileName ��� ��������� �����.
 * @return            true � ������ ������, false � ������ ������.
*/
bool CreateGraph(Akinator* akinator, const char* outFileName);

/**
 * @brief          �������� ���� ��� ������ ������ ������ ���������.
 * @param akinator ��������� �� ��������� akinator.
 * @return         true � ������ ������, false � ������ ������. 
*/
bool ShowMenu(Akinator* akinator);

/**
 * @brief          ����� ���� � ��������. ��������� �������� ������� ������, ���������� �������������.
 * @param akinator ��������� �� ��������� akinator.
*/
void PlayAkinator(Akinator* akinator);

/**
 * @brief          ����� ����������� �������.
 * @param akinator ��������� �� ��������� akinator.
 * @param str      ������������ ������.
*/
void GetObjectDefinition(Akinator* akinator, String* str);

/**
 * @brief          ����� ��������� ��������.
 * @param akinator ��������� �� ��������� akinator.
 * @param str1     ������ ������.
 * @param str2     ������ ������.
*/
void CompareObjects(Akinator* akinator, String* str1, String* str2);


#endif