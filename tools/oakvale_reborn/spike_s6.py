#!/usr/bin/env python3
"""spike_s6.py -- build the Phase 0 spike S6 Lua stage (child-hero combat) for Oakvale Reborn.

The Stranger's accept road needs the CHILD hero to fight: draw a sword, swing,
kill a villager, be fought back by a guard -- with the child skeleton, which
retail never animates in combat. This stage answers that with the smallest
possible probe, no story attached:

  copies the v4-proven readable stage to work/oakvale_reborn/spike_s6/lua and
  adds ONE quest thread (SpikeS6) to NewOakValeIntro.lua that, once the Father's
  intro is over and the hero has moved (first deed or 3 gold),

    GiveHeroWeapon(OBJECT_HERO_SWORD_FIRST, true) ; SetHeroWeaponsAsUsable(true)
    every creature except the hero: EntitySetAsKillable(true)
    every *_GUARD creature:          SetAttackHeroOnSight(true)
    then polls IsDead() over that list and logs each new corpse.

  --grown builds the fallback variant instead: TurnCreatureInto(hero, CREATURE_HERO)
  before arming (the "grown for the night" mechanic), so both S6 answers can be
  compared from the same save.

Verdict = FableScriptExtender.log + eyes (CHECKLIST.md "Spike S6"):
  OVR_SPIKE_S6: armed              the bindings did not throw
  OVR_SPIKE_S6: kill N <def>       a villager died to the hero
  draw/swing/kill animations play on the child, no T-pose, no crash; the guard
  fights back.  Fail -> the accept road ships with Stranger.GROWN_FOR_THE_NIGHT.

Then: python tools/oakvale_reborn/build_custom_intro.py bundle --lua work/oakvale_reborn/spike_s6/lua --tag spike-s6
"""
from __future__ import annotations

import argparse
import pathlib
import shutil

REPO = pathlib.Path(__file__).resolve().parents[2]
SOURCE = REPO / 'work/oakvale_readable_stage_20260916b'
OUT = REPO / 'work/oakvale_reborn/spike_s6/lua'
TARGET = 'NewOakValeIntro/NewOakValeIntro.lua'

MAIN_ANCHOR = '    quest:CreateThread("StartBarrelTimer")\n    DoMission(quest)\n'
MAIN_PATCH = '    quest:CreateThread("StartBarrelTimer")\n    quest:CreateThread("SpikeS6")\n    DoMission(quest)\n'

THREAD = '''
-- Oakvale Reborn spike S6: can the CHILD hero fight? (GROWN = %s)
function SpikeS6(quest)
    local function waitUntil(predicate)
        while not predicate() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    if not waitUntil(function()
        return quest:GetStateBool("DadFinishedIntro")
            and (quest:GetStateInt("GoodDeedsPerformed") + quest:GetStateInt("BadDeedsPerformed") >= 1
                 or quest:GetHeroGold() >= 3)
    end) then return end
    quest:Log("OVR_SPIKE_S6: start")
    local ok, err = pcall(function()
        local hero = quest:GetHero()
        if %s then
            hero = quest:TurnCreatureInto(hero, "CREATURE_HERO") or hero
            quest:Log("OVR_SPIKE_S6: grown for the night")
        end
        quest:GiveHeroWeapon("OBJECT_HERO_SWORD_FIRST", true)
        quest:SetHeroWeaponsAsUsable(true)
        local targets = {}
        for _, thing in ipairs(quest:GetAllCreaturesExcludingHero()) do
            local def = thing:GetDefName()
            quest:EntitySetAsKillable(thing, true)
            if type(def) == "string" and def:find("GUARD", 1, true) then
                quest:SetAttackHeroOnSight(thing, true)
            end
            targets[#targets + 1] = { thing = thing, def = def, dead = false }
        end
        quest:Log("OVR_SPIKE_S6: armed, " .. #targets .. " killable creatures")
        local kills = 0
        while kills < #targets do
            for _, t in ipairs(targets) do
                if not t.dead and t.thing:IsDead() then
                    t.dead = true
                    kills = kills + 1
                    quest:Log("OVR_SPIKE_S6: kill " .. kills .. " " .. tostring(t.def))
                end
            end
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
    end)
    quest:Log("OVR_SPIKE_S6: done ok=" .. tostring(ok) .. " err=" .. tostring(err))
end
'''


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--grown', action='store_true', help='build the TurnCreatureInto(CREATURE_HERO) fallback variant')
    args = a.parse_args()
    if OUT.exists():
        shutil.rmtree(OUT)
    shutil.copytree(SOURCE, OUT)
    path = OUT / TARGET
    text = path.read_text(encoding='utf-8')
    if text.count(MAIN_ANCHOR) != 1:
        raise SystemExit(f'{TARGET}: Main anchor not found')
    text = text.replace(MAIN_ANCHOR, MAIN_PATCH)
    text += THREAD % ('true' if args.grown else 'false', 'true' if args.grown else 'false')
    path.write_text(text, encoding='utf-8', newline='\n')
    print(f'spike S6 stage ({"grown" if args.grown else "child"}) -> {OUT}')
    print(f'next: python tools/oakvale_reborn/build_custom_intro.py bundle --lua {OUT.relative_to(REPO)} --tag spike-s6{"-grown" if args.grown else ""}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
