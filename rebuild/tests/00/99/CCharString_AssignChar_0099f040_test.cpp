#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"

static bool failAllocation;
static int releases;
static int constructions;
void* operator new(size_t size)
{
    if (failAllocation && size == sizeof(CCharStringData))
        return 0;
    return malloc(size);
}
void operator delete(void* memory) { free(memory); }
template<> CBasicString<char>::CBasicString(const char* text)
    : PData(0), StringLength(strlen(text)),
      DataLength(strlen(text) + 1), UseFastExtend(true)
{
    PData = static_cast<char*>(malloc(StringLength + 1));
    memcpy(PData, text, StringLength + 1);
    ++constructions;
}
void CCharString::UnassignString()
{
    ++releases;
    free(storage_->Data.PData);
    free(storage_);
    storage_ = 0;
}
int main()
{
    unsigned long storage = 0;
    CCharString& value = *reinterpret_cast<CCharString*>(&storage);
    if (&value.operator=('\0') != &value || storage || constructions || releases) return 1;
    if (&value.operator=('a') != &value || !storage || constructions != 1) return 2;
    CCharStringData* data = reinterpret_cast<CCharStringData*>(storage);
    if (data->owners != 1 || data->unknown04 != 1 || strcmp(data->text, "a")) return 3;
    if (&value.operator=(static_cast<char>(0x80)) != &value || releases != 1 || constructions != 2) return 4;
    data = reinterpret_cast<CCharStringData*>(storage);
    if (data->unknown04 != 1 || static_cast<unsigned char>(data->text[0]) != 0x80) return 5;
    if (&value.operator=('\0') != &value || storage || releases != 2) return 6;
    failAllocation = true;
    if (&value.operator=('z') != &value || storage || constructions != 2) return 7;
    failAllocation = false;
    puts("CHARSTRING_ASSIGN_CHAR_PASS");
    return 0;
}
