-- Readable native conversion: KG_Chief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- KG_Chief.Main (retail 0x00e17840)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult, scratchValue2, fret_0, fret_04, this_00, scratchValue12
    local scratchValue13, movie
    scratchValue13 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetAsToAddToStatChangesWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, false, false)
    predicateResult2 = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult2 then
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then break end
        local scratchValue = scratchValue13
        scratchValue13 = scratchValue13 | 2
        if me:MsgIsHitByHero() then
            goto LAB_00e17f13
        else
            scratchValue12 = scratchValue | 6
            scratchValue13 = scratchValue12
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue12 = scratchValue | 14
                scratchValue13 = scratchValue12
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e17f13 end
            end
            goto LAB_00e17f21
        end
        goto FLOW_past_lab_00e17f13
        ::LAB_00e17f13::
        scratchValue2 = 1
        if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21 end
        ::FLOW_past_lab_00e17f13::
        goto FLOW_past_lab_00e17f21
        ::LAB_00e17f21::
        scratchValue2 = 0
        ::FLOW_past_lab_00e17f21::
        if scratchValue12 & 8 ~= 0 then
            scratchValue12 = scratchValue12 & 0xfffffff7
            scratchValue13 = scratchValue12
        end
        if scratchValue12 & 4 ~= 0 then
            scratchValue12 = scratchValue12 & 0xfffffffb
            scratchValue13 = scratchValue12
        end
        if scratchValue12 & 2 ~= 0 then
            scratchValue13 = scratchValue12 & 0xfffffffd
        end
        if scratchValue2 == 0 then quest:NewScriptFrame(me); predicateResult2 = quest:IsActiveThreadTerminating(); goto continue_1 end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        fret_04 = quest:GetHealth(resources:ScriptThing(resource))
        if 0.0 < fret_04 then
            me:Speak(hero, "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6 end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6 end
            goto FLOW_past_lab_00e181f6
            ::LAB_00e181f6::
            this_00 = movie
            resources:DestroyMovie(this_00)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e181f6::
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            me:SetFriendsWithEverythingFlag(true)
            resources:PrepareResource(resource)
        else
            resources:ReleaseResource(resource)
            return
        end
        quest:NewScriptFrame(me)
        predicateResult2 = quest:IsActiveThreadTerminating()
        ::continue_1::
    end
    ::FLOW_after_lab_00e17e8f::
    if not quest:IsActiveThreadTerminating() then
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if not quest:IsActiveThreadTerminating() then
            local movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local balverineState = quest:GetStateInt("BalverineState")
            if balverineState ~= 2 then
                if balverineState == 3 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = movie3
                        resources:DestroyMovie(this_00)
                        resources:ReleaseResource(resource)
                        return
                    end
                    local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_00 then
                        if not me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e181ca end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie3
                            resources:DestroyMovie(this_00)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                else
                    if balverineState == 4 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e181ca end
                        local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_01 then
                            if not me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e181ca end
                            goto LAB_00e17e55
                        end
                        goto LAB_00e17e64
                    end
                    if balverineState == 6 then
                        if not quest:IsActiveThreadTerminating() then
                            local fret_02 = quest:GetHealth(resources:ScriptThing(resource))
                            if 0.0 < fret_02 then
                                if not me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e181ca end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie3
                                    resources:DestroyMovie(this_00)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            goto LAB_00e17e64
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = movie3
                        resources:DestroyMovie(this_00)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:GetStateInt("BalverineState") ~= 7 then goto LAB_00e17d91 end
                    scratchValue13 = scratchValue13 | 1
                    if quest:IsQuestActive("Q_WhiteBalverineWW") then goto LAB_00e17d91 end
                    predicateResult = true
                    goto FLOW_past_lab_00e17d91
                    ::LAB_00e17d91::
                    predicateResult = false
                    ::FLOW_past_lab_00e17d91::
                    if scratchValue13 & 1 ~= 0 then
                        scratchValue13 = scratchValue13 & 0xfffffffe
                    end
                    if predicateResult then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e181ca end
                        local fret_03 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_03 then
                            if not me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e181ca end
                            goto LAB_00e17e55
                        end
                    end
                end
                goto LAB_00e17e64
            end
            goto FLOW_past_lab_00e17e64
            ::LAB_00e17e64::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:PrepareResource(resource)
            scratchValue13 = scratchValue13 | 2
            if me:MsgIsHitByHero() then
                scratchValue2 = 1
                if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21_c1 end
            end
            goto FLOW_past_lab_00e17f21_c1
            ::LAB_00e17f21_c1::
            scratchValue2 = 0
            ::FLOW_past_lab_00e17f21_c1::
            if scratchValue12 & 8 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffff7
                scratchValue13 = scratchValue12
            end
            if scratchValue12 & 4 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffffb
                scratchValue13 = scratchValue12
            end
            if scratchValue12 & 2 ~= 0 then
                scratchValue13 = scratchValue12 & 0xfffffffd
            end
            if scratchValue2 ~= 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                fret_04 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_04 then
                    me:Speak(hero, "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6_c1 end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6_c1 end
                    goto FLOW_past_lab_00e181f6_c1
                    ::LAB_00e181f6_c1::
                    this_00 = movie2
                    resources:DestroyMovie(this_00)
                    resources:ReleaseResource(resource)
                    do return end
                    ::FLOW_past_lab_00e181f6_c1::
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    me:SetFriendsWithEverythingFlag(true)
                    resources:PrepareResource(resource)
                else
                    resources:ReleaseResource(resource)
                    return
                end
            end
            quest:NewScriptFrame(me)
            goto FLOW_after_lab_00e17e8f
            ::FLOW_past_lab_00e17e64::
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie3
                resources:DestroyMovie(this_00)
                resources:ReleaseResource(resource)
                return
            end
            fret_0 = quest:GetHealth(resources:ScriptThing(resource))
            if fret_0 <= 0.0 then goto LAB_00e17e64 end
            if not me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e181ca end
            ::LAB_00e17e55::
            if not quest:IsActiveThreadTerminating() then goto LAB_00e17e64 end
            ::LAB_00e181ca::
            quest:PauseAllNonScriptedEntities(false)
            this_00 = movie3
            resources:DestroyMovie(this_00)
            resources:ReleaseResource(resource)
            return
        end
    end
    resources:ReleaseResource(resource)
    do return end
    resources:DestroyMovie(this_00)
    resources:ReleaseResource(resource)
end

-- KG_Chief.Init (retail 0x00e17800)
function Init(quest, me)
end

-- KG_Chief.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- KG_Chief.OnPredicateFail (retail 0x00e17810)
function OnPredicateFail(quest, me)
end

