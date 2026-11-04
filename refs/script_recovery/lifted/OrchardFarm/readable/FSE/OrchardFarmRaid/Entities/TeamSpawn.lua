-- Readable native conversion: TeamSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GuardReinforcementsYourTeam = 3436,  -- 90
    GuardReinforcementsEnemyTeam = 3440,  -- 75
    BanditReinforcementDelay = 3444,  -- 10
}

local helpers = require("OrchardFarmRaid.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local teamID, teamRespawnTime_, teamMemberLimit_, teamMemberDefName_, banditsLeftID, teamMemberName_
local otherSpawnPoint

-- TeamSpawn.Main (retail 0x00dcd350)
function Main(quest, me)
    local scratchValue, i_stk_70_1, i_stk_70_2, scratchValue4, entity3
    local teamId = teamID
    local teamRespawnTime = teamRespawnTime_
    local heroTeam = quest:GetStateInt("HeroTeam")
    local teamMemberLimit = teamMemberLimit_
    local teamMemberDefName = teamMemberDefName_
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer"), teamRespawnTime)
    if teamId == 1 and heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:DisplayQuestInfo(true)
        banditsLeftID = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", teamMemberLimit * 3, 1.0)
        quest:UpdateQuestInfoCounterList(banditsLeftID, teamMemberLimit * 3, -1)
    end
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        if teamId == 0 then
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer")) == 0 and quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") < teamMemberLimit then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer"), teamRespawnTime)
                local getStateInt = quest:GetStateInt("Teams_" .. teamId .. "_MemberCount")
                local thing1 = quest:GetHero()
                if quest:IsDistanceBetweenThingsOver(thing1, me, 15.0) then
                    local position = me:GetPos()
                    if not quest:IsCameraPosOnScreen(position) then
                        if quest:IsActiveThreadTerminating() then return end
                        i_stk_70_1 = 0
                        if teamMemberLimit ~= getStateInt and -1 < ((teamMemberLimit + 2147483648) % 4294967296 - 2147483648) - getStateInt then
                            repeat
                                if quest:IsActiveThreadTerminating() then return end
                                local teamMemberName = teamMemberName_
                                local position2 = me:GetPos()
                                local entity = quest:CreateCreature(teamMemberDefName, position2, teamMemberName, false)
                                quest:EntityAttachToScript(entity, "Q_OrchardFarmRaid")
                                quest:Pause(2.0)
                                i_stk_70_1 = i_stk_70_1 + 1
                            until not (i_stk_70_1 < teamMemberLimit - getStateInt)
                        end
                        goto LAB_00dcd9ed
                    end
                end
                if quest:IsActiveThreadTerminating() then return end
                i_stk_70_2 = 0
                if teamMemberLimit ~= getStateInt and -1 < ((teamMemberLimit + 2147483648) % 4294967296 - 2147483648) - getStateInt then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        local teamMemberName2 = teamMemberName_
                        local getStateThing = otherSpawnPoint:GetPos()
                        local entity2 = quest:CreateCreature(teamMemberDefName, getStateThing, teamMemberName2, false)
                        quest:EntityAttachToScript(entity2, "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        scratchValue4 = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                        otherSpawnPoint = scratchValue4
                        i_stk_70_2 = i_stk_70_2 + 1
                    until not (i_stk_70_2 < teamMemberLimit - getStateInt)
                end
                goto LAB_00dcd9ed
            end
        else
            if quest:IsActiveThreadTerminating() then return end
            if teamId == heroTeam then
                if quest:IsActiveThreadTerminating() then return end
                quest:UpdateQuestInfoCounterList(banditsLeftID, (2 - quest:GetStateInt("BanditWavesSpawned")) * teamMemberLimit + quest:GetStateInt("Teams_" .. teamId .. "_MemberCount"), -1)
            end
            if quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                quest:Pause(quest:ReadGlobalGameData(SCRIPT_DEF.BanditReinforcementDelay))
                quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                if teamId ~= heroTeam then
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue4 = quest:GetHero()
                    local guardTeamMember = quest:GetNearestWithScriptName(scratchValue4, "GuardTeamMember")
                    if guardTeamMember ~= nil and guardTeamMember:IsAlive() then
                        scratchValue4 = quest:GetHero()
                        if quest:IsDistanceBetweenThingsUnder(guardTeamMember, scratchValue4, 15.0) then
                            if quest:IsActiveThreadTerminating() then return end
                            helpers.MakeTeamMemberComment(quest, me, "NEXT_WAVE", guardTeamMember, 0)
                        end
                    end
                end
                if quest:GetStateInt("BanditWavesSpawned") == 3 then
                    if quest:IsActiveThreadTerminating() then return end
                    if teamId ~= heroTeam then
                        return
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateInt("MissionFailed", 3)
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                local getStateInt2 = quest:GetStateInt("Teams_" .. teamId .. "_MemberCount")
                scratchValue = 0
                if teamMemberLimit ~= getStateInt2 and -1 < ((teamMemberLimit + 2147483648) % 4294967296 - 2147483648) - getStateInt2 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        entity3 = nil
                        if scratchValue == 1 and heroTeam == 0 then
                            if quest:IsActiveThreadTerminating() then return end
                            local teamMemberName3 = teamMemberName_
                            local position3 = me:GetPos()
                            scratchValue4 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", position3, teamMemberName3, false)
                            entity3 = scratchValue4
                        else
                            if quest:IsActiveThreadTerminating() then return end
                            local teamMemberName4 = teamMemberName_
                            local position4 = me:GetPos()
                            scratchValue4 = quest:CreateCreature(teamMemberDefName, position4, teamMemberName4, false)
                            entity3 = scratchValue4
                        end
                        quest:EntityAttachToScript(entity3, "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        scratchValue = scratchValue + 1
                    until not (scratchValue < teamMemberLimit - getStateInt2)
                end
                goto LAB_00dcd9ed
            end
        end
        goto FLOW_past_lab_00dcd9ed
        ::LAB_00dcd9ed::
        if quest:IsActiveThreadTerminating() then return end
        ::FLOW_past_lab_00dcd9ed::
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(banditsLeftID)
end

-- TeamSpawn.Init (retail 0x00dcd1d0)
function Init(quest, me)
    local getRandomThingWithScriptName, scratchValue
    getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    otherSpawnPoint = getRandomThingWithScriptName
    getRandomThingWithScriptName = nil
    local name = me:GetName()
    if name ~= nil and name == "BanditTeamSpawn" then
        teamID = 1
        teamMemberName_ = "BanditTeamMember"
        scratchValue = "CREATURE_BANDIT_GRUNT"
    else
        teamID = 0
        teamMemberName_ = "GuardTeamMember"
        scratchValue = "CREATURE_ORCHARD_FARM_GUARD"
    end
    teamMemberDefName_ = scratchValue
    if teamID == quest:GetStateInt("HeroTeam") then
        teamMemberLimit_ = 2
        local scratchValue2 = quest:ReadGlobalGameData(SCRIPT_DEF.GuardReinforcementsYourTeam)
        banditsLeftID = 0
        teamRespawnTime_ = scratchValue2
        return
    end
    teamMemberLimit_ = 3
    local scratchValue3 = quest:ReadGlobalGameData(SCRIPT_DEF.GuardReinforcementsEnemyTeam)
    banditsLeftID = 0
    teamRespawnTime_ = scratchValue3
end

-- TeamSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TeamSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

