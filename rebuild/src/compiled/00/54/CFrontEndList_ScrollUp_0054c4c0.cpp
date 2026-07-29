// NUISystem::CFrontEndList::ScrollUp @ 0x0054C4C0
// VC7.1, x86, /O2 /Oy.
//
// The retail body depends on the binary layouts of CFrontEndList,
// CComponent, CFrontEndManager, and CDefPointeeBase.  Keeping the recovered
// instruction schedule here makes those ABI details explicit:
//   list +15c  selected row
//        +164  component pointer array begin
//        +168  component pointer array end
//        +1f8  per-row alpha falloff
//   list vtable +0d8 update time, +2ac stop/wrapping policy
//   component vtable +080 SetPosition, +098 SetColour, +0c0 SetState,
//                    +0dc GetPosition, +0e0 GetColour
//   manager vtable +0a0 PlaySound, +0a8 GetUIMiscThingsDef
//
// The local CDef pointer is released with the retail intrusive refcount
// sequence.  The update-time sentinel comparison and alpha calculation retain
// the original x87 comparisons and __ftol2 boundary.

extern "C" void* __cdecl FableFrontEndScrollGetManager();
extern "C" int __cdecl FableFrontEndScrollFtol2();

extern "C" const float FableFrontEndScrollMinusOne;
extern "C" const float FableFrontEndScrollInv255;
extern "C" const float FableFrontEndScrollOne;
extern "C" const float FableFrontEndScrollZero;
extern "C" const float FableFrontEndScroll255;

extern "C" __declspec(naked) void __fastcall
CFrontEndList_ScrollUp_0054c4c0(void*, void*)
{
    __asm
    {
        sub esp, 20h
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 168h]
        push edi
        sub eax, dword ptr [esi + 164h]
        sar eax, 2
        cmp eax, 1
        jbe reject
        mov eax, dword ptr [esi + 15ch]
        test eax, eax
        jne scroll
        mov eax, dword ptr [esi]
        call dword ptr [eax + 2ach]
        test al, al
        je scroll

reject:
        call FableFrontEndScrollGetManager
        mov esi, eax
        call FableFrontEndScrollGetManager
        mov edx, dword ptr [esi]
        mov edi, eax
        lea eax, [esp + 8]
        push eax
        mov ecx, esi
        call dword ptr [edx + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [edi]
        push 1
        add eax, 1a4h
        push eax
        mov ecx, edi
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [esp + 8]
        test eax, eax
        je commonReturn
        mov esi, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec esi
        mov dword ptr [eax], esi
        mov edx, esi
        test edx, edx
        jne commonReturn
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]
        pop edi
        pop esi
        add esp, 20h
        ret

scroll:
        push ebx
        call FableFrontEndScrollGetManager
        mov edi, eax
        call FableFrontEndScrollGetManager
        lea ecx, [esp + 0ch]
        mov ebx, eax
        mov eax, dword ptr [edi]
        push ecx
        mov ecx, edi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [ebx]
        push 1
        add eax, 194h
        push eax
        mov ecx, ebx
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [esp + 0ch]
        test eax, eax
        je defReleased
        mov edi, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec edi
        mov edx, edi
        test edx, edx
        mov dword ptr [eax], edi
        jne defReleased
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]

defReleased:
        mov eax, dword ptr [esi + 15ch]
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx + eax*4]
        mov edx, dword ptr [ecx]
        push 4
        call dword ptr [edx + 0c0h]
        mov eax, dword ptr [esi + 15ch]
        dec eax
        mov dword ptr [esi + 15ch], eax
        jns selectedReady
        mov eax, dword ptr [esi + 168h]
        sub eax, dword ptr [esi + 164h]
        sar eax, 2
        dec eax

selectedReady:
        mov ecx, dword ptr [esi + 164h]
        mov dword ptr [esi + 15ch], eax
        mov eax, dword ptr [ecx + eax*4]
        mov edx, dword ptr [eax]
        push 3
        mov ecx, eax
        call dword ptr [edx + 0c0h]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0d8h]
        fld dword ptr [FableFrontEndScrollMinusOne]
        fucompp
        fnstsw ax
        test ah, 44h
        jnp zeroUpdateTime
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 0d8h]
        fstp dword ptr [esp + 0ch]
        jmp updateTimeReady

