-- Readable native conversion: CrateTeamMember. Review coverage report before use.
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
    local predicateResult, scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5
    local scratchValue6, scratchValue7, scratchValue8, predicateResult2, predicateResult3
    local isInCutscene, isInCutscene2, predicateResult4, predicateResult5, predicateResult6
    local predicateResult7, scratchValue9, getStateThing, isDistanceBetweenThingsOver
    local predicateResult8, predicateResult9, controlAcquired, predicateResult10, predicateResult11
    local isDistanceBetweenThingsUnder, predicateResult12, predicateResult13, doneIntroduction
    local whisperSpawned, getStateThing2, scratchValue10, isBeingCarriedBy, getStateThing3
    local getStateThing4, getStateThing5, getStateThing6, getStateInt, getCurrentStateGroupType
    local scratchValue11, scratchValue12, scratchValue13, scratchValue14, taskRunning, sequence12
    local sequence22, sequence32, p0_00, getNearestWithScriptName, thing_38
    local alive = true
    predicateResult8 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    predicateResult2 = not alive
    if not predicateResult2 then
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
        while not doneIntroduction do
            alive = quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                return
            end
            if quest:GetStateBool("HeroAtWrongEntrance") then
                if quest:IsActiveThreadTerminating() then
                    return
                end
                quest:RemoveThing(me, false, false)
                if 0 < quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) + -1)
                end
                getStateInt = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 16
                quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"))) + -1)
                return
            end
            doneIntroduction = quest:GetStateBool("DoneIntroduction")
        end
        alive = not quest:IsActiveThreadTerminating()
        predicateResult3 = not alive
        if not predicateResult3 then
            if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
                if quest:IsActiveThreadTerminating() then
                    return
                end
                isInCutscene = quest:IsInCutscene()
                if isInCutscene then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        isInCutscene2 = quest:IsInCutscene()
                    until not (isInCutscene2)
                end
                if quest:IsActiveThreadTerminating() then
                    return
                end
                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
            end
            predicateResult4 = false
            whisperSpawned = quest:GetStateBool("WhisperSpawned")
            while not whisperSpawned do
                alive = quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    return
                end
                GoOnPatrol(quest, me)
                getCurrentStateGroupType = me:GetCurrentStateGroupType()
                __native_entity_state:SetStateInt("CurrentAIState", getCurrentStateGroupType)
                if getCurrentStateGroupType ~= __native_entity_state:GetStateInt("PreviousAIState") then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    if __native_entity_state:GetStateInt("CurrentAIState") == 2 then
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        if __native_entity_state:GetStateInt("MemberState") == 3 then
                            if quest:IsActiveThreadTerminating() then
                                return
                            end
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                            -- TODO(native): this_00 = auStack_58;
                        else
                            if quest:IsActiveThreadTerminating() then
                                return
                            end
                            require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FOLLOWING", me, 0)
                            -- TODO(native): this_00 = auStack_54;
                        end
                    else
                        if __native_entity_state:GetStateInt("CurrentAIState") ~= 1 then goto LAB_00dce500 end
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "ATTACKING", me, 0)
                        -- TODO(native): this_00 = auStack_50;
                    end
                end
                ::LAB_00dce500::
                __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
                if __native_entity_state:GetStateInt("MemberState") == 1 then
                    getStateThing2 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsDead()
                    if (getStateThing2) and (quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_2")) == 0) then
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        thing_38 = GetNearestCrateToMe(quest, me)
                        predicateResult = not (thing_38 ~= nil and not thing_38:IsNull())
                        if not predicateResult then
                            scratchValue10 = (thing_38 ~= nil and thing_38:IsAlive())
                            predicateResult = not scratchValue10
                        end
                        if predicateResult then
                            -- LAB_00dce594: (native jump target)
                            predicateResult6 = false
                        else
                            -- TODO(native): CCharString::CCharString((CCharString *)(auStack_50 + 4),"",-1);
                            predicateResult8 = true

                            if (thing_38 ~= nil and not thing_38:IsNull()) then
                                sequence12 = true
                            else
                                sequence12 = false
                            end
                            if sequence12 then
                                isBeingCarriedBy = thing_38:IsBeingCarriedBy("ATTACKING")
                                if isBeingCarriedBy then
                                    sequence12 = true
                                else
                                    sequence12 = false
                                end
                            end
                            if sequence12 then
                                predicateResult6 = false
                                goto FLOW_after_lab_00dce594
                            end
                            predicateResult6 = true
                        end
                        ::FLOW_after_lab_00dce594::
                        if predicateResult8 then
                            predicateResult8 = false
                        end
                        if predicateResult6 then
                            alive = not quest:IsActiveThreadTerminating()
                            predicateResult7 = not alive
                            if predicateResult7 then
                                -- LAB_00dcebab: (native jump target)
                                return
                            end
                            scratchValue9 = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
                            if (quest:GetStateInt("HeroTeam") == 0) and (__native_entity_state:GetStateInt("TeamID") == 1) then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end
                                scratchValue9 = true
                            end
                            isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(thing_38, (__native_entity_state:GetStateInt("self_0x2c") + 36), 5.0)
                            if (isDistanceBetweenThingsOver) and (scratchValue9 ~= false) then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end
                                if __native_entity_state:GetStateInt("TeamID") == 1 then
                                    if quest:IsActiveThreadTerminating() then
                                        return
                                    end
                                    quest:EntityStopFollowing(me)

                                    quest:SetCombatNearbyBreakOffRange(me, (4.0))

                                    quest:SetStealStealableItems(me, (true))
                                    predicateResult5 = false
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        return
                                    end

                                    quest:SetCombatNearbyBreakOffRange(me, (4.0))

                                    quest:SetStealStealableItems(me, (false))
                                    predicateResult5 = true
                                end
                                quest:SetRecoverStealableItems(me, predicateResult5)
                                predicateResult4 = true
                                helper_DCEC50(quest, me, 2)
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                            elseif predicateResult4 then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end
                                -- TODO(native): pThing_06._4_4_ = uVar8;
                                -- TODO(native): pThing_06._0_4_ = auStack_48;
                                -- TODO(native): pThing_06._8_4_ = uVar10;
                                quest:ResetCombatNearbyBreakOffRange(nil --[[missing]])

                                quest:SetStealStealableItems(me, (false))

                                quest:SetRecoverStealableItems(me, (false))
                                helper_DCEC50(quest, me, 0)
                                predicateResult4 = false
                            end
                        end
                    end
                    scratchValue2 = __native_entity_state:GetStateInt("MemberState") == 1
                    if scratchValue2 then
                        getStateThing3 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsAlive()
                        scratchValue2 = getStateThing3
                    end
                    scratchValue = scratchValue2
                    if scratchValue then
                        getStateThing = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsEqualTo(me)
                        scratchValue = not getStateThing
                    end
                    if scratchValue then
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        helper_DCEC50(quest, me, 3)
                        quest:EntityStopFollowing(me)
                        alive = quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        quest:EntityFollowThing(me, quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")), 1.0, true)
                        require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                    end
                end
                scratchValue3 = __native_entity_state:GetStateInt("MemberState") == 3
                if scratchValue3 then
                    getStateThing4 = quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")):IsDead()
                    scratchValue4 = getStateThing4
                    if not scratchValue4 then
                        scratchValue11 = IsThingCarryingCrate(quest, me, quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")))
                        scratchValue4 = scratchValue11 == 0
                    end
                    scratchValue3 = scratchValue4
                end
                if scratchValue3 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:EntityStopFollowing(me)
                    helper_DCEC50(quest, me, 0)
                end
                scratchValue5 = __native_entity_state:GetStateInt("MemberState") == 1
                if scratchValue5 then
                    getStateThing5 = quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")):IsAlive()
                    scratchValue5 = getStateThing5
                end
                if scratchValue5 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    helper_DCEC50(quest, me, 4)
                    quest:EntityStopFollowing(me)
                    quest:EntityFollowThing(me, nil --[[missing]], quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")), 1.0)
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                end
                scratchValue6 = __native_entity_state:GetStateInt("MemberState") == 4
                if scratchValue6 then
                    getStateThing6 = quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")):IsDead()
                    scratchValue7 = getStateThing6
                    if not scratchValue7 then
                        scratchValue12 = IsThingCarryingCrate(quest, me, quest:GetStateThing(("Teams_" .. quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam")) .. "_TeamCrateCarrier")))
                        scratchValue7 = scratchValue12 == 0
                    end
                    scratchValue6 = scratchValue7
                end
                if scratchValue6 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    helper_DCEC50(quest, me, 0)
                end

                if __native_entity_state:GetStateInt("MemberState") == 2 then
                    sequence22 = true
                else
                    sequence22 = false
                end
                if sequence22 then
                    scratchValue13 = IsThingCarryingCrate(quest, me, me)
                    if scratchValue13 ~= 0 then
                        sequence22 = true
                    else
                        sequence22 = false
                    end
                end
                if sequence22 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    helper_DCEC50(quest, me, 5)
                    quest:SetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"), me)
                    require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "REQUEST_PROTECTION", me, 0)
                end

                if __native_entity_state:GetStateInt("MemberState") == 5 then
                    sequence32 = true
                else
                    sequence32 = false
                end
                if sequence32 then
                    scratchValue14 = IsThingCarryingCrate(quest, me, me)
                    if scratchValue14 == 0 then
                        sequence32 = true
                    else
                        sequence32 = false
                    end
                end
                if sequence32 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end

                    quest:SetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"), (nil))
                    helper_DCEC50(quest, me, 0)
                end
                whisperSpawned = quest:GetStateBool("WhisperSpawned")
            end
            alive = not quest:IsActiveThreadTerminating()
            predicateResult9 = not alive
            if not predicateResult9 then
                quest:EntityStopFollowing(me)
                getNearestWithScriptName = quest:GetNearestWithScriptName(me, "TeamExitMarker")
                scratchValue8 = resources:NewResource()
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_10);
                controlAcquired = resources:TryAcquire(scratchValue8, me, 4)
                while not controlAcquired do
                    alive = quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then goto LAB_00dcec33 end
                    controlAcquired = resources:TryAcquire(scratchValue8, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                predicateResult10 = not alive
                if not predicateResult10 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult11 = not alive
                    while not predicateResult11 do
                        taskRunning = me:IsPerformingScriptTask()
                        if not taskRunning then
                            if quest:IsActiveThreadTerminating() then break end
                            if not (getNearestWithScriptName ~= nil and not getNearestWithScriptName:IsNull()) then
                            else
                                p0_00 = getNearestWithScriptName:GetPos()
                            end
                            me:MoveToPosition(p0_00, 0x3f000000, 1, false, true)
                        end
                        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, getNearestWithScriptName, 2.0)
                        if isDistanceBetweenThingsUnder then
                            alive = not quest:IsActiveThreadTerminating()
                            predicateResult12 = not alive
                            if not predicateResult12 then
                                quest:FadeOutAndKillEntity(me, true, 3.0, true)
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    predicateResult13 = not alive
                                until not (not predicateResult13)
                            end
                            break
                        end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult11 = not alive
                    end
                end
                ::LAB_00dcec33::
                resources:ReleaseResource(scratchValue8)
            end
        end
    end
end

function Init(quest, me)
    local predicateResult, scratchValue, hero, scratchValue2
    local function __region_LAB_00dce00a()
        __native_entity_state:SetStateInt("TeamID", 1)
    end
    scratchValue2 = me:GetName()
    if scratchValue2 == nil then
    else
        scratchValue = ((scratchValue2 == "BanditTeamMember") and 0 or 1)
        predicateResult = not (scratchValue ~= 0)
        if predicateResult ~= 0 then __region_LAB_00dce00a(); goto LAB_00dcdf96 end
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    ::LAB_00dcdf96::
    if __native_entity_state:GetStateInt("TeamID") == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
            hero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero)
        else
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:SetIsThingTurncoatable(me, false)
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("BANDIT")
    else
        if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
            quest:EntitySetInFaction(me, "FACTION_VILLAGERS")
            hero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero)
        else
            quest:EntitySetInFaction(me, "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("GUARD")
    end
    __native_entity_state:SetStateInt("MyTeam", __native_entity_state:GetStateInt("TeamID"))
    scratchValue2 = (scratchValue + 24)
    -- TODO(native): *piVar1 = *piVar1 + 1;
    __native_entity_state:SetStateInt("CurrentAIState", 2)
    __native_entity_state:SetStateInt("PreviousAIState", 2)
    __native_entity_state:SetStateInt("MemberState", 0)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0")) + 1)
    __native_entity_state:SetStateThing("ThingToPatrolTo", nil)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) + -1)
    local getStateInt = __native_entity_state:GetStateInt("MemberState") + __native_entity_state:GetStateInt("TeamID") * 16
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"))) + -1)
    -- TODO(native): pvStack_4 = this;
    local scratchValue = me:MsgIsKilledBy("")
    if scratchValue then
        if __native_entity_state:GetStateInt("TeamID") == 1 then
            quest:SetMasterGameState("OrchardFarmBanditKilled", true)
            return
        end
        quest:SetMasterGameState("OrchardFarmGuardKilled", true)
    end
end

function GoOnPatrol(quest, me)
    local predicateResult, isActiveThreadTerminating, isActiveThreadTerminating2, hero, hero2
    local alive = true
    if __native_entity_state:GetStateInt("MemberState") ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        return predicateResult
    end
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end


        hero = quest:GetHero()
        quest:EntityFollowThing(me, hero, (3.0), (true))
    else
        isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating2 then
            return isActiveThreadTerminating2
        end


        hero2 = quest:GetHero()
        quest:EntityFollowThing(me, hero2, (1.0), (true))
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"))) + -1)
    __native_entity_state:SetStateInt("MemberState", 1)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1"), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1")) + 1)
    return piVar1
