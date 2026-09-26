-- Readable native conversion: CellWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CellWhisper.Main (retail 0x00f170c0)
function Main(quest, me)
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, this_00, scratchValue4, scratchValue6, movie2, meControl
    scratchValue6 = 0
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
                if not quest:NewScriptFrame(me) then goto LAB_00f17b3f end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f17b3f end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local switch1 = math.random(0, 32767) % (quest:GetStateInt("ArenaRound") - 2)
            repeat
                if switch1 == 0 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_0 = quest:GetHealth(nil --[[missing]])
                    if fret_0 <= 0.0 then break end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                    end
                    goto LAB_00f17734
                elseif switch1 == 1 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_00 = quest:GetHealth(nil --[[missing]])
                    if 0.0 < fret_00 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 2 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_01 = quest:GetHealth(nil --[[missing]])
                    if 0.0 < fret_01 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 3 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_02 = quest:GetHealth(nil --[[missing]])
                    if 0.0 < fret_02 then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 4 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_03 = quest:GetHealth(nil --[[missing]])
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
                this_00 = movie2
                goto LAB_00f17b36
                ::FLOW_past_lab_00f17734::
            until true
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            goto LAB_00f17a59
        else
            local scratchValue5 = scratchValue6
            scratchValue6 = scratchValue6 | 1
            if me:MsgIsHitByHero() then
                goto LAB_00f177f7
            else
                scratchValue4 = scratchValue5 | 3
                scratchValue6 = scratchValue4
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue4 = scratchValue5 | 7
                    scratchValue6 = scratchValue4
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f177f7 end
                end
                scratchValue = 0
            end
            goto FLOW_past_lab_00f177f7
            ::LAB_00f177f7::
            scratchValue = 1
            ::FLOW_past_lab_00f177f7::
            if scratchValue4 & 4 ~= 0 then
                scratchValue4 = scratchValue4 & 0xfffffffb
                scratchValue6 = scratchValue4
            end
            if scratchValue4 & 2 ~= 0 then
                scratchValue4 = scratchValue4 & 0xfffffffd
                scratchValue6 = scratchValue4
            end
            if scratchValue4 & 1 ~= 0 then
                scratchValue6 = scratchValue4 & 0xfffffffe
            end
            if scratchValue ~= 0 then
                resources:PrepareResource(meControl)
                while not resources:TryAcquire(meControl, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f17b3f end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00f17b3f end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    local movie = resources:StartMovie("")
                    -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_8c,*(int *)(this + 4));
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_9c);
                    local fret_04 = quest:GetHealth(nil --[[missing]])
                    if 0.0 < fret_04 then
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                                goto LAB_00f17b32
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                            goto LAB_00f17b32
                        end
                        goto FLOW_past_lab_00f17b32
                        ::LAB_00f17b32::
                        this_00 = movie
                        goto LAB_00f17b36
                        ::FLOW_past_lab_00f17b32::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    quest:EntitySetThingAsAllyOfThing(me, hero_)
                    quest:EntitySetThingAsAllyOfThing(hero_, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
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
    this_00 = movie2
    ::LAB_00f17b36::
    resources:DestroyMovie(this_00)
    ::LAB_00f17b3f::
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

