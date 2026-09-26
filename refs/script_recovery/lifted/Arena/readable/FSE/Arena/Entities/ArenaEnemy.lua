-- Readable native conversion: ArenaEnemy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14, myCreatureType, bigCreature, healthBarIndex

-- ArenaEnemy.Main (retail 0x00f1a990)
function Main(quest, me)
    local predicateResult, predicateResult5
    local self_0x14
    local globalCrowdTimer = quest:GetStateInt("GlobalCrowdTimer")
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    myCreatureType = 0
    -- TODO(native): if 0 < *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + *(self_0x14 + 0x98)) + 0x28 + quest:GetStateInt("ArenaRoundWave") * 0x3c) then
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    bigCreature = false
    if me:GetDefName() ~= "CREATURE_EARTH_TROLL_START_STANDING" then
        if me:GetDefName() ~= "CREATURE_ROCK_TROLL_START_STANDING" then
            if me:GetDefName() ~= "CREATURE_SCORPION_KING" then goto LAB_00f1acb5 end
            if quest:IsActiveThreadTerminating() then return end
        else
            if quest:IsActiveThreadTerminating() then return end
        end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    -- TODO(native): iVar11 = quest:AddQuestInfoBarHealth(me, &xStack_50, pcVar13, 1.0)
    --[[unresolved native value]]
    healthBarIndex = nil
    bigCreature = true
    ::LAB_00f1acb5::
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if me:MsgIsHitByHeroWithFlourish() then
            if quest:GetTimer(globalCrowdTimer) >= 1 then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then return end
            quest:PlayCriteriaSoundOnThing(quest:GetNearestWithScriptName(hero, "ArenaSpawn"), self0X14 + 84 + quest:GetStateInt("NewCrowdBaseLevel") * 20)
            quest:SetTimer(globalCrowdTimer, 10)
            quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 5)
        elseif me:MsgIsHitByHeroWithDecapitate() then
            if quest:GetTimer(globalCrowdTimer) < 9 then
                quest:PlayCriteriaSoundOnThing(quest:GetNearestWithScriptName(hero, "ArenaSpawn"), self0X14 + 84 + quest:GetStateInt("NewCrowdBaseLevel") * 20)
                quest:SetTimer(globalCrowdTimer, 10)
                quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 7)
            end
        else
            if me:MsgIsHitByHero() then
                goto LAB_00f1af52
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1af52 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00f1af52
            ::LAB_00f1af52::
            predicateResult = true
            ::FLOW_past_lab_00f1af52::
            if predicateResult then
                quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 1)
                quest:GiveThingBestEnemyTarget(me, hero)
            else
                if me:MsgIsHitBy("WhisperAlly") then
                    goto LAB_00f1b041
                else
                    predicateResult5 = false
                    if me:MsgIsHitByAnySpecialAbilityFrom("WhisperAlly") then goto LAB_00f1b041 end
                end
                goto FLOW_past_lab_00f1b041
                ::LAB_00f1b041::
                predicateResult5 = true
                ::FLOW_past_lab_00f1b041::
                if predicateResult5 then
                    quest:GiveThingBestEnemyTarget(me, quest:GetThingWithScriptName("WhisperAlly"))
                end
            end
        end
        quest:NewScriptFrame(me)
        ::continue_1::
    until false
end

-- ArenaEnemy.Init (retail 0x00f1a8c0)
function Init(quest, me)
end

-- ArenaEnemy.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ArenaEnemy.OnPredicateFail (retail 0x00f1a8f0)
function OnPredicateFail(quest, me)
    -- TODO(native): *piVar1 = *piVar1 - 1;
    if me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        -- TODO(native): quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + *(*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + *(self0X14 + 0x98)) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x34 + myCreatureType * 0x38))
    end
    if bigCreature then
        quest:RemoveQuestInfoElement(healthBarIndex)
    end
end

