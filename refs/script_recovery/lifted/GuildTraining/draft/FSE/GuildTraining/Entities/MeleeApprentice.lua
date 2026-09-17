-- Generated native draft: MeleeApprentice. Review coverage report before use.
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
    local __native_condition_1, bVar3, bVar4, cVar5, fVar2, iVar11, iVar12, iVar13, iVar14, p0, pCVar6, pCVar7, pCVar9, pThing, pcVar10, puVar8, r1, r10, r11, r2, r3, r4, r5, r6, r7, r8, r9, xStack_18, xStack_24, xStack_3c, xStack_48, xStack_64, xStack_74, xStack_84, xStack_94, xStack_a4, xStack_c, xStack_f8
    local alive = true
    local function __cleanup_LAB_00d419fe()
        pCVar9 = xStack_a4
        resources:DestroyMovie(pCVar9)
        resources:ReleaseResource(0)
    end
    local function __cleanup_LAB_00d41a02()
        resources:DestroyMovie(pCVar9)
        resources:ReleaseResource(0)
    end
    xStack_f8 = resources:NewResource()
    bVar3 = false
    if bVar3 ~= 0 then
    end
    bVar3 = resources:TryAcquire(xStack_f8, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            if false then
                -- TODO(native): (**(code **)((int)xStack_e8 + 4))();
            end
            return
        end
        bVar3 = resources:TryAcquire(xStack_f8, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        resources:ReleaseResource(xStack_f8)
        return
    end
    quest:EntitySheatheWeapons(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAllowBossPhaseChanges(me, false)
    p0 = 0x1
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    r1 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            r1 = nil
            return
        end
        if quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
            bVar3 = false
            if bVar3 ~= 0 then
            end
            cVar5 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            while cVar5 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                cVar5 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(0, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                bVar3 = resources:TryAcquire(0, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
        end
        if quest:GetStateBool("StartedMeleeTesting") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
            bVar3 = false
            if bVar3 ~= 0 then
            end
            cVar5 = quest:GetStateBool("StartedMeleeTesting")
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                cVar5 = quest:GetStateBool("StartedMeleeTesting")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(0, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                bVar3 = resources:TryAcquire(0, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(0)
                return
            end
            me:ClearCommands()
            quest:EntitySheatheWeapons(me, false)
        end
        bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar3 then
            if bVar4 then
                resources:ReleaseResource(0)
                return
            end
            if not __native_entity_state:GetStateBool("WillWoodsChatDone") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK")
                iVar14 = 1
                iVar13 = 0
                iVar12 = 1
                iVar11 = 0x40400000
                pCVar7 = pCVar6:GetPos()
                me:MoveToPosition(pCVar7, iVar11, iVar12, (iVar13 ~= 0), (iVar14 ~= 0))
                __native_entity_state:SetStateBool("WillWoodsChatDone", true)
            else
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        resources:ReleaseResource(0)
                        return
                    end
                    me:ClearCommands()
                    xStack_74 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_18 = resources:ScriptThing(0)
                    pCVar6 = xStack_18
                    r2 = quest:GetHealth(pCVar6)
                    fVar2 = 0.0
                    if fVar2 < fret_0 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar12 = 0
                        iVar11 = 0
                        pcVar10 = "TEXT_QST_028_WHISPER_SCORPION_WOODS"
                        pCVar6 = quest:GetHero()
                        r3 = me:Speak(pCVar6, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar11 = me:IsPerformingScriptTask()
                        cVar5 = iVar11
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar9 = xStack_74
                                -- LAB_00d41a02_c14: (native jump target)
                                resources:DestroyMovie(pCVar9)
                                -- TODO(native): goto LAB_00d41a07_c14
                            end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar9 = xStack_74
                            -- LAB_00d41a02_c15: (native jump target)
                            resources:DestroyMovie(pCVar9)
                            -- TODO(native): goto LAB_00d41a07_c15
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_74)
                    pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK")
                    iVar14 = 1
                    iVar13 = 0
                    iVar12 = 1
                    iVar11 = 0x40400000
                    pCVar7 = pCVar6:GetPos()
                    me:MoveToPosition(pCVar7, iVar11, iVar12, (iVar13 ~= 0), (iVar14 ~= 0))
                end
            end
        else
            if bVar4 then
                resources:ReleaseResource(0)
                return
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(0)
                    return
                end
                me:ClearCommands()
                bVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        resources:ReleaseResource(0)
                        return
                    end
                    xStack_94 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_48 = resources:ScriptThing(0)
                    pCVar6 = xStack_48
                    r4 = quest:GetHealth(pCVar6)
                    fVar2 = 0.0
                    if fVar2 < fret_00 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar12 = 0
                        iVar11 = 0
                        pcVar10 = "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN"
                        pCVar6 = quest:GetHero()
                        r5 = me:Speak(pCVar6, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar11 = me:IsPerformingScriptTask()
                        cVar5 = iVar11
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar9 = xStack_94
                                __cleanup_LAB_00d41a02(); return
                            end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar9 = xStack_94
                            __cleanup_LAB_00d41a02(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar9 = xStack_94
                    -- LAB_00d41813: (native jump target)
                    resources:DestroyMovie(pCVar9)
                else
                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWill")
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            resources:ReleaseResource(0)
                            return
                        end
                        xStack_84 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_3c = resources:ScriptThing(0)
                        pCVar6 = xStack_3c
                        r6 = quest:GetHealth(pCVar6)
                        fVar2 = 0.0
                        if fVar2 < fret_01 then
                            iVar14 = 0
                            iVar13 = 1
                            iVar12 = 0
                            iVar11 = 0
                            pcVar10 = "TEXT_QST_028_TEEN_WHISPER_WILL_MOAN"
                            pCVar6 = quest:GetHero()
                            r7 = me:Speak(pCVar6, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar9 = xStack_84
                                    __cleanup_LAB_00d41a02(); return
                                end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar5 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar9 = xStack_84
                                __cleanup_LAB_00d41a02(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        pCVar9 = xStack_84
                        resources:DestroyMovie(pCVar9)
                        goto FLOW_after_lab_00d41813
                    end
                    bVar3 = quest:IsQuestActive("Q_GuildTrainingDeparture")
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            resources:ReleaseResource(0)
                            return
                        end
                        bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar3 then
                            if bVar4 then
                                resources:ReleaseResource(0)
                                return
                            end
                            xStack_64 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_24 = resources:ScriptThing(0)
                            pCVar6 = xStack_24
                            r8 = quest:GetHealth(pCVar6)
                            fVar2 = 0.0
                            if fVar2 < fret_02 then
                                iVar14 = 0
                                iVar13 = 1
                                iVar12 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_028_WHISPER_END_MOAN"
                                pCVar6 = quest:GetHero()
                                r9 = me:Speak(pCVar6, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar5 = iVar11
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar9 = xStack_64
                                        resources:DestroyMovie(pCVar9)
                                        resources:ReleaseResource(0)
                                        return
                                    end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar5 = iVar11
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar9 = xStack_64
                                    __cleanup_LAB_00d41a02()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar9 = xStack_64
                        else
                            if bVar4 then
                                resources:ReleaseResource(0)
                                return
                            end
                            xStack_a4 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_c = resources:ScriptThing(0)
                            pCVar6 = xStack_c
                            r10 = quest:GetHealth(pCVar6)
                            fVar2 = 0.0
                            if fVar2 < fret_03 then
                                iVar14 = 0
                                iVar13 = 1
                                iVar12 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_028_WHISPER_MELEE_MOAN"
                                pCVar6 = quest:GetHero()
                                r11 = me:Speak(pCVar6, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar5 = iVar11
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        __cleanup_LAB_00d419fe(); return
                                    end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar5 = iVar11
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    __cleanup_LAB_00d419fe()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar9 = xStack_a4
                        end
                        resources:DestroyMovie(pCVar9)
                        goto FLOW_after_lab_00d41813
                    end
                end
                ::FLOW_after_lab_00d41813::
                if r1 == nil then
                else
                    puVar8 = (**(*r1 + 0x18))()
                end
                me:MoveToPosition(pCVar7, puVar8, 0x40400000, true, false)
            end
            bVar3 = quest:IsDistanceBetweenThingsOver(me, r1, 4.0)
            __native_condition_1 = bVar3
            if __native_condition_1 then
                iVar11 = me:IsPerformingScriptTask()
                __native_condition_1 = not iVar11
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00d41a07: (native jump target)
                    resources:ReleaseResource(0)
                    return
                end
                if r1 == nil then
                else
                    puVar8 = (**(*r1 + 0x18))()
                end
                me:MoveToPosition(nil --[[missing]], puVar8, 0x40400000, true, false)
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WaitingForFight", true)
    __native_entity_state:SetStateBool("WillWoodsChatDone", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

