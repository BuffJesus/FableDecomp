-- Generated native draft: FishermansWife. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local __native_condition_1, __native_condition_2, bVar5, cVar6, fVar18, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar15, iVar19, iVar7, iVar9, p0, p0_00, pCVar12, pCVar16, pCVar8, pQuestName, pcVar14, pppuVar17, puVar11, r1, r10, r11, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar13, uVar4, u_stk_164, xStack_120, xStack_130, xStack_174, xStack_178, xStack_28, xStack_38, xStack_50, xStack_5c, xStack_68, xStack_78, xStack_9c, xStack_b0, xStack_c4, x_stk_18, x_stk_44, x_stk_84
    local alive = true
    u_stk_164 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_174 = resources:NewResource()
    resources:PrepareResource(xStack_174)
    bVar5 = resources:TryAcquire(xStack_174, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00d8d583 end
        bVar5 = resources:TryAcquire(xStack_174, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00d8d583 end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    r1 = quest:GetThingWithScriptName("HiddenBooty")
    xStack_178 = quest:RegisterTimer()
    quest:SetTimer(xStack_178, 5)
    cVar6 = __native_entity_state:GetStateBool("LeavingHappy")
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00d8d571 end
        iVar7 = quest:GetTimer(xStack_178)
        if iVar7 < 1 then
            fVar18 = 5.5
            pCVar8 = quest:GetHero()
            bVar5 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar18)
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d8d571 end
                iVar9 = quest:AddNewConversation(me, false, false)
                pCVar8 = quest:GetHero()
                quest:AddPersonToConversation(iVar9, pCVar8)
                pCVar8 = quest:GetHero()
                quest:AddLineToConversation(iVar9, "TEXT_QST_032_FISHERWIFE_CRY", me, pCVar8, false)
                quest:SetTimer(xStack_178, 0xf)
            end
        end
        bVar5 = me:IsTalkedToByHero()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d8d571 end
            if not quest:GetStateBool("Helped") then
                __native_condition_2 = not quest:GetStateBool("Helping")
                if not __native_condition_2 then
                    bVar5 = quest:IsDiggingSpotEnabled(r1)
                    __native_condition_2 = bVar5
                end
                __native_condition_1 = __native_condition_2
                if not __native_condition_1 then
                    iVar7 = quest:GetHeroGold()
                    __native_condition_1 = iVar7 < 500
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8d571 end
                    xStack_b0 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if not __native_entity_state:GetStateBool("TalkedTo") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00d8cffe: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_b0)
                            goto LAB_00d8d571
                        end
                        iVar9 = 4
                        xStack_130 = resources:NewResource()
                        pCVar8 = quest:GetHero()
                        resources:TryAcquire(pppuVar17, pCVar8, iVar9)
                        xStack_78 = resources:NewActorMap()
                        resources:SetActor(xStack_78, "HERO", xStack_130)
                        resources:SetActor(xStack_78, "WIFE", xStack_174)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GHOSTFISH_WIFE_INTRO", xStack_78, false, true)
                        quest:FixMovieSequenceCamera(false)
                        __native_entity_state:SetStateBool("TalkedTo", true)
                        resources:DestroyActorMap(xStack_78)
                        resources:ReleaseResource(xStack_130)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_b0)
                            goto LAB_00d8d571
                        end
                        xStack_68 = resources:ScriptThing(xStack_174)
                        pCVar8 = xStack_68
                        fret_01 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_01 then
                            iVar19 = 0
                            iVar15 = 1
                            iVar9 = 0
                            iVar7 = 0
                            pcVar14 = "TEXT_QST_032_FISHERWIFE_REPEAT_GREET"
                            pCVar8 = quest:GetHero()
                            r2 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_b0)
                                    goto LAB_00d8d571
                                end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_b0)
                                goto LAB_00d8d571
                            end
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar12 = xStack_b0
                    goto LAB_00d8ca0f
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d8d571 end
                xStack_120 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:GiveHeroYesNoQuestion("TEXT_QST_032_FISHERWIFE_GOLD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar7 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_120)
                        goto LAB_00d8d571
                    end
                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_120)
                    goto LAB_00d8d571
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if iVar7 == 1 then
                    if not bVar5 then
                        quest:SetStateBool("Helped", true)
                        xStack_28 = resources:NewResource()
                        iVar9 = 4
                        pCVar16 = xStack_28
                        pCVar8 = quest:GetHero()
                        resources:TryAcquire(pCVar16, pCVar8, iVar9)
                        xStack_5c = resources:NewActorMap()
                        resources:SetActor(xStack_5c, "HERO", xStack_28)
                        resources:SetActor(xStack_5c, "WIFE", xStack_174)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GHOSTFISH_WIFE_SUCCESS", xStack_5c, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xf60))
                        quest:AddItemToContainer(me, "OBJECT_GOLDBAG_MEDIUM_WITH_COINS_500")
                        quest:GiveHeroGold(-500)
                        pQuestName = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_03", "", "OakBay")
                        quest:ClearThingHasInformation(me)
                        quest:MiniMapRemoveMarker(me)
                        pCVar8 = quest:GetThingWithScriptName("GhostFisherman")
                        quest:MiniMapAddMarker(pCVar8, "HUD_ORB_QUEST_VIGNETTE")
                        quest:FadeScreenIn()
                        __native_entity_state:SetStateBool("LeavingHappy", true)
                        resources:DestroyActorMap(xStack_5c)
                        this_00 = xStack_28
                        goto LAB_00d8c74b
                    end
                    goto FLOW_hoist_lab_00d8c74b_1
                end
                goto FLOW_past_lab_00d8c74b
                ::LAB_00d8c74b::
                resources:ReleaseResource(this_00)
                goto LAB_00d8c750
                ::FLOW_hoist_lab_00d8c74b_1::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_120)
                goto LAB_00d8d571
                ::FLOW_past_lab_00d8c74b::
                if bVar5 then
                    -- LAB_00d8cfc3: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_120)
                    goto LAB_00d8d571
                end
                if not __native_entity_state:GetStateBool("TalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_38 = resources:NewResource()
                        iVar9 = 4
                        pCVar16 = xStack_38
                        pCVar8 = quest:GetHero()
                        resources:TryAcquire(pCVar16, pCVar8, iVar9)
                        xStack_50 = resources:NewActorMap()
                        resources:SetActor(xStack_50, "HERO", xStack_38)
                        resources:SetActor(xStack_50, "WIFE", xStack_174)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GHOSTFISH_WIFE_INTRO", xStack_50, false, true)
                        quest:FixMovieSequenceCamera(false)
                        __native_entity_state:SetStateBool("TalkedTo", true)
                        resources:DestroyActorMap(xStack_50)
                        this_00 = xStack_38
                        goto LAB_00d8c74b
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_120)
                    goto LAB_00d8d571
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00d8c604: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_120)
                    goto LAB_00d8d571
                end
                x_stk_44 = resources:ScriptThing(xStack_174)
                pCVar8 = x_stk_44
                fret_00 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_00 then
                    iVar19 = 0
                    iVar15 = 1
                    iVar9 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_032_FISHERWIFE_REPEAT_GREET"
                    pCVar8 = quest:GetHero()
                    r3 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_120)
                            goto LAB_00d8d571
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_120)
                        goto LAB_00d8d571
                    end
                end
                ::LAB_00d8c750::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_120)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d8d571 end
                xStack_c4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_18 = resources:ScriptThing(xStack_174)
                pCVar8 = x_stk_18
                fret_0 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_0 then
                    iVar19 = 0
                    iVar15 = 1
                    iVar9 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_032_FISHERWIFE_THANKS"
                    pCVar8 = quest:GetHero()
                    r4 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar12 = xStack_c4
                            quest:DeregisterTimer(xStack_178)
                            goto LAB_00d8d57a
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_c4)
                        goto LAB_00d8d571
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                pCVar12 = xStack_c4
                goto LAB_00d8ca0f
            end
            goto FLOW_past_lab_00d8ca0f
            ::LAB_00d8ca0f::
            ::FLOW_past_lab_00d8ca0f::
            quest:EntitySetFacingAngle(me, __native_entity_state:GetStateFloat("initial_angle"), true)
        end
        uVar4 = u_stk_164
        u_stk_164 = u_stk_164 | 1
        bVar5 = me:MsgIsHitByHero()
        if bVar5 then
            goto LAB_00d8cab0
        else
            uVar13 = uVar4 | 3
            u_stk_164 = uVar13
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                uVar13 = uVar4 | 7
                u_stk_164 = uVar13
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00d8cab0 end
            end
            bVar5 = false
        end
        goto FLOW_past_lab_00d8cab0
        ::LAB_00d8cab0::
        bVar5 = true
        ::FLOW_past_lab_00d8cab0::
        if (uVar13 & 4) ~= 0 then
            uVar13 = uVar13 & 0xfffffffb
            u_stk_164 = uVar13
        end
        if (uVar13 & 2) ~= 0 then
            uVar13 = uVar13 & 0xfffffffd
            u_stk_164 = uVar13
        end
        if (uVar13 & 1) ~= 0 then
            u_stk_164 = uVar13 & 0xfffffffe
        end
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d8d571 end
            xStack_9c = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            x_stk_84 = resources:ScriptThing(xStack_174)
            pCVar8 = x_stk_84
            fret_02 = quest:GetHealth(pCVar8)
            fVar3 = 0.0
            x_stk_84 = nil
            x_stk_84 = 0
            if fVar3 < fret_02 then
                iVar19 = 0
                iVar15 = 1
                iVar9 = 0
                iVar7 = 0
                pcVar14 = "TEXT_QST_032_FISHERWIFE_ATTACKED"
                pCVar8 = quest:GetHero()
                r5 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                if iVar7 then
                    -- LAB_00d8cc31: (native jump target)
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then goto LAB_00d8cc48 end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar12 = xStack_9c
                    -- LAB_00d8d044: (native jump target)
                    quest:DeregisterTimer(xStack_178)
                    goto LAB_00d8d57a
                end
                -- LAB_00d8cc55: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_9c)
                    goto LAB_00d8d571
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_9c)
            quest:EntitySetAsKillable(me, true, true)
            r6 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
            quest:SetStateBool("WifeAttacked", true)
            if not (r6 ~= nil and not r6:IsNull()) then
                puVar11 = {x = 0, y = 0, z = 0}
            else
                puVar11 = r6:GetPos()
            end
            me:MoveToPosition(puVar11, 3.0, 1, false, true)
            iVar7 = me:IsPerformingScriptTask()
            cVar6 = iVar7
            while cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:DeregisterTimer(xStack_178)
                    goto LAB_00d8d57a
                end
                iVar7 = me:IsPerformingScriptTask()
                cVar6 = iVar7
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d8d568 end
            quest:SetStateBool("WifeAttackedAndLeft", true)
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
        cVar6 = __native_entity_state:GetStateBool("LeavingHappy")
    end
    ::FLOW_after_lab_00d8cc55::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        pCVar8 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
        iVar19 = 1
        iVar15 = 0
        iVar9 = 0
        iVar7 = 3.0
        p0_00 = pCVar8:GetPos()
        me:MoveToPosition(p0_00, iVar7, iVar9, (iVar15 ~= 0), (iVar19 ~= 0))
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while not bVar5 do
            iVar7 = quest:GetTimer(xStack_178)
            if iVar7 < 1 then
                fVar18 = 5.5
                pCVar8 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar18)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    iVar9 = quest:AddNewConversation(me, false, false)
                    pCVar8 = quest:GetHero()
                    quest:AddPersonToConversation(iVar9, pCVar8)
                    pCVar8 = quest:GetHero()
                    quest:AddLineToConversation(iVar9, "TEXT_QST_032_FISHERWIFE_SHOPPING_ASIDE", me, pCVar8, false)
                    quest:SetTimer(xStack_178, 0xf)
                end
            end
            bVar5 = me:IsTalkedToByHero()
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                me:ClearCommands()
                xStack_130 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                pCVar8 = resources:ScriptThing(xStack_174)
                pCVar8 = pCVar8
                fret_03 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_03 then
                    iVar19 = 0
                    iVar15 = 1
                    iVar9 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_032_FISHERWIFE_SHOPPING"
                    pCVar8 = quest:GetHero()
                    r7 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_130)
                            goto LAB_00d8d571
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_130)
                        break
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_130)
                r8 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
                if not (r8 ~= nil and not r8:IsNull()) then
                    puVar11 = {x = 0, y = 0, z = 0}
                else
                    puVar11 = r8:GetPos()
                end
                me:MoveToPosition(puVar11, 3.0, 0, false, true)
            end
            uVar4 = u_stk_164
            u_stk_164 = u_stk_164 | 8
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then
                goto LAB_00d8d21b
            else
                uVar13 = uVar4 | 0x18
                u_stk_164 = uVar13
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    uVar13 = uVar4 | 0x38
                    u_stk_164 = uVar13
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00d8d21b end
                end
                bVar5 = false
            end
            goto FLOW_past_lab_00d8d21b
            ::LAB_00d8d21b::
            bVar5 = true
            ::FLOW_past_lab_00d8d21b::
            if (uVar13 & 0x20) ~= 0 then
                uVar13 = uVar13 & 0xffffffdf
                u_stk_164 = uVar13
            end
            if (uVar13 & 0x10) ~= 0 then
                uVar13 = uVar13 & 0xffffffef
                u_stk_164 = uVar13
            end
            if (uVar13 & 8) ~= 0 then
                u_stk_164 = uVar13 & 0xfffffff7
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                me:ClearCommands()
                xStack_120 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_44 = resources:ScriptThing(xStack_174)
                pCVar8 = x_stk_44
                fret_04 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_04 then
                    iVar19 = 0
                    iVar15 = 1
                    iVar9 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_032_FISHERWIFE_ATTACKED_AFTER"
                    pCVar8 = quest:GetHero()
                    r9 = me:Speak(pCVar8, pcVar14, iVar7, (iVar9 ~= 0), (iVar15 ~= 0), (iVar19 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_120)
                            goto LAB_00d8d571
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_120)
                        break
                    end
                end
                quest:EntitySetAsKillable(me, true, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_120)
                resources:PrepareResource(xStack_174)
                bVar5 = resources:TryAcquire(xStack_174, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8d571 end
                    bVar5 = resources:TryAcquire(xStack_174, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                quest:SetStateBool("WifeAttacked", true)
                r10 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
                if not (r10 ~= nil and not r10:IsNull()) then
                    puVar11 = {x = 0, y = 0, z = 0}
                else
                    puVar11 = r10:GetPos()
                end
                me:MoveToPosition(puVar11, 3.0, 1, false, true)
                iVar7 = me:IsPerformingScriptTask()
                cVar6 = iVar7
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8d568 end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d8d568 end
                quest:SetStateBool("WifeAttackedAndLeft", true)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
            iVar7 = me:IsPerformingScriptTask()
            if not iVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        end
    end
    ::LAB_00d8d571::
    quest:DeregisterTimer(xStack_178)
    ::LAB_00d8d57a::
    ::LAB_00d8d583::
    resources:ReleaseResource(xStack_174)
    do return end
    ::LAB_00d8cc48::
    iVar7 = me:IsPerformingScriptTask()
    if not iVar7 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_9c)
            goto LAB_00d8d571
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_9c)
        quest:EntitySetAsKillable(me, true, true)
        r11 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
        quest:SetStateBool("WifeAttacked", true)
        if not (r11 ~= nil and not r11:IsNull()) then
            puVar11 = {x = 0, y = 0, z = 0}
        end
        me:MoveToPosition(puVar11, 3.0, 1, false, true)
        iVar7 = me:IsPerformingScriptTask()
        cVar6 = iVar7
        while cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                quest:DeregisterTimer(xStack_178)
                goto LAB_00d8d57a
            end
            iVar7 = me:IsPerformingScriptTask()
            cVar6 = iVar7
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00d8d568 end
        quest:SetStateBool("WifeAttackedAndLeft", true)
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
        cVar6 = __native_entity_state:GetStateBool("LeavingHappy")
        goto FLOW_after_lab_00d8cc55
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then goto LAB_00d8cc48 end
    quest:PauseAllNonScriptedEntities(false)
    pCVar12 = xStack_9c
    -- LAB_00d8d044_c10: (native jump target)
    quest:DeregisterTimer(xStack_178)
    goto LAB_00d8d57a
    ::LAB_00d8d568::
    goto LAB_00d8d571
end

function Init(quest, me)
    __native_entity_state:SetStateBool("TalkedTo", false)
    __native_entity_state:SetStateBool("LeavingHappy", false)
    local fVar1 = me:GetAngleXY()
    __native_entity_state:SetStateFloat("initial_angle", fVar1)
    quest:SetThingHasInformation(me, false, false, false)
    if (quest:GetStateBool("Helping")) and (not quest:GetStateBool("Helped")) then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

function OnPersist(quest, me, context)
    local talkedTo = quest:GetStateBool("TalkedTo") or false
    talkedTo = quest:PersistTransferBool(context, "TalkedTo", talkedTo)
    quest:SetStateBool("TalkedTo", talkedTo)
    local leavingHappy = quest:GetStateBool("LeavingHappy") or false
    leavingHappy = quest:PersistTransferBool(context, "LeavingHappy", leavingHappy)
    quest:SetStateBool("LeavingHappy", leavingHappy)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("SCRIPT_NAME_HERO")
    if cVar1 then
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xf64))
        quest:SetStateBool("WifeAttackedAndLeft", true)
    end
end

