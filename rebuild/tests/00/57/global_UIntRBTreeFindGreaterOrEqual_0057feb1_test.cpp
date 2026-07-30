#include <stdio.h>

struct RBNode {
    unsigned int _pad0;   // 0x00
    RBNode* parent;       // 0x04
    RBNode* left;         // 0x08
    RBNode* right;        // 0x0C
    unsigned int key;     // 0x10
};

struct RBTree {
    RBNode* header;       // 0x00
};

RBNode* __fastcall UInt_RBTreeFindGreaterOrEqual(RBTree* self, int /*edx*/, unsigned int* key)
{
    RBNode* result = self->header;
    RBNode* node = result->parent;
    while (node != 0) {
        if (node->key >= *key) {
            result = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return result;
}

static RBNode* call_find(RBTree* self, unsigned int* key)
{
    RBNode* ret;
    __asm {
        mov  ecx, self
        push key
        call UInt_RBTreeFindGreaterOrEqual
        mov  ret, eax
    }
    return ret;
}

int main()
{
    RBNode H, R, A, B;
    H._pad0=R._pad0=A._pad0=B._pad0=0;
    H.parent=H.left=H.right=0; H.key=0;
    R.parent=R.left=R.right=0; R.key=0;
    A.parent=A.left=A.right=0; A.key=0;
    B.parent=B.left=B.right=0; B.key=0;

    // Build tree: root R(20), R.left=A(10), R.right=B(30)
    R.key=20; A.key=10; B.key=30;
    R.left=&A; R.right=&B;
    H.parent=&R;   // header->parent = root

    RBTree t; t.header=&H;

    unsigned int k15=15; RBNode* r1=call_find(&t,&k15); // ->R(20)
    unsigned int k10=10; RBNode* r2=call_find(&t,&k10); // ->A(10)
    unsigned int k25=25; RBNode* r3=call_find(&t,&k25); // ->B(30)
    unsigned int k35=35; RBNode* r4=call_find(&t,&k35); // ->H (none)
    unsigned int k5=5;   RBNode* r5=call_find(&t,&k5);  // ->A(10)

    bool ok = (r1==&R) && (r2==&A) && (r3==&B) && (r4==&H) && (r5==&A);
    if (!ok) {
        printf("FAIL r1=%p r2=%p r3=%p r4=%p r5=%p\n",
            (void*)r1,(void*)r2,(void*)r3,(void*)r4,(void*)r5);
        return 1;
    }
    printf("OK_0x0057feb1\n");
    return 0;
}