-- Generated native draft: TeamSpawn. Review coverage report before use.
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
    local __gsivt_1, __gsivt_2, __gsivt_3, __gsivt_4, __gsivt_5, __gsivt_6, bVar3, fVar9, iVar4, iVar7, i_stk_70, pCVar5, pCVar6, pCVar8, r1, r2, r3, thing1
    local alive = true
    local cVar1 = quest:GetStateBool("DoneIntroduction")
    while not cVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar1 = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
        if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            quest:DisplayQuestInfo(true)
            iVar4 = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", __native_entity_state:GetStateInt("TeamMemberLimit") * 3, 1.0)
            __native_entity_state:SetStateInt("BanditsLeftID", iVar4)
            quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), __native_entity_state:GetStateInt("TeamMemberLimit") * 3, -1)
        end
        cVar1 = quest:GetStateBool("WhisperSpawned")
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            if __native_entity_state:GetStateInt("TeamID") == 0 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                iVar4 = quest:GetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")))
                if (iVar4 == 0) and (quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) < __native_entity_state:GetStateInt("TeamMemberLimit")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
                    iVar4 = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))
                    fVar9 = 15.0
                    thing1 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsOver(thing1, me, fVar9)
                    if bVar3 then
                        __gsivt_1 = **(this + 4)
                        pCVar5 = me:GetPos()
                        bVar3 = quest:IsCameraPosOnScreen(pCVar5)
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            i_stk_70 = 0
                            if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 ~= 0 then
                                repeat
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        return
                                    end
                                    __gsivt_2 = **(this + 4)
                                    bVar3 = false
                                    pCVar8 = __native_entity_state:GetStateString("TeamMemberName")
                                    pCVar5 = me:GetPos()
                                    r1 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), pCVar5, pCVar8)
                                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                                    quest:Pause(2.0)
                                    i_stk_70 = i_stk_70 + 1
                                until not (i_stk_70 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
                            end
                            -- TODO(native): goto LAB_00dcd9ed
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    i_stk_70 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 ~= 0 then
                        repeat
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            __gsivt_3 = **(this + 4)
                            bVar3 = false
                            pCVar8 = __native_entity_state:GetStateString("TeamMemberName")
                            pCVar5 = __native_entity_state:GetStateThing("OtherSpawnPoint"):GetPos()
                            r2 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), pCVar5, pCVar8)
                            quest:EntityAttachToScript(r2, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            pCVar6 = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                            __native_entity_state:SetStateThing("OtherSpawnPoint", pCVar6)
                            i_stk_70 = i_stk_70 + 1
                        until not (i_stk_70 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
                    end
                    -- LAB_00dcd9ed: (native jump target)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * __native_entity_state:GetStateInt("TeamMemberLimit") + quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")), -1)
                end
                if quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) == 0 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:Pause(quest:ReadGlobalGameData(0xd74))
                    quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                    if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        __gsivt_4 = **(this + 4)
                        pCVar6 = quest:GetHero()
                        r3 = quest:GetNearestWithScriptName(pCVar6, "GuardTeamMember")
                        iVar4 = (r3 ~= nil and r3:IsAlive())
                        if iVar4 then
                            fVar9 = 15.0
                            pCVar6 = quest:GetHero()
                            bVar3 = quest:IsDistanceBetweenThingsUnder(r3, pCVar6, fVar9)
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "NEXT_WAVE", r3, 0)
                            end
                        end
                    end
                    if quest:GetStateInt("BanditWavesSpawned") == 3 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") ~= 0 then
                            return
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        quest:SetStateInt("MissionFailed", 3)
                        return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    iVar4 = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))
                    iVar7 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 ~= 0 then
                        repeat
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            r3 = resources:StartMovie("")
                            if (iVar7 == 1) and (quest:GetStateInt("HeroTeam") == 0) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                __gsivt_5 = **(this + 4)
                                bVar3 = false
                                pCVar8 = __native_entity_state:GetStateString("TeamMemberName")
                                pCVar5 = me:GetPos()
                                pCVar6 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", pCVar5, pCVar8)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end
                                __gsivt_6 = **(this + 4)
                                bVar3 = false
                                pCVar8 = __native_entity_state:GetStateString("TeamMemberName")
                                pCVar5 = me:GetPos()
                                pCVar6 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), pCVar5, pCVar8)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            end
                            quest:EntityAttachToScript(pCVar6, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            iVar7 = iVar7 + 1
                        until not (iVar7 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
                    end
                    -- TODO(native): goto LAB_00dcd9ed
                end
            end
            cVar1 = quest:GetStateBool("WhisperSpawned")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BanditsLeftID"))
        end
    end
end

function Init(quest, me)
    local bVar7, iVar5, pCVar3, pcVar8, piVar4, uVar1
    local function __region_LAB_00dcd306()
        __native_entity_state:SetStateInt("TeamID", 1)
        __native_entity_state:SetStateString("TeamMemberName", "BanditTeamMember")
        pcVar8 = "CREATURE_BANDIT_GRUNT"
    end
    pCVar3 = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    __native_entity_state:SetStateThing("OtherSpawnPoint", pCVar3)
    pCVar3 = nil
    -- TODO(native): GetName is not a ForgeFSE binding
    piVar4 = me:GetName()
    if piVar4 == nil then
        bVar7 = false
        if bVar7 then
            __region_LAB_00dcd306()
            goto LAB_00dcd2bb
        end
    else
        iVar5 = ((piVar4 == "BanditTeamSpawn") and 0 or 1)
        if iVar5 == 0 then __region_LAB_00dcd306(); goto LAB_00dcd2bb end
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    __native_entity_state:SetStateString("TeamMemberName", "GuardTeamMember")
    pcVar8 = "CREATURE_ORCHARD_FARM_GUARD"
    ::LAB_00dcd2bb::
    __native_entity_state:SetStateString("TeamMemberDefName", pcVar8)
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") ~= 0 then
        __native_entity_state:SetStateInt("TeamMemberLimit", 2)
        uVar1 = quest:ReadGlobalGameData(0xd6c)
        __native_entity_state:SetStateInt("BanditsLeftID", 0)
        __native_entity_state:SetStateInt("TeamRespawnTime", uVar1)
        return
    end
    __native_entity_state:SetStateInt("TeamMemberLimit", 3)
    uVar1 = quest:ReadGlobalGameData(0xd70)
    __native_entity_state:SetStateInt("BanditsLeftID", 0)
    __native_entity_state:SetStateInt("TeamRespawnTime", uVar1)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

