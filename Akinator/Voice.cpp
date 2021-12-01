#include "Config.h"

#ifdef AKINATOR_VOICE

#include <stdio.h>
#include <assert.h>
#include <sapi.h>
#include <atlstr.h>


#include "Voice.h"
#include "Tree.h"
#include "Akinator.h"


#include "..\Logs\Logs.h"
#include "..\StringLibrary\StringLibrary.h"


static ISpVoice* pVoice = nullptr;
static wchar_t* speakBuffer = nullptr;

bool VoiceConstructor()
{
    LogLine("Вызван VoiceConstructor()", LOG_DEBUG);

    speakBuffer = (WCHAR*)calloc(SPEAK_BUFFER_SIZE, sizeof(WCHAR));
    if (!speakBuffer)
    {
        LogLine("VoiceConstructor: Не хватает памяти для создания голосового буфера", LOG_ERROR, true);
    }

    atexit(VoiceDestructoror);

    if (FAILED(CoInitialize(nullptr)))
        return false;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if (SUCCEEDED(hr))
    {
        return true;
    }

    return true;
}

void VoiceDestructoror()
{
    LogLine("Вызван VoiceDestructoror()", LOG_DEBUG);
    free(speakBuffer);

    pVoice->Release();
    pVoice = nullptr;
    CoUninitialize();
}

bool SpeakUp(const char* ptr)
{
    LogLine("Вызван SpeakUp()", LOG_DEBUG);
    assert(ptr);

    if (!pVoice)
        return false;

    size_t strLenth = strlen(ptr);

    wchar_t wcharStr[SPEAK_BUFFER_SIZE * sizeof(wchar_t)] = L"";
    
	MultiByteToWideChar(CP_ACP, 0, ptr, strLenth + 1, wcharStr, SPEAK_BUFFER_SIZE);

    pVoice->Speak(wcharStr, 0, nullptr);
    return true;
}

bool AddStringToSpeakBuffer(const char* ptr)
{	 
    LogLine("Вызван AddStringToSpeakBuffer()", LOG_DEBUG);
    assert(ptr);

    if (!pVoice)
        return false;

    size_t strLenth = strlen(ptr);

    wchar_t wcharStr[SPEAK_BUFFER_SIZE * sizeof(wchar_t)] = L"";
    
	MultiByteToWideChar(CP_ACP, 0, ptr, strLenth + 1, wcharStr, SPEAK_BUFFER_SIZE);

    wcscat(speakBuffer, wcharStr);
    return true;
}

void ClearSpeakBuffer()
{
    LogLine("Вызван ClearSpeakBuffer()", LOG_DEBUG);
    if (speakBuffer)
        memset(speakBuffer, 0, sizeof(WCHAR) * SPEAK_BUFFER_SIZE);
}

bool AddNodeToSpeakBuffer(const char* prefix, String* str, const char* postfix)
{
    LogLine("Вызван AddNodeToSpeakBuffer()", LOG_DEBUG);
    assert(str);

    if (!pVoice)
        return false;

    size_t strSize = str->length;

    if (prefix)
        strSize += strlen(prefix);

    if (postfix)
        strSize += strlen(postfix);

    char    concated[SPEAK_BUFFER_SIZE] = "";
    wchar_t wcharStr[SPEAK_BUFFER_SIZE * sizeof(wchar_t)] = L"";
    
    if (prefix)
        strcpy(concated, prefix);
    
    strncat(concated, str->ptr, str->length);

    if (postfix)
        strcat(concated, postfix);

	MultiByteToWideChar(CP_ACP, 0, concated, strSize + 1, wcharStr, strSize + 1);
	MultiByteToWideChar(CP_ACP, 0, concated, strSize + 1, wcharStr, SPEAK_BUFFER_SIZE);

    wcscat(speakBuffer, wcharStr);

    return true;
}

void SpeakUpBuffer()
{
    LogLine("Вызван SpeakUpBuffer()", LOG_DEBUG);
    if (!pVoice)
        return;

    pVoice->Speak(speakBuffer, 0, nullptr);
}

#endif