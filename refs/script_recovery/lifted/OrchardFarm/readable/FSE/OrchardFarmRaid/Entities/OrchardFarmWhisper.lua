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
    local predicateResult, scratchValue, predicateResult2, predicateResult3, conversationId
    local conversationId2, hero, hero2, hero3, hero4, scratchValue2, health
    local alive = true
    local whisperInCutscene = quest:GetStateBool("WhisperInCutscene")
    while whisperInCutscene do
        alive = quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            return
        end
        whisperInCutscene = quest:GetStateBool("WhisperInCutscene")
    end
    alive = not quest:IsActiveThreadTerminating()
    predicateResult = not alive
    if not predicateResult then
        quest:EntitySetAsKillable(me, false, false)
        quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_ORCHARD_FARM_BLOCK")
        conversationId = quest:AddNewConversation(me, false, false)
        hero = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero)
        hero2 = quest:GetHero()
        quest:AddLineToConversation(conversationId, "TEXT_QST_051_WHISPER_ATTACK_WITH_FLOURISH_10", me, hero2, false)
        scratchValue = me:MsgIsHitByHeroWithFlourish()
        while not scratchValue do
            alive = quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                return
            end
            scratchValue = me:MsgIsHitByHeroWithFlourish()
        end
        alive = not quest:IsActiveThreadTerminating()
        predicateResult2 = not alive
        if not predicateResult2 then
            conversationId2 = quest:AddNewConversation(me, false, false)
            hero3 = quest:GetHero()
            quest:AddPersonToConversation(conversationId2, hero3)
            hero4 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10", me, hero4, false)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsDamageable(me, true)
            quest:DisplayQuestInfo(true)
            scratchValue2 = quest:AddQuestInfoBarHealth(me, {R = 0, G = 255, B = 0, A = 255}, "HUD_WHISPER_ICON", 1.0)
            quest:EntitySetCombatType(me, "HERO_WHISPER_ATTACK_STYLE_OFARM")
            alive = not quest:IsActiveThreadTerminating()
            predicateResult3 = not alive
            while not predicateResult3 do
                health = quest:GetHealth(me)
                if extraout_ST0 < _DAT_0122ded8 ~= (extraout_ST0 == _DAT_0122ded8) then
                    if quest:IsActiveThreadTerminating() then
                        return
                    end
                    quest:SetStateBool("MissionSucceeded", true)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                predicateResult3 = not alive
            end
        end
    end
end

function Init(quest, me)
    if quest:GetStateInt("HeroTeam") == 1 then
    else
    end
    -- TODO(native): CStack_4 = (CCharString)this;
    quest:EntitySetInFaction(me, "FACTION_BANDITS")
    quest:EntitySetAsKillable(me, false, true)
    -- TODO(native): this_00 = *(int **)(this + 4);
    local pThing2 = quest:GetHero()
    quest:EntitySetAsAwareOfThing(me, pThing2)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAllowBossPhaseChanges(me, false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

