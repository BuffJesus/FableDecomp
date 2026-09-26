-- Readable native conversion: DarkwoodAssassinSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_AssassinSpawnDistance = 3604,  -- 30
    TE_AssassinName = 3608,  -- 'CREATURE_BANDIT_GRUNT_LEVEL1_A'
}

-- DarkwoodAssassinSpawn.Main (retail 0x00e02e50)
function Main(quest, me)
    local timerId, p0, entity, scratchValue, scratchValue15
    local hero = quest:GetHero()
    local function DeregisterTimers()
        quest:DeregisterTimer(timerId)
    end
    if not quest:NewScriptFrame(me) then return end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 2)
    local scratchValue17 = quest:ReadGlobalGameData(SCRIPT_DEF.TE_AssassinSpawnDistance)
    quest:ReadGlobalGameData(944)
    quest:ReadGlobalGameData(940)
    math.random(0, 32767)
    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x3ac) + (uVar5 % (uint)(iVar8 - iVar1 >> 2)) * 4),(int)&xStack_28);
    -- TODO(native): CCharString::CCharString(&xStack_30,&xStack_28);
    if not quest:IsActiveThreadTerminating() then
        -- TODO(native): xStack_20 = CVar3;
        repeat
            if not quest:IsDistanceBetweenThingsUnder(me, hero, scratchValue17) then goto LAB_00e02f91 end
            if quest:IsCameraPosOnScreen(me:GetPos()) then goto LAB_00e02f91 end
            if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
            goto FLOW_past_lab_00e02f91
            ::LAB_00e02f91::
            if quest:IsActiveThreadTerminating() then break end
            quest:SetTimer(timerId, 2)
            ::FLOW_past_lab_00e02f91::
            if quest:GetTimer(timerId) ~= 0 then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
            entity = quest:CreateCreature(quest:ReadGlobalGameDataString(SCRIPT_DEF.TE_AssassinName), me:GetPos(), "DarkwoodAssassin")
            quest:SetThingPersistent(entity, true)
            quest:EntityAttachToScript(entity, quest:GetActiveQuestName())
            if math.random(0, 32767) % 5 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e03105 end
                goto FLOW_hoist_lab_00e03105_1
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e03105 end
                quest:GiveThingBestEnemyTarget(entity, quest:GetNearestWithScriptName(entity, "DarkwoodTrader"))
            end
            goto FLOW_past_lab_00e03105
            ::LAB_00e03105::
            break
            ::FLOW_hoist_lab_00e03105_1::
            quest:GiveThingBestEnemyTarget(entity, hero)
            ::FLOW_past_lab_00e03105::
            if not quest:IsDistanceBetweenThingsOver(entity, hero, 18.0) then goto LAB_00e0320d end
            if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
            if quest:IsDistanceBetweenThingsUnder(entity, hero, 18.0) then goto LAB_00e031e9 end
            goto LAB_00e031b3
            quest:NewScriptFrame(me)
            ::continue_1::
        until quest:IsActiveThreadTerminating()
    end
    quest:DeregisterTimer(timerId)
    do return end
    ::LAB_00e031b3::
    while true do
        if not quest:NewScriptFrame(me) then goto LAB_00e03372 end
        if quest:IsDistanceBetweenThingsUnder(entity, hero, 18.0) then break end
    end
    ::LAB_00e031e9::
    quest:PlaySoundOnThing(entity, "DarkwoodAssassin")
    ::LAB_00e0320d::
    if quest:IsDistanceBetweenThingsOver(entity, hero, 12.0) then
        if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
        while not quest:IsDistanceBetweenThingsUnder(entity, hero, 12.0) do
            if not quest:NewScriptFrame(me) then goto LAB_00e03372 end
        end
        scratchValue = quest:PlaySoundOnThing(entity, nil --[[missing]])
    end
    if not quest:IsDistanceBetweenThingsOver(entity, hero, 8.0) then quest:RemoveThing(p0, false, true); goto LAB_00e03372 end
    if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
    while not quest:IsDistanceBetweenThingsUnder(entity, hero, 8.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00e03372 end
    end
    scratchValue15 = quest:PlaySoundOnThing(entity, nil --[[missing]])
    quest:RemoveThing(p0, false, true)
    ::LAB_00e03372::
    quest:DeregisterTimer(timerId)
end

-- DarkwoodAssassinSpawn.Init (retail 0x00e02e20)
function Init(quest, me)
end

-- DarkwoodAssassinSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DarkwoodAssassinSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

