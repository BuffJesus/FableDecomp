// CEngineVSConstantLayoutParticles::PeekName  @ 0x00bdb040
// Retail:
//   push esi
//   mov  esi,[esp+8]        ; hidden return-buffer ptr (this shifted by push)
//   push -1
//   push 0x12a7cd0          ; const char* literal
//   mov  ecx,esi
//   call CCharString::CCharString(const char*,int)   @ 0x99ebf0
//   mov  eax,esi            ; return the buffer
//   pop  esi
//   ret  4
//
// i.e.  CCharString PeekName() { return CCharString("...", -1); }

class CCharString {
public:
    // Constructs a string from a literal with an explicit length (-1 = strlen).
    CCharString(const char* s, int len);
private:
    void* m_data;
};

class CEngineVSConstantLayoutParticles {
public:
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutParticles::PeekName()
{
    return CCharString("engineParticleVSConstantLayout", -1);
}