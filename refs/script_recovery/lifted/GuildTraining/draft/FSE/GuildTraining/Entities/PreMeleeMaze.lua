-- Generated native draft: PreMeleeMaze. Review coverage report before use.
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
    local __native_condition_1, bVar3, bVar4, bVar6, cVar5, fVar2, fret_0, fret_00, iVar10, iVar11, iVar12, iVar7, p0, pCVar8, pThing, pcVar9, r1, r2, r3, r4, xStack_10, xStack_20, xStack_30, xStack_48, x_stk_3c
    local alive = true
    bVar6 = false
    bVar3 = false
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    r1 = quest:GetThingWithScriptName("PreMeleeMazeTargetMarker")
    xStack_30 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_30)
            r1 = nil
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
        bVar4 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        resources:ReleaseResource(xStack_30)
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    while true do
        if bVar4 then
            resources:ReleaseResource(xStack_30)
            r1 = nil
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
        if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
            bVar4 = false
            if bVar4 ~= 0 then
            end
            cVar5 = quest:GetMasterGameState("GuildWarningOccuring")
            while cVar5 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d444a1 end
                cVar5 = quest:GetMasterGameState("GuildWarningOccuring")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
            bVar4 = false
            if bVar4 ~= 0 then
            end
            bVar4 = resources:TryAcquire(xStack_30, me, 4)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d444a1 end
                bVar4 = resources:TryAcquire(xStack_30, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
        end
        bVar4 = quest:IsDistanceBetweenThingsOver(me, r1, 4.0)
        __native_condition_1 = bVar4
        if __native_condition_1 then
            iVar7 = me:IsPerformingScriptTask()
            __native_condition_1 = not iVar7
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
            if not (r1 ~= nil and not r1:IsNull()) then
            else
                p0 = r1:GetPos()
            end
            me:MoveToPosition(p0, 3.0, 0, false, true)
        end
        bVar4 = me:IsTalkedToByHero()
        if bVar4 then break end
        -- LAB_00d441bb: (native jump target)
        bVar4 = me:MsgIsHitByHero()
        if bVar4 then
            -- LAB_00d44243: (native jump target)
            bVar4 = true
        else
            bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar6 then
                bVar6 = true
                bVar3 = true
                bVar4 = me:MsgIsHitByHeroSpecialAbility(me)
                if not bVar4 then
                    bVar4 = true
                    goto FLOW_after_lab_00d44243
                end
            end
            bVar6 = true
            bVar4 = false
        end
        ::FLOW_after_lab_00d44243::
        if bVar3 then
            bVar3 = false
        end
        if bVar6 then
            bVar6 = false
        end
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
            me:ClearCommands()
            xStack_10 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(bVar6)
            x_stk_3c = resources:ScriptThing(xStack_30)
            pCVar8 = x_stk_3c
            fret_00 = quest:GetHealth(pCVar8)
            fVar2 = 0.0
            if fVar2 < fret_00 then
                iVar12 = 0
                iVar11 = 1
                iVar10 = 0
                iVar7 = 0
                pcVar9 = "TEXT_QST_028_MAZE_HIT"
                pCVar8 = quest:GetHero()
                r2 = me:Speak(pCVar8, pcVar9, iVar7, (iVar10 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                cVar5 = iVar7
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                        goto LAB_00d44498
                    end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar5 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d44494: (native jump target)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                    goto LAB_00d44498
                end
            end
            me:SetFriendsWithEverythingFlag(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    end
    ::FLOW_after_lab_00d441bb::
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00d444a1 end
    me:ClearCommands()
    xStack_20 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    xStack_48 = resources:ScriptThing(xStack_30)
    pCVar8 = xStack_48
    fret_0 = quest:GetHealth(pCVar8)
    fVar2 = 0.0
    if fret_0 <= fVar2 then
        -- LAB_00d441a3: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_20)
        bVar4 = me:MsgIsHitByHero()
        if bVar4 then
            -- LAB_00d44243_c3: (native jump target)
            bVar4 = true
        end
        if bVar3 then
            bVar3 = false
        end
        if bVar6 then
            bVar6 = false
        end
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d444a1 end
            me:ClearCommands()
            xStack_10 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(bVar6)
            x_stk_3c = resources:ScriptThing(xStack_30)
            pCVar8 = x_stk_3c
            fret_00 = quest:GetHealth(pCVar8)
            fVar2 = 0.0
            if fVar2 < fret_00 then
                iVar12 = 0
                iVar11 = 1
                iVar10 = 0
                iVar7 = 0
                pcVar9 = "TEXT_QST_028_MAZE_HIT"
                pCVar8 = quest:GetHero()
                r3 = me:Speak(pCVar8, pcVar9, iVar7, (iVar10 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                cVar5 = iVar7
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00d44494_c3
                    end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar5 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d44494_c3: (native jump target)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                    goto LAB_00d44498
                end
            end
            me:SetFriendsWithEverythingFlag(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        goto FLOW_after_lab_00d441bb
    end
    iVar12 = 0
    iVar11 = 1
    iVar10 = 0
    iVar7 = 0
    pcVar9 = "TEXT_QST_028_MAZE_LEAVE_ME"
    pCVar8 = quest:GetHero()
    r4 = me:Speak(pCVar8, pcVar9, iVar7, (iVar10 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
    iVar7 = me:IsPerformingScriptTask()
    cVar5 = iVar7
    while cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
            goto LAB_00d44498
        end
        iVar7 = me:IsPerformingScriptTask()
        cVar5 = iVar7
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then return end  -- TODO(native): goto LAB_00d441a3
    quest:PauseAllNonScriptedEntities(false)
    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
    ::LAB_00d44498::
    resources:DestroyMovie(this_00)
    ::LAB_00d444a1::
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

