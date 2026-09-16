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
    local predicateResult, isDistanceBetweenThingsOver, isCameraPosOnScreen
    local isDistanceBetweenThingsUnder, predicateResult2, scratchValue, timeRemaining, getStateInt
    local scratchValue2, getStateInt2, scratchValue3, i_stk_70_1, i_stk_70_2, position, position2
    local getStateThing, position3, position4, getRandomThingWithScriptName, hero, hero2
    local scratchValue4, teamMemberName, teamMemberName2, teamMemberName3, teamMemberName4
    local scratchValue5, scratchValue6, r3_1, r3_2, thing1
    local alive = true
    local scratchValue7 = quest:GetStateBool("DoneIntroduction")
    while not scratchValue7 do
        alive = quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            return
        end
        scratchValue7 = quest:GetStateBool("DoneIntroduction")
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
        scratchValue7 = quest:GetStateBool("WhisperSpawned")
        while not scratchValue7 do
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
                            -- TODO(native): goto LAB_00dcd9ed
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
                    -- LAB_00dcd9ed: (native jump target)
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
                        r3_1 = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                        scratchValue2 = (r3_1 ~= nil and r3_1:IsAlive())
                        if scratchValue2 then

                            hero2 = quest:GetHero()
                            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(r3_1, hero2, (15.0))
                            if isDistanceBetweenThingsUnder then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end
                                require("OrchardFarmRaid.native_quest_helpers").MakeTeamMemberComment(quest, me, "NEXT_WAVE", r3_1, 0)
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
                            r3_2 = resources:StartMovie("")
                            if (scratchValue3 == 1) and (quest:GetStateInt("HeroTeam") == 0) then
                                if quest:IsActiveThreadTerminating() then
                                    return
                                end

                                teamMemberName3 = __native_entity_state:GetStateString("TeamMemberName")
                                position3 = me:GetPos()
                                scratchValue4 = quest:CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", position3, teamMemberName3)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            else
                                if quest:IsActiveThreadTerminating() then return end

                                teamMemberName4 = __native_entity_state:GetStateString("TeamMemberName")
                                position4 = me:GetPos()
                                scratchValue4 = quest:CreateCreature(__native_entity_state:GetStateString("TeamMemberDefName"), position4, teamMemberName4)
                                -- TODO(native): CScriptThing::operator=((CScriptThing *)CStack_54,(int)pCVar6);
                            end
                            quest:EntityAttachToScript(scratchValue4, "Q_OrchardFarmRaid")
                            quest:Pause(2.0)
                            scratchValue3 = scratchValue3 + 1
                        until not (scratchValue3 < __native_entity_state:GetStateInt("TeamMemberLimit") - getStateInt2)
                    end
                    -- TODO(native): goto LAB_00dcd9ed
                end
            end
            scratchValue7 = quest:GetStateBool("WhisperSpawned")
        end
        alive = not quest:IsActiveThreadTerminating()
        predicateResult2 = not alive
        if not predicateResult2 then
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

