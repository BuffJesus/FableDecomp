class CGameScriptInterface { public: bool GetHeroHasMarried() const; };
extern "C" void HeroResolveStage1_00449970();
extern "C" void HeroResolveStage2_00487dc0();
extern "C" void InterfaceMapLowerBound_004365b0();
bool __declspec(naked) CGameScriptInterface::GetHeroHasMarried() const {
 __asm {
  push ecx
  mov ecx, dword ptr [ecx+14h]
  call HeroResolveStage1_00449970
  mov ecx, eax
  call HeroResolveStage2_00487dc0
  test eax, eax
  je no_marriage
  test byte ptr [eax+91h], 1
  jne no_marriage
  test byte ptr [eax+20h], 10h
  je no_marriage
  push esi
  lea esi, [eax+44h]
  lea eax, [esp+4]
  push eax
  mov ecx, esi
  mov dword ptr [esp+8], 4
  call InterfaceMapLowerBound_004365b0
  mov esi, dword ptr [esi+4]
  cmp eax, esi
  je use_end
  cmp dword ptr [eax], 4
  jle have_entry
 use_end:
  mov eax, esi
 have_entry:
  mov eax, dword ptr [eax+4]
  mov eax, dword ptr [eax+1d8h]
  mov edx, dword ptr [eax]
  xor ecx, ecx
  cmp edx, eax
  setne cl
  mov al, cl
  pop esi
  pop ecx
  ret
 no_marriage:
  xor al, al
  pop ecx
  ret
 }
}