#include <cstdio>

struct CResource {
    char pad[0xc];
    CResource* a;   // +0xc
    CResource* b;   // +0x10
    void ListInsert(CResource* p1, CResource* p2);
};

void CResource::ListInsert(CResource* p1, CResource* p2)
{
    this->a = p1;
    this->b = p2;
    p1->b = this;
    this->b->a = this;
}

int main()
{
    CResource n, p1, p2;
    n.a = n.b = 0;
    p1.a = p1.b = 0;
    p2.a = p2.b = 0;

    n.ListInsert(&p1, &p2);

    bool ok = (n.a == &p1) && (n.b == &p2) &&
              (p1.b == &n) && (p2.a == &n);
    if (ok) {
        printf("LISTINSERT_OK\n");
        return 0;
    }
    printf("LISTINSERT_FAIL\n");
    return 1;
}