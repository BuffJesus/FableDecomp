// Behaviour test for the genuine CCharString::operator!=(const CCharString&)
// (retail 0x0099E900): shared storage compares equal, a null side differs,
// different lengths differ without comparing, otherwise the characters
// decide. The strings are raw storage pointers laid out as CCharStringData's
// leading CBasicString<char> (text, length), which is all the comparison reads.
#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CCharString_NotEqual_0099e900.cpp"

struct TestStorage
{
    const char* text;
    unsigned long length;
};

struct TestString
{
    TestStorage* storage;
};

static bool NotEqual(TestStorage* left, TestStorage* right)
{
    TestString a = {left};
    TestString b = {right};
    return reinterpret_cast<const CCharString&>(a) != reinterpret_cast<const CCharString&>(b);
}

int main()
{
    TestStorage abc = {"abc", 3};
    TestStorage abc2 = {"abc", 3};
    TestStorage abd = {"abd", 3};
    TestStorage abcd = {"abcd", 4};
    int failures = 0;
    failures += NotEqual(&abc, &abc) != false;   // shared storage
    failures += NotEqual(0, 0) != false;         // both empty
    failures += NotEqual(&abc, 0) != true;       // one empty
    failures += NotEqual(0, &abc) != true;
    failures += NotEqual(&abc, &abcd) != true;   // lengths differ
    failures += NotEqual(&abc, &abc2) != false;  // same characters
    failures += NotEqual(&abc, &abd) != true;    // characters differ
    if (failures != 0)
    {
        std::printf("CHAR_STRING_NOT_EQUAL_0099e900_TEST FAIL %d\n", failures);
        return 1;
    }
    std::printf("CHAR_STRING_NOT_EQUAL_0099e900_TEST PASS\n");
    return 0;
}
