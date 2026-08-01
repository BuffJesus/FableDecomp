#include <cstddef>
#include <cstdio>
#include <cstring>

namespace NUISystem
{
class CManager
{
public:
    enum EUsedKeys
    {
        EUsedKeys_dummy = 0
    };
};
}

// This toolchain's std::_Tree node layout (verified empirically):
//   _Left @0x00, _Parent @0x04, _Right @0x08, key(first) @0x0C.
// The map object holds the comparator at +0x00 and _Myhead at +0x04.
struct Node
{
    Node* pLeft;   // 0x00
    Node* pParent; // 0x04
    Node* pRight;  // 0x08
    long usedKey;  // 0x0C
};
struct FakeMap
{
    void* comparator; // 0x00
    Node* pHead;      // 0x04  (_Myhead sentinel)
    unsigned size;    // 0x08  (_Mysize)
};

// The retail walk (identical algorithm to source_cpp), returning max key + 1.
// pHead is the sentinel; leaf links point back to the sentinel (never null in
// a real std::map), so a plain sentinel-terminated in-order walk is faithful.
long RegisterTimer_walk(FakeMap* pMap)
{
    Node* pHead = pMap->pHead;
    long highestUsedKey = 0;
    Node* pNode = pHead->pLeft;
    while (pNode != pHead)
    {
        if (pNode->usedKey > highestUsedKey) highestUsedKey = pNode->usedKey;
        if (pNode->pRight != pHead)
        {
            pNode = pNode->pRight;
            while (pNode->pLeft != pHead) pNode = pNode->pLeft;
        }
        else
        {
            Node* pScratch = pNode->pParent;
            while (pScratch != pHead && pNode == pScratch->pRight)
            {
                pNode = pScratch;
                pScratch = pNode->pParent;
            }
            pNode = pScratch;
        }
    }
    return highestUsedKey + 1;
}

// Build a small balanced BST over the given sorted keys into a node pool with
// this toolchain's layout, with a sentinel head. Leaf child links point to the
// sentinel. Returns the sentinel via *outMap. No heap, no std::map, no EH.
static Node g_pool[64];
static Node g_head;

static Node* build(long* keys, int lo, int hi, Node* parent, int* next)
{
    if (lo > hi) return &g_head; // sentinel as nil
    int mid = (lo + hi) / 2;
    Node* n = &g_pool[(*next)++];
    n->usedKey = keys[mid];
    n->pParent = parent;
    n->pLeft = build(keys, lo, mid - 1, n, next);
    n->pRight = build(keys, mid + 1, hi, n, next);
    return n;
}

static Node* leftmost(Node* n)
{
    while (n->pLeft != &g_head) n = n->pLeft;
    return n;
}
static Node* rightmost(Node* n)
{
    while (n->pRight != &g_head) n = n->pRight;
    return n;
}

static long run(long* keys, int count)
{
    memset(g_pool, 0, sizeof(g_pool));
    int next = 0;
    FakeMap m;
    m.comparator = 0;
    m.pHead = &g_head;
    m.size = (unsigned)count;
    if (count == 0)
    {
        g_head.pLeft = &g_head;
        g_head.pParent = &g_head;
        g_head.pRight = &g_head;
    }
    else
    {
        Node* root = build(keys, 0, count - 1, &g_head, &next);
        g_head.pParent = root;                 // _Myhead->_Parent = root
        g_head.pLeft = leftmost(root);          // _Myhead->_Left  = begin
        g_head.pRight = rightmost(root);        // _Myhead->_Right = last
    }
    return RegisterTimer_walk(&m);
}

int main()
{
    long k1[] = { 3, 5, 7 };
    long r1 = run(k1, 3);

    long r2 = run((long*)0, 0);

    long k3[] = { 1, 2, 4, 9 };
    long r3 = run(k3, 4);

    long k4[] = { 10, 20, 30, 40, 50, 60, 70 };
    long r4 = run(k4, 7);

    bool ok = (r1 == 8) && (r2 == 1) && (r3 == 10) && (r4 == 71);

    if (ok) printf("REGTIMER_OK r1=%ld r2=%ld r3=%ld r4=%ld\n", r1, r2, r3, r4);
    else printf("REGTIMER_FAIL r1=%ld r2=%ld r3=%ld r4=%ld\n", r1, r2, r3, r4);
    return ok ? 0 : 1;
}