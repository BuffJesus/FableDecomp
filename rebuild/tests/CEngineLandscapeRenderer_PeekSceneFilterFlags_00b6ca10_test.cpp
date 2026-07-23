#include <stdio.h>
#include <string.h>
#include "candidates/landscape_shader_accessors.h"

int main()
{
    CEngineLandscapeRenderer renderer;
    memset(&renderer, 0, sizeof(renderer));
    if (renderer.PeekSceneFilterFlags() != 1) return 1;
    printf("LANDSCAPE_RENDERER_SCENE_FILTER_FLAGS_TEST PASS\n");
    return 0;
}
