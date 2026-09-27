-- Readable native conversion: WB_WhiteBalverine. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local wbCounter

-- WB_WhiteBalverine.Main (retail 0x00e158f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult4, predicateResult, predicateResult17, predicateResult23
    local predicateResult30, timeRemaining, timeRemaining2, scratchValue6, scratchValue8
    local scratchValue, scratchValue12, runPoint, runPoint22, timerId, scratchValue16, timerId2
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    quest:OpinionSourceSetAsExclusive(me, true)
    quest:EntitySetTargetable(me, true)
    predicateResult2 = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult2 then
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetStateInt("BalverineState") == 2 then break end
        if quest:GetStateInt("BalverineState") == 4 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetThingPersistent(me, true)
            quest:GiveThingBestEnemyTarget(me, hero)
            quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
            scratchValue6 = 0
            wbCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
            quest:DisplayQuestInfo(true)
            repeat
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                if me:MsgIsHitByHero() then
                    goto LAB_00e16078
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e16078 end
                    end
                    predicateResult4 = false
                end
                goto FLOW_past_lab_00e16078
                ::LAB_00e16078::
                predicateResult4 = true
                ::FLOW_past_lab_00e16078::
                if predicateResult4 then
                    scratchValue6 = scratchValue6 + 1
                    quest:UpdateQuestInfoCounter(wbCounter, scratchValue6, -1)
                end
            until scratchValue6 >= 10
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:RemoveQuestInfoElement(wbCounter)
            quest:DisplayQuestInfo(false)
            quest:EntitySetTargetable(me, false)
            quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local position = quest:GetThingWithScriptName("WB_EscapePoint2"):GetPos()
            scratchValue16 = {x = position.x, y = position.y, z = position.z}
            local scratchValue7 = resources:ScriptThing(resource)
            if scratchValue7 ~= nil and scratchValue7:IsDistanceFromPositionOver(scratchValue16, 2.5) then
                repeat
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                    me:MoveToPosition(scratchValue16, 0.5, ENTITY_MOVE_RUN, false, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                    end
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    scratchValue8 = resources:ScriptThing(resource)
                until not (scratchValue8 ~= nil and scratchValue8:IsDistanceFromPositionOver(scratchValue16, 2.5))
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
            quest:Pause(1.0)
            quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", me:GetPos(), 0.0, false)
            quest:RemoveThing(me, false, true)
            quest:SetStateInt("BalverineState", 5)
            goto LAB_00e1647d
            resources:ReleaseResource(resource)
            return
        end
        ::LAB_00e1647d::
        if quest:GetStateInt("BalverineState") == 6 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e16964 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e16964 end
            quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
            resources:PrepareResource(resource)
            quest:SetThingPersistent(me, true)
            quest:GiveThingBestEnemyTarget(me, hero)
            scratchValue6 = 0
            wbCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 5, 1.0)
            quest:DisplayQuestInfo(true)
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00e16964 end
                if me:MsgIsHitByHero() then
                    goto LAB_00e16666
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e16666 end
                    end
                    predicateResult = false
                end
                goto FLOW_past_lab_00e16666
                ::LAB_00e16666::
                predicateResult = true
                ::FLOW_past_lab_00e16666::
                if predicateResult then
                    scratchValue6 = scratchValue6 + 1
                    quest:UpdateQuestInfoCounter(wbCounter, scratchValue6, -1)
                end
            until scratchValue6 >= 5
            if quest:IsActiveThreadTerminating() then goto LAB_00e16964 end
            quest:RemoveQuestInfoElement(wbCounter)
            quest:DisplayQuestInfo(false)
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e16964 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e16964 end
            runPoint = quest:GetThingWithScriptName("WB_RunPoint2")
            timerId2 = quest:RegisterTimer()
            timerId = timerId2
            quest:SetTimer(timerId2, 5)
            timeRemaining = quest:GetTimer(timerId)
            while 0 < timeRemaining do
                if not quest:NewScriptFrame(me) then goto LAB_00e1694f end
                if not me:IsPerformingScriptTask() then
                    me:MoveToThing(runPoint, 0, ENTITY_MOVE_RUN)
                end
                if quest:IsCameraPosOnScreen(me:GetPos()) then
                    if not me:IsPerformingScriptTask() then
                        if not quest:IsActiveThreadTerminating() then quest:SetStateInt("BalverineState", 7); goto LAB_00e1687b end
                        goto LAB_00e1694f
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e1694f end
                    quest:SetStateInt("BalverineState", 7)
                    goto LAB_00e1687b
                end
                goto FLOW_past_lab_00e1687b
                ::LAB_00e1687b::
                quest:FadeOutAndKillEntity(me, true, 3.0, true)
                ::FLOW_past_lab_00e1687b::
                timeRemaining = quest:GetTimer(timerId2)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e1694f end
            quest:SetStateInt("BalverineState", 7)
            quest:FadeOutAndKillEntity(me, true, 3.0, true)
            quest:DeregisterTimer(timerId)
            goto LAB_00e168ed
            ::LAB_00e1694f::
            quest:DeregisterTimer(timerId)
            ::LAB_00e16964::
            resources:ReleaseResource(resource)
            return
        end
        ::LAB_00e168ed::
        quest:NewScriptFrame(me)
        predicateResult2 = quest:IsActiveThreadTerminating()
    end
    ::FLOW_after_lab_00e15f22::
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:SetThingPersistent(me, true)
    quest:GiveThingBestEnemyTarget(me, hero)
    scratchValue6 = 0
    wbCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
    quest:DisplayQuestInfo(true)
    repeat
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if me:MsgIsHitByHero() then
            goto LAB_00e15b1a
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e15b1a end
            end
            predicateResult17 = false
        end
        goto FLOW_past_lab_00e15b1a
        ::LAB_00e15b1a::
        predicateResult17 = true
        ::FLOW_past_lab_00e15b1a::
        if predicateResult17 then
            scratchValue6 = scratchValue6 + 1
            quest:UpdateQuestInfoCounter(wbCounter, scratchValue6, -1)
        end
    until scratchValue6 >= 10
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:RemoveQuestInfoElement(wbCounter)
    quest:DisplayQuestInfo(false)
    quest:EntitySetTargetable(me, false)
    quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local position4 = quest:GetThingWithScriptName("WB_EscapePoint1"):GetPos()
    local scratchValue17 = {x = position4.x, y = position4.y, z = position4.z}
    local scratchValue9 = resources:ScriptThing(resource)
    if scratchValue9 ~= nil and scratchValue9:IsDistanceFromPositionOver(scratchValue17, 2.5) then
        repeat
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            me:MoveToPosition(scratchValue17, 0.5, ENTITY_MOVE_RUN, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            scratchValue = resources:ScriptThing(resource)
        until not (scratchValue ~= nil and scratchValue:IsDistanceFromPositionOver(scratchValue17, 2.5))
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
    quest:Pause(1.0)
    quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", me:GetPos(), 0.0, false)
    quest:RemoveThing(me, false, true)
    quest:SetStateInt("BalverineState", 3)
    if quest:GetStateInt("BalverineState") == 4 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:SetThingPersistent(me, true)
        quest:GiveThingBestEnemyTarget(me, hero)
        quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
        scratchValue6 = 0
        wbCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
        quest:DisplayQuestInfo(true)
        repeat
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            if me:MsgIsHitByHero() then
                goto LAB_00e16078_c1
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e16078_c1 end
                end
                predicateResult23 = false
            end
            goto FLOW_past_lab_00e16078_c1
            ::LAB_00e16078_c1::
            predicateResult23 = true
            ::FLOW_past_lab_00e16078_c1::
            if predicateResult23 then
                scratchValue6 = scratchValue6 + 1
                quest:UpdateQuestInfoCounter(wbCounter, scratchValue6, -1)
            end
        until scratchValue6 >= 10
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:RemoveQuestInfoElement(wbCounter)
        quest:DisplayQuestInfo(false)
        quest:EntitySetTargetable(me, false)
        quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        local position6 = quest:GetThingWithScriptName("WB_EscapePoint2"):GetPos()
        scratchValue16 = {x = position6.x, y = position6.y, z = position6.z}
        local scratchValue11 = resources:ScriptThing(resource)
        if scratchValue11 ~= nil and scratchValue11:IsDistanceFromPositionOver(scratchValue16, 2.5) then
            repeat
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                me:MoveToPosition(scratchValue16, 0.5, ENTITY_MOVE_RUN, false, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                scratchValue12 = resources:ScriptThing(resource)
            until not (scratchValue12 ~= nil and scratchValue12:IsDistanceFromPositionOver(scratchValue16, 2.5))
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
        quest:Pause(1.0)
        quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", me:GetPos(), 0.0, false)
        quest:RemoveThing(me, false, true)
        quest:SetStateInt("BalverineState", 5)
        goto LAB_00e1647d_c1
        resources:ReleaseResource(resource)
        return
    end
    ::LAB_00e1647d_c1::
    if quest:GetStateInt("BalverineState") == 6 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00e16964_c1 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e16964_c1 end
        quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
        resources:PrepareResource(resource)
        quest:SetThingPersistent(me, true)
        quest:GiveThingBestEnemyTarget(me, hero)
        scratchValue6 = 0
        wbCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 5, 1.0)
        quest:DisplayQuestInfo(true)
        repeat
            if not quest:NewScriptFrame(me) then goto LAB_00e16964_c1 end
            if me:MsgIsHitByHero() then
                goto LAB_00e16666_c1
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e16666_c1 end
                end
                predicateResult30 = false
            end
            goto FLOW_past_lab_00e16666_c1
            ::LAB_00e16666_c1::
            predicateResult30 = true
            ::FLOW_past_lab_00e16666_c1::
            if predicateResult30 then
                scratchValue6 = scratchValue6 + 1
                quest:UpdateQuestInfoCounter(wbCounter, scratchValue6, -1)
            end
        until scratchValue6 >= 5
        if quest:IsActiveThreadTerminating() then goto LAB_00e16964_c1 end
        quest:RemoveQuestInfoElement(wbCounter)
        quest:DisplayQuestInfo(false)
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00e16964_c1 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e16964_c1 end
        runPoint22 = quest:GetThingWithScriptName("WB_RunPoint2")
        timerId2 = quest:RegisterTimer()
        timerId = timerId2
        quest:SetTimer(timerId2, 5)
        timeRemaining2 = quest:GetTimer(timerId)
        while 0 < timeRemaining2 do
            if not quest:NewScriptFrame(me) then goto LAB_00e1694f_c1 end
            if not me:IsPerformingScriptTask() then
                me:MoveToThing(runPoint22, 0, ENTITY_MOVE_RUN)
            end
            if quest:IsCameraPosOnScreen(me:GetPos()) then
                if not me:IsPerformingScriptTask() then
                    if not quest:IsActiveThreadTerminating() then quest:SetStateInt("BalverineState", 7); goto LAB_00e1687b_c1 end
                    goto LAB_00e1694f_c1
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e1694f_c1 end
                quest:SetStateInt("BalverineState", 7)
                goto LAB_00e1687b_c1
            end
            goto FLOW_past_lab_00e1687b_c1
            ::LAB_00e1687b_c1::
            quest:FadeOutAndKillEntity(me, true, 3.0, true)
            ::FLOW_past_lab_00e1687b_c1::
            timeRemaining2 = quest:GetTimer(timerId2)
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e1694f_c1 end
        quest:SetStateInt("BalverineState", 7)
        quest:FadeOutAndKillEntity(me, true, 3.0, true)
        quest:DeregisterTimer(timerId)
        goto LAB_00e168ed_c1
        ::LAB_00e1694f_c1::
        quest:DeregisterTimer(timerId)
        ::LAB_00e16964_c1::
        resources:ReleaseResource(resource)
        return
    end
    ::LAB_00e168ed_c1::
    quest:NewScriptFrame(me)
    goto FLOW_after_lab_00e15f22
    resources:ReleaseResource(resource)
end

-- WB_WhiteBalverine.Init (retail 0x00e15830)
function Init(quest, me)
    quest:PlayCriteriaSoundOnThing(quest:GetThingWithScriptName("KG_Gate"), "KNG_HORN")
    quest:OverrideMusic(23, false, false)
end

-- WB_WhiteBalverine.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WB_WhiteBalverine.OnPredicateFail (retail 0x00e15800)
function OnPredicateFail(quest, me)
end

