// Retail 0x00993E50. Virtual dispatch on `this` at vftable offset 0x10 (slot 4)
// with the argument's own stack slot as the source and a length of 1.
struct CDataStream {
    virtual void VSlot0() = 0;
    virtual void VSlot1() = 0;
    virtual void VSlot2() = 0;
    virtual void VSlot3() = 0;
    virtual void WriteRaw(const void* pSource, int size) = 0;
    void WriteCBYTE(char value);
};

void CDataStream::WriteCBYTE(char value) {
    WriteRaw(&value, 1);
}