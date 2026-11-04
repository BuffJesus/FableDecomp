-- Readable native conversion: ArenaEnemy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local myCreatureType, bigCreature, healthBarIndex

-- ArenaEnemy.Main (retail 0x00f1a990)
function Main(quest, me)
    local predicateResult, predicateResult5, ctr_50, scratchValue
    local globalCrowdTimer = quest:GetStateInt("GlobalCrowdTimer")
    if not quest:NewScriptFrame(me) then return end
    myCreatureType = 0
    ctr_50 = 0
    if 0 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures") then
        repeat
            if quest:IsActiveThreadTerminating() then return end
            if me:GetDefName() ~= quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. ctr_50 .. "_CreatureType") then
                ctr_50 = ctr_50 + 1
            else
                if quest:IsActiveThreadTerminating() then return end
                myCreatureType = ctr_50
                ctr_50 = ctr_50 + 1
            end
        until not (ctr_50 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures"))
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    bigCreature = false
    if me:GetDefName() ~= "CREATURE_EARTH_TROLL_START_STANDING" then
        if me:GetDefName() ~= "CREATURE_ROCK_TROLL_START_STANDING" then
            if me:GetDefName() ~= "CREATURE_SCORPION_KING" then goto LAB_00f1acb5 end
            if quest:IsActiveThreadTerminating() then return end
            scratchValue = "HUD_QUEST_ICON_SCORPION_KING"
        else
            if quest:IsActiveThreadTerminating() then return end
            scratchValue = "HUD_QUEST_ICON_ROCK_TROLL"
        end
    else
        if quest:IsActiveThreadTerminating() then return end
        scratchValue = "HUD_QUEST_ICON_EARTH_TROLL"
    end
    healthBarIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 255, B = 0, A = 255}, scratchValue, 1.0)
    bigCreature = true
    ::LAB_00f1acb5::
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if me:MsgIsHitByHeroWithFlourish() then
            if quest:IsActiveThreadTerminating() then return end
            if not (quest:GetTimer(globalCrowdTimer) < 1) then quest:NewScriptFrame(me); goto continue_2 end
            if quest:IsActiveThreadTerminating() then return end
            local getStateString = quest:GetStateString("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3)
            local hero = quest:GetHero()
            local arenaSpawn = quest:GetNearestWithScriptName(hero, "ArenaSpawn")
            quest:PlayCriteriaSoundOnThing(arenaSpawn, getStateString)
            quest:SetTimer(globalCrowdTimer, 10)
            quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 5)
        elseif me:MsgIsHitByHeroWithDecapitate() then
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetTimer(globalCrowdTimer) < 9 then
                if quest:IsActiveThreadTerminating() then return end
                local getStateString2 = quest:GetStateString("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3)
                local hero2 = quest:GetHero()
                local arenaSpawn2 = quest:GetNearestWithScriptName(hero2, "ArenaSpawn")
                quest:PlayCriteriaSoundOnThing(arenaSpawn2, getStateString2)
                quest:SetTimer(globalCrowdTimer, 10)
                quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 7)
            end
        else
            if me:MsgIsHitByHero() then
                goto LAB_00f1af52
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1af52 end
                predicateResult = false
            end
            goto FLOW_past_lab_00f1af52
            ::LAB_00f1af52::
            predicateResult = true
            ::FLOW_past_lab_00f1af52::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 1)
                local hero3 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero3)
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
                    if quest:IsActiveThreadTerminating() then return end
                    local whisperAlly = quest:GetThingWithScriptName("WhisperAlly")
                    quest:GiveThingBestEnemyTarget(me, whisperAlly)
                end
            end
        end
        quest:NewScriptFrame(me)
        ::continue_2::
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
    quest:SetStateInt("TotalCreatures_" .. myCreatureType, quest:GetStateInt("TotalCreatures_" .. myCreatureType) - 1)
    if me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. myCreatureType .. "_DeathScore"))
    end
    if bigCreature then
        quest:RemoveQuestInfoElement(healthBarIndex)
    end
end

