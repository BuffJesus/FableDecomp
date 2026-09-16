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
    local bVar4, iVar3, iVar5, pCVar6, r1, r2
    local alive = true
    -- TODO(native): p0 = (CScriptThing *)(this + 8);
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
        quest:EntitySetAsKillable(nil --[[missing]], p0, false)
        quest:EntitySetAsToAddToComboMultiplierWhenHit(nil --[[missing]], p0)
        quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_ORCHARD_FARM_BLOCK")
        iVar5 = quest:AddNewConversation(nil --[[missing]], p0, false)
        iVar3 = *piVar2
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar5, pCVar6)
        iVar3 = *piVar2
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar5, "TEXT_QST_051_WHISPER_ATTACK_WITH_FLOURISH_10", pCVar6, nil --[[missing]], false)
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
            iVar5 = quest:AddNewConversation(nil --[[missing]], p0, false)
            iVar3 = *piVar2
            pCVar6 = quest:GetHero()
            quest:AddPersonToConversation(iVar5, pCVar6)
            iVar3 = *piVar2
            pCVar6 = quest:GetHero()
            quest:AddLineToConversation(iVar5, "TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10", pCVar6, nil --[[missing]], false)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(nil --[[missing]], p0)
            quest:EntitySetAsKillable(nil --[[missing]], p0, false)
            quest:EntitySetAsDamageable(nil --[[missing]], p0)
            quest:DisplayQuestInfo(true)
            -- TODO(native): CStack_18._1_1_ = 0xff;
            -- TODO(native): CStack_18._3_1_ = 0xff;
            -- TODO(native): CStack_18._2_1_ = 0;
            -- TODO(native): CStack_18._0_1_ = 0;
            r1 = quest:AddQuestInfoBarHealth(nil --[[missing]], p0, "HUD_WHISPER_ICON", &CStack_18)
            quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_ATTACK_STYLE_OFARM")
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            while not bVar4 do
                r2 = quest:GetHealth(nil --[[missing]])
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
        -- TODO(native): string = "FACTION_GUARDS_ENEMY";
    else
        -- TODO(native): string = "FACTION_BANDITS";
    end
    -- TODO(native): CStack_4 = (CCharString)this;
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_4,string,-1);
    -- TODO(native): pThing = (CScriptThing *)(this + 8);
    quest:EntitySetInFaction(nil --[[missing]], nil --[[missing]])
    quest:EntitySetAsKillable(nil --[[missing]], pThing, false)
    -- TODO(native): this_00 = *(int **)(this + 4);
    local iVar1 = *this_00
    -- TODO(native): pThing2 = (**(code **)(*this_00 + 0x118))(this_00);
    quest:EntitySetAsAwareOfThing(nil --[[missing]], nil --[[missing]])
    quest:EntitySetAsDamageable(nil --[[missing]], pThing)
    quest:EntitySetAllowBossPhaseChanges(nil --[[missing]], pThing)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

