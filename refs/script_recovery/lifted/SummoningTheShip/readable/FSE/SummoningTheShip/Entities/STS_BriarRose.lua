-- Readable native conversion: STS_BriarRose. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("SummoningTheShip.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local brainState

-- STS_BriarRose.Main (retail 0x00df1830)
function Main(quest, me)
    local predicateResult, predicateResult18, predicateResult21, scratchValue, getStateInt, sequence
    local nearest, pOther, summonerAttacker, scratchValue15
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("SummonerAttacksStarted") do
        if not quest:NewScriptFrame(me) then return end
    end
    scratchValue = 0
    local timerId = quest:RegisterTimer()
    getStateInt = 0
    brainState = 2
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03", "HookCoast", "HookCoast")
    RemoveNeighbours(quest, me, 1)
    scratchValue15 = 0
    summonerAttacker = nil
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateInt("CurrentAttackWave") == 1 then
            if scratchValue15 ~= 1 then
                nearest = quest:GetNearestWithScriptName(me, "SummonerAttacker")
                summonerAttacker = nearest
                if summonerAttacker ~= nil and summonerAttacker:IsAlive() then
                    quest:GiveThingBestEnemyTarget(me, summonerAttacker)
                end
                scratchValue15 = 1
            end
            -- TODO(native): } else {
            if scratchValue15 == 1 and not (summonerAttacker ~= nil and summonerAttacker:IsAlive()) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                local summonerMinion = quest:GetNearestWithScriptName(me, "SummonerMinion")
                if (quest:GetDistanceBetweenThings(me, quest:GetNearestWithScriptName(me, "SummonerAttacker")) ^ 2) <= (quest:GetDistanceBetweenThings(me, summonerMinion) ^ 2) then
                    if not quest:IsActiveThreadTerminating() then goto LAB_00df1b40 end
                elseif not quest:IsActiveThreadTerminating() then
                    nearest = summonerMinion
                    goto LAB_00df1b40
                end
                goto FLOW_past_lab_00df1b40
                ::LAB_00df1b40::
                summonerAttacker = nearest
                if summonerAttacker ~= nil and summonerAttacker:IsAlive() then
                    quest:GiveThingBestEnemyTarget(me, summonerAttacker)
                end
                goto LAB_00df1b8b
                ::FLOW_past_lab_00df1b40::
                ::LAB_00df204e::
                quest:DeregisterTimer(timerId)
                return
            end
        end
        ::LAB_00df1b8b::
        if getStateInt ~= quest:GetStateInt("SummonersAlive") then
            if quest:IsActiveThreadTerminating() then break end
            local summonersAlive = quest:GetStateInt("SummonersAlive")
            if summonersAlive == 1 then
                goto LAB_00df1bd5
            else
                if summonersAlive == 2 then
                    goto LAB_00df1bd5
                end
                if summonersAlive == 3 then
                    goto LAB_00df1bd5
                end
            end
            goto FLOW_past_lab_00df1bd5
            ::LAB_00df1bd5::
            ::FLOW_past_lab_00df1bd5::
            -- TODO(native): iVar9 = *(*(this + 0x14) + 0x48)
    --[[unresolved native value]]
            sequence = 0 < nil and nil < 4
            if sequence then
                -- TODO(native): bVar5 = helpers.MakeBriarRoseComment(quest, me, &pOther)
    --[[unresolved native value]]
                sequence = nil
            end
            if sequence then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                getStateInt = quest:GetStateInt("SummonersAlive")
            end
        end
        local getCurrentStateGroupType = me:GetCurrentStateGroupType()
        if scratchValue ~= getCurrentStateGroupType then
            if quest:IsActiveThreadTerminating() then break end
            if getCurrentStateGroupType == 2 then
                helpers.MakeBriarRoseComment(quest, me, "FOLLOWING")
            elseif getCurrentStateGroupType == 1 then
                helpers.MakeBriarRoseComment(quest, me, "ATTACKING")
            end
        end
        scratchValue = getCurrentStateGroupType
        if quest:GetTimer(timerId) == 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            if me:MsgIsHitBy("SummonerAttacker") then
                goto LAB_00df1d56
            else
                if me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1d56 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00df1d56
            ::LAB_00df1d56::
            predicateResult = true
            ::FLOW_past_lab_00df1d56::
            if predicateResult then
                if helpers.MakeBriarRoseComment(quest, me, "SUMMONER_ATTACKED") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:MsgIsHitBy("SummonerMinion") then
                goto LAB_00df1e73
            else
                if me:MsgIsHitByAnySpecialAbilityFrom("SummonerMinion") then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1e73 end
                end
                predicateResult18 = false
            end
            goto FLOW_past_lab_00df1e73
            ::LAB_00df1e73::
            predicateResult18 = true
            ::FLOW_past_lab_00df1e73::
            if predicateResult18 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if helpers.MakeBriarRoseComment(quest, me, "MINION_ATTACKED") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    quest:SetTimer(timerId, 30)
                end
            end
        end
        if me:MsgIsHitByHero() then
            goto LAB_00df1f96
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1f96 end
            end
            predicateResult21 = false
        end
        goto FLOW_past_lab_00df1f96
        ::LAB_00df1f96::
        predicateResult21 = true
        ::FLOW_past_lab_00df1f96::
        if predicateResult21 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            helpers.MakeBriarRoseComment(quest, me, pQuestName)
        end
        quest:NewScriptFrame(me)
    end
    quest:DeregisterTimer(timerId)
end

-- STS_BriarRose.Init (retail 0x00df1760)
function Init(quest, me)
    brainState = 1
    quest:EntitySetAsRespondingToFollowAndWaitExpressions(me, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsAllowedToFollowHero(me, true)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySheatheWeapons(me, false)
    quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

-- STS_BriarRose.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- STS_BriarRose.OnPredicateFail (retail 0x00df1800)
function OnPredicateFail(quest, me)
end

-- STS_BriarRose.RemoveNeighbours (retail 0x00df2090)
-- DF2090: bsim names this body CNavQuadTreeNode::RemoveNeighbours (a homologous script member); no PDB name
function RemoveNeighbours(quest, me, param1)
    if param1 ~= 0 then
        quest:ClearThingBestEnemyTarget(me)
        quest:EntityFollowThing(me, quest:GetHero(), 1.0, true)
        return
    end
    quest:EntityStopFollowing(me)
end

