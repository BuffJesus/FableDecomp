#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"

// Retail dependencies:
// - IDirect3DDevice9-compatible pointer at CDisplayManager +0x58
// - vtable slot +0xBC (IDirect3DDevice9::SetViewport)
// - the process-wide shader manager at retail address 0x013BC470
// - the one-byte $E2 notification target at retail address 0x00A0AAC0
extern CShaderRenderManager g_CShaderRenderManager_013BC470;
extern "C" void FableViewportE2_00A0AAC0();

__declspec(naked)
void CDisplayManager::SetIntegerViewportEndpoint(const C2DBoxI&)
{
    __asm
    {
        sub esp, 18h
        push ebx
        push ebp
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esp + 28h]
        mov eax, dword ptr [ecx]
        mov ebx, dword ptr [esi + 198h]
        xor ebp, ebp
        cmp eax, ebp
        push edi
        mov edi, dword ptr [esi + 194h]
        jge clamp_left_high
        xor edx, edx
        jmp left_clamped
clamp_left_high:
        cmp eax, edi
        jle left_in_range
        mov eax, edi
left_in_range:
        mov edx, eax
left_clamped:
        mov eax, dword ptr [ecx + 08h]
        inc eax
        cmp eax, ebp
        mov dword ptr [esp + 14h], edx
        mov dword ptr [esp + 18h], edx
        jge clamp_right_high
        mov dword ptr [esp + 2ch], ebp
        jmp right_clamped
clamp_right_high:
        cmp eax, edi
        mov dword ptr [esp + 2ch], edi
        jg right_clamped
        mov dword ptr [esp + 2ch], eax
right_clamped:
        mov eax, dword ptr [esp + 2ch]
        mov dword ptr [esp + 20h], eax
        mov eax, dword ptr [ecx + 04h]
        cmp eax, ebp
        jl top_clamped
        cmp eax, ebx
        jle top_in_range
        mov eax, ebx
top_in_range:
        mov ebp, eax
top_clamped:
        mov eax, dword ptr [ecx + 0ch]
        inc eax
        mov edi, ebp
        mov dword ptr [esp + 10h], ebp
        mov dword ptr [esp + 1ch], edi
        jns clamp_bottom_high
        xor eax, eax
        jmp bottom_clamped
clamp_bottom_high:
        cmp eax, ebx
        jle bottom_clamped
        mov eax, ebx
bottom_clamped:
        cmp edx, dword ptr [esi + 1f4h]
        mov ebx, eax
        mov dword ptr [esp + 24h], ebx
        jne viewport_changed
        cmp ebp, dword ptr [esi + 1f8h]
        jne viewport_changed
        mov ecx, dword ptr [esp + 2ch]
        cmp ecx, dword ptr [esi + 1fch]
        jne viewport_changed
        cmp eax, dword ptr [esi + 200h]
        je viewport_done
viewport_changed:
        test edx, edx
        mov byte ptr [esi + 1d8h], 0
        mov ecx, 1
        jne test_full_width
        mov edx, dword ptr [esp + 2ch]
        test edx, edx
        jne reload_left
        mov dword ptr [esp + 20h], ecx
        mov byte ptr [esi + 1d8h], cl
        jmp test_height
reload_left:
        mov edx, dword ptr [esp + 14h]
test_full_width:
        mov ebp, dword ptr [esi + 194h]
        cmp edx, ebp
        jne restore_top
        cmp dword ptr [esp + 2ch], ebp
        jne restore_top
        dec ebp
        mov dword ptr [esp + 18h], ebp
        mov byte ptr [esi + 1d8h], cl
restore_top:
        mov ebp, dword ptr [esp + 10h]
test_height:
        test ebp, ebp
        jne test_full_height
        test eax, eax
        jne test_full_height
        mov ebx, ecx
        mov dword ptr [esp + 24h], ebx
        jmp viewport_is_full
test_full_height:
        mov edx, dword ptr [esi + 198h]
        cmp ebp, edx
        jne build_viewport
        cmp eax, edx
        jne build_viewport
        lea edi, dword ptr [edx - 1]
        mov dword ptr [esp + 1ch], edi
viewport_is_full:
        mov byte ptr [esi + 1d8h], cl
build_viewport:
        mov ecx, dword ptr [esp + 18h]
        mov edx, dword ptr [esp + 20h]
        sub edx, ecx
        lea eax, dword ptr [esi + 1dch]
        mov dword ptr [eax], ecx
        mov ecx, ebx
        sub ecx, edi
        mov dword ptr [esi + 1e8h], ecx
        mov ecx, dword ptr [esi + 58h]
        push eax
        mov dword ptr [esi + 1e0h], edi
        mov dword ptr [esi + 1e4h], edx
        mov dword ptr [esi + 1ech], 0
        mov dword ptr [esi + 1f0h], 3f800000h
        mov edx, dword ptr [ecx]
        push ecx
        call dword ptr [edx + 0bch]
        test eax, eax
        jl post_hardware
        fild dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 18h]
        mov ecx, dword ptr [esp + 20h]
        mov dword ptr [esi + 1f4h], eax
        fstp dword ptr [esi + 204h]
        mov dword ptr [esi + 1f8h], edi
        fild dword ptr [esp + 1ch]
        mov dword ptr [esi + 1fch], ecx
        mov dword ptr [esi + 200h], ebx
        fstp dword ptr [esi + 208h]
        fild dword ptr [esp + 20h]
        fstp dword ptr [esi + 20ch]
        fild dword ptr [esp + 24h]
        fstp dword ptr [esi + 210h]
post_hardware:
        mov ecx, dword ptr [esi + 08h]
        mov edx, ecx
        neg edx
        sbb edx, edx
        test edx, 009c1220h
        je notify_shader
        call FableViewportE2_00A0AAC0
notify_shader:
        mov ecx, offset g_CShaderRenderManager_013BC470
        call CShaderRenderManager::OnPostViewportChanged
viewport_done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 18h
        ret 4
    }
}
