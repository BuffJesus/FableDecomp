-- Readable native conversion: WillDummy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- WillDummy.Main (retail 0x00d43450)
function Main(quest, me)
    local angle, predicateResult6, predicateResult8, conversationId, conversationId2
    local conversationId3, conversationId4, willApprentice, willApprentice3
    local willHelpTimer = quest:GetStateInt("WillHelpTimer")
    angle = me:GetAngleXY()
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            quest:SetTimer(willHelpTimer, 15)
            while quest:GetMasterGameState("WillTrainingStarted") ~= '\x01' do
                if not quest:NewScriptFrame(me) then return end
            end
            if not quest:IsActiveThreadTerminating() then
                if not quest:IsActiveThreadTerminating() then
                    repeat
                        if me:MsgIsHitByHeroSpecialAbility(me) then
                            -- LAB_00d43568: (native jump target)
                            predicateResult6 = false
                        else
                            predicateResult6 = not me:MsgIsHitByHero()
                        end
                        if not predicateResult6 then goto LAB_00d435c1 end
                        if not quest:NewScriptFrame(me) then return end
                    until false
                end
            end
        end
    end
    ::FLOW_after_lab_00d434fc::
    do return end
    ::LAB_00d435c1::
    if quest:IsActiveThreadTerminating() then return end
    if me:MsgIsHitByHeroSpecialAbility(me) then
        quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        -- TODO(native): xStack_94 = angle + (float)0.0;
        quest:EntitySetFacingAngle(me, xStack_94, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle + 0.0, true)
        -- TODO(native): *piVar1 = *piVar1 + 1;
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            willApprentice = quest:GetThingWithScriptName("WillApprentice")
            if willApprentice ~= nil and willApprentice:IsAlive() then
                conversationId = quest:AddNewConversation(quest:GetThingWithScriptName("WillApprentice"), false, false)
                quest:AddPersonToConversation(conversationId, quest:GetHero())
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT", quest:GetThingWithScriptName("WillApprentice"), quest:GetHero(), false)
            else
                conversationId2 = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
                quest:AddPersonToConversation(conversationId2, quest:GetHero())
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT", quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetHero(), false)
            end
            quest:SetTimer(willHelpTimer, 7)
        end
        quest:EntitySetTargetable(me, false)
        quest:Pause(quest:ReadGlobalGameDataFloat(3864))
        quest:EntitySetFacingAngle(me, xStack_94, true)
        if not quest:NewScriptFrame(me) then return end
        if not quest:NewScriptFrame(me) then return end
        quest:EntitySetFacingAngle(me, angle, true)
        quest:EntitySetTargetable(me, true)
    elseif me:MsgIsHitByHero() then
        quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
        if quest:GetTimer(willHelpTimer) < 1 then
            if quest:IsActiveThreadTerminating() then return end
            willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
            if willApprentice3 ~= nil and willApprentice3:IsAlive() then
                conversationId3 = quest:AddNewConversation(quest:GetThingWithScriptName("WillApprentice"), false, false)
                quest:AddPersonToConversation(conversationId3, quest:GetHero())
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPRENTICE_WILL_TROUBLE", quest:GetThingWithScriptName("WillApprentice"), quest:GetHero(), false)
            else
                conversationId4 = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
                quest:AddPersonToConversation(conversationId4, quest:GetHero())
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_GUILDMASTER_WILL_TROUBLE", quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetHero(), false)
            end
            quest:SetTimer(willHelpTimer, 7)
        end
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        repeat
            if me:MsgIsHitByHeroSpecialAbility(me) then
                -- LAB_00d43568_c2: (native jump target)
                predicateResult8 = false
            else
                predicateResult8 = true
                if me:MsgIsHitByHero() then return end  -- TODO(native): goto LAB_00d43568_c2
            end
            if not predicateResult8 then goto LAB_00d435c1 end
            if not quest:NewScriptFrame(me) then return end
        until false
    end
    goto FLOW_after_lab_00d434fc
end

-- WillDummy.Init (retail 0x00d43410)
function Init(quest, me)
end

-- WillDummy.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- WillDummy.OnPredicateFail (retail 0x00d43420)
function OnPredicateFail(quest, me)
end

