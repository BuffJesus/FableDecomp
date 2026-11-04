#include "fable_world.h"
#include "fable_thing_manager.h"
#include "fable_game.h"
unsigned long* CTCBase::GetPLocalSeed() const
{
    return CThing::PeekPCurrentThingManager()->PeekComponent().GetLocalSeed();
}
