// Retail 0x004159E0. `this` reaches CMemoryBuffer::Copy (0x009F1760) unmodified,
// so CGameEvent derives from CMemoryBuffer at offset 0. The byte argument is appended
// by copying one byte from its own stack slot.
struct CMemoryBuffer {
    void Copy(int size, const void* pSource);
};

struct CGameEvent : CMemoryBuffer {
    void AddUBYTE(unsigned char value);
};

void CGameEvent::AddUBYTE(unsigned char value) {
    Copy(1, &value);
}