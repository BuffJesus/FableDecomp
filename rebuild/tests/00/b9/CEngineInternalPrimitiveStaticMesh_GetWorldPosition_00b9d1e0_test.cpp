#include <stdio.h>
#include <string.h>
#include "engine/CEngineInternalPrimitiveStaticMesh.h"

int main()
{
    CEngineInternalPrimitiveStaticMesh mesh;
    C3DVector position;
    memset(&mesh, 0, sizeof(mesh));
    mesh.ObjectTransform.E41 = 10.0f;
    mesh.ObjectTransform.E42 = -20.0f;
    mesh.ObjectTransform.E43 = 30.5f;

    if (!mesh.GetWorldPosition(position)) return 1;
    if (position.X != 10.0f) return 2;
    if (position.Y != -20.0f) return 3;
    if (position.Z != 30.5f) return 4;

    printf("STATIC_MESH_WORLD_POSITION PASS\n");
    return 0;
}