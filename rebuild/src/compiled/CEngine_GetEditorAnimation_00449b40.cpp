extern "C" void *ResolveEditorAnimation(void *);
extern "C" void *FetchAnimationInfo(void *);
extern "C" __declspec(naked) void *GetEditorAnimation(void *, void *) {
    __asm { mov eax, dword ptr [esp+4] }
    __asm { push eax }
    __asm { call ResolveEditorAnimation }
    __asm { mov ecx, eax }
    __asm { call FetchAnimationInfo }
    __asm { ret 4 }
}