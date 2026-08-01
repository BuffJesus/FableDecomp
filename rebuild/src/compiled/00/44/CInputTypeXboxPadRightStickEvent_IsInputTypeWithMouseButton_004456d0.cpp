// CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton (0x004456d0)
// Retail: 32 c0 c2 04 00  => xor al,al ; ret 4  (returns false)
bool __fastcall candidate_004456d0(void *this_ptr, void *edx, int mouseButton)
{
    (void)this_ptr; (void)edx; (void)mouseButton;
    return false;
}
