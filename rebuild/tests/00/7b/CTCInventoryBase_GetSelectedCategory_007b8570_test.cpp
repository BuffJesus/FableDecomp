#include <cstdio>

struct CategoryVec {
    unsigned char* m_begin;
    unsigned char* m_end;
    unsigned int size() const { return (unsigned int)(m_end - m_begin); }
    unsigned char at(unsigned int i) const { return m_begin[i]; }
};

struct Inv {
    char _pad[0x34];
    CategoryVec m_cats;
};

unsigned char __fastcall GetSelectedCategory(Inv* self, int, unsigned int index)
{
    if (self->m_cats.size() > index)
        return self->m_cats.at(index);
    return 0;
}

int main()
{
    unsigned char data[4] = { 10, 20, 30, 40 };
    Inv inv;
    inv.m_cats.m_begin = data;
    inv.m_cats.m_end = data + 4;

    int ok = 1;
    if (GetSelectedCategory(&inv, 0, 0) != 10) ok = 0;
    if (GetSelectedCategory(&inv, 0, 3) != 40) ok = 0;
    if (GetSelectedCategory(&inv, 0, 4) != 0) ok = 0;   // out of bounds
    if (GetSelectedCategory(&inv, 0, 999) != 0) ok = 0; // out of bounds

    inv.m_cats.m_end = data; // empty
    if (GetSelectedCategory(&inv, 0, 0) != 0) ok = 0;

    if (ok) printf("BEHAVIOR_OK\n");
    else printf("BEHAVIOR_FAIL\n");
    return 0;
}