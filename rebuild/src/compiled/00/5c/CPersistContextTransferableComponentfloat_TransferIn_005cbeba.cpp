#pragma optimize("s",on)
// CPersistContext_TransferableComponent<float>::TransferIn  @ 005cbe9d
// __fastcall void TransferIn(CPersistContext*, CStringParser*)

class CPersistContext;
class CStringParser;

// Helper temp object constructed on the stack (single dword, zero-inited),
// then a __fastcall method invoked on it with (persist, parser, value).
struct TransferHelper {
    unsigned int m_dword;
    // out-of-line __fastcall method -> emitted as a direct call
    void Apply(CPersistContext* ctx, CStringParser* parser, unsigned int value);
};

struct CPersistContext_TransferableComponent_float {
    void* m_vtbl;             // +0
    unsigned int m_value;     // +4  (raw 32-bit float bits, pushed as dword)
    void TransferIn(CPersistContext* ctx, CStringParser* parser);
};

void CPersistContext_TransferableComponent_float::TransferIn(CPersistContext* ctx, CStringParser* parser)
{
    TransferHelper tmp;
    tmp.m_dword = 0;
    tmp.Apply(ctx, parser, this->m_value);
}