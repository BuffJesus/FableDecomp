-- Generated native draft: CampHostage2. Review coverage report before use.
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
    local bVar3, cVar1, dist, iVar4, iVar5, i_stk_30, i_stk_34, p0, pCVar6, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        bVar3 = resources:TryAcquire(xStack_20, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d08a59 end
            bVar3 = resources:TryAcquire(xStack_20, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            iVar4 = quest:RegisterTimer()
            i_stk_34 = iVar4
            quest:SetTimer(i_stk_34, 0)
            cVar1 = quest:GetStateBool("HostagesRescued")
            i_stk_30 = 0
            while (not cVar1 and (not quest:GetStateBool("HostageKilled"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d08a50 end
                iVar5 = quest:GetTimer(i_stk_34)
                if iVar5 < 1 then
                    dist = 8.0
                    pCVar6 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, dist)
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d08a50 end
                        bVar3 = false
                        pCVar6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                        iVar4 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar4, pCVar6)
                        me:PlayAnimation("CS_DISAPPOINTED", false, false, false, true, true, false, false)
                        if i_stk_30 == 0 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST", me, pCVar6, false)
                            i_stk_30 = 1
                        elseif i_stk_30 == 1 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND", me, pCVar6, false)
                            i_stk_30 = 0
                        end
                        quest:SetTimer(i_stk_34, 8)
                    end
                end
                cVar1 = quest:GetStateBool("HostagesRescued")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                resources:PrepareResource(xStack_20)
            end
            ::LAB_00d08a50::
            quest:DeregisterTimer(i_stk_34)
        end
        ::LAB_00d08a59::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_NEUTRALS")
    quest:EntitySetOpinionReactionEnabled(me, 0x22, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

