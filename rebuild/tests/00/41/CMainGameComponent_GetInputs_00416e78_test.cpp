// Behaviour fixture for CMainGameComponent::GetInputs @ 0x00416E78.
// Camera, input manager, player interface and event sinks are recording
// stand-ins; the component is a raw retail-sized buffer. Checks the six
// camera samples (interpolation 0,0,0,1,1,1; X,Y,Z,X,Y,Z), the truncating
// checksum sum, the CChecksum and frame handed to UpdateChecksum, call order,
// the event pump for both playback modes, and every early-out.
#include "fable_game.h"
#include "fable_camera.h"
#include "fable_checksum.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include "fable_system_manager.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

CInputManager* g_FableInputManager_013B8388;

struct MainGameProbe : CMainGameComponent
{
    static void CallGetInputs(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->CMainGameComponent::GetInputs(); }
};

static int seq;
static bool ok;
static unsigned char compStorage[0x161E8 + 16];
static unsigned char* comp = compStorage;
static void Wr(unsigned off, unsigned long v) { memcpy(comp + off, &v, 4); }

// ---- camera ----
static const float camPos[6][3] = {
    { 1.75f, 100.0f, 200.0f },
    { 300.0f, -2.5f, 400.0f },
    { 500.0f, 600.0f, 3.25f },
    { 10.5f, 700.0f, 800.0f },
    { 900.0f, 20.25f, 1000.0f },
    { 1100.0f, 1200.0f, -0.75f },
};
static int camCalls;
static float camArg[6];
static unsigned long cameraObject[4];
static void* cameraVtable[4];
static CCamera* FABLE_FASTCALL GetRenderCameraStub(void* self, void*, CCamera* result, float t)
{
    ok = ok && self == cameraObject && camCalls < 6;
    int i = camCalls < 6 ? camCalls : 5;
    camArg[i] = t;
    memset(result, 0xCC, sizeof(CCamera));
    result->WorldPosition.X = camPos[i][0];
    result->WorldPosition.Y = camPos[i][1];
    result->WorldPosition.Z = camPos[i][2];
    ++camCalls;
    return result;
}
static void FABLE_FASTCALL BadSlot(void*, void*) { ok = false; }
static unsigned char worldStorage[0x198];

// ---- component members called out of line ----
static int checksumAt, updateChecksumAt, updateAt, activeAt, frameAt, piUpdateAt;
unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    checksumAt = ++seq;
    ok = ok && static_cast<const void*>(this) == comp;
    return 0xC0DE1234;
}

// ---- input manager ----
static unsigned long inputObject[0x84 / 4];
static void* inputVtable[4];
static long ucFrame;
static CChecksum ucChecksum(0, 0, 0);
void CInputManager::UpdateChecksum(long frame, const CChecksum& checksum)
{
    updateChecksumAt = ++seq;
    ok = ok && static_cast<void*>(this) == inputObject && camCalls == 6 && checksumAt != 0;
    ucFrame = frame;
    ucChecksum = checksum;
}
static void FABLE_FASTCALL InputUpdateStub(void* self, void*)
{
    updateAt = ++seq;
    ok = ok && self == inputObject;
}

// ---- system manager / world frame ----
static bool appActive;
static long worldFrame;
static unsigned char systemStorage[0x180];
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(systemStorage); }
bool CSystemManager::IsApplicationActive()
{
    activeAt = ++seq;
    ok = ok && static_cast<void*>(this) == systemStorage;
    return appActive;
}
long FABLE_FASTCALL CWorld::GetFrame()
{
    frameAt = ++seq;
    return worldFrame;
}

// ---- player interface ----
static unsigned long piObject[4];
static void* piVtable[4];
static int eventsToDeliver, eventsDelivered, getEventCalls;
static const CGameEvent* eventAddr;
void CGamePlayerInterface::Update()
{
    piUpdateAt = ++seq;
    ok = ok && static_cast<void*>(this) == piObject;
}
static bool FABLE_FASTCALL GetGameEventStub(const void* self, void*, CGameEvent* event)
{
    ++getEventCalls;
    ok = ok && self == piObject;
    if (getEventCalls == 1)
    {
        // The constructor's two stores, nothing else touched yet.
        ok = ok && event->Player == -1 && event->Valid == false;
        eventAddr = event;
    }
    ok = ok && event == eventAddr;
    if (eventsDelivered == eventsToDeliver)
        return false;
    ++eventsDelivered;
    event->Type = 100 + eventsDelivered;
    return true;
}

// ---- event sinks ----
static int addCount, systemCount, processCount;
static long addTypes[8], processTypes[8];
static bool systemAnswer[8];
void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    ok = ok && static_cast<void*>(this) == comp && &event == eventAddr && addCount < 8;
    addTypes[addCount++ & 7] = event.Type;
}
bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == comp && event == eventAddr && systemCount < 8;
    return systemAnswer[systemCount++ & 7];
}
void CMainGameComponent::ProcessEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == comp && event == eventAddr && processCount < 8;
    processTypes[processCount++ & 7] = event->Type;
}

