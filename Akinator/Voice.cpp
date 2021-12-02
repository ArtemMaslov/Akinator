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

bool VoiceConstructor()
{
    LogLine("Вызван VoiceConstructor()", LOG_DEBUG);

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

    wchar_t wcharStr[VOICE_BUFFER_SIZE * sizeof(wchar_t)] = L"";
    
	MultiByteToWideChar(CP_ACP, 0, ptr, strLenth + 1, wcharStr, VOICE_BUFFER_SIZE);

    pVoice->Speak(wcharStr, 0, nullptr);
    return true;
}

bool AddStringToBuffer(VoiceBuffer* buffer, const char* ptr, size_t length)
{
    LogLine("Вызван AddStringToBuffer()", LOG_DEBUG);

    assert(buffer);
    assert(ptr);

    size_t strLength = strlen(ptr);

    if (length < strLength)
        strLength = length;

    if (buffer->size + 1 + strLength < buffer->capacity)
    {
        memmove(buffer->buffer + buffer->size, ptr, sizeof(char) * strLength);
        buffer->size += strLength;
        return true;
    }
    else
    {
        LogLine("AddStringToBuffer: voiceBuffer переполнен", LOG_ERROR, true);
        return false;
    }
}

#endif