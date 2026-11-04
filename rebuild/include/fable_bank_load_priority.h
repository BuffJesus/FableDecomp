#pragma once
// Ego_r/FableWin enum EBankLoadPriority, shared by the bank families that take a load priority:
// ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z (retail 0x00A23C50) and
// FableWin ?PreloadMesh@CEnginePrimitiveManagerMeshBase@@...W4EBankLoadPriority@@... It lives in its
// own header so a later family can fill in the enumerators without editing a family header.
// The enumerators are not recovered yet (no recovered body compares against one).
enum EBankLoadPriority
{
};
