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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar2, bVar3, bVar5, bVar6, cVar4, iVar7, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, native_arg_sequence_5, p0, p0_00, p0_01, piVar1, r1
    local alive = true
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
                quest:RemoveThing(me, false, false)
                if 0 < quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount", quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("TeamID")) .. "_MemberCount") + -1)
                end
                iVar7 = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 0x10
                quest:SetStateInt("Teams_0_StateCounter_" .. tostring(iVar7) .. "", quest:GetStateInt("Teams_0_StateCounter_" .. tostring(iVar7) .. "") + -1)
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
                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
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
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
                            -- TODO(native): this_00 = auStack_58;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            -- TODO(native): in_stack_ffffff84 = (CScriptThing *)auStack_54;
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
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
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
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
                            bVar3 = quest:IsDistanceBetweenThingsUnder(me, nil --[[missing]], auStack_38)
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
                                    quest:SetCombatNearbyBreakOffRange(nil --[[missing]], bVar3)
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
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
                            elseif bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                -- TODO(native): in_stack_ffffff7c = (CScriptThing *)0xdce74c;
                                quest:ResetCombatNearbyBreakOffRange(nil --[[missing]])
                                -- TODO(native): in_stack_ffffff6c = (CScriptThing *)0xdce766;
                                quest:SetStealStealableItems(nil --[[missing]], nil --[[missing]])
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
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
                    end
                end
                __native_condition_1 = __native_entity_state:GetStateInt("MemberState") == 3
                if __native_condition_1 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsDead()
                    __native_condition_1 = cVar4 or (iVar7 = IsThingCarryingCrate(this,__native_entity_state:GetStateInt("self_0x2c") + 0x30), iVar7 == 0 )
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
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, in_stack_ffffff84, me, 0)
                end
                __native_condition_3 = __native_entity_state:GetStateInt("MemberState") == 4
                if __native_condition_3 then
                    cVar4 = quest:GetStateThing(__key("Teams_":IsDead()
                    __native_condition_3 = cVar4 or (iVar7 = IsThingCarryingCrate(this,*(__native_entity_state:GetStateInt("self_0x2c") + 0x3c) + 0x30), iVar7 == 0)
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
                    iVar7 = IsThingCarryingCrate(quest, me, me)
                    if iVar7 ~= 0 then
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
                    quest:SetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_TeamCrateCarrier", me)
                    -- TODO(native): CCharString::CCharString((CCharString *)(auStack_40 + 4),"REQUEST_PROTECTION",-1);
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, ("FETCHING" + 4), me, 0)
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
                    iVar7 = IsThingCarryingCrate(quest, me, me)
                    if iVar7 == 0 then
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
                    quest:SetStateThing("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_TeamCrateCarrier", &local_20)
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
                        iVar7 = me:IsPerformingScriptTask()
                        if not iVar7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then break end
                            if "DEFENDING" == nil then
                            else
                                p0_00 = (**(*"DEFENDING" + 0x18))()
                            end
                            me:MoveToPosition(nil --[[missing]], 0x3f000000, 1, false, true)
                        end
                        bVar6 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                quest:FadeOutAndKillEntity(me, true, 3.0, true)
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
    local bVar6, iVar3, pCVar2, pThing, piVar1
    -- TODO(native): GetName is not a ForgeFSE binding
    piVar1 = me:GetName()
    if *piVar1 == nil then
        iVar3 = 16
        bVar6 = false
        if bVar6 then
            -- LAB_00dce00a: (native jump target)
            __native_entity_state:SetStateInt("TeamID", 1)
            goto LAB_00dcdf96
        end
    else
        -- TODO(native): iVar3 = CBasicString<char>::Compare(*(CBasicString<char> **)*piVar1);
        -- TODO(native): cStack_11 = '\x01' - (iVar3 != 0);
        if cStack_11 ~= 0 then return end  -- TODO(native): goto LAB_00dce00a
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    ::LAB_00dcdf96::
    if __native_entity_state:GetStateInt("TeamID") == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
            pCVar2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, pCVar2)
        else
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:SetIsThingTurncoatable(me, false)
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        -- TODO(native): SetDataString is not a ForgeFSE binding
        me:SetDataString("BANDIT")
    else
        if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
            quest:EntitySetInFaction(me, "FACTION_VILLAGERS")
            pCVar2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, pCVar2)
        else
            quest:EntitySetInFaction(me, "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        -- TODO(native): SetDataString is not a ForgeFSE binding
        me:SetDataString("GUARD")
    end
    __native_entity_state:SetStateInt("MyTeam", __native_entity_state:GetStateInt("TeamID"))
    piVar1 = (iVar3 + 0x18)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    __native_entity_state:SetStateInt("CurrentAIState", 2)
    __native_entity_state:SetStateInt("PreviousAIState", 2)
    __native_entity_state:SetStateInt("MemberState", 0)
    quest:SetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_0", quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_0") + 1)
    __native_entity_state:SetStateThing("ThingToPatrolTo", nil)
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
    quest:SetStateInt("Teams_0_StateCounter_" .. tostring(iVar3) .. "", quest:GetStateInt("Teams_0_StateCounter_" .. tostring(iVar3) .. "") + -1)
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
        bVar3 = not alive
        return CONCAT31(extraout_var,bVar3)
    end
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return CONCAT31(extraout_var_00,bVar3)
        end
        uVar4 = quest:GetHero()
        quest:EntityFollowThing(me, uVar4, nil --[[missing]], nil --[[missing]])
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return CONCAT31(extraout_var_01,bVar3)
        end
        uVar4 = quest:GetHero()
        quest:EntityFollowThing(me, uVar4, nil --[[missing]], nil --[[missing]])
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", 1)
    quest:SetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_1", quest:GetStateInt("Teams_" .. tostring(__native_entity_state:GetStateInt("MyTeam")) .. "_StateCounter_1") + 1)
    return piVar1
