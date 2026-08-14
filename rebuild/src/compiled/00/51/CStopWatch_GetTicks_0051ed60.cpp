// Byte-exact reconstruction of CStopWatch::GetTicks @ 0x0051ed60
// Return is a 32-bit value in eax (edx untouched); ret 8 => 8-byte by-value key arg,
// whose stack slot is reused as the iterator scratch buffer.
// Forwards to Map::Find on subobject at this+0x58 (callee 0x527d80),
// compares the returned node to the map head sentinel [*map], returns node->[0x18] or 0.

struct Node {
    Node* left;         // +0
    char  pad4[0x14];
    unsigned int value; // +0x18
};

struct Map {
    Node* head;         // +0
    // thiscall member; no __fastcall keyword needed (default for members).
    Node* Find(void* iterOut);
};

struct CStopWatch {
    char pad[0x58];
    Map  themap;        // +0x58
};

// 8-byte by-value key => ret 8. Return is 32-bit (eax only).
unsigned int __fastcall GetTicks(CStopWatch* self, void* edx, __int64 key)
{
    Map* m = &self->themap;
    Node* n = m->Find(&key);
    if (n != m->head)
        return n->value;
    return 0;
}