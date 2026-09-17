-- Readable native conversion: TeamSpawn. Review coverage report before use.
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
    local getStateInt, getStateInt2, scratchValue3, i_stk_70_1, i_stk_70_2, getNearestWithScriptName
    local scratchValue8
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTimer(quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer"), __native_entity_state:GetStateInt("TeamRespawnTime"))
    if __native_entity_state:GetStateInt("TeamID") == 1 and quest:GetStateInt("HeroTeam") == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:DisplayQuestInfo(true)
        __native_entity_state:SetStateInt("BanditsLeftID", quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", __native_entity_state:GetStateInt("TeamMemberLimit") * 3, 1.0))
        quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), __native_entity_state:GetStateInt("TeamMemberLimit") * 3, -1)
    end
    while not quest:GetStateBool("WhisperSpawned") do
        if not quest:NewScriptFrame(me) then return end
        if __native_entity_state:GetStateInt("TeamID") == 0 then
            if quest:GetTimer(quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")) == 0 and quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") < __native_entity_state:GetStateInt("TeamMemberLimit") then
                quest:SetTimer(quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer"), __native_entity_state:GetStateInt("TeamRespawnTime"))
                getStateInt = quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")
                if quest:IsDistanceBetweenThingsOver(quest:GetHero(), me, 15.0) then
                    if not quest:IsCameraPosOnScreen(me:GetPos()) then
                        if quest:IsActiveThreadTerminating() then return end
                        i_stk_70_1 = 0
                        if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                            repeat
                                if quest:IsActiveThreadTerminating() then return end
                                quest:EntityAttachToScript(quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), me:GetPos(), __native_entity_state:GetStateString("TeamMemberName")), "Q_OrchardFarmRaid")
                                quest:Pause(2.0)
                                i_stk_70_1 = i_stk_70_1 + 1
                            until not (i_stk_70_1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                        end
                        goto LAB_00dcd9ed
                    end
                end
                if quest:IsActiveThreadTerminating() then return end
                i_stk_70_2 = 0
                if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        quest:EntityAttachToScript(quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), __native_entity_state:GetStateThing("OtherSpawnPoint"):GetPos(), __native_entity_state:GetStateString("TeamMemberName")), "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        __native_entity_state:SetStateThing("OtherSpawnPoint", quest:GetRandomThingWithScriptName("EitherTeamSpawn"))
                        i_stk_70_2 = i_stk_70_2 + 1
                    until not (i_stk_70_2 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                end
                ::LAB_00dcd9ed::
                if quest:IsActiveThreadTerminating() then return end
            end
        else
            if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
                quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * __native_entity_state:GetStateInt("TeamMemberLimit") + quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"), -1)
            end
            if quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount") == 0 then
                if quest:IsActiveThreadTerminating() then return end
                quest:Pause(quest:ReadGlobalGameData(3444))
                quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
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
                    if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 3)
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                getStateInt2 = quest:GetStateInt("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")
                scratchValue3 = 0
                if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt2 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if scratchValue3 == 1 and quest:GetStateInt("HeroTeam") == 0 then
                            scratchValue8 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", me:GetPos(), __native_entity_state:GetStateString("TeamMemberName"))
                        else
                            scratchValue8 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), me:GetPos(), __native_entity_state:GetStateString("TeamMemberName"))
                        end
                        quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
                        quest:Pause(2.0)
                        scratchValue3 = scratchValue3 + 1
                    until not (scratchValue3 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2)
                end
                if quest:IsActiveThreadTerminating() then return end
                goto FLOW_after_lab_00dcd9ed
            end
        end
        ::FLOW_after_lab_00dcd9ed::
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BanditsLeftID"))
end

function Init(quest, me)
    local scratchValue, getName, scratchValue2, scratchValue3
    __native_entity_state:SetStateThing("OtherSpawnPoint", quest:GetRandomThingWithScriptName("EitherTeamSpawn"))
    getName = me:GetName()
    if getName ~= nil then
        if getName == "BanditTeamSpawn" then
            __native_entity_state:SetStateInt("TeamID", 1)
            __native_entity_state:SetStateString("TeamMemberName", "BanditTeamMember")
            scratchValue = "CREATURE_BANDIT_GRUNT"
            goto LAB_00dcd2bb
        end
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    __native_entity_state:SetStateString("TeamMemberName", "GuardTeamMember")
    scratchValue = "CREATURE_ORCHARD_FARM_GUARD"
    ::LAB_00dcd2bb::
    __native_entity_state:SetStateString("TeamMemberDefName", scratchValue)
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        __native_entity_state:SetStateInt("TeamMemberLimit", 2)
        scratchValue2 = quest:ReadGlobalGameData(3436)
        __native_entity_state:SetStateInt("BanditsLeftID", 0)
        __native_entity_state:SetStateInt("TeamRespawnTime", scratchValue2)
        return
    end
    __native_entity_state:SetStateInt("TeamMemberLimit", 3)
    scratchValue3 = quest:ReadGlobalGameData(3440)
    __native_entity_state:SetStateInt("BanditsLeftID", 0)
    __native_entity_state:SetStateInt("TeamRespawnTime", scratchValue3)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

