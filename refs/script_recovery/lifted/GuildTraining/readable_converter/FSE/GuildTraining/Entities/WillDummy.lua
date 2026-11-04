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
                predicateResult6 = false
            else
                predicateResult6 = true
                if me:MsgIsHitByHero() then predicateResult6 = false end
            end
            if not predicateResult6 then goto LAB_00d435c1 end
            if not quest:NewScriptFrame(me) then return end
        until false
    end
    ::FLOW_past_lab_00d434fc::
    do return end
    ::LAB_00d435c1::
    if quest:IsActiveThreadTerminating() then return end
    if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        local scratchValue5 = angle + 0.25
        quest:EntitySetFacingAngle(me, scratchValue5, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle + 0.5, true)
        quest:SetMasterGameState("WillScore", quest:GetMasterGameState("WillScore") + 1)
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice = quest:GetThingWithScriptName("WillApprentice")
            local scratchValue = willApprentice ~= nil and willApprentice:IsAlive()
            local predicateResult7 = quest:IsActiveThreadTerminating()
            if scratchValue then
                if predicateResult7 then
                    return
                end
                local willApprentice2 = quest:GetThingWithScriptName("WillApprentice")
                local conversationId = quest:AddNewConversation(willApprentice2, false, false)
                local hero = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero)
                local hero2 = quest:GetHero()
                local willApprentice5 = quest:GetThingWithScriptName("WillApprentice")
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT", willApprentice5, hero2, false)
            else
                if predicateResult7 then
                    return
                end
                local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                local conversationId2 = quest:AddNewConversation(theRealGuildmaster, false, false)
                local hero3 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero3)
                local hero4 = quest:GetHero()
                local theRealGuildmaster3 = quest:GetThingWithScriptName("TheRealGuildmaster")
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT", theRealGuildmaster3, hero4, false)
            end
            quest:SetTimer(willHelpTimer, 7)
        end
        quest:EntitySetTargetable(me, false)
        quest:Pause(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillDummySpinTimer))
        quest:EntitySetFacingAngle(me, scratchValue5, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle, true)
        quest:EntitySetTargetable(me, true)
    elseif me:MsgIsHitByHero() then
        if quest:IsActiveThreadTerminating() then return end
        quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
            local scratchValue4 = willApprentice3 ~= nil and willApprentice3:IsAlive()
            local predicateResult8 = quest:IsActiveThreadTerminating()
            if scratchValue4 then
                if predicateResult8 then
                    return
                end
                local willApprentice4 = quest:GetThingWithScriptName("WillApprentice")
                local conversationId3 = quest:AddNewConversation(willApprentice4, false, false)
                local hero5 = quest:GetHero()
                quest:AddPersonToConversation(conversationId3, hero5)
                local hero6 = quest:GetHero()
                local willApprentice6 = quest:GetThingWithScriptName("WillApprentice")
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPRENTICE_WILL_TROUBLE", willApprentice6, hero6, false)
            else
                if predicateResult8 then
                    return
                end
                local theRealGuildmaster2 = quest:GetThingWithScriptName("TheRealGuildmaster")
                local conversationId4 = quest:AddNewConversation(theRealGuildmaster2, false, false)
                local hero7 = quest:GetHero()
                quest:AddPersonToConversation(conversationId4, hero7)
                local hero8 = quest:GetHero()
                local theRealGuildmaster4 = quest:GetThingWithScriptName("TheRealGuildmaster")
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_GUILDMASTER_WILL_TROUBLE", theRealGuildmaster4, hero8, false)
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

