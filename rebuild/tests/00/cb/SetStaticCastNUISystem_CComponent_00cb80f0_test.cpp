#include <cstdio>

struct CComponent {};
struct CList {};

struct Holder
{
    CList*      m_first;
    CComponent* m_second;

    CList*      seen_a;
    CComponent* seen_b;
    int         finalized;

    void Assign(CList* a, CComponent* b) { seen_a = a; seen_b = b; }
    void Finalize() { finalized = 1; }

    void SetStaticCast()
    {
        Assign(m_first, m_second);
        Finalize();
    }
};

int main()
{
    CList l;
    CComponent c;
    Holder h;
    h.m_first = &l;
    h.m_second = &c;
    h.seen_a = 0;
    h.seen_b = 0;
    h.finalized = 0;

    h.SetStaticCast();

    bool ok = (h.seen_a == &l) && (h.seen_b == &c) && (h.finalized == 1);
    if (ok) { std::printf("SETSTATICCAST_OK\n"); return 0; }
    std::printf("SETSTATICCAST_FAIL\n");
    return 1;
}