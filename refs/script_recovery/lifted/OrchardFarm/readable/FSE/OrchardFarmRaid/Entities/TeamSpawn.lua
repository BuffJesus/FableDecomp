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
    local resources = quest:RetailResources()
    local scratchValue, doneIntroduction, whisperSpawned, scratchValue2, timeRemaining, getStateInt
    local scratchValue3, getStateInt2, scratchValue4, i_stk_70_1, i_stk_70_2, scratchValue5
    local getRandomThingWithScriptName, hero, hero2, scratchValue6, scratchValue7, teamMemberName
    local scratchValue8, scratchValue9, r3_1, r3_2, thing1
    local alive = true
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue = not alive
    if not scratchValue then
        quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
        if (__native_entity_state:GetStateInt("TeamID") == 1) and (quest:GetStateInt("HeroTeam") == 1) then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then
                return
            end
            quest:DisplayQuestInfo(true)
            scratchValue2 = quest:AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", __native_entity_state:GetStateInt("TeamMemberLimit") * 3, 1.0)
            __native_entity_state:SetStateInt("BanditsLeftID", scratchValue2)
            quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), __native_entity_state:GetStateInt("TeamMemberLimit") * 3, -1)
        end
        whisperSpawned = quest:GetStateBool("WhisperSpawned")
        while not whisperSpawned do
            alive = quest:NewScriptFrame(me)
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then
                return
            end
            if __native_entity_state:GetStateInt("TeamID") == 0 then
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    return
                end
                timeRemaining = quest:GetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")))
                if (timeRemaining == 0) and (quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) < __native_entity_state:GetStateInt("TeamMemberLimit")) then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetTimer(quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_TeamReinforcementsTimer")), __native_entity_state:GetStateInt("TeamRespawnTime"))
                    getStateInt = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))

                    thing1 = quest:GetHero()
                    scratchValue = quest:IsDistanceBetweenThingsOver(thing1, me, (15.0))
                    if scratchValue then
                        scratchValue5 = me:GetPos()
                        scratchValue = quest:IsCameraPosOnScreen(scratchValue5)
                        if not scratchValue then
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                return
                            end
                            i_stk_70_1 = 0
                            if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                                repeat
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then
                                        return
                                    end
                                    scratchValue = false
                                    teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                                    scratchValue5 = me:GetPos()
                                    scratchValue8 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), scratchValue5, teamMemberName)
                                    quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
                                    quest:Pause(2.0)
                                    i_stk_70_1 = i_stk_70_1 + 1
                                until not (i_stk_70_1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                            end
                            goto LAB_00dcd9ed
                        end
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    i_stk_70_2 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt then
                        repeat
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                return
                            end
                            scratchValue = false
                            teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                            scratchValue5 = __native_entity_state:GetStateThing("OtherSpawnPoint"):GetPos()
                            scratchValue9 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), scratchValue5, teamMemberName)
                            quest:EntityAttachToScript(scratchValue9, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
                            __native_entity_state:SetStateThing("OtherSpawnPoint", getRandomThingWithScriptName)
                            i_stk_70_2 = i_stk_70_2 + 1
                        until not (i_stk_70_2 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt)
                    end
                    ::LAB_00dcd9ed::
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                end
            else
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    return
                end
                if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:UpdateQuestInfoCounterList(__native_entity_state:GetStateInt("BanditsLeftID"), (2 - quest:GetStateInt("BanditWavesSpawned")) * __native_entity_state:GetStateInt("TeamMemberLimit") + quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")), -1)
                end
                if quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount")) == 0 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:Pause(quest:ReadGlobalGameData(3444))
                    quest:SetStateInt("BanditWavesSpawned", quest:GetStateInt("BanditWavesSpawned") + 1)
                    if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then
                            return
                        end
                        hero = quest:GetHero()
                        r3_1 = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                        scratchValue3 = (r3_1 ~= nil and r3_1:IsAlive())
                        if scratchValue3 then

                            hero2 = quest:GetHero()
                            scratchValue = quest:IsDistanceBetweenThingsUnder(r3_1, hero2, (15.0))
                            if scratchValue then
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then
                                    return
                                end
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "NEXT_WAVE", r3_1, 0)
                            end
                        end
                    end
                    if quest:GetStateInt("BanditWavesSpawned") == 3 then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then
                            return
                        end
                        if __native_entity_state:GetStateInt("TeamID") ~= quest:GetStateInt("HeroTeam") then
                            return
                        end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then
                            return
                        end
                        quest:SetStateInt("MissionFailed", 3)
                        return
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    getStateInt2 = quest:GetStateInt(("Teams_" .. __native_entity_state:GetStateInt("TeamID") .. "_MemberCount"))
                    scratchValue4 = 0
                    if __native_entity_state:GetStateInt("TeamMemberLimit") ~= getStateInt2 and -1 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2 then
                        repeat
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                return
                            end
                            r3_2 = resources:StartMovie("")
                            if (scratchValue4 == 1) and (quest:GetStateInt("HeroTeam") == 0) then
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue = not alive
                                if scratchValue then
                                    -- LAB_00dcda68: (native jump target)
                                    return
                                end
                                scratchValue = false
                                teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                                scratchValue5 = me:GetPos()
                                scratchValue6 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", scratchValue5, teamMemberName)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue = not alive
                                do return end
                                scratchValue = false
                                teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                                scratchValue5 = me:GetPos()
                                scratchValue7 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", scratchValue5, teamMemberName)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                                if scratchValue then goto FLOW_after_lab_00dcda68 end
                                scratchValue = false
                                teamMemberName = __native_entity_state:GetStateString("TeamMemberName")
                                scratchValue5 = me:GetPos()
                                scratchValue7 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), scratchValue5, teamMemberName)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            end
                            ::FLOW_after_lab_00dcda68::
                            quest:EntityAttachToScript(scratchValue6, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            scratchValue4 = scratchValue4 + 1
                        until not (scratchValue4 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2)
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    goto FLOW_after_lab_00dcd9ed
                end
            end
            ::FLOW_after_lab_00dcd9ed::
            whisperSpawned = quest:GetStateBool("WhisperSpawned")
        end
        alive = not quest:IsActiveThreadTerminating()
        scratchValue = not alive
        if not scratchValue then
            quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BanditsLeftID"))
        end
    end
end

function Init(quest, me)
    local scratchValue, scratchValue2, getName, scratchValue3
    local function __region_LAB_00dcd306()
        __native_entity_state:SetStateInt("TeamID", 1)
        __native_entity_state:SetStateString("TeamMemberName", "BanditTeamMember")
        scratchValue2 = "CREATURE_BANDIT_GRUNT"
    end
    scratchValue = quest:GetRandomThingWithScriptName("EitherTeamSpawn")
    __native_entity_state:SetStateThing("OtherSpawnPoint", scratchValue)
    scratchValue = nil
    -- TODO(native): GetName is not a ForgeFSE binding
    getName = me:GetName()
    if getName == nil then
    else
        if getName == "BanditTeamSpawn" then __region_LAB_00dcd306(); goto LAB_00dcd2bb end
    end
    __native_entity_state:SetStateInt("TeamID", 0)
    __native_entity_state:SetStateString("TeamMemberName", "GuardTeamMember")
    scratchValue2 = "CREATURE_ORCHARD_FARM_GUARD"
    ::LAB_00dcd2bb::
    __native_entity_state:SetStateString("TeamMemberDefName", scratchValue2)
    if __native_entity_state:GetStateInt("TeamID") == quest:GetStateInt("HeroTeam") then
        __native_entity_state:SetStateInt("TeamMemberLimit", 2)
        scratchValue3 = quest:ReadGlobalGameData(3436)
        __native_entity_state:SetStateInt("BanditsLeftID", 0)
        __native_entity_state:SetStateInt("TeamRespawnTime", scratchValue3)
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