end

function IsThingCarryingCrate(quest, me, native_arg_thing)
    local bVar1, cVar2, iVar5, iVar6, uVar3, uVar4, uVar7
    local alive = true
    iVar5 = (quest:GetStateListCount("CrateList") * 0xc)
    iVar6 = iVar5 >> 0x1f
    uVar7 = 0
    if iVar5 / 0xc + iVar6 ~= iVar6 then
        iVar6 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            uVar4 = extraout_var
            if bVar1 then goto LAB_00dcedc1 end
            cVar2 = (**(*(*QUESTLIST_Begin("CrateList") + iVar6) + 300))()
            if cVar2 ~= 0 then
                iVar5 = *(*QUESTLIST_Begin("CrateList") + iVar6)
                uVar3 = (**(*native_arg_thing + 4))()
                cVar2 = quest:IsHeroAllowedHenchmenInRegion(nil --[[missing]])
                if cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    return CONCAT31(extraout_var_01,'\x01' - bVar1)
                end
            end
            uVar7 = uVar7 + 1
            iVar6 = iVar6 + 0xc
        until not (uVar7 < quest:GetStateListCount("CrateList"))
    end
    alive = not quest:IsActiveThreadTerminating()
    uVar4 = extraout_var_00
    ::LAB_00dcedc1::
    return uVar4 << 8
end

function GetNearestCrateToMe(quest, me)
    local iVar4, piVar1, uVar2
    -- TODO(native): local_18 = 1e+07;
    local iVar3 = (quest:GetStateListCount("CrateList") * 0xc)
    iVar4 = iVar3 >> 0x1f
    if iVar3 / 0xc + iVar4 ~= iVar4 then
        iVar4 = 0
        repeat
            -- TODO(native): GetSquaredDistanceBetweenThings((void *)(this + 8));
            if extraout_ST0 < local_18 then
                -- TODO(native): local_18 = (float)extraout_ST0;
                iVar3 = *QUESTLIST_Begin("CrateList")
                piVar1 = *(iVar3 + 8 + iVar4)
                uVar2 = *(iVar3 + 4 + iVar4)
                if 0x0 ~= piVar1 then
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
            end
            -- TODO(native): local_14 = local_14 + 1;
            iVar4 = iVar4 + 0xc
        until not (local_14 < quest:GetStateListCount("CrateList"))
    end
    -- TODO(native): in_stack_00000004[1] = local_8;
    -- TODO(native): in_stack_00000004[2] = local_4;
    if piVar1 ~= nil then
        -- TODO(native): *local_4 = *local_4 + 1;
    end
    return in_stack_00000004
end

function helper_DCEC50(quest, me, native_arg_param_1)
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", native_arg_param_1)
    piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + native_arg_param_1 * 4)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    return piVar1
end

