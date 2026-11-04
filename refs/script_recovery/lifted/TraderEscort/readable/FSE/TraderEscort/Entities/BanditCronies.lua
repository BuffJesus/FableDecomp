-- Readable native conversion: BanditCronies. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    AssassinScreamSoundIds = 940,  -- '04000000cd0b0600f30b0600190c06003f0c0600'
}

-- BanditCronies.Main (retail 0x00e03810)
function Main(quest, me)
    quest:NewScriptFrame(me)
    local isDistanceBetweenThingsUnder
    if quest:IsActiveThreadTerminating() then return end
    local hero = quest:GetHero()
    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 13.0)
    while not isDistanceBetweenThingsUnder do
        if not quest:NewScriptFrame(me) then return end
        local hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero2, 13.0)
    end
    if quest:IsActiveThreadTerminating() then return end
    local scratchValue = quest:ReadGlobalGameData(944)
    local scratchValue2 = quest:ReadGlobalGameData(SCRIPT_DEF.AssassinScreamSoundIds)
    local randomChoice = quest:RetailRandModulo(32768)
    local sampledSound = quest:ReadGlobalGameDataDefStringAt(940, randomChoice % ((scratchValue - scratchValue2) >> 2))
    quest:PlaySoundOnThing(me, sampledSound)
    local hero3 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, hero3)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- BanditCronies.Init (retail 0x00e037e0)
function Init(quest, me)
end

-- BanditCronies.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BanditCronies.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

