#pragma once
#include "fable_singleton.h"
#include "fable_counted_pointer.h"
#include "fable_stlport_list.h"
#include "fable_surface.h"
class CIEngine;
class CEngineManager;
// Ego_r.pdb identifies the polymorphic singleton base; retail manager is20
// bytes because its list/vector use STLport storage (4/12 bytes).
template<> class CTBaseSingleton<CEngineManager> {
public:
    virtual ~CTBaseSingleton() {}
    static CEngineManager& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Release();
    static CCountedPointer<CEngineManager> PClass;
};
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    void AddEngine(CIEngine&);
    void RemoveEngine(CIEngine&);
    _STL::list<CIEngine*> Engines;
    _STL::vector<C2DBoxI> ChangedAreas;
};
FABLE_STATIC_ASSERT(sizeof(CEngineManager)==20);
