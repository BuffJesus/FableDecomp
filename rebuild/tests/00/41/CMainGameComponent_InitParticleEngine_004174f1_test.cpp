// Behaviour fixture for CMainGameComponent::InitParticleEngine @ 0x004174F1.
// Checks the call order (Initialise -> new interaction -> counted pointer ->
// SetEnvironmentInteraction(copy) -> bank name -> OpenBank -> releases), that
// the interaction is a 0x24-byte CParticleEnvironmentInteraction bound to
// the component's world, that the engine receives a counted copy (the local
// reference is released on exit, the engine's survives), and the bank name.
#include "fable_game.h"
#include "fable_particle_engine.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->InitParticleEngine(); }
};

typedef NParticleEngine::CAEnvironmentInteraction CAEI;

static int seq;
static bool ok;
static unsigned char compStorage[0x161E8 + 16];
static unsigned char* comp = compStorage;
static unsigned char worldStorage[0x198];

static int initAt, newAt, ptrCtorAt, copyAt, setAt, strAt, dbAt, openAt, strDtorAt, deleteAt;
static unsigned newSizes[8];
static int newCount;
static void* interaction;

void* operator new(size_t size)
{
    if (newCount < 8) newSizes[newCount] = size;
    ++newCount;
    void* p = malloc(size);
    if (size == 0x24) { newAt = ++seq; interaction = p; }
    return p;
}
void operator delete(void* p) { free(p); }

void FABLE_FASTCALL NParticleEngine::Initialise() { initAt = ++seq; }

NParticleEngine::CAEnvironmentInteraction::~CAEnvironmentInteraction() {}
bool CAEI::IsWindActive() const { ok = false; return false; }
C3DVector CAEI::GetGravityVector() const { ok = false; C3DVector v(0, 0, 0); return v; }
C3DVector CAEI::GetVectorParam(const CCharString&) const { ok = false; C3DVector v(0, 0, 0); return v; }
CParticleEnvironmentInteraction::~CParticleEnvironmentInteraction() { deleteAt = ++seq; }
bool CParticleEnvironmentInteraction::IsParticleCollidingAt(
    const C3DVector&, const C3DVector&, bool, C3DVector&, C3DVector&, bool&) const { ok = false; return false; }
C3DVector CParticleEnvironmentInteraction::GetWindVelocity() const { ok = false; C3DVector v(0, 0, 0); return v; }
void CParticleEnvironmentInteraction::GetCameraPos(C3DVector&) const { ok = false; }

// Real counted-pointer semantics (retail 0x0041956C / 0x00418FBC).
static void FABLE_FASTCALL DeleteInteraction(void* data) { delete static_cast<CAEI*>(data); }
static void* localPtr;
template<> CCountedPointer<CAEI>::CCountedPointer(CAEI* data)
{
    ptrCtorAt = ++seq;
    localPtr = this;
    ok = ok && data == interaction;
    Data = data;
    Info = data ? new CCPPointerInfo(&DeleteInteraction, data) : 0;
}
template<> CCountedPointer<CAEI>::CCountedPointer(const CCountedPointer<CAEI>& other)
{
    copyAt = ++seq;
    ok = ok && &other == localPtr;
    Data = other.Data;
    Info = other.Info;
    if (Info) ++Info->RefCount;
}
static CCountedPointer<CAEI> engineCopy;
static unsigned long refsAtSet;
void FABLE_FASTCALL NParticleEngine::SetEnvironmentInteraction(CCountedPointer<CAEI> environmentInteraction)
{
    setAt = ++seq;
    ok = ok && environmentInteraction.Data == interaction && &environmentInteraction != localPtr;
    refsAtSet = environmentInteraction.Info ? environmentInteraction.Info->RefCount : 0;
    engineCopy.Data = environmentInteraction.Data;
    engineCopy.Info = environmentInteraction.Info;
    if (engineCopy.Info) ++engineCopy.Info->RefCount;
}

static const char* strText;
static long strLen;
static void* strThis;
CCharString::CCharString(const char* text, long length)
{
    strAt = ++seq;
    strText = text;
    strLen = length;
    strThis = this;
}
CCharString::~CCharString()
{
    strDtorAt = ++seq;
    ok = ok && static_cast<void*>(this) == strThis;
}
static unsigned char dbStorage[16];
NParticleEngine::CParticleEmitterDatabase& FABLE_FASTCALL NParticleEngine::GetParticleEmitterDatabase()
{
    dbAt = ++seq;
    return *reinterpret_cast<NParticleEngine::CParticleEmitterDatabase*>(dbStorage);
}
void NParticleEngine::CParticleEmitterDatabase::OpenBank(const CCharString& bankName)
{
    openAt = ++seq;
    ok = ok && static_cast<void*>(this) == dbStorage && static_cast<const void*>(&bankName) == strThis;
}

// Emits the class's vtable and scalar deleting destructor in this object
// (never called).
CAEI* ForceInteractionVtable()
{
    return new CParticleEnvironmentInteraction(*reinterpret_cast<const CWorld*>(worldStorage));
}

int main()
{
    memset(compStorage, 0xA5, sizeof(compStorage));
    unsigned long w = (unsigned long)worldStorage;
    memcpy(comp + 0x24, &w, 4);
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));
    ok = true;

    interaction = 0;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));
    bool pass = true;
    CParticleEnvironmentInteraction* made = static_cast<CParticleEnvironmentInteraction*>(engineCopy.Data);
    if (static_cast<void*>(made) != interaction) pass = false;
    if (!(initAt == 1 && newAt == 2 && ptrCtorAt == 3 && copyAt == 4 && setAt == 5 &&
          strAt == 6 && dbAt == 7 && openAt == 8 && strDtorAt == 9 && deleteAt == 0))
    {
        printf("order %d %d %d %d %d %d %d %d %d %d\n", initAt, newAt, ptrCtorAt, copyAt, setAt, strAt, dbAt, openAt, strDtorAt, deleteAt);
        pass = false;
    }
    if (newCount != 2 || newSizes[0] != 0x24 || newSizes[1] != sizeof(CCPPointerInfo)) pass = false;
    if (made == 0 || made->PWorld != reinterpret_cast<CWorld*>(worldStorage)) pass = false;
    if (made && *reinterpret_cast<void**>(made) == 0) pass = false;          // vptr stored
    if (refsAtSet != 2) pass = false;                                          // local + by-value copy
    if (engineCopy.Info == 0 || engineCopy.Info->RefCount != 1) pass = false;  // only the engine's left
    if (strText == 0 || strcmp(strText, "PARTICLE_MAIN") != 0 || strLen != -1) pass = false;
    if (memcmp(before, compStorage, sizeof(compStorage)) != 0) pass = false;
    // Releasing the engine's reference destroys the interaction through its vtable.
    engineCopy.~CCountedPointer<CAEI>();
    if (deleteAt == 0) pass = false;
    pass = pass && ok;
    if (pass)
        printf("FABLETLC_MAINGAME_INITPARTICLEENGINE PASS\n");
    else
        printf("FABLETLC_MAINGAME_INITPARTICLEENGINE FAIL\n");
    return pass ? 0 : 1;
}