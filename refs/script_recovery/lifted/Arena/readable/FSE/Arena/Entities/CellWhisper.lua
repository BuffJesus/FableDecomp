-- Readable native conversion: CellWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CellWhisper.Main (retail 0x00f170c0)
function Main(quest, me)
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue, scratchValue12, movie2, meControl
    scratchValue12 = 0
    local resource = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    quest:SetWanderCentrePoint(me, quest:GetThingWithScriptName("FlickPoint"):GetPos())
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local switch1 = math.random(0, 32767) % (quest:GetStateInt("ArenaRound") - 2)
            repeat
                if switch1 == 0 then
                    local fret_0 = quest:GetHealth(resources:ScriptThing(meControl))
                    if fret_0 <= 0.0 then break end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                    end
                    goto LAB_00f17734
                elseif switch1 == 1 then
                    local fret_00 = quest:GetHealth(resources:ScriptThing(meControl))
                    if 0.0 < fret_00 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 2 then
                    local fret_01 = quest:GetHealth(resources:ScriptThing(meControl))
                    if 0.0 < fret_01 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 3 then
                    local fret_02 = quest:GetHealth(resources:ScriptThing(meControl))
                    if 0.0 < fret_02 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 4 then
                    local fret_03 = quest:GetHealth(resources:ScriptThing(meControl))
                    if 0.0 < fret_03 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                end
                goto FLOW_past_lab_00f17734
                ::LAB_00f17734::
                if not quest:IsActiveThreadTerminating() then break end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_past_lab_00f17734::
            until true
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            goto LAB_00f17a59
        else
            local scratchValue11 = scratchValue12
            scratchValue12 = scratchValue12 | 1
            if me:MsgIsHitByHero() then
                goto LAB_00f177f7
            else
                scratchValue = scratchValue11 | 3
                scratchValue12 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = scratchValue11 | 7
                    scratchValue12 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f177f7 end
                end
                scratchValue2 = 0
            end
            goto FLOW_past_lab_00f177f7
            ::LAB_00f177f7::
            scratchValue2 = 1
            ::FLOW_past_lab_00f177f7::
            if scratchValue & 4 ~= 0 then
                scratchValue = scratchValue & 0xfffffffb
                scratchValue12 = scratchValue
            end
            if scratchValue & 2 ~= 0 then
                scratchValue = scratchValue & 0xfffffffd
                scratchValue12 = scratchValue
            end
            if scratchValue & 1 ~= 0 then
                scratchValue12 = scratchValue & 0xfffffffe
            end
            if scratchValue2 ~= 0 then
                resources:PrepareResource(meControl)
                while not resources:TryAcquire(meControl, me, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_04 = quest:GetHealth(resources:ScriptThing(meControl))
                    if 0.0 < fret_04 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f17b32 end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f17b32 end
                        goto FLOW_past_lab_00f17b32
                        ::LAB_00f17b32::
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
                        ::FLOW_past_lab_00f17b32::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    quest:EntitySetThingAsAllyOfThing(me, hero_)
                    quest:EntitySetThingAsAllyOfThing(hero_, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
                goto LAB_00f17a59
            end
        end
        goto FLOW_past_lab_00f17a59
        ::LAB_00f17a59::
        resources:PrepareResource(meControl)
        ::FLOW_past_lab_00f17a59::
        quest:NewScriptFrame(me)
    end
    do return end
    ::LAB_00f17af0::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:ReleaseResource(resource)
end

-- CellWhisper.Init (retail 0x00f17090)
function Init(quest, me)
end

-- CellWhisper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CellWhisper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

