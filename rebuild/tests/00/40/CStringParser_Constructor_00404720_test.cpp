#include <stdio.h>
#include <string.h>

#include <new>

#include "fable_string_parser.h"

char g_FableEmptyNarrowString_0129AAF4[1];

namespace
{
    struct TestCallLog
    {
        unsigned int baseConstructs;
        unsigned int stringConstructs;
        unsigned int assigns;
        unsigned int inits;
        const void* stringConstructed[2];
        const CCharString* assignedTo;
        const CCharString* assignedFrom;
        bool vtableWasBaseAtFirstString;
        const CStringParser* initSelf;
        const char* initString;
        CStringParser::ErrorCallbackFunction initCallback;
        const CCharString* initFilename;
        long initBaseLineOffset;
        bool commentsReadyAtInit;
        bool tempAssignedBeforeInit;
    };

    TestCallLog g_log;
    void* g_baseVTable;

    // Stands in for CCharString's private data pointer.
    CCharStringData*& Storage(CCharString& string)
    {
        return *reinterpret_cast<CCharStringData**>(&string);
    }

    void* VTable(const void* object)
    {
        return *static_cast<void* const*>(object);
    }

    void* CommentsSentinel(const CStringParser& parser)
    {
        return *reinterpret_cast<void* const*>(&parser.Comments);
    }

    void FABLE_FASTCALL ErrorCallback(const CCharString&)
    {
    }

    bool Construct(CCharString& input, long baseLineOffset, CStringParser*& parser)
    {
        static void* storage[sizeof(CStringParser) / sizeof(void*)];
        memset(storage, 0xA5, sizeof(storage));
        CCharString filename;
        memset(&g_log, 0, sizeof(g_log));
        g_baseVTable = 0;
        parser = new (storage) CStringParser(
            input,
            &ErrorCallback,
            filename,
            baseLineOffset);

        void* sentinel = CommentsSentinel(*parser);
        return
            g_log.baseConstructs == 1 &&
            g_log.stringConstructs == 2 &&
            g_log.stringConstructed[0] == &parser->Filename &&
            g_log.stringConstructed[1] == &parser->TempCharString &&
            g_log.vtableWasBaseAtFirstString == false &&
            VTable(parser) != g_baseVTable &&
            g_log.assigns == 1 &&
            g_log.assignedTo == &parser->TempCharString &&
            g_log.assignedFrom == &input &&
            g_log.inits == 1 &&
            g_log.initSelf == parser &&
            g_log.initCallback == &ErrorCallback &&
            g_log.initFilename == &filename &&
            g_log.initBaseLineOffset == baseLineOffset &&
            g_log.commentsReadyAtInit &&
            g_log.tempAssignedBeforeInit &&
            sentinel != 0 &&
            static_cast<void* const*>(sentinel)[0] == sentinel &&
            static_cast<void* const*>(sentinel)[1] == sentinel;
    }

    bool CheckTextInput()
    {
        char text[] = "Language English;";
        CCharStringData data;
        memset(&data, 0, sizeof(data));
        data.text = text;
        CCharString input;
        Storage(input) = &data;
        CStringParser* parser;
        return Construct(input, 7, parser) && g_log.initString == text;
    }

    bool CheckEmptyInput()
    {
        CCharString input;
        CStringParser* parser;
        return
            Construct(input, 0, parser) &&
            g_log.initString == g_FableEmptyNarrowString_0129AAF4;
    }
}

CBaseClass::CBaseClass()
{
    ++g_log.baseConstructs;
    g_baseVTable = VTable(this);
}

CBaseClass::~CBaseClass()
{
}



CCharString::CCharString()
{
    if (g_log.stringConstructs < 2)
    {
        g_log.stringConstructed[g_log.stringConstructs] = this;
    }
    if (g_log.stringConstructs == 0 && g_log.baseConstructs == 1)
    {
        // CStringParser's own vtable is installed before its members exist.
        g_log.vtableWasBaseAtFirstString =
            VTable(reinterpret_cast<char*>(this) - 0x0C) == g_baseVTable;
    }
    ++g_log.stringConstructs;
    storage_ = 0;
}

CCharString::~CCharString()
{
}

CCharString& CCharString::operator=(const CCharString& other)
{
    ++g_log.assigns;
    g_log.assignedTo = this;
    g_log.assignedFrom = &other;
    // A private copy, so parsing the caller's text is distinguishable.
    static char copyText[32];
    static CCharStringData copy;
    copy.text = copyText;
    storage_ = other.storage_ ? &copy : 0;
    return *this;
}

void CStringParser::Init(
    const char* string,
    ErrorCallbackFunction errorCallback,
    const CCharString& filename,
    long baseLineOffset)
{
    ++g_log.inits;
    g_log.initSelf = this;
    g_log.initString = string;
    g_log.initCallback = errorCallback;
    g_log.initFilename = &filename;
    g_log.initBaseLineOffset = baseLineOffset;
    void* sentinel = CommentsSentinel(*this);
    g_log.commentsReadyAtInit =
        sentinel != 0 && static_cast<void**>(sentinel)[0] == sentinel;
    g_log.tempAssignedBeforeInit = g_log.assigns == 1;
}

int main()
{
    if (!CheckTextInput() || !CheckEmptyInput())
    {
        printf("FABLETLC_STRING_PARSER_CTOR_00404720 FAIL\n");
        return 1;
    }

    printf("FABLETLC_STRING_PARSER_CTOR_00404720 PASS\n");
    return 0;
}
