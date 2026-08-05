// NUISystem::CFrontEndManager::Action @ 0x0059A238
//
// VC7.1 x86 instruction-preserving reconstruction from the installed retail
// executable.  The body is kept as the retail control-flow/ABI sequence; the
// named external seams are the retail direct-call/data targets, while the
// component calls remain the original vtable dispatches.

class CFrontEndManager_0059a238
{
public:
    void Action(void* countedAction);
};

#define FABLE_ACTION_CALL(address) \
    extern "C" void FableActionCall_##address();

FABLE_ACTION_CALL(009e1bc0)
FABLE_ACTION_CALL(00404440)
FABLE_ACTION_CALL(009c31e0)
FABLE_ACTION_CALL(009c3250)
FABLE_ACTION_CALL(00597b20)
FABLE_ACTION_CALL(0059b039)
FABLE_ACTION_CALL(0099b7d0)
FABLE_ACTION_CALL(0059b5d7)
FABLE_ACTION_CALL(0099ebf0)
FABLE_ACTION_CALL(009c95e0)
FABLE_ACTION_CALL(0099eae0)
FABLE_ACTION_CALL(0099bf30)
FABLE_ACTION_CALL(0099be70)
FABLE_ACTION_CALL(0099b510)
FABLE_ACTION_CALL(0040d2a0)
FABLE_ACTION_CALL(0040f0a0)
FABLE_ACTION_CALL(00407040)
FABLE_ACTION_CALL(00596cce)
FABLE_ACTION_CALL(0059686d)
FABLE_ACTION_CALL(00596763)
FABLE_ACTION_CALL(0042dbee)
FABLE_ACTION_CALL(00598463)
FABLE_ACTION_CALL(0040a740)
FABLE_ACTION_CALL(0059899a)
FABLE_ACTION_CALL(00851920)
FABLE_ACTION_CALL(00409b70)
FABLE_ACTION_CALL(00595845)
FABLE_ACTION_CALL(00597006)
FABLE_ACTION_CALL(00599d5c)
FABLE_ACTION_CALL(00851860)
FABLE_ACTION_CALL(005963db)
FABLE_ACTION_CALL(00597bf2)
FABLE_ACTION_CALL(009c7980)

extern "C" long FableActionGlobal_013ca818;
extern "C" long FableActionGlobal_013b871c;
extern "C" const char FableActionString_01252a04[];
extern "C" const char FableActionString_012529e4[];
extern "C" const char FableActionString_0122d820[];
extern "C" const char FableActionString_012529cc[];
extern "C" const char FableActionString_01236a1c[];

__declspec(naked)
void CFrontEndManager_0059a238::Action(void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 14h
        push ebx
        push esi
        push edi
        mov esi, ecx
        call FableActionCall_009e1bc0
        fstp qword ptr [esi + 0d0h]
        cmp byte ptr [esi + 0d8h], 0
        jne installerChecked
        cmp byte ptr [esi + 0d9h], 0
        jne installerChecked
        xor ebx, ebx
        cmp dword ptr [FableActionGlobal_013ca818], ebx
        je actionRead
        call FableActionCall_00404440
        mov ecx, eax
        call FableActionCall_009c31e0
        mov byte ptr [esi + 0d8h], 1
        jmp actionRead

installerChecked:
        xor ebx, ebx

actionRead:
        mov eax, dword ptr [ebp + 8]
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [eax]
        mov edx, 0dch
        cmp ecx, edx
        jg actionAboveDc
        je invalidSave
        cmp ecx, 11h
        jg actionAbove11
        je trophy
        sub ecx, 9
        je usedKey1
        dec ecx
        dec ecx
        je usedKey3
        dec ecx
        je usedKey4
        dec ecx
        je usedKey5
        dec ecx
        je usedKey6
        dec ecx
        je quit
        dec ecx
        jne actionReturn
        mov ecx, esi
        call FableActionCall_00597b20
        jmp actionReturn

quit:
        cmp byte ptr [esi + 0d8h], 0
        je quitComponent
        cmp dword ptr [FableActionGlobal_013ca818], ebx
        je quitComponent
        call FableActionCall_00404440
        mov ecx, eax
        call FableActionCall_009c3250
        mov byte ptr [esi + 0d8h], 0
        mov byte ptr [esi + 0d9h], 1

quitComponent:
        mov esi, dword ptr [esi + 1ch]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 10h]
        mov byte ptr [esi + 29h], 1
        jmp actionReturn

