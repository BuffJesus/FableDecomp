#pragma once

// Static set-up entry points CMainGameComponent::Init (0x004184BD) calls
// between its progress messages. Only these statics are modelled; the classes'
// layouts are not, except CTCBase's shared primary prefix.

#include "rebuild_abi.h"
#include "fable_thing_components.h"

// FableWin ?RegisterTCs@CTCBase@@SIXXZ, retail 0x004EE23F (runs under the
// "Init Thing Components" progress text; registers CTCHeroMorph,
// CTCSimpleAppearanceMorph, CTCAtmosPlayer, ... with their def types).
// RegisterTCs is declared on the shared CTCBase in fable_thing_components.h.

// FableWin ?Initialise@CSubtitledMessage@@SIXXZ, retail 0x004CDB10
// ("Init Subtitled Message").
class CSubtitledMessage
{
public:
    static void FABLE_FASTCALL Initialise();
};

// FableWin ?Init@CConversationAttitude@@SIXXZ, retail 0x004CD670
// ("Init Conversation Attitude").
class CConversationAttitude
{
public:
    static void FABLE_FASTCALL Init();
};
