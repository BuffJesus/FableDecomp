// CActionDoCreatureAction::GetActionName @ 0x006922a0
// Stack-return-pointer variant of the action-name factory. The hidden
// CCharString result pointer arrives as the single stdcall stack argument,
// the body constructs it from a relocated string constant with the length
// sentinel -1, then returns that pointer. Shape:
//   push esi; mov esi,[esp+8]; push -1; push STR; mov ecx,esi; call ctor;
//   mov eax,esi; pop esi; ret 4.
struct FableCharStringResult_006922a0
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_006922a0* __stdcall
FableGetActionName_006922a0(FableCharStringResult_006922a0* self)
{
    self->build((const char*)0x0125c18c, -1);
    return self;
}