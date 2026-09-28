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
    local always_update, angle, bUnknown, bVar5, cVar6, c_stk_1b1, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar17, iVar18, iVar19, iVar20, pCVar10, pCVar11, pCVar7, pCVar9, pcVar14, piVar1, pppuVar16, pvVar12, r1, r2, r3, r4, r5, r6, r7, r8, this_00, uVar13, uVar4, u_stk_1a0, xStack_198, xStack_1b0, xStack_94, xStack_a0, xStack_b8, xStack_c8, xStack_d8, xStack_ec, x_stk_18, x_stk_30, x_stk_48, x_stk_64, x_stk_78, x_stk_84, x_stk_c
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
                    r1 = quest:CreateEffectAtPos("NEWTELEPORTER2", pCVar9, angle, bVar5)
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
                            iVar19 = (pCVar7 ~= nil and pCVar7:IsDistanceFromPositionUnder(pCVar9, iVar19))
                            if iVar19 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    resources:ReleaseResource(xStack_1b0)
                                    return
                                end
                                bUnknown = false
                                pCVar7 = quest:GetThingWithScriptName("FocalSitesHSP")
                                pCVar10 = quest:GetHero()
                                quest:EntityTeleportToThing(pCVar10, pCVar7, bUnknown)
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
                                    pCVar11 = require("V_GuildMaster.native_quest_helpers").helper_E91F20(quest, me)
                                    pvVar12 = pCVar11
                                    pCVar7 = quest:GetHero()
                                    r2 = me:Speak(pCVar7, pvVar12, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                                    r3 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                        r4 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                                    pCVar11 = require("V_GuildMaster.native_quest_helpers").helper_E91F20(quest, me)
                                    pvVar12 = pCVar11
                                    pCVar7 = quest:GetHero()
                                    r5 = me:Speak(pCVar7, pvVar12, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                                r6 = me:Speak(pCVar10, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                                        r7 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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
                                r8 = me:Speak(pCVar7, pcVar14, iVar19, (iVar18 ~= 0), (iVar17 ~= 0), (iVar20 ~= 0))
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

