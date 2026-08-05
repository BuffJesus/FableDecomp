#include <cstddef>
#include <cstdint>

struct CIDrawEnvironment;
struct CWorld;
struct CPlayer;
struct CPlayerManager;
struct CAIStateGroup_JackOfBladesBase;

void* __cdecl operator_new(std::size_t size);

struct CPlayerManagerPlayerVector
{
    std::uint32_t* begin;   // 0x00
    std::uint32_t* current; // 0x04
    std::uint32_t* end;     // 0x08

    void __thiscall InsertWithBuffer(
        std::uint32_t* currentSlot,
        const void* first,
        const void* last,
        int growCount,
        int elementCount);
};

static_assert(offsetof(CPlayerManagerPlayerVector, begin) == 0x00);
static_assert(offsetof(CPlayerManagerPlayerVector, current) == 0x04);
static_assert(offsetof(CPlayerManagerPlayerVector, end) == 0x08);

struct CPlayerInit
{
    CWorld* world;                         // 0x00
    std::uint32_t worldField24;           // 0x04
    std::uint32_t unknown08;              // 0x08
    int playerNumber;                     // 0x0C
    long playerDefIndex;                  // 0x10
    bool hasDrawEnvironmentA;             // 0x14
    std::uint8_t pad15[3];                // 0x15
    CPlayerManager* playerManager;        // 0x18
    CIDrawEnvironment* drawEnvironment;   // 0x1C
    bool hasDrawEnvironmentB;             // 0x20
    std::uint8_t pad21[3];                // 0x21
};

static_assert(offsetof(CPlayerInit, world) == 0x00);
static_assert(offsetof(CPlayerInit, worldField24) == 0x04);
static_assert(offsetof(CPlayerInit, unknown08) == 0x08);
static_assert(offsetof(CPlayerInit, playerNumber) == 0x0C);
static_assert(offsetof(CPlayerInit, playerDefIndex) == 0x10);
static_assert(offsetof(CPlayerInit, hasDrawEnvironmentA) == 0x14);
static_assert(offsetof(CPlayerInit, playerManager) == 0x18);
static_assert(offsetof(CPlayerInit, drawEnvironment) == 0x1C);
static_assert(offsetof(CPlayerInit, hasDrawEnvironmentB) == 0x20);
static_assert(sizeof(CPlayerInit) == 0x24);

struct CPlayerManagerOverlay
{
    std::uint8_t pad00[0x04];
    CWorld* world;                             // 0x04
    std::uint32_t unknown08;                  // 0x08
    CPlayerManagerPlayerVector playerVector;  // 0x0C
};

static_assert(offsetof(CPlayerManagerOverlay, world) == 0x04);
static_assert(offsetof(CPlayerManagerOverlay, unknown08) == 0x08);
static_assert(offsetof(CPlayerManagerOverlay, playerVector) == 0x0C);

struct CWorldOverlay
{
    std::uint8_t pad00[0x24];
    std::uint32_t field24; // 0x24
};

static_assert(offsetof(CWorldOverlay, field24) == 0x24);

bool __thiscall CPlayerManager::IsPlayer(int playerNumber);
long __thiscall CPlayerManager::GetPlayerDefIndexFromNumber(int playerNumber);
CIDrawEnvironment* __thiscall CWorld::DrawGetEnvironment();
void __thiscall CPlayer::PlayerInit(CPlayerInit* init);
CAIStateGroup_JackOfBladesBase* __thiscall CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase();

void __thiscall CPlayerManager_AddPlayerOrAI(CPlayerManager* self, int playerNumber)
{
    const int originalPlayerNumber = playerNumber;
    if (self->IsPlayer(playerNumber))
    {
        return;
    }

    auto* aiStateGroup = static_cast<CAIStateGroup_JackOfBladesBase*>(operator_new(0x22C));
    if (aiStateGroup == nullptr)
    {
        playerNumber = 0;
    }
    else
    {
        playerNumber = reinterpret_cast<int>(aiStateGroup->CAIStateGroup_JackOfBladesBase());
    }

    auto* const selfOverlay = reinterpret_cast<CPlayerManagerOverlay*>(self);
    std::uint32_t* const currentSlot = selfOverlay->playerVector.current;
    if (currentSlot == selfOverlay->playerVector.end)
    {
        selfOverlay->playerVector.InsertWithBuffer(
            currentSlot,
            &playerNumber,
            &playerNumber,
            1,
            1);
    }
    else
    {
        if (currentSlot != nullptr)
        {
            *currentSlot = static_cast<std::uint32_t>(playerNumber);
        }

        selfOverlay->playerVector.current = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(selfOverlay->playerVector.current) + 4);
    }

    auto* const createdPlayer =
        reinterpret_cast<CPlayer*>(selfOverlay->playerVector.current[-1]);

    CPlayerInit init{};
    init.world = selfOverlay->world;
    init.worldField24 = reinterpret_cast<CWorldOverlay*>(init.world)->field24;
    init.unknown08 = selfOverlay->unknown08;
    init.playerNumber = originalPlayerNumber;
    init.playerDefIndex = self->GetPlayerDefIndexFromNumber(originalPlayerNumber);

    const bool hasDrawEnvironment = originalPlayerNumber != 4;
    if (hasDrawEnvironment)
    {
        init.drawEnvironment = selfOverlay->world->DrawGetEnvironment();
    }
    else
    {
        init.drawEnvironment = nullptr;
    }

    init.hasDrawEnvironmentA = hasDrawEnvironment;
    init.playerManager = self;
    init.hasDrawEnvironmentB = hasDrawEnvironment;

    createdPlayer->PlayerInit(&init);
}