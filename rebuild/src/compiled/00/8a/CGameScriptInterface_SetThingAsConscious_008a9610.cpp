class CScriptThing;
class CCharString;
class CGameScriptInterface { public: void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const; };
extern "C" void ConstructConsciousAction_008ae1b0();
extern "C" void ConstructUnconsciousAction_008eda90();
extern "C" void CreatureSetCurrentAction_006644f0();
extern "C" void DestroyTemporaryAction_0062e900();
void __declspec(naked) CGameScriptInterface::SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const {
 __asm {
  sub esp, 118h
  push esi
  mov esi, dword ptr [esp+120h]
  mov eax, dword ptr [esi]
  mov ecx, esi
  call dword ptr [eax+12ch]
  test al, al
  je done
  mov edx, dword ptr [esi]
  mov ecx, esi
  call dword ptr [edx+2ch]
  mov esi, eax
  test esi, esi
  je done
  test byte ptr [esi+91h], 1
  jne done
  test byte ptr [esi+6ch], 8
  je done
  test dword ptr [esi+24h], 20000h
  je done
  mov al, byte ptr [esp+124h]
  test al, al
  je unconscious
  push 0
  push esi
  lea ecx, [esp+0ch]
  call ConstructConsciousAction_008ae1b0
  push eax
  mov ecx, esi
  call CreatureSetCurrentAction_006644f0
  lea ecx, [esp+4]
  call DestroyTemporaryAction_0062e900
  pop esi
  add esp, 118h
  ret 0ch
 unconscious:
  mov eax, dword ptr [esp+128h]
  push eax
  push 0
  push esi
  lea ecx, [esp+10h]
  call ConstructUnconsciousAction_008eda90
  push eax
  mov ecx, esi
  call CreatureSetCurrentAction_006644f0
  lea ecx, [esp+4]
  call DestroyTemporaryAction_0062e900
 done:
  pop esi
  add esp, 118h
  ret 0ch
 }
}