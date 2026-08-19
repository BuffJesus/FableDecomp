// CTCCarryable::OnKill @ 0047c880  (51 bytes)
// __fastcall, void, modeled as __fastcall(this in ecx).
//
// Retail idiom:
//   esi = this
//   if (this->m4 != 0) {
//       this->Detach( this->m0->f4 );   // __fastcall, one int arg
//       this->m0->f8 = this->m0;        // reloads this->m0 each store
//       this->m0->f4 = 0;
//       this->m0->fc = this->m0;
//       this->m4 = 0;
//   }

struct CTCCarryableNode {
    void*        f0;   /* +0x0 */
    unsigned int f4;   /* +0x4 */
    void*        f8;   /* +0x8 */
    void*        fc;   /* +0xc */
};

struct CTCCarryable {
    CTCCarryableNode* m0;   /* +0x0 */
    unsigned int      m4;   /* +0x4 */
    void Detach(unsigned int a);   /* masked __fastcall callee */
};

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->m4 != 0) {
        self->Detach(self->m0->f4);
        self->m0->f8 = self->m0;
        self->m0->f4 = 0;
        self->m0->fc = self->m0;
        self->m4 = 0;
    }
}