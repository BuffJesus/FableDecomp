// Byte-exact reconstruction of the compiler-generated pair destructor at 008fd2ca.
// Layout: member A at +0 (CCharString first of the pair), member B at +0x14,
// member C at +0x30.  Compiler destroys in reverse declared order:
//   ~C (+0x30), ~B (+0x14), then tail-call ~A (+0).

struct MemberA {   // occupies offset 0, size 0x14
    int a0, a4, a8, ac, a10;
    ~MemberA();
};

struct MemberB {   // occupies offset 0x14, size 0x1c
    int b0, b4, b8, bc, b10, b14, b18;
    ~MemberB();
};

struct MemberC {   // occupies offset 0x30
    int c0, c4, c8;
    ~MemberC();
};

struct Pair {
    MemberA a;   // +0x00
    MemberB b;   // +0x14
    MemberC c;   // +0x30
    ~Pair();
};

Pair::~Pair() {}

// The three member dtors are out-of-line (defined in another TU) so the
// compiler must emit real calls in reverse order, with the last (a, at +0)
// becoming a tail jmp.