-- Readable native conversion: OrchardFarmWhisper. Review coverage report before use.
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
    local conversationId, conversationId2
    while quest:GetStateBool("WhisperInCutscene") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_ORCHARD_FARM_BLOCK")
    conversationId = quest:AddNewConversation(me, false, false)
    quest:AddPersonToConversation(conversationId, quest:GetHero())
    quest:AddLineToConversation(conversationId, "TEXT_QST_051_WHISPER_ATTACK_WITH_FLOURISH_10", me, quest:GetHero(), false)
    while not me:MsgIsHitByHeroWithFlourish() do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    conversationId2 = quest:AddNewConversation(me, false, false)
    quest:AddPersonToConversation(conversationId2, quest:GetHero())
    quest:AddLineToConversation(conversationId2, "TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10", me, quest:GetHero(), false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, true)
    quest:DisplayQuestInfo(true)
    quest:AddQuestInfoBarHealth(me, {R = 0, G = 255, B = 0, A = 255}, "HUD_WHISPER_ICON", 1.0)
    quest:EntitySetCombatType(me, "HERO_WHISPER_ATTACK_STYLE_OFARM")
    while not quest:IsActiveThreadTerminating() do
        if quest:GetHealth(me) <= 1.0 then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("MissionSucceeded", true)
        end
        quest:NewScriptFrame(me)
    end
end

function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_BANDITS")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsAwareOfThing(me, quest:GetHero())
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAllowBossPhaseChanges(me, false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

