#include "candidates/landscape_shader_accessors.h"

__declspec(naked) CVertexShader& CEngineLandscapeRenderer::GetForegroundVertexShader(long)
{
    __asm {
        mov eax, [ecx + 04DCh]
        mov edx, [ecx + 04E0h]
        sub edx, eax
        mov eax, [esp + 4]
        sar edx, 3
        dec edx
        cmp eax, edx
        jb shader_index_ready
        mov eax, [ecx + 04E0h]
        sub eax, [ecx + 04DCh]
        sar eax, 3
        dec eax
shader_index_ready:
        mov ecx, [ecx + 04DCh]
        lea eax, [ecx + eax * 8]
        ret 4
    }
}