zeroUpdateTime:
        mov dword ptr [esp + 0ch], 0

updateTimeReady:
        mov eax, dword ptr [esi + 168h]
        push ebp
        mov ebp, dword ptr [esi + 164h]
        sub eax, ebp
        sar eax, 2
        mov ecx, ebp
        mov ecx, dword ptr [ecx + eax*4 - 4]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 18h]
        push eax
        call dword ptr [edx + 0e0h]
        mov ebp, dword ptr [esi + 164h]
        mov ecx, dword ptr [esi + 168h]
        sub ecx, ebp
        sar ecx, 2
        mov edx, ebp
        mov ecx, dword ptr [edx + ecx*4 - 4]
        mov eax, dword ptr [ecx]
        lea edx, [esp + 28h]
        push edx
        call dword ptr [eax + 0dch]
        mov edi, dword ptr [esi + 168h]
        mov edx, dword ptr [esi + 164h]
        mov ebp, dword ptr [esp + 10h]
        sub edi, edx
        sar edi, 2
        dec edi
        mov dword ptr [esp + 20h], 0
        mov dword ptr [esp + 24h], 041f00000h
        je wrapLast

rotateRows:
        mov eax, dword ptr [esi + 164h]
        mov ecx, dword ptr [eax + edi*4 - 4]
        mov ebx, dword ptr [eax + edi*4]
        mov edx, dword ptr [ecx]
        lea eax, [eax + edi*4]
        mov eax, dword ptr [ebx]
        push 0
        push ebp
        lea ebp, [esp + 24h]
        push ebp
        mov dword ptr [esp + 20h], eax
        call dword ptr [edx + 0e0h]
        push eax
        mov eax, dword ptr [esp + 20h]
        mov ecx, ebx
        call dword ptr [eax + 98h]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 2ach]
        test al, al
        mov ebp, dword ptr [esp + 10h]
        je nextRow
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx + edi*4]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 28h]
        push eax
        call dword ptr [edx + 80h]

nextRow:
        dec edi
        jne rotateRows

wrapLast:
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 20h]
        push eax
        call dword ptr [edx + 98h]
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2ach]
        test al, al
        je alphaPolicy
        mov eax, dword ptr [esi + 164h]
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 28h]
        push eax
        call dword ptr [edx + 80h]

alphaPolicy:
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2ach]
        test al, al
        je done
        mov ebx, dword ptr [esi + 164h]
        mov al, 0ffh
        mov byte ptr [esp + 12h], al
        mov byte ptr [esp + 11h], al
        mov byte ptr [esp + 10h], al
        mov byte ptr [esp + 13h], al
        mov eax, dword ptr [esi + 168h]
        sub eax, ebx
        sar eax, 2
        xor edi, edi
        test eax, eax
        jbe done

alphaLoop:
        mov eax, dword ptr [esi + 15ch]
        movzx ecx, byte ptr [esi + 1f8h]
        sub eax, edi
        cdq
        xor eax, edx
        sub eax, edx
        mov dword ptr [esp + 14h], eax
        fild dword ptr [esp + 14h]
        mov dword ptr [esp + 14h], ecx
        fild dword ptr [esp + 14h]
        fmul dword ptr [FableFrontEndScrollInv255]
        fmulp st(1), st(0)
        fsubr dword ptr [FableFrontEndScrollOne]
        fcom dword ptr [FableFrontEndScrollZero]
        fnstsw ax
        test ah, 5
        jp positiveAlpha
        fstp st(0)
        fld dword ptr [FableFrontEndScrollZero]
        jmp convertAlpha

positiveAlpha:
        fmul dword ptr [FableFrontEndScroll255]

convertAlpha:
        call FableFrontEndScrollFtol2
        mov edx, dword ptr [esi + 164h]
        mov byte ptr [esp + 13h], al
        mov ecx, dword ptr [edx + edi*4]
        mov eax, dword ptr [ecx]
        push 0
        push ebp
        lea edx, [esp + 18h]
        push edx
        call dword ptr [eax + 98h]
        mov eax, dword ptr [esi + 168h]
        sub eax, dword ptr [esi + 164h]
        inc edi
        sar eax, 2
        cmp edi, eax
        jb alphaLoop

done:
        pop ebp
        pop ebx

commonReturn:
        pop edi
        pop esi
        add esp, 20h
        ret
    }
}
