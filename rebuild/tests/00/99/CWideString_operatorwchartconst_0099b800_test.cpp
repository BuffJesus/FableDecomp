#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CWideStringData
{
    wchar_t* text;
    unsigned int unknown04;
    unsigned int unknown08;
    int owners;
};

struct CWideString
{
    CWideStringData* storage_;
};

static int g_allocationCalls = 0;
static const wchar_t* g_observedText = 0;
static long g_observedLength = 0;
static CWideStringData g_newStorage;

/* Stand-in for CWideString::AllocStringData(text, length). */
CWideStringData* __fastcall CWideString_AllocStringData(
    CWideString* self, void* /*edx*/, const wchar_t* text, long length)
{
    (void)self;
    ++g_allocationCalls;
    g_observedText = text;
    g_observedLength = length;
    return &g_newStorage;
}

/* Faithful re-expression of retail 0x0099b800:
   CWideString::operator=(const wchar_t*) */
CWideString* __fastcall CWideString_AssignWCharPtr(
    CWideString* self, void* /*edx*/, const wchar_t* text)
{
    CWideStringData* storage = self->storage_;
    if (storage != 0)
    {
        if (--storage->owners <= 0)
        {
            storage = self->storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        self->storage_ = 0;
    }

    if (text != 0 && *text != L'\0')
        self->storage_ = CWideString_AllocStringData(self, 0, text, -1);

    return self;
}

int main()
{
    /* Case 1: null storage, assign non-empty text -> allocates new storage. */
    {
        memset(&g_newStorage, 0, sizeof(g_newStorage));
        CWideString value;
        value.storage_ = 0;
        g_allocationCalls = 0;

        CWideString* result = CWideString_AssignWCharPtr(&value, 0, L"names.bin");

        if (result != &value)
            return 1;
        if (value.storage_ != &g_newStorage || g_allocationCalls != 1)
            return 2;
        if (
            g_observedText == 0 ||
            wcscmp(g_observedText, L"names.bin") != 0 ||
            g_observedLength != -1)
        {
            return 3;
        }
    }

    /* Case 2: existing storage with owners > 1, assign empty text ->
       decrement owners, detach, no allocation, no free. */
    {
        CWideStringData* shared = new CWideStringData;
        shared->text = 0;
        shared->owners = 2;

        CWideString value;
        value.storage_ = shared;
        g_allocationCalls = 0;

        CWideString_AssignWCharPtr(&value, 0, L"");

        if (value.storage_ != 0)
            return 4;
        if (shared->owners != 1)
            return 5;
        if (g_allocationCalls != 0)
            return 6;

        delete shared;
    }

    /* Case 3: existing storage with owners == 1 (drops to 0), assign
       non-empty text -> frees old buffer + storage, then allocates new. */
    {
        wchar_t* oldBuffer = (wchar_t*)malloc(sizeof(wchar_t) * 4);
        wcscpy(oldBuffer, L"old");
        CWideStringData* owned = new CWideStringData;
        owned->text = oldBuffer;
        owned->owners = 1;

        CWideString value;
        value.storage_ = owned;
        g_allocationCalls = 0;

        CWideString_AssignWCharPtr(&value, 0, L"new-text");

        if (value.storage_ != &g_newStorage || g_allocationCalls != 1)
            return 8;
        if (g_observedLength != -1 || wcscmp(g_observedText, L"new-text") != 0)
            return 9;
    }

    /* Case 4: null storage, assign null text -> no-op, storage stays null. */
    {
        CWideString value;
        value.storage_ = 0;
        g_allocationCalls = 0;

        CWideString_AssignWCharPtr(&value, 0, 0);

        if (value.storage_ != 0 || g_allocationCalls != 0)
            return 10;
    }

    puts("FABLETLC_WIDE_ASSIGN_LITERAL_BEHAVIOR PASS");
    return 0;
}