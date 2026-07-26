#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_relocation_compiled.h"

static int g_priorityCalls;
static int g_postRelocationCalls;
static CBankFileAsyncData* g_lastAsyncData;
static CLandscapeLayerMesh* g_lastLayerMesh;

void CBankFileAsyncData::ChangeLoadingPriority()
{
    ++g_priorityCalls;
    g_lastAsyncData = this;
}

void CLandscapeLayerMesh::OnPostRelocation()
{
    ++g_postRelocationCalls;
    g_lastLayerMesh = this;
}

static void ResetPatch(CEngineLandscapePatch& patch)
{
    memset(&patch, 0, sizeof(patch));
    g_priorityCalls = 0;
    g_postRelocationCalls = 0;
    g_lastAsyncData = 0;
    g_lastLayerMesh = 0;
}

int main()
{
    CEngineLandscapePatch patch;
    CLandscapeLayerMesh oldMesh;
    CLandscapeLayerMesh newMesh;
    CLandscapeLayerMesh head;
    CLandscapeLayerMesh missing;
    CBankFileAsyncData oldAsyncData;
    CBankFileAsyncData newAsyncData;
    memset(&oldMesh, 0, sizeof(oldMesh));
    memset(&newMesh, 0, sizeof(newMesh));
    memset(&head, 0, sizeof(head));
    memset(&missing, 0, sizeof(missing));

    ResetPatch(patch);
    patch.RelocatedLayerMesh = &oldMesh;
    patch.RelocateData(1, &oldMesh, &newMesh, sizeof(oldMesh));
    if (patch.RelocatedLayerMesh != &newMesh) return 1;
    if (g_priorityCalls != 0 || g_postRelocationCalls != 0) return 2;

    ResetPatch(patch);
    patch.AsyncData = &oldAsyncData;
    patch.RelocateData(2, &oldAsyncData, &newAsyncData, sizeof(oldAsyncData));
    if (patch.AsyncData != &newAsyncData) return 3;
    if (g_priorityCalls != 1 || g_lastAsyncData != &newAsyncData) return 4;

    ResetPatch(patch);
    patch.LayerMeshes = &oldMesh;
    patch.RelocateData(3, &oldMesh, &newMesh, sizeof(oldMesh));
    if (patch.LayerMeshes != &newMesh) return 5;
    if (g_postRelocationCalls != 1 || g_lastLayerMesh != &newMesh) return 6;

    ResetPatch(patch);
    head.Next = &oldMesh;
    patch.LayerMeshes = &head;
    patch.RelocateData(4, &oldMesh, &newMesh, sizeof(oldMesh));
    if (patch.LayerMeshes != &head || head.Next != &newMesh) return 7;
    if (g_postRelocationCalls != 1 || g_lastLayerMesh != &newMesh) return 8;

    ResetPatch(patch);
    head.Next = &oldMesh;
    oldMesh.Next = 0;
    patch.LayerMeshes = &head;
    patch.RelocateData(5, &missing, &newMesh, sizeof(missing));
    if (patch.LayerMeshes != &head || head.Next != &oldMesh) return 9;
    if (g_postRelocationCalls != 0) return 10;

    ResetPatch(patch);
    patch.AsyncData = 0;
    patch.RelocateData(6, 0, &newAsyncData, 0);
    if (g_priorityCalls != 0 || patch.AsyncData != 0) return 11;

    printf("LANDSCAPE_PATCH_RELOCATE_DATA_TEST PASS\n");
    return 0;
}
