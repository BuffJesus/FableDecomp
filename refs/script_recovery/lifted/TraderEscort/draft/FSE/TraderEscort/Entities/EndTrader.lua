-- Generated native draft: EndTrader. Review coverage report before use.
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
    local bVar2, cVar3, fVar1, fVar10, fret_0, fret_00, fret_01, fret_02, iVar11, iVar7, iVar8, iVar9, p0, pCVar4, pcVar6, piVar5, r1, r2, r3, r4, r5, r6, this_00, xStack_10, xStack_20, xStack_30, xStack_54, xStack_68, xStack_88
    local alive = true
    local function __region_LAB_00e0601a()
        this_00 = xStack_10
        resources:DestroyMovie(this_00)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_88 = resources:NewResource()
    resources:PrepareResource(xStack_88)
    bVar2 = resources:TryAcquire(xStack_88, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e06038 end
        bVar2 = resources:TryAcquire(xStack_88, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e06038 end
    r1 = quest:GetThingWithScriptName("M_EndTheQuestHere")
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        while quest:GetStateBool("IntroFinished") do
            fVar10 = 3.0
            pCVar4 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(r1, pCVar4, fVar10)
            while not bVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e0602f end
                fVar10 = 3.0
                pCVar4 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(r1, pCVar4, fVar10)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e0602f end
            xStack_68 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
            iVar7 = quest:GetStateInt("TradersStillAliveCounter")
            if (#xStack_68 == iVar7) and (0 < iVar7) then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto FLOW_after_lab_00e0602a
                end
                __native_entity_state:SetStateBool("EndingCanStart", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto FLOW_after_lab_00e0602a
                end
            end
            if __native_entity_state:GetStateBool("EndingCanStart") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto FLOW_after_lab_00e0602a
                end
                xStack_54 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:SheatheHeroWeapons()
                quest:Pause(0.2)
                xStack_30 = resources:ScriptThing(xStack_88)
                pCVar4 = xStack_30
                fret_01 = quest:GetHealth(pCVar4)
                fVar1 = 0.0
                if fret_01 <= fVar1 then goto LAB_00e05c45 end
                iVar11 = 0
                iVar9 = 1
                iVar8 = 0
                iVar7 = 0
                pcVar6 = "TEXT_QST_067_ENDTRADER_GREETINGS"
                pCVar4 = quest:GetHero()
                r2 = me:Speak(pCVar4, pcVar6, iVar7, (iVar8 ~= 0), (iVar9 ~= 0), (iVar11 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                cVar3 = iVar7
                goto LAB_00e05be8
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                goto FLOW_after_lab_00e0602a
            end
            xStack_20 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:Pause(0.5)
            xStack_54 = resources:ScriptThing(xStack_88)
            pCVar4 = xStack_54
            fret_0 = quest:GetHealth(pCVar4)
            fVar1 = 0.0
            if fVar1 < fret_0 then
                iVar11 = 0
                iVar9 = 1
                iVar8 = 0
                iVar7 = 0
                pcVar6 = "TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT"
                pCVar4 = quest:GetHero()
                r3 = me:Speak(pCVar4, pcVar6, iVar7, (iVar8 ~= 0), (iVar9 ~= 0), (iVar11 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                cVar3 = iVar7
                while cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_20
                        resources:DestroyMovie(this_00)
                        -- LAB_00e0602a_c5: (native jump target)
                        goto FLOW_after_lab_00e0602a
                    end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar3 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_20
                    resources:DestroyMovie(this_00)
                    -- LAB_00e0602a_c6: (native jump target)
                    goto FLOW_after_lab_00e0602a
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_20)
            bVar2 = quest:IsRegionLoaded("BarrowFields")
            if bVar2 then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        goto FLOW_after_lab_00e0602a
                    end
                    bVar2 = me:IsTalkedToByHero()
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto FLOW_after_lab_00e0602a
                        end
                        xStack_10 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_30 = resources:ScriptThing(xStack_88)
                        pCVar4 = xStack_30
                        fret_00 = quest:GetHealth(pCVar4)
                        fVar1 = 0.0
                        if fVar1 < fret_00 then
                            iVar11 = 0
                            iVar9 = 1
                            iVar8 = 0
                            iVar7 = 0
                            pcVar6 = "TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT"
                            pCVar4 = quest:GetHero()
                            r4 = me:Speak(pCVar4, pcVar6, iVar7, (iVar8 ~= 0), (iVar9 ~= 0), (iVar11 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar3 = iVar7
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    __region_LAB_00e0601a(); return  -- TODO(native): goto FLOW_after_lab_00e0602a
                                end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar3 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                __region_LAB_00e0601a()
                                goto FLOW_after_lab_00e0602a
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                    end
                    bVar2 = quest:IsRegionLoaded("BarrowFields")
                until not (bVar2)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                goto FLOW_after_lab_00e0602a
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e0602f end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            resources:PrepareResource(xStack_88)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
            until not (not bVar2)
        end
    end
    ::LAB_00e0602f::
    ::LAB_00e06038::
    resources:ReleaseResource(xStack_88)
    do return end
    ::LAB_00e05be8::
    if not cVar3 then goto LAB_00e05c10 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:PauseAllNonScriptedEntities(false)
        this_00 = xStack_54
        resources:DestroyMovie(this_00)
        -- LAB_00e0602a_c11: (native jump target)
        goto FLOW_after_lab_00e0602a
    end
    iVar7 = me:IsPerformingScriptTask()
    cVar3 = iVar7
    goto LAB_00e05be8
    ::LAB_00e05c10::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:PauseAllNonScriptedEntities(false)
        this_00 = xStack_54
        -- LAB_00e06021: (native jump target)
        resources:DestroyMovie(this_00)
        -- LAB_00e0602a: (native jump target)
    else
        goto LAB_00e05c45
    end
    ::FLOW_after_lab_00e0602a::
    goto FLOW_past_lab_00e05c45
    ::LAB_00e05c45::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_54)
    quest:SetStateBool("EndStarted", true)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        r5 = quest:GetThingWithScriptName("TraderEndPos")
        if not (r5 ~= nil and not r5:IsNull()) then
            piVar5 = {x = 0, y = 0, z = 0}
        else
            piVar5 = r5:GetPos()
        end
        xStack_68 = {x = piVar5.x, y = piVar5.y, z = piVar5.z}
        fVar10 = 5.0
        xStack_54 = resources:ScriptThing(xStack_88)
        pCVar4 = xStack_54
        bVar2 = (pCVar4 ~= nil and pCVar4:IsDistanceFromPositionOver(xStack_68, fVar10))
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e05fd1 end
                me:MoveToPosition(xStack_68, 3.0, 0, false, true)
                iVar7 = me:IsPerformingScriptTask()
                cVar3 = iVar7
                while cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e05fd1 end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar3 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e05fd1 end
                fVar10 = 5.0
                xStack_54 = resources:ScriptThing(xStack_88)
                pCVar4 = xStack_54
                bVar2 = (pCVar4 ~= nil and pCVar4:IsDistanceFromPositionOver(xStack_68, fVar10))
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:MiniMapRemoveMarker(me)
            resources:PrepareResource(xStack_88)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                repeat
                    bVar2 = me:IsTalkedToByHero()
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e05fd1 end
                        resources:PrepareResource(xStack_88)
                        bVar2 = resources:TryAcquire(xStack_88, me, 4)
                        while not bVar2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e05fd1 end
                            bVar2 = resources:TryAcquire(xStack_88, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e05fd1 end
                        xStack_54 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_30 = resources:ScriptThing(xStack_88)
                        pCVar4 = xStack_30
                        fret_02 = quest:GetHealth(pCVar4)
                        fVar1 = 0.0
                        if fVar1 < fret_02 then
                            iVar11 = 0
                            iVar9 = 1
                            iVar8 = 0
                            iVar7 = 1
                            pcVar6 = "TEXT_QST_067_ENDTRADER_THANKS"
                            pCVar4 = quest:GetHero()
                            r6 = me:Speak(pCVar4, pcVar6, iVar7, (iVar8 ~= 0), (iVar9 ~= 0), (iVar11 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar3 = iVar7
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_54)
                                    goto LAB_00e05fd1
                                end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar3 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00e05fc8: (native jump target)
                                resources:DestroyMovie(xStack_54)
                                goto LAB_00e05fd1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_54)
                        resources:PrepareResource(xStack_88)
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                until not (not bVar2)
                goto LAB_00e0602f
            end
        end
        ::LAB_00e05fd1::
    end
    ::FLOW_past_lab_00e05c45::
    goto LAB_00e0602f
end

function Init(quest, me)
    __native_entity_state:SetStateBool("EndingCanStart", false)
    quest:SetIsPushableByHero(me, false)
    if quest:GetStateBool("IntroFinished") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    end
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if (cVar1) and (__native_entity_state:GetStateBool("EndingCanStart")) then
        quest:SetStateBool("EndStarted", true)
    end
end

