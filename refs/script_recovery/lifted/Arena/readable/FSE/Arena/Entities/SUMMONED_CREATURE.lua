-- Readable native conversion: SUMMONED_CREATURE. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- SUMMONED_CREATURE.Main (retail 0x00f1b220)
function Main(quest, me)
    local predicateResult, getStateInt
    local globalCrowdTimer = quest:GetStateInt("GlobalCrowdTimer")
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    quest:SetStateInt("TotalCreatures_0", quest:GetStateInt("TotalCreatures_0") + 1)
    quest:SetStateInt("ExtraCreatures", quest:GetStateInt("ExtraCreatures") + 1)
    while not quest:IsActiveThreadTerminating() do
        if me:MsgIsHitByHeroWithFlourish() then
            if quest:GetTimer(globalCrowdTimer) < 1 then
                quest:PlayCriteriaSoundOnThing(quest:GetNearestWithScriptName(hero, "ArenaSpawn"), quest:GetStateString("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                quest:SetTimer(globalCrowdTimer, 10)
                getStateInt = quest:GetStateInt("NewCrowdPoints") + 5
                goto LAB_00f1b586
            end
        elseif me:MsgIsHitByHeroWithDecapitate() then
            if quest:GetTimer(globalCrowdTimer) < 9 then
                quest:PlayCriteriaSoundOnThing(quest:GetNearestWithScriptName(hero, "ArenaSpawn"), quest:GetStateString("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                quest:SetTimer(globalCrowdTimer, 10)
                getStateInt = quest:GetStateInt("NewCrowdPoints") + 7
                goto LAB_00f1b586
            end
        else
            if me:MsgIsHitByHero() then
                goto LAB_00f1b523
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1b523 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00f1b523
            ::LAB_00f1b523::
            predicateResult = true
            ::FLOW_past_lab_00f1b523::
            if predicateResult then
                getStateInt = quest:GetStateInt("NewCrowdPoints") + 2
                goto LAB_00f1b586
            end
        end
        goto FLOW_past_lab_00f1b586
        ::LAB_00f1b586::
        quest:SetStateInt("NewCrowdPoints", getStateInt)
        ::FLOW_past_lab_00f1b586::
        quest:NewScriptFrame(me)
    end
end

-- SUMMONED_CREATURE.Init (retail 0x00f1b1a0)
function Init(quest, me)
end

-- SUMMONED_CREATURE.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SUMMONED_CREATURE.OnPredicateFail (retail 0x00f1b1b0)
function OnPredicateFail(quest, me)
    quest:SetStateInt("TotalCreatures_0", quest:GetStateInt("TotalCreatures_0") - 1)
    if me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 8)
    end
end

