-- Readable native conversion: Gate1GuardInner. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- Gate1GuardInner.Main (retail 0x00d02b80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, taskRunning, movie
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d03140 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d03140 end
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateBool("Gate1Open") then
            if me:IsTalkedToByHero() then
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_0 then
                    if not me:Speak(hero, "TEXT_QST_009_BANDIT1B_ASIDE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d02f2c end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d02f2c end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
            if quest:GetStateBool("AttackedOuterGateGuards") then
                if not quest:IsActiveThreadTerminating() then
                    quest:GiveThingBestEnemyTarget(me, hero)
                    resources:PrepareResource(resource)
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                end
                break
            end
            if me:MsgIsHitByHero() then
                goto LAB_00d02eb5
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d02eb5 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00d02eb5
            ::LAB_00d02eb5::
            predicateResult = true
            ::FLOW_past_lab_00d02eb5::
            if not predicateResult then
                quest:NewScriptFrame(me)
            else
                if not quest:IsActiveThreadTerminating() then
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d030c7 end
                    me:Speak(hero, "TEXT_QST_009_BANDIT1B_ATTACKED_NEW", GROUP_SELECT_FIRST, false, true, false)
                    taskRunning = me:IsPerformingScriptTask()
                    goto LAB_00d0306b
                end
                break
            end
        else
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00d03137::
    ::LAB_00d03140::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0306b::
    if not taskRunning then goto LAB_00d03094 end
    if not quest:NewScriptFrame(me) then goto LAB_00d02f2c end
    taskRunning = me:IsPerformingScriptTask()
    goto LAB_00d0306b
    ::LAB_00d02f2c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d030ab::
    resources:DestroyMovie(movie)
    goto LAB_00d03137
    ::LAB_00d03094::
    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d030ab end
    ::LAB_00d030c7::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    quest:SetStateBool("AttackedOuterGateGuards", true)
    quest:GiveThingBestEnemyTarget(me, hero)
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    goto LAB_00d03137
end

-- Gate1GuardInner.Init (retail 0x00d02b20)
function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetStateBool("AttackedOuterGateGuards", false)
end

-- Gate1GuardInner.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Gate1GuardInner.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