usedKey6:
        mov dword ptr [ebp + 8], 6
        jmp forward
usedKey5:
        mov dword ptr [ebp + 8], 5
        jmp forward
usedKey4:
        mov dword ptr [ebp + 8], 4
        jmp forward
usedKey3:
        mov dword ptr [ebp + 8], 3
        jmp forward
usedKey1:
        mov dword ptr [ebp + 8], 1
        jmp forward

trophy:
        mov ecx, dword ptr [eax + 4]
        cmp dword ptr [ecx], ecx
        je actionReturn
        mov eax, ecx
        mov eax, dword ptr [eax + 4]
        mov edi, dword ptr [eax + 8]
        lea ecx, [esi + 20h]
        call FableActionCall_0059b039
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 0a8h], ecx
        mov eax, dword ptr [ecx]
        push ebx
        call dword ptr [eax + 0c0h]
        add edi, 8
        push edi
        lea ecx, [esi + 0ach]
        call FableActionCall_0099b7d0
        jmp actionReturn

actionAbove11:
        sub ecx, 42h
        je refreshSavedGames
        dec ecx
        je usedKey9
        sub ecx, 13h
        je previous
        sub ecx, 80h
        je deleteProfile
        dec ecx
        je profileMenu
        sub ecx, 4
        jne actionReturn
        mov dword ptr [ebp + 8], 0fh
        jmp forward

profileMenu:
        mov ecx, dword ptr [eax + 4]
        cmp dword ptr [ecx], ecx
        je actionReturn
        mov eax, ecx
        mov eax, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        add eax, 8
        push eax
        lea ecx, [esi + 0b0h]
        mov dword ptr [ebp - 0ch], eax
        call FableActionCall_0099b7d0
        lea eax, [ebp + 8]
        lea ecx, [esi + 54h]
        push eax
        mov dword ptr [ebp + 8], 0ah
        call FableActionCall_0059b5d7
        mov edi, dword ptr [eax]
        mov ebx, dword ptr [edi]
        push ecx
        mov ecx, esp
        push -1
        push offset FableActionString_01252a04
        call FableActionCall_0099ebf0
        mov ecx, edi
        call dword ptr [ebx + 0ch]
        mov edi, eax
        test edi, edi
        je profileMenuNoAction
        push -1
        push offset FableActionString_012529e4
        lea ecx, [ebp - 4]
        call FableActionCall_0099ebf0
        lea eax, [ebp - 4]
        push eax
        lea eax, [ebp + 8]
        push eax
        mov eax, dword ptr [FableActionGlobal_013b871c]
        mov ecx, dword ptr [eax + 60h]
        call FableActionCall_009c95e0
        lea ecx, [ebp - 4]
        call FableActionCall_0099eae0
        mov ebx, dword ptr [edi]
        push 1
        push 1
        push dword ptr [ebp - 0ch]
        lea edx, [ebp + 8]
        push offset FableActionString_0122d820
        lea ecx, [ebp - 14h]
        call FableActionCall_0099bf30
        mov edx, eax
        lea ecx, [ebp - 10h]
        call FableActionCall_0099be70
        push eax
        mov ecx, edi
        call dword ptr [ebx + 240h]
        lea ecx, [ebp - 10h]
        call FableActionCall_0099b510
        lea ecx, [ebp - 14h]
        call FableActionCall_0099b510
        lea ecx, [ebp + 8]
        call FableActionCall_0099b510

profileMenuNoAction:
        lea eax, [ebp + 8]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp + 8], 0ah
        call FableActionCall_0059b5d7
        push 0
        jmp gotoNext

