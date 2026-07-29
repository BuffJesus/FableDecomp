#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CCharString_NotEqual_0099e900.cpp"

struct TestStringStorage_0099e900
{
    const char* text;
    unsigned long length;
};

struct TestCharString_0099e900
{
    TestStringStorage_0099e900* storage;
};

static int g_CompareCalls;

extern "C" long __fastcall
CCharString_CompareBytes_0099e900(const char* left, const char* right)
{
    ++g_CompareCalls;
    return std::strcmp(left, right);
}

static int Check(
    TestStringStorage_0099e900* leftStorage,
    TestStringStorage_0099e900* rightStorage,
    unsigned char expected,
    int expectedCompareCalls)
{
    TestCharString_0099e900 left;
    TestCharString_0099e900 right;
    left.storage = leftStorage;
    right.storage = rightStorage;
    g_CompareCalls = 0;

    typedef unsigned char (__fastcall *Predicate)(
        const void*,
        void*,
        const void*);
    Predicate volatile notEqual = &CCharString_NotEqual_0099e900;
    unsigned char result = notEqual(&left, 0, &right);
    return result != expected || g_CompareCalls != expectedCompareCalls;
}

int main()
{
    TestStringStorage_0099e900 headerA = { "HEADER", 6 };
    TestStringStorage_0099e900 headerB = { "HEADER", 6 };
    TestStringStorage_0099e900 player = { "PLAYER", 6 };
    TestStringStorage_0099e900 shortName = { "HEAD", 4 };

    int failures = 0;
    failures += Check(&headerA, &headerA, 0, 0);
    failures += Check(&headerA, &headerB, 0, 1);
    failures += Check(&headerA, &player, 1, 1);
    failures += Check(&headerA, &shortName, 1, 0);
    failures += Check(&headerA, 0, 1, 0);
    failures += Check(0, 0, 0, 0);

    if (failures != 0)
    {
        std::printf(
            "CHAR_STRING_NOT_EQUAL_0099e900_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("CHAR_STRING_NOT_EQUAL_0099e900_TEST PASS\n");
    return 0;
}
