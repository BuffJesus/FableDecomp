class CGameScriptInterface { public: void RemoveHeroHairstyle() const; };
extern "C" void HeroResolveStage1_00449970();
extern "C" void HeroResolveStage2_00487dc0();
extern "C" void InterfaceMapLowerBound5E_0040f020();
extern "C" void RemoveAllAppearanceModifiers_00706640();
void __declspec(naked) CGameScriptInterface::RemoveHeroHairstyle() const {
 __asm {
  push ecx
  mov ecx, dword ptr [ecx+14h]
  call HeroResolveStage1_00449970
  mov ecx, eax
  call HeroResolveStage2_00487dc0
  test eax, eax
  je done
  test byte ptr [eax+91h], 1
  jne done
  test dword ptr [eax+28h], 40000000h
  je done
  push esi
  lea esi, [eax+44h]
  lea eax, [esp+4]
  push eax
  mov ecx, esi
  mov dword ptr [esp+8], 5eh
  call InterfaceMapLowerBound5E_0040f020
  mov esi, dword ptr [esi+4]
  cmp eax, esi
  je use_end
  cmp dword ptr [eax], 5eh
  jle have_entry
 use_end:
  mov eax, esi
 have_entry:
  mov eax, dword ptr [eax+4]
  push 0
  mov ecx, eax
  call RemoveAllAppearanceModifiers_00706640
  pop esi
 done:
  pop ecx
  ret
 }
}