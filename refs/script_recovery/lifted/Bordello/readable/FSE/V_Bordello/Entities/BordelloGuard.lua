-- Readable native conversion: BordelloGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- BordelloGuard.Main (retail 0x00e40420)
function Main(quest, me)
    local resources = quest:RetailResources()
    local p1, this_02, movie, movie2, scriptThing, scratchValue28
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:AssignResource(resources:MemberResource("seh_Guard"), resource)
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while true do
        if me:IsTalkedToByHero() then break end
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scriptThing = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scriptThing) then
                p1 = "TEXT_QST_B13_GUARD_ATTACKED" .. helpers.GetHeroStatusTextTag(quest, me)
                local hero = quest:GetHero()
                if not me:Speak(hero, p1, 2, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a end
                    goto FLOW_past_lab_00e40e5a
                end
                ::LAB_00e40e5a::
                resources:DestroyMovie(movie2)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_past_lab_00e40e5a::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        if not quest:GetStateBool("HeroPartying") and ((not quest:GetStateBool("HeroTricking") or not helpers.IsHeroWearingBeard(quest, me)) and not quest:GetStateBool("PlayerOwned")) then
            local hero2 = quest:GetHero()
            if (quest:IsDistanceBetweenThingsUnder(me, hero2, 3.0) and (not quest:GetStateBool("CutscenePlaying"))) and not quest:GetStateBool("MagicianSleeping") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local scratchValue = "TEXT_QST_B13_GUARD_ENTRY_REFUSED" .. helpers.GetHeroStatusTextTag(quest, me)
                resources:SetString(resources:MemberStringMap("csargs"), "$DIALOGUE", scratchValue)
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                helpers.PlayCutscene(quest, me, "CS_BORDELLO_KICKEDOUT", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    ::FLOW_after_lab_00e40a6c::
    if not quest:IsActiveThreadTerminating() then
        local movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if not quest:GetStateBool("BecomeNunnery") then
            if quest:GetStateBool("HeroTricking") and helpers.IsHeroWearingBeard(quest, me) then
                if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                local scratchValue26 = resources:ScriptThing(resource)
                local fret_00 = quest:GetHealth(scratchValue26)
                if 0.0 < fret_00 then
                    local hero3 = quest:GetHero()
                    if not me:Speak(hero3, "TEXT_QST_B13_GUARD_STRANGE_LADY", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00e40673
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                    end
                    goto LAB_00e40a48
                end
                goto LAB_00e40a57
            end
            if not quest:GetStateBool("HeroPartying") then
                if helpers.IsHeroWearingBeard(quest, me) then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        resources:ReleaseResource(resource)
                        return
                    end
                    local scratchValue24 = resources:ScriptThing(resource)
                    local fret_02 = quest:GetHealth(scratchValue24)
                    if 0.0 < fret_02 then
                        local hero4 = quest:GetHero()
                        if not me:Speak(hero4, "TEXT_QST_B13_GUARD_FEMALE_NO_ENTRY", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e40673
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                        end
                        goto LAB_00e40a48
                    end
                elseif not quest:GetStateBool("PlayerOwned") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    local scratchValue23 = resources:ScriptThing(resource)
                    local fret_04 = quest:GetHealth(scratchValue23)
                    if 0.0 < fret_04 then
                        local hero5 = quest:GetHero()
                        if not me:Speak(hero5, "TEXT_QST_B13_GUARD_MALE_NO_ENTRY", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e40673
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                        end
                        goto LAB_00e40a48
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    local scratchValue27 = resources:ScriptThing(resource)
                    local fret_03 = quest:GetHealth(scratchValue27)
                    if 0.0 < fret_03 then
                        local hero6 = quest:GetHero()
                        if not me:Speak(hero6, "TEXT_QST_B13_GUARD_GREETING", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e40673
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                local scratchValue25 = resources:ScriptThing(resource)
                local fret_01 = quest:GetHealth(scratchValue25)
                if 0.0 < fret_01 then
                    local hero7 = quest:GetHero()
                    if not me:Speak(hero7, "TEXT_QST_B13_GUARD_HERO_EXHAUSTED", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00e40673
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                end
            end
            goto FLOW_past_lab_00e40a48
            ::LAB_00e40a48::
            if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
            ::FLOW_past_lab_00e40a48::
            goto LAB_00e40a57
        end
        goto FLOW_past_lab_00e40a57
        ::LAB_00e40a57::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scriptThing = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scriptThing) then
                p1 = "TEXT_QST_B13_GUARD_ATTACKED" .. helpers.GetHeroStatusTextTag(quest, me)
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, p1, 2, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a_c1 end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a_c1 end
                    goto FLOW_past_lab_00e40e5a_c1
                end
                ::LAB_00e40e5a_c1::
                resources:DestroyMovie(movie2)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_past_lab_00e40e5a_c1::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        if not quest:GetStateBool("HeroPartying") and ((not quest:GetStateBool("HeroTricking") or not helpers.IsHeroWearingBeard(quest, me)) and not quest:GetStateBool("PlayerOwned")) then
            local hero9 = quest:GetHero()
            if (quest:IsDistanceBetweenThingsUnder(me, hero9, 3.0) and (not quest:GetStateBool("CutscenePlaying"))) and not quest:GetStateBool("MagicianSleeping") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local scratchValue22 = "TEXT_QST_B13_GUARD_ENTRY_REFUSED" .. helpers.GetHeroStatusTextTag(quest, me)
                resources:SetString(resources:MemberStringMap("csargs"), "$DIALOGUE", scratchValue22)
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                helpers.PlayCutscene(quest, me, "CS_BORDELLO_KICKEDOUT", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
        end
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        goto FLOW_after_lab_00e40a6c
        ::FLOW_past_lab_00e40a57::
        if not (quest:IsActiveThreadTerminating()) then
            scratchValue28 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue28) then
                local hero10 = quest:GetHero()
                if not me:Speak(hero10, "TEXT_QST_B13_GUARD_EMPLOYED_NUNNERY", GROUP_SELECT_FIRST, false, true, false) then
                    goto LAB_00e40673
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e40673 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
            end
            goto LAB_00e40a57
        end
        ::LAB_00e40673::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        resources:ReleaseResource(resource)
        return
    end
    resources:ReleaseResource(resource)
    do return end
    resources:DestroyMovie(this_02)
    resources:ReleaseResource(resource)
end

-- BordelloGuard.Init (retail 0x00e3af30)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

-- BordelloGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BordelloGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

