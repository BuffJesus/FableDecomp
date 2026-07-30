#include <stdio.h>

#include "fable_bank_file.h"

extern "C" fable_u8 g_FableBankHeaderDirectory_013CA7D4[
    sizeof(CWideString)] = {0};

namespace
{
    unsigned int g_assignmentCalls;
    CWideString* g_assignmentDestination;
    const CWideString* g_assignmentSource;
}

CWideString& CWideString::operator=(
    const CWideString& other)
{
    ++g_assignmentCalls;
    g_assignmentDestination = this;
    g_assignmentSource = &other;
    storage_ = other.storage_;
    return *this;
}

int main()
{
    fable_u8 sourceStorage[sizeof(CWideString)] = {0};
    *reinterpret_cast<void**>(sourceStorage) =
        reinterpret_cast<void*>(0x12345678);
    const CWideString& source =
        *reinterpret_cast<const CWideString*>(sourceStorage);

    NBankFileManager::SetHeaderDir(source);

    const bool passed =
        g_assignmentCalls == 1 &&
        g_assignmentDestination == reinterpret_cast<CWideString*>(
            g_FableBankHeaderDirectory_013CA7D4) &&
        g_assignmentSource == &source &&
        *reinterpret_cast<void**>(
            g_FableBankHeaderDirectory_013CA7D4) ==
                reinterpret_cast<void*>(0x12345678);
    if (!passed)
    {
        printf("FABLETLC_BANK_HEADER_DIRECTORY_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_BANK_HEADER_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
