-- Generated native draft: BanditTeamSpawn. Review coverage report before use.
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
    local bVar3, fVar8, iVar4, iVar9, pCVar5, pCVar6, pCVar7, r1, r2, r3, thing1
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
        quest:SetTimer(quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_TeamReinforcementsTimer"), __native_entity_state:GetStateInt("TeamRespawnTime"))
        if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            quest:DisplayQuestInfo(true)
            iVar4 = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", &"HUD_QUEST_ICON_BANDIT", __native_entity_state:GetStateInt("TeamMemberLimit") * 3)
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
            if __native_entity_state:GetStateInt("TeamID") == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                iVar4 = quest:GetTimer(* (__native_entity_state:GetStateInt("TeamID") * 0x40 + 0xa8 + __native_entity_state:GetStateInt("self_0x14")))
                if (iVar4 == 0) and (quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount") < __native_entity_state:GetStateInt("TeamMemberLimit")) then
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
                    quest:SetTimer(quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_TeamReinforcementsTimer"), __native_entity_state:GetStateInt("TeamRespawnTime"))
                    iVar4 = quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount")
                    fVar8 = 15.0
                    thing1 = quest:GetHero()
                    bVar3 = IsDistanceBetweenThingsOver(thing1,me,fVar8)
                    if bVar3 then
                        pCVar5 = me:GetPos()
                        bVar3 = quest:IsCameraPosOnScreen(pCVar5)
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            iVar9 = 0
                            if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 then
                                repeat
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        return
                                    end
                                    bVar3 = false
                                    pCVar7 = (this + 0x34)
                                    pCVar5 = me:GetPos()
                                    r1 = quest:CreateCreature(nil --[[missing]], pCVar5, nil --[[missing]])
                                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                                    quest:Pause(2.0)
                                    iVar9 = iVar9 + 1
                                until not (iVar9 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
                            end
                            -- TODO(native): goto LAB_00dcd9ed
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    iVar9 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 then
                        repeat
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            bVar3 = false
                            pCVar7 = (this + 0x34)
                            pCVar5 = __native_entity_state:GetStateThing("OtherSpawnPoint"):GetPos()
                            r2 = quest:CreateCreature(nil --[[missing]], pCVar5, nil --[[missing]])
                            quest:EntityAttachToScript(r2, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            pCVar6 = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                            __native_entity_state:SetStateThing("OtherSpawnPoint", pCVar6)
                            iVar9 = iVar9 + 1
                        until not (iVar9 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
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
                if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * __native_entity_state:GetStateInt("TeamMemberLimit") + quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount"), -1)
                end
                if quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:Pause(*(DAT_0143e90c + 0xd74))
                    quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                    if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        pCVar6 = quest:GetHero()
                        r3 = quest:GetNearestWithScriptName(pCVar6, "GuardTeamMember")
                        iVar4 = (r3 ~= nil and r3:IsAlive())
                        if iVar4 then
                            fVar8 = 15.0
                            pCVar6 = quest:GetHero()
                            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, r3, fVar8)
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
                    if quest:GetStateInt("BanditWavesSpawned") == 3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                            return
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        -- TODO(native): *(undefined4 *)(*(int *)(this + 0x14) + 100) = 3;
                        return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    iVar4 = quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount")
                    iVar9 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= iVar4 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4 then
                        repeat
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_5c);
                            if (iVar9 == 1) and (quest:GetStateInt("HeroTeam") == 0) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    -- LAB_00dcda68: (native jump target)
                                    return
                                end
                                bVar3 = false
                                pCVar7 = (this + 0x34)
                                pCVar5 = me:GetPos()
                                pCVar6 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", pCVar5, "NEXT_WAVE")
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)&CStack_5c,(int)pCVar6);
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00dcda68
                                bVar3 = false
                                pCVar7 = (this + 0x34)
                                pCVar5 = me:GetPos()
                                pCVar6 = quest:CreateCreature("GuardTeamMember", pCVar5, nil --[[missing]])
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)&CStack_5c,(int)pCVar6);
                            end
                            quest:EntityAttachToScript(pCVar6, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            iVar9 = iVar9 + 1
                        until not (iVar9 < __native_entity_state:GetStateInt("TeamMemberLimit") - iVar4)
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
    local bVar7, iVar5, pCVar3, pcVar8, piVar4
    pCVar3 = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    __native_entity_state:SetStateThing("OtherSpawnPoint", pCVar3)
    if uStack_4 ~= nil then
        -- TODO(native): *uStack_4 = *uStack_4 + -1;
        if *uStack_4 == 0 then
        end
    end
    -- TODO(native): local_c._4_4_ = 0;
    -- TODO(native): GetName is not a ForgeFSE binding
    piVar4 = me:GetName()
    if *piVar4 == nil then
        iVar5 = 15
        bVar7 = false
        if bVar7 then
            -- LAB_00dcd306: (native jump target)
            __native_entity_state:SetStateInt("TeamID", 1)
            -- TODO(native): name field 0x34 (CCharString)
            quest:SetStateString("self_0x34", "BanditTeamMember")
            pcVar8 = "CREATURE_BANDIT_GRUNT"
            goto LAB_00dcd2bb
        end
    else
        -- TODO(native): iVar5 = CBasicString<char>::Compare(*(CBasicString<char> **)*piVar4);
        if iVar5 == 0 then return end  -- TODO(native): goto LAB_00dcd306
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    -- TODO(native): name field 0x34 (CCharString)
    quest:SetStateString("self_0x34", "GuardTeamMember")
    pcVar8 = "CREATURE_ORCHARD_FARM_GUARD"
    ::LAB_00dcd2bb::
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x38),pcVar8);
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        __native_entity_state:SetStateInt("TeamMemberLimit", 2)
        -- TODO(native): uVar1 = *(undefined4 *)(DAT_0143e90c + 0xd6c);
        __native_entity_state:SetStateInt("BanditsLeftID", 0)
        __native_entity_state:SetStateInt("TeamRespawnTime", uVar1)
        return
    end
    __native_entity_state:SetStateInt("TeamMemberLimit", 3)
    -- TODO(native): uVar1 = *(undefined4 *)(DAT_0143e90c + 0xd70);
    __native_entity_state:SetStateInt("BanditsLeftID", 0)
    __native_entity_state:SetStateInt("TeamRespawnTime", uVar1)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

