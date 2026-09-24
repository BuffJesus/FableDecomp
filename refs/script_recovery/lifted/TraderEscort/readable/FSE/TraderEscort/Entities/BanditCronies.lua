-- Readable native conversion: BanditCronies. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- BanditCronies.Main (retail 0x00e03810)
function Main(quest, me)
    local scratchValue
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 13.0) do
        if not quest:NewScriptFrame(me) then return end
    end
    quest:ReadGlobalGameData(944)
    quest:ReadGlobalGameData(940)
    math.random(0, 32767)
    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x3ac) + (uVar5 % (uint)(iVar1 - iVar2 >> 2)) * 4),(int)&xStack_18);
    -- TODO(native): CCharString::CCharString(&xStack_14,&xStack_18);
    scratchValue = quest:PlaySoundOnThing(me, nil --[[missing]])
    quest:GiveThingBestEnemyTarget(me, hero)
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

