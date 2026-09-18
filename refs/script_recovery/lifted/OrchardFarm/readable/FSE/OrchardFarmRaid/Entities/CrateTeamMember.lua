-- Readable native conversion: CrateTeamMember. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- CrateTeamMember.Main (retail 0x00dce230)
function Main(quest, me)
    local predicateResult, predicateResult5, isDistanceBetweenThingsUnder, getCurrentStateGroupType
    local p0_00, teamExitMarker, thing_38
    local teamId = state:GetInt("TeamID")
    local heroTeam = quest:GetStateInt("HeroTeam")
    local myTeam = state:GetInt("MyTeam")
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
        if quest:GetStateBool("HeroAtWrongEntrance") then
            quest:RemoveThing(me, false, false)
            if 0 < quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("Teams_" .. teamId .. "_MemberCount", quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") - 1)
            end
            quest:SetStateInt("Teams_" .. teamId .. "_StateCounter_" .. state:GetInt("MemberState"), quest:GetStateInt("Teams_" .. teamId .. "_StateCounter_" .. state:GetInt("MemberState")) - 1)
            return
        end
    end
    if teamId == 1 and heroTeam == 1 then
        while quest:IsInCutscene() do
            if not quest:NewScriptFrame(me) then return end
        end
        helpers.MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
    end
    predicateResult = false
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        GoOnPatrol(quest, me)
        getCurrentStateGroupType = me:GetCurrentStateGroupType()
        state:SetInt("CurrentAIState", getCurrentStateGroupType)
        if getCurrentStateGroupType ~= state:GetInt("PreviousAIState") then
            if quest:IsActiveThreadTerminating() then return end
            if state:GetInt("CurrentAIState") == 2 then
                if state:GetInt("MemberState") == 3 then
                    helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                else
                    helpers.MakeTeamMemberComment(quest, me, "FOLLOWING", me, 0)
                end
            elseif state:GetInt("CurrentAIState") == 1 then
                helpers.MakeTeamMemberComment(quest, me, "ATTACKING", me, 0)
            end
        end
        state:SetInt("PreviousAIState", state:GetInt("CurrentAIState"))
        if state:GetInt("MemberState") == 1 then
            if quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsDead() and quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_2") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                thing_38 = GetNearestCrateToMe(quest, me)
                if (thing_38 ~= nil and not thing_38:IsNull()) and (thing_38 ~= nil and thing_38:IsAlive()) and not ((thing_38 ~= nil and not thing_38:IsNull()) and thing_38:IsBeingCarriedBy("")) then
                    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
                    if heroTeam == 0 and teamId == 1 then
                        isDistanceBetweenThingsUnder = true
                    end
                    if quest:IsDistanceBetweenThingsOver(thing_38, quest:GetStateThing("Teams_" .. myTeam .. "_CrateDropPos"), 5.0) and isDistanceBetweenThingsUnder then
                        if teamId == 1 then
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
                        predicateResult = true
                        SetMemberState(quest, me, 2)
                        helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                    elseif predicateResult then
                        quest:ResetCombatNearbyBreakOffRange(me)
                        quest:SetStealStealableItems(me, false)
                        quest:SetRecoverStealableItems(me, false)
                        SetMemberState(quest, me, 0)
                        predicateResult = false
                    end
                end
            end
            if (state:GetInt("MemberState") == 1 and quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsAlive()) and not quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsEqualTo(me) then
                if quest:IsActiveThreadTerminating() then return end
                SetMemberState(quest, me, 3)
                quest:EntityStopFollowing(me)
                if not quest:NewScriptFrame(me) then return end
                quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"), 1.0, true)
                helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
            end
        end
        if state:GetInt("MemberState") == 3 and (quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            quest:EntityStopFollowing(me)
            SetMemberState(quest, me, 0)
        end
        if state:GetInt("MemberState") == 1 and quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 4)
            quest:EntityStopFollowing(me)
            quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"), 1.0, true)
            helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
        end
        if state:GetInt("MemberState") == 4 and (quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 0)
        end
        if state:GetInt("MemberState") == 2 and IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 5)
            quest:SetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier", me)
            helpers.MakeTeamMemberComment(quest, me, "REQUEST_PROTECTION", me, 0)
        end
        if state:GetInt("MemberState") == 5 and not IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier", nil)
            SetMemberState(quest, me, 0)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntityStopFollowing(me)
    teamExitMarker = quest:GetNearestWithScriptName(me, "TeamExitMarker")
    if me:AcquireControl(4) then
        while not quest:IsActiveThreadTerminating() do
            if not me:IsPerformingScriptTask() then
                if teamExitMarker ~= nil and not teamExitMarker:IsNull() then
                    p0_00 = teamExitMarker:GetPos()
                end
                me:MoveToPosition(p0_00, 0.5, 1, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(me, teamExitMarker, 2.0) then
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
    me:ReleaseControl()
end

-- CrateTeamMember.Init (retail 0x00dcdf60)
function Init(quest, me)
    local name
    local hero = quest:GetHero()
    name = me:GetName()
    if name ~= nil and name == "BanditTeamMember" then
        state:SetInt("TeamID", 1)
    else
        state:SetInt("TeamID", 0)
    end
    if state:GetInt("TeamID") == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
            quest:EntitySetThingAsAllyOfThing(me, hero)
        else
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:SetIsThingTurncoatable(me, false)
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("BANDIT")
    else
        if state:GetInt("TeamID") == quest:GetStateInt("HeroTeam") then
            quest:EntitySetInFaction(me, "FACTION_VILLAGERS")
            quest:EntitySetThingAsAllyOfThing(me, hero)
        else
            quest:EntitySetInFaction(me, "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("GUARD")
    end
    state:SetInt("MyTeam", state:GetInt("TeamID"))
    quest:SetStateInt("Teams_" .. state:GetInt("TeamID") .. "_MemberCount", quest:GetStateInt("Teams_" .. state:GetInt("TeamID") .. "_MemberCount") + 1)
    state:SetInt("CurrentAIState", 2)
    state:SetInt("PreviousAIState", 2)
    state:SetInt("MemberState", 0)
    quest:SetStateInt("Teams_" .. state:GetInt("MyTeam") .. "_StateCounter_0", quest:GetStateInt("Teams_" .. state:GetInt("MyTeam") .. "_StateCounter_0") + 1)
    state:SetThing("ThingToPatrolTo", nil)
end

-- CrateTeamMember.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- CrateTeamMember.OnPredicateFail (retail 0x00dcded0)
function OnPredicateFail(quest, me)
    local teamId = state:GetInt("TeamID")
    quest:SetStateInt("Teams_" .. teamId .. "_MemberCount", quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") - 1)
    quest:SetStateInt("Teams_" .. teamId .. "_StateCounter_" .. state:GetInt("MemberState"), quest:GetStateInt("Teams_" .. teamId .. "_StateCounter_" .. state:GetInt("MemberState")) - 1)
    if me:MsgIsKilledBy("") then
        if teamId == 1 then
            quest:SetMasterGameState("OrchardFarmBanditKilled", true)
            return
        end
        quest:SetMasterGameState("OrchardFarmGuardKilled", true)
    end
end

-- CrateTeamMember.GoOnPatrol (retail 0x00dcec70)
function GoOnPatrol(quest, me)
    local myTeam = state:GetInt("MyTeam")
    local hero = quest:GetHero()
    if state:GetInt("MemberState") ~= 0 then
        return
    end
    if state:GetInt("TeamID") == quest:GetStateInt("HeroTeam") then
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityFollowThing(me, hero, 3.0, true)
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityFollowThing(me, hero, 1.0, true)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. state:GetInt("MemberState"), quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. state:GetInt("MemberState")) - 1)
    state:SetInt("MemberState", 1)
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_1", quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_1") + 1)
end

-- CrateTeamMember.IsThingCarryingCrate (retail 0x00dced10)
function IsThingCarryingCrate(quest, me, thing)
    local scratchValue, scratchValue2
    scratchValue2 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        scratchValue = 0
        repeat
            if quest:IsActiveThreadTerminating() then return false end
            if quest:GetStateListAt("CrateList", scratchValue):IsAlive() then
                if quest:GetStateListAt("CrateList", scratchValue):IsBeingCarriedBy(thing:GetName()) then
                    return not quest:IsActiveThreadTerminating()
                end
            end
            scratchValue2 = scratchValue2 + 1
            scratchValue = scratchValue + 1
        until scratchValue2 >= quest:GetStateListCount("CrateList")
    end
    return false
end

-- CrateTeamMember.GetNearestCrateToMe (retail 0x00dcedf0)
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

-- helper 0xDCEC50 (named after the state it writes)
function SetMemberState(quest, me, param1)
    local myTeam = state:GetInt("MyTeam")
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. state:GetInt("MemberState"), quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. state:GetInt("MemberState")) - 1)
    state:SetInt("MemberState", param1)
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. param1, quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. param1) + 1)
end

