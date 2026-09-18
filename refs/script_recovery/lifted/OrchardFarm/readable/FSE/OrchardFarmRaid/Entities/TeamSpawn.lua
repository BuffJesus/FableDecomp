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
local teamID, teamRespawnTime, teamMemberLimit, teamMemberDefName, teamMemberName, banditsLeftID
local otherSpawnPoint

-- TeamSpawn.Main (retail 0x00dcd350)
function Main(quest, me)
    local getStateInt, getStateInt2, scratchValue, i_stk_70_1, i_stk_70_2, guardTeamMember
    local scratchValue7
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTimer(quest:GetStateInt("Teams_" .. teamID .. "_TeamReinforcementsTimer"), teamRespawnTime)
    if teamID == 1 and heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:DisplayQuestInfo(true)
        banditsLeftID = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", teamMemberLimit * 3, 1.0)
        quest:UpdateQuestInfoCounterList(banditsLeftID, teamMemberLimit * 3, -1)
    end
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        if teamID == 0 then
            if quest:GetTimer(quest:GetStateInt("Teams_" .. teamID .. "_TeamReinforcementsTimer")) == 0 and quest:GetStateInt("Teams_" .. teamID .. "_MemberCount") < teamMemberLimit then
                quest:SetTimer(quest:GetStateInt("Teams_" .. teamID .. "_TeamReinforcementsTimer"), teamRespawnTime)
                getStateInt = quest:GetStateInt("Teams_" .. teamID .. "_MemberCount")
                if quest:IsDistanceBetweenThingsOver(hero, me, 15.0) and not quest:IsCameraPosOnScreen(me:GetPos()) then
                    if quest:IsActiveThreadTerminating() then return end
                    i_stk_70_1 = 0
                    if teamMemberLimit ~= getStateInt and -1 < teamMemberLimit - getStateInt then
                        repeat
                            if quest:IsActiveThreadTerminating() then return end
                            quest:EntityAttachToScript(quest:CreateCreature(teamMemberDefName, me:GetPos(), teamMemberName), "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            i_stk_70_1 = i_stk_70_1 + 1
                        until not (i_stk_70_1 < teamMemberLimit - getStateInt)
                    end
                else
                    if quest:IsActiveThreadTerminating() then return end
                    i_stk_70_2 = 0
                    if teamMemberLimit ~= getStateInt and -1 < teamMemberLimit - getStateInt then
                        repeat
                            if quest:IsActiveThreadTerminating() then return end
                            quest:EntityAttachToScript(quest:CreateCreature(teamMemberDefName, otherSpawnPoint:GetPos(), teamMemberName), "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            otherSpawnPoint = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                            i_stk_70_2 = i_stk_70_2 + 1
                        until not (i_stk_70_2 < teamMemberLimit - getStateInt)
                end
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        else
            if teamID == heroTeam then
                quest:UpdateQuestInfoCounterList(banditsLeftID, (2 - quest:GetStateInt("BanditWavesSpawned")) * teamMemberLimit + quest:GetStateInt("Teams_" .. teamID .. "_MemberCount"), -1)
            end
            if quest:GetStateInt("Teams_" .. teamID .. "_MemberCount") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                quest:Pause(quest:ReadGlobalGameData(SCRIPT_DEF.BanditReinforcementDelay))
                quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                if teamID ~= heroTeam then
                    if quest:IsActiveThreadTerminating() then return end
                    guardTeamMember = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                    if guardTeamMember ~= nil and guardTeamMember:IsAlive() then
                        if quest:IsDistanceBetweenThingsUnder(guardTeamMember, hero, 15.0) then
                            helpers.MakeTeamMemberComment(quest, me, "NEXT_WAVE", guardTeamMember, 0)
                        end
                    end
                end
                if quest:GetStateInt("BanditWavesSpawned") == 3 then
                    if quest:IsActiveThreadTerminating() then return end
                    if teamID ~= heroTeam then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 3)
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                getStateInt2 = quest:GetStateInt("Teams_" .. teamID .. "_MemberCount")
                scratchValue = 0
                if teamMemberLimit ~= getStateInt2 and -1 < teamMemberLimit - getStateInt2 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if scratchValue == 1 and heroTeam == 0 then
                            scratchValue7 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", me:GetPos(), teamMemberName)
                        else
                            scratchValue7 = quest:CreateCreature(teamMemberDefName, me:GetPos(), teamMemberName)
                        end
                        quest:EntityAttachToScript(scratchValue7, "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        scratchValue = scratchValue + 1
                    until not (scratchValue < teamMemberLimit - getStateInt2)
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(banditsLeftID)
end

-- TeamSpawn.Init (retail 0x00dcd1d0)
function Init(quest, me)
    local scratchValue, name
    otherSpawnPoint = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    name = me:GetName()
    if name ~= nil and name == "BanditTeamSpawn" then
        teamID = 1
        teamMemberName = "BanditTeamMember"
        scratchValue = "CREATURE_BANDIT_GRUNT"
    else
        teamID = 0
        teamMemberName = "GuardTeamMember"
        scratchValue = "CREATURE_ORCHARD_FARM_GUARD"
    end
    teamMemberDefName = scratchValue
    if teamID == quest:GetStateInt("HeroTeam") then
        teamMemberLimit = 2
        banditsLeftID = 0
        teamRespawnTime = quest:ReadGlobalGameData(SCRIPT_DEF.GuardReinforcementsYourTeam)
        return
    end
    teamMemberLimit = 3
    banditsLeftID = 0
    teamRespawnTime = quest:ReadGlobalGameData(SCRIPT_DEF.GuardReinforcementsEnemyTeam)
end

-- TeamSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TeamSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

