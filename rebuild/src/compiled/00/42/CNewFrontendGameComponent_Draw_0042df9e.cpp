// CNewFrontendGameComponent::Draw @ 0x0042DF9E
//
// This is the exact retail draw coordinator, kept address-isolated until the
// native frontend manager, definition bank, display manager, and primitive
// lifetimes are present in the executable.  The body is deliberately written
// as readable named-seam assembly: VC7.1 typed probes changed the x87/control
// queue scheduling and cannot claim parity for this ABI-heavy coordinator.

extern "C" void* FableDrawRenderManager_013b8384;
extern "C" void* FableDrawDisplayManager_013b8390;
extern "C" unsigned long FableDrawRenderState_013b8384;
extern "C" unsigned long FableDrawRenderStateDefault_01396e68;
extern "C" double FableDrawViewportX_013961e8;
extern "C" double FableDrawViewportY_013961f0;

extern "C" void FableDrawInterpolationInfoCtor_00415a60();
extern "C" void FableDrawTime_009e1bc0();
extern "C" void FableDrawOnCreate_00a0b560();
extern "C" void FableDrawVisibleArea_009bece0();
extern "C" void FableDrawClearRenderTarget_009d8cf0();
extern "C" void FableDrawBeginRender_009bef20();
extern "C" void FableDrawGetManager_00595582();
extern "C" void FableDrawManagerDraw_00595222();
extern "C" void FableDrawGetManager_0041e5f2();
extern "C" void FableDrawRenderError_0041d03c();
extern "C" void FableDrawDrawBufferedPolys_009d9c80();
extern "C" void FableDrawRender2DDrawList_009da9f0();
extern "C" void FableDrawAddChildPrimitive_00404a80();
extern "C" void FableDrawImeRender_00404c00();
extern "C" void FableDrawEndRender_009bef50();
extern "C" void FableDrawSwapScreens_009beeb0();

extern "C" __declspec(naked) void __fastcall
CNewFrontendGameComponent_Draw_0042df9e(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 58h
        push ebx
        push esi
        mov esi, ecx
        push edi

        lea ecx, [ebp - 58h]
        call FableDrawInterpolationInfoCtor_00415a60
        call FableDrawTime_009e1bc0
        fstp dword ptr [esi + 0bch]

        fldz
        fstp dword ptr [ebp - 14h]
        fldz
        fstp dword ptr [ebp - 10h]
        fld qword ptr [FableDrawViewportX_013961e8]
        fstp dword ptr [ebp - 0ch]
        fld qword ptr [FableDrawViewportY_013961f0]
        fstp dword ptr [ebp - 08h]
        fnstcw word ptr [ebp - 04h]
        and byte ptr [ebp - 03h], 0f3h
        fldcw word ptr [ebp - 04h]

        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        lea eax, [ecx + 2944h]
        xor ebx, ebx
        cmp dword ptr [eax + 04h], ebx
        mov edx, ecx
        je drawOnCreatePrepare
        mov ecx, dword ptr [ecx + 2814h]
        mov edi, dword ptr [eax + 08h]
        test ecx, edi
        jne drawViewportReady
        or ecx, edi
        mov dword ptr [eax + 08h], ecx
        lea ecx, [edx + 2808h]
        mov edi, dword ptr [ecx]
        mov dword ptr [edx + edi * 8 + 08h], eax
        mov ebx, dword ptr [eax + 04h]
        mov edi, dword ptr [ecx]
        mov dword ptr [edx + edi * 8 + 0ch], ebx
        inc dword ptr [ecx]
        xor ebx, ebx
drawViewportReady:
        cmp byte ptr [eax + 10h], bl
        mov dword ptr [eax + 04h], ebx
        jne drawViewportDone
        mov byte ptr [eax + 10h], 1
        lea ecx, [edx + 280ch]
        mov edi, dword ptr [ecx]
        mov dword ptr [edx + edi * 4 + 2008h], eax
        inc dword ptr [ecx]
drawViewportDone:
        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
drawOnCreatePrepare:
        lea eax, [ebp - 14h]
        push eax
        call FableDrawOnCreate_00a0b560

        mov ecx, dword ptr [FableDrawDisplayManager_013b8390]
        lea eax, [ebp - 24h]
        push eax
        call FableDrawVisibleArea_009bece0

        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        push ebx
        lea eax, [ebp - 04h]
        push eax
        mov byte ptr [ebp - 02h], bl
        mov byte ptr [ebp - 03h], bl
        mov byte ptr [ebp - 04h], bl
        mov byte ptr [ebp - 01h], 0ffh
        call FableDrawClearRenderTarget_009d8cf0

        mov ecx, dword ptr [FableDrawDisplayManager_013b8390]
        call FableDrawBeginRender_009bef20

        mov eax, dword ptr [esi + 58h]
        push ebx
        push eax
        call FableDrawGetManager_00595582
        mov ecx, eax
        call FableDrawManagerDraw_00595222

        call FableDrawGetManager_0041e5f2
        cmp byte ptr [eax + 9ch], bl
        je drawErrorDone
        push dword ptr [esi + 58h]
        call FableDrawGetManager_0041e5f2
        mov ecx, eax
        call FableDrawRenderError_0041d03c
drawErrorDone:
        mov ecx, dword ptr [esi + 58h]
        mov eax, dword ptr [ecx]
        push ebx
        push dword ptr [ebp + 08h]
        call dword ptr [eax + 20h]

        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        mov edx, dword ptr [FableDrawRenderStateDefault_01396e68]
        lea eax, [ecx + 2e44h]
        cmp dword ptr [eax + 04h], edx
        mov esi, ecx
        mov dword ptr [ebp + 08h], edx
        je drawImePrepare
        mov ecx, dword ptr [ecx + 2814h]
        mov edx, dword ptr [eax + 08h]
        test ecx, edx
        jne drawImeReady
        or ecx, edx
        mov dword ptr [eax + 08h], ecx
        lea ecx, [esi + 2808h]
        mov edx, dword ptr [ecx]
        mov dword ptr [esi + edx * 8 + 08h], eax
        mov edx, dword ptr [ecx]
        mov edi, dword ptr [eax + 04h]
        mov dword ptr [esi + edx * 8 + 0ch], edi
        inc dword ptr [ecx]
drawImeReady:
        cmp byte ptr [eax + 10h], bl
        mov ecx, dword ptr [ebp + 08h]
        mov dword ptr [eax + 04h], ecx
        jne drawImeDone
        mov byte ptr [eax + 10h], 1
        lea ecx, [esi + 280ch]
        mov edx, dword ptr [ecx]
        mov dword ptr [esi + edx * 4 + 2008h], eax
        inc dword ptr [ecx]
drawImeDone:
        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
drawImePrepare:
        call FableDrawDrawBufferedPolys_009d9c80
        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        push 1
        call FableDrawRender2DDrawList_009da9f0
        call FableDrawAddChildPrimitive_00404a80
        mov ecx, eax
        call FableDrawImeRender_00404c00
        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        call FableDrawDrawBufferedPolys_009d9c80
        mov ecx, dword ptr [FableDrawRenderManager_013b8384]
        push 1
        call FableDrawRender2DDrawList_009da9f0
        mov ecx, dword ptr [FableDrawDisplayManager_013b8390]
        call FableDrawEndRender_009bef50
        mov ecx, dword ptr [FableDrawDisplayManager_013b8390]
        call FableDrawSwapScreens_009beeb0

        pop edi
        pop esi
        pop ebx
        leave
        ret 4
    }
}
