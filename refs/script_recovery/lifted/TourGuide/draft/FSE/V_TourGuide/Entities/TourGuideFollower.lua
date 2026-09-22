-- Generated native draft: TourGuideFollower. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, bVar2, cVar3, iVar4, iVar5, i_stk_50, pCVar6, pOther, pThing, puVar7, r1, xStack_10, xStack_40, xStack_54
    local alive = true
    iVar4 = quest:RegisterTimer()
    i_stk_50 = iVar4
    quest:SetTimer(i_stk_50, 0)
    xStack_10 = resources:NewResource()
    resources:PrepareResource(xStack_10)
    bVar2 = resources:TryAcquire(xStack_10, me, 2)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_10)
            quest:DeregisterTimer(i_stk_50)
            return
        end
        bVar2 = resources:TryAcquire(xStack_10, me, 2)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        r1 = quest:GetThingWithScriptName("TourGuideGuide")
        xStack_40 = nil
        if not quest:GetStateBool("TourGuideKilled") then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00ee52f3: (native jump target)
                resources:ReleaseResource(xStack_10)
                quest:DeregisterTimer(i_stk_50)
                return
            end
            me:FollowThing(r1, quest:ReadGlobalGameData(0x8d0), true)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while true do
            if bVar2 then
                xStack_40 = nil
                r1 = nil
                resources:ReleaseResource(xStack_10)
                quest:DeregisterTimer(i_stk_50)
                return
            end
            if (quest:GetStateBool("TourGuideKilled")) or (quest:GetStateBool("TourFinished")) then break end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                resources:ReleaseResource(xStack_10)
                quest:DeregisterTimer(i_stk_50)
                return
            end
            bVar2 = me:IsTalkedToByHero()
            __native_condition_1 = bVar2
            if __native_condition_1 then
                iVar5 = quest:GetTimer(i_stk_50)
                __native_condition_1 = iVar5 == 0
            end
            if __native_condition_1 then
                bVar2 = true
            else
                bVar2 = false
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    resources:ReleaseResource(xStack_10)
                    quest:DeregisterTimer(i_stk_50)
                    return
                end
                me:ClearCommands()
                bVar2 = false
                pCVar6 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar2)
                iVar4 = math.random(0, 32767)
                iVar5 = quest:EntityGetSex(me)
                if iVar5 == 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        -- LAB_00ee5237: (native jump target)
                        resources:ReleaseResource(xStack_10)
                        quest:DeregisterTimer(i_stk_50)
                        return
                    end
                    pOther = (__native_entity_state:GetStateInt("self_0x14") + 0x13c + (iVar4 % 5) * 4)
                    goto LAB_00ee50ab
                elseif iVar5 == 2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        resources:ReleaseResource(xStack_10)
                        quest:DeregisterTimer(i_stk_50)
                        return
                    end
                    pOther = (__native_entity_state:GetStateInt("self_0x14") + 0x150 + (iVar4 % 5) * 4)
                    goto LAB_00ee50ab
                end
                goto FLOW_past_lab_00ee50ab
                ::LAB_00ee50ab::
                xStack_54 = pOther
                ::FLOW_past_lab_00ee50ab::
                iVar5 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar5, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar5, xStack_54, me, pCVar6, false)
                quest:SetTimer(i_stk_50, quest:ReadGlobalGameData(0x8e4))
                me:FollowThing(nil --[[missing]], quest:ReadGlobalGameData(0x8d0), true)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:ClearThingHasInformation(me)
            resources:PrepareResource(xStack_10)
            bVar2 = resources:TryAcquire(xStack_10, me, 4)
            while not bVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    resources:ReleaseResource(xStack_10)
                    quest:DeregisterTimer(i_stk_50)
                    return
                end
                bVar2 = resources:TryAcquire(xStack_10, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                iVar4 = me:IsPerformingScriptTask()
                cVar3 = iVar4
                while cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00ee5535 end
                    iVar4 = me:IsPerformingScriptTask()
                    cVar3 = iVar4
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    pCVar6 = quest:GetThingWithScriptName("M_TG_ClosingTimeExit")
                    xStack_40 = pCVar6
                    bVar2 = false
                    __native_condition_2 = bVar2
                    if __native_condition_2 then
                        iVar4 = (xStack_40 ~= nil and xStack_40:IsAlive())
                        __native_condition_2 = iVar4
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            if not (xStack_40 ~= nil and not xStack_40:IsNull()) then
                                puVar7 = {x = 0, y = 0, z = 0}
                            else
                                puVar7 = xStack_40:GetPos()
                            end
                            -- TODO(native): xStack_28 = puVar7.x;
                            iVar4 = 3.0
                            -- TODO(native): pvVar8 = (void *)CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)xStack_10,(int)&xStack_1c);
                            -- TODO(native): iVar4 = IsDistanceFromThingToPositionOver(pvVar8,&xStack_28,iVar4);
                            iVar4 = nil --[[unresolved native result]]
                            cVar3 = iVar4
                            while cVar3 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00ee5535 end
                                -- TODO(native): me:MoveToPosition(puVar7, &xStack_28, 0x3f800000, false, false)
                                iVar4 = me:IsPerformingScriptTask()
                                cVar3 = iVar4
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00ee5535 end
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00ee5535 end
                                iVar4 = 3.0
                                -- TODO(native): pvVar8 = (void *)CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *) xStack_10,(int)&xStack_1c);
                                -- TODO(native): iVar4 = IsDistanceFromThingToPositionOver(pvVar8,&xStack_28,iVar4);
                                iVar4 = nil --[[unresolved native result]]
                                cVar3 = iVar4
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                quest:FadeOutAndKillEntity(me, true, 1.0, true)
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            resources:PrepareResource(xStack_10)
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                            until not (not bVar2)
                        end
                    end
                end
                ::LAB_00ee5535::
                resources:ReleaseResource(xStack_10)
                quest:DeregisterTimer(i_stk_50)
                return
            end
        end
    end
    resources:ReleaseResource(xStack_10)
    quest:DeregisterTimer(i_stk_50)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("FollowerSpokenToHeroThisWaypoint", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

