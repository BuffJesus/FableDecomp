-- Readable native conversion: Assassin2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- Assassin2.Main (retail 0x00d05a60)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult7, assassinsUnderAttack
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d05b79 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
    assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
    predicateResult = false
    while not assassinsUnderAttack and not quest:GetStateBool("AssassinCutsceneTriggered") do
        if not quest:NewScriptFrame(me) then goto LAB_00d05b79 end
        if me:MsgIsHitByHero() then
            goto LAB_00d05c31
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d05c31 end
            end
            predicateResult7 = false
        end
        goto FLOW_past_lab_00d05c31
        ::LAB_00d05c31::
        predicateResult7 = true
        ::FLOW_past_lab_00d05c31::
        if predicateResult7 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
            quest:SetStateBool("AssassinsUnderAttack", true)
        end
        if not me:IsTalkedToByHero() then
            assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
            if predicateResult then
                if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 >= fret_00 then quest:PauseAllNonScriptedEntities(false); assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack"); goto continue_1 end
                me:Speak(hero, "TEXT_QST_009_ASSASSIN2_REPEAT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
                quest:PauseAllNonScriptedEntities(false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_0 then
                    me:Speak(hero, "TEXT_QST_009_ASSASSIN2_INTRO", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                predicateResult = true
                quest:PauseAllNonScriptedEntities(false)
            end
            assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
    if not quest:GetStateBool("AssassinCutsceneTriggered") then quest:ClearThingHasInformation(me); goto LAB_00d05b79 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d05b79 end
    resources:PrepareResource(resource)
    quest:ClearThingHasInformation(me)
    ::LAB_00d05b79::
    resources:ReleaseResource(resource)
end

-- Assassin2.Init (retail 0x00d059e0)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:SetIsPushableByHero(me, false)
end

-- Assassin2.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Assassin2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

