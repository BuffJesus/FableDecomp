#pragma once
#include "fable_stlport_list.h"
#include "fable_d3d9_query.h"
// Ego_r names; retail A61880/A61A80 use one-pointer STLport lists,
// making the native class 20 bytes rather than Ego_r's 36.
class CBusyResourceManager {
public:
    struct CPendingQuery { IDirect3DQuery9* Query; long Frame; };
    CBusyResourceManager();
    ~CBusyResourceManager();
    void ClearAllQueries();
    void OnPostSwapScreens();
    void SetDevice(IDirect3DDevice9* device) { D3DDevice=device; }
private:
    friend struct CBusyResourceManagerTestAccess;
    _STL::list<CPendingQuery> FreeQueoryObjects;
    _STL::list<CPendingQuery> PendingQueories;
    long PendingFrame,FinishedFrame;
    IDirect3DDevice9* D3DDevice;
};
FABLE_STATIC_ASSERT(sizeof(CBusyResourceManager)==20);
FABLE_STATIC_ASSERT(sizeof(CBusyResourceManager::CPendingQuery)==8);
