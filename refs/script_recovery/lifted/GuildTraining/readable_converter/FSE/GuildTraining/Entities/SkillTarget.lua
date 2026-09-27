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
local dummyNumber, speed, dummyWorth

-- SkillTarget.Main (retail 0x00d41d00)
function Main(quest, me)
    local predicateResult4, scratchValue3, getMasterGameState, scratchValue4, scratchValue5
    local scratchValue6, readGlobalGameDataFloat, scratchValue48, scratchValue49, getStateInt
    local i_stk_16c_1, i_stk_16c_2, scratchValue50, pPos, getPos, getPos2, getPos3, nearest
    local theRealGuildmaster, skillApprentice2, timerId
    local hero = quest:GetHero()
    local function ReleaseEverything()
        local scratchValue50 = theRealGuildmaster
        quest:DeregisterTimer(timerId)
    end
    local function ReleaseEverything2()
        local scratchValue50 = skillApprentice2
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
    predicateResult4 = false
    scratchValue48 = 0
    repeat
        if predicateResult4 then
            return
        end
        local position = me:GetPos()
        local scratchValue43 = position.x
        local scratchValue42 = position.y
        local scratchValue41 = position.z
        if not (dummyStartMarker ~= nil and not dummyStartMarker:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = dummyStartMarker:GetPos()
        end
        local scratchValue18 = getPos.x
        local scratchValue17 = getPos.y
        local scratchValue16 = getPos.z
        if not (dummyEndMarker ~= nil and not dummyEndMarker:IsNull()) then
            getPos2 = {x = 0, y = 0, z = 0}
        else
            getPos2 = dummyEndMarker:GetPos()
        end
        local scratchValue11 = getPos2.x
        local scratchValue = getPos2.y
        local scratchValue9 = getPos2.z
        if not (nearest ~= nil and not nearest:IsNull()) then
            getPos3 = {x = 0, y = 0, z = 0}
        else
            getPos3 = nearest:GetPos()
        end
        local f_stk_84_1 = speed
        local scratchValue14 = getPos3.x
        local scratchValue13 = getPos3.y
        local scratchValue12 = getPos3.z
        local scratchValue40 = (scratchValue11 - scratchValue18) / f_stk_84_1
        scratchValue5 = 0
        quest:SetMasterGameState("SkillDummyReset", false)
        local scratchValue39 = (scratchValue - scratchValue17) / f_stk_84_1
        local scratchValue32 = (scratchValue18 - scratchValue11) / f_stk_84_1
        local scratchValue31 = (scratchValue17 - scratchValue) / f_stk_84_1
        local scratchValue34 = (scratchValue14 - scratchValue43) / f_stk_84_1
        local scratchValue33 = (scratchValue13 - scratchValue42) / f_stk_84_1
        local scratchValue35 = (scratchValue18 - scratchValue14) / f_stk_84_1
        local f_stk_84_2 = (scratchValue17 - scratchValue13) / f_stk_84_1
        repeat
            if not quest:NewScriptFrame(me) then return end
            quest:EntityTeleportToPosition(me, {x = (scratchValue34 * i_stk_16c_1 + scratchValue43), y = ((scratchValue33 * i_stk_16c_1) + scratchValue42), z = scratchValue41}, f_stk_164_1, false, false)
            scratchValue48 = scratchValue48 + 1
            i_stk_16c_1 = scratchValue48
        until scratchValue48 == speed
        if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
        if not quest:GetMasterGameState("MovingDummiesNeeded") then
            if quest:IsActiveThreadTerminating() then return end
            while true do
                repeat
                    if not quest:NewScriptFrame(me) then return end
                    scratchValue6 = me:MsgIsHitByHeroWithProjectileWeapon()
                until scratchValue6 ~= nil
                if 0.5 < scratchValue6 then break end
                quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") - 1)
            end
            if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
            quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
            scratchValue49 = 0
            i_stk_16c_2 = 0
            quest:EntitySetTargetable(me, false)
            repeat
                if not quest:NewScriptFrame(me) then return end
                quest:EntityTeleportToPosition(me, {x = (scratchValue35 * i_stk_16c_2 + scratchValue14), y = ((f_stk_84_2 * i_stk_16c_2) + scratchValue13), z = scratchValue12}, f_stk_164_1, false, false)
                scratchValue49 = scratchValue49 + 1
                i_stk_16c_2 = scratchValue49
            until scratchValue49 == speed
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
                pPos = {x = (scratchValue40 * i_stk_16c_1 + scratchValue18), y = ((scratchValue39 * i_stk_16c_1) + scratchValue17), z = scratchValue16}
            else
                pPos = {x = (scratchValue32 * i_stk_16c_1 + scratchValue11), y = ((scratchValue31 * i_stk_16c_1) + scratchValue), z = scratchValue9}
            end
            quest:EntityTeleportToPosition(me, pPos, f_stk_164_1, false, false)
            local scratchValue56 = me:MsgIsHitByHeroWithProjectileWeapon()
            if scratchValue56 ~= nil then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if quest:GetMasterGameState("MovingDummiesNeeded") then
                    if quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then
                        if quest:IsQuestActive("Q_GuildTrainingSkill") then
                            local theGuildmaster = quest:GetThingWithScriptName("TheGuildmaster")
                            local conversationId = quest:AddNewConversation(theGuildmaster, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HIT_OUT", theGuildmaster, hero, false)
                        else
                            local skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
                            local conversationId2 = quest:AddNewConversation(skillApprentice, false, false)
                            quest:AddPersonToConversation(conversationId2, hero)
                            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", skillApprentice, hero, false)
                        end
                    else
                        quest:PlaySoundOnThing(hero, "SND_ARROWIMPACT_02")
                        if 0.25 <= scratchValue56 then
                            if 0.5 <= scratchValue56 then
                                if scratchValue56 < 0.75 then
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
                            local scratchValue55 = math.random(0, 32767) & 0x80000003
                            scratchValue3 = scratchValue55 == 0
                            if scratchValue55 < 0 then
                                scratchValue3 = (scratchValue55 - 1 | 0xfffffffc) == 0xffffffff
                            end
                            if scratchValue3 then
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                quest:SetTimer(timerId, 8)
                                local predicateResult = quest:IsActiveThreadTerminating()
                                if quest:IsQuestActive("Q_GuildTrainingSkill") then
                                    if predicateResult then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                    local conversationId3 = quest:AddNewConversation(theRealGuildmaster, false, false)
                                    quest:AddPersonToConversation(conversationId3, hero)
                                    if 0.25 <= scratchValue56 then
                                        if 0.5 <= scratchValue56 then
                                            if 0.75 <= scratchValue56 then
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", theRealGuildmaster, hero, false)
                                            else
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", theRealGuildmaster, hero, false)
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                            quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", theRealGuildmaster, hero, false)
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", theRealGuildmaster, hero, false)
                                    end
                                else
                                    if predicateResult then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    skillApprentice2 = quest:GetThingWithScriptName("SkillApprentice")
                                    local conversationId4 = quest:AddNewConversation(skillApprentice2, false, false)
                                    quest:AddPersonToConversation(conversationId4, hero)
                                    if 0.25 <= scratchValue56 then
                                        if 0.5 <= scratchValue56 then
                                            if 0.75 <= scratchValue56 then
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", skillApprentice2, hero, false)
                                            else
                                                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", skillApprentice2, hero, false)
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", skillApprentice2, hero, false)
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", skillApprentice2, hero, false)
                                    end
                                end
                            end
                        end
                        quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                    end
                else
                    quest:PlaySoundOnThing(hero, "SND_ARROWIMPACT_02")
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
        scratchValue48 = i_stk_16c_1
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
    dummyNumber = parseGameInteger(me:GetDataString())
end

-- SkillTarget.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SkillTarget.OnPredicateFail (retail 0x00d41cd0)
function OnPredicateFail(quest, me)
end