end

function IsThingCarryingCrate(quest, me, thing)
    local getStateListAt, getStateListAt2, scratchValue, getName, scratchValue2
    scratchValue2 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        scratchValue = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00dcedc1 end
            getStateListAt = quest:GetStateListAt("CrateList", scratchValue):IsAlive()
            if getStateListAt then
                getName = thing:GetName()
                getStateListAt2 = quest:GetStateListAt("CrateList", scratchValue):IsBeingCarriedBy(getName)
                if getStateListAt2 then
                    return not quest:IsActiveThreadTerminating()
                end
            end
            scratchValue2 = scratchValue2 + 1
            scratchValue = scratchValue + 1
        until not (scratchValue2 < quest:GetStateListCount("CrateList"))
    end
    ::LAB_00dcedc1::
    return false
end

function GetNearestCrateToMe(quest, me)
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, v_stk_c_1
    scratchValue4 = 10000000.0
    v_stk_c_1 = nil
    scratchValue3 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        scratchValue2 = 0
        repeat
            scratchValue = (quest:GetDistanceBetweenThings(me, (quest:GetStateListAt("CrateList", scratchValue2))) ^ 2)
            if scratchValue < scratchValue4 then
                scratchValue4 = scratchValue
                v_stk_c_1 = quest:GetStateListAt("CrateList", scratchValue2)
            end
            scratchValue3 = scratchValue3 + 1
            scratchValue2 = scratchValue2 + 1
        until not (scratchValue3 < (quest:GetStateListCount("CrateList")))
    end
    local ret_thing = v_stk_c_1
    return ret_thing
end

function helper_DCEC50(quest, me, param1)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"))) + -1)
    __native_entity_state:SetStateInt("MemberState", param1)
    quest:SetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. param1), quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. param1)) + 1)
    return piVar1
end

