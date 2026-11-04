-- Readable native conversion: DarkwoodAssassinSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    AssassinScreamSoundIds = 940,  -- '04000000cd0b0600f30b0600190c06003f0c0600'
    TE_AssassinSpawnDistance = 3604,  -- 30
    TE_AssassinName = 3608,  -- 'CREATURE_BANDIT_GRUNT_LEVEL1_A'
}

-- DarkwoodAssassinSpawn.Main (retail 0x00e02e50)
function Main(quest, me)
    local isDistanceBetweenThingsUnder, isDistanceBetweenThingsUnder5, timerId, hero2, hero3, hero4
    local hero6, hero9, hero10, position, position2, getActiveQuestName, entity, scratchValue12
    local scratchValue13, scratchValue14, scratchValue18
    local function DeregisterTimers()
        quest:DeregisterTimer(timerId)
    end
    if not quest:NewScriptFrame(me) then return end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 2)
    local scratchValue15 = quest:ReadGlobalGameData(SCRIPT_DEF.TE_AssassinSpawnDistance)
    local scratchValue11 = quest:ReadGlobalGameData(944)
    local scratchValue = quest:ReadGlobalGameData(SCRIPT_DEF.AssassinScreamSoundIds)
    local randomChoice = quest:RetailRandModulo(32768)
    local sampledSound = quest:ReadGlobalGameDataDefStringAt(940, randomChoice % ((scratchValue11 - scratchValue) >> 2))
    if not quest:IsActiveThreadTerminating() then
        repeat
            local hero = quest:GetHero()
            if not quest:IsDistanceBetweenThingsUnder(me, hero, scratchValue15) then goto LAB_00e02f91 end
            position = me:GetPos()
            if quest:IsCameraPosOnScreen(position) then goto LAB_00e02f91 end
            if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
            goto FLOW_past_lab_00e02f91
            ::LAB_00e02f91::
            if quest:IsActiveThreadTerminating() then break end
            quest:SetTimer(timerId, 2)
            ::FLOW_past_lab_00e02f91::
            if quest:GetTimer(timerId) ~= 0 then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
            scratchValue18 = quest:ReadGlobalGameDataString(SCRIPT_DEF.TE_AssassinName)
            position2 = me:GetPos()
            entity = quest:CreateCreature(scratchValue18, position2, "DarkwoodAssassin", true)
            quest:SetThingPersistent(entity, true)
            getActiveQuestName = quest:GetActiveQuestName()
            quest:EntityAttachToScript(entity, getActiveQuestName)
            if quest:RetailRandModulo(32768) % 5 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e03105 end
                goto FLOW_hoist_lab_00e03105_1
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e03105 end
                local darkwoodTrader = quest:GetNearestWithScriptName(entity, "DarkwoodTrader")
                quest:GiveThingBestEnemyTarget(entity, darkwoodTrader)
            end
            goto FLOW_past_lab_00e03105
            ::LAB_00e03105::
            break
            ::FLOW_hoist_lab_00e03105_1::
            hero2 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(entity, hero2)
            ::FLOW_past_lab_00e03105::
            hero3 = quest:GetHero()
            if not quest:IsDistanceBetweenThingsOver(entity, hero3, 18.0) then goto LAB_00e0320d end
            if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
            hero4 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(entity, hero4, 18.0) then goto LAB_00e031e9 end
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
        local hero5 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(entity, hero5, 18.0) then break end
    end
    ::LAB_00e031e9::
    if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
    scratchValue12 = quest:PlaySoundOnThing(entity, sampledSound)
    ::LAB_00e0320d::
    hero6 = quest:GetHero()
    if quest:IsDistanceBetweenThingsOver(entity, hero6, 12.0) then
        if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
        local hero7 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(entity, hero7, 12.0)
        while not isDistanceBetweenThingsUnder do
            if not quest:NewScriptFrame(me) then goto LAB_00e03372 end
            local hero8 = quest:GetHero()
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(entity, hero8, 12.0)
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
        scratchValue13 = quest:PlaySoundOnThing(entity, sampledSound)
    end
    hero9 = quest:GetHero()
    if not quest:IsDistanceBetweenThingsOver(entity, hero9, 8.0) then quest:RemoveThing(me, false, true); goto LAB_00e03372 end
    if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
    hero10 = quest:GetHero()
    isDistanceBetweenThingsUnder5 = quest:IsDistanceBetweenThingsUnder(entity, hero10, 8.0)
    while not isDistanceBetweenThingsUnder5 do
        if not quest:NewScriptFrame(me) then goto LAB_00e03372 end
        local hero11 = quest:GetHero()
        isDistanceBetweenThingsUnder5 = quest:IsDistanceBetweenThingsUnder(entity, hero11, 8.0)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e03372 end
    scratchValue14 = quest:PlaySoundOnThing(entity, sampledSound)
    quest:RemoveThing(me, false, true)
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

