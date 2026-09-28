-- Readable native conversion: WhisperAlly. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- WhisperAlly.Main (retail 0x00f22d80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addNewConversation, predicateResult, getStateBool, c_stk_ed_1, c_stk_ed_2
    local predicateResult54, infoElement, addNewConversation2, scratchValue21, resource, nearest
    local summonedCreature, uVar21_b3, resource2, scratchValue23, movie, resource4, resource5
    local actorMap
    local function ReleaseEverything()
        quest:DeregisterTimer(scratchValue21)
        resources:ReleaseResource(resource2)
    end
    resource2 = resources:NewResource()
    while quest:GetStateBool("WhisperNeededForCutscene") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetTargetingType(me, 26)
    c_stk_ed_1 = 1
    scratchValue21 = quest:RegisterTimer()
    infoElement = quest:GetStateInt("ArenaState")
    addNewConversation2 = scratchValue21
    while infoElement ~= 8 do
        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
        resources:PrepareResource(resource2)
        while not resources:TryAcquire(resource2, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue21)
            resources:ReleaseResource(resource2)
            return
        end
        infoElement = 208
        repeat
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
            infoElement = infoElement + 4
        until infoElement >= 220
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue21)
            resources:ReleaseResource(resource2)
            return
        end
        while scratchValue23 == 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            infoElement = 208
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                infoElement = infoElement + 4
            until infoElement >= 220
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            if math.random(0, 32767) % 60 ~= 0 then
                if not quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                    me:MoveToPosition(hero:GetPos(), 3.0, ENTITY_MOVE_RUN, false, true)
                end
            end
            if quest:GetStateBool("WhisperNeededForCutscene") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                resources:PrepareResource(resource2)
                while quest:GetStateBool("WhisperNeededForCutscene") do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                resources:PrepareResource(resource2)
                while not resources:TryAcquire(resource2, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(scratchValue23)
                        resources:ReleaseResource(resource2)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(scratchValue23)
                    resources:ReleaseResource(resource2)
                    return
                end
                -- TODO(native): (**(code **)(*(int *)xStack_c0 + 0x5ec))();
                resources:DestroyMovie(movie)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00f2337d
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f2337d end
                end
                predicateResult54 = false
            end
            goto FLOW_past_lab_00f2337d
            ::LAB_00f2337d::
            predicateResult54 = true
            ::FLOW_past_lab_00f2337d::
            if predicateResult54 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue23); resources:ReleaseResource(resource2); return end
                quest:ModifyThingHealth(me, 10000.0, false)
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
        if nearest ~= nil and (nearest ~= nil and nearest:IsAlive()) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            quest:GiveThingBestEnemyTarget(me, nearest)
        end
        resources:PrepareResource(resource2)
        quest:SetTimer(scratchValue21, 10)
        while scratchValue23 ~= 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            if me:MsgIsHitByHero() then
                goto LAB_00f235a1
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f235a1 end
                end
                predicateResult54 = false
            end
            goto FLOW_past_lab_00f235a1
            ::LAB_00f235a1::
            predicateResult54 = true
            ::FLOW_past_lab_00f235a1::
            if predicateResult54 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
            if quest:GetStateInt("ArenaRound") ~= 7 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                getStateBool = nil --[[unresolved native value]]
                if not getStateBool then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue21); resources:ReleaseResource(resource2); return end
                    nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                    if not (nearest ~= nil and nearest:IsAlive()) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                        nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        if not (nearest ~= nil and nearest:IsAlive()) then goto LAB_00f241aa end
                    end
                    if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(me, nearest); goto LAB_00f241aa end
                    goto LAB_00f243b3
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                if 0 < quest:GetTimer(scratchValue21) then goto LAB_00f241aa end
                if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                if quest:GetHealth(nearest) < 10.0 then
                    scratchValue21 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue21, hero)
                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                    scratchValue21 = addNewConversation2
                    goto LAB_00f24190
                end
                goto FLOW_past_lab_00f24190
                ::LAB_00f24190::
                addNewConversation2 = scratchValue21
                quest:SetTimer(scratchValue21, 10)
                goto LAB_00f241aa
                ::FLOW_past_lab_00f24190::
                if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                    -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                    if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue21 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue21, hero)
                        quest:AddLineToConversation(scratchValue21, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                        scratchValue21 = addNewConversation2
                        goto LAB_00f24190
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    scratchValue21 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue21, hero)
                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                    scratchValue21 = addNewConversation2
                    goto LAB_00f24190
                end
                goto LAB_00f243b3
            end
            goto FLOW_past_lab_00f243b3
            ::LAB_00f243b3::
            quest:DeregisterTimer(scratchValue23)
            resources:ReleaseResource(resource2)
            do return end
            ::FLOW_past_lab_00f243b3::
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            if c_stk_ed_1 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                getStateBool = nil --[[unresolved native value]]
                if not getStateBool then
                    if not quest:IsActiveThreadTerminating() then
                        nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        if not (nearest ~= nil and nearest:IsAlive()) then
                            if not quest:IsActiveThreadTerminating() then
                                nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                                if nearest ~= nil and nearest:IsAlive() then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                                    quest:GiveThingBestEnemyTarget(me, nearest)
                                    c_stk_ed_1 = 1
                                end
                                goto LAB_00f241aa
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            quest:GiveThingBestEnemyTarget(me, nearest)
                            c_stk_ed_1 = 0
                            goto LAB_00f241aa
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                    if 0 < quest:GetTimer(scratchValue21) then goto LAB_00f241aa end
                    if quest:GetHealth(nearest) < 10.0 then
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                        goto LAB_00f23d7f
                    end
                    goto FLOW_past_lab_00f23d7f
                    ::LAB_00f23d7f::
                    quest:SetTimer(scratchValue21, 10)
                    goto LAB_00f241aa
                    ::FLOW_past_lab_00f23d7f::
                    if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue21 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue21, hero)
                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                            scratchValue21 = addNewConversation2
                            quest:SetTimer(addNewConversation2, 10)
                            goto LAB_00f241aa
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                        goto LAB_00f23d7f
                    end
                end
                goto LAB_00f243b3
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                return
            end
            summonedCreature = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
            if summonedCreature ~= nil and summonedCreature:IsAlive() then
                if not quest:IsActiveThreadTerminating() then
                    nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                    quest:GiveThingBestEnemyTarget(me, nearest)
                    c_stk_ed_1 = 0
                    goto LAB_00f241aa
                end
                goto LAB_00f243a7
            end
            goto FLOW_past_lab_00f243a7
            ::LAB_00f243a7::
            goto LAB_00f243b3
            ::FLOW_past_lab_00f243a7::
            if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
            if quest:GetTimer(scratchValue21) < 1 then
                if 10.0 <= quest:GetHealth(nearest) then
                    if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f23a17 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                    else
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                    end
                else
                    scratchValue23 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue23, hero)
                    quest:AddLineToConversation(scratchValue23, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                end
                quest:SetTimer(scratchValue21, 10)
            end
            ::LAB_00f23a17::
            ::LAB_00f241aa::
            infoElement = 208
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue21)
                    resources:ReleaseResource(resource2)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                infoElement = infoElement + 4
            until infoElement >= 220
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue21)
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if not quest:IsActiveThreadTerminating() then
            infoElement = quest:GetStateInt("ArenaState")
        else
            quest:DeregisterTimer(scratchValue21)
            resources:ReleaseResource(resource2)
            do return end
            infoElement = quest:GetStateInt("ArenaState")
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue21)
        resources:ReleaseResource(resource2)
        return
    end
    while not quest:GetStateBool("FinalBattleCS") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue21)
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue21)
        resources:ReleaseResource(resource2)
        return
    end
    resources:PrepareResource(resource2)
    quest:ClearThingHasInformation(me)
    quest:EntitySetInFaction(me, "FACTION_MONSTER")
    quest:GiveThingBestEnemyTarget(me, hero)
    quest:EntityUnsetThingAsAllyOfThing(me, hero)
    quest:EntityUnsetThingAsAllyOfThing(hero, me)
    quest:ClearThingHasInformation(me)
    quest:EntitySetTargetingType(me, 58)
    quest:ModifyThingHealth(me, 10000.0, false)
    -- TODO(native): xStack_ac = quest:AddQuestInfoBarHealth(me, &0xffffff00, "HUD_WHISPER_ICON", 1.0)
    local infoElement2 = nil --[[unresolved native value]]
    addNewConversation = math.tointeger(math.modf(quest:GetHealth(me)))
    c_stk_ed_2 = 0
    local scratchValue24 = addNewConversation
    local scratchValue = math.tointeger(math.modf(quest:GetHealth(me) * 0.25))
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while scratchValue < quest:GetHealth(me) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
        if (quest:GetHealth(me) < math.tointeger(math.modf((addNewConversation * 3) / 4)) and not uVar21_b3) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_QUARTER", me, hero, false)
            quest:SetTimer(timerId, 13)
            addNewConversation = scratchValue24
        end
        if (quest:GetHealth(me) < math.tointeger(math.modf(addNewConversation / 2)) and c_stk_ed_2 == 0) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_HALF", me, hero, false)
            c_stk_ed_2 = 1
            quest:SetTimer(timerId, 13)
        end
        if quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            if me:MsgIsHitBy("") then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
            else
                -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f249f7 end
                if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
            end
            goto LAB_00f249e1
        elseif quest:GetTimer(timerId) < 1 then
            if not quest:IsActiveThreadTerminating() then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHTING_HERO", me, hero, false)
                goto LAB_00f249e1
            end
            goto LAB_00f257fd
        end
        goto FLOW_past_lab_00f249e1
        ::LAB_00f249e1::
        quest:SetTimer(timerId, 15)
        ::FLOW_past_lab_00f249e1::
        ::LAB_00f249f7::
        addNewConversation = scratchValue24
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    quest:StopOverrideMusic(false)
    quest:ModifyThingHealth(me, scratchValue - quest:GetHealth(me), false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    quest:EntitySetAsDamageable(me, false)
    quest:RemoveQuestInfoElement(infoElement2)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    resource5 = resources:NewResource()
    resources:PrepareResource(resource5)
    resource = resource5
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00f24bc4 end
        resource = resource5
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00f24bc4
    else
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "Hero", resource5)
        resources:SetActor(actorMap, "Whisper", resource2)
        resources:RunMacro("CS_ARENA_WHISPER_BEFORESTRIKE", actorMap, false, true)
        resources:DestroyActorMap(actorMap)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource5)
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_10", "Arena", "KnotholeGlade")
        quest:SetThingAsUsable(quest:GetThingWithScriptName("ArenaMainExit"), true)
        quest:OpenDoor(quest:GetThingWithScriptName("ArenaHeroGate"))
        scratchValue23 = quest:RegisterTimer()
        quest:SetTimer(scratchValue23, 0)
        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        quest:EntitySetFacingAngleTowardsThing(hero, me, false)
        while not quest:IsActiveThreadTerminating() do
            if 0 < quest:GetTimer(scratchValue23) then goto LAB_00f25062 end
            addNewConversation2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation2, hero)
            repeat
                if true then
                    quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIRST_PLEAD", me, hero, false)
                    break
                elseif 0 == 1 then
                    quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_SECOND_PLEAD", me, hero, false)
                    break
                else
                    if 0 == 2 then
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_THIRD_PLEAD", me, hero, false)
                        goto LAB_00f25040
                    elseif 0 == 3 then
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FOURTH_PLEAD", me, hero, false)
                        break
                    elseif 0 == 4 then
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIFTH_PLEAD", me, hero, false)
                        goto LAB_00f25040
                    end
                    goto FLOW_past_lab_00f25040
                    ::LAB_00f25040::
                    ::FLOW_past_lab_00f25040::
                end
            until true
            quest:SetTimer(scratchValue23, 10)
            ::LAB_00f25062::
            if me:MsgIsHitByHero() then
                goto LAB_00f250f0
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f250f0 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00f250f0
            ::LAB_00f250f0::
            predicateResult = true
            ::FLOW_past_lab_00f250f0::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then break end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00f257f4
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        break
                    end
                end
                resources:PrepareResource(resource2)
                quest:GiveThingBestEnemyTarget(me, hero)
                quest:EntitySetAsDamageable(me, true)
                quest:EntitySetInFaction(me, "FACTION_MONSTER")
                quest:EntityUnsetThingAsAllyOfThing(me, hero)
                quest:EntityUnsetThingAsAllyOfThing(hero, me)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DisplayQuestInfo(true)
                infoElement = quest:AddQuestInfoBar(scratchValue24, 0.0, 0xffff0000, {R = 255, G = 0, B = 0, A = 255}, "HUD_WHISPER_ICON", "", 1.0)
                while 1.0 < quest:GetHealth(me) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257f4 end
                    quest:UpdateQuestInfoBar(infoElement, quest:GetHealth(me), -1.0, -1.0)
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(infoElement)
                resource4 = resources:NewResource()
                local resource3 = resources:NewResource()
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                end
                if not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("P_CROWD4"), false)
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "Hero", resource4)
                        quest:EntitySetAsDrawable(me, false)
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        resources:RunMacro("CS_ARENA_WHISPER_KILLED", actorMap, false, true)
                        quest:GiveHeroGold(10000)
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(2800))
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource3)
                        resources:ReleaseResource(resource4)
                        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaCellEntrance2"), false)
                        quest:SetMasterGameState("WhisperKilledByHero", true)
                        quest:RemoveThing(me, false, true)
                        goto LAB_00f2578b
                    end
                end
                ::LAB_00f257e3::
                resources:ReleaseResource(resource3)
                resources:ReleaseResource(resource4)
                break
            end
            ::LAB_00f2578b::
            quest:NewScriptFrame(me)
        end
        ::LAB_00f257f4::
        quest:DeregisterTimer(scratchValue23)
    end
    goto FLOW_past_lab_00f24bc4
    ::LAB_00f24bc4::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00f24bc4::
    ::LAB_00f257fd::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(scratchValue23)
    resources:ReleaseResource(resource2)
end

-- WhisperAlly.Init (retail 0x00f1b8b0)
function Init(quest, me)
end

-- WhisperAlly.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WhisperAlly.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

