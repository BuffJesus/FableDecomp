-- Readable native conversion: SkillTarget. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_RearDummySegements = 3812,  -- 40.0
    GUI_MiddleDummySegements = 3816,  -- 60.0
    GUI_FrontDummySegements = 3820,  -- 80.0
    GUI_RearDummyWorth = 3824,  -- 9.0
    GUI_MiddleDummyWorth = 3828,  -- 3.0
    GUI_FrontDummyWorth = 3832,  -- 1.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local dummyNumber_, speed, dummyWorth

-- SkillTarget.Main (retail 0x00d41d00)
function Main(quest, me)
    local predicateResult4, scratchValue3, getMasterGameState, scratchValue4, scratchValue5
    local scratchValue6, readGlobalGameDataFloat, scratchValue7, scratchValue36, scratchValue37
    local getStateInt, i_stk_16c_1, i_stk_16c_2, scratchValue38, pPos, getPos, getPos2, getPos3
    local nearest, scratchValue40, theRealGuildmaster, skillApprentice2, scratchValue41, timerId
    local dummyNumber = dummyNumber_
    local function ReleaseEverything()
        local scratchValue38 = theRealGuildmaster
        quest:DeregisterTimer(timerId)
    end
    local function ReleaseEverything2()
        local scratchValue38 = skillApprentice2
        quest:DeregisterTimer(timerId)
    end
    local f_stk_164_1 = me:GetAngleXY()
    local dummyEndMarker = quest:GetNearestWithScriptName(me, "DummyEndMarker")
    local dummyStartMarker = quest:GetNearestWithScriptName(me, "DummyStartMarker")
    if dummyNumber == 1 then
        if quest:IsActiveThreadTerminating() then return end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummySegements)))
        nearest = quest:GetNearestWithScriptName(me, "StaticDummyMarker3")
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummyWorth)
    elseif dummyNumber == 2 then
        if quest:IsActiveThreadTerminating() then return end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummySegements)))
        nearest = quest:GetNearestWithScriptName(me, "StaticDummyMarker2")
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummyWorth)
    else
        if quest:IsActiveThreadTerminating() then return end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummySegements)))
        nearest = quest:GetNearestWithScriptName(me, "StaticDummyMarker1")
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummyWorth)
    end
    dummyWorth = math.tointeger(math.modf(readGlobalGameDataFloat))
    if not quest:NewScriptFrame(me) then return end
    if not quest:NewScriptFrame(me) then return end
    getMasterGameState = quest:GetMasterGameState("SkillTrainingStarted")
    i_stk_16c_1 = 0
    while not getMasterGameState do
        if not quest:NewScriptFrame(me) then return end
        getMasterGameState = quest:GetMasterGameState("SkillTrainingStarted")
    end
    if quest:IsActiveThreadTerminating() then return end
    predicateResult4 = false
    scratchValue36 = 0
    scratchValue7 = f_stk_164_1
    repeat
        local f_stk_164_2 = scratchValue7
        if predicateResult4 then
            return
        end
        local position = me:GetPos()
        local vec_bc = {x = position.x, y = position.y, z = position.z}
        if not (dummyStartMarker ~= nil and not dummyStartMarker:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = dummyStartMarker:GetPos()
        end
        local vec_160 = {x = getPos.x, y = getPos.y, z = getPos.z}
        if not (dummyEndMarker ~= nil and not dummyEndMarker:IsNull()) then
            getPos2 = {x = 0, y = 0, z = 0}
        else
            getPos2 = dummyEndMarker:GetPos()
        end
        local vec_124 = {x = getPos2.x, y = getPos2.y, z = getPos2.z}
        if not (nearest ~= nil and not nearest:IsNull()) then
            getPos3 = {x = 0, y = 0, z = 0}
        else
            getPos3 = nearest:GetPos()
        end
        local f_stk_84_1 = speed
        local vec_13c = {x = getPos3.x, y = getPos3.y, z = getPos3.z}
        local scratchValue31 = (vec_124.x - vec_160.x) / f_stk_84_1
        scratchValue5 = 0
        quest:SetMasterGameState("SkillDummyReset", false)
        local scratchValue30 = (vec_124.y - vec_160.y) / f_stk_84_1
        local scratchValue23 = (vec_160.x - vec_124.x) / f_stk_84_1
        local scratchValue22 = (vec_160.y - vec_124.y) / f_stk_84_1
        local scratchValue25 = (vec_13c.x - vec_bc.x) / f_stk_84_1
        local scratchValue24 = (vec_13c.y - vec_bc.y) / f_stk_84_1
        local scratchValue26 = (vec_160.x - vec_13c.x) / f_stk_84_1
        local f_stk_84_2 = (vec_160.y - vec_13c.y) / f_stk_84_1
        repeat
            if not quest:NewScriptFrame(me) then return end
            local scratchValue19 = vec_bc.z
            local scratchValue11 = scratchValue24 * i_stk_16c_1
            local scratchValue21 = scratchValue25 * i_stk_16c_1 + vec_bc.x
            local scratchValue20 = scratchValue11 + vec_bc.y
            quest:EntityTeleportToPosition(me, {x = scratchValue21, y = scratchValue20, z = scratchValue19}, scratchValue7, false, false)
            scratchValue36 = scratchValue36 + 1
            i_stk_16c_1 = scratchValue36
        until scratchValue36 == speed
        if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
        if not quest:GetMasterGameState("MovingDummiesNeeded") then
            if quest:IsActiveThreadTerminating() then return end
            while true do
                repeat
                    if not quest:NewScriptFrame(me) then return end
                    scratchValue6 = me:MsgIsHitByHeroWithProjectileWeapon()
                until scratchValue6 ~= nil
                if quest:IsActiveThreadTerminating() then return end
                if 0.5 < scratchValue6 then break end
                if quest:IsActiveThreadTerminating() then return end
                quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") - 1)
            end
            if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
            quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
            scratchValue37 = 0
            i_stk_16c_2 = 0
            quest:EntitySetTargetable(me, false)
            repeat
                if not quest:NewScriptFrame(me) then return end
                local scratchValue13 = vec_13c.z
                local scratchValue9 = f_stk_84_2 * i_stk_16c_2
                local scratchValue15 = scratchValue26 * i_stk_16c_2 + vec_13c.x
                local scratchValue14 = scratchValue9 + vec_13c.y
                quest:EntityTeleportToPosition(me, {x = scratchValue15, y = scratchValue14, z = scratchValue13}, scratchValue7, false, false)
                scratchValue37 = scratchValue37 + 1
                i_stk_16c_2 = scratchValue37
            until scratchValue37 == speed
            if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
            while not quest:GetMasterGameState("MovingDummiesNeeded") do
                if not quest:NewScriptFrame(me) then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:EntitySetTargetable(me, true)
        end
        i_stk_16c_1 = 0
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        scratchValue4 = 0
        repeat
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
            if scratchValue5 == 0 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                local scratchValue = scratchValue30 * i_stk_16c_1
                local scratchValue27 = vec_160.z
                local scratchValue29 = scratchValue31 * i_stk_16c_1 + vec_160.x
                local scratchValue28 = scratchValue + vec_160.y
                pPos = {x = scratchValue29, y = scratchValue28, z = scratchValue27}
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                local scratchValue12 = scratchValue22 * i_stk_16c_1
                local scratchValue16 = vec_124.z
                local scratchValue18 = scratchValue23 * i_stk_16c_1 + vec_124.x
                local scratchValue17 = scratchValue12 + vec_124.y
                pPos = {x = scratchValue18, y = scratchValue17, z = scratchValue16}
            end
            quest:EntityTeleportToPosition(me, pPos, f_stk_164_2, false, false)
            local scratchValue43 = me:MsgIsHitByHeroWithProjectileWeapon()
            if scratchValue43 ~= nil then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if quest:GetMasterGameState("MovingDummiesNeeded") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    scratchValue38 = quest:GetThingWithScriptName("ArcheryRing")
                    local thing1 = quest:GetHero()
                    local isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(thing1, scratchValue38, 6.0)
                    local predicateResult = quest:IsActiveThreadTerminating()
                    if isDistanceBetweenThingsOver then
                        if predicateResult then
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        local isQuestActive = quest:IsQuestActive("Q_GuildTrainingSkill")
                        local predicateResult35 = quest:IsActiveThreadTerminating()
                        if isQuestActive then
                            if predicateResult35 then
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            local theGuildmaster = quest:GetThingWithScriptName("TheGuildmaster")
                            local conversationId = quest:AddNewConversation(theGuildmaster, false, false)
                            scratchValue38 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId, scratchValue38)
                            scratchValue38 = quest:GetHero()
                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HIT_OUT", theGuildmaster, scratchValue38, false)
                        else
                            if predicateResult35 then
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            local skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
                            local conversationId2 = quest:AddNewConversation(skillApprentice, false, false)
                            scratchValue38 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId2, scratchValue38)
                            scratchValue38 = quest:GetHero()
                            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", skillApprentice, scratchValue38, false)
                        end
                    else
                        if predicateResult then
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        scratchValue38 = quest:GetHero()
                        scratchValue40 = quest:PlaySoundOnThing(scratchValue38, "SND_ARROWIMPACT_02")
                        if 0.25 <= scratchValue43 then
                            if 0.5 <= scratchValue43 then
                                if scratchValue43 < 0.75 then
                                    if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + dummyWorth * 3); goto LAB_00d42874 end
                                    quest:DeregisterTimer(timerId)
                                    return
                                end
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                getStateInt = dummyWorth << 2
                            else
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                getStateInt = dummyWorth << 1
                            end
                            quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + getStateInt)
                        else
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + dummyWorth)
                        end
                        ::LAB_00d42874::
                        if quest:GetTimer(timerId) < 1 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            local randomChoice = quest:RetailRandModulo(32768) & 0x80000003
                            scratchValue3 = randomChoice == 0
                            if randomChoice < 0 then
                                scratchValue3 = (randomChoice - 1 | 0xfffffffc) == 0xffffffff
                            end
                            if scratchValue3 then
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                quest:SetTimer(timerId, 8)
                                local isQuestActive2 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                local predicateResult36 = quest:IsActiveThreadTerminating()
                                if isQuestActive2 then
                                    if predicateResult36 then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                    local conversationId3 = quest:AddNewConversation(theRealGuildmaster, false, false)
                                    scratchValue38 = quest:GetHero()
                                    quest:AddPersonToConversation(conversationId3, scratchValue38)
                                    if 0.25 <= scratchValue43 then
                                        if 0.5 <= scratchValue43 then
                                            if 0.75 <= scratchValue43 then
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                                scratchValue38 = quest:GetHero()
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", theRealGuildmaster, scratchValue38, false)
                                                scratchValue38 = theRealGuildmaster
                                            else
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                                scratchValue38 = quest:GetHero()
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", theRealGuildmaster, scratchValue38, false)
                                                scratchValue38 = theRealGuildmaster
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                            scratchValue38 = quest:GetHero()
                                            quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", theRealGuildmaster, scratchValue38, false)
                                            scratchValue38 = theRealGuildmaster
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                        scratchValue38 = quest:GetHero()
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", theRealGuildmaster, scratchValue38, false)
                                        scratchValue38 = theRealGuildmaster
                                    end
                                else
                                    if predicateResult36 then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    skillApprentice2 = quest:GetThingWithScriptName("SkillApprentice")
                                    local conversationId4 = quest:AddNewConversation(skillApprentice2, false, false)
                                    scratchValue38 = quest:GetHero()
                                    quest:AddPersonToConversation(conversationId4, scratchValue38)
                                    if 0.25 <= scratchValue43 then
                                        if 0.5 <= scratchValue43 then
                                            if 0.75 <= scratchValue43 then
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                                scratchValue38 = quest:GetHero()
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", skillApprentice2, scratchValue38, false)
                                            else
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                                scratchValue38 = quest:GetHero()
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", skillApprentice2, scratchValue38, false)
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                            scratchValue38 = quest:GetHero()
                                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", skillApprentice2, scratchValue38, false)
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                        scratchValue38 = quest:GetHero()
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", skillApprentice2, scratchValue38, false)
                                    end
                                    scratchValue38 = skillApprentice2
                                end
                            end
                        end
                        quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                    end
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    scratchValue38 = quest:GetHero()
                    scratchValue41 = quest:PlaySoundOnThing(scratchValue38, "SND_ARROWIMPACT_02")
                    quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                end
            end
            i_stk_16c_1 = i_stk_16c_1 + 1
            if i_stk_16c_1 == speed then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                scratchValue5 = scratchValue5 == 0 and 1 or 0
                i_stk_16c_1 = 0
            end
            if quest:GetMasterGameState("SkillRepeatKnown") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if quest:GetMasterGameState("SkillRepeating") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    scratchValue4 = 1
                elseif quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    return
                end
                quest:SetMasterGameState("SkillDummyReset", true)
                while quest:GetMasterGameState("SkillRepeatKnown") do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            end
        until scratchValue4 ~= 0
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:DeregisterTimer(timerId)
        quest:NewScriptFrame(me)
        predicateResult4 = quest:IsActiveThreadTerminating()
        scratchValue36 = i_stk_16c_1
        scratchValue7 = f_stk_164_2
    until false
end

-- SkillTarget.Init (retail 0x00d41ca0)
function Init(quest, me)
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local this_00 = me:GetDataString()
    dummyNumber_ = parseGameInteger(this_00)
end

-- SkillTarget.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SkillTarget.OnPredicateFail (retail 0x00d41cd0)
function OnPredicateFail(quest, me)
end

