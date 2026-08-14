#include <stdio.h>

struct CComponentPtr { void* p; };

struct CSubList {
    int callCount;
    void* lastArg;
    void Push(CComponentPtr* c) { callCount++; lastArg = c; }
};

struct CSpellContainerList {
    CSubList listA;
    CSubList listB;
    void AddChild(CComponentPtr* c) {
        this->listA.Push(c);
        this->listB.Push(c);
    }
};

int main() {
    CSpellContainerList lst;
    lst.listA.callCount = 0; lst.listA.lastArg = 0;
    lst.listB.callCount = 0; lst.listB.lastArg = 0;

    CComponentPtr node; node.p = (void*)0xDEAD;
    CComponentPtr* pnode = &node;
    lst.AddChild(pnode);

    bool ok = lst.listA.callCount == 1 && lst.listB.callCount == 1
           && lst.listA.lastArg == (void*)pnode
           && lst.listB.lastArg == (void*)pnode;

    if (ok) printf("PARITY_OK_AddChild\n");
    else printf("FAIL\n");
    return 0;
}