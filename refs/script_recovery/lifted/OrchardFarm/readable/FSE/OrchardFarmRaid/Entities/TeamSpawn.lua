-- Readable native conversion: TeamSpawn. Review coverage report before use.
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

-- TeamSpawn.Main (retail 0x00dcd350)
function Main(quest, me)
    local getStateInt, getStateInt2, scratchValue3, i_stk_70_1, i_stk_70_2, getNearestWithScriptName
    local scratchValue8
    local teamId = state:GetInt("TeamID")
    local teamRespawnTime = state:GetInt("TeamRespawnTime")
    local heroTeam = quest:GetStateInt("HeroTeam")
    local teamMemberLimit = state:GetInt("TeamMemberLimit")
    local teamMemberDefName = state:GetString("TeamMemberDefName")
    local teamMemberName = state:GetString("TeamMemberName")
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer"), teamRespawnTime)
    if teamId == 1 and heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:DisplayQuestInfo(true)
        state:SetInt("BanditsLeftID", quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", teamMemberLimit * 3, 1.0))
        quest:UpdateQuestInfoCounterList(state:GetInt("BanditsLeftID"), teamMemberLimit * 3, -1)
    end
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        if teamId == 0 then
            if quest:GetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer")) == 0 and quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") < teamMemberLimit then
                quest:SetTimer(quest:GetStateInt("Teams_" .. teamId .. "_TeamReinforcementsTimer"), teamRespawnTime)
                getStateInt = quest:GetStateInt("Teams_" .. teamId .. "_MemberCount")
                if quest:IsDistanceBetweenThingsOver(quest:GetHero(), me, 15.0) and not quest:IsCameraPosOnScreen(me:GetPos()) then
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
                            quest:EntityAttachToScript(quest:CreateCreature(teamMemberDefName, state:GetThing("OtherSpawnPoint"):GetPos(), teamMemberName), "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            state:SetThing("OtherSpawnPoint", quest:GetRandomThingWithScriptName("EitherTeamSpawn"))
                            i_stk_70_2 = i_stk_70_2 + 1
                        until not (i_stk_70_2 < teamMemberLimit - getStateInt)
                end
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        else
            if teamId == heroTeam then
                quest:UpdateQuestInfoCounterList(state:GetInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * teamMemberLimit + quest:GetStateInt("Teams_" .. teamId .. "_MemberCount"), -1)
            end
            if quest:GetStateInt("Teams_" .. teamId .. "_MemberCount") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                quest:Pause(quest:ReadGlobalGameData(3444))
                quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                if teamId ~= heroTeam then
                    if quest:IsActiveThreadTerminating() then return end
                    getNearestWithScriptName = quest:GetNearestWithScriptName(quest:GetHero(), "GuardTeamMember")
                    if getNearestWithScriptName ~= nil and getNearestWithScriptName:IsAlive() then
                        if quest:IsDistanceBetweenThingsUnder(getNearestWithScriptName, quest:GetHero(), 15.0) then
                            helpers.MakeTeamMemberComment(quest, me, "NEXT_WAVE", getNearestWithScriptName, 0)
                        end
                    end
                end
                if quest:GetStateInt("BanditWavesSpawned") == 3 then
                    if quest:IsActiveThreadTerminating() then return end
                    if teamId ~= heroTeam then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 3)
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                getStateInt2 = quest:GetStateInt("Teams_" .. teamId .. "_MemberCount")
                scratchValue3 = 0
                if teamMemberLimit ~= getStateInt2 and -1 < teamMemberLimit - getStateInt2 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if scratchValue3 == 1 and heroTeam == 0 then
                            scratchValue8 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", me:GetPos(), teamMemberName)
                        else
                            scratchValue8 = quest:CreateCreature(teamMemberDefName, me:GetPos(), teamMemberName)
                        end
                        quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        scratchValue3 = scratchValue3 + 1
                    until not (scratchValue3 < teamMemberLimit - getStateInt2)
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(state:GetInt("BanditsLeftID"))
end

-- TeamSpawn.Init (retail 0x00dcd1d0)
function Init(quest, me)
    local scratchValue, getName, scratchValue2, scratchValue3
    state:SetThing("OtherSpawnPoint", quest:GetRandomThingWithScriptName("EitherTeamSpawn"))
    getName = me:GetName()
    if getName ~= nil and getName == "BanditTeamSpawn" then
        state:SetInt("TeamID", 1)
        state:SetString("TeamMemberName", "BanditTeamMember")
        scratchValue = "CREATURE_BANDIT_GRUNT"
    else
        state:SetInt("TeamID", 0)
        state:SetString("TeamMemberName", "GuardTeamMember")
        scratchValue = "CREATURE_ORCHARD_FARM_GUARD"
    end
    state:SetString("TeamMemberDefName", scratchValue)
    if state:GetInt("TeamID") == quest:GetStateInt("HeroTeam") then
        state:SetInt("TeamMemberLimit", 2)
        scratchValue2 = quest:ReadGlobalGameData(3436)
        state:SetInt("BanditsLeftID", 0)
        state:SetInt("TeamRespawnTime", scratchValue2)
        return
    end
    state:SetInt("TeamMemberLimit", 3)
    scratchValue3 = quest:ReadGlobalGameData(3440)
    state:SetInt("BanditsLeftID", 0)
    state:SetInt("TeamRespawnTime", scratchValue3)
end

-- TeamSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TeamSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

