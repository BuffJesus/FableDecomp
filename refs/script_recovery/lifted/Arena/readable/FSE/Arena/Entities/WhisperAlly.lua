-- Readable native conversion: WhisperAlly. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- WhisperAlly.Main (retail 0x00f22d80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, c_stk_ed_1, c_stk_ed_2, predicateResult, arenaState
    local scratchValue15, scratchValue16, scratchValue24, timerId2, conversationId6, scratchValue30
    local resource, nearest, summonedCreature, scratchValue33, scratchValue36, scratchValue37
    local scratchValue38, uVar21_b3, scratchValue40, resource4, scratchValue41, resource6, resource7
    local actorMap
    local function ReleaseEverything()
        quest:DeregisterTimer(scratchValue30)
        resources:ReleaseResource(resource4)
    end
    scratchValue40 = 0
    resource4 = resources:NewResource()
    while quest:GetStateBool("WhisperNeededForCutscene") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetTargetingType(me, 26)
    c_stk_ed_1 = 1
    scratchValue30 = quest:RegisterTimer()
    arenaState = quest:GetStateInt("ArenaState")
    timerId2 = scratchValue30
    while arenaState ~= 8 do
        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
        resources:PrepareResource(resource4)
        while not resources:TryAcquire(resource4, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue30)
            resources:ReleaseResource(resource4)
            return
        end
        scratchValue15 = 208
        repeat
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
            scratchValue15 = scratchValue15 + 4
        until scratchValue15 >= 220
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue30)
            resources:ReleaseResource(resource4)
            return
        end
        while scratchValue41 == 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            scratchValue16 = 208
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                scratchValue16 = scratchValue16 + 4
            until scratchValue16 >= 220
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            resources:PrepareResource(resource4)
            while not resources:TryAcquire(resource4, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            if math.random(0, 32767) % 60 ~= 0 then
                if not quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                    me:MoveToPosition(hero:GetPos(), 3.0, ENTITY_MOVE_RUN, false, true)
                end
            end
            if quest:GetStateBool("WhisperNeededForCutscene") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                resources:PrepareResource(resource4)
                while quest:GetStateBool("WhisperNeededForCutscene") do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(scratchValue41)
                        resources:ReleaseResource(resource4)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(resource6)
                    quest:DeregisterTimer(scratchValue41)
                    resources:ReleaseResource(resource4)
                    return
                end
                -- TODO(native): (**(code **)(*(int *)xStack_c0 + 0x5ec))();
                resources:DestroyMovie(movie)
            end
            local scratchValue34 = scratchValue40
            scratchValue40 = scratchValue40 | 1
            if me:MsgIsHitByHero() then
                goto LAB_00f2337d
            else
                scratchValue33 = scratchValue34 | 3
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue33 = scratchValue34 | 7
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f2337d end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00f2337d
            ::LAB_00f2337d::
            predicateResult = true
            ::FLOW_past_lab_00f2337d::
            if scratchValue33 & 4 ~= 0 then
                scratchValue33 = scratchValue33 & 0xfffffffb
            end
            if scratchValue33 & 2 ~= 0 then
                scratchValue33 = scratchValue33 & 0xfffffffd
            end
            if scratchValue33 & 1 ~= 0 then
                scratchValue40 = scratchValue33 & 0xfffffffe
            end
            if predicateResult then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue41); resources:ReleaseResource(resource4); return end
                quest:ModifyThingHealth(me, 10000.0, false)
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
        if nearest ~= nil and (nearest ~= nil and nearest:IsAlive()) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            quest:GiveThingBestEnemyTarget(me, nearest)
        end
        resources:PrepareResource(resource4)
        quest:SetTimer(scratchValue30, 10)
        while scratchValue41 ~= 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            local scratchValue35 = scratchValue40
            scratchValue40 = scratchValue40 | 8
            if me:MsgIsHitByHero() then
                goto LAB_00f235a1
            else
                scratchValue33 = scratchValue35 | 24
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue33 = scratchValue35 | 56
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f235a1 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00f235a1
            ::LAB_00f235a1::
            predicateResult = true
            ::FLOW_past_lab_00f235a1::
            if scratchValue33 & 32 ~= 0 then
                scratchValue33 = scratchValue33 & 0xffffffdf
            end
            if scratchValue33 & 16 ~= 0 then
                scratchValue33 = scratchValue33 & 0xffffffef
            end
            if scratchValue33 & 8 ~= 0 then
                scratchValue33 = scratchValue33 & 0xfffffff7
            end
            if predicateResult then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
            if quest:GetStateInt("ArenaRound") ~= 7 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                local scratchValue10 = nil --[[unresolved native value]]
                if not scratchValue10 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue30); resources:ReleaseResource(resource4); return end
                    nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                    if not (nearest ~= nil and nearest:IsAlive()) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                        nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        if not (nearest ~= nil and nearest:IsAlive()) then goto LAB_00f241aa end
                    end
                    if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(me, nearest); goto LAB_00f241aa end
                    goto LAB_00f243b3
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                if 0 < quest:GetTimer(scratchValue30) then goto LAB_00f241aa end
                if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                if quest:GetHealth(nearest) < 10.0 then
                    scratchValue30 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue30, hero)
                    quest:AddLineToConversation(scratchValue30, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                    scratchValue30 = timerId2
                    goto LAB_00f24190
                end
                goto FLOW_past_lab_00f24190
                ::LAB_00f24190::
                timerId2 = scratchValue30
                quest:SetTimer(scratchValue30, 10)
                goto LAB_00f241aa
                ::FLOW_past_lab_00f24190::
                scratchValue36 = scratchValue33 | 1024
                scratchValue40 = scratchValue36
                if not me:MsgIsHitBy("") then goto LAB_00f2405a end
                scratchValue36 = scratchValue33 | 3072
                scratchValue40 = scratchValue36
                predicateResult = true
                if me:MsgIsHitByHero() then goto LAB_00f2405a end
                goto FLOW_past_lab_00f2405a
                ::LAB_00f2405a::
                predicateResult = false
                ::FLOW_past_lab_00f2405a::
                if scratchValue36 & 2048 ~= 0 then
                    scratchValue36 = scratchValue36 & 0xfffff7ff
                    scratchValue40 = scratchValue36
                end
                if scratchValue36 & 1024 ~= 0 then
                    scratchValue40 = scratchValue36 & 0xfffffbff
                end
                if not predicateResult then
                    -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                    if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue30 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue30, hero)
                        quest:AddLineToConversation(scratchValue30, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                        scratchValue30 = timerId2
                        goto LAB_00f24190
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    scratchValue30 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue30, hero)
                    quest:AddLineToConversation(scratchValue30, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                    scratchValue30 = timerId2
                    goto LAB_00f24190
                end
                goto LAB_00f243b3
            end
            goto FLOW_past_lab_00f243b3
            ::LAB_00f243b3::
            quest:DeregisterTimer(scratchValue41)
            resources:ReleaseResource(resource4)
            do return end
            ::FLOW_past_lab_00f243b3::
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                return
            end
            if c_stk_ed_1 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                local scratchValue11 = nil --[[unresolved native value]]
                if not scratchValue11 then
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
                    if 0 < quest:GetTimer(scratchValue30) then goto LAB_00f241aa end
                    if quest:GetHealth(nearest) < 10.0 then
                        scratchValue41 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue41, hero)
                        quest:AddLineToConversation(scratchValue41, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                        goto LAB_00f23d7f
                    end
                    goto FLOW_past_lab_00f23d7f
                    ::LAB_00f23d7f::
                    quest:SetTimer(scratchValue30, 10)
                    goto LAB_00f241aa
                    ::FLOW_past_lab_00f23d7f::
                    scratchValue37 = scratchValue33 | 256
                    scratchValue40 = scratchValue37
                    if not me:MsgIsHitBy("") then goto LAB_00f23cba end
                    scratchValue37 = scratchValue33 | 768
                    scratchValue40 = scratchValue37
                    predicateResult = true
                    if me:MsgIsHitByHero() then goto LAB_00f23cba end
                    goto FLOW_past_lab_00f23cba
                    ::LAB_00f23cba::
                    predicateResult = false
                    ::FLOW_past_lab_00f23cba::
                    if scratchValue37 & 512 ~= 0 then
                        scratchValue37 = scratchValue37 & 0xfffffdff
                        scratchValue40 = scratchValue37
                    end
                    if scratchValue37 & 256 ~= 0 then
                        scratchValue40 = scratchValue37 & 0xfffffeff
                    end
                    if not predicateResult then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue30 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue30, hero)
                            quest:AddLineToConversation(scratchValue30, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                            scratchValue30 = timerId2
                            quest:SetTimer(timerId2, 10)
                            goto LAB_00f241aa
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        scratchValue41 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue41, hero)
                        quest:AddLineToConversation(scratchValue41, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                        goto LAB_00f23d7f
                    end
                end
                goto LAB_00f243b3
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
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
            if quest:GetTimer(scratchValue30) < 1 then
                if 10.0 <= quest:GetHealth(nearest) then
                    scratchValue38 = scratchValue33 | 64
                    scratchValue40 = scratchValue38
                    if not me:MsgIsHitBy("") then goto LAB_00f2389f end
                    scratchValue38 = scratchValue33 | 192
                    scratchValue40 = scratchValue38
                    predicateResult = true
                    if me:MsgIsHitByHero() then goto LAB_00f2389f end
                    goto FLOW_past_lab_00f2389f
                    ::LAB_00f2389f::
                    predicateResult = false
                    ::FLOW_past_lab_00f2389f::
                    if scratchValue38 & 128 ~= 0 then
                        scratchValue38 = scratchValue38 & 0xffffff7f
                        scratchValue40 = scratchValue38
                    end
                    if scratchValue38 & 64 ~= 0 then
                        scratchValue40 = scratchValue38 & 0xffffffbf
                    end
                    if not predicateResult then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f23a17 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                        scratchValue41 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue41, hero)
                        quest:AddLineToConversation(scratchValue41, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
                    else
                        scratchValue41 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue41, hero)
                        quest:AddLineToConversation(scratchValue41, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
                    end
                else
                    scratchValue41 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue41, hero)
                    quest:AddLineToConversation(scratchValue41, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, hero, false)
                end
                quest:SetTimer(scratchValue30, 10)
            end
            ::LAB_00f23a17::
            ::LAB_00f241aa::
            scratchValue24 = 208
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue30)
                    resources:ReleaseResource(resource4)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                scratchValue24 = scratchValue24 + 4
            until scratchValue24 >= 220
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue30)
                resources:ReleaseResource(resource4)
                do return end
            end
        end
        if not quest:IsActiveThreadTerminating() then
            arenaState = quest:GetStateInt("ArenaState")
        else
            quest:DeregisterTimer(scratchValue30)
            resources:ReleaseResource(resource4)
            do return end
            arenaState = quest:GetStateInt("ArenaState")
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue30)
        resources:ReleaseResource(resource4)
        return
    end
    while not quest:GetStateBool("FinalBattleCS") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue30)
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue30)
        resources:ReleaseResource(resource4)
        return
    end
    resources:PrepareResource(resource4)
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
    local scratchValue = math.tointeger(math.modf(quest:GetHealth(me)))
    c_stk_ed_2 = 0
    local scratchValue2 = math.tointeger(math.modf(quest:GetHealth(me) * 0.25))
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while scratchValue2 < quest:GetHealth(me) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
        if (quest:GetHealth(me) < math.tointeger(math.modf((scratchValue * 3) / 4)) and not uVar21_b3) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_QUARTER", me, hero, false)
            quest:SetTimer(timerId, 13)
        end
        if (quest:GetHealth(me) < math.tointeger(math.modf(scratchValue / 2)) and c_stk_ed_2 == 0) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            local conversationId2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId2, hero)
            quest:AddLineToConversation(conversationId2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_HALF", me, hero, false)
            c_stk_ed_2 = 1
            quest:SetTimer(timerId, 13)
        end
        if quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            if me:MsgIsHitBy("") then
                local conversationId3 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId3, hero)
                quest:AddLineToConversation(conversationId3, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, hero, false)
            else
                -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f249f7 end
                if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
                local conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, hero, false)
            end
            goto LAB_00f249e1
        elseif quest:GetTimer(timerId) < 1 then
            if not quest:IsActiveThreadTerminating() then
                local conversationId5 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId5, hero)
                quest:AddLineToConversation(conversationId5, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHTING_HERO", me, hero, false)
                goto LAB_00f249e1
            end
            goto LAB_00f257fd
        end
        goto FLOW_past_lab_00f249e1
        ::LAB_00f249e1::
        quest:SetTimer(timerId, 15)
        ::FLOW_past_lab_00f249e1::
        ::LAB_00f249f7::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    quest:StopOverrideMusic(false)
    quest:ModifyThingHealth(me, scratchValue2 - quest:GetHealth(me), false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    quest:EntitySetAsDamageable(me, false)
    quest:RemoveQuestInfoElement(infoElement2)
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    resource7 = resources:NewResource()
    resources:PrepareResource(resource7)
    resource = resource7
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00f24bc4 end
        resource = resource7
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00f24bc4
    else
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "Hero", resource7)
        resources:SetActor(actorMap, "Whisper", resource4)
        resources:RunMacro("CS_ARENA_WHISPER_BEFORESTRIKE", actorMap, false, true)
        resources:DestroyActorMap(actorMap)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:ReleaseResource(resource7)
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_10", "Arena", "KnotholeGlade")
        quest:SetThingAsUsable(quest:GetThingWithScriptName("ArenaMainExit"), true)
        quest:OpenDoor(quest:GetThingWithScriptName("ArenaHeroGate"))
        scratchValue41 = quest:RegisterTimer()
        quest:SetTimer(scratchValue41, 0)
        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        quest:EntitySetFacingAngleTowardsThing(hero, me, false)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        while not isActiveThreadTerminating do
            if 0 < quest:GetTimer(scratchValue41) then goto LAB_00f25062 end
            if quest:IsActiveThreadTerminating() then break end
            conversationId6 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId6, hero)
            repeat
                if true then
                    quest:AddLineToConversation(conversationId6, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIRST_PLEAD", me, hero, false)
                    break
                elseif 0 == 1 then
                    quest:AddLineToConversation(conversationId6, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_SECOND_PLEAD", me, hero, false)
                    break
                else
                    if 0 == 2 then
                        quest:AddLineToConversation(conversationId6, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_THIRD_PLEAD", me, hero, false)
                        goto LAB_00f25040
                    elseif 0 == 3 then
                        quest:AddLineToConversation(conversationId6, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FOURTH_PLEAD", me, hero, false)
                        break
                    elseif 0 == 4 then
                        quest:AddLineToConversation(conversationId6, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIFTH_PLEAD", me, hero, false)
                        goto LAB_00f25040
                    end
                    goto FLOW_past_lab_00f25040
                    ::LAB_00f25040::
                    ::FLOW_past_lab_00f25040::
                end
            until true
            quest:SetTimer(scratchValue41, 10)
            ::LAB_00f25062::
            local scratchValue39 = scratchValue40
            scratchValue40 = scratchValue40 | 4096
            if me:MsgIsHitByHero() then
                goto LAB_00f250f0
            else
                scratchValue33 = scratchValue39 | 0x3000
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue33 = scratchValue39 | 0x7000
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f250f0 end
                end
                isActiveThreadTerminating = false
            end
            goto FLOW_past_lab_00f250f0
            ::LAB_00f250f0::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00f250f0::
            if scratchValue33 & 0x4000 ~= 0 then
                scratchValue33 = scratchValue33 & 0xffffbfff
            end
            if scratchValue33 & 0x2000 ~= 0 then
                scratchValue33 = scratchValue33 & 0xffffdfff
            end
            if scratchValue33 & 4096 ~= 0 then
                scratchValue40 = scratchValue33 & 0xffffefff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then break end
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00f257f4
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        break
                    end
                end
                resources:PrepareResource(resource4)
                quest:GiveThingBestEnemyTarget(me, hero)
                quest:EntitySetAsDamageable(me, true)
                quest:EntitySetInFaction(me, "FACTION_MONSTER")
                quest:EntityUnsetThingAsAllyOfThing(me, hero)
                quest:EntityUnsetThingAsAllyOfThing(hero, me)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                quest:DisplayQuestInfo(true)
                local infoElement = quest:AddQuestInfoBar(scratchValue, 0.0, 0xffff0000, {R = 255, G = 0, B = 0, A = 255}, "HUD_WHISPER_ICON", "", 1.0)
                while 1.0 < quest:GetHealth(me) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257f4 end
                    quest:UpdateQuestInfoBar(infoElement, quest:GetHealth(me), -1.0, -1.0)
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(infoElement)
                resource6 = resources:NewResource()
                local resource5 = resources:NewResource()
                resources:PrepareResource(resource6)
                while not resources:TryAcquire(resource6, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                end
                if not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource5)
                    while not resources:TryAcquire(resource5, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("P_CROWD4"), false)
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "Hero", resource6)
                        quest:EntitySetAsDrawable(me, false)
                        local movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        resources:RunMacro("CS_ARENA_WHISPER_KILLED", actorMap, false, true)
                        quest:GiveHeroGold(10000)
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(2800))
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource5)
                        resources:ReleaseResource(resource6)
                        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaCellEntrance2"), false)
                        quest:SetMasterGameState("WhisperKilledByHero", true)
                        quest:RemoveThing(me, false, true)
                        goto LAB_00f2578b
                    end
                end
                ::LAB_00f257e3::
                resources:ReleaseResource(resource5)
                resources:ReleaseResource(resource6)
                break
            end
            ::LAB_00f2578b::
            quest:NewScriptFrame(me)
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        end
        ::LAB_00f257f4::
        quest:DeregisterTimer(scratchValue41)
    end
    goto FLOW_past_lab_00f24bc4
    ::LAB_00f24bc4::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00f24bc4::
    ::LAB_00f257fd::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(scratchValue41)
    resources:ReleaseResource(resource4)
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

