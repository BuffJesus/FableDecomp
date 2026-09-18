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

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- SkillTarget.Main (retail 0x00d41d00)
function Main(quest, me)
    local scratchValue, predicateResult, getMasterGameState, scratchValue2, scratchValue3
    local scratchValue4, scratchValue29, conversationId, scratchValue30, theGuildmaster
    local skillApprentice, theRealGuildmaster, skillApprentice2, scratchValue36, scratchValue37
    local timerId
    local dummyNumber = state:GetInt("DummyNumber")
    local hero = quest:GetHero()
    local function __cleanup_LAB_00d42ef8()
        quest:DeregisterTimer(timerId)
    end
    local function __cleanup_LAB_00d42f02()
        quest:DeregisterTimer(timerId)
    end
    scratchValue37 = me:GetAngleXY()
    if dummyNumber == 1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        state:SetInt("Speed", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummySegements))))
        scratchValue4 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_FrontDummyWorth)
    elseif dummyNumber == 2 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        state:SetInt("Speed", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummySegements))))
        scratchValue4 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MiddleDummyWorth)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d41f6f end
        state:SetInt("Speed", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummySegements))))
        scratchValue4 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RearDummyWorth)
    end
    state:SetInt("DummyWorth", math.tointeger(math.modf(scratchValue4)))
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            getMasterGameState = quest:GetMasterGameState("SkillTrainingStarted")
            while getMasterGameState ~= 1 do
                if not quest:NewScriptFrame(me) then goto LAB_00d41f6f end
                getMasterGameState = quest:GetMasterGameState("SkillTrainingStarted")
            end
            if not quest:IsActiveThreadTerminating() then
                scratchValue = quest:IsActiveThreadTerminating()
                scratchValue29 = 0
                repeat
                    if scratchValue then
                        return
                    end
                    scratchValue3 = 0
                    quest:SetMasterGameState("SkillDummyReset", false)
                    repeat
                        if not quest:NewScriptFrame(me) then return end
                        -- TODO(native): xStack_58 = f_stk_7c * (float)i_stk_16c + f_stk_bc;
                        -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_58, fVar12, false, false)
                        scratchValue29 = scratchValue29 + 1
                    until scratchValue29 == state:GetInt("Speed")
                    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
                    if quest:GetMasterGameState("MovingDummiesNeeded") == 0 then
                        if quest:IsActiveThreadTerminating() then return end
                        while true do
                            repeat
                                if not quest:NewScriptFrame(me) then return end
                            until me:MsgIsHitByHeroWithProjectileWeapon()
                            if 0.0 < fStack_b0 then break end
                            -- TODO(native): *piVar1 = *piVar1 - 1;
                        end
                        if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                        scratchValue29 = 0
                        quest:EntitySetTargetable(me, false)
                        repeat
                            if not quest:NewScriptFrame(me) then return end
                            -- TODO(native): xStack_40 = f_stk_88 * (float)i_stk_16c + f_stk_13c;
                            -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_40, fVar12, false, false)
                            scratchValue29 = scratchValue29 + 1
                        until scratchValue29 == state:GetInt("Speed")
                        if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
                        getMasterGameState = quest:GetMasterGameState("MovingDummiesNeeded")
                        while getMasterGameState ~= 1 do
                            if not quest:NewScriptFrame(me) then return end
                            getMasterGameState = quest:GetMasterGameState("MovingDummiesNeeded")
                        end
                        if quest:IsActiveThreadTerminating() then return end
                        quest:EntitySetTargetable(me, true)
                    end
                    scratchValue30 = 0
                    timerId = quest:RegisterTimer()
                    quest:SetTimer(timerId, 0)
                    scratchValue2 = 0
                    repeat
                        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                        if scratchValue3 == 0 then
                            -- TODO(native): fStack_a0 = f_stk_ac * (float)i_stk_16c + f_stk_160;
                        else
                            -- TODO(native): fStack_58 = f_stk_70 * (float)i_stk_16c + f_stk_124;
                        end
                        quest:EntityTeleportToPosition(me, pPos, scratchValue37, false, false)
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
                                        conversationId = quest:AddNewConversation(skillApprentice, false, false)
                                        quest:AddPersonToConversation(conversationId, hero)
                                        quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", skillApprentice, hero, false)
                                    end
                                else
                                    quest:PlaySoundOnThing(hero, "SND_ARROWIMPACT_02")
                                    if 0.0 <= xStack_18c then
                                        if 0.0 <= xStack_18c then
                                            if xStack_18c < 0.0 then
                                                if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + state:GetInt("DummyWorth") * 3); goto LAB_00d42874 end
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
                                    scratchValue29 = quest:GetTimer(timerId)
                                    if scratchValue29 < 1 then
                                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                                        scratchValue36 = scratchValue36 & 0x80000003
                                        scratchValue = scratchValue36 == 0
                                        if scratchValue36 < 0 then
                                            scratchValue = (scratchValue36 - 1 | 0xfffffffc) == 0xffffffff
                                        end
                                        if scratchValue then
                                            quest:SetTimer(timerId, 8)
                                            predicateResult = quest:IsActiveThreadTerminating()
                                            if quest:IsQuestActive("Q_GuildTrainingSkill") then
                                                if predicateResult then
                                                    quest:DeregisterTimer(timerId)
                                                    return
                                                end
                                                theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                conversationId = quest:AddNewConversation(theRealGuildmaster, false, false)
                                                quest:AddPersonToConversation(conversationId, hero)
                                                if 0.0 <= xStack_18c then
                                                    if 0.0 <= xStack_18c then
                                                        if 0.0 <= xStack_18c then
                                                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", theRealGuildmaster, hero, false)
                                                        else
                                                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", theRealGuildmaster, hero, false)
                                                        end
                                                    else
                                                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                        quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", theRealGuildmaster, hero, false)
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d42ef8(); return end
                                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", theRealGuildmaster, hero, false)
                                                end
                                            else
                                                if predicateResult then
                                                    quest:DeregisterTimer(timerId)
                                                    return
                                                end
                                                skillApprentice2 = quest:GetThingWithScriptName("SkillApprentice")
                                                conversationId = quest:AddNewConversation(skillApprentice2, false, false)
                                                quest:AddPersonToConversation(conversationId, hero)
                                                if 0.0 <= xStack_18c then
                                                    if 0.0 <= xStack_18c then
                                                        if 0.0 <= xStack_18c then
                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
                                                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", skillApprentice2, hero, false)
                                                        else
                                                            if quest:IsActiveThreadTerminating() then
                                                                -- LAB_00d42efe: (native jump target)
                                                                __cleanup_LAB_00d42f02(); return
                                                            end
                                                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", skillApprentice2, hero, false)
                                                        end
                                                    else
                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
                                                        quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", skillApprentice2, hero, false)
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d42efe
                                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", skillApprentice2, hero, false)
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
                        scratchValue30 = scratchValue30 + 1
                        if scratchValue30 == state:GetInt("Speed") then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            scratchValue3 = scratchValue3 == 0
                            scratchValue30 = 0
                        end
                        if quest:GetMasterGameState("SkillRepeatKnown") ~= 0 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                            if quest:GetMasterGameState("SkillRepeating") == 1 then
                                scratchValue2 = 1
                            elseif quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            quest:SetMasterGameState("SkillDummyReset", true)
                            getMasterGameState = quest:GetMasterGameState("SkillRepeatKnown")
                            while getMasterGameState ~= 0 do
                                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
                                getMasterGameState = quest:GetMasterGameState("SkillRepeatKnown")
                            end
                        end
                    until scratchValue2 ~= 0
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    quest:DeregisterTimer(timerId)
                    quest:NewScriptFrame(me)
                    scratchValue = quest:IsActiveThreadTerminating()
                until false
            end
        end
    end
    ::LAB_00d41f6f::
end

-- SkillTarget.Init (retail 0x00d41ca0)
function Init(quest, me)
    state:SetInt("DummyNumber", tonumber(me:GetDataString()))
end

-- SkillTarget.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- SkillTarget.OnPredicateFail (retail 0x00d41cd0)
function OnPredicateFail(quest, me)
end

