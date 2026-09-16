-- Generated native draft: GuardTeamMember. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar2, bVar3, bVar5, bVar6, cVar4, iVar7, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, native_arg_sequence_5, pCVar8, piVar1, r1
    local alive = true
    -- TODO(native): p0 = (CScriptThing *)(this + 8);
    bVar6 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        cVar4 = quest:GetStateBool("DoneIntroduction")
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            if quest:GetStateBool("HeroAtWrongEntrance") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                quest:RemoveThing(nil --[[missing]], p0, false)
                if 0 < quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetStateInt(nil --[[missing]], "Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount")
                end
                iVar7 = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 0x10
                quest:SetStateInt(nil --[[missing]], "Teams_0_StateCounter_" .. tostring(iVar7) .. "")
                return
            end
            cVar4 = quest:GetStateBool("DoneIntroduction")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsInCutscene()
                if bVar2 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        bVar2 = quest:IsInCutscene()
                    until not (bVar2)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_58;
                -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce357;
                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
            end
            bVar2 = false
            cVar4 = quest:GetStateBool("WhisperSpawned")
            while not cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                GoOnPatrol(quest, me)
                -- TODO(native): GetCurrentStateGroupType is not a ForgeFSE binding
                iVar7 = me:GetCurrentStateGroupType()
                __native_entity_state:SetStateInt("CurrentAIState", iVar7)
                if iVar7 ~= __native_entity_state:GetStateInt("PreviousAIState") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    if __native_entity_state:GetStateInt("CurrentAIState") == 2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if __native_entity_state:GetStateInt("MemberState") == 3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_58;
                            -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce40a;
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                            -- TODO(native): this_00 = auStack_58;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_54;
                            -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce4be;
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                            -- TODO(native): this_00 = auStack_54;
                        end
                    else
                        if __native_entity_state:GetStateInt("CurrentAIState") ~= 1 then goto LAB_00dce500 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_50;
                        -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce4f7;
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                        -- TODO(native): this_00 = auStack_50;
                    end
                end
                ::LAB_00dce500::
                __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
                if __native_entity_state:GetStateInt("MemberState") == 1 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsDead()
                    if (cVar4) and (quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_2") == 0) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        GetNearestCrateToMe(quest, me)
                        native_arg_sequence_1 = false
                        if auStack_38._4_4_ == nil then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if not native_arg_sequence_1 then
                            cVar4 = (**(*auStack_38._4_4_ + 300))()
                            if cVar4 == 0 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            -- LAB_00dce594: (native jump target)
                            bVar3 = false
                        else
                            -- TODO(native): in_stack_ffffff84 = (CScriptThing *)0xdce572;
                            -- TODO(native): CCharString::CCharString((CCharString *)(auStack_50 + 4),&DAT_0122d70e,-1);
                            bVar6 = true
                            native_arg_sequence_2 = false
                            if auStack_38._4_4_ ~= nil then
                                native_arg_sequence_2 = true
                            else
                                native_arg_sequence_2 = false
                            end
                            if native_arg_sequence_2 then
                                cVar4 = (**(*auStack_38._4_4_ + 0xfc))()
                                if cVar4 ~= 0 then
                                    native_arg_sequence_2 = true
                                else
                                    native_arg_sequence_2 = false
                                end
                            end
                            if native_arg_sequence_2 then return end  -- TODO(native): goto LAB_00dce594
                            bVar3 = true
                        end
                        if bVar6 then
                            bVar6 = false
                        end
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00dcebab: (native jump target)
                                return
                            end
                            bVar3 = quest:IsDistanceBetweenThingsUnder(nil --[[missing]], nil --[[missing]], p0)
                            if (quest:GetStateInt("HeroTeam") == 0) and (__native_entity_state:GetStateInt("TeamID") == 1) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00dcebab
                                bVar3 = true
                            end
                            bVar5 = IsDistanceBetweenThingsOver(auStack_38, quest:GetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_CrateDropPos"),5.0)
                            if (bVar5) and (bVar3 ~= false) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                if __native_entity_state:GetStateInt("TeamID") == 1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                    quest:EntityStopFollowing(nil --[[missing]])
                                    -- TODO(native): in_stack_ffffff74 = (CScriptThing *)0xdce662;
                                    quest:SetCombatNearbyBreakOffRange(nil --[[missing]], bVar3)
                                    -- TODO(native): in_stack_ffffff64 = (CScriptThing *)0xdce67d;
                                    quest:SetStealStealableItems(nil --[[missing]], bVar6)
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                    quest:SetCombatNearbyBreakOffRange(nil --[[missing]], nil --[[missing]])
                                    -- TODO(native): in_stack_ffffff7c = (CScriptThing *)0x0;
                                    -- TODO(native): in_stack_ffffff6c = (CScriptThing *)0xdce6ce;
                                    quest:SetStealStealableItems(nil --[[missing]], nil --[[missing]])
                                end
                                quest:SetRecoverStealableItems(nil --[[missing]], nil --[[missing]])
                                bVar2 = true
                                helper_DCEC50(quest, me, 2)
                                -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_48;
                                -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce716;
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                            elseif bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                -- TODO(native): in_stack_ffffff7c = (CScriptThing *)0xdce74c;
                                quest:ResetCombatNearbyBreakOffRange(nil --[[missing]])
                                -- TODO(native): in_stack_ffffff80 = (undefined **)0x0;
                                -- TODO(native): in_stack_ffffff6c = (CScriptThing *)0xdce766;
                                quest:SetStealStealableItems(nil --[[missing]], nil --[[missing]])
                                -- TODO(native): in_stack_ffffff70 = (CScriptThing *)0x0;
                                -- TODO(native): in_stack_ffffff5c = (CScriptThing *)0xdce780;
                                quest:SetRecoverStealableItems(nil --[[missing]], nil --[[missing]])
                                helper_DCEC50(quest, me, 0)
                                bVar2 = false
                            end
                        end
                    end
                    native_arg_sequence_3 = false
                    if __native_entity_state:GetStateInt("MemberState") == 1 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                    if native_arg_sequence_3 then
                        cVar4 = quest:GetStateThing(__key("Teams_":IsAlive()
                        if cVar4 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                    end
                    if native_arg_sequence_3 then
                        cVar4 = (**(**(__native_entity_state:GetStateInt("self_0x2c") + 0x34) + 0x138))()
                        if cVar4 == 0 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                    end
                    if native_arg_sequence_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        helper_DCEC50(quest, me, 3)
                        quest:EntityStopFollowing(nil --[[missing]])
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        -- TODO(native): in_stack_ffffff7c = p0;
                        quest:EntityFollowThing(nil --[[missing]], nil --[[missing]], bVar2, nil --[[missing]])
                        -- TODO(native): in_stack_ffffff84 = (CScriptThing *)&piStack_44;
                        -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce84e;
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                    end
                end
                __native_condition_1 = __native_entity_state:GetStateInt("MemberState") == 3
                if __native_condition_1 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsDead()
                    __native_condition_1 = cVar4 or (bVar3 = IsThingCarryingCrate(this,quest:GetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_TeamCrateCarrier")), not bVar3)
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:EntityStopFollowing(nil --[[missing]])
                    helper_DCEC50(quest, me, 0)
                end
                __native_condition_2 = __native_entity_state:GetStateInt("MemberState") == 1
                if __native_condition_2 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsAlive()
                    __native_condition_2 = cVar4
                end
                if __native_condition_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    helper_DCEC50(quest, me, 4)
                    quest:EntityStopFollowing(nil --[[missing]])
                    -- TODO(native): in_stack_ffffff7c = p0;
                    quest:EntityFollowThing(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_40;
                    -- TODO(native): in_stack_ffffff80 = (undefined **)0xdce924;
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, p0, 0)
                end
                __native_condition_3 = __native_entity_state:GetStateInt("MemberState") == 4
                if __native_condition_3 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsDead()
                    __native_condition_3 = cVar4 or (bVar3 = IsThingCarryingCrate(this,quest:GetStateThing("Teams_" .. tostring(quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_EnemyTeam")) .. "_TeamCrateCarrier") ), not bVar3)
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    helper_DCEC50(quest, me, 0)
                end
                native_arg_sequence_4 = false
                if __native_entity_state:GetStateInt("MemberState") == 2 then
                    native_arg_sequence_4 = true
                else
                    native_arg_sequence_4 = false
                end
                if native_arg_sequence_4 then
                    bVar3 = IsThingCarryingCrate(quest, me, p0)
                    if bVar3 then
                        native_arg_sequence_4 = true
                    else
                        native_arg_sequence_4 = false
                    end
                end
                if native_arg_sequence_4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    helper_DCEC50(quest, me, 5)
                    -- TODO(native): CScriptThing::operator=(quest:GetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_TeamCrateCarrier"),(CScriptThing *)p0);
                    -- TODO(native): CCharString::CCharString((CCharString *)(auStack_40 + 4),"REQUEST_PROTECTION",-1);
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, ("FETCHING" + 4), p0, 0)
                    -- TODO(native): in_stack_ffffff84 = *(CScriptThing **)(this + 0xc);
                    piVar1 = *(this + 0x10)
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                    -- TODO(native): in_stack_ffffff7c = (CScriptThing *)0xdcea0a;
                    quest:SetCombatNearbyBreakOffRange(nil --[[missing]], nil --[[missing]])
                end
                native_arg_sequence_5 = false
                if __native_entity_state:GetStateInt("MemberState") == 5 then
                    native_arg_sequence_5 = true
                else
                    native_arg_sequence_5 = false
                end
                if native_arg_sequence_5 then
                    bVar3 = IsThingCarryingCrate(quest, me, p0)
                    if not bVar3 then
                        native_arg_sequence_5 = true
                    else
                        native_arg_sequence_5 = false
                    end
                end
                if native_arg_sequence_5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    -- TODO(native): CScriptThing::operator= (quest:GetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_TeamCrateCarrier"),(CScriptThing *)&local_20);
                    helper_DCEC50(quest, me, 0)
                    -- TODO(native): in_stack_ffffff84 = *(CScriptThing **)(this + 0xc);
                    piVar1 = *(this + 0x10)
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                    -- TODO(native): in_stack_ffffff7c = (CScriptThing *)0xdceaa0;
                    quest:SetCombatNearbyBreakOffRange(nil --[[missing]], 0)
                end
                cVar4 = quest:GetStateBool("WhisperSpawned")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:EntityStopFollowing(nil --[[missing]])
                r1 = quest:GetNearestWithScriptName(nil --[[missing]], "TeamExitMarker")
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&local_20);
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&local_20);
                if bVar6 then
                end
                -- TODO(native): pppuVar9 = &local_20;
                -- TODO(native): p0_00 = p0;
                cVar4 = me:AcquireControl(4)
                while not cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00dcec33 end
                    cVar4 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    while not bVar6 do
                        bVar6 = me:IsPerformingScriptTask()
                        if not bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then break end
                            if "DEFENDING" == nil then
                            else
                                pCVar8 = (**(*"DEFENDING" + 0x18))()
                            end
                            me:MoveToPosition(nil --[[missing]], 0x1, 0x0, true, SUB41(p0_00,0))
                        end
                        bVar6 = quest:IsDistanceBetweenThingsUnder(r1, nil --[[missing]], p0)
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                quest:FadeOutAndKillEntity(nil --[[missing]], p0, true, 3.0)
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                until not (not bVar6)
                            end
                            break
                        end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                    end
                end
                ::LAB_00dcec33::
            end
        end
    end
end

function Init(quest, me)
    local bVar7, iVar4, pCVar2, piVar1
    -- TODO(native): pThing = (CScriptThing *)(this + 8);
    -- TODO(native): GetName is not a ForgeFSE binding
    piVar1 = me:GetName()
    if *piVar1 == nil then
        iVar4 = 16
        bVar7 = false
        if bVar7 then
            -- LAB_00dce00a: (native jump target)
            __native_entity_state:SetStateInt("TeamID", 1)
            goto LAB_00dcdf96
        end
    else
        -- TODO(native): lVar3 = CBasicString<char>::Compare(*(char **)*piVar1,"BanditTeamMember");
        -- TODO(native): cStack_11 = '\x01' - (lVar3 != 0);
        if cStack_11 ~= 0 then return end  -- TODO(native): goto LAB_00dce00a
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    ::LAB_00dcdf96::
    if __native_entity_state:GetStateInt("TeamID") == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(nil --[[missing]], "FACTION_BANDITS_FRIENDLY")
            pCVar2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(pCVar2, nil --[[missing]])
        else
            quest:EntitySetInFaction(nil --[[missing]], "FACTION_BANDITS")
            quest:MiniMapAddMarker(nil --[[missing]], "HUD_ORB_RED_SMALL")
            quest:SetIsThingTurncoatable(nil --[[missing]], pThing)
            quest:EntitySetAsDisplayingEmoteIcon(nil --[[missing]], pThing)
        end
        -- TODO(native): SetDataString is not a ForgeFSE binding
        me:SetDataString("BANDIT")
    else
        if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
            quest:EntitySetInFaction(nil --[[missing]], "FACTION_VILLAGERS")
            pCVar2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(pCVar2, nil --[[missing]])
        else
            quest:EntitySetInFaction(nil --[[missing]], "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(nil --[[missing]], "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(nil --[[missing]], pThing)
        end
        -- TODO(native): SetDataString is not a ForgeFSE binding
        me:SetDataString("GUARD")
    end
    -- TODO(native): __native_entity_state:SetStateInt("MyTeam", *(int *)(this + 0x24));
    piVar1 = (iVar4 + 0x18)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    __native_entity_state:SetStateInt("CurrentAIState", 2)
    __native_entity_state:SetStateInt("PreviousAIState", 2)
    __native_entity_state:SetStateInt("MemberState", 0)
    quest:SetStateInt(nil --[[missing]], "Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_0")
    -- TODO(native): __native_entity_state:SetStateThing("ThingToPatrolTo", nil);
    if nil ~= nil then
        -- TODO(native): *piStack_8 = *piStack_8 + -1;
        if *0x0 == 0 then
        end
    end
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local piVar1 = (__native_entity_state:GetStateInt("TeamID") * 0x40 + 0xa4 + __native_entity_state:GetStateInt("self_0x14"))
    -- TODO(native): *piVar1 = *piVar1 + -1;
    local iVar3 = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 0x10
    quest:SetStateInt(nil --[[missing]], "Teams_0_StateCounter_" .. tostring(iVar3) .. "")
    -- TODO(native): pvStack_4 = this;
    -- TODO(native): CCharString::CCharString((CCharString *)&pvStack_4,&DAT_0122d70e,-1);
    local cVar2 = me:MsgIsKilledBy(nil --[[missing]])
    if cVar2 then
        if __native_entity_state:GetStateInt("TeamID") == 1 then
            quest:SetMasterGameState("OrchardFarmBanditKilled", true)
            return
        end
        quest:SetMasterGameState("OrchardFarmGuardKilled", true)
    end
end

function GoOnPatrol(quest, me)
    local bVar3, uVar4
    local alive = true
    if __native_entity_state:GetStateInt("MemberState") ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        return
    end
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        uVar4 = quest:GetHero()
        quest:EntityFollowThing(me, uVar4, nil --[[missing]], nil --[[missing]])
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        uVar4 = quest:GetHero()
        quest:EntityFollowThing(me, uVar4, nil --[[missing]], nil --[[missing]])
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", 1)
    quest:SetStateInt(nil --[[missing]], "Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_1")
end

function IsThingCarryingCrate(quest, me, native_arg_param_1)
    local bVar1, cVar2, iVar5, uVar3, uVar6
    local alive = true
    local iVar4 = (quest:GetStateListCount("CrateList") * 0xc)
    iVar5 = iVar4 >> 0x1f
    uVar6 = 0
    if iVar4 / 0xc + iVar5 ~= iVar5 then
        iVar5 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return false
            end
            cVar2 = (**(*(*QUESTLIST_Begin("CrateList") + iVar5) + 300))()
            if cVar2 ~= 0 then
                iVar4 = *(*QUESTLIST_Begin("CrateList") + iVar5)
                uVar3 = (**(*native_arg_param_1 + 4))()
                cVar2 = quest:IsHeroAllowedHenchmenInRegion(nil --[[missing]])
                if cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    return ('\x01' - bVar1)
                end
            end
            uVar6 = uVar6 + 1
            iVar5 = iVar5 + 0xc
        until not (uVar6 < quest:GetStateListCount("CrateList"))
    end
    alive = not quest:IsActiveThreadTerminating()
    return false
end

function GetNearestCrateToMe(quest, me, native_arg_param_2)
    local fVar5, iVar4, uVar1
    -- TODO(native): local_18 = 1e+07;
    local piVar2 = QUESTLIST_Begin("CrateList")
    local iVar3 = QUESTLIST_End("CrateList") - *piVar2
    iVar4 = iVar3 >> 0x1f
    if iVar3 / 0xc + iVar4 ~= iVar4 then
        iVar4 = 0
        repeat
            fVar5 = GetSquaredDistanceBetweenThings((me),(*piVar2 + iVar4))
            if fVar5 < local_18 then
                iVar3 = *QUESTLIST_Begin("CrateList")
                piVar2 = *(iVar3 + 8 + iVar4)
                uVar1 = *(iVar3 + 4 + iVar4)
                if 0x0 ~= piVar2 then
                    if piVar2 ~= nil then
                        -- TODO(native): *piVar2 = *piVar2 + 1;
                    end
                end
            end
            piVar2 = QUESTLIST_Begin("CrateList")
            -- TODO(native): local_14 = local_14 + 1;
            iVar4 = iVar4 + 0xc
        until not (local_14 < ((QUESTLIST_End("CrateList") - *piVar2) / 0xc))
    end
    -- TODO(native): native_arg_param_2[1] = local_8;
    -- TODO(native): native_arg_param_2[2] = local_4;
    if piVar2 ~= nil then
        -- TODO(native): *local_4 = *local_4 + 1;
    end
    return native_arg_param_2
end

function helper_DCEC50(quest, me, native_arg_param_1)
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", native_arg_param_1)
    piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + native_arg_param_1 * 4)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    return piVar1
end

