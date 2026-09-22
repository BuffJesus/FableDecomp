-- Readable native conversion: WillDummy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_WillDummySpinTimer = 3864,  -- 3.0
}

-- WillDummy.Main (retail 0x00d43450)
function Main(quest, me)
    local predicateResult, predicateResult6
    local willHelpTimer = quest:GetStateInt("WillHelpTimer")
    local hero = quest:GetHero()
    local angle = me:GetAngleXY()
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            quest:SetTimer(willHelpTimer, 15)
            while not quest:GetMasterGameState("WillTrainingStarted") do
                if not quest:NewScriptFrame(me) then return end
            end
            if not quest:IsActiveThreadTerminating() then predicateResult = quest:IsActiveThreadTerminating(); goto LAB_00d434fc end
        end
    end
    goto FLOW_past_lab_00d434fc
    ::LAB_00d434fc::
    if not predicateResult then
        repeat
            if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                goto LAB_00d43568
            else
                predicateResult6 = true
                if me:MsgIsHitByHero() then goto LAB_00d43568 end
            end
            goto FLOW_past_lab_00d43568
            ::LAB_00d43568::
            predicateResult6 = false
            ::FLOW_past_lab_00d43568::
            if not predicateResult6 then goto LAB_00d435c1 end
            if not quest:NewScriptFrame(me) then return end
        until false
    end
    ::FLOW_past_lab_00d434fc::
    do return end
    ::LAB_00d435c1::
    if quest:IsActiveThreadTerminating() then return end
    if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
        quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        local scratchValue = angle + 0.25
        quest:EntitySetFacingAngle(me, scratchValue, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle + 0.5, true)
        quest:SetMasterGameState("WillScore", quest:GetMasterGameState("WillScore") + 1)
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice = quest:GetThingWithScriptName("WillApprentice")
            if willApprentice ~= nil and willApprentice:IsAlive() then
                local conversationId = quest:AddNewConversation(quest:GetThingWithScriptName("WillApprentice"), false, false)
                quest:AddPersonToConversation(conversationId, hero)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT", quest:GetThingWithScriptName("WillApprentice"), hero, false)
            else
                local conversationId2 = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT", quest:GetThingWithScriptName("TheRealGuildmaster"), hero, false)
            end
            quest:SetTimer(willHelpTimer, 7)
        end
        quest:EntitySetTargetable(me, false)
        quest:Pause(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillDummySpinTimer))
        quest:EntitySetFacingAngle(me, scratchValue, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle, true)
        quest:EntitySetTargetable(me, true)
    elseif me:MsgIsHitByHero() then
        quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
            if willApprentice3 ~= nil and willApprentice3:IsAlive() then
                local conversationId3 = quest:AddNewConversation(quest:GetThingWithScriptName("WillApprentice"), false, false)
                quest:AddPersonToConversation(conversationId3, hero)
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPRENTICE_WILL_TROUBLE", quest:GetThingWithScriptName("WillApprentice"), hero, false)
            else
                local conversationId4 = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_GUILDMASTER_WILL_TROUBLE", quest:GetThingWithScriptName("TheRealGuildmaster"), hero, false)
            end
            quest:SetTimer(willHelpTimer, 7)
        end
    end
    quest:NewScriptFrame(me)
    predicateResult = quest:IsActiveThreadTerminating()
    goto LAB_00d434fc
end

-- WillDummy.Init (retail 0x00d43410)
function Init(quest, me)
end

-- WillDummy.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WillDummy.OnPredicateFail (retail 0x00d43420)
function OnPredicateFail(quest, me)
end