deleteProfile:
        mov byte ptr [ebp + 0bh], 0
        call FableActionCall_0040d2a0
        cmp byte ptr [eax + 0ch], 0
        jne deleteProfileChecked
        call FableActionCall_0040d2a0
        mov ecx, dword ptr [esi + 0b0h]
        add eax, 8
        mov eax, dword ptr [eax]
        cmp eax, ecx
        je deleteProfileSetFlag
        cmp eax, ebx
        je deleteProfileSet
        cmp ecx, ebx
        je deleteProfileSet
        push ecx
        push eax
        call FableActionCall_0040f0a0
        pop ecx
        pop ecx
        jmp deleteProfileCompareDone
deleteProfileSet:
        xor al, al
deleteProfileCompareDone:
        test al, al
        je deleteProfileChecked
deleteProfileSetFlag:
        mov byte ptr [ebp + 0bh], 1

deleteProfileChecked:
        lea eax, [esi + 0b0h]
        push eax
        call FableActionCall_0040d2a0
        mov ecx, eax
        call FableActionCall_00407040
        mov ecx, esi
        call FableActionCall_00596cce
        cmp byte ptr [ebp + 0bh], 0
        mov edi, eax
        jne deleteProfileExisting
        cmp edi, ebx
        je deleteProfileExisting
        lea eax, [ebp + 8]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp + 8], 7
        call FableActionCall_0059b5d7
        push dword ptr [eax]
        mov ecx, esi
        call FableActionCall_0059686d
        jmp actionReturn

deleteProfileExisting:
        lea eax, [ebp + 8]
        lea ebx, [esi + 54h]
        push eax
        mov ecx, ebx
        mov dword ptr [ebp + 8], 14h
        call FableActionCall_0059b5d7
        push 1
        push dword ptr [eax]
        mov ecx, esi
        call FableActionCall_00596763
        mov ecx, dword ptr [FableActionGlobal_013b871c]
        call FableActionCall_0042dbee
        test edi, edi
        jle actionReturn
        mov edi, dword ptr [esi + 98h]
        lea eax, [ebp + 8]
        push eax
        mov ecx, ebx
        mov dword ptr [ebp + 8], 7
        call FableActionCall_0059b5d7
        push 0
        push dword ptr [eax]
        mov ecx, esi
        call FableActionCall_00596763
        mov dword ptr [esi + 98h], edi
        jmp actionReturn

previous:
        push ebx
        mov ecx, esi
        call FableActionCall_00597bf2
        jmp actionReturn

usedKey9:
        mov dword ptr [ebp + 8], 9
        jmp forward

refreshSavedGames:
        mov ecx, esi
        call FableActionCall_00598463
        test eax, eax
        jne refreshSavedGamesHave
        cmp dword ptr [esi + 0bch], ebx
        ja refreshSavedGamesHave
        mov byte ptr [esi + 64h], 1
        call FableActionCall_0040d2a0
        mov ecx, eax
        call FableActionCall_0040a740
        lea ecx, [esi + 20h]
        call FableActionCall_0059b039
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 0a4h], ecx
        mov eax, dword ptr [ecx]
        push ebx
        call dword ptr [eax + 0c0h]
        jmp actionReturn

refreshSavedGamesHave:
        mov dword ptr [ebp + 8], 8
        jmp forward

invalidSave:
        mov eax, dword ptr [eax + 4]
        cmp dword ptr [eax], eax
        je invalidSaveNoDialog
        lea eax, [ebp - 0ch]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 0ch], 10h
        call FableActionCall_0059b5d7
        mov edi, dword ptr [eax]
        mov ebx, dword ptr [edi]
        push ecx
        mov ecx, esp
        push -1
        push offset FableActionString_012529cc
        call FableActionCall_0099ebf0
        mov ecx, edi
        call dword ptr [ebx + 0ch]
        mov ebx, eax
        test ebx, ebx
        mov eax, dword ptr [ebp + 8]
        mov eax, dword ptr [eax]
        mov eax, dword ptr [eax + 4]
        mov eax, dword ptr [eax]
        mov edi, dword ptr [eax + 8]
        je invalidSaveFinish
        test edi, edi
        je invalidSaveFinish
        push -1
        push offset FableActionString_01236a1c
        lea ecx, [ebp - 8]
        call FableActionCall_0099ebf0
        lea eax, [ebp - 8]
        push eax
        lea eax, [ebp + 8]
        push eax
        mov eax, dword ptr [FableActionGlobal_013b871c]
        mov ecx, dword ptr [eax + 60h]
        call FableActionCall_009c95e0
        lea ecx, [ebp - 8]
        call FableActionCall_0099eae0
        lea edx, [edi + 8]
        lea ecx, [ebp + 8]
        call FableActionCall_009c7980
        mov eax, dword ptr [ebx]
        push 1
        push 1
        lea ecx, [ebp + 8]
        push ecx
        mov ecx, ebx
        call dword ptr [eax + 240h]
        lea ecx, [ebp + 8]
        call FableActionCall_0099b510

