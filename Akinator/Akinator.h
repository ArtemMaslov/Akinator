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
 * @brief          Констркуктор структуры акинатор.
 * @param file     Указатель на файл, из которого нужно прочитать дерево.
 * @param akinator Указатель на структуру akinator.
 * @return         true в случае успеха, false в случае ошибки.
*/
bool AkinatorConstructor(FILE* file, Akinator* akinator);

/**
 * @brief          Дестркуктор структуры акинатор.
 * @param akinator Указатель на структуру akinator.
*/
void AkinatorDestructor(Akinator* akinator);

/**
 * @brief             Строит граф по дереву.
 * @param akinator    Указатель на структуру akinator.
 * @param outFileName Имя выходного файла.
 * @return            true в случае успеха, false в случае ошибки.
*/
bool CreateGraph(Akinator* akinator, const char* outFileName);

/**
 * @brief          Показать меню для выбора режима работы акинатора.
 * @param akinator Указатель на структуру akinator.
 * @return         true в случае успеха, false в случае ошибки. 
*/
bool ShowMenu(Akinator* akinator);

/**
 * @brief          Режим игры в акинатор. Программа пытается угадать объект, задуманный пользователем.
 * @param akinator Указатель на структуру akinator.
*/
void PlayAkinator(Akinator* akinator);

/**
 * @brief          Режим определения объекта.
 * @param akinator Указатель на структуру akinator.
 * @param str      Определяемый объект.
*/
void GetObjectDefinition(Akinator* akinator, String* str);

/**
 * @brief          Режим сравнения объектов.
 * @param akinator Указатель на структуру akinator.
 * @param str1     Первый объект.
 * @param str2     Второй объект.
*/
void CompareObjects(Akinator* akinator, String* str1, String* str2);


#endif