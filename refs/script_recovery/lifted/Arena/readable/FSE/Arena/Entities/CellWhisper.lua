-- Readable native conversion: CellWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CellWhisper.Main (retail 0x00f170c0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, center, movie2
    local cellWhisperResource = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local position = quest:GetThingWithScriptName("FlickPoint"):GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(cellWhisperResource)
            while not resources:TryAcquire(cellWhisperResource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(cellWhisperResource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
            if me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
                me:ClearCommands()
            end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local switch1 = quest:RetailRandModulo(32768) % (quest:GetStateInt("ArenaRound") - 2)
            repeat
                if switch1 == 0 then
                    local scratchValue3 = resources:ScriptThing(cellWhisperResource)
                    local fret_0 = quest:GetHealth(scratchValue3)
                    if fret_0 <= 0.0 then break end
                    local hero = quest:GetHero()
                    if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_THIRD_ROUND", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00f17af0
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                    end
                    goto LAB_00f17734
                elseif switch1 == 1 then
                    local scratchValue4 = resources:ScriptThing(cellWhisperResource)
                    local fret_00 = quest:GetHealth(scratchValue4)
                    if 0.0 < fret_00 then
                        local hero2 = quest:GetHero()
                        if not me:Speak(hero2, "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_FOURTH_ROUND", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00f17af0
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 2 then
                    local scratchValue5 = resources:ScriptThing(cellWhisperResource)
                    local fret_01 = quest:GetHealth(scratchValue5)
                    if 0.0 < fret_01 then
                        local hero3 = quest:GetHero()
                        if not me:Speak(hero3, "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_FIFTH_ROUND", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00f17af0
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 3 then
                    local scratchValue6 = resources:ScriptThing(cellWhisperResource)
                    local fret_02 = quest:GetHealth(scratchValue6)
                    if 0.0 < fret_02 then
                        local hero4 = quest:GetHero()
                        if not me:Speak(hero4, "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_SIXTH_ROUND", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00f17af0
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f17af0 end
                        end
                        goto LAB_00f17734
                    end
                    break
                elseif switch1 == 4 then
                    local scratchValue7 = resources:ScriptThing(cellWhisperResource)
                    local fret_03 = quest:GetHealth(scratchValue7)
                    if 0.0 < fret_03 then
                        local hero5 = quest:GetHero()
                        if not me:Speak(hero5, "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_SEVENTH_ROUND", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00f17af0
                        end
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
                resources:ReleaseResource(cellWhisperResource)
                do return end
                ::FLOW_past_lab_00f17734::
            until true
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            goto LAB_00f17a59
        else
            if me:MsgIsHitByHero() then
                goto LAB_00f177f7
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f177f7 end
                scratchValue = 0
            end
            goto FLOW_past_lab_00f177f7
            ::LAB_00f177f7::
            scratchValue = 1
            ::FLOW_past_lab_00f177f7::
            if scratchValue ~= 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
                resources:PrepareResource(cellWhisperResource)
                while not resources:TryAcquire(cellWhisperResource, me, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(cellWhisperResource); return end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
                if me:IsPerformingScriptTask() then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
                    me:ClearCommands()
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(cellWhisperResource); return end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue8 = resources:ScriptThing(cellWhisperResource)
                    local fret_04 = quest:GetHealth(scratchValue8)
                    if 0.0 < fret_04 then
                        local hero6 = quest:GetHero()
                        if not me:Speak(hero6, "TEXT_QST_005_WHISPER_ENEMY_BEENHITATSTART_10", GROUP_SELECT_FIRST, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                        else
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f17b32 end
                            end
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f17b32 end
                            goto FLOW_past_lab_00f17b32
                        end
                        ::LAB_00f17b32::
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(cellWhisperResource)
                        do return end
                        ::FLOW_past_lab_00f17b32::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    local hero7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, hero7)
                    local pThing1 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pThing1, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
                goto LAB_00f17a59
            end
        end
        goto FLOW_past_lab_00f17a59
        ::LAB_00f17a59::
        resources:PrepareResource(cellWhisperResource)
        ::FLOW_past_lab_00f17a59::
        quest:NewScriptFrame(me)
    end
    resources:ReleaseResource(cellWhisperResource)
    do return end
    ::LAB_00f17af0::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:ReleaseResource(cellWhisperResource)
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

