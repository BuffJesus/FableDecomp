-- Readable native conversion: Q_WhiteBalverineWW. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- Q_WhiteBalverineWW.Main (retail 0x00e18630)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local missionFailed, scratchValue, scratchValue2, scratchValue3, predicateResult
    local predicateResult5
    quest:AddEntityBinding("KG_Chief", "WhiteBalverineWW/Entities/KG_Chief", 1)
    scratchValue2 = not predicateResult5 and not scratchValue3
    quest:AddEntityBinding("WBWW_WhiteBalverine", "WhiteBalverineWW/Entities/WBWW_WhiteBalverine", 1)
    quest:AddEntityBinding("WBWW_SoldierBalverine", "WhiteBalverineWW/Entities/WBWW_SoldierBalverine", 1)
    quest:FinalizeEntityBindings()
    quest:SetCreatureGeneratorsEnabled("Witchwood4", false)
    while not quest:IsLevelLoaded("WitchWood_7") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_07", "Witchwood4", "KnotholeGlade")
    quest:SetStateThing("WhiteBalverine", quest:GetThingWithScriptName("WBWW_WhiteBalverine"))
    local getStateThing = quest:GetStateThing("WhiteBalverine")
    quest:SetThingPersistent(getStateThing, true)
    quest:CreateThread("MonitorBalverine")  -- native thread body MonitorBalverine: lift it as function MonitorBalverine(quest)
    if scratchValue2 & 8 ~= 0 then
        scratchValue2 = scratchValue2 & 247
    end
    quest:CreateThread("SpawnBalverines")  -- native thread body 0x00E19560: lift it as function SpawnBalverines(quest)
    if scratchValue2 & 16 ~= 0 then
        scratchValue2 = scratchValue2 & 239
    end
    local resource = resources:NewResource()
    resources:TryAcquire(resource, getStateThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "BALV", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_WBW_DRINK", actorMap, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    local resource2 = resources:NewResource()
    resources:TryAcquire(resource2, getStateThing, 4)
    repeat
        scratchValue = scratchValue2
        if quest:IsDistanceBetweenThingsUnder(hero, getStateThing, 8.0) then
            goto LAB_00e18ce8
        else
            scratchValue = scratchValue2 | 32
            if getStateThing:MsgIsHitByHero() then goto LAB_00e18ce8 end
            scratchValue = scratchValue2 | 96
            if getStateThing:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = scratchValue2 | 224
                if not getStateThing:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e18ce8 end
            end
            predicateResult = true
        end
        goto FLOW_past_lab_00e18ce8
        ::LAB_00e18ce8::
        predicateResult = false
        ::FLOW_past_lab_00e18ce8::
        if scratchValue < 0 then
            scratchValue = scratchValue & 127
        end
        if scratchValue & 64 ~= 0 then
            scratchValue = scratchValue & 191
        end
        if scratchValue & 32 ~= 0 then
            scratchValue2 = scratchValue & 223
        end
        if predicateResult then
            quest:NewScriptFrame()
        else
            if not quest:IsActiveThreadTerminating() then
                resources:PrepareResource(resource2)
                quest:OverrideMusic(23, false, false)
                quest:GiveThingBestEnemyTarget(getStateThing, hero)
                missionFailed = quest:GetStateBool("MissionFailed")
                goto LAB_00e18d99
            end
            break
            quest:NewScriptFrame()
        end
    until quest:IsActiveThreadTerminating()
    ::LAB_00e18e80::
    resources:ReleaseResource(resource2)
    do return end
    ::LAB_00e18d99::
    if not (missionFailed or quest:GetStateBool("MissionSucceeded")) then
        if not quest:NewScriptFrame() then goto LAB_00e18e80 end
        missionFailed = quest:GetStateBool("MissionFailed")
        goto LAB_00e18d99
    end
    if not quest:IsActiveThreadTerminating() then
        if not quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then goto LAB_00e18e80 end
            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e18e80 end
            quest:SetMasterGameState("WhiteBalverineFinished", true)
        end
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        quest:DisplayQuestInfo(false)
    end
    goto LAB_00e18e80
end

-- Q_WhiteBalverineWW.Init (retail 0x00e18480)
function Init(quest)
    quest:AddQuestRegion("Q_WhiteBalverineWW", "KnotholeGlade")
    quest:AddQuestRegion("Q_WhiteBalverineWW", "Witchwood4")
    quest:AddQuestRegion("Q_WhiteBalverineWW", "DemonDoor_KnotholeGlade")
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("WhiteBalverineAlive", true)
end

-- Q_WhiteBalverineWW.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Q_WhiteBalverineWW.MonitorBalverine (retail 0x00e18eb0)
function MonitorBalverine(quest)
    local pThing = quest:GetStateThing("WhiteBalverine")
    quest:AddQuestInfoBarHealth(pThing, {R = 255, G = 255, B = 255, A = 255}, "HUD_QUEST_ICON_WHITE_BALVERINE", 1.0)
    quest:DisplayQuestInfo(true)
    local scratchValue = pThing ~= nil and pThing:MsgIsKilledBy("")
    while true do
        if scratchValue then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_08", "KnotholeGlade", "KnotholeGlade")
            quest:SetStateBool("WhiteBalverineAlive", false)
            quest:Pause(0.5)
            quest:StopOverrideMusic(false)
            quest:GiveHeroObject("OBJECT_TROPHY_BALVERINE_FM_HEAD_01", -1, false)
            quest:SetCreatureGeneratorsEnabled("Witchwood4", true)
            return
        end
        if not quest:NewScriptFrame() then break end
        scratchValue = pThing ~= nil and pThing:MsgIsKilledBy("")
    end
end

-- Q_WhiteBalverineWW.SpawnBalverines (retail 0x00e19560)
function SpawnBalverines(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult7, scratchValue, scratchValue2, scratchValue5
    local scratchValue6, pPosition, thing, this_01, this_02, resource2, movie3
    local pThing = quest:GetStateThing("WhiteBalverine")
    local function ReleaseEverything()
        resources:DestroyMovie(movie3)
        local this_02 = resource2
        resources:ReleaseResource(this_02)
    end
    predicateResult = false
    predicateResult7 = false
    quest:ModifyThingHealth(pThing, 250.0, false)
    local scratchValue10 = math.tointeger(math.modf(quest:GetHealth(pThing) * 0.25))
    local scratchValue11 = math.tointeger(math.modf(quest:GetHealth(pThing) - scratchValue10))
    scratchValue2 = 1
    scratchValue6 = 1
    repeat
        if not quest:NewScriptFrame() then return end
        if quest:GetHealth(pThing) < scratchValue11 then
            thing = quest:GetThingWithScriptName("MK_WBW_FIRSTSPAWN")
            if quest:IsDistanceBetweenThingsUnder(thing, hero, 10.0) then
                thing = quest:GetThingWithScriptName("MK_WBW_FIRSTSPAWNB")
                predicateResult = true
            end
            scratchValue5 = 0
            if 0 < scratchValue2 then
                repeat
                    if quest:IsActiveThreadTerminating() then return end
                    if not (thing ~= nil and not thing:IsNull()) then
                        pPosition = {x = 0, y = 0, z = 0}
                    else
                        pPosition = thing:GetPos()
                    end
                    quest:GiveThingBestEnemyTarget(quest:CreateCreature("CREATURE_BALVERINE_01", pPosition, "WBWW_SoldierBalverine"), hero)
                    quest:Pause(0.2)
                    scratchValue5 = scratchValue5 + 1
                until scratchValue5 >= scratchValue6
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:EntitySetAsDamageable(pThing, false)
            resource2 = resources:NewResource()
            resources:TryAcquire(resource2, pThing, 4)
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:Pause(0.2)
            quest:EntitySetAttackThingImmediately(pThing, hero, true, true)
            quest:Pause(0.2)
            quest:PlaySoundOnThing(pThing, "SND_LONGWOLFHOWL_01")
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
            -- TODO(native): IsPerformingScriptTask: unresolved entity receiver/resource in quest context; arguments: 
            local scratchValue3 = nil --[[unresolved native result]]
            scratchValue = scratchValue3
            while scratchValue do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything(); return
                end
                -- TODO(native): IsPerformingScriptTask: unresolved entity receiver/resource in quest context; arguments: 
                local scratchValue4 = nil --[[unresolved native result]]
                scratchValue = scratchValue4
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                ReleaseEverything()
                return
            end
            quest:CameraDefault()
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:PrepareResource(resource2)
            resources:ReleaseResource(resource2)
            quest:EntitySetAsDamageable(pThing, true)
            if not predicateResult7 then
                if quest:IsActiveThreadTerminating() then return end
                local resource = resources:NewResource()
                resources:TryAcquire(resource, quest:GetRandomThingWithScriptName("WBWW_SoldierBalverine"), 4)
                if predicateResult then
                    local actorMap = resources:NewActorMap()
                    resources:SetActor(actorMap, "BALV", resource)
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_WBW_FIRSTSPAWNB", actorMap, false, true)
                    quest:Pause(0.1)
                    quest:CameraDefault()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    this_01 = actorMap
                else
                    local actorMap2 = resources:NewActorMap()
                    resources:SetActor(actorMap2, "BALV", resource)
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_WBW_FIRSTSPAWN", actorMap2, false, true)
                    quest:Pause(0.1)
                    quest:CameraDefault()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    this_01 = actorMap2
                end
                resources:DestroyActorMap(this_01)
                predicateResult7 = true
                resources:ReleaseResource(resource)
            end
            scratchValue2 = scratchValue6 + 1
            scratchValue6 = scratchValue2
        end
        if 3 < scratchValue2 then
            return
        end
    until false
end

