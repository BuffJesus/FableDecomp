#include "candidates/landscape_patch_relocation_compiled.h"

// The retail routine was built with a register/layout choice that VC7.1 does
// not recover from the readable translation alone. These fastcall thunks keep
// the two dependency calls mockable while the lifted body preserves its exact
// control flow and instruction layout.
extern "C" void __fastcall LandscapePatchChangeLoadingPriority(CBankFileAsyncData* data)
{
    data->ChangeLoadingPriority();
}

extern "C" void __fastcall LandscapeLayerMeshPostRelocation(CLandscapeLayerMesh* mesh)
{
    mesh->OnPostRelocation();
}

__declspec(naked) void CEngineLandscapePatch::RelocateData(long, void*, void*, long)
{
    __asm {
        mov eax, [ecx + 0Ch]
        push esi
        mov esi, [esp + 0Ch]
        cmp esi, eax
        jne check_async_data
        test eax, eax
        je relocation_done
        mov edx, [esp + 10h]
        sub eax, esi
        add eax, edx
        mov [ecx + 0Ch], eax
        pop esi
        ret 10h

check_async_data:
        mov eax, [ecx + 14h]
        cmp esi, eax
        jne scan_layer_meshes
        test eax, eax
        je notify_async_data
        mov edx, [esp + 10h]
        sub eax, esi
        add eax, edx
        mov [ecx + 14h], eax
notify_async_data:
        mov ecx, [ecx + 14h]
        call LandscapePatchChangeLoadingPriority
        pop esi
        ret 10h

scan_layer_meshes:
        mov eax, [ecx + 08h]
        xor edx, edx
        test eax, eax
        je relocation_done
        _emit 08Dh
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
scan_next_layer_mesh:
        cmp esi, eax
        je relocate_layer_mesh
        mov edx, eax
        mov eax, [eax + 3Ch]
        test eax, eax
        jne scan_next_layer_mesh
        pop esi
        ret 10h

relocate_layer_mesh:
        test eax, eax
        je update_layer_link
        push edi
        mov edi, [esp + 14h]
        sub edi, esi
        add eax, edi
        pop edi
update_layer_link:
        test edx, edx
        je update_layer_head
        mov ecx, eax
        mov [edx + 3Ch], eax
        call LandscapeLayerMeshPostRelocation
        pop esi
        ret 10h
update_layer_head:
        mov [ecx + 08h], eax
        mov ecx, eax
        call LandscapeLayerMeshPostRelocation
relocation_done:
        pop esi
        ret 10h
    }
}
