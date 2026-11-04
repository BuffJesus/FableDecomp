-- Readable native conversion: Spectator. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local spectatorNumber_

-- Spectator.Main (retail 0x00e63890)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, center, native_greeting_done, p1, movie, resource
    local spectatorNumber = spectatorNumber_
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    native_greeting_done = false
    resource = resources:NewResource()
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local position = me:GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 10.0)
    p1 = nil
    quest:SetScriptingStateGroup(me, 4)
    while not quest:GetStateBool("SpectatorsUnderAttack") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not native_greeting_done then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local scratchValue20 = ("TEXT_QST_B17_SPECTATOR_" .. tostring(spectatorNumber)) .. "_GREETING"
                local scratchValue = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue) then
                    p1 = 0
                    local line = scratchValue20
                    local hero = quest:GetHero()
                    if not me:Speak(hero, line, 0, p1 ~= 0, true, false) then
                        ReleaseEverything(); return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            ReleaseEverything(); return
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                native_greeting_done = true
            else
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything2(); return
                end
                local scratchValue18 = ("TEXT_QST_B17_SPECTATOR_" .. tostring(spectatorNumber)) .. "_TIP"
                local scratchValue15 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue15) then
                    p1 = 0
                    local line2 = scratchValue18
                    local hero2 = quest:GetHero()
                    if not me:Speak(hero2, line2, 2, p1 ~= 0, true, false) then
                        ReleaseEverything(); return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            ReleaseEverything(); return
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e63e78
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e63e78 end
            predicateResult = false
        end
        goto FLOW_past_lab_00e63e78
        ::LAB_00e63e78::
        predicateResult = true
        ::FLOW_past_lab_00e63e78::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetStateBool("SpectatorsUnderAttack", true)
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySetAsScared(me, true)
    quest:SetIsPushableByHero(me, true)
    quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    resources:ReleaseResource(resource)
end

-- Spectator.Init (retail 0x00e63730)
function Init(quest, me)
    local thing = quest:GetThingWithScriptName("Spectator1")
    local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, thing, 1.0)
    thing = nil
    if isDistanceBetweenThingsUnder then
        if quest:IsActiveThreadTerminating() then return end
        spectatorNumber_ = 1
        return
    else
        thing = quest:GetThingWithScriptName("Spectator2")
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, thing, 1.0)
        thing = nil
        local predicateResult = quest:IsActiveThreadTerminating()
        if isDistanceBetweenThingsUnder then
            if predicateResult then return end
            spectatorNumber_ = 2
            return
        elseif not predicateResult then
            spectatorNumber_ = 3
        end
    end
end

-- Spectator.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Spectator.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

