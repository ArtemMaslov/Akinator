#ifndef VOICE_H_
#define VOICE_H_

#include "Config.h"
#include "..\StringLibrary\StringLibrary.h"

#ifdef AKINATOR_VOICE

const size_t SPEAK_BUFFER_SIZE = 1024;

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
 * @brief Очистить внутренний буфер озвучки.
*/
void ClearSpeakBuffer();

/**
 * @brief Озвучить содержимое внутреннего буфера.
*/
void SpeakUpBuffer();

/**
 * @brief     Добавить строку во внутренний буфер.
 * @param ptr Указатель на строку.
 * @return    true в случае успеха, false в случае ошибки.
*/
bool AddStringToSpeakBuffer(const char* ptr);

/**
 * @brief         Добавить содержимое узла во внутренний буфер.
 * @param prefix  Префиксное сообщение.
 * @param str     Указатель на строку (значение узла).
 * @param postfix Постфиксное сообщение.
 * @return        true в случае успеха, false в случае ошибки.
*/
bool AddNodeToSpeakBuffer(const char* prefix, String* str, const char* postfix);

/// Выводит информацию об узле в консоль и добавляет его во внутренний буфер.
#define PRINT_NODE_AND_SPEAK(prefix, str, postfix)                          \
        {                                                                   \
            PrintNodeValue(prefix, str, postfix, stdout);                   \
            AddNodeToSpeakBuffer(prefix, str, postfix);                     \
        }

/// Выводит строку в консоль и добавляет её во внутренний буфер.
#define PRINT_AND_SPEAK(ptr)                                                \
        {                                                                   \
            fputs(ptr, stdout);                                             \
            AddStringToSpeakBuffer(ptr);                                    \
        }
#else

#define VoiceConstructor() false;
#define VoiceDestructoror();
#define SpeakUp(ptr) false;
#define ClearSpeakBuffer();
#define SpeakUpBuffer();
#define AddStringToSpeakBuffer(ptr) false;
#define AddNodeToSpeakBuffer(prefix, str, postfix) false;

#define PRINT_NODE_AND_SPEAK(prefix, str, postfix)                          \
        {                                                                   \
            PrintNodeValue(prefix, str, postfix, stdout);                   \
        }

#define PRINT_AND_SPEAK(ptr)                                                \
        {                                                                   \
            fputs(ptr, stdout);                                             \
        }

#endif

#endif
