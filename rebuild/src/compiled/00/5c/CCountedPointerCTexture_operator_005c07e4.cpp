#pragma optimize("s",on)
// CCountedPointer<CTexture>::operator=
// Retail 0x005c07e4. Assigns from another counted pointer by forwarding the
// rhs's two members (m0, m4) to an internal assign helper, returns this.

struct CCountedPointer_CTexture {
    void* m0;
    void* m4;

    // helper at 0x5c053d: __fastcall taking the two members by value.
    void Assign(void* a, void* b);

    CCountedPointer_CTexture* operator=(const CCountedPointer_CTexture& rhs);
};

CCountedPointer_CTexture* CCountedPointer_CTexture::operator=(const CCountedPointer_CTexture& rhs)
{
    this->Assign(rhs.m0, rhs.m4);
    return this;
}