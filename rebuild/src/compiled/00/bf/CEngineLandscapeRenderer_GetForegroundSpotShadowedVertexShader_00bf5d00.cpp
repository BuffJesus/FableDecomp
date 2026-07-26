#include "candidates/landscape_shader_accessors.h"

__declspec(naked) CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotShadowedVertexShader(long)
{
    __asm {
        mov ecx, [ecx + 054Ch]
        mov eax, [ecx]
        mov edx, [ecx + 4]
        sub edx, eax
        mov eax, [esp + 4]
        sar edx, 3
        dec edx
        cmp eax, edx
        jb shader_index_ready
        mov eax, [ecx + 4]
        sub eax, [ecx]
        sar eax, 3
        dec eax
shader_index_ready:
        mov ecx, [ecx]
        lea eax, [ecx + eax * 8]
        ret 4
    }
}
