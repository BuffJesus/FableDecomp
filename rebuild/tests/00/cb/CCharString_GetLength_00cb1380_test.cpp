#include <stdio.h>

struct Node {
    Node* f0;
    Node* f4;
    long  f8;
};

struct CCharString {
    Node* f0;
};

long __stdcall GetLength(CCharString* self);

int main()
{
    // null case
    CCharString a;
    a.f0 = 0;
    long r0 = GetLength(&a);

    // non-null case
    Node n2;
    n2.f0 = 0;
    n2.f4 = (Node*)0x1234;
    n2.f8 = 77;
    Node n1;
    n1.f0 = 0;
    n1.f4 = &n2;
    n1.f8 = 42;
    CCharString b;
    b.f0 = &n1;
    long r1 = GetLength(&b);

    bool ok = (r0 == 0) && (r1 == 42) && (b.f0 == &n2);
    if (ok)
        printf("00cb1380_TEST PASS\n");
    else
        printf("FAIL r0=%ld r1=%ld advanced=%d\n", r0, r1, (int)(b.f0 == &n2));
    return 0;
}