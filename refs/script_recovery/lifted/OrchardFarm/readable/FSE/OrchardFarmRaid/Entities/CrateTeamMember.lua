-- Readable native conversion: CrateTeamMember. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

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
    local predicateResult4, predicateResult5, predicateResult6, scratchValue8
    local getCurrentStateGroupType, p0_00, getNearestWithScriptName, thing_38, scratchValue14
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
        if quest:GetStateBool("HeroAtWrongEntrance") then
            quest:RemoveThing(me, false, false)
            if 0 < quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount", quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") - 1)
            end
            quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"), quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")) - 1)
            return
        end
    end
    if __native_entity_state:GetStateInt("TeamID") == 1 and quest:GetStateInt("HeroTeam") == 1 then
        while quest:IsInCutscene() do
            if not quest:NewScriptFrame(me) then return end
        end
        helpers.MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
    end
    predicateResult4 = false
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        GoOnPatrol(quest, me)
        getCurrentStateGroupType = me:GetCurrentStateGroupType()
        __native_entity_state:SetStateInt("CurrentAIState", getCurrentStateGroupType)
        if getCurrentStateGroupType ~= __native_entity_state:GetStateInt("PreviousAIState") then
            if quest:IsActiveThreadTerminating() then return end
            if __native_entity_state:GetStateInt("CurrentAIState") == 2 then
                if __native_entity_state:GetStateInt("MemberState") == 3 then
                    helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                else
                    helpers.MakeTeamMemberComment(quest, me, "FOLLOWING", me, 0)
                end
            else
                if __native_entity_state:GetStateInt("CurrentAIState") ~= 1 then goto LAB_00dce500 end
                helpers.MakeTeamMemberComment(quest, me, "ATTACKING", me, 0)
            end
        end
        ::LAB_00dce500::
        __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
        if __native_entity_state:GetStateInt("MemberState") == 1 then
            if quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"):IsDead() and quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_2") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                thing_38 = GetNearestCrateToMe(quest, me)
                if not (thing_38 ~= nil and not thing_38:IsNull()) or not (thing_38 ~= nil and thing_38:IsAlive()) then
                    predicateResult6 = false
                else
                    if (thing_38 ~= nil and not thing_38:IsNull()) and thing_38:IsBeingCarriedBy("") then
                        predicateResult6 = false
                        goto FLOW_after_lab_00dce594
                    end
                    predicateResult6 = true
                end
                ::FLOW_after_lab_00dce594::
                if predicateResult6 then
                    scratchValue8 = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
                    if quest:GetStateInt("HeroTeam") == 0 and __native_entity_state:GetStateInt("TeamID") == 1 then
                        scratchValue8 = true
                    end
                    if quest:IsDistanceBetweenThingsOver(thing_38, quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_CrateDropPos"), 5.0) and scratchValue8 then
                        if __native_entity_state:GetStateInt("TeamID") == 1 then
                            quest:EntityStopFollowing(me)
                            quest:SetCombatNearbyBreakOffRange(me, 4.0)
                            quest:SetStealStealableItems(me, true)
                            predicateResult5 = false
                        else
                            quest:SetCombatNearbyBreakOffRange(me, 4.0)
                            quest:SetStealStealableItems(me, false)
                            predicateResult5 = true
                        end
                        quest:SetRecoverStealableItems(me, predicateResult5)
                        predicateResult4 = true
                        helper_DCEC50(quest, me, 2)
                        helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                    elseif predicateResult4 then
                        quest:ResetCombatNearbyBreakOffRange(me)
                        quest:SetStealStealableItems(me, false)
                        quest:SetRecoverStealableItems(me, false)
                        helper_DCEC50(quest, me, 0)
                        predicateResult4 = false
                    end
                end
            end
            if (__native_entity_state:GetStateInt("MemberState") == 1 and quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"):IsAlive()) and not quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"):IsEqualTo(me) then
                if quest:IsActiveThreadTerminating() then return end
                helper_DCEC50(quest, me, 3)
                quest:EntityStopFollowing(me)
                if not quest:NewScriptFrame(me) then return end
                quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"), 1.0, true)
                helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
            end
        end
        if __native_entity_state:GetStateInt("MemberState") == 3 and (quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            quest:EntityStopFollowing(me)
            helper_DCEC50(quest, me, 0)
        end
        if __native_entity_state:GetStateInt("MemberState") == 1 and quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            helper_DCEC50(quest, me, 4)
            quest:EntityStopFollowing(me)
            quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam") .. "_TeamCrateCarrier"), 1.0, true)
            helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
        end
        if __native_entity_state:GetStateInt("MemberState") == 4 and (quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_EnemyTeam") .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            helper_DCEC50(quest, me, 0)
        end
        if __native_entity_state:GetStateInt("MemberState") == 2 and IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            helper_DCEC50(quest, me, 5)
            quest:SetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier", me)
            helpers.MakeTeamMemberComment(quest, me, "REQUEST_PROTECTION", me, 0)
        end
        if __native_entity_state:GetStateInt("MemberState") == 5 and not IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateThing("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier", nil)
            helper_DCEC50(quest, me, 0)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntityStopFollowing(me)
    getNearestWithScriptName = quest:GetNearestWithScriptName(me, "TeamExitMarker")
    scratchValue14 = resources:NewResource()
    while not resources:TryAcquire(scratchValue14, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00dcec33 end
    end
    if not quest:IsActiveThreadTerminating() then
        while not quest:IsActiveThreadTerminating() do
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then break end
                if getNearestWithScriptName ~= nil and not getNearestWithScriptName:IsNull() then
                    p0_00 = getNearestWithScriptName:GetPos()
                end
                me:MoveToPosition(p0_00, 0.5, 1, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(me, getNearestWithScriptName, 2.0) then
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeOutAndKillEntity(me, true, 3.0, true)
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                end
                break
            end
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00dcec33::
    resources:ReleaseResource(scratchValue14)
end

function Init(quest, me)
    local getName = me:GetName()
    if getName ~= nil then
        if getName == "BanditTeamMember" then
            __native_entity_state:SetStateInt("TeamID", 1)
            goto LAB_00dcdf96
        end
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    ::LAB_00dcdf96::
    if __native_entity_state:GetStateInt("TeamID") == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
            quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
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
            quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
        else
            quest:EntitySetInFaction(me, "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("GUARD")
    end
    __native_entity_state:SetStateInt("MyTeam", __native_entity_state:GetStateInt("TeamID"))
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount", quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") + 1)
    __native_entity_state:SetStateInt("CurrentAIState", 2)
    __native_entity_state:SetStateInt("PreviousAIState", 2)
    __native_entity_state:SetStateInt("MemberState", 0)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0", quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_0") + 1)
    __native_entity_state:SetStateThing("ThingToPatrolTo", nil)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount", quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") - 1)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"), quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")) - 1)
    if me:MsgIsKilledBy("") then
        if __native_entity_state:GetStateInt("TeamID") == 1 then
            quest:SetMasterGameState("OrchardFarmBanditKilled", true)
            return
        end
        quest:SetMasterGameState("OrchardFarmGuardKilled", true)
    end
