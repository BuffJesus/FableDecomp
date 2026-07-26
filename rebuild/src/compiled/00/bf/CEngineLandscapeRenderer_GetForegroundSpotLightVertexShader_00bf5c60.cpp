#include "candidates/landscape_shader_accessors.h"

__declspec(naked) CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long)
{
    __asm {
        mov eax, [ecx + 04E8h]
        mov edx, [ecx + 04ECh]
        sub edx, eax
        mov eax, [esp + 4]
        sar edx, 3
        dec edx
        cmp eax, edx
        jb shader_index_ready
        mov eax, [ecx + 04ECh]
        sub eax, [ecx + 04E8h]
        sar eax, 3
        dec eax
shader_index_ready:
        mov ecx, [ecx + 04E8h]
        lea eax, [ecx + eax * 8]
        ret 4
    }
}
