-- Readable native conversion: BordelloGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- BordelloGuard.Main (retail 0x00e40420)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue7, this_02, scratchValue24, scratchValue25, scratchValue26, movie, movie2
    scratchValue26 = 0
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
        scratchValue24 = scratchValue26
        scratchValue26 = scratchValue26 | 1
        if me:MsgIsHitByHero() then goto LAB_00e40afa end
        scratchValue25 = scratchValue24 | 3
        scratchValue26 = scratchValue25
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue25 = scratchValue24 | 7
            scratchValue26 = scratchValue25
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e40afa end
        end
        scratchValue7 = 0
        goto FLOW_past_lab_00e40afa
        ::LAB_00e40afa::
        scratchValue7 = 1
        ::FLOW_past_lab_00e40afa::
        if scratchValue25 & 4 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffb
            scratchValue26 = scratchValue25
        end
        if scratchValue25 & 2 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffd
            scratchValue26 = scratchValue25
        end
        if scratchValue25 & 1 ~= 0 then
            scratchValue26 = scratchValue25 & 0xfffffffe
        end
        if scratchValue7 ~= 0 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, a .. helpers.GetHeroStatusTextTag(quest, me), 2, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a end
                goto FLOW_past_lab_00e40e5a
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
            if (quest:IsDistanceBetweenThingsUnder(me, hero, 3.0) and (not quest:GetStateBool("CutscenePlaying"))) and not quest:GetStateBool("MagicianSleeping") then
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
                local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_00 then
                    if not me:Speak(hero, "TEXT_QST_B13_GUARD_STRANGE_LADY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
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
                    local fret_02 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_02 then
                        if not me:Speak(hero, "TEXT_QST_B13_GUARD_FEMALE_NO_ENTRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
                        goto LAB_00e40a48
                    end
                elseif not quest:GetStateBool("PlayerOwned") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    local fret_04 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_04 then
                        if not me:Speak(hero, "TEXT_QST_B13_GUARD_MALE_NO_ENTRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
                        goto LAB_00e40a48
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    local fret_03 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_03 then
                        if not me:Speak(hero, "TEXT_QST_B13_GUARD_GREETING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
                local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_01 then
                    if not me:Speak(hero, "TEXT_QST_B13_GUARD_HERO_EXHAUSTED", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
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
        scratchValue24 = scratchValue26
        scratchValue26 = scratchValue26 | 1
        if not me:MsgIsHitByHero() then
            scratchValue25 = scratchValue24 | 3
            scratchValue26 = scratchValue25
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue25 = scratchValue24 | 7
                scratchValue26 = scratchValue25
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e40afa_c1 end
            end
            scratchValue7 = 0
        end
        goto FLOW_past_lab_00e40afa_c1
        ::LAB_00e40afa_c1::
        scratchValue7 = 1
        ::FLOW_past_lab_00e40afa_c1::
        if scratchValue25 & 4 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffb
            scratchValue26 = scratchValue25
        end
        if scratchValue25 & 2 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffd
            scratchValue26 = scratchValue25
        end
        if scratchValue25 & 1 ~= 0 then
            scratchValue26 = scratchValue25 & 0xfffffffe
        end
        if scratchValue7 ~= 0 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, a .. helpers.GetHeroStatusTextTag(quest, me), 2, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a_c1 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e40e5a_c1 end
                goto FLOW_past_lab_00e40e5a_c1
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
            if (quest:IsDistanceBetweenThingsUnder(me, hero, 3.0) and (not quest:GetStateBool("CutscenePlaying"))) and not quest:GetStateBool("MagicianSleeping") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local scratchValue23 = "TEXT_QST_B13_GUARD_ENTRY_REFUSED" .. helpers.GetHeroStatusTextTag(quest, me)
                resources:SetString(resources:MemberStringMap("csargs"), "$DIALOGUE", scratchValue23)
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
        if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
            if not me:Speak(hero, "TEXT_QST_B13_GUARD_EMPLOYED_NUNNERY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e40673 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e40673 end
        end
        goto LAB_00e40a57
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