static long ExpectedSum()
{
    long sum = 0;
    sum += camPos[0][0];
    sum += camPos[1][1];
    sum += camPos[2][2];
    sum += camPos[3][0];
    sum += camPos[4][1];
    sum += camPos[5][2];
    return sum;
}

static bool Run(bool active, long frame, bool loading, int events)
{
    memset(compStorage, 0xA5, sizeof(compStorage));
    memset(worldStorage, 0, sizeof(worldStorage));
    Wr(0x24, (unsigned long)worldStorage);
    unsigned long cam = (unsigned long)cameraObject;
    memcpy(worldStorage + 0x34, &cam, 4);
    Wr(0x20, (unsigned long)piObject);
    Wr(0x48, 0x00004321);             // CurrentServerFrame
    Wr(0x1613C, 0x5EED5EED);          // WorldSeed
    comp[0x16170] = loading ? 1 : 0;  // LoadingEventPackages
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));

    cameraVtable[0] = (void*)&BadSlot;
    cameraVtable[1] = (void*)&GetRenderCameraStub;
    cameraVtable[2] = cameraVtable[3] = (void*)&BadSlot;
    cameraObject[0] = (unsigned long)cameraVtable;
    inputVtable[0] = inputVtable[1] = inputVtable[3] = (void*)&BadSlot;
    inputVtable[2] = (void*)&InputUpdateStub;
    inputObject[0] = (unsigned long)inputVtable;
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(inputObject);
    piVtable[0] = piVtable[2] = piVtable[3] = (void*)&BadSlot;
    piVtable[1] = (void*)&GetGameEventStub;
    piObject[0] = (unsigned long)piVtable;

    seq = camCalls = 0;
    checksumAt = updateChecksumAt = updateAt = activeAt = frameAt = piUpdateAt = 0;
    ucFrame = -1;
    ucChecksum = CChecksum(-1, -1, -1);
    ucChecksum.Checksum4 = -1;
    appActive = active;
    worldFrame = frame;
    eventsToDeliver = events;
    eventsDelivered = getEventCalls = 0;
    eventAddr = 0;
    addCount = systemCount = processCount = 0;
    for (int i = 0; i < 8; ++i) { systemAnswer[i] = (i % 2) == 0; addTypes[i] = processTypes[i] = 0; }
    ok = true;

    MainGameProbe::CallGetInputs(reinterpret_cast<CMainGameComponent*>(comp));

    // Camera sampling.
    if (camCalls != 6) ok = false;
    for (int i = 0; i < 6; ++i)
        if (camArg[i] != (i < 3 ? 0.0f : 1.0f)) ok = false;

    // Checksum.
    if (ucFrame != 0x4321) ok = false;
    if ((unsigned long)ucChecksum.Checksum1 != 0xC0DE1234 ||
        (unsigned long)ucChecksum.Checksum2 != 0x5EED5EED ||
        ucChecksum.Checksum3 != ExpectedSum() || ucChecksum.Checksum4 != 0)
        ok = false;

    // Order: checksum -> UpdateChecksum -> Update -> IsApplicationActive -> GetFrame -> PI Update.
    if (!(checksumAt == 1 && updateChecksumAt == 2 && updateAt == 3 && activeAt == 4)) ok = false;
    bool pump = active && frame > 1;
    if (frameAt != (active ? 5 : 0)) ok = false;
    if (piUpdateAt != (pump ? 6 : 0)) ok = false;

    // Event pump.
    if (pump)
    {
        if (getEventCalls != events + 1) ok = false;
        if (!loading)
        {
            if (addCount != events || systemCount != 0 || processCount != 0) ok = false;
            for (int i = 0; i < events; ++i) if (addTypes[i] != 101 + i) ok = false;
        }
        else
        {
            int expectProcess = 0;
            for (int i = 0; i < events; ++i)
                if (systemAnswer[i]) { if (processTypes[expectProcess] != 101 + i) ok = false; ++expectProcess; }
            if (addCount != 0 || systemCount != events || processCount != expectProcess) ok = false;
        }
    }
    else if (getEventCalls != 0 || addCount != 0 || systemCount != 0 || processCount != 0)
        ok = false;

    // GetInputs writes no component field.
    if (memcmp(before, compStorage, sizeof(compStorage)) != 0) ok = false;
    if (!ok) printf("case active=%d frame=%ld loading=%d events=%d failed\n", active, frame, loading, events);
    return ok;
}

int main()
{
    bool pass = true;
    pass = Run(true, 5, false, 3) && pass;
    pass = Run(true, 5, true, 5) && pass;
    pass = Run(true, 2, false, 0) && pass;
    pass = Run(true, 1, false, 3) && pass;
    pass = Run(false, 5, false, 3) && pass;
    if (pass)
        printf("FABLETLC_MAINGAME_GETINPUTS PASS\n");
    else
        printf("FABLETLC_MAINGAME_GETINPUTS FAIL\n");
    return pass ? 0 : 1;
}