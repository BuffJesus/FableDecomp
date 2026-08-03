struct CEditInputProcessCopyPaste {
    char pad[0x10];
    unsigned char m_flag; /* +0x10 */
};

/* external base implementation, tail-called */
void __fastcall AddToInterfaceBase(CEditInputProcessCopyPaste* self);

void __fastcall AddToInterface(CEditInputProcessCopyPaste* self)
{
    self->m_flag = 0;            /* mov byte ptr [ecx+0x10], 0 */
    AddToInterfaceBase(self);    /* jmp <base>  (tail call)   */
}