-- Readable native conversion: SummonerAttacker. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local waveID

-- SummonerAttacker.Main (retail 0x00df2380)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, p0, createCreatureNearby, createCreatureNearby2
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetAlpha(me, 0.0, true)
    quest:EntitySetInLimbo(me, true, true)
    while not quest:GetStateBool("SummonerAttacksStarted") or quest:GetStateInt("CurrentAttackWave") ~= waveID do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:EntitySetInLimbo(me, false, true)
    -- TODO(native): FadeInThing(p0,0x40000000);
    if waveID == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_SUMMONER_BLUE", 1.0)
    end
    local resource = resources:NewResource()
    createCreatureNearby = nil
    createCreatureNearby2 = nil
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while true do
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then break end
        while quest:IsDistanceBetweenThingsOver(me, hero, 15.0) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                me:SummonerLightningOrbAttackTarget(hero)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00df26bd
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df26bd end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00df26bd
            ::LAB_00df26bd::
            predicateResult = true
            ::FLOW_past_lab_00df26bd::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not (createCreatureNearby ~= nil and createCreatureNearby:IsAlive()) then
                    createCreatureNearby = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", hero:GetPos(), 15.0, "SummonerMinion")
                end
                if not (createCreatureNearby2 ~= nil and createCreatureNearby2:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    createCreatureNearby2 = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", hero:GetPos(), 15.0, "SummonerMinion")
                end
            end
        end
        if quest:IsActiveThreadTerminating() then break end
        resources:PrepareResource(resource)
        quest:MiniMapRemoveMarker(me)
        while quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then break end
        if not quest:NewScriptFrame(me) then break end
    end
    resources:ReleaseResource(resource)
end

-- SummonerAttacker.Init (retail 0x00df2310)
function Init(quest, me)
    quest:SetStateInt("SummonersAlive", quest:GetStateInt("SummonersAlive") + 1)
    local predicateResult = me:GetDataString() == "WAVE2"
    waveID = ((predicateResult and predicateResult ~= nil and predicateResult ~= 0) and 1 or 0) + 1
end

-- SummonerAttacker.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SummonerAttacker.OnPredicateFail (retail 0x00df22a0)
function OnPredicateFail(quest, me)
    quest:SetStateInt("SummonersAlive", quest:GetStateInt("SummonersAlive") - 1)
    if me:MsgIsKilledBy("") then
        quest:SetStateInt("CurrentAttackWave", quest:GetStateInt("CurrentAttackWave") + 1)
    end
end

