-- Generated native draft: GuildMasterGameFlow. Review coverage report before use.
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
    local always_update, angle, bUnknown, bVar5, cVar6, c_stk_1b1, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar17, iVar18, iVar19, iVar20, pCVar10, pCVar11, pCVar7, pCVar9, pcVar14, piVar1, pppuVar16, pvVar12, r1, r2, r3, r4, r5, r6, r7, this_00, uVar13, uVar4, u_stk_1a0, xStack_198, xStack_1b0, xStack_94, xStack_a0, xStack_b8, xStack_c8, xStack_d8, xStack_ec, x_stk_18, x_stk_30, x_stk_48, x_stk_64, x_stk_78, x_stk_84, x_stk_c
    local alive = true
    u_stk_1a0 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_1b0 = resources:NewResource()
        resources:PrepareResource(xStack_1b0)
        bVar5 = resources:TryAcquire(xStack_1b0, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_1b0)
                return
            end
            bVar5 = resources:TryAcquire(xStack_1b0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            -- LAB_00e90cb3: (native jump target)
            resources:ReleaseResource(xStack_1b0)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            repeat
                bVar5 = quest:IsQuestActive("Q_EndGameFocalSites")
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_ec = resources:NewResource()
                    resources:PrepareResource(xStack_ec)
                    iVar18 = 4
                    pppuVar16 = xStack_ec
                    pCVar7 = quest:GetHero()
                    bVar5 = resources:TryAcquire(pppuVar16, pCVar7, iVar18)
                    while not bVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            resources:ReleaseResource(xStack_ec)
                            resources:ReleaseResource(xStack_1b0)
                            return
                        end
                        iVar18 = 4
                        pppuVar16 = xStack_ec
                        pCVar7 = quest:GetHero()
                        bVar5 = resources:TryAcquire(pppuVar16, pCVar7, iVar18)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e91df7: (native jump target)
                        resources:ReleaseResource(xStack_ec)
                        resources:ReleaseResource(xStack_1b0)
                        return
                    end
                    xStack_a0 = resources:NewActorMap()
                    resources:SetActor(xStack_a0, "HERO", xStack_ec)
                    resources:SetActor(xStack_a0, "GUILDDUDE", xStack_1b0)
                    xStack_94 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_FOCALSITEINTRO", xStack_a0, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_94)
                    resources:DestroyActorMap(xStack_a0)
                    resources:ReleaseResource(xStack_ec)
                    quest:SetTeleportingAsActive(false)
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES", "Q_EndGameFocalSites", false)
                    quest:SetQuestCardObjective("Q_EndGameFocalSites", "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01", "", "")
                    pCVar7 = quest:GetThingWithScriptName("TeleporterResidue")
                    always_update = false
                    bVar5 = false
                    angle = 0.0
                    pCVar9 = pCVar7:GetPos()
                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                    quest:CreateEffect("NEWTELEPORTER2", pCVar9, "", angle, bVar5, always_update)
                    pCVar7 = quest:GetThingWithScriptName("TeleporterResidue")
                    quest:MiniMapAddMarker(pCVar7, "HUD_ORB_QUEST_CORE")
                    bVar5 = quest:IsQuestActive("Q_EndGameFocalSites")
                    if bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                resources:ReleaseResource(xStack_1b0)
                                return
                            end
                            pCVar7 = quest:GetThingWithScriptName("TeleporterResidue")
                            iVar19 = 3.0
                            pCVar9 = pCVar7:GetPos()
                            pCVar7 = quest:GetHero()
                            iVar19 = IsDistanceFromThingToPositionUnder(pCVar7,pCVar9,iVar19)
                            if iVar19 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    resources:ReleaseResource(xStack_1b0)
                                    return
                                end
                                bUnknown = 0
                                pCVar7 = quest:GetThingWithScriptName("FocalSitesHSP")
                                pCVar10 = quest:GetHero()
                                quest:EntityTeleportToThing(pCVar10, pCVar7, (bUnknown ~= 0))
                            end
                            cVar6 = me:IsTalkedToByHero()
                            if cVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    resources:ReleaseResource(xStack_1b0)
                                    return
                                end
                                xStack_c8 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_30 = resources:ScriptThing(xStack_1b0)
                                pCVar7 = x_stk_30
                                fret_0 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_0 then
                                    iVar20 = 0
                                    iVar17 = 1
                                    iVar18 = 0
                                    iVar19 = 0
                                    -- TODO(native): pCVar11 = helper_E91F20(quest, me, *(this + 0x14))
                                    pCVar11 = nil --[[unresolved native value]]
                                    pvVar12 = pCVar11
                                    pCVar7 = quest:GetHero()
                                    r1 = me:Speak(pCVar7, pvVar12, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                    iVar19 = me:IsPerformingScriptTask()
                                    cVar6 = iVar19
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_c8)
                                            resources:ReleaseResource(xStack_1b0)
                                            return
                                        end
                                        iVar19 = me:IsPerformingScriptTask()
                                        cVar6 = iVar19
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_c8)
                                        resources:ReleaseResource(xStack_1b0)
                                        return
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_c8)
                            end
                            uVar4 = u_stk_1a0
                            u_stk_1a0 = u_stk_1a0 | 1
                            cVar6 = me:MsgIsHitByHero()
                            if not cVar6 then
                                uVar13 = uVar4 | 3
                                u_stk_1a0 = uVar13
                                -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                                bVar5 = nil --[[unresolved native value]]
                                if bVar5 then
                                    uVar13 = uVar4 | 7
                                    u_stk_1a0 = uVar13
                                    -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                                    bVar5 = nil --[[unresolved native value]]
                                    if not bVar5 then goto LAB_00e914b3 end
                                end
                                c_stk_1b1 = 0
                            else
                                goto LAB_00e914b3
                            end
                            goto FLOW_past_lab_00e914b3
                            ::LAB_00e914b3::
                            c_stk_1b1 = 1
                            ::FLOW_past_lab_00e914b3::
                            if (uVar13 & 4) ~= 0 then
                                uVar13 = uVar13 & 0xfffffffb
                                u_stk_1a0 = uVar13
                            end
                            if (uVar13 & 2) ~= 0 then
                                uVar13 = uVar13 & 0xfffffffd
                                u_stk_1a0 = uVar13
                            end
                            if (uVar13 & 1) ~= 0 then
                                u_stk_1a0 = uVar13 & 0xfffffffe
                            end
                            if c_stk_1b1 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    resources:ReleaseResource(xStack_1b0)
                                    return
                                end
                                xStack_d8 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_48 = resources:ScriptThing(xStack_1b0)
                                pCVar7 = x_stk_48
                                fret_00 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_00 then
                                    iVar20 = 0
                                    iVar17 = 1
                                    iVar18 = 0
                                    iVar19 = 0
                                    pcVar14 = "TEXT_QST_081_ATTACKED"
                                    pCVar7 = quest:GetHero()
                                    r2 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                    iVar19 = me:IsPerformingScriptTask()
                                    cVar6 = iVar19
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_d8)
                                            resources:ReleaseResource(xStack_1b0)
                                            return
                                        end
                                        iVar19 = me:IsPerformingScriptTask()
                                        cVar6 = iVar19
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        -- LAB_00e91e97: (native jump target)
                                        resources:DestroyMovie(xStack_d8)
                                        resources:ReleaseResource(xStack_1b0)
                                        return
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_d8)
                            end
                            bVar5 = quest:IsQuestActive("Q_EndGameFocalSites")
                        until not (bVar5)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e91ea7: (native jump target)
                        resources:ReleaseResource(xStack_1b0)
                        return
                    end
                end
                uVar4 = u_stk_1a0
                u_stk_1a0 = u_stk_1a0 | 8
                cVar6 = me:MsgIsHitByHero()
                if not cVar6 then
                    uVar13 = uVar4 | 0x18
                    u_stk_1a0 = uVar13
                    -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                    bVar5 = nil --[[unresolved native value]]
                    if bVar5 then
                        uVar13 = uVar4 | 0x38
                        u_stk_1a0 = uVar13
                        -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                        bVar5 = nil --[[unresolved native value]]
                        if not bVar5 then goto LAB_00e91742 end
                    end
                    c_stk_1b1 = 0
                else
                    goto LAB_00e91742
                end
                goto FLOW_past_lab_00e91742
                ::LAB_00e91742::
                c_stk_1b1 = 1
                ::FLOW_past_lab_00e91742::
                if (uVar13 & 0x20) ~= 0 then
                    uVar13 = uVar13 & 0xffffffdf
                    u_stk_1a0 = uVar13
                end
                if (uVar13 & 0x10) ~= 0 then
                    uVar13 = uVar13 & 0xffffffef
                    u_stk_1a0 = uVar13
                end
                if (uVar13 & 8) ~= 0 then
                    u_stk_1a0 = uVar13 & 0xfffffff7
                end
                if c_stk_1b1 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_b8 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_78 = resources:ScriptThing(xStack_1b0)
                        pCVar7 = x_stk_78
                        fret_01 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fret_01 <= fVar3 then
                            goto LAB_00e91898
                        end
                        goto FLOW_past_lab_00e91898
                        ::LAB_00e91898::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b8)
                        goto LAB_00e918af
                        ::FLOW_past_lab_00e91898::
                        iVar20 = 0
                        iVar17 = 1
                        iVar18 = 0
                        iVar19 = 0
                        pcVar14 = "TEXT_QST_081_ATTACKED"
                        pCVar7 = quest:GetHero()
                        r3 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                        iVar19 = me:IsPerformingScriptTask()
                        cVar6 = iVar19
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_b8
                                goto LAB_00e91ef9
                            end
                            iVar19 = me:IsPerformingScriptTask()
                            cVar6 = iVar19
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then goto LAB_00e91898 end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_b8
                        goto LAB_00e91ef9
                    end
                    goto FLOW_hoist_lab_00e91ef9_1
                end
                goto FLOW_past_lab_00e91ef9
                ::LAB_00e91ef9::
                resources:DestroyMovie(this_00)
                ::FLOW_hoist_lab_00e91ef9_1::
                break
                ::FLOW_past_lab_00e91ef9::
                ::LAB_00e918af::
                cVar6 = me:IsTalkedToByHero()
                if cVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_198 = resources:StartMovie("")
                        pCVar7 = 0x1
                        quest:PauseAllNonScriptedEntities(true)
                        if quest:GetMasterGameState("PostSavePosition") ~= 0x5aa then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                x_stk_84 = resources:ScriptThing(xStack_1b0)
                                pCVar10 = x_stk_84
                                fret_05 = quest:GetHealth(pCVar10)
                                fVar3 = 0.0
                                if fVar3 < fret_05 then
                                    iVar20 = 0
                                    iVar17 = 1
                                    iVar18 = 0
                                    iVar19 = 0
                                    -- TODO(native): pCVar11 = helper_E91F20(quest, me, *(this + 0x14))
                                    pCVar11 = nil --[[unresolved native value]]
                                    pvVar12 = pCVar11
                                    pCVar7 = quest:GetHero()
                                    r4 = me:Speak(pCVar7, pvVar12, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                    iVar19 = me:IsPerformingScriptTask()
                                    cVar6 = iVar19
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e91ccb end
                                        iVar19 = me:IsPerformingScriptTask()
                                        cVar6 = iVar19
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e91ee8 end
                                end
                                goto LAB_00e91db4
                            end
                            goto LAB_00e91ee8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            goto LAB_00e91ccb
                        else
                            x_stk_c = resources:ScriptThing(xStack_1b0)
                            pCVar10 = x_stk_c
                            fret_02 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_02 then
                                iVar20 = 0
                                iVar17 = 1
                                iVar18 = 0
                                iVar19 = 0
                                pcVar14 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                                pCVar10 = quest:GetHero()
                                r5 = me:Speak(pCVar10, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                iVar19 = me:IsPerformingScriptTask()
                                cVar6 = iVar19
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e91ee8 end
                                    iVar19 = me:IsPerformingScriptTask()
                                    cVar6 = iVar19
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e91ccb end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar19 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar19 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e91ee8 end
                                iVar19 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e91ccb end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if iVar19 ~= 1 then
                                if not bVar5 then
                                    x_stk_64 = resources:ScriptThing(xStack_1b0)
                                    pCVar10 = x_stk_64
                                    fret_04 = quest:GetHealth(pCVar10)
                                    fVar3 = 0.0
                                    if fVar3 < fret_04 then
                                        iVar20 = 0
                                        iVar17 = 1
                                        iVar18 = 0
                                        iVar19 = 0
                                        pcVar14 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO"
                                        pCVar7 = quest:GetHero()
                                        r6 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                        iVar19 = me:IsPerformingScriptTask()
                                        cVar6 = iVar19
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e91ee8 end
                                            iVar19 = me:IsPerformingScriptTask()
                                            cVar6 = iVar19
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e91ccb end
                                    end
                                    goto LAB_00e91db4
                                end
                                goto FLOW_hoist_lab_00e91db4_1
                            end
                            goto FLOW_hoist_lab_00e91db4_2
                        end
                        goto FLOW_past_lab_00e91db4
                        ::LAB_00e91db4::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_198)
                        goto LAB_00e91dcc
                        ::FLOW_hoist_lab_00e91db4_1::
                        goto LAB_00e91ccb
                        ::FLOW_hoist_lab_00e91db4_2::
                        if not bVar5 then
                            x_stk_18 = resources:ScriptThing(xStack_1b0)
                            pCVar10 = x_stk_18
                            fret_03 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_03 then
                                iVar20 = 0
                                iVar17 = 1
                                iVar18 = 0
                                iVar19 = 0
                                pcVar14 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES"
                                pCVar7 = quest:GetHero()
                                r7 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
                                iVar19 = me:IsPerformingScriptTask()
                                cVar6 = iVar19
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e91ccb end
                                    iVar19 = me:IsPerformingScriptTask()
                                    cVar6 = iVar19
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e91ee8 end
                            end
                            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false)
                            quest:TakeObjectFromHero("OBJECT_MAZE_JOURNAL")
                            goto LAB_00e91db4
                        end
                        goto LAB_00e91ee8
                        ::FLOW_past_lab_00e91db4::
                        goto FLOW_past_lab_00e91ee8
                        ::LAB_00e91ee8::
                        quest:PauseAllNonScriptedEntities(false)
                        ::FLOW_past_lab_00e91ee8::
                        goto FLOW_past_lab_00e91ccb
                        ::LAB_00e91ccb::
                        quest:PauseAllNonScriptedEntities(false)
                        ::FLOW_past_lab_00e91ccb::
                        this_00 = xStack_198
                        goto LAB_00e91ef9
                    end
                    break
                end
                ::LAB_00e91dcc::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_1b0)
                    return
                end
            until false
        end
        resources:ReleaseResource(xStack_1b0)
    end
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

