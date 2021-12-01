#ifndef VOICE_H_
#define VOICE_H_

#include "Config.h"
#include "..\StringLibrary\StringLibrary.h"

#ifdef AKINATOR_VOICE

const size_t SPEAK_BUFFER_SIZE = 1024;

/**
 * @brief  ����������� �������.
 * @return true � ������ ������, false � ������ ������.
*/
bool VoiceConstructor();

/**
 * @brief ���������� �������. ���������� �������������.
*/
void VoiceDestructoror();

/**
 * @brief     ���������� �����.
 * @param ptr ��������� �� ������.
 * @return    true � ������ ������, false � ������ ������.
*/
bool SpeakUp(const char* ptr);

/**
 * @brief �������� ���������� ����� �������.
*/
void ClearSpeakBuffer();

/**
 * @brief �������� ���������� ����������� ������.
*/
void SpeakUpBuffer();

/**
 * @brief     �������� ������ �� ���������� �����.
 * @param ptr ��������� �� ������.
 * @return    true � ������ ������, false � ������ ������.
*/
bool AddStringToSpeakBuffer(const char* ptr);

/**
 * @brief         �������� ���������� ���� �� ���������� �����.
 * @param prefix  ���������� ���������.
 * @param str     ��������� �� ������ (�������� ����).
 * @param postfix ����������� ���������.
 * @return        true � ������ ������, false � ������ ������.
*/
bool AddNodeToSpeakBuffer(const char* prefix, String* str, const char* postfix);

/// ������� ���������� �� ���� � ������� � ��������� ��� �� ���������� �����.
#define PRINT_NODE_AND_SPEAK(prefix, str, postfix)                          \
        {                                                                   \
            PrintNodeValue(prefix, str, postfix, stdout);                   \
            AddNodeToSpeakBuffer(prefix, str, postfix);                     \
        }

/// ������� ������ � ������� � ��������� � �� ���������� �����.
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
