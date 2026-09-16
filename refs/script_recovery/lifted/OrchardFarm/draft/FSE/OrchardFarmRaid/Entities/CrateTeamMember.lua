-- Generated native draft: CrateTeamMember. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, aCStack_10, bVar2, bVar3, bVar5, bVar6, cVar4, fVar9, iVar7, local_20, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, p0, p0_00, piVar1, r1, thing_38
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
                if 0 < quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) + -1)
                end
                iVar7 = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 0x10
                quest:SetStateInt(("Teams_0_StateCounter_" .. iVar7), quest:GetStateInt(("Teams_0_StateCounter_" .. iVar7)) + -1)
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
                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
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
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                            -- TODO(native): this_00 = auStack_58;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FOLLOWING", me, 0)
                            -- TODO(native): this_00 = auStack_54;
                        end
                    else
                        if __native_entity_state:GetStateInt("CurrentAIState") ~= 1 then goto LAB_00dce500 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "ATTACKING", me, 0)
                        -- TODO(native): this_00 = auStack_50;
                    end
                end
                ::LAB_00dce500::
                __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
                if __native_entity_state:GetStateInt("MemberState") == 1 then
                    cVar4 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsDead()
                    if (cVar4) and (quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_2")) == 0) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        thing_38 = GetNearestCrateToMe(quest, me)
                        __native_condition_1 = not IsThingValid(thing_38)
                        if not __native_condition_1 then
                            cVar4 = (thing_38 ~= nil and thing_38:IsAlive())
                            __native_condition_1 = not cVar4
                        end
                        if __native_condition_1 then
                            -- LAB_00dce594: (native jump target)
                            bVar3 = false
                        else
                            -- TODO(native): CCharString::CCharString((CCharString *)(auStack_50 + 4),"",-1);
                            bVar6 = true
                            native_arg_sequence_1 = false
                            if IsThingValid(thing_38) then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                            if native_arg_sequence_1 then
                                -- TODO(native): IsBeingCarriedBy is not a ForgeFSE binding
                                cVar4 = thing_38:IsBeingCarriedBy("ATTACKING")
                                if cVar4 then
                                    native_arg_sequence_1 = true
                                else
                                    native_arg_sequence_1 = false
                                end
                            end
                            if native_arg_sequence_1 then return end  -- TODO(native): goto LAB_00dce594
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
                            bVar3 = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
                            if (quest:GetStateInt("HeroTeam") == 0) and (__native_entity_state:GetStateInt("TeamID") == 1) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00dcebab
                                bVar3 = true
                            end
                            bVar5 = IsDistanceBetweenThingsOver(thing_38, quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_CrateDropPos")),5.0)
                            if (bVar5) and (bVar3 ~= false) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                if __native_entity_state:GetStateInt("TeamID") == 1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                    quest:EntityStopFollowing(me)
                                    fVar9 = 4.0
                                    quest:SetCombatNearbyBreakOffRange(me, fVar9)
                                    bVar2 = true
                                    quest:SetStealStealableItems(me, bVar2)
                                    bVar2 = false
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                    fVar9 = 4.0
                                    quest:SetCombatNearbyBreakOffRange(me, fVar9)
                                    bVar2 = false
                                    quest:SetStealStealableItems(me, bVar2)
                                    bVar2 = true
                                end
                                quest:SetRecoverStealableItems(me, bVar2)
                                bVar2 = true
                                helper_DCEC50(quest, me, 2)
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                            elseif bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then return end  -- TODO(native): goto LAB_00dcebab
                                -- TODO(native): pThing_06._4_4_ = uVar8;
                                -- TODO(native): pThing_06._0_4_ = auStack_48;
                                -- TODO(native): pThing_06._8_4_ = uVar10;
                                quest:ResetCombatNearbyBreakOffRange(nil --[[missing]])
                                bVar2 = false
                                quest:SetStealStealableItems(me, bVar2)
                                bVar2 = false
                                quest:SetRecoverStealableItems(me, bVar2)
                                helper_DCEC50(quest, me, 0)
                                bVar2 = false
                            end
                        end
                    end
                    native_arg_sequence_2 = false
                    if __native_entity_state:GetStateInt("MemberState") == 1 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                    if native_arg_sequence_2 then
                        cVar4 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsAlive()
                        if cVar4 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        cVar4 = (**(**(__native_entity_state:GetStateInt("self_0x2c") + 0x34) + 0x138))()
                        if cVar4 == 0 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        helper_DCEC50(quest, me, 3)
                        quest:EntityStopFollowing(me)
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        quest:EntityFollowThing(me, nil --[[missing]], quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")), 1.0)
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                    end
                end
                __native_condition_2 = __native_entity_state:GetStateInt("MemberState") == 3
                if __native_condition_2 then
                    cVar4 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsDead()
                    __native_condition_2 = cVar4 or (iVar7 = IsThingCarryingCrate(this,__native_entity_state:GetStateInt("self_0x2c") + 0x30), iVar7 == 0 )
                end
                if __native_condition_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:EntityStopFollowing(me)
                    helper_DCEC50(quest, me, 0)
                end
                __native_condition_3 = __native_entity_state:GetStateInt("MemberState") == 1
                if __native_condition_3 then
                    cVar4 = quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")):IsAlive()
                    __native_condition_3 = cVar4
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    helper_DCEC50(quest, me, 4)
                    quest:EntityStopFollowing(me)
                    quest:EntityFollowThing(me, nil --[[missing]], quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")), 1.0)
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                end
                __native_condition_4 = __native_entity_state:GetStateInt("MemberState") == 4
                if __native_condition_4 then
                    cVar4 = quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")):IsDead()
                    __native_condition_4 = cVar4 or (iVar7 = IsThingCarryingCrate(this,*(__native_entity_state:GetStateInt("self_0x2c") + 0x3c) + 0x30), iVar7 == 0)
                end
                if __native_condition_4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    helper_DCEC50(quest, me, 0)
                end
                native_arg_sequence_3 = false
                if __native_entity_state:GetStateInt("MemberState") == 2 then
                    native_arg_sequence_3 = true
                else
                    native_arg_sequence_3 = false
                end
                if native_arg_sequence_3 then
                    iVar7 = IsThingCarryingCrate(quest, me, me)
                    if iVar7 ~= 0 then
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
                    helper_DCEC50(quest, me, 5)
                    quest:SetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"), me)
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "REQUEST_PROTECTION", me, 0)
                    piVar1 = *(this + 0x10)
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                native_arg_sequence_4 = false
                if __native_entity_state:GetStateInt("MemberState") == 5 then
                    native_arg_sequence_4 = true
                else
                    native_arg_sequence_4 = false
                end
                if native_arg_sequence_4 then
                    iVar7 = IsThingCarryingCrate(quest, me, me)
                    if iVar7 == 0 then
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
                    local_20 = nil
                    quest:SetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"), local_20)
                    helper_DCEC50(quest, me, 0)
                    piVar1 = *(this + 0x10)
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                cVar4 = quest:GetStateBool("WhisperSpawned")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:EntityStopFollowing(me)
                -- TODO(native): CCharString::CCharString("REQUEST_PROTECTION","TeamExitMarker",-1);
                r1 = quest:GetNearestWithScriptName(me, "REQUEST_PROTECTION")
                aCStack_10 = resources:NewResource()
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_10);
                if bVar6 then
                end
                bVar6 = resources:TryAcquire(aCStack_10, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00dcec33 end
                    bVar6 = resources:TryAcquire(aCStack_10, me, 4)
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
                            if not IsThingValid(r1) then
                            else
                                p0_00 = r1:GetPos()
                            end
                            me:MoveToPosition(p0_00, 0x3f000000, 1, false, true)
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
                resources:ReleaseResource(aCStack_10)
            end
        end
    end
end

function Init(quest, me)
    local bVar6, c_stk_11, iVar3, pCVar2, pThing, piVar1, u_stk_c
    -- TODO(native): GetName is not a ForgeFSE binding
    piVar1 = me:GetName()
    if piVar1 == nil then
        iVar3 = 16
        bVar6 = false
        if bVar6 then
            -- LAB_00dce00a: (native jump target)
            __native_entity_state:SetStateInt("TeamID", 1)
            goto LAB_00dcdf96
        end
    else
        iVar3 = ((piVar1 == "BanditTeamMember") and 0 or 1)
        c_stk_11 = '\x01' - (iVar3 ~= 0)
        if c_stk_11 ~= 0 then return end  -- TODO(native): goto LAB_00dce00a
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
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0")) + 1)
    u_stk_c = 0
    __native_entity_state:SetStateThing("ThingToPatrolTo", nil)
    if nil ~= nil then
        -- TODO(native): *piStack_8 = *piStack_8 + -1;
        if *0x0 == 0 then
        end
    end
    u_stk_c = 0
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local piVar1 = (__native_entity_state:GetStateInt("TeamID") * 0x40 + 0xa4 + __native_entity_state:GetStateInt("self_0x14"))
    -- TODO(native): *piVar1 = *piVar1 + -1;
    local iVar3 = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 0x10
    quest:SetStateInt(("Teams_0_StateCounter_" .. iVar3), quest:GetStateInt(("Teams_0_StateCounter_" .. iVar3)) + -1)
    -- TODO(native): pvStack_4 = this;
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        if __native_entity_state:GetStateInt("TeamID") == 1 then
            quest:SetMasterGameState("OrchardFarmBanditKilled", true)
            return
        end
        quest:SetMasterGameState("OrchardFarmGuardKilled", true)
    end
end

function GoOnPatrol(quest, me)
    local bVar3, fVar5, pCVar4
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
        bVar3 = true
        fVar5 = 3.0
        pCVar4 = quest:GetHero()
        quest:EntityFollowThing(me, pCVar4, fVar5, bVar3)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return CONCAT31(extraout_var_01,bVar3)
        end
        bVar3 = true
        fVar5 = 1.0
        pCVar4 = quest:GetHero()
        quest:EntityFollowThing(me, pCVar4, fVar5, bVar3)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", 1)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1")) + 1)
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
            cVar2 = quest:GetStateListAt("CrateList", (iVar6) / 0xc):IsAlive()
            if cVar2 then
                -- TODO(native): GetName is not a ForgeFSE binding
                uVar3 = native_arg_thing:GetName()
                -- TODO(native): IsBeingCarriedBy is not a ForgeFSE binding
                cVar2 = quest:GetStateListAt("CrateList", (iVar6) / 0xc):IsBeingCarriedBy(uVar3)
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
    local fVar5, iVar4, v_stk_14, v_stk_18
    v_stk_18 = 10000000.0
    local local_c = nil
    local iVar3 = (quest:GetStateListCount("CrateList") * 0xc)
    iVar4 = iVar3 >> 0x1f
    v_stk_14 = 0
    if iVar3 / 0xc + iVar4 ~= iVar4 then
        iVar4 = 0
        repeat
            fVar5 = (quest:GetDistanceBetweenThings(me, (quest:GetStateListAt("CrateList", (iVar4) / 0xc))) ^ 2)
            if fVar5 < v_stk_18 then
                iVar3 = quest:GetStateListRef("CrateList")
                v_stk_18 = fVar5
                local_c = quest:GetStateListAt("CrateList", (iVar4) / 0xc)
            end
            v_stk_14 = v_stk_14 + 1
            iVar4 = iVar4 + 0xc
        until not (v_stk_14 < (((quest:GetStateListCount("CrateList") * 0xc)) / 0xc))
    end
    local ret_thing = local_c
    local_c = nil
    return ret_thing
end

function helper_DCEC50(quest, me, native_arg_param_1)
    local piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + __native_entity_state:GetStateInt("MemberState") * 4)
    -- TODO(native): *piVar1 = *piVar1 + -1;
    __native_entity_state:SetStateInt("MemberState", native_arg_param_1)
    piVar1 = (__native_entity_state:GetStateInt("self_0x2c") + native_arg_param_1 * 4)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    return piVar1
end

