-- Readable native conversion: KG_Chief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- KG_Chief.Main (retail 0x00e17840)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, c_stk_85_1, c_stk_85_2, fret_0, fret_04, hero7, this_00, movie
    local x_stk_18_1, scratchValue
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetAsToAddToStatChangesWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, false, false)
    predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then break end
        if me:MsgIsHitByHero() then
            goto LAB_00e17f13
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e17f13 end
            goto LAB_00e17f21
        end
        goto FLOW_past_lab_00e17f13
        ::LAB_00e17f13::
        c_stk_85_1 = 1
        if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21 end
        ::FLOW_past_lab_00e17f13::
        goto FLOW_past_lab_00e17f21
        ::LAB_00e17f21::
        c_stk_85_1 = 0
        ::FLOW_past_lab_00e17f21::
        if c_stk_85_1 == 0 then
            quest:NewScriptFrame(me);
            predicateResult = quest:IsActiveThreadTerminating();
            goto continue_1
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        x_stk_18_1 = resources:ScriptThing(resource)
        fret_04 = quest:GetHealth(x_stk_18_1)
        if 0.0 < fret_04 then
            local hero = quest:GetHero()
            if not me:Speak(hero, "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_FIRST, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6 end
                goto FLOW_past_lab_00e181f6
            end
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
        predicateResult = quest:IsActiveThreadTerminating()
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
                    local scratchValue12 = resources:ScriptThing(resource)
                    local fret_00 = quest:GetHealth(scratchValue12)
                    if 0.0 < fret_00 then
                        local hero2 = quest:GetHero()
                        if not me:Speak(hero2, "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e181ca
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e181ca end
                        end
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
                        local scratchValue13 = resources:ScriptThing(resource)
                        local fret_01 = quest:GetHealth(scratchValue13)
                        if 0.0 < fret_01 then
                            local hero3 = quest:GetHero()
                            if not me:Speak(hero3, "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK", GROUP_SELECT_FIRST, false, true, false) then
                                goto LAB_00e181ca
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e181ca end
                            end
                            goto LAB_00e17e55
                        end
                        goto LAB_00e17e64
                    end
                    if balverineState == 6 then
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue14 = resources:ScriptThing(resource)
                            local fret_02 = quest:GetHealth(scratchValue14)
                            if 0.0 < fret_02 then
                                local hero4 = quest:GetHero()
                                if not me:Speak(hero4, "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK", GROUP_SELECT_FIRST, false, true, false) then
                                    goto LAB_00e181ca
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e181ca end
                                end
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
                    if not ((quest:GetStateInt("BalverineState") ~= 7) or quest:IsQuestActive("Q_WhiteBalverineWW")) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e181ca end
                        local scratchValue11 = resources:ScriptThing(resource)
                        local fret_03 = quest:GetHealth(scratchValue11)
                        if 0.0 < fret_03 then
                            local hero5 = quest:GetHero()
                            if not me:Speak(hero5, "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK", GROUP_SELECT_FIRST, false, true, false) then
                                goto LAB_00e181ca
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e181ca end
                            end
                            goto LAB_00e17e55
                        end
                    end
                end
            else
                goto FLOW_past_lab_00e17e64
            end
            ::LAB_00e17e64::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:PrepareResource(resource)
            if me:MsgIsHitByHero() then
                goto LAB_00e17f13_c1
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e17f13_c1 end
                goto LAB_00e17f21_c1
            end
            goto FLOW_past_lab_00e17f13_c1
            ::LAB_00e17f13_c1::
            c_stk_85_2 = 1
            if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21_c1 end
            ::FLOW_past_lab_00e17f13_c1::
            goto FLOW_past_lab_00e17f21_c1
            ::LAB_00e17f21_c1::
            c_stk_85_2 = 0
            ::FLOW_past_lab_00e17f21_c1::
            if c_stk_85_2 ~= 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local x_stk_18_2 = resources:ScriptThing(resource)
                fret_04 = quest:GetHealth(x_stk_18_2)
                if 0.0 < fret_04 then
                    local hero6 = quest:GetHero()
                    if not me:Speak(hero6, "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6_c1 end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e181f6_c1 end
                        goto FLOW_past_lab_00e181f6_c1
                    end
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
            scratchValue = resources:ScriptThing(resource)
            fret_0 = quest:GetHealth(scratchValue)
            if fret_0 <= 0.0 then goto LAB_00e17e64 end
            hero7 = quest:GetHero()
            if not me:Speak(hero7, "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK", GROUP_SELECT_FIRST, false, true, false) then
                goto LAB_00e181ca
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e181ca end
            end
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

