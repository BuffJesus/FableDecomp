-- Readable native conversion: WhisperAlly. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    ArenaKillWhisperMorality = 2800,  -- -0.07999999821186066
}

-- WhisperAlly.Main (retail 0x00f22d80)
function Main(quest, me)
    local resources = quest:RetailResources()
    local addNewConversation, predicateResult48, scratchValue18, creatureTotal, fret_07
    local predicateResult54, infoElement, addNewConversation2, scratchValue22, resource, hero
    local nearest, summonedCreature, uVar21_b, resource2, scratchValue24, movie, resource4
    local resource5, actorMap
    local function ReleaseEverything()
        quest:DeregisterTimer(scratchValue22)
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
    scratchValue18 = 1
    scratchValue22 = quest:RegisterTimer()
    infoElement = quest:GetStateInt("ArenaState")
    addNewConversation2 = scratchValue22
    while infoElement ~= 8 do
        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
        resources:PrepareResource(resource2)
        while not resources:TryAcquire(resource2, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue22)
            resources:ReleaseResource(resource2)
            return
        end
        creatureTotal = 0
        infoElement = 0
        repeat
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            creatureTotal = creatureTotal + quest:GetStateInt("TotalCreatures_" .. infoElement)
            infoElement = infoElement + 1
        until not (infoElement < 3)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue22)
            resources:ReleaseResource(resource2)
            return
        end
        while creatureTotal == 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            creatureTotal = 0
            infoElement = 0
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                creatureTotal = creatureTotal + quest:GetStateInt("TotalCreatures_" .. infoElement)
                infoElement = infoElement + 1
            until not (infoElement < 3)
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            if quest:RetailRandModulo(32768) % 60 ~= 0 then
                resource = quest:GetHero()
                if not quest:IsDistanceBetweenThingsUnder(me, resource, 5.0) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                    local p0 = quest:GetHero():GetPos()
                    me:MoveToPosition(p0, 3.0, ENTITY_MOVE_RUN, false, true)
                end
            end
            if quest:GetStateBool("WhisperNeededForCutscene") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                resources:PrepareResource(resource2)
                while quest:GetStateBool("WhisperNeededForCutscene") do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                resources:PrepareResource(resource2)
                while not resources:TryAcquire(resource2, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resource = quest:GetHero()
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(scratchValue24)
                        resources:ReleaseResource(resource2)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(scratchValue24)
                    resources:ReleaseResource(resource2)
                    return
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00f2337d
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f2337d end
                predicateResult54 = false
            end
            goto FLOW_past_lab_00f2337d
            ::LAB_00f2337d::
            predicateResult54 = true
            ::FLOW_past_lab_00f2337d::
            if predicateResult54 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue24); resources:ReleaseResource(resource2); return end
                quest:ModifyThingHealth(me, 10000.0, false)
                resource = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, resource)
                hero = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
        if nearest ~= nil and (nearest ~= nil and nearest:IsAlive()) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            quest:GiveThingBestEnemyTarget(me, nearest)
        end
        resources:PrepareResource(resource2)
        quest:SetTimer(scratchValue22, 10)
        while creatureTotal ~= 0 and quest:GetStateInt("ArenaState") ~= 8 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                nearest = nil
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            if me:MsgIsHitByHero() then
                goto LAB_00f235a1
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f235a1 end
                predicateResult54 = false
            end
            goto FLOW_past_lab_00f235a1
            ::LAB_00f235a1::
            predicateResult54 = true
            ::FLOW_past_lab_00f235a1::
            if predicateResult54 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                resource = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, resource)
                hero = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(hero, me)
            end
            if quest:GetStateInt("ArenaRound") ~= 7 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                if not (nearest ~= nil and nearest:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue22); resources:ReleaseResource(resource2); return end
                    nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                    if not (nearest ~= nil and nearest:IsAlive()) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                        nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        if not (nearest ~= nil and nearest:IsAlive()) then goto LAB_00f241aa end
                    end
                    if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(me, nearest); goto LAB_00f241aa end
                    goto LAB_00f243b3
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                if 0 < quest:GetTimer(scratchValue22) then goto LAB_00f241aa end
                if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                if quest:GetHealth(nearest) < 10.0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                    scratchValue22 = quest:AddNewConversation(me, false, false)
                    resource = quest:GetHero()
                    quest:AddPersonToConversation(scratchValue22, resource)
                    resource = quest:GetHero()
                    quest:AddLineToConversation(scratchValue22, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, resource, false)
                    scratchValue22 = addNewConversation2
                    goto LAB_00f24190
                end
                goto FLOW_past_lab_00f24190
                ::LAB_00f24190::
                addNewConversation2 = scratchValue22
                quest:SetTimer(scratchValue22, 10)
                goto LAB_00f241aa
                ::FLOW_past_lab_00f24190::
                if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                    if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue22 = quest:AddNewConversation(me, false, false)
                        resource = quest:GetHero()
                        quest:AddPersonToConversation(scratchValue22, resource)
                        resource = quest:GetHero()
                        quest:AddLineToConversation(scratchValue22, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, resource, false)
                        scratchValue22 = addNewConversation2
                        goto LAB_00f24190
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    scratchValue22 = quest:AddNewConversation(me, false, false)
                    resource = quest:GetHero()
                    quest:AddPersonToConversation(scratchValue22, resource)
                    resource = quest:GetHero()
                    quest:AddLineToConversation(scratchValue22, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, resource, false)
                    scratchValue22 = addNewConversation2
                    goto LAB_00f24190
                end
                goto LAB_00f243b3
            end
            goto FLOW_past_lab_00f243b3
            ::LAB_00f243b3::
            quest:DeregisterTimer(scratchValue24)
            resources:ReleaseResource(resource2)
            do return end
            ::FLOW_past_lab_00f243b3::
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            if scratchValue18 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                if not (nearest ~= nil and nearest:IsAlive()) then
                    if not quest:IsActiveThreadTerminating() then
                        nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        if not (nearest ~= nil and nearest:IsAlive()) then
                            if not quest:IsActiveThreadTerminating() then
                                nearest = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                                if nearest ~= nil and nearest:IsAlive() then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                                    quest:GiveThingBestEnemyTarget(me, nearest)
                                    scratchValue18 = 1
                                end
                                goto LAB_00f241aa
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            quest:GiveThingBestEnemyTarget(me, nearest)
                            scratchValue18 = 0
                            goto LAB_00f241aa
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                    if 0 < quest:GetTimer(scratchValue22) then goto LAB_00f241aa end
                    if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                    if quest:GetHealth(nearest) < 10.0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243b3 end
                        scratchValue24 = quest:AddNewConversation(me, false, false)
                        resource = quest:GetHero()
                        quest:AddPersonToConversation(scratchValue24, resource)
                        resource = quest:GetHero()
                        quest:AddLineToConversation(scratchValue24, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, resource, false)
                        goto LAB_00f23d7f
                    end
                    goto FLOW_past_lab_00f23d7f
                    ::LAB_00f23d7f::
                    quest:SetTimer(scratchValue22, 10)
                    goto LAB_00f241aa
                    ::FLOW_past_lab_00f23d7f::
                    if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f241aa end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue22 = quest:AddNewConversation(me, false, false)
                            resource = quest:GetHero()
                            quest:AddPersonToConversation(scratchValue22, resource)
                            resource = quest:GetHero()
                            quest:AddLineToConversation(scratchValue22, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, resource, false)
                            scratchValue22 = addNewConversation2
                            addNewConversation2 = scratchValue22
                            quest:SetTimer(scratchValue22, 10)
                            goto LAB_00f241aa
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        scratchValue24 = quest:AddNewConversation(me, false, false)
                        resource = quest:GetHero()
                        quest:AddPersonToConversation(scratchValue24, resource)
                        resource = quest:GetHero()
                        quest:AddLineToConversation(scratchValue24, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, resource, false)
                        goto LAB_00f23d7f
                    end
                end
                goto LAB_00f243b3
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                return
            end
            summonedCreature = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
            if summonedCreature ~= nil and summonedCreature:IsAlive() then
                if not quest:IsActiveThreadTerminating() then
                    nearest = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                    quest:GiveThingBestEnemyTarget(me, nearest)
                    scratchValue18 = 0
                    goto LAB_00f241aa
                end
                goto LAB_00f243a7
            end
            goto FLOW_past_lab_00f243a7
            ::LAB_00f243a7::
            goto LAB_00f243b3
            ::FLOW_past_lab_00f243a7::
            if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
            if quest:GetTimer(scratchValue22) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                if 10.0 <= quest:GetHealth(nearest) then
                    if not me:MsgIsHitBy("") or me:MsgIsHitByHero() then
                        if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f23a17 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                        scratchValue24 = quest:AddNewConversation(me, false, false)
                        resource = quest:GetHero()
                        quest:AddPersonToConversation(scratchValue24, resource)
                        resource = quest:GetHero()
                        quest:AddLineToConversation(scratchValue24, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, resource, false)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                        scratchValue24 = quest:AddNewConversation(me, false, false)
                        resource = quest:GetHero()
                        quest:AddPersonToConversation(scratchValue24, resource)
                        resource = quest:GetHero()
                        quest:AddLineToConversation(scratchValue24, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, resource, false)
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00f243a7 end
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    resource = quest:GetHero()
                    quest:AddPersonToConversation(scratchValue24, resource)
                    resource = quest:GetHero()
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, resource, false)
                end
                quest:SetTimer(scratchValue22, 10)
            end
            ::LAB_00f23a17::
            ::LAB_00f241aa::
            creatureTotal = 0
            infoElement = 0
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue22)
                    resources:ReleaseResource(resource2)
                    return
                end
                creatureTotal = creatureTotal + quest:GetStateInt("TotalCreatures_" .. infoElement)
                infoElement = infoElement + 1
            until not (infoElement < 3)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue22)
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        local predicateResult = quest:IsActiveThreadTerminating()
        nearest = nil
        if not predicateResult then
            infoElement = quest:GetStateInt("ArenaState")
        else
            nearest = nil
            quest:DeregisterTimer(scratchValue22)
            resources:ReleaseResource(resource2)
            do return end
            infoElement = quest:GetStateInt("ArenaState")
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue22)
        resources:ReleaseResource(resource2)
        return
    end
    while not quest:GetStateBool("FinalBattleCS") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue22)
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(scratchValue22)
        resources:ReleaseResource(resource2)
        return
    end
    resources:PrepareResource(resource2)
    quest:ClearThingHasInformation(me)
    quest:EntitySetInFaction(me, "FACTION_MONSTER")
    resource = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, resource)
    resource = quest:GetHero()
    quest:EntityUnsetThingAsAllyOfThing(me, resource)
    hero = quest:GetHero()
    quest:EntityUnsetThingAsAllyOfThing(hero, me)
    quest:ClearThingHasInformation(me)
    quest:EntitySetTargetingType(me, 58)
    quest:ModifyThingHealth(me, 10000.0, false)
    local healthBar = quest:AddQuestInfoBarHealth(me, {R = 255, G = 255, B = 0, A = 255}, "HUD_WHISPER_ICON", 1.0)
    local fret_v0 = quest:GetHealth(me)
    addNewConversation = math.tointeger(math.modf(fret_v0))
    uVar21_b = false
    scratchValue18 = 0
    local scratchValue25 = addNewConversation
    local fret_02 = quest:GetHealth(me)
    local scratchValue = math.tointeger(math.modf(fret_02 * 0.25))
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while scratchValue < quest:GetHealth(me) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
        if (quest:GetHealth(me) < math.tointeger(math.modf((addNewConversation * 3) / 4)) and not uVar21_b) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            addNewConversation = quest:AddNewConversation(me, false, false)
            resource = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, resource)
            resource = quest:GetHero()
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_QUARTER", me, resource, false)
            uVar21_b = true
            quest:SetTimer(timerId, 13)
            addNewConversation = scratchValue25
        end
        if (quest:GetHealth(me) < math.tointeger(math.modf(addNewConversation / 2)) and scratchValue18 == 0) and quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            addNewConversation = quest:AddNewConversation(me, false, false)
            resource = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, resource)
            resource = quest:GetHero()
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_HALF", me, resource, false)
            scratchValue18 = 1
            quest:SetTimer(timerId, 13)
        end
        if quest:GetTimer(timerId) < 6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
            if me:MsgIsHitBy("") then
                if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
                addNewConversation = quest:AddNewConversation(me, false, false)
                resource = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, resource)
                resource = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, resource, false)
            else
                if not me:MsgHitEnemyWithMeleeWeapon() then goto LAB_00f249f7 end
                if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
                addNewConversation = quest:AddNewConversation(me, false, false)
                resource = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, resource)
                resource = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, resource, false)
            end
            goto LAB_00f249e1
        elseif quest:GetTimer(timerId) < 1 then
            if not quest:IsActiveThreadTerminating() then
                addNewConversation = quest:AddNewConversation(me, false, false)
                resource = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, resource)
                resource = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHTING_HERO", me, resource, false)
                goto LAB_00f249e1
            end
            goto LAB_00f257fd
        end
        goto FLOW_past_lab_00f249e1
        ::LAB_00f249e1::
        quest:SetTimer(timerId, 15)
        ::FLOW_past_lab_00f249e1::
        ::LAB_00f249f7::
        addNewConversation = scratchValue25
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    quest:StopOverrideMusic(false)
    fret_07 = quest:GetHealth(me)
    quest:ModifyThingHealth(me, string.unpack("<f", string.pack("<f", scratchValue - fret_07)), false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resource = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, resource)
    hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(hero, me)
    quest:EntitySetAsDamageable(me, false)
    quest:RemoveQuestInfoElement(healthBar)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00f257fd end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f257fd end
    resource5 = resources:NewResource()
    resources:PrepareResource(resource5)
    addNewConversation2 = 4
    resource = resource5
    while not resources:TryAcquire(resource, quest:GetHero(), addNewConversation2) do
        if not quest:NewScriptFrame(me) then goto LAB_00f24bc4 end
        addNewConversation2 = 4
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
        local pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_10", "Arena", "KnotholeGlade")
        resource = quest:GetThingWithScriptName("ArenaMainExit")
        quest:SetThingAsUsable(resource, true)
        local arenaHeroGate = quest:GetThingWithScriptName("ArenaHeroGate")
        quest:OpenDoor(arenaHeroGate)
        scratchValue24 = quest:RegisterTimer()
        quest:SetTimer(scratchValue24, 0)
        resource = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, resource, false)
        hero = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(hero, me, false)
        while not quest:IsActiveThreadTerminating() do
            if 0 < quest:GetTimer(scratchValue24) then goto LAB_00f25062 end
            if quest:IsActiveThreadTerminating() then break end
            addNewConversation2 = quest:AddNewConversation(me, false, false)
            resource = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation2, resource)
            repeat
                if true then
                    resource = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIRST_PLEAD", me, resource, false)
                    break
                elseif 0 == 1 then
                    resource = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_SECOND_PLEAD", me, resource, false)
                    break
                else
                    if 0 == 2 then
                        resource = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_THIRD_PLEAD", me, resource, false)
                        goto LAB_00f25040
                    elseif 0 == 3 then
                        resource = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FOURTH_PLEAD", me, resource, false)
                        break
                    elseif 0 == 4 then
                        resource = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation2, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIFTH_PLEAD", me, resource, false)
                        goto LAB_00f25040
                    end
                    goto FLOW_past_lab_00f25040
                    ::LAB_00f25040::
                    ::FLOW_past_lab_00f25040::
                end
            until true
            quest:SetTimer(scratchValue24, 10)
            ::LAB_00f25062::
            if me:MsgIsHitByHero() then
                goto LAB_00f250f0
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f250f0 end
                predicateResult48 = false
            end
            goto FLOW_past_lab_00f250f0
            ::LAB_00f250f0::
            predicateResult48 = true
            ::FLOW_past_lab_00f250f0::
            if predicateResult48 then
                if quest:IsActiveThreadTerminating() then break end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resource = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(resource) then
                    resource = quest:GetHero()
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
                resource = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, resource)
                quest:EntitySetAsDamageable(me, true)
                quest:EntitySetInFaction(me, "FACTION_MONSTER")
                resource = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(me, resource)
                hero = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(hero, me)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DisplayQuestInfo(true)
                infoElement = quest:AddQuestInfoBar(scratchValue25, 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_WHISPER_ICON", "", 1.0)
                while 1.0 < quest:GetHealth(me) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257f4 end
                    local fret_10 = quest:GetHealth(me)
                    quest:UpdateQuestInfoBar(infoElement, fret_10, -1.0, -1.0)
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(infoElement)
                resource4 = resources:NewResource()
                local resource3 = resources:NewResource()
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, quest:GetHero(), 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                end
                if not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00f257e3 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        resource = quest:GetThingWithScriptName("P_CROWD4")
                        quest:EntityTeleportToThing(me, resource, false)
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "Hero", resource4)
                        quest:EntitySetAsDrawable(me, false)
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        resources:RunMacro("CS_ARENA_WHISPER_KILLED", actorMap, false, true)
                        quest:GiveHeroGold(10000)
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.ArenaKillWhisperMorality))
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource3)
                        resources:ReleaseResource(resource4)
                        resource = quest:GetThingWithScriptName("ArenaCellEntrance2")
                        hero = quest:GetHero()
                        quest:EntityTeleportToThing(hero, resource, false)
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
        quest:DeregisterTimer(scratchValue24)
    end
    goto FLOW_past_lab_00f24bc4
    ::LAB_00f24bc4::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00f24bc4::
    ::LAB_00f257fd::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(scratchValue24)
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

