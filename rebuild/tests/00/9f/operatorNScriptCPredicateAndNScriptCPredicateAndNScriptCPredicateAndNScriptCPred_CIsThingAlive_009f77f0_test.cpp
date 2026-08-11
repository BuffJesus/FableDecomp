#include <cstdio>

struct CPredicateAndBuffer {
    void Construct_41c780(void *lhs);
    CPredicateAndBuffer *operator_andand_CIsThingAlive_(void *lhs);
};

// Local stand-in for the external ctor at 0x41c780: capture what it received.
static CPredicateAndBuffer *g_seen_this = 0;
static void               *g_seen_lhs  = 0;

void CPredicateAndBuffer::Construct_41c780(void *lhs)
{
    g_seen_this = this;
    g_seen_lhs  = lhs;
}

CPredicateAndBuffer *CPredicateAndBuffer::operator_andand_CIsThingAlive_(void *lhs)
{
    this->Construct_41c780(lhs);
    return this;
}

int main()
{
    CPredicateAndBuffer buf;
    int lhsDummy = 0x1234;
    void *lhsPtr = &lhsDummy;

    CPredicateAndBuffer *r = buf.operator_andand_CIsThingAlive_(lhsPtr);

    bool ok = (r == &buf)                 // returns the buffer (this)
           && (g_seen_this == &buf)       // ctor ran on the buffer
           && (g_seen_lhs  == lhsPtr);    // lhs forwarded intact

    if (ok) {
        std::printf("PRED_ANDAND_OK\n");
        return 0;
    }
    std::printf("PRED_ANDAND_FAIL\n");
    return 1;
}