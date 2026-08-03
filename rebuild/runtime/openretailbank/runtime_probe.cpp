#include "fable_retail_banks.h"
#include "../../integration/fable_bank_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

struct BankListNode
{
    BankListNode* next;
    BankListNode* prev;
    void* object;
    void* owners;
};

struct ContainedBankMapNode
{
    unsigned char treeHeader[0x10];
    void* keyData;
    unsigned long values[5];
    ContainedBankMapNode* next;
};

struct ContainedBankKeyData
{
    char* text;
    unsigned long length;
    unsigned long unknown08;
    unsigned char flags0C;
    long owners;
};

struct RawThreadedFile
{
    unsigned long vtable;
    unsigned long deviceId;
    FILE* fileHandle;
    void* filenameStorage;
    unsigned long length;
    unsigned long physicalSortKey;
    unsigned char openedForWrite;
    unsigned char openFlag;
    unsigned char padding1A[2];
};

static const char* kDefaultBank =
    "C:\\Programs\\Steam\\steamapps\\common\\Fable The Lost Chapters"
    "\\data\\lang\\English\\fonts.big";

static bool MakeWideString(
    CWideString* destination,
    CWideStringData* storage,
    wchar_t* text,
    size_t capacity,
    const char* source)
{
    size_t length = mbstowcs(text, source, capacity - 1);
    if (length == (size_t)-1)
        return false;
    text[capacity - 1] = 0;
    storage->text = text;
    storage->unknown04 =
        (fable_u32)(text + length);
    storage->unknown08 = 0;
    storage->owners = 1;
    *reinterpret_cast<CWideStringData**>(destination) = storage;
    return true;
}

static bool CompareContainedBankMap(
    const fable_bank::Bank* oracle,
    const NBankFileManager::CRetailBank* bank)
{
    bool match = bank->containedBankCount_ == oracle->subCount;
    unsigned long mapCount = 0;
    ContainedBankMapNode* mapHead =
        reinterpret_cast<ContainedBankMapNode*>(bank->containedBanksHead_);
    for (ContainedBankMapNode* node = mapHead ? mapHead->next : 0;
         mapHead && node && node != mapHead && mapCount <= oracle->subCount;
         node = node->next)
    {
        ++mapCount;
        ContainedBankKeyData* key =
            reinterpret_cast<ContainedBankKeyData*>(node->keyData);
        const char* name = key && key->text ? key->text : "";
        int subIndex = fable_bank::FindSubBank(oracle, name);
        bool entryMatch = subIndex >= 0;
        if (entryMatch)
        {
            const fable_bank::SubBank& expected = oracle->subs[subIndex];
            const unsigned long expectedValues[5] = {
                expected.version, expected.entryCount, expected.tocOffset,
                expected.tocSize, expected.align};
            for (unsigned int i = 0; i < 5; ++i)
                entryMatch = entryMatch && node->values[i] == expectedValues[i];
        }
        if (!entryMatch)
        {
            printf("PROBE map-mismatch key=%s values=%lu,%lu,%lu,%lu,%lu\n",
                   name, node->values[0], node->values[1], node->values[2],
                   node->values[3], node->values[4]);
        }
        match = match && entryMatch;
    }
    match = match && mapCount == oracle->subCount;
    printf("PROBE map-parity expected=%u actual=%lu\n",
           oracle->subCount, mapCount);
    return match;
}

static bool CompareThreadedPayloads(
    const fable_bank::Bank* oracle,
    const NBankFileManager::CRetailBank* bank)
{
    RawThreadedFile* threaded =
        bank ? reinterpret_cast<RawThreadedFile*>(bank->threadedFile_) : 0;
    bool match = threaded && threaded->fileHandle && threaded->openFlag &&
        threaded->length == oracle->size;
    unsigned long totalBytes = 0;
    unsigned int expectedEntries = 0;
    unsigned int checkedEntries = 0;
    unsigned char actual[65536];

    for (unsigned int i = 0; i < oracle->subCount; ++i)
        expectedEntries += oracle->subs[i].entryCount;

    if (match)
    {
        for (unsigned int i = 0; i < oracle->subCount && match; ++i)
        {
            const fable_bank::SubBank& sub = oracle->subs[i];
            fable_bank::Cursor cursor;
            cursor.b = oracle->buf;
            cursor.size = oracle->size;
            cursor.p = sub.tocOffset;
            unsigned int save = cursor.p;
            unsigned int statsCount = cursor.u32v();
            if (statsCount < 1000)
            {
                for (unsigned int k = 0; k < statsCount; ++k)
                {
                    cursor.u32v();
                    cursor.u32v();
                }
            }
            else
            {
                cursor.p = save;
            }

            for (unsigned int entry = 0;
                 entry < sub.entryCount && match;
                 ++entry)
            {
                cursor.u32v();
                cursor.u32v();
                cursor.u32v();
                unsigned int size = cursor.u32v();
                unsigned int offset = cursor.u32v();
                cursor.u32v();
                unsigned int nameLength = 0;
                cursor.lpstr(&nameLength);
                cursor.u32v();
                unsigned int dependencyCount = cursor.u32v();
                for (unsigned int d = 0; d < dependencyCount; ++d)
                {
                    unsigned int dependencyLength = 0;
                    cursor.lpstr(&dependencyLength);
                }
                unsigned int infoSize = cursor.u32v();
                cursor.p += infoSize;

                if (offset > oracle->size || size > oracle->size - offset)
                {
                    match = false;
                    break;
                }

                unsigned int position = 0;
                while (position < size)
                {
                    unsigned int request = size - position;
                    if (request > sizeof(actual))
                        request = sizeof(actual);
                    if (fseek(threaded->fileHandle,
                              (long)(offset + position), SEEK_SET) != 0 ||
                        fread(actual, 1, request, threaded->fileHandle) != request ||
                        memcmp(actual, oracle->buf + offset + position, request) != 0)
                    {
                        match = false;
                        break;
                    }
                    position += request;
                    totalBytes += request;
                }
                ++checkedEntries;
            }
        }
    }

    printf("PROBE threaded-parity entries=%u/%u bytes=%lu state=%s\n",
           checkedEntries, expectedEntries, totalBytes,
           match ? "OK" : "FAILED");
    return match && checkedEntries == expectedEntries;
}

int main(int argc, char** argv)
{
    const char* path = argc > 1 ? argv[1] : kDefaultBank;
    printf("PROBE before-oracle path=%s\n", path);
    fflush(stdout);

    fable_bank::Bank oracle;
    if (!fable_bank::Open(&oracle, path))
    {
        printf("PROBE oracle-open FAILED path=%s\n", path);
        return 2;
    }
    // The standalone integration test owns the full TOC/glyph oracle. This
    // runtime probe compares the subbank count and contained-bank records with
    // the raw engine object.
    printf("PROBE oracle-open OK size=%u subbanks=%u\n",
           oracle.size, oracle.subCount);
    fflush(stdout);
    unsigned char managerBytes[0x38];
    memset(managerBytes, 0, sizeof(managerBytes));
    *reinterpret_cast<unsigned long*>(managerBytes + 0x34) = 100;
    CBankFileManager* manager =
        reinterpret_cast<CBankFileManager*>(managerBytes);

    BankListNode sentinel;
    memset(&sentinel, 0, sizeof(sentinel));
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;
    *reinterpret_cast<BankListNode**>(managerBytes + 0x10) = &sentinel;

    CWideStringData pathStorage;
    wchar_t widePath[512];
    unsigned char pathnameBytes[sizeof(CWideString)];
    CWideString* pathname =
        reinterpret_cast<CWideString*>(pathnameBytes);
    if (!MakeWideString(
            pathname, &pathStorage, widePath,
            sizeof(widePath) / sizeof(widePath[0]), path))
    {
        printf("PROBE path-conversion FAILED path=%s\n", path);
        fable_bank::Close(&oracle);
        return 4;
    }
    printf("PROBE boundary-ready\n");
    fflush(stdout);

    manager->OpenRetailBank(*pathname, true);

    BankListNode* first = sentinel.next;
    if (first == &sentinel)
    {
        printf("PROBE raw-open FAILED list-empty oracle-subbanks=%u\n",
               oracle.subCount);
        fable_bank::Close(&oracle);
        return 5;
    }

    NBankFileManager::CRetailBank* bank =
        reinterpret_cast<NBankFileManager::CRetailBank*>(first->object);
    if (!bank)
    {
        printf("PROBE raw-open FAILED list-node-null\n");
        fable_bank::Close(&oracle);
        return 6;
    }

    printf("PROBE raw-open OK oracle-subbanks=%u raw-subbanks=%lu\n",
           oracle.subCount,
           (unsigned long)bank->containedBankCount_);
    fflush(stdout);
    const bool mapMatch = CompareContainedBankMap(&oracle, bank);
    const bool threadedMatch = CompareThreadedPayloads(&oracle, bank);
    fable_bank::Close(&oracle);
    return mapMatch && threadedMatch ? 0 : 7;
}
