-- Generated native draft: GTDI_Maze. Review coverage report before use.
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
    local b3, bVar17, bVar5, cVar6, fVar4, fret_0, fret_00, fret_01, iVar14, iVar16, iVar18, iVar19, p0, pCVar10, pCVar7, pCVar8, pcVar13, pppuVar15, r1, r2, r3, this_00, xStack_10, xStack_20, xStack_30, xStack_40, xStack_58, x_stk_4c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    quest:SetThingHasInformation(me, false, true, false)
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar5 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e28c8a end
        bVar5 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAsDamageable(me, false)
        quest:SetIsPushableByHero(me, false)
        bVar17 = false
        bVar5 = false
        pCVar7 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pCVar7, bVar5, bVar17)
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            repeat
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00e280a7
                else
                    bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar5 then
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00e280a7 end
                    end
                    bVar5 = false
                end
                goto FLOW_past_lab_00e280a7
                ::LAB_00e280a7::
                bVar5 = true
                ::FLOW_past_lab_00e280a7::
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    pCVar8 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar8)
                    pCVar10 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pCVar10, me)
                    xStack_40 = resources:NewResource()
                    resources:PrepareResource(xStack_40)
                    iVar18 = 4
                    pppuVar15 = xStack_40
                    pCVar8 = quest:GetHero()
                    bVar5 = resources:TryAcquire(pppuVar15, pCVar8, iVar18)
                    if not bVar5 then goto LAB_00e281e0 end
                    goto LAB_00e28214
                end
                bVar5 = me:IsTalkedToByHero()
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_40 = resources:NewResource()
                    resources:PrepareResource(xStack_40)
                    iVar18 = 4
                    pppuVar15 = xStack_40
                    pCVar8 = quest:GetHero()
                    bVar5 = resources:TryAcquire(pppuVar15, pCVar8, iVar18)
                    if not bVar5 then goto LAB_00e28460 end
                    goto LAB_00e28494
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_30)
                    return
                end
            until false
        end
    end
    goto LAB_00e28c8a
    ::LAB_00e281e0::
    while true do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            return
        end
        iVar18 = 4
        pppuVar15 = xStack_40
        pCVar8 = quest:GetHero()
        bVar5 = resources:TryAcquire(pppuVar15, pCVar8, iVar18)
        if bVar5 then break end
    end
    ::LAB_00e28214::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        return
    end
    resources:PrepareResource(xStack_30)
    bVar5 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            return
        end
        bVar5 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        return
    end
    xStack_58 = resources:NewActorMap()
    resources:SetActor(xStack_58, "MAZE", xStack_30)
    resources:SetActor(xStack_58, "HERO", xStack_40)
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_SETUP", xStack_58, false, false)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_HITME", xStack_58, false, true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO", xStack_58, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_58)
    resources:ReleaseResource(xStack_40)
    goto LAB_00e28674
    ::LAB_00e28c3f::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00e28c85::
    resources:ReleaseResource(this_00)
    goto LAB_00e28c8a
    ::LAB_00e28460::
    while true do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            return
        end
        iVar18 = 4
        pppuVar15 = xStack_40
        pCVar8 = quest:GetHero()
        bVar5 = resources:TryAcquire(pppuVar15, pCVar8, iVar18)
        if bVar5 then break end
    end
    ::LAB_00e28494::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00e28502: (native jump target)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        return
    end
    resources:PrepareResource(xStack_30)
    bVar5 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            return
        end
        bVar5 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        return
    end
    xStack_58 = resources:NewActorMap()
    resources:SetActor(xStack_58, "MAZE", xStack_30)
    resources:SetActor(xStack_58, "HERO", xStack_40)
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_SETUP", xStack_58, false, false)
    resources:RunMacro("CS_MAZE_TROPHY_INFO", xStack_58, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_58)
    resources:ReleaseResource(xStack_40)
    ::LAB_00e28674::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIND_TROPHY_DEALER", "V_TrophyDealer", false)
        bVar5 = true
        pCVar8 = quest:GetThingWithScriptName("WitchwoodTeleporter")
        quest:SetTeleporterAsActive(pCVar8, bVar5)
        quest:ClearThingHasInformation(me)
        quest:MiniMapRemoveMarker(me)
        quest:EntitySetFacingAngle(me, __native_entity_state:GetStateFloat("InitialAngle"), false)
        quest:AddLogbookStoryEntry(130)
        b3 = false
        bVar17 = false
        bVar5 = false
        pCVar7 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar7, bVar5, bVar17, b3)
        quest:SetStateBool("PieceOver", true)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            repeat
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00e28814
                else
                    bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar5 then
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00e28814 end
                    end
                    bVar5 = false
                end
                goto FLOW_past_lab_00e28814
                ::LAB_00e28814::
                bVar5 = true
                ::FLOW_past_lab_00e28814::
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        pCVar8 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(me, pCVar8)
                        pCVar10 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(pCVar10, me)
                        resources:PrepareResource(xStack_30)
                        bVar5 = resources:TryAcquire(xStack_30, me, 4)
                        while not bVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e28c8a end
                            bVar5 = resources:TryAcquire(xStack_30, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            xStack_20 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            bVar5 = false
                            pCVar8 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar5)
                            pCVar8 = resources:ScriptThing(xStack_30)
                            pCVar8 = pCVar8
                            fret_0 = quest:GetHealth(pCVar8)
                            fVar4 = 0.0
                            if fret_0 <= fVar4 then
                                goto LAB_00e28a13
                            end
                            goto FLOW_past_lab_00e28a13
                            ::LAB_00e28a13::
                            x_stk_4c = resources:ScriptThing(xStack_30)
                            pCVar8 = x_stk_4c
                            fret_00 = quest:GetHealth(pCVar8)
                            fVar4 = 0.0
                            if fVar4 < fret_00 then
                                iVar19 = 0
                                iVar16 = 1
                                iVar18 = 0
                                iVar14 = 0
                                pcVar13 = "TEXT_QST_077_MAZE_REPEAT"
                                pCVar8 = quest:GetHero()
                                r1 = me:Speak(pCVar8, pcVar13, iVar14, (iVar18 ~= 0), (iVar16 ~= 0), (iVar19 ~= 0))
                                iVar14 = me:IsPerformingScriptTask()
                                cVar6 = iVar14
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e28c3f end
                                    iVar14 = me:IsPerformingScriptTask()
                                    cVar6 = iVar14
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e28c3f end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20)
                            goto LAB_00e28ac9
                            ::FLOW_past_lab_00e28a13::
                            iVar19 = 0
                            iVar16 = 1
                            iVar18 = 0
                            iVar14 = 0
                            pcVar13 = "TEXT_QST_077_MAZE_ON_HIT"
                            pCVar8 = quest:GetHero()
                            r2 = me:Speak(pCVar8, pcVar13, iVar14, (iVar18 ~= 0), (iVar16 ~= 0), (iVar19 ~= 0))
                            iVar14 = me:IsPerformingScriptTask()
                            cVar6 = iVar14
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e28c3f end
                                iVar14 = me:IsPerformingScriptTask()
                                cVar6 = iVar14
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then goto LAB_00e28a13 end
                            -- LAB_00e28c52: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_20
                            goto LAB_00e28c85
                        end
                    end
                    break
                end
                ::LAB_00e28ac9::
                bVar5 = me:IsTalkedToByHero()
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    bVar5 = false
                    pCVar8 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar5)
                    xStack_40 = resources:ScriptThing(xStack_30)
                    pCVar8 = xStack_40
                    fret_01 = quest:GetHealth(pCVar8)
                    fVar4 = 0.0
                    if fVar4 < fret_01 then
                        iVar19 = 0
                        iVar16 = 1
                        iVar18 = 0
                        iVar14 = 0
                        pcVar13 = "TEXT_QST_077_MAZE_REPEAT"
                        pCVar8 = quest:GetHero()
                        r3 = me:Speak(pCVar8, pcVar13, iVar14, (iVar18 ~= 0), (iVar16 ~= 0), (iVar19 ~= 0))
                        iVar14 = me:IsPerformingScriptTask()
                        cVar6 = iVar14
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e28c81
                            end
                            iVar14 = me:IsPerformingScriptTask()
                            cVar6 = iVar14
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00e28c81
                        end
                        goto FLOW_past_lab_00e28c81
                        ::LAB_00e28c81::
                        this_00 = xStack_10
                        goto LAB_00e28c85
                        ::FLOW_past_lab_00e28c81::
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_30)
                    return
                end
            until false
        end
    end
    ::LAB_00e28c8a::
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
    local fVar1 = me:GetAngleXY()
    __native_entity_state:SetStateFloat("InitialAngle", fVar1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

