#include <stdio.h>

struct RBNode {
    char pad[8];
    RBNode* left;
    RBNode* right;
    int key;
};
struct RBTree { RBNode* head; };

RBNode* __fastcall RBTree_FindLE_Reverse(RBTree* self, void* /*edx*/, int* pkey)
{
    RBNode* header = self->head;
    RBNode* result = header;
    RBNode* cur = *(RBNode**)((char*)header + 4);
    if (cur != 0) {
        int key = *pkey;
        do {
            if (cur->key >= key) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        } while (cur != 0);
    }
    return result;
}

static RBNode* mknode(int k) {
    RBNode* n = new RBNode();
    n->left = 0; n->right = 0; n->key = k;
    return n;
}

static RBNode* call_find(RBTree* t, int* pkey) {
    RBNode* r;
    __asm {
        mov  ecx, t
        mov  eax, pkey
        push eax
        call RBTree_FindLE_Reverse
        mov  r, eax
    }
    return r;
}

int main() {
    RBNode* n50 = mknode(50);
    RBNode* n30 = mknode(30);
    RBNode* n70 = mknode(70);
    RBNode* n20 = mknode(20);
    RBNode* n40 = mknode(40);
    n50->left = n30; n50->right = n70;
    n30->left = n20; n30->right = n40;

    RBNode* header = new RBNode();
    header->left = 0; header->right = 0; header->key = 0;
    *(RBNode**)((char*)header + 4) = n50;

    RBTree tree; tree.head = header;

    int k;
    k = 35; if (RBTree_FindLE_Reverse(&tree, 0, &k) != n40) { printf("FAIL a\n"); return 1; }
    k = 30; if (RBTree_FindLE_Reverse(&tree, 0, &k) != n30) { printf("FAIL b\n"); return 1; }
    k = 50; if (RBTree_FindLE_Reverse(&tree, 0, &k) != n50) { printf("FAIL c\n"); return 1; }
    k = 15; if (RBTree_FindLE_Reverse(&tree, 0, &k) != n20) { printf("FAIL d\n"); return 1; }
    k = 100; if (RBTree_FindLE_Reverse(&tree, 0, &k) != header) { printf("FAIL e\n"); return 1; }
    k = 65; if (RBTree_FindLE_Reverse(&tree, 0, &k) != n70) { printf("FAIL f\n"); return 1; }

    k = 40; if (call_find(&tree, &k) != n40) { printf("FAIL shim\n"); return 1; }

    printf("OK_0x005bad11\n");
    return 0;
}