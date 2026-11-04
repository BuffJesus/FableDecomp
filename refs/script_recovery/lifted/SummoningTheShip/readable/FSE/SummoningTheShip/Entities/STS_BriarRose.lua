-- Readable native conversion: STS_BriarRose. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("SummoningTheShip.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local brainState

-- STS_BriarRose.Main (retail 0x00df1830)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult18, predicateResult21, scratchValue, getStateInt
    local scratchValue16, pOther, nearest, scratchValue19
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("SummonerAttacksStarted") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    scratchValue = 0
    local timerId = quest:RegisterTimer()
    getStateInt = 0
    brainState = 2
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03", "HookCoast", "HookCoast")
    RemoveNeighbours(quest, me, 1)
    scratchValue19 = 0
    nearest = nil
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateInt("CurrentAttackWave") == 1 then
            if scratchValue19 ~= 1 then
                if quest:IsActiveThreadTerminating() then break end
                nearest = quest:GetNearestWithScriptName(me, "SummonerAttacker")
                if nearest ~= nil and nearest:IsAlive() then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    quest:GiveThingBestEnemyTarget(me, nearest)
                end
                scratchValue19 = 1
            end
        elseif scratchValue19 == 1 and not (nearest ~= nil and nearest:IsAlive()) then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local summonerAttacker = quest:GetNearestWithScriptName(me, "SummonerAttacker")
            local summonerMinion = quest:GetNearestWithScriptName(me, "SummonerMinion")
            if ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(me, summonerAttacker))) <= resources:DistanceBetweenThings(me, summonerMinion) then
                if not quest:IsActiveThreadTerminating() then scratchValue16 = summonerAttacker; goto LAB_00df1b40 end
            elseif not quest:IsActiveThreadTerminating() then
                scratchValue16 = summonerMinion
                goto LAB_00df1b40
            end
            goto FLOW_past_lab_00df1b40
            ::LAB_00df1b40::
            nearest = scratchValue16
            if nearest ~= nil and nearest:IsAlive() then
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00df1b40 end
                quest:GiveThingBestEnemyTarget(me, nearest)
            end
            goto LAB_00df1b8b
            ::FLOW_past_lab_00df1b40::
            quest:DeregisterTimer(timerId)
            return
        end
        ::LAB_00df1b8b::
        if getStateInt ~= quest:GetStateInt("SummonersAlive") then
            if quest:IsActiveThreadTerminating() then break end
            local summonersAlive = quest:GetStateInt("SummonersAlive")
            if summonersAlive == 1 then
                pOther = "SUMMONER_ONE_LEFT"
                goto LAB_00df1bd5
            else
                if summonersAlive == 2 then
                    pOther = "SUMMONER_TWO_LEFT"
                    goto LAB_00df1bd5
                end
                if summonersAlive == 3 then
                    pOther = "SUMMONER_THREE_LEFT"
                end
            end
            ::LAB_00df1bd5::
            local summonersAlive2 = quest:GetStateInt("SummonersAlive")
            if 0 < summonersAlive2 and summonersAlive2 < 4 then
                if helpers.MakeBriarRoseComment(quest, me, pOther) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    getStateInt = quest:GetStateInt("SummonersAlive")
                end
            end
        end
        local getCurrentStateGroupType = me:GetCurrentStateGroupType()
        if scratchValue ~= getCurrentStateGroupType then
            if quest:IsActiveThreadTerminating() then break end
            if getCurrentStateGroupType == 2 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                helpers.MakeBriarRoseComment(quest, me, "FOLLOWING")
            elseif getCurrentStateGroupType == 1 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                helpers.MakeBriarRoseComment(quest, me, "ATTACKING")
            end
        end
        scratchValue = getCurrentStateGroupType
        if quest:GetTimer(timerId) == 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            if me:MsgIsHitBy("SummonerAttacker") then
                goto LAB_00df1d56
            else
                if me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1d56 end
                predicateResult = false
            end
            goto FLOW_past_lab_00df1d56
            ::LAB_00df1d56::
            predicateResult = true
            ::FLOW_past_lab_00df1d56::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                if helpers.MakeBriarRoseComment(quest, me, "SUMMONER_ATTACKED") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:MsgIsHitBy("SummonerMinion") then
                goto LAB_00df1e73
            else
                if me:MsgIsHitByAnySpecialAbilityFrom("SummonerMinion") and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1e73 end
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
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df1f96 end
            predicateResult21 = false
        end
        goto FLOW_past_lab_00df1f96
        ::LAB_00df1f96::
        predicateResult21 = true
        ::FLOW_past_lab_00df1f96::
        if not predicateResult21 then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            helpers.MakeBriarRoseComment(quest, me, pQuestName)
            quest:NewScriptFrame(me)
        end
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
    local pThing = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, pThing)
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
    if param1 == 0 then quest:EntityStopFollowing(me); return end
    quest:ClearThingBestEnemyTarget(me)
    local pTarget = quest:GetHero()
    quest:EntityFollowThing(me, pTarget, 1.0, true)
    do return end
    quest:EntityStopFollowing(me)
end