end

function GoOnPatrol(quest, me)
    if __native_entity_state:GetStateInt("MemberState") ~= 0 then
        return
    end
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityFollowThing(me, quest:GetHero(), 3.0, true)
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityFollowThing(me, quest:GetHero(), 1.0, true)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"), quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")) - 1)
    __native_entity_state:SetStateInt("MemberState", 1)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1", quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_1") + 1)
end

function IsThingCarryingCrate(quest, me, thing)
    local scratchValue, scratchValue2
    scratchValue2 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        scratchValue = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00dcedc1 end
            if quest:GetStateListAt("CrateList", scratchValue):IsAlive() then
                if quest:GetStateListAt("CrateList", scratchValue):IsBeingCarriedBy(thing:GetName()) then
                    return not quest:IsActiveThreadTerminating()
                end
            end
            scratchValue2 = scratchValue2 + 1
            scratchValue = scratchValue + 1
        until scratchValue2 >= quest:GetStateListCount("CrateList")
    end
    ::LAB_00dcedc1::
    return false
end

function GetNearestCrateToMe(quest, me)
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, x_stk_c_1
    scratchValue4 = 10000000.0
    x_stk_c_1 = nil
    scratchValue3 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        scratchValue2 = 0
        repeat
            scratchValue = quest:GetDistanceBetweenThings(me, quest:GetStateListAt("CrateList", scratchValue2)) ^ 2
            if scratchValue < scratchValue4 then
                scratchValue4 = scratchValue
                x_stk_c_1 = quest:GetStateListAt("CrateList", scratchValue2)
            end
            scratchValue3 = scratchValue3 + 1
            scratchValue2 = scratchValue2 + 1
        until scratchValue3 >= quest:GetStateListCount("CrateList")
    end
    return x_stk_c_1
end

function helper_DCEC50(quest, me, param1)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState"), quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. __native_entity_state:GetStateInt("MemberState")) - 1)
    __native_entity_state:SetStateInt("MemberState", param1)
    quest:SetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. param1, quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_StateCounter_" .. param1) + 1)
end

