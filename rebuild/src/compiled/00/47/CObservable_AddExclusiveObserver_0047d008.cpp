// NUISystem::CObservable::AddExclusiveObserver @ 0x0047d008
//   mov eax,[esp+4] ; mov [ecx+0x64],eax ; ret 4
// __fastcall setter: this=ecx, one stack arg -> field at +0x64.
__declspec(naked) void AddExclusiveObserver()
{
	__asm {
		mov eax, dword ptr [esp+4]
		mov dword ptr [ecx+0x64], eax
		ret 4
	}
}