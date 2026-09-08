// Retail 0x00A62A60. These are the original CPackedUIntArray member names
// and offsets recovered from Ego_r.pdb.
struct CPackedUIntArray {
    unsigned long* PackedInts;
    unsigned long Size;
    unsigned char Bits;
    unsigned char _alignment[3];
    unsigned long Bias;
    unsigned long Sizeof() const;
};

unsigned long CPackedUIntArray::Sizeof() const {
    const unsigned long packedBitCount = Size * Bits;
    const unsigned long packedWordCount = (packedBitCount + 31) >> 5;
    return sizeof(CPackedUIntArray) + packedWordCount * sizeof(unsigned long);
}