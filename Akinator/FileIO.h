#ifndef FILE_IO_H_
#define FILE_IO_H_


#include "Tree.h"
#include "Akinator.h"
#include "..\StringLibrary\StringLibrary.h"

/**
 * @brief            ��������� ������ � �������� � � ���������� String.
 * @param str        ��������� �� ������.
 * @param bufferSize ������������ ����� ������.
 * @param msg        ���������, ������� ���������� ������� � �������
*/
void ReadString(String* str, size_t bufferSize, const char* msg);

/**
 * @brief            ��������� ������ � ������ char[].
 * @param buffer     ��������� �� ������.
 * @param bufferSize ������������ ����� ������.
 * @return           ��������� �� ������ � ������ ������, nullptr � ������ ������.
*/
char* ReadLine(char* buffer, size_t bufferSize);

/**
 * @brief            ��������� ����� �� ������ (��/���).
 * @param buffer     ��������� �� ������.
 * @param bufferSize ������������ ����� ������.
 * @return           true ���� ����� ��, false ���� ����� ���.
*/
bool ReadQuestionAnswer(char* buffer, size_t bufferSize);

/**
 * @brief           ������� ���������� �� ������� �� ������.
 * @param indexFrom ������ ���������� ���� � ������.
 * @param indexTo   ������ ��������� ���� � ������.
 * @param stk       ��������� �� ����.
 * @param child     ��������� ����.
*/
void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child);

/**
 * @brief         ������� ���������� �� ����.
 * @param prefix  ���������� ���������.
 * @param str     ��������� �� ������ (�������� ����).
 * @param postfix ����������� ���������.
 * @param file    �������� ����.
*/
void PrintNodeValue(const char* prefix, String* str, const char* postfix, FILE* file);

/**
 * @brief          �������� ������ � ����.
 * @param akinator ��������� �� ��������� akinator.
 * @param file     �������� ����.
*/
void WriteTreeToFile(Akinator* akinator, FILE* file);


#endif