-- Readable native conversion: CrateTeamMember. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("OrchardFarmRaid.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local teamID, myTeam_, memberState, currentAIState, previousAIState, thingToPatrolTo

-- CrateTeamMember.Main (retail 0x00dce230)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult5, predicateResult6, isDistanceBetweenThingsUnder
    local getStateInt, p0_00
    local teamId = teamID
    local heroTeam = quest:GetStateInt("HeroTeam")
    local myTeam = myTeam_
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
        if quest:GetStateBool("HeroAtWrongEntrance") then
            if quest:IsActiveThreadTerminating() then return end
            quest:RemoveThing(me, false, false)
            if 0 < quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("Teams_" .. teamId .. "_MemberCount", quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") - 1)
            end
            getStateInt = memberState + teamId * 16
            quest:SetStateInt("Teams_" .. teamId .. "_StateCounter_" .. memberState, quest:GetStateInt("Teams_" .. teamId .. "_StateCounter_" .. memberState) - 1)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    if teamId == 1 and heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsInCutscene() do
            if not quest:NewScriptFrame(me) then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        helpers.MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
    end
    predicateResult = false
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        GoOnPatrol(quest, me)
        local getCurrentStateGroupType = me:GetCurrentStateGroupType()
        currentAIState = getCurrentStateGroupType
        if getCurrentStateGroupType ~= previousAIState then
            if quest:IsActiveThreadTerminating() then return end
            if currentAIState == 2 then
                if quest:IsActiveThreadTerminating() then return end
                if memberState == 3 then
                    if quest:IsActiveThreadTerminating() then return end
                    helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
                else
                    if quest:IsActiveThreadTerminating() then return end
                    helpers.MakeTeamMemberComment(quest, me, "FOLLOWING", me, 0)
                end
            elseif currentAIState == 1 then
                if quest:IsActiveThreadTerminating() then return end
                helpers.MakeTeamMemberComment(quest, me, "ATTACKING", me, 0)
            end
        end
        previousAIState = currentAIState
        if memberState == 1 then
            if quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsDead() and quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_2") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                local thing_38 = GetNearestCrateToMe(quest, me)
                if not (thing_38 ~= nil and not thing_38:IsNull()) or not (thing_38 ~= nil and thing_38:IsAlive()) then
                    goto LAB_00dce594
                else
                    if (thing_38 ~= nil and not thing_38:IsNull()) and (thing_38 ~= nil and thing_38:IsBeingCarriedBy("")) then goto LAB_00dce594 end
                    predicateResult6 = true
                end
                goto FLOW_past_lab_00dce594
                ::LAB_00dce594::
                predicateResult6 = false
                ::FLOW_past_lab_00dce594::
                if predicateResult6 then
                    if quest:IsActiveThreadTerminating() then return end
                    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
                    if heroTeam == 0 and teamId == 1 then
                        if quest:IsActiveThreadTerminating() then return end
                        isDistanceBetweenThingsUnder = true
                    end
                    if quest:IsDistanceBetweenThingsOver(thing_38, quest:GetStateThing("Teams_" .. myTeam .. "_CrateDropPos"), 5.0) and isDistanceBetweenThingsUnder then
                        if quest:IsActiveThreadTerminating() then return end
                        if teamId == 1 then
                            if quest:IsActiveThreadTerminating() then return end
                            quest:EntityStopFollowing(me)
                            quest:SetCombatNearbyBreakOffRange(me, 4.0)
                            quest:SetStealStealableItems(me, true)
                            predicateResult5 = false
                        else
                            if quest:IsActiveThreadTerminating() then return end
                            quest:SetCombatNearbyBreakOffRange(me, 4.0)
                            quest:SetStealStealableItems(me, false)
                            predicateResult5 = true
                        end
                        quest:SetRecoverStealableItems(me, predicateResult5)
                        predicateResult = true
                        SetMemberState(quest, me, 2)
                        helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
                    elseif predicateResult then
                        if quest:IsActiveThreadTerminating() then return end
                        quest:ResetCombatNearbyBreakOffRange(me)
                        quest:SetStealStealableItems(me, false)
                        quest:SetRecoverStealableItems(me, false)
                        SetMemberState(quest, me, 0)
                        predicateResult = false
                    end
                end
            end
            if (memberState == 1 and quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsAlive()) and not quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsEqualTo(me) then
                if quest:IsActiveThreadTerminating() then return end
                SetMemberState(quest, me, 3)
                quest:EntityStopFollowing(me)
                if not quest:NewScriptFrame(me) then return end
                quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"), 1.0, true)
                helpers.MakeTeamMemberComment(quest, me, "DEFENDING", me, 0)
            end
        end
        if memberState == 3 and (quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            quest:EntityStopFollowing(me)
            SetMemberState(quest, me, 0)
        end
        if memberState == 1 and quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 4)
            quest:EntityStopFollowing(me)
            quest:EntityFollowThing(me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"), 1.0, true)
            helpers.MakeTeamMemberComment(quest, me, "FETCHING", me, 0)
        end
        if memberState == 4 and (quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"):IsDead() or not IsThingCarryingCrate(quest, me, quest:GetStateThing("Teams_" .. quest:GetStateInt("Teams_" .. myTeam .. "_EnemyTeam") .. "_TeamCrateCarrier"))) then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 0)
        end
        if memberState == 2 and IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            SetMemberState(quest, me, 5)
            quest:SetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier", me)
            helpers.MakeTeamMemberComment(quest, me, "REQUEST_PROTECTION", me, 0)
        end
        if memberState == 5 and not IsThingCarryingCrate(quest, me, me) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateThing("Teams_" .. myTeam .. "_TeamCrateCarrier", nil)
            SetMemberState(quest, me, 0)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntityStopFollowing(me)
    local teamExitMarker = quest:GetNearestWithScriptName(me, "TeamExitMarker")
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while not quest:IsActiveThreadTerminating() do
        if not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then break end
            if not (teamExitMarker ~= nil and not teamExitMarker:IsNull()) then
                p0_00 = {x = 0, y = 0, z = 0}
            else
                p0_00 = teamExitMarker:GetPos()
            end
            me:MoveToPosition(p0_00, 0.5, ENTITY_MOVE_RUN, false, true)
        end
        if not quest:IsDistanceBetweenThingsUnder(me, teamExitMarker, 2.0) then
            quest:NewScriptFrame(me)
        else
            if not quest:IsActiveThreadTerminating() then
                quest:FadeOutAndKillEntity(me, true, 3.0, true)
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
            end
            break
            quest:NewScriptFrame(me)
        end
    end
    resources:ReleaseResource(resource)
end

-- CrateTeamMember.Init (retail 0x00dcdf60)
function Init(quest, me)
    local name = me:GetName()
    if name ~= nil and name == "BanditTeamMember" then
        teamID = 1
    else
        teamID = 0
    end
    if teamID == 1 then
        if quest:GetStateInt("HeroTeam") == 1 then
            quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
            local hero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero)
        else
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:SetIsThingTurncoatable(me, false)
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("BANDIT")
    else
        if teamID == quest:GetStateInt("HeroTeam") then
            quest:EntitySetInFaction(me, "FACTION_VILLAGERS")
            local hero2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero2)
        else
            quest:EntitySetInFaction(me, "FACTION_GUARDS_ENEMY")
            quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")
            quest:EntitySetAsDisplayingEmoteIcon(me, false)
        end
        me:SetDataString("GUARD")
    end
    myTeam_ = teamID
    quest:SetStateInt("Teams_" .. teamID .. "_MemberCount", quest:GetStateInt("Teams_" .. teamID .. "_MemberCount") + 1)
    currentAIState = 2
    previousAIState = 2
    memberState = 0
    quest:SetStateInt("Teams_" .. myTeam_ .. "_StateCounter_0", quest:GetStateInt("Teams_" .. myTeam_ .. "_StateCounter_0") + 1)
    thingToPatrolTo = nil
end

-- CrateTeamMember.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CrateTeamMember.OnPredicateFail (retail 0x00dcded0)
function OnPredicateFail(quest, me)
    local teamId = teamID
    quest:SetStateInt("Teams_" .. teamId .. "_MemberCount", quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") - 1)
    quest:SetStateInt("Teams_" .. teamId .. "_StateCounter_" .. memberState, quest:GetStateInt("Teams_" .. teamId .. "_StateCounter_" .. memberState) - 1)
    if not me:MsgIsKilledBy("") then return end
    if teamId == 1 then
        quest:SetMasterGameState("OrchardFarmBanditKilled", true)
        return
    end
    quest:SetMasterGameState("OrchardFarmGuardKilled", true)
end

-- CrateTeamMember.GoOnPatrol (retail 0x00dcec70)
function GoOnPatrol(quest, me)
    local myTeam = myTeam_
    if memberState ~= 0 then
        return
    end
    if teamID == quest:GetStateInt("HeroTeam") then
        if quest:IsActiveThreadTerminating() then return end
        local hero = quest:GetHero()
        quest:EntityFollowThing(me, hero, 3.0, true)
    else
        if quest:IsActiveThreadTerminating() then return end
        local hero2 = quest:GetHero()
        quest:EntityFollowThing(me, hero2, 1.0, true)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    end
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. memberState, quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. memberState) - 1)
    memberState = 1
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_1", quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_1") + 1)
end