invalidSaveFinish:
        xor ebx, ebx
invalidSaveNoDialog:
        mov dword ptr [ebp + 8], 10h
        jmp forward

actionAboveDc:
        mov edx, 127h
        cmp ecx, edx
        jg actionAbove127
        je keyboardCancel
        sub ecx, 0e5h
        je pressStart
        push 15h
        pop edi
        sub ecx, edi
        je newProfile
        sub ecx, 21h
        je deleteProfileAction
        sub ecx, 9
        je keyboardConfirm
        dec ecx
        je keyboardCancelAction
        dec ecx
        jne actionReturn
        mov esi, dword ptr [esi + 60h]
        cmp esi, ebx
        je actionReturn
        mov ecx, esi
        call FableActionCall_00851920
        jmp actionReturn

keyboardCancelAction:
        call FableActionCall_0040d2a0
        mov ecx, eax
        call FableActionCall_00409b70
        mov ecx, esi
        call FableActionCall_00595845
        jmp actionReturn

keyboardConfirm:
        mov ecx, dword ptr [eax + 4]
        cmp dword ptr [ecx], ecx
        je keyboardConfirmNoProfile
        mov eax, ecx
        mov eax, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        add eax, 8
        push eax
        mov ecx, esi
        call FableActionCall_0059899a

keyboardConfirmNoProfile:
        lea eax, [ebp + 8]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp + 8], ebx
        call FableActionCall_0059b5d7
        push 1
        jmp gotoNext

deleteProfileAction:
        mov dword ptr [ebp + 8], 16h
        jmp forward

newProfile:
        mov ecx, esi
        call FableActionCall_00597006
        test eax, eax
        lea eax, [ebp + 8]
        lea ecx, [esi + 54h]
        push eax
        je newProfileFallback
        mov dword ptr [ebp + 8], edi
forwardResolve:
        call FableActionCall_0059b5d7
        push ebx
gotoNext:
        push dword ptr [eax]
        mov ecx, esi
        call FableActionCall_00596763
        jmp actionReturn

newProfileFallback:
        mov dword ptr [ebp + 8], 0ch
        jmp forwardResolve

pressStart:
        mov ecx, esi
        call FableActionCall_00599d5c
        jmp actionReturn

keyboardCancel:
        mov esi, dword ptr [esi + 60h]
        cmp esi, ebx
        je actionReturn
        mov ecx, esi
        call FableActionCall_00851860
        jmp actionReturn

actionAbove127:
        sub ecx, 128h
        je scoreOptions
        dec ecx
        je key18
        sub ecx, 0fh
        je key19
        dec ecx
        dec ecx
        je key1a
        sub ecx, 7
        je key1c
        sub ecx, 0a90h
        jne actionReturn
        mov dword ptr [ebp + 8], 14h
        jmp forward

key1c:
        mov dword ptr [ebp + 8], 1ch
        jmp forward
key1a:
        mov dword ptr [ebp + 8], 1ah

forward:
        lea eax, [ebp + 8]
        push eax
        lea ecx, [esi + 54h]
        jmp forwardResolve

key19:
        mov ecx, esi
        call FableActionCall_005963db
        mov dword ptr [ebp + 8], 19h
        jmp forward

key18:
        mov dword ptr [ebp + 8], 18h
        jmp forward

scoreOptions:
        mov ecx, dword ptr [FableActionGlobal_013b871c]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 10h]

actionReturn:
        pop edi
        pop esi
        pop ebx
        leave
        ret 4
    }
}