function helper_E91F20(quest, me)
    local CVar2, bVar3, bVar6, iVar1, iVar4, native_arg_sequence_1, native_arg_switch_3, native_arg_switch_4, native_arg_switch_5, native_arg_switch_6, native_arg_switch_7, pcVar7, uVar5
    local alive = true
    -- TODO(native): CCharString::CCharString(&xStack_c,other);
    local function __region_LAB_00e92061()
        do return "" end
    end
    iVar4 = __native_entity_state:GetStateInt("self_0x44")
    -- TODO(native): iVar1 = *(iVar4 + 4)
    iVar1 = nil --[[unresolved native value]]
    if iVar1 < 0x385 then
        if iVar1 == 900 then
            pcVar7 = "TEXT_QST_081_ARENA"
        else
            if iVar1 < 0x2bd then
                if iVar1 == 700 then
                    goto LAB_00e92080
                else
                    if iVar1 < 0x191 then
                        if iVar1 == 400 then
                            pcVar7 = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
                        else
                            native_arg_switch_3 = iVar1
                            repeat
                                if native_arg_switch_3 == 100 then
                                    pcVar7 = "TEXT_QST_081_TRAINING"
                                    break
                                else
                                    if native_arg_switch_3 == 0x96 then
                                        pcVar7 = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
                                        break
                                    else
                                        if native_arg_switch_3 == 200 then
                                            pcVar7 = "TEXT_QST_081_WASP_BOSS"
                                            break
                                        else
                                            if native_arg_switch_3 == 300 then
                                                pcVar7 = "TEXT_QST_081_MAZE_MEETING"
                                            else
                                                -- FLOW_native_label_2_c1: (native jump target)
                                                -- LAB_00e922bb_c1: (native jump target)
                                                -- LAB_00e922c9_c1: (native jump target)
                                                do return "" end
                                                goto FLOW_after_flow_native_label_2
                                            end
                                        end
                                    end
                                end
                            until not (false)
                        end
                    else
                        native_arg_switch_4 = iVar1
                        repeat
                            if native_arg_switch_4 == 0x1c2 then
                                pcVar7 = "TEXT_QST_081_ORCHARD_FARM"
                                break
                            else
                                if native_arg_switch_4 == 500 then
                                    pcVar7 = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
                                    break
                                else
                                    if native_arg_switch_4 == 0x226 then
                                        pcVar7 = "TEXT_QST_081_TRADER_ESCORT"
                                        break
                                    else
                                        if native_arg_switch_4 == 600 then
                                            bVar6 = quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar6 then
                                                if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                                goto LAB_00e92080
                                            end
                                            if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                            pcVar7 = "TEXT_QST_081_SECOND_MAZE_MEETING"
                                        else
                                            -- FLOW_native_label_2_c2: (native jump target)
                                            -- LAB_00e922bb_c2: (native jump target)
                                            -- LAB_00e922c9_c2: (native jump target)
                                            do return "" end
                                            goto FLOW_after_flow_native_label_2
                                        end
                                    end
                                end
                            end
                        until not (false)
                    end
                end
                goto FLOW_past_lab_00e92080
                ::LAB_00e92080::
                pcVar7 = "TEXT_QST_081_BANDIT_CAMP"
                ::FLOW_past_lab_00e92080::
            else
                native_arg_switch_5 = iVar1
                repeat
                    if native_arg_switch_5 == 0x2dd then
                        pcVar7 = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
                        break
                    else
                        if native_arg_switch_5 == 0x2fe then
                            pcVar7 = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
                            break
                        else
                            if native_arg_switch_5 == 800 then
                                bVar6 = quest:IsQuestActive("V_TrophyDealer")
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar6 then
                                    if bVar3 then
                                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                        -- LAB_00e922c9_c3: (native jump target)
                                        do return "" end
                                        goto FLOW_after_flow_native_label_2
                                    end
                                    goto FLOW_native_label_1
                                end
                                if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                pcVar7 = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                                break
                            else
                                if native_arg_switch_5 == 0x352 then
                                    goto FLOW_native_label_1
                                else
                                    if native_arg_switch_5 == 0x358 then
                                        pcVar7 = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
                                        break
                                    else
                                        if native_arg_switch_5 == 0x361 then
                                            pcVar7 = "TEXT_QST_081_WHITE_BALVERINE_KHG"
                                            break
                                        else
                                            if native_arg_switch_5 == 0x366 then
                                                pcVar7 = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
                                                break
                                            else
                                                if native_arg_switch_5 == 0x36b then
                                                    pcVar7 = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
                                                else
                                                    -- FLOW_native_label_2_c4: (native jump target)
                                                    -- LAB_00e922bb_c4: (native jump target)
                                                    -- LAB_00e922c9_c4: (native jump target)
                                                    do return "" end
                                                    goto FLOW_after_flow_native_label_2
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                            goto FLOW_past_flow_native_label_1
                            ::FLOW_native_label_1::
                            pcVar7 = "TEXT_QST_081_TROPHY_DEALER"
                            break
                            ::FLOW_past_flow_native_label_1::
                        end
                    end
                until not (false)
            end
        end
    else
        if iVar1 < 0x5dd then
            if iVar1 == 0x5dc then
                pcVar7 = "TEXT_QST_081_WIZARD_BATTLE"
            else
                if iVar1 < 0x4c1 then
                    if iVar1 == 0x4c0 then
                        pcVar7 = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
                    else
                        native_arg_switch_6 = iVar1
                        repeat
                            if native_arg_switch_6 == 1000 then
                                pcVar7 = "TEXT_QST_081_MEET_SISTER"
                                break
                            else
                                if native_arg_switch_6 == 0x41a then
                                    pcVar7 = "TEXT_QST_081_WAITING_FOR_MCC"
                                    break
                                else
                                    if native_arg_switch_6 == 0x44c then
                                        pcVar7 = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
                                        break
                                    else
                                        if native_arg_switch_6 == 0x4b0 then
                                            pcVar7 = "TEXT_QST_081_GRAVEYARD"
                                        else
                                            -- LAB_00e922bb_c5: (native jump target)
                                            -- LAB_00e922c9_c5: (native jump target)
                                            do return "" end
                                            goto FLOW_after_flow_native_label_2_243
                                        end
                                    end
                                end
                            end
                        until not (false)
                    end
                else
                    native_arg_switch_7 = iVar1
                    repeat
                        if native_arg_switch_7 == 0x4d1 then
                            pcVar7 = "TEXT_QST_081_FINALGRAVEYARD"
                            break
                        else
                            if native_arg_switch_7 == 0x4e2 then
                                pcVar7 = "TEXT_QST_081_PRISON"
                                break
                            else
                                if native_arg_switch_7 == 0x514 then
                                    -- TODO(native): if *(iVar4 + 0x60) == 0 then
                                    if false then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                        pcVar7 = "TEXT_QST_081_BEFORE_HOOK_COAST"
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                        pcVar7 = "TEXT_QST_081_HOOK_COAST"
                                    end
                                    break
                                else
                                    if native_arg_switch_7 == 0x5aa then
                                        pcVar7 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                                    else
                                        -- FLOW_native_label_2: (native jump target)
                                        -- LAB_00e922bb: (native jump target)
                                        -- LAB_00e922c9: (native jump target)
                                        return ""
                                    end
                                end
                            end
                        end
                    until not (false)
                end
                ::FLOW_after_flow_native_label_2_243::
            end
        else
            if iVar1 < 0x8fd then
                if iVar1 == 0x8fc then goto LAB_00e923f7 end
                if iVar1 < 0x6a5 then
                    if iVar1 == 0x6a4 then
                        pcVar7 = "TEXT_QST_081_JACK_BOSS_FIGHT"
                    else
                        if iVar1 == 0x60e then
                            pcVar7 = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                        else
                            if iVar1 ~= 0x640 then
                                -- LAB_00e922bb_c6: (native jump target)
                                -- LAB_00e922c9_c6: (native jump target)
                                do return "" end
                                goto FLOW_after_flow_native_label_2_358
                            end
                            pcVar7 = "TEXT_QST_081_FOCAL_SITES"
                        end
                    end
                else
                    if iVar1 ~= 0x834 then
                        -- LAB_00e922bb_c7: (native jump target)
                        -- LAB_00e922c9_c7: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2_358
                    end
                    pcVar7 = "TEXT_QST_081_SHIP_SUMMONING"
                end
            else
                if iVar1 == 0x960 then
                    goto LAB_00e923f7
                else
                    if iVar1 == 0x9c4 then
                        -- TODO(native): iVar4 = *(iVar4 + 0x70)
                        iVar4 = nil --[[unresolved native value]]
                        if iVar4 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                __region_LAB_00e92061()
                                goto FLOW_after_flow_native_label_2_358
                            end
                            pcVar7 = "TEXT_QST_081_HERO_SOULS"
                        else
                            if iVar4 == 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00e92207: (native jump target)
                                    -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                    do return "" end
                                    goto FLOW_after_flow_native_label_2_358
                                end
                                pcVar7 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                            else
                                if iVar4 == 2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- LAB_00e92048: (native jump target)
                                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                        do return "" end
                                        goto FLOW_after_flow_native_label_2_358
                                    end
                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                                else
                                    if iVar4 == 3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                        pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                                    else
                                        if iVar4 == 4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                            pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                                        else
                                            if iVar4 == 5 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                                pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                                            else
                                                if iVar4 == 6 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                                                else
                                                    if iVar4 ~= 7 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then
                                                            -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                                            do return "" end
                                                            goto FLOW_after_flow_native_label_2_358
                                                        end
                                                        -- LAB_00e922c9_c12: (native jump target)
                                                        do return "" end
                                                        goto FLOW_after_flow_native_label_2_358
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    else
                        if iVar1 ~= 0xa28 then
                            -- LAB_00e922bb_c13: (native jump target)
                            -- LAB_00e922c9_c13: (native jump target)
                            do return "" end
                            goto FLOW_after_flow_native_label_2_358
                        end
                        pcVar7 = "TEXT_QST_081_DRAGON_FIGHT_10"
                    end
                end
            end
            goto FLOW_past_lab_00e923f7
            ::LAB_00e923f7::
            pcVar7 = "TEXT_QST_081_THE_ORACLE"
            ::FLOW_past_lab_00e923f7::
        end
        ::FLOW_after_flow_native_label_2_358::
    end
    ::FLOW_after_flow_native_label_2::
    CVar2 = self_0x50
    native_arg_sequence_1 = false
    if CVar2 == in_stack_fffffff0 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if not native_arg_sequence_1 then
        if CVar2 ~= nil then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            if in_stack_fffffff0 ~= nil then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            -- TODO(native): if *(CVar2 + 4) == *(in_stack_fffffff0 + 4) then
            if false then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            -- TODO(native): iVar4 = CBasicString<char>::Compare(*(void **)CVar2,*(void **)in_stack_fffffff0);
            if iVar4 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
    end
    if native_arg_sequence_1 then
        uVar5 = math.random(0, 32767)
        uVar5 = uVar5 & 0x80000003
        bVar6 = uVar5 == 0
        if uVar5 < 0 then
            bVar6 = (uVar5 - 1 | 0xfffffffc) == 0xffffffff
        end
        if bVar6 then
            if not __native_entity_state:GetStateBool("self_0x4a") then
                -- TODO(native): name field 0x4a (undefined1)
                __native_entity_state:SetStateBool("self_0x4a", true)
                pcVar7 = "TEXT_QST_081_INFO_TELEPORTERS"
            else
                if not __native_entity_state:GetStateBool("self_0x4b") then
                    -- TODO(native): name field 0x4b (undefined1)
                    __native_entity_state:SetStateBool("self_0x4b", true)
                    pcVar7 = "TEXT_QST_081_INFO_WEAPONS"
                else
                    if not __native_entity_state:GetStateBool("self_0x4c") then
                        -- TODO(native): name field 0x4c (undefined1)
                        __native_entity_state:SetStateBool("self_0x4c", true)
                        pcVar7 = "TEXT_QST_081_INFO_LOG_BOOK"
                    else
                        iVar4 = quest:GetHeroTitle()
                        if (iVar4 ~= 0x13) or (__native_entity_state:GetStateBool("self_0x49")) then goto LAB_00e924b3 end
                        -- TODO(native): name field 0x49 (undefined1)
                        __native_entity_state:SetStateBool("self_0x49", true)
                        pcVar7 = "TEXT_QST_081_INFO_HERO_TITLE"
                    end
                end
            end
        end
    end
    ::LAB_00e924b3::
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x50),&xStack_10);
    return pcVar7
end

