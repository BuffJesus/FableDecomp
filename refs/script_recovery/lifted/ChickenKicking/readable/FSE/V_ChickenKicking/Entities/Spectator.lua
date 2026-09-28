-- Readable native conversion: Spectator. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local spectatorNumber

-- Spectator.Main (retail 0x00e63890)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, movie, resource
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    quest:SetWanderCentrePoint(me, me:GetPos())
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 10.0)
    quest:SetScriptingStateGroup(me, 4)
    while not quest:GetStateBool("SpectatorsUnderAttack") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if me:IsTalkedToByHero() then
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
            if unaff_EBX >> 16 == 0 then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local line = ("TEXT_QST_B17_SPECTATOR_" .. tostring(spectatorNumber)) .. "_GREETING"
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    if not me:Speak(hero, line, 0, false, true, false) then
                        ReleaseEverything(); return
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                -- TODO(native): unaff_EBX = 0x10000;
            else
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything2(); return
                end
                local line2 = ("TEXT_QST_B17_SPECTATOR_" .. tostring(spectatorNumber)) .. "_TIP"
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    if not me:Speak(hero, line2, 2, false, true, false) then
                        ReleaseEverything(); return
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        -- TODO(native): xStack_64 = xStack_64 | 1;
        if me:MsgIsHitByHero() then
            goto LAB_00e63e78
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e63e78 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e63e78
        ::LAB_00e63e78::
        predicateResult = true
        ::FLOW_past_lab_00e63e78::
            -- TODO(native): xStack_64 = CVar11 & 0xfffffffe;
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
    if quest:IsDistanceBetweenThingsUnder(me, quest:GetThingWithScriptName("Spectator1"), 1.0) then
        if quest:IsActiveThreadTerminating() then return end
        spectatorNumber = 1
        return
    else
        local predicateResult = quest:IsActiveThreadTerminating()
        if quest:IsDistanceBetweenThingsUnder(me, quest:GetThingWithScriptName("Spectator2"), 1.0) then
            if predicateResult then return end
            spectatorNumber = 2
            return
        elseif not predicateResult then
            spectatorNumber = 3
        end
    end
end

-- Spectator.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Spectator.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

