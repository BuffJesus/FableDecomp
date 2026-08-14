// CTCInventoryBase::GetSelectedCategory @ 0x007b8570

struct CategoryVec {
    unsigned char* m_begin; // +0x00 (struct placed at +0x34)
    unsigned char* m_end;   // +0x04

    unsigned int size() const { return (unsigned int)(m_end - m_begin); }
    unsigned char at(unsigned int i) const { return m_begin[i]; }
};

struct CTCInventoryBase {
    char _pad[0x34];
    CategoryVec m_cats; // +0x34
};

unsigned char __fastcall GetSelectedCategory(CTCInventoryBase* self, int /*edx*/, unsigned int index)
{
    if (self->m_cats.size() > index)
        return self->m_cats.at(index);
    return 0;
}