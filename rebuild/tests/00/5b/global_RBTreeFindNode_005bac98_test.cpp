#include <cstdio>

struct Node {
    Node* pad0;
    Node* pad4;
    Node* left;
    Node* right;
    int   key;
};

struct Tree {
    Node* head;
    Node* RBTree_FindNode(const int* pKey);
};

Node* Tree::RBTree_FindNode(const int* pKey)
{
    Node* result = this->head;
    Node* node   = result->pad4;
    if (node != 0) {
        int k = *pKey;
        do {
            if (node->key >= k) {
                result = node;
                node = node->left;
            } else {
                node = node->right;
            }
        } while (node != 0);
    }
    return result;
}

int main()
{
    //          20
    //         /  \
    //       10    30
    //      /  \
    //     5   15
    Node n5, n10, n15, n20, n30, head;
    Node* NIL = 0;

    n5.left=NIL;  n5.right=NIL;  n5.key=5;
    n15.left=NIL; n15.right=NIL; n15.key=15;
    n10.left=&n5; n10.right=&n15; n10.key=10;
    n30.left=NIL; n30.right=NIL; n30.key=30;
    n20.left=&n10; n20.right=&n30; n20.key=20;

    head.pad0=0; head.pad4=&n20; head.left=0; head.right=0; head.key=0;

    Tree t; t.head=&head;

    int rc = 0;
    struct { int k; Node* exp; const char* name; } cases[] = {
        { 5,  &n5,  "exact-min" },
        { 10, &n10, "exact" },
        { 12, &n15, "between-10-15" },
        { 15, &n15, "exact-15" },
        { 20, &n20, "exact-20" },
        { 25, &n30, "between-20-30" },
        { 30, &n30, "exact-max" },
        { 31, &head,"past-max-returns-head" },
        { 0,  &n5,  "below-min" },
    };
    for (int i = 0; i < (int)(sizeof(cases)/sizeof(cases[0])); ++i) {
        int key = cases[i].k;
        Node* got = t.RBTree_FindNode(&key);
        if (got != cases[i].exp) {
            printf("FAIL case %s: key=%d got=%p exp=%p\n",
                   cases[i].name, key, (void*)got, (void*)cases[i].exp);
            rc = 1;
        }
    }

    Node ehead; ehead.pad4=0; ehead.pad0=0; ehead.left=0; ehead.right=0; ehead.key=0;
    Tree et; et.head=&ehead;
    int q = 7;
    if (et.RBTree_FindNode(&q) != &ehead) {
        printf("FAIL empty-tree\n");
        rc = 1;
    }

    if (rc == 0) {
        printf("OK_0x005bac98 all lower_bound cases passed\n");
        return 0;
    }
    return 1;
}