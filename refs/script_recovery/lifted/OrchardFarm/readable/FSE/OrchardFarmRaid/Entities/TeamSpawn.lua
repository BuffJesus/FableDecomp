-- Readable native conversion: TeamSpawn. Review coverage report before use.
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
    local predicateResult, isDistanceBetweenThingsOver, isCameraPosOnScreen
    local isDistanceBetweenThingsUnder, predicateResult2, predicateResult3, doneIntroduction
    local whisperSpawned, scratchValue, timeRemaining, getStateInt, scratchValue2, getStateInt2
    local scratchValue3, i_stk_70_1, i_stk_70_2, position, position2, getStateThing, position3
    local position4, getRandomThingWithScriptName, hero, hero2, scratchValue4, teamMemberName
    local teamMemberName2, teamMemberName3, teamMemberName4, scratchValue5, scratchValue6
    local getNearestWithScriptName, thing1
    local alive = true
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    predicateResult = not alive
    if not predicateResult then
        quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
        if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
            if quest:IsActiveThreadTerminating() then
                return
            end
            quest:DisplayQuestInfo(true)
            scratchValue = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", __native_entity_state:GetStateInt("TeamMemberLimit") * 3, 1.0)
            __native_entity_state:SetStateInt("BanditsLeftID", scratchValue)
            quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), __native_entity_state:GetStateInt("TeamMemberLimit") * 3, -1)
        end
        whisperSpawned = quest:GetStateBool("WhisperSpawned")
        while not whisperSpawned do
            alive = quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                return
            end
            if __native_entity_state:GetStateInt("TeamID") == 0 then
                if quest:IsActiveThreadTerminating() then
                    return
                end
                timeRemaining = quest:GetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")))
                if (timeRemaining == 0) and (quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) < __native_entity_state:GetStateInt("TeamMemberLimit")) then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
                    getStateInt = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))

                    thing1 = quest:GetHero()
                    isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(thing1, me, (15.0))
                    if isDistanceBetweenThingsOver then
                        position = me:GetPos()
                        isCameraPosOnScreen = quest:IsCameraPosOnScreen(position)
                        if not isCameraPosOnScreen then
                            if quest:IsActiveThreadTerminating() then
                                return
                            end
                            i_stk_70_1 = 0
                            if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                                repeat
                                    if quest:IsActiveThreadTerminating() then
                                        return
                                    end

                                    teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                                    position2 = me:GetPos()
                                    scratchValue5 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), position2, teamMemberName)
                                    quest:EntityAttachToScript(scratchValue5, "Q_OrchardFarmRaid")
                                    quest:Pause(2.0)
                                    i_stk_70_1 = i_stk_70_1 + 1
                                until not (i_stk_70_1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                            end
                            goto LAB_00dcd9ed
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    i_stk_70_2 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                        repeat
                            if quest:IsActiveThreadTerminating() then
                                return
                            end

                            teamMemberName2 = __native_entity_state:GetStateString("TeamMemberName")
                            getStateThing = __native_entity_state:GetStateThing("OtherSpawnPoint"):GetPos()
                            scratchValue6 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), getStateThing, teamMemberName2)
                            quest:EntityAttachToScript(scratchValue6, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                            __native_entity_state:SetStateThing("OtherSpawnPoint", getRandomThingWithScriptName)
                            i_stk_70_2 = i_stk_70_2 + 1
                        until not (i_stk_70_2 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                    end
                    ::LAB_00dcd9ed::
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then
                    return
                end
                if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * __native_entity_state:GetStateInt("TeamMemberLimit") + quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")), -1)
                end
                if quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) == 0 then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:Pause(quest:ReadGlobalGameData(3444))
                    quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                    if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        hero = quest:GetHero()
                        getNearestWithScriptName = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                        scratchValue2 = (getNearestWithScriptName ~= nil and getNearestWithScriptName:IsAlive())
                        if scratchValue2 then

                            hero2 = quest:GetHero()
                            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(getNearestWithScriptName, hero2, (15.0))
                            if isDistanceBetweenThingsUnder then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "NEXT_WAVE", getNearestWithScriptName, 0)
                            end
                        end
                    end
                    if quest:GetStateInt("BanditWavesSpawned") == 3 then
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                            return
                        end
                        if quest:IsActiveThreadTerminating() then
                            return
                        end
                        quest:SetStateInt("MissionFailed", 3)
                        return
                    end
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    getStateInt2 = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))
                    scratchValue3 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt2 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2 then
                        repeat
                            if quest:IsActiveThreadTerminating() then
                                return
                            end

                            if (scratchValue3 == 1) and (quest:GetStateInt("HeroTeam") == 0) then
                                alive = not quest:IsActiveThreadTerminating()
                                predicateResult2 = not alive
                                if predicateResult2 then
                                    -- LAB_00dcda68: (native jump target)
                                    return
                                end

                                teamMemberName3 = __native_entity_state:GetStateString("TeamMemberName")
                                position3 = me:GetPos()
                                scratchValue4 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", position3, teamMemberName3)
                            else
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end

                                teamMemberName4 = __native_entity_state:GetStateString("TeamMemberName")
                                position4 = me:GetPos()
                                scratchValue4 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), position4, teamMemberName4)
                            end
                            quest:EntityAttachToScript(scratchValue4, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            scratchValue3 = scratchValue3 + 1
                        until not (scratchValue3 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2)
                    end
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    goto FLOW_after_lab_00dcd9ed
                end
            end
            ::FLOW_after_lab_00dcd9ed::
            whisperSpawned = quest:GetStateBool("WhisperSpawned")
        end
        alive = not quest:IsActiveThreadTerminating()
        predicateResult3 = not alive
        if not predicateResult3 then
            quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BanditsLeftID"))
        end
    end
end

function Init(quest, me)
    local getRandomThingWithScriptName, scratchValue, getName, scratchValue2, scratchValue3
    getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    __native_entity_state:SetStateThing("OtherSpawnPoint", getRandomThingWithScriptName)

    getName = me:GetName()
    if getName == nil then
    else
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