-- CrateTeamMember.IsThingCarryingCrate (retail 0x00dced10)
function IsThingCarryingCrate(quest, me, thing)
    local crateListIndex, name, scratchValue
    scratchValue = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        crateListIndex = 0
        repeat
            if quest:IsActiveThreadTerminating() then return false end
            if not quest:GetStateListAt("CrateList", crateListIndex):IsAlive() then scratchValue = scratchValue + 1; crateListIndex = crateListIndex + 1; goto continue_1 end
            name = thing:GetName()
            if quest:GetStateListAt("CrateList", crateListIndex):IsBeingCarriedBy(name) then
                return not quest:IsActiveThreadTerminating()
            end
            scratchValue = scratchValue + 1
            crateListIndex = crateListIndex + 1
            ::continue_1::
        until not (scratchValue < quest:GetStateListCount("CrateList"))
    end
    quest:IsActiveThreadTerminating()
    return false
end

-- CrateTeamMember.GetNearestCrateToMe (retail 0x00dcedf0)
function GetNearestCrateToMe(quest, me)
    local resources = quest:RetailResources()
    local crateListIndex, scratchValue2, scratchValue3, getStateListAt
    scratchValue3 = 10000000.0
    getStateListAt = nil
    scratchValue2 = 0
    if quest:GetStateListCount("CrateList") ~= 0 then
        crateListIndex = 0
        repeat
            local scratchValue = resources:DistanceBetweenThings(me, (quest:GetStateListAt("CrateList", crateListIndex)))
            if scratchValue < scratchValue3 then
                scratchValue3 = ("<f"):unpack(("<f"):pack(scratchValue))
                getStateListAt = quest:GetStateListAt("CrateList", crateListIndex)
            end
            scratchValue2 = scratchValue2 + 1
            crateListIndex = crateListIndex + 1
        until not (scratchValue2 < quest:GetStateListCount("CrateList"))
    end
    local ret_thing = getStateListAt
    getStateListAt = nil
    return ret_thing
end

-- helper 0xDCEC50 (named after the state it writes)
function SetMemberState(quest, me, param1)
    local myTeam = myTeam_
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. memberState, quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. memberState) - 1)
    memberState = param1
    quest:SetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. param1, quest:GetStateInt("Teams_" .. myTeam .. "_StateCounter_" .. param1) + 1)
end

