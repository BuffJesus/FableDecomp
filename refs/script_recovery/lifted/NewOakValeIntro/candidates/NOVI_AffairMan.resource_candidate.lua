-- DISABLED CANDIDATE: resource-aware NOVI_AffairMan generated from the converter draft
-- plus the verified native resource map (native_affair_man_resources_witness.json).
-- Not registered, not installed. Requires the unapplied LuaRetailResources extension:
-- MoveToPosition, PlayAnimation, ClearCommands, ClearAllActions, ThingIsDistanceFromPositionOver, ReadAnimationArgument5, IsHitByHeroExceptAbility, ThingsAreWithinDistance, FaceThing, AddConversationPerson, AddConversationLine.
-- One retail resource local (man_resource) spans Main: constructed after the entry
-- termination check, prepared/acquired at the three native sites, used by every
-- speech, task query, animation, move and clear call. Normal exits release at the
-- native join; errors use scope Close, including movie destruction and unpause.
-- Speech is the native non-waiting wrapper followed by the retail task poll.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("BadgerIndex", 0)
    __native_entity_state:SetStateBool("EncounterOver", false)
    __native_entity_state:SetStateBool("SaidFirstRangedComment", false)
    __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", false)
    __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:SetThingHasInformation(me, false, false, false)
end

local function __resource_main(quest, me, resources)
    local man_resource, man_thing, man_movie, bVar4, cVar5, fVar18, fVar25, fVar3, iVar10, native_arg_man_cleanup_mask, native_arg_man_conversation_id, native_arg_man_hit, native_arg_man_home_position, native_arg_man_line_index, native_arg_man_line_terminating, native_arg_man_near_key, native_arg_man_ranged_index, native_arg_man_ranged_key, native_arg_man_talk_terminating, native_arg_sequence_1, native_arg_woman_near_key, native_arg_woman_ranged_key, pCVar1, pCVar12, pCVar13, pCVar20, pCVar21, pCVar22, pCVar26, pCVar7, pcVar19, ppVar24, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, uVar14, uVar17, uVar6, uVar8, uVar9
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        man_resource = resources:NewResource()
        resources:PrepareResource(man_resource)
        cVar5 = resources:TryAcquire(man_resource, me, 4)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db1d8e end
            cVar5 = resources:TryAcquire(man_resource, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            r1 = resources:NewThingFromScriptName("NOVI_AffairWoman")
            r2 = resources:NewThingFromScriptName("NOVI_AffairWife")
            native_arg_man_conversation_id = 0
            alive = not quest:IsActiveThreadTerminating()
            uVar14 = 0
            if alive then
                ::LAB_00db0b30::
                native_arg_man_hit = resources:IsHitByHeroExceptAbility(me, 14)
                if native_arg_man_hit then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    pCVar20 = ""
                    man_movie = resources:StartMovie("")
                    resources:Pause(true)
                    resources:PrepareResource(man_resource)
                    cVar5 = resources:TryAcquire(man_resource, me, 4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                        cVar5 = resources:TryAcquire(man_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        man_thing = resources:NewThingFromResource(man_resource)
                        fVar18 = resources:ThingHealth(man_thing)
                        resources:DestroyThing(man_thing)
                        fVar3 = 0.0
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_AFFAIRMAN_ON_HIT"
                            pCVar7 = quest:GetHero()
                            resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(man_movie)
                                goto LAB_00db1d7c
                            end
                        end
                        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                        resources:Pause(false)
                        resources:DestroyMovie(man_movie)
                        goto LAB_00db1c71
                    end
                    resources:Pause(false)
                    resources:DestroyMovie(man_movie)
                    goto LAB_00db1d7c
                end
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    pCVar20 = ""
                    man_movie = resources:StartMovie("")
                    pCVar21 = 0x1
                    -- TODO(native): piStack_12c = piVar2;
                    resources:Pause((pCVar21 ~= 0))
                    resources:PrepareResource(man_resource)
                    cVar5 = resources:TryAcquire(man_resource, me, 4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                        cVar5 = resources:TryAcquire(man_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        resources:Pause(false)
                        resources:DestroyMovie(man_movie)
                        goto LAB_00db1d7c
                    end
                    resources:ClearAllActions(man_resource)
                    resources:ClearCommands(man_resource)
                    bVar4 = resources:ThingsAreWithinDistance(me, r2, 5.0)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                        if not __native_entity_state:GetStateBool("HeroAgreedToKeepQuiet") then
                            if __native_entity_state:GetStateBool("HeroSaidHeWouldReportMan") then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    man_thing = resources:NewThingFromResource(man_resource)
                                    fVar18 = resources:ThingHealth(man_thing)
                                    resources:DestroyThing(man_thing)
                                    fVar3 = 0.0
                                    if fVar3 < fVar18 then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_AFFAIRMAN_SOME_NERVE"
                                        pCVar7 = quest:GetHero()
                                        resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = resources:IsPerformingScriptTask(man_resource)
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    goto LAB_00db1d7c
                                                end
                                                bVar4 = resources:IsPerformingScriptTask(man_resource)
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(man_movie)
                                            goto LAB_00db1d7c
                                        end
                                    end
                                    quest:ClearThingHasInformation(me)
                                    goto LAB_00db1593
                                end
                                -- LAB_00db1085: (native jump target)
                                resources:Pause(false)
                                resources:DestroyMovie(man_movie)
                                goto LAB_00db1d7c
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                man_thing = resources:NewThingFromResource(man_resource)
                                fVar18 = resources:ThingHealth(man_thing)
                                resources:DestroyThing(man_thing)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT"
                                    pCVar7 = quest:GetHero()
                                    resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    native_arg_man_talk_terminating = not alive
                                    if native_arg_man_talk_terminating then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                    quest:ClearThingHasInformation(me)
                                    goto LAB_00db1593
                                end
                                quest:ClearThingHasInformation(me)
                                goto LAB_00db1593
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                man_thing = resources:NewThingFromResource(man_resource)
                                fVar18 = resources:ThingHealth(man_thing)
                                resources:DestroyThing(man_thing)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_HAD_A_DEAL"
                                    pCVar7 = quest:GetHero()
                                    resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                                        until not (bVar4)
                                    end
                                    -- LAB_00db1144: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                end
                                -- LAB_00db1153: (native jump target)
                                quest:ClearThingHasInformation(me)
                                goto LAB_00db1593
                            end
                        end
                        -- LAB_00db1cf3: (native jump target)
                        resources:Pause(false)
                        resources:DestroyMovie(man_movie)
                        goto LAB_00db1d7c
                    end
                    if not __native_entity_state:GetStateBool("EncounterOver") then
                        alive = not quest:IsActiveThreadTerminating()
                        uVar8 = 0
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                        uVar9 = quest:GetHero()
                        resources:FaceThing(r1, uVar9, false)
                        man_thing = resources:NewThingFromResource(man_resource)
                        fVar18 = resources:ThingHealth(man_thing)
                        resources:DestroyThing(man_thing)
                        fVar3 = 0.0
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_AFFAIRMAN_INTRO"
                            pCVar7 = quest:GetHero()
                            resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(man_movie)
                                goto LAB_00db1d7c
                            end
                        end
                        quest:SetStateBool("HeroDiscoveredInfidelity", true)
                        pCVar20 = "TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL"
                        quest:GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        uVar6 = uVar8
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(man_movie)
                                goto LAB_00db1d7c
                            end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            uVar6 = uVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            alive = not quest:IsActiveThreadTerminating()
                            if iVar10 == 1 then
                                if not alive then
                                    -- LAB_00db1d23: (native jump target)
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    goto LAB_00db1d7c
                                end
                                man_thing = resources:NewThingFromResource(man_resource)
                                fVar18 = resources:ThingHealth(man_thing)
                                resources:DestroyThing(man_thing)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_GOOD_LAD"
                                    pCVar7 = quest:GetHero()
                                    resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                end
                                require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 4)
                                quest:GiveHeroGold(1)
                                __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", true)
                            else
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    goto LAB_00db1d7c
                                end
                                man_thing = resources:NewThingFromResource(man_resource)
                                fVar18 = resources:ThingHealth(man_thing)
                                resources:DestroyThing(man_thing)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_DONT_TELL"
                                    pCVar7 = quest:GetHero()
                                    resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = resources:IsPerformingScriptTask(man_resource)
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = resources:IsPerformingScriptTask(man_resource)
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        goto LAB_00db1d7c
                                    end
                                end
                                __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", true)
                            end
                            quest:ClearThingHasInformation(me)
                            __native_entity_state:SetStateBool("EncounterOver", true)
                            goto LAB_00db1593
                        end
                        -- LAB_00db1d0a: (native jump target)
                        resources:Pause(false)
                        resources:DestroyMovie(man_movie)
                        goto LAB_00db1d7c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        resources:Pause(false)
                        resources:DestroyMovie(man_movie)
                        goto LAB_00db1d7c
                    end
                    man_thing = resources:NewThingFromResource(man_resource)
                    fVar18 = resources:ThingHealth(man_thing)
                    resources:DestroyThing(man_thing)
                    fVar3 = 0.0
                    if fVar3 < fVar18 then
                        bVar4 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar20 = 0x0
                        pcVar19 = "TEXT_QST_048_AFFAIRMAN_SHOO"
                        pCVar7 = quest:GetHero()
                        resources:Speak(man_resource, pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                        bVar4 = resources:IsPerformingScriptTask(man_resource)
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    goto LAB_00db1d7c
                                end
                                bVar4 = resources:IsPerformingScriptTask(man_resource)
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                    end
                    ::LAB_00db1593::
                    bVar4 = resources:ThingAlive(r1)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                        resources:FaceThing(r1, me, false)
                    else
                        bVar4 = resources:ThingAlive(r2)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                resources:FaceThing(r2, me, false)
                                goto LAB_00db15e6
                            end
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            goto LAB_00db1d7c
                        end
                    end
                    ::LAB_00db15e6::
                    resources:Pause(false)
                    resources:DestroyMovie(man_movie)
                    goto LAB_00db1c71
                end
                bVar4 = resources:IsPerformingScriptTask(man_resource)
                if bVar4 then goto LAB_00db1c71 end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db1d7c end
                bVar4 = resources:ThingsAreWithinDistance(me, r2, 5.0)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    resources:FaceThing(me, r2, false)
                    iVar10 = quest:RetailRandModulo(50)
                    if iVar10 ~= 0 then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    resources:PlayAnimation(man_resource, "ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, resources:ReadAnimationArgument5(), false, false)
                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_b8;
                else
                    fVar25 = 0.1
                    pCVar12 = me:GetHomePos()
                    bVar4 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar12, fVar25))
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        native_arg_man_home_position = me:GetHomePos()
                        fVar25 = 2.0
                        man_thing = resources:NewThingFromResource(man_resource)
                        bVar4 = resources:ThingIsDistanceFromPositionOver(man_thing, native_arg_man_home_position, 2.0)
                        resources:DestroyThing(man_thing)
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                resources:MoveToPosition(man_resource, native_arg_man_home_position, 0.0, 0, false, true)
                                bVar4 = resources:IsPerformingScriptTask(man_resource)
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db1d7c end
                                        bVar4 = resources:IsPerformingScriptTask(man_resource)
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                fVar25 = 2.0
                                man_thing = resources:NewThingFromResource(man_resource)
                                bVar4 = resources:ThingIsDistanceFromPositionOver(man_thing, native_arg_man_home_position, 2.0)
                                resources:DestroyThing(man_thing)
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        goto LAB_00db1c71
                    end
                    cVar5 = quest:IsConversationActive(native_arg_man_conversation_id)
                    native_arg_sequence_1 = false
                    if cVar5 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if not native_arg_sequence_1 then
                        if __native_entity_state:GetStateBool("SaidFirstRangedComment") then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if native_arg_sequence_1 then
                            iVar10 = quest:RetailRandModulo(100)
                            if iVar10 ~= 0 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if not native_arg_sequence_1 then
                            bVar4 = resources:ThingsAreWithinDistance(me, r1, 2.0)
                            if not bVar4 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    fVar25 = 5.0
                    pCVar7 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, 5.0)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        uVar8 = quest:AddNewConversation(me, false, false)
                        native_arg_man_conversation_id = uVar8
                        resources:AddConversationPerson(uVar8, r1)
                        iVar10 = __native_entity_state:GetStateInt("BadgerIndex")
                        __native_entity_state:SetStateInt("BadgerIndex", iVar10 + 10)
                        if 0x32 < iVar10 + 10 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db1d7c end
                            __native_entity_state:SetStateInt("BadgerIndex", 10)
                        end
                        native_arg_man_line_index = __native_entity_state:GetStateInt("BadgerIndex")
                        native_arg_man_near_key = "TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_" .. tostring(native_arg_man_line_index)
                        native_arg_woman_near_key = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_" .. tostring(native_arg_man_line_index)
                        if native_arg_man_line_index == 10 then
                            alive = not quest:IsActiveThreadTerminating()
                            native_arg_man_line_terminating = not alive
                            if native_arg_man_line_terminating then goto LAB_00db1d7c end
                            resources:AddConversationLine(uVar8, native_arg_woman_near_key, r1, me, false)
                            resources:AddConversationLine(uVar8, native_arg_man_near_key, me, r1, false)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            native_arg_man_line_terminating = not alive
                            if native_arg_man_line_terminating then goto LAB_00db1d7c end
                            resources:AddConversationLine(uVar8, native_arg_man_near_key, me, r1, false)
                            resources:AddConversationLine(uVar8, native_arg_woman_near_key, r1, me, false)
                        end
                        quest:SetStateBool("HeroDiscoveredInfidelity", true)
                    else
                        fVar25 = 13.0
                        pCVar7 = quest:GetHero()
                        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, 13.0)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db1d7c end
                            __native_entity_state:SetStateBool("SaidFirstRangedComment", true)
                            uVar8 = quest:AddNewConversation(me, false, false)
                            native_arg_man_conversation_id = uVar8
                            resources:AddConversationPerson(uVar8, r1)
                            iVar10 = __native_entity_state:GetStateInt("BadgerIndex")
                            __native_entity_state:SetStateInt("BadgerIndex", iVar10 + 10)
                            if 0x32 < iVar10 + 10 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                __native_entity_state:SetStateInt("BadgerIndex", 10)
                            end
                            native_arg_man_ranged_index = __native_entity_state:GetStateInt("BadgerIndex")
                            native_arg_man_ranged_key = "TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_" .. tostring(native_arg_man_ranged_index)
                            native_arg_woman_ranged_key = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_" .. tostring(native_arg_man_ranged_index)
                            resources:AddConversationLine(uVar8, native_arg_man_ranged_key, me, r1, false)
                            resources:AddConversationLine(uVar8, native_arg_woman_ranged_key, r1, me, false)
                        end
                    end
                    if quest:GetStateBool("TalkingToWoman") then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    quest:Pause(0.4000000059604645)
                    resources:FaceThing(me, r1, true)
                    resources:FaceThing(r1, me, true)
                    uVar14 = quest:RetailRandModulo(2)
                    bVar4 = uVar14 == 0
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            quest:SetStateBool("ReceiveKiss", true)
                            resources:PlayAnimation(man_resource, "GIVE_KISS", false, true, false, true, resources:ReadAnimationArgument5(), false, false)
                            -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e4;
                            goto LAB_00db1c6c
                        end
                        goto LAB_00db1d7c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    quest:SetStateBool("ReceiveHug", true)
                    resources:PlayAnimation(man_resource, "GIVE_HUG", false, true, false, true, resources:ReadAnimationArgument5(), false, false)
                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_dc;
                end
                ::LAB_00db1c6c::
                ::LAB_00db1c71::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db1d7c end
                goto LAB_00db0b30
            end
            ::LAB_00db1d7c::
        resources:DestroyThing(r2)
        resources:DestroyThing(r1)
        end
        ::LAB_00db1d8e::
        resources:ReleaseResource(man_resource)
    end
end

function Main(quest, me)
    -- Normal exits release at DB1D92's join; Close is a fallback for Lua errors.
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
