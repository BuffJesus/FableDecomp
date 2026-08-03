// CChunkCollectionIndicator::OnPredicateFail  @ 0x005d8e50  __fastcall (this=ecx)
// Byte-exact transcription:
//   8bc1              mov eax,ecx
//   8b4810            mov ecx,[eax+0x10]     ; target = this->m_target
//   8b4004            mov eax,[eax+0x4]      ; arg    = this->m_arg
//   8b11              mov edx,[ecx]          ; vtbl   = target->vtbl
//   50                push eax               ; arg
//   ff924c010000      call [edx+0x14c]       ; vtbl slot 0x14c (__fastcall this=ecx=target)
//   c3                ret
__declspec(naked) void __fastcall OnPredicateFail(void* this_) {
    (void)this_;
    __asm {
        mov eax, ecx
        mov ecx, [eax+0x10]
        mov eax, [eax+0x4]
        mov edx, [ecx]
        push eax
        call dword ptr [edx+0x14c]
        ret
    }
}