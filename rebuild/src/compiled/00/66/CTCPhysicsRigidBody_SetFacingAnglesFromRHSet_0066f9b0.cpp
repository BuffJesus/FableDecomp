extern "C" void __fastcall
CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0_SetAsMoving(
    void* physics, void*, long moving);

extern "C" __declspec(naked) void __fastcall
CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0(
    void* physics, void*, const void* right_handed_set)
{
    __asm
    {
        mov edx, [esp + 4]
        push esi
        mov esi, [edx]
        lea eax, [ecx + 28h]
        mov [eax], esi
        mov esi, [edx + 4]
        mov [eax + 4], esi
        mov edx, [edx + 8]
        mov [eax + 8], edx
        pop esi
        mov dword ptr [esp + 4], 1
        jmp CTCPhysicsRigidBody_SetFacingAnglesFromRHSet_0066f9b0_SetAsMoving
    }
}
