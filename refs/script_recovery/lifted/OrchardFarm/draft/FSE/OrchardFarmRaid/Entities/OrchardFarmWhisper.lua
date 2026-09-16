-- Generated native draft: OrchardFarmWhisper. Review coverage report before use.
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
    local bVar4, iVar5, pCVar6, r1, r2
    local alive = true
    local cVar1 = quest:GetStateBool("WhisperInCutscene")
    while cVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("WhisperInCutscene")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:EntitySetAsKillable(me, false, false)
        quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_ORCHARD_FARM_BLOCK")
        iVar5 = quest:AddNewConversation(me, false, false)
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar5, pCVar6)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar5, "TEXT_QST_051_WHISPER_ATTACK_WITH_FLOURISH_10", me, pCVar6, false)
        bVar4 = me:MsgIsHitByHeroWithFlourish()
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = me:MsgIsHitByHeroWithFlourish()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            iVar5 = quest:AddNewConversation(me, false, false)
            pCVar6 = quest:GetHero()
            quest:AddPersonToConversation(iVar5, pCVar6)
            pCVar6 = quest:GetHero()
            quest:AddLineToConversation(iVar5, "TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10", me, pCVar6, false)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsDamageable(me, true)
            quest:DisplayQuestInfo(true)
            r1 = quest:AddQuestInfoBarHealth(me, {R = 0, G = 255, B = 0, A = 255}, "HUD_WHISPER_ICON", 1.0)
            quest:EntitySetCombatType(me, "HERO_WHISPER_ATTACK_STYLE_OFARM")
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            while not bVar4 do
                r2 = quest:GetHealth(me)
                if extraout_ST0 < _DAT_0122ded8 ~= (extraout_ST0 == _DAT_0122ded8) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:SetStateBool("MissionSucceeded", true)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
            end
        end
    end
end

function Init(quest, me)
    if quest:GetStateInt("HeroTeam") == 1 then
    else
    end
    quest:EntitySetInFaction(me, "FACTION_BANDITS")
    quest:EntitySetAsKillable(me, false, true)
    local pThing2 = quest:GetHero()
    quest:EntitySetAsAwareOfThing(me, pThing2)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAllowBossPhaseChanges(me, false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

