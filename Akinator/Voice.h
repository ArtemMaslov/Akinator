#ifndef VOICE_H_
#define VOICE_H_

#include "Config.h"
#include "..\StringLibrary\StringLibrary.h"

#ifdef AKINATOR_VOICE

const size_t VOICE_BUFFER_SIZE = 1024;

struct VoiceBuffer
{
    /// Указатель на строку
    char buffer[VOICE_BUFFER_SIZE];
    /// Максимальная длина строки, включая 0
    size_t capacity;
    /// Текущая длина строки, не включая 0
    size_t size;
};

/**
 * @brief  Конструктор озвучки.
 * @return true в случае успеха, false в случае ошибки.
*/
bool VoiceConstructor();

/**
 * @brief Деструктор озвучки. Вызывается автоматически.
*/
void VoiceDestructoror();

/**
 * @brief     Произнести текст.
 * @param ptr Указатель на строку.
 * @return    true в случае успеха, false в случае ошибки.
*/
bool SpeakUp(const char* ptr);

/**
 * @brief        Добавляет строку в конец буфера
 * @param buffer Буфер, в который допишется строка
 * @param ptr    Строка
 * @param length Количество символов, которые необходимо добавить
 * @return       false, если буфер переполнен, true в случае успеха
*/
bool AddStringToBuffer(VoiceBuffer* buffer, const char* ptr, size_t length = -1);

#else

#define VoiceConstructor() false;
#define VoiceDestructoror();
#define SpeakUp(ptr) false;
#define AddStringToBuffer(buffer, ptr) false;

#endif

#endif
