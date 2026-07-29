#include <cstdio>

struct CScriptThingRecord
{
    void*          m_pVTable;
    void*          m_pData;
    unsigned long* m_pReference;
};

struct CScriptThingVector
{
    CScriptThingRecord* m_pBegin;
    CScriptThingRecord* m_pEnd;
    CScriptThingRecord* m_pCapacity;
};

struct CGameScriptInterface
{
    void** m_pVTable;
};

struct ReferenceInfo
{
    unsigned long m_Count;
    void*         m_Release;
    void*         m_Context;
};

struct CandidateData
{
    void** m_pVTable;
};

extern "C" unsigned long g_ScriptThingVTable = 0;
extern "C" float g_DefaultPosition[2] = {0.0f, 0.0f};

static CScriptThingRecord* g_vector_begin;
static CScriptThingRecord* g_vector_end;
static const void* g_observed_name;
static unsigned long g_find_calls;
static unsigned long g_init_calls;
static unsigned long g_destroy_local_calls;
static unsigned long g_vector_destroy_calls;
static unsigned long g_free_calls;
static void* g_freed_pointer;
static unsigned long g_delete_reference_calls;
static float g_candidate_position[2];
static float g_origin_position[2];
static unsigned long g_relation_calls;
static void* g_observed_origin_data;

static void __fastcall FindByScriptName(
    CGameScriptInterface*,
    int,
    const void* pName,
    CScriptThingVector* pVector)
{
    ++g_find_calls;
    g_observed_name = pName;
    pVector->m_pBegin = g_vector_begin;
    pVector->m_pEnd = g_vector_end;
    pVector->m_pCapacity = g_vector_end;
}

static void __fastcall DestroyVectorRecord(
    CScriptThingRecord* pRecord,
    int,
    int)
{
    ++g_vector_destroy_calls;
    if (pRecord->m_pReference != 0)
        --*pRecord->m_pReference;
}

static float* __fastcall CandidatePosition(void*)
{
    return g_candidate_position;
}

static float* __fastcall OriginPosition(void*)
{
    return g_origin_position;
}

static bool __fastcall IsSameEntity(
    void*,
    int,
    void* pOriginData)
{
    ++g_relation_calls;
    g_observed_origin_data = pOriginData;
    return false;
}

extern "C" void* __fastcall Sub_InitScriptThing(void* self)
{
    ++g_init_calls;
    return self;
}

extern "C" void __fastcall Sub_DestroyScriptThing(void*)
{
    ++g_destroy_local_calls;
}

extern "C" void __cdecl Sub_Free(void* pMemory)
{
    ++g_free_calls;
    g_freed_pointer = pMemory;
}

extern "C" __declspec(naked) long __cdecl Sub_RoundFloat()
{
    __asm
    {
        sub esp, 4
        fistp dword ptr [esp]
        mov eax, dword ptr [esp]
        add esp, 4
        ret
    }
}

extern "C" void __cdecl Sub_DeleteReference(void*)
{
    ++g_delete_reference_calls;
}

CScriptThingRecord* __fastcall
CGameScriptInterface_GetNearestWithScriptName(
    CGameScriptInterface* self,
    int edxUnused,
    CScriptThingRecord* pResult,
    CScriptThingRecord* pOrigin,
    const void* pScriptName);

static void ResetObservations()
{
    g_observed_name = 0;
    g_find_calls = 0;
    g_init_calls = 0;
    g_destroy_local_calls = 0;
    g_vector_destroy_calls = 0;
    g_free_calls = 0;
    g_freed_pointer = 0;
    g_delete_reference_calls = 0;
    g_relation_calls = 0;
    g_observed_origin_data = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a510_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    void* interfaceVTable[0x12c / 4 + 1] = {};
    void* recordVTable[7] = {};
    void* originVTable[7] = {};
    void* dataVTable[0x138 / 4 + 1] = {};
    CGameScriptInterface gsi = {};
    CScriptThingRecord result = {};
    CScriptThingRecord origin = {};
    CScriptThingRecord candidate = {};
    CandidateData candidateData = {};
    ReferenceInfo reference = {};
    unsigned long originMarker = 0;
    unsigned long nameMarker = 0;

    interfaceVTable[0x12c / 4] = (void*)FindByScriptName;
    recordVTable[0] = (void*)DestroyVectorRecord;
    originVTable[0x18 / 4] = (void*)OriginPosition;
    dataVTable[0x18 / 4] = (void*)CandidatePosition;
    dataVTable[0x138 / 4] = (void*)IsSameEntity;
    gsi.m_pVTable = interfaceVTable;
    origin.m_pVTable = originVTable;
    origin.m_pData = &originMarker;

    ResetObservations();
    g_vector_begin = 0;
    g_vector_end = 0;
    CScriptThingRecord* returned =
        CGameScriptInterface_GetNearestWithScriptName(
            &gsi, 0, &result, &origin, &nameMarker);
    if (returned != &result ||
        result.m_pVTable != &g_ScriptThingVTable ||
        result.m_pData != 0 ||
        result.m_pReference != 0 ||
        g_find_calls != 1 ||
        g_observed_name != &nameMarker ||
        g_init_calls != 1 ||
        g_free_calls != 0)
        return Fail("empty result construction");

    ResetObservations();
    candidateData.m_pVTable = dataVTable;
    reference.m_Count = 1;
    candidate.m_pVTable = recordVTable;
    candidate.m_pData = &candidateData;
    candidate.m_pReference = &reference.m_Count;
    g_candidate_position[0] = 3.0f;
    g_candidate_position[1] = 4.0f;
    g_origin_position[0] = 0.0f;
    g_origin_position[1] = 0.0f;
    g_vector_begin = &candidate;
    g_vector_end = &candidate + 1;
    result.m_pVTable = 0;
    result.m_pData = 0;
    result.m_pReference = 0;

    returned = CGameScriptInterface_GetNearestWithScriptName(
        &gsi, 0, &result, &origin, &nameMarker);
    if (returned != &result ||
        result.m_pVTable != &g_ScriptThingVTable ||
        result.m_pData != &candidateData ||
        result.m_pReference != &reference.m_Count ||
        reference.m_Count != 1 ||
#ifndef FURTHEST_VARIANT
        g_relation_calls != 1 ||
        g_observed_origin_data != &originMarker ||
#endif
        g_destroy_local_calls != 1 ||
        g_vector_destroy_calls != 1 ||
        g_free_calls != 1 ||
        g_freed_pointer != &candidate ||
        g_delete_reference_calls != 0)
        return Fail("single nearest candidate");

    std::printf("CGameScriptInterface_0089a510_TEST PASS\n");
    return 0;
}
