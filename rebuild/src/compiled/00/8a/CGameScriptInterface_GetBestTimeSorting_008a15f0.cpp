class CGameScriptInterface { public: float GetBestTimeSorting() const; };
extern "C" void HeroResolveStage1_00449970();
extern "C" void HeroResolveStage2_00487de0();
extern "C" void InterfaceMapLowerBound_00436570();
extern "C" float DefaultBestTime_0122dedc;
float __declspec(naked) CGameScriptInterface::GetBestTimeSorting() const { __asm {
push ecx
mov ecx,dword ptr [ecx+14h]
call HeroResolveStage1_00449970
mov ecx,eax
call HeroResolveStage2_00487de0
test eax,eax
je fallback
test byte ptr [eax+91h],1
jne fallback
test byte ptr [eax+20h],10h
je fallback
push esi
lea esi,[eax+44h]
lea eax,[esp+4]
push eax
mov ecx,esi
mov dword ptr [esp+8],4
call InterfaceMapLowerBound_00436570
mov esi,dword ptr [esi+4]
cmp eax,esi
je use_end
cmp dword ptr [eax],4
jle have_entry
use_end:
mov eax,esi
have_entry:
mov eax,dword ptr [eax+4]
fld dword ptr [eax+17ch]
pop esi
pop ecx
ret
fallback:
fld dword ptr [DefaultBestTime_0122dedc]
pop ecx
ret
} }