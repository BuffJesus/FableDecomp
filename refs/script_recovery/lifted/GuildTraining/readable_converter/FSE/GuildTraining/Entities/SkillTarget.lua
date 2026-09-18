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
    local predicateResult4, scratchValue, predicateResult, scratchValue4, scratchValue5
    local scratchValue6, readGlobalGameDataFloat, getTimer, conversationId, conversationId2
    local conversationId3, conversationId4, i_stk_16c_1, pPos, theGuildmaster, skillApprentice
    local theRealGuildmaster, skillApprentice2, scratchValue42, getAngleXY, scratchValue44, timerId
    local hero = quest:GetHero()
    local function __cleanup_LAB_00d42ef8()
        quest:DeregisterTimer(timerId)
    end
    local function __cleanup_LAB_00d42f02()
        quest:DeregisterTimer(timerId)
    end
    getAngleXY = me:GetAngleXY()
    if dummyNumber == 1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummySegements)))
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummyWorth)
    elseif dummyNumber == 2 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummySegements)))
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummyWorth)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        speed = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummySegements)))
        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummyWorth)
    end
    dummyWorth = math.tointeger(math.modf(readGlobalGameDataFloat))
    if not quest:NewScriptFrame(me) then goto LAB_00d41f6f end
    if not quest:NewScriptFrame(me) then goto LAB_00d41f6f end
    while quest:GetMasterGameState("SkillTrainingStarted") ~= 1 do
        if not quest:NewScriptFrame(me) then goto LAB_00d41f6f end
    end
    predicateResult4 = false
    getTimer = 0
    repeat
        if predicateResult4 then
            return
        end
        scratchValue5 = 0
        quest:SetMasterGameState("SkillDummyReset", false)
        repeat
            if not quest:NewScriptFrame(me) then return end
            -- TODO(native): xStack_58 = f_stk_7c * (float)i_stk_16c + f_stk_bc;
            -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_58, fVar12, false, false)
            getTimer = getTimer + 1
        until getTimer == speed
        if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
        if quest:GetMasterGameState("MovingDummiesNeeded") == 0 then
            if quest:IsActiveThreadTerminating() then return end
            while true do
                repeat
                    if not quest:NewScriptFrame(me) then return end
                until me:MsgIsHitByHeroWithProjectileWeapon()
                if 0.0 < scratchValue6 then break end
                -- TODO(native): *piVar1 = *piVar1 - 1;
            end
            if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
            -- TODO(native): *piVar1 = *piVar1 + 1;
            getTimer = 0
            quest:EntitySetTargetable(me, false)
            repeat
                if not quest:NewScriptFrame(me) then return end
                -- TODO(native): xStack_40 = f_stk_88 * (float)i_stk_16c + f_stk_13c;
                -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_40, fVar12, false, false)
                getTimer = getTimer + 1
            until getTimer == speed
            if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
            while quest:GetMasterGameState("MovingDummiesNeeded") ~= 1 do
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
            if scratchValue5 ~= 0 then
                -- TODO(native): fStack_58 = f_stk_70 * (float)i_stk_16c + f_stk_124;
            end
            quest:EntityTeleportToPosition(me, pPos, getAngleXY, false, false)
            if me:MsgIsHitByHeroWithProjectileWeapon() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if quest:GetMasterGameState("MovingDummiesNeeded") == 1 then
                    if quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then
                        if quest:IsQuestActive("Q_GuildTrainingSkill") then
                            theGuildmaster = quest:GetThingWithScriptName("TheGuildmaster")
                            conversationId = quest:AddNewConversation(theGuildmaster, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HIT_OUT", theGuildmaster, hero, false)
                        else
                            skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
                            conversationId2 = quest:AddNewConversation(skillApprentice, false, false)
                            quest:AddPersonToConversation(conversationId2, hero)
                            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", skillApprentice, hero, false)
                        end
                    else
                        quest:PlaySoundOnThing(hero, "SND_ARROWIMPACT_02")
                        if 0.0 <= scratchValue44 then
                            if 0.0 <= scratchValue44 then
                                if scratchValue44 < 0.0 then
                                    if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + dummyWorth * 3); goto LAB_00d42874 end
                                    quest:DeregisterTimer(timerId)
                                    return
                                end
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            elseif quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            -- TODO(native): *piVar1 = *piVar1 + iVar5;
                        else
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            -- TODO(native): *piVar1 = *piVar1 + *(int *)(this + 0x24);
                        end
                        ::LAB_00d42874::
                        getTimer = quest:GetTimer(timerId)
                        if getTimer < 1 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            scratchValue42 = math.random(0, 32767) & 0x80000003
                            scratchValue = scratchValue42 == 0
                            if scratchValue42 < 0 then
                                scratchValue = (scratchValue42 - 1 | 0xfffffffc) == 0xffffffff
                            end
                            if scratchValue then
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                quest:SetTimer(timerId, 8)
                                predicateResult = quest:IsActiveThreadTerminating()
                                if quest:IsQuestActive("Q_GuildTrainingSkill") then
                                    if predicateResult then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                    conversationId3 = quest:AddNewConversation(theRealGuildmaster, false, false)
                                    quest:AddPersonToConversation(conversationId3, hero)
                                    if 0.0 <= scratchValue44 then
                                        if 0.0 <= scratchValue44 then
                                            if 0.0 <= scratchValue44 then
                                                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", theRealGuildmaster, hero, false)
                                            else
                                                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", theRealGuildmaster, hero, false)
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                            quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", theRealGuildmaster, hero, false)
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", theRealGuildmaster, hero, false)
                                    end
                                else
                                    if predicateResult then
                                        quest:DeregisterTimer(timerId)
                                        return
                                    end
                                    skillApprentice2 = quest:GetThingWithScriptName("SkillApprentice")
                                    conversationId4 = quest:AddNewConversation(skillApprentice2, false, false)
                                    quest:AddPersonToConversation(conversationId4, hero)
                                    if 0.0 <= scratchValue44 then
                                        if 0.0 <= scratchValue44 then
                                            if 0.0 <= scratchValue44 then
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", skillApprentice2, hero, false)
                                            else
                                                if quest:IsActiveThreadTerminating() then
                                                    -- LAB_00d42efe: (native jump target)
                                                    __cleanup_LAB_00d42f02(); return
                                                end
                                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", skillApprentice2, hero, false)
                                            end
                                        else
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
                                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", skillApprentice2, hero, false)
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
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
                scratchValue5 = scratchValue5 == 0
                i_stk_16c_1 = 0
            end
            if quest:GetMasterGameState("SkillRepeatKnown") == 0 then goto continue_1 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            if quest:GetMasterGameState("SkillRepeating") == 1 then
                scratchValue4 = 1
            elseif quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                return
            end
            quest:SetMasterGameState("SkillDummyReset", true)
            while quest:GetMasterGameState("SkillRepeatKnown") ~= 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            ::continue_1::
        until scratchValue4 ~= 0
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:DeregisterTimer(timerId)
        quest:NewScriptFrame(me)
        predicateResult4 = quest:IsActiveThreadTerminating()
    until false
    ::LAB_00d41f6f::
end

-- SkillTarget.Init (retail 0x00d41ca0)
function Init(quest, me)
    dummyNumber = tonumber(me:GetDataString())
end

-- SkillTarget.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- SkillTarget.OnPredicateFail (retail 0x00d41cd0)
function OnPredicateFail(quest, me)
end

