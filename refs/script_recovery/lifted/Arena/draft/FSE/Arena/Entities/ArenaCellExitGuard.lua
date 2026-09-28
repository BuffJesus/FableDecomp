-- Generated native draft: ArenaCellExitGuard. Review coverage report before use.
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
    local bVar19, bVar4, bVar5, bVar6, bVar8, bVar9, cVar7, dist, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, iVar10, p0, p2, p3, p4, pCVar11, pCVar12, pCVar13, pCVar15, pCVar16, pCVar17, pCVar18, pcVar14, ppuVar20, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, this_00, xStack_114, xStack_12c, xStack_ac
    local alive = true
    local function __cleanup_LAB_00f1ca06()
        this_00 = xStack_ac
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(ppuVar20)
    end
    local function __cleanup_LAB_00f1ca16()
        resources:ReleaseResource(ppuVar20)
    end
    bVar8 = false
    bVar4 = false
    ppuVar20 = resources:NewResource()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resources:PrepareResource(xStack_12c)
    bVar5 = resources:TryAcquire(xStack_12c, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        bVar5 = resources:TryAcquire(xStack_12c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        resources:ReleaseResource(ppuVar20)
        return
    end
    bVar5 = false
    bVar9 = false
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    repeat
        if bVar6 then
            return
        end
        cVar7 = me:IsTalkedToByHero()
        if cVar7 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then __cleanup_LAB_00f1ca16(); return end
            iVar10 = me:IsPerformingScriptTask()
            if iVar10 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then __cleanup_LAB_00f1ca16(); return end
            end
            xStack_114 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetStateInt("ArenaState") == 3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00f1c9b3: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_114
                else
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_114
                            goto FLOW_after_lab_00f1c9b3
                        end
                        pCVar11 = resources:ScriptThing(xStack_12c)
                        fret_01 = quest:GetHealth(pCVar11)
                        fVar3 = 0.0
                        if fVar3 < fret_01 then
                            bVar6 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND"
                            pCVar11 = quest:GetHero()
                            r1 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00f1c9d8_c2: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_114
                                    goto FLOW_after_lab_00f1c9d8
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto FLOW_after_lab_00f1c9b3
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00f1c9d8_c4: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_114
                            goto FLOW_after_lab_00f1c9d8
                        end
                        pCVar11 = resources:ScriptThing(xStack_12c)
                        fret_0 = quest:GetHealth(pCVar11)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            bVar5 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST"
                            pCVar11 = quest:GetHero()
                            r2 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar5)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_114
                                    goto FLOW_after_lab_00f1c9b3
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00f1c9d8_c6: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        pCVar11 = resources:ScriptThing(xStack_12c)
                        fret_00 = quest:GetHealth(pCVar11)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            bVar5 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST_EXTRA"
                            pCVar11 = quest:GetHero()
                            r3 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar5)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_114
                                    goto FLOW_after_lab_00f1c9b3
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00f1c9d8_c8: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        bVar5 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00f1bfb3 end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if iVar10 == 1 then
                            if not bVar6 then
                                pCVar11 = resources:ScriptThing(xStack_12c)
                                fret_02 = quest:GetHealth(pCVar11)
                                fVar3 = 0.0
                                if fVar3 < fret_02 then
                                    bVar6 = false
                                    pCVar17 = 0x1
                                    pCVar16 = 0x0
                                    pCVar15 = 0x0
                                    pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING"
                                    pCVar11 = quest:GetHero()
                                    r4 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00f1c9c5 end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar7 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00f1bfb3 end
                                end
                                goto LAB_00f1c512
                            end
                            goto LAB_00f1bfb3
                        end
                        if not bVar6 then
                            pCVar11 = resources:ScriptThing(xStack_12c)
                            fret_03 = quest:GetHealth(pCVar11)
                            fVar3 = 0.0
                            if fVar3 < fret_03 then
                                bVar6 = false
                                pCVar17 = 0x1
                                pCVar16 = 0x0
                                pCVar15 = 0x0
                                pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING"
                                pCVar11 = quest:GetHero()
                                r5 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00f1bfb3 end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00f1c9c5 end
                            end
                            -- TODO(native): CCharString__AssignFromWide("");
                            bVar19 = true
                            bVar6 = true
                            pCVar12 = quest:GetActiveQuestName()
                            quest:SetQuestAsFailed(pCVar12, bVar6, "", pCVar18)
                            quest:SetStateBool("MissionFailed", true)
                            goto LAB_00f1c512
                        end
                    end
                    ::LAB_00f1c9c5::
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_114
                end
                ::FLOW_after_lab_00f1c9b3::
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00f1c9d8: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_114
                else
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_114
                            goto FLOW_after_lab_00f1c9d8_375
                        end
                        pCVar11 = resources:ScriptThing(xStack_12c)
                        fret_05 = quest:GetHealth(pCVar11)
                        fVar3 = 0.0
                        if fVar3 < fret_05 then
                            bVar6 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND"
                            pCVar11 = quest:GetHero()
                            r6 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00f1c9b3_c10: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_114
                                    goto FLOW_after_lab_00f1c9d8
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto FLOW_after_lab_00f1c9d8_375
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00f1c9b3_c12: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_114
                            goto FLOW_after_lab_00f1c9d8
                        end
                        pCVar11 = resources:ScriptThing(xStack_12c)
                        fret_04 = quest:GetHealth(pCVar11)
                        fVar3 = 0.0
                        if fVar3 < fret_04 then
                            bVar5 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST"
                            pCVar11 = quest:GetHero()
                            r7 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar5)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_114
                                    goto FLOW_after_lab_00f1c9d8_375
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00f1c9b3_c14: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        bVar5 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00f1bfb3 end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if iVar10 == 1 then
                            if bVar6 then
                                -- LAB_00f1c3f8: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_114
                                goto LAB_00f1ca0d
                            end
                            pCVar11 = resources:ScriptThing(xStack_12c)
                            fret_06 = quest:GetHealth(pCVar11)
                            fVar3 = 0.0
                            if fVar3 < fret_06 then
                                bVar6 = false
                                pCVar17 = 0x1
                                pCVar16 = 0x0
                                pCVar15 = 0x0
                                pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING"
                                pCVar11 = quest:GetHero()
                                r8 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00f1bfb3 end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00f1bfb3 end
                            end
                        else
                            if bVar6 then goto LAB_00f1bfb3 end
                            pCVar11 = resources:ScriptThing(xStack_12c)
                            fret_07 = quest:GetHealth(pCVar11)
                            fVar3 = 0.0
                            if fVar3 < fret_07 then
                                bVar6 = false
                                pCVar17 = 0x1
                                pCVar16 = 0x0
                                pCVar15 = 0x0
                                pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING"
                                pCVar11 = quest:GetHero()
                                r9 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00f1bfb3 end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00f1bfb3 end
                            end
                            quest:SetStateBool("MissionFailed", true)
                            -- TODO(native): CCharString__AssignFromWide("");
                            bVar19 = true
                            bVar6 = true
                            pCVar12 = quest:GetActiveQuestName()
                            quest:SetQuestAsFailed(pCVar12, bVar6, nil --[[missing]], pCVar18)
                        end
                        goto LAB_00f1c512
                    end
                    goto FLOW_hoist_lab_00f1c512_1
                end
                ::FLOW_after_lab_00f1c9d8_375::
            end
            ::FLOW_after_lab_00f1c9d8::
            goto FLOW_past_lab_00f1c512
            ::LAB_00f1c512::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_114)
            goto LAB_00f1c953
            ::FLOW_hoist_lab_00f1c512_1::
            ::LAB_00f1bfb3::
            quest:PauseAllNonScriptedEntities(false)
            this_00 = xStack_114
            ::FLOW_past_lab_00f1c512::
            ::LAB_00f1ca0d::
            resources:DestroyMovie(this_00)
            __cleanup_LAB_00f1ca16(); return
        end
        bVar6 = me:MsgIsHitByHero()
        if bVar6 then
            goto LAB_00f1c5ba
        else
            bVar8 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar8 then
                bVar8 = true
                bVar4 = true
                bVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar6 then goto LAB_00f1c5ba end
            end
            bVar8 = true
            bVar6 = false
        end
        goto FLOW_past_lab_00f1c5ba
        ::LAB_00f1c5ba::
        bVar6 = true
        ::FLOW_past_lab_00f1c5ba::
        if bVar4 then
            bVar4 = false
        end
        if bVar8 then
            bVar8 = false
        end
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then __cleanup_LAB_00f1ca16(); return end
            iVar10 = me:IsPerformingScriptTask()
            if iVar10 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then __cleanup_LAB_00f1ca16(); return end
            end
            if not quest:GetStateBool("InHitCutsceneAlready") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then __cleanup_LAB_00f1ca16(); return end
                quest:SetStateBool("InHitCutsceneAlready", true)
                xStack_ac = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                pCVar11 = resources:ScriptThing(xStack_12c)
                fret_08 = quest:GetHealth(pCVar11)
                fVar3 = 0.0
                if fVar3 < fret_08 then
                    bVar6 = false
                    pCVar17 = 0x1
                    pCVar16 = 0x0
                    pCVar15 = 0x0
                    pcVar14 = "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_ATTACKED"
                    pCVar11 = quest:GetHero()
                    r10 = me:Speak(pCVar11, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                    iVar10 = me:IsPerformingScriptTask()
                    cVar7 = iVar10
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            __cleanup_LAB_00f1ca06(); return
                        end
                        iVar10 = me:IsPerformingScriptTask()
                        cVar7 = iVar10
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        __cleanup_LAB_00f1ca06()
                        return
                    end
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                pCVar13 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, pCVar13)
                pCVar13 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(pCVar13, me)
                quest:SetStateBool("InHitCutsceneAlready", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_ac)
            end
        else
            dist = 1.0
            pCVar13 = quest:GetThingWithScriptName("CellExitMarker")
            bVar6 = quest:IsDistanceBetweenThingsOver(me, pCVar13, dist)
            if bVar6 then
                iVar10 = me:IsPerformingScriptTask()
                bVar6 = true
                if iVar10 then goto LAB_00f1c818 end
            else
                goto LAB_00f1c818
            end
            goto FLOW_past_lab_00f1c818
            ::LAB_00f1c818::
            bVar6 = false
            ::FLOW_past_lab_00f1c818::
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then __cleanup_LAB_00f1ca16(); return end
                pCVar11 = quest:GetThingWithScriptName("CellExitMarker")
                p4 = 1
                p3 = 0
                p2 = 0
                iVar10 = 0
                p0 = pCVar11:GetPos()
                me:MoveToPosition(p0, iVar10, p2, (p3 ~= 0), (p4 ~= 0))
                bVar9 = false
            elseif not bVar9 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    __cleanup_LAB_00f1ca16()
                    return
                end
                iVar10 = me:IsPerformingScriptTask()
                if not iVar10 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
                        resources:ReleaseResource(ppuVar20)
                        return
                    end
                    bVar9 = true
                    pCVar13 = quest:GetThingWithScriptName("CellExitMarker")
                    bVar6 = true
                    fret_09 = pCVar13:GetAngleXY()
                    quest:EntitySetFacingAngle(me, fret_09, bVar6)
                end
            end
        end
        ::LAB_00f1c953::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

