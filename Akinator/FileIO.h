#ifndef FILE_IO_H_
#define FILE_IO_H_


#include "Tree.h"
#include "Voice.h"
#include "Akinator.h"
#include "..\StringLibrary\StringLibrary.h"

/**
 * @brief            Прочитать строку и записать её в стркуктуру String.
 * @param str        Указатель на строку.
 * @param bufferSize Максимальная длина строки.
 * @param msg        Сообщение, которое необходимо вывести в консоль
*/
void ReadString(String* str, size_t bufferSize, const char* msg);

/**
 * @brief            Прочитать строку в массив char[].
 * @param buffer     Указатель на строку.
 * @param bufferSize Максимальная длина строки.
 * @return           Указатель на строку в случае успеха, nullptr в случае ошибки.
*/
char* ReadLine(char* buffer, size_t bufferSize);

/**
 * @brief            Прочитать ответ на вопрос (да/нет).
 * @param buffer     Указатель на строку.
 * @param bufferSize Максимальная длина строки.
 * @return           true если ответ да, false если ответ нет.
*/
bool ReadQuestionAnswer(char* buffer, size_t bufferSize);

/**
 * @brief           Вывести информацию об объекте из дерева.
 * @param indexFrom Индекс начального узла в дереве.
 * @param indexTo   Индекс конечного узла в дереве.
 * @param stk       Указатель на стек.
 * @param child     Начальный узел.
 * @param buffer    Голосовой буфер.
*/
void PrintObjectInformation(size_t indexFrom, size_t indexTo, Stack* stk, Node* child, VoiceBuffer* buffer);

/**
 * @brief             Вывести информацию об узле.
 * @param prefix      Префиксное сообщение.
 * @param str         Указатель на строку (значение узла).
 * @param postfix     Постфиксное сообщение.
 * @param voiceBuffer Голосовой буфер
 * @param file        Выходной файл.
*/
void PrintNodeValue(const char* prefix, String* str, const char* postfix, VoiceBuffer* voiceBuffer, FILE* file);

/**
 * @brief          Записать дерево в файл.
 * @param akinator Указатель на структуру akinator.
 * @param file     Выходной файл.
*/
void WriteTreeToFile(Akinator* akinator, FILE* file);

/**
 * @brief          Преобразует текст из файла в структуру дерево.
 * @param akinator Указатель на структуру акинатор.
 * @return         true в случае успеха, false в случае ошибки
*/
bool ParseText(Akinator* akinator);


#endif