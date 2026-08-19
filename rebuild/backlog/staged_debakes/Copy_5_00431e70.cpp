// One-line forwarder that VC7.1 tail-calls (`jmp rel32`): void member -> void member,
// identical signature, so the frame is reused. __fastcall this=ecx, src=stack.
struct CControlsDef {
    void Copy(const CControlsDef* src);
    void CopyImpl(const CControlsDef* src);
};
void CControlsDef::Copy(const CControlsDef* src) { this->CopyImpl(src); }
