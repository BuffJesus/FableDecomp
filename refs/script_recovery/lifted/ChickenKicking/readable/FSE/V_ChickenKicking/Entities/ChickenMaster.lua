-- Readable native conversion: ChickenMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    CHK_LowPrize = 4008,  -- 50.0
    CHK_MidPrize = 4012,  -- 150.0
    CHK_HighPrize = 4016,  -- 250.0
}

local helpers = require("V_ChickenKicking.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local ghostChat, haveTalked, heroHasPlayed, kickingChickens, self0X18

-- ChickenMaster.Main (retail 0x00e64fb0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, C_stk_370_1, isActiveThreadTerminating, predicateResult, scratchValue12
    local scratchValue13, scratchValue15, questionAnswer, questionAnswer2, scratchValue18
    local conversationId, sequence1, sequence, sequence3, thing, line, mkCkOrg, this_00
    local scratchValue29, movie3, movie4, movie5, movie6
    C_stk_370_1 = 0
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:AssignResource(resources:MemberResource("seh_ChickenMaster"), resource)
    quest:EntitySetAppearanceMorphSeed(me, 4)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, true)
    local position = me:GetPos()
    local vec_188 = {x = position.x, y = position.y, z = position.z}
    local timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 0)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    scratchValue12 = 0
    if quest:GetStateBool("KnowGhostHasGone") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource)
            return
        end
        quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("MK_CK_ORG"), false)
    end
    while not quest:GetStateBool("KnowGhostHasGone") do
        if not quest:NewScriptFrame(me) then goto LAB_00e68af4 end
        if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and (me ~= nil and me:IsDistanceFromPositionUnder(vec_188, 6.0))) and quest:GetTimer(timerId3) < 1 then
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            if not ghostChat then
                if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
                quest:AddLineToConversation(conversationId, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_PRECHAT_10", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
                quest:AddLineToConversation(conversationId, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_POSTCHAT", me, hero, false)
            end
            quest:SetTimer(timerId3, 8)
            while (0 < quest:GetTimer(timerId3) and (scratchValue12 == 0)) and not quest:GetStateBool("TalkedTo") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource)
                    return
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId3)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:SetStateBool("TalkedTo", true)
                end
                -- TODO(native): ctr_CVar19 = xStack_374;
                -- TODO(native): xStack_374 = xStack_374 | 1;
                if me:MsgIsHitByHero() then goto LAB_00e654e0 end
                scratchValue = ctr_CVar19 | 3
                C_stk_370_1 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = ctr_CVar19 | 7
                    C_stk_370_1 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e654e0 end
                end
                scratchValue13 = 0
                goto FLOW_past_lab_00e654e0
                ::LAB_00e654e0::
                scratchValue13 = 1
                ::FLOW_past_lab_00e654e0::
                if scratchValue & 4 ~= 0 then
                    scratchValue = scratchValue & 0xfffffffb
                    C_stk_370_1 = scratchValue
                end
                if scratchValue & 2 ~= 0 then
                    scratchValue = scratchValue & 0xfffffffd
                    C_stk_370_1 = scratchValue
                end
                if scratchValue & 1 ~= 0 then
                    C_stk_370_1 = scratchValue & 0xfffffffe
                end
                if scratchValue13 ~= 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource)
                        return
                    end
                    scratchValue12 = 1
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
            quest:SetTimer(timerId3, 15)
        end
        -- TODO(native): ctr_CVar19 = C_stk_370;
        scratchValue29 = C_stk_370_1 | 8
        if me:MsgIsHitByHero() then goto LAB_00e65617 end
        scratchValue29 = ctr_CVar19 | 24
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue29 = ctr_CVar19 | 56
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e65617 end
        end
        scratchValue13 = 0
        if scratchValue12 ~= 0 then goto LAB_00e65617 end
        goto FLOW_past_lab_00e65617
        ::LAB_00e65617::
        scratchValue13 = 1
        ::FLOW_past_lab_00e65617::
        if scratchValue29 & 32 ~= 0 then
            scratchValue29 = scratchValue29 & 0xffffffdf
        end
        if scratchValue29 & 16 ~= 0 then
            scratchValue29 = scratchValue29 & 0xffffffef
        end
        if scratchValue29 & 8 ~= 0 then
            scratchValue29 = scratchValue29 & 0xfffffff7
        end
        if scratchValue13 ~= 0 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then goto LAB_00e68af4 end
            scratchValue12 = isActiveThreadTerminating
            movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            helpers.helper_E68B20(quest, me, "CS_CHICKING_HITGUYTOP")
            quest:SetStateBool("RanOff", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie5)
        end
        sequence1 = me:IsTalkedToByHero()
        if not sequence1 then
            scratchValue13 = 0
            sequence1 = quest:GetStateBool("TalkedTo")
        end
        if sequence1 then
            scratchValue13 = 1
        end
        C_stk_370_1 = scratchValue29 & 0xffffffbf
        if scratchValue13 == 0 then goto continue_3 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
        quest:SetStateBool("TalkedTo", false)
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_CHICKEN_KICKING", quest:GetActiveQuestName(), false)
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_01", "", "")
        if not ghostChat then
            if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
            ghostChat = true
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            thing = quest:GetThingWithScriptName("GhostFisherman")
            predicateResult = quest:IsActiveThreadTerminating()
            if thing ~= nil and thing:IsAlive() then
                if predicateResult then
                    goto LAB_00e66150
                end
                goto FLOW_past_lab_00e66150
                ::LAB_00e66150::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00e68af4
                ::FLOW_past_lab_00e66150::
                local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_0 then
                    me:Speak(hero, "TEXT_QST_B17_MASTER_INITIAL_MEETING", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e66150 end
                end
            else
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e68af4
                end
                helpers.helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK1")
                quest:SetStateBool("KnowGhostHasGone", true)
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
            end
            quest:PauseAllNonScriptedEntities(false)
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
            movie6 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            thing = quest:GetThingWithScriptName("GhostFisherman")
            predicateResult = quest:IsActiveThreadTerminating()
            if thing ~= nil and thing:IsAlive() then
                if predicateResult then
                    goto LAB_00e661e4
                end
                goto FLOW_past_lab_00e661e4
                ::LAB_00e661e4::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                goto LAB_00e68af4
                ::FLOW_past_lab_00e661e4::
                local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_00 then
                    me:Speak(hero, "TEXT_QST_B17_MASTER_RETURN_MEETING", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e661e4 end
                end
            else
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    goto LAB_00e68af4
                end
                helpers.helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK2")
                quest:SetStateBool("KnowGhostHasGone", true)
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
            end
            quest:PauseAllNonScriptedEntities(false)
        end
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e68af4 end
    if not quest:NewScriptFrame(me) then goto LAB_00e68af4 end
    mkCkOrg = quest:GetThingWithScriptName("MK_CK_ORG")
    isActiveThreadTerminating = false
    while not isActiveThreadTerminating do
        if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:IsDistanceBetweenThingsUnder(mkCkOrg, me, 6.0)) and quest:GetTimer(timerId3) < 1 then
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_B17_MASTER_GAME_ASIDE", me, hero, false)
            quest:SetTimer(timerId3, 8)
            while (0 < quest:GetTimer(timerId3) and (not scratchValue12)) and not quest:GetStateBool("TalkedTo") do
                if not quest:NewScriptFrame(me) then goto LAB_00e68aeb end
                if me:IsTalkedToByHero() then
                    quest:SetStateBool("TalkedTo", true)
                end
                -- TODO(native): ctr_CVar19 = C_stk_370;
                C_stk_370_1 = C_stk_370_1 | 128
                if me:MsgIsHitByHero() then goto LAB_00e661fd end
                scratchValue = ctr_CVar19 | 384
                C_stk_370_1 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = ctr_CVar19 | 896
                    C_stk_370_1 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e661fd end
                end
                scratchValue13 = 0
                goto FLOW_past_lab_00e661fd
                ::LAB_00e661fd::
                scratchValue13 = 1
                ::FLOW_past_lab_00e661fd::
                if scratchValue & 512 ~= 0 then
                    scratchValue = scratchValue & 0xfffffdff
                    C_stk_370_1 = scratchValue
                end
                if scratchValue & 256 ~= 0 then
                    scratchValue = scratchValue & 0xfffffeff
                    C_stk_370_1 = scratchValue
                end
                if scratchValue & 128 ~= 0 then
                    C_stk_370_1 = scratchValue & 0xffffff7f
                end
                if scratchValue13 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
                    scratchValue12 = 1
                end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:SetTimer(timerId3, 15)
        end
        -- TODO(native): ctr_CVar19 = C_stk_370;
        scratchValue29 = C_stk_370_1 | 1024
        if me:MsgIsHitByHero() then goto LAB_00e6634f end
        scratchValue29 = ctr_CVar19 | 3072
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue29 = ctr_CVar19 | 0x1c00
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e6634f end
        end
        sequence = scratchValue12 ~= 0
        if not sequence then
            scratchValue13 = 0
            sequence = quest:GetStateBool("SpectatorsUnderAttack")
        end
        if sequence then goto LAB_00e6634f end
        goto FLOW_past_lab_00e6634f
        ::LAB_00e6634f::
        scratchValue13 = 1
        ::FLOW_past_lab_00e6634f::
        if scratchValue29 & 4096 ~= 0 then
            scratchValue29 = scratchValue29 & 0xffffefff
        end
        if scratchValue29 & 2048 ~= 0 then
            scratchValue29 = scratchValue29 & 0xfffff7ff
        end
        if scratchValue29 & 1024 ~= 0 then
            scratchValue29 = scratchValue29 & 0xfffffbff
        end
        if scratchValue13 ~= 0 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then break end
            quest:SetStateBool("SpectatorsUnderAttack", false)
            scratchValue12 = isActiveThreadTerminating
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            helpers.helper_E68B20(quest, me, "CS_CHICKING_HITGUYBOTTOM")
            quest:SetStateBool("RanOff", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        sequence3 = me:IsTalkedToByHero()
        if not sequence3 then
            scratchValue13 = 0
            sequence3 = quest:GetStateBool("TalkedTo")
        end
        if sequence3 then
            scratchValue13 = 1
        end
        C_stk_370_1 = scratchValue29 & 0xffffdfff
        if scratchValue13 ~= 0 then
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("TalkedTo", false)
            if haveTalked then
                movie5 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                if fret_01 <= 0.0 then
                    goto LAB_00e665da
                else
                    if not me:Speak(hero, "TEXT_QST_B17_MASTER_GREETING_RETURN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e667c9 end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e665da end
                end
                goto FLOW_past_lab_00e665da
                ::LAB_00e665da::
                quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e667c9 end
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if not quest:IsActiveThreadTerminating() then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if not isActiveThreadTerminating then
                            if quest:GetHeroGold() < 50 then
                                if not quest:IsActiveThreadTerminating() then
                                    local fret_04 = quest:GetHealth(resources:ScriptThing(resource))
                                    if 0.0 < fret_04 then
                                        if not me:Speak(hero, "TEXT_QST_B17_MASTER_NO_MONEY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e667c9 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e66c23 end
                                    end
                                    goto LAB_00e66ceb
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                if not heroHasPlayed then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e66c23 end
                                    local fret_03 = quest:GetHealth(resources:ScriptThing(resource))
                                    if 0.0 < fret_03 then
                                        if not me:Speak(hero, "TEXT_QST_B17_MASTER_START_KICKING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e667c9 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e66c23 end
                                    end
                                    heroHasPlayed = true
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    local fret_02 = quest:GetHealth(resources:ScriptThing(resource))
                                    if 0.0 < fret_02 then
                                        if not me:Speak(hero, "TEXT_QST_B17_MASTER_START_KICKING_RETURN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e66c23 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    end
                                end
                                quest:FadeScreenOut(0.5, 0.5)
                                if quest:GetStateBool("KnowGhostHasGone") and not quest:GetStateBool("SpectatorsCreated") then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator1"):GetPos(), "Spectator")
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator2"):GetPos(), "Spectator")
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator3"):GetPos(), "Spectator")
                                    quest:SetStateBool("SpectatorsCreated", true)
                                end
                                quest:ConfiscateAllHeroWeapons()
                                quest:SetHeroWillAsUsable(false)
                                quest:SetTeleportingAsActive(false)
                                quest:GiveHeroGold(-50)
                                kickingChickens = true
                                goto LAB_00e66cef
                            end
                            goto LAB_00e66c23
                        end
                    elseif not isActiveThreadTerminating then
                        local fret_05 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_05 then
                            if not me:Speak(hero, "TEXT_QST_B17_MASTER_DONT_KICK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e66c23 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                        end
                        goto LAB_00e66ceb
                    end
                    goto FLOW_past_lab_00e66ceb
                    ::LAB_00e66ceb::
                    kickingChickens = false
                    ::LAB_00e66cef::
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie5
                    goto LAB_00e6762e
                    ::FLOW_past_lab_00e66ceb::
                    goto LAB_00e667c9
                end
                goto FLOW_past_lab_00e667c9
                ::LAB_00e667c9::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                break
                ::FLOW_past_lab_00e667c9::
                ::FLOW_past_lab_00e665da::
                ::LAB_00e66c23::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                break
            end
            movie4 = resources:StartMovie("")
            -- TODO(native): ctr_CVar19 = *(int **)(this + 4);
            -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,true);
            haveTalked = true
            if not me:IsPerformingScriptTask() or not quest:IsDistanceBetweenThingsOver(mkCkOrg, me, 6.0) then
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeScreenOut(0.5, 0.5)
                    if quest:GetStateBool("KnowGhostHasGone") and not quest:GetStateBool("SpectatorsCreated") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e68a59 end
                        goto FLOW_hoist_lab_00e68a59_1
                    end
                    goto FLOW_hoist_lab_00e68a59_2
                end
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                local fret_06 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_06 then
                    if not me:Speak(hero, "TEXT_QST_B17_MASTER_QUICK_WALK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e68a59 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68a3c end
                end
                quest:FadeScreenOut(0.5, 0.5)
                quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("MK_CK_ORG"), false)
                if not quest:GetStateBool("KnowGhostHasGone") or quest:GetStateBool("SpectatorsCreated") then
                    goto LAB_00e6743e
                else
                    if not quest:IsActiveThreadTerminating() then
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator1"):GetPos(), "Spectator")
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator2"):GetPos(), "Spectator")
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator3"):GetPos(), "Spectator")
                        quest:SetStateBool("SpectatorsCreated", true)
                        goto LAB_00e6743e
                    end
                    goto LAB_00e68a8f
                end
                goto FLOW_hoist_lab_00e68a8f_1
            end
            goto FLOW_past_lab_00e68a8f
            ::LAB_00e68a8f::
            -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
            resources:DestroyMovie(movie4)
            ::FLOW_hoist_lab_00e68a8f_1::
            goto FLOW_hoist_lab_00e6743e_1
            ::FLOW_past_lab_00e68a8f::
            goto FLOW_past_lab_00e68a59
            ::LAB_00e68a59::
            -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,false);
            resources:DestroyMovie(movie4)
            break
            ::FLOW_hoist_lab_00e68a59_1::
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator1"):GetPos(), "Spectator")
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator2"):GetPos(), "Spectator")
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", quest:GetThingWithScriptName("Spectator3"):GetPos(), "Spectator")
            quest:SetStateBool("SpectatorsCreated", true)
            ::FLOW_hoist_lab_00e68a59_2::
            quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("MK_CK_ORG"), false)
            goto LAB_00e6743e
            ::FLOW_past_lab_00e68a59::
            goto FLOW_past_lab_00e6743e
            ::LAB_00e6743e::
            helpers.helper_E68B20(quest, me, "CS_CHICKING_INTRO")
            quest:FadeScreenIn()
            quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer2 < 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e68a73 end
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e68a8f end
            if questionAnswer2 == 1 then
                if 49 < quest:GetHeroGold() then
                    helpers.helper_E68B20(quest, me, "CS_CHICKING_START")
                    heroHasPlayed = true
                    quest:ConfiscateAllHeroWeapons()
                    quest:SetHeroWillAsUsable(false)
                    quest:SetTeleportingAsActive(false)
                    quest:GiveHeroGold(-50)
                    kickingChickens = true
                    goto LAB_00e67619
                end
                if not quest:IsActiveThreadTerminating() then line = "CS_CHICKING_NOCASH"; goto LAB_00e6760c end
                goto LAB_00e68a73
            else
                line = "CS_CHICKING_NOKICK"
                goto LAB_00e6760c
            end
            goto FLOW_past_lab_00e6760c
            ::LAB_00e6760c::
            helpers.helper_E68B20(quest, me, line)
            goto LAB_00e67619
            ::FLOW_past_lab_00e6760c::
            goto FLOW_past_lab_00e67619
            ::LAB_00e67619::
            -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
            this_00 = movie4
            goto LAB_00e6762e
            ::FLOW_past_lab_00e67619::
            goto FLOW_past_lab_00e68a73
            ::LAB_00e68a73::
            -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
            resources:DestroyMovie(movie4)
            ::FLOW_past_lab_00e68a73::
            ::FLOW_hoist_lab_00e6743e_1::
            goto FLOW_hoist_lab_00e6762e_2
            ::FLOW_past_lab_00e6743e::
            goto FLOW_past_lab_00e6762e
            ::LAB_00e6762e::
            resources:DestroyMovie(this_00)
            if not kickingChickens then goto LAB_00e68a20 end
            if not quest:IsActiveThreadTerminating() then
                local infoCounter3 = quest:AddQuestInfoCounter("HUD_ICON_CHICKEN_BROWN", 5, 1.0)
                local infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                local infoCounter4 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("MaxChickenKickingScore"), -1)
                quest:DisplayQuestInfo(true)
                quest:Pause(1.0)
                quest:EntityTeleportToThing(hero, hero, false)
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                scratchValue18 = 0
                -- TODO(native): ctr_CVar19 = 0;
                repeat
                    if not quest:NewScriptFrame(me) then goto LAB_00e68ae2 end
                    local kickingChicken01 = quest:CreateCreature("CREATURE_KICKING_CHICKEN_01", quest:GetThingWithScriptName("CK_ChickenStart"):GetPos(), "KickedChicken")
                    quest:SetPlayerCreatureOnlyTarget(kickingChicken01)
                    quest:SetIsPushableByHero(kickingChicken01, false)
                    quest:UpdateQuestInfoCounter(infoCounter4, ctr_CVar19, -1)
                    quest:UpdateQuestInfoCounter(infoCounter3, scratchValue18, -1)
                    quest:FadeScreenIn()
                    if not quest:GetStateBool("InfoDisplayed") then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_10")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_20")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_30")
                                    while not quest:MsgIsGameInfoClickedPast() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                                    end
                                    if not quest:IsActiveThreadTerminating() then quest:SetStateBool("InfoDisplayed", true); goto LAB_00e67a8c end
                                end
                            end
                        end
                        goto LAB_00e68ace
                    end
                    goto FLOW_past_lab_00e68ace
                    ::LAB_00e68ace::
                    goto LAB_00e68ae2
                    ::FLOW_past_lab_00e68ace::
                    ::LAB_00e67a8c::
                    quest:SetStateBool("ChickenLanded", false)
                    while not quest:GetStateBool("ChickenLanded") do
                        if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ace end
                    movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): CCharString::operator=(&xStack_398,"CS_CHICKING_LANDED");
                    conversationId = quest:GetStateInt("DistanceBand")
                    if conversationId == 4 then
                        if not quest:IsActiveThreadTerminating() then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c4);
                            goto LAB_00e6804a
                        end
                        goto LAB_00e68aab
                    end
                    if conversationId == 0 then
                        if not quest:IsActiveThreadTerminating() then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c0);
                            goto LAB_00e6805d
                        end
                        goto LAB_00e68ab9
                    end
                    goto FLOW_past_lab_00e68ab9
                    ::LAB_00e68ab9::
                    quest:PauseAllNonScriptedEntities(false)
                    ::LAB_00e68ac5::
                    resources:DestroyMovie(movie6)
                    goto LAB_00e68ace
                    ::FLOW_past_lab_00e68ab9::
                    if conversationId ~= -1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                        conversationId = quest:GetStateInt("FinalSector")
                        if conversationId == 1 then
                            if not quest:IsActiveThreadTerminating() then
                                conversationId = quest:GetStateInt("DistanceBand")
                                if conversationId == 3 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_240);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x64;
                                elseif conversationId == 2 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1e8);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x32;
                                elseif conversationId == 1 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_238);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                                end
                                goto LAB_00e6805d
                            end
                        elseif conversationId == 2 then
                            if not quest:IsActiveThreadTerminating() then
                                conversationId = quest:GetStateInt("DistanceBand")
                                if conversationId == 3 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1a8);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0xa;
                                elseif conversationId == 2 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2b8);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                                elseif conversationId == 1 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),(CCharString *)xStack_1e0);
                                    -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0xa;
                                end
                                goto LAB_00e6805d
                            end
                        else
                            if conversationId ~= 3 then
                                if conversationId ~= 0 then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_218);
                                end
                                goto LAB_00e6804a
                            end
                            goto FLOW_hoist_lab_00e6804a_1
                        end
                        goto FLOW_hoist_lab_00e6804a_2
                    end
                    goto FLOW_past_lab_00e6804a
                    ::LAB_00e6804a::
                    line = "CS_CHICKING_MISSED"
                    goto LAB_00e68054
                    ::FLOW_hoist_lab_00e6804a_1::
                    if not quest:IsActiveThreadTerminating() then
                        conversationId = quest:GetStateInt("DistanceBand")
                        if conversationId == 3 then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_228);
                            -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x64;
                        elseif conversationId == 2 then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1b8);
                            -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x32;
                        elseif conversationId == 1 then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_220);
                            -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                        end
                        goto LAB_00e6805d
                    end
                    ::FLOW_hoist_lab_00e6804a_2::
                    ::LAB_00e68aab::
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00e68ac5
                    ::FLOW_past_lab_00e6804a::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68aab end
                    line = "CS_CHICKING_FOWL"
                    ::LAB_00e68054::
                    ::LAB_00e6805d::
                    -- TODO(native): CCharString::CCharString(&xStack_398_2,&xStack_398);
                    -- TODO(native): Game_InitializeArena(*(undefined4 *)(this + 0x14));
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    thing = quest:GetThingWithScriptName("KickedChicken")
                    quest:RemoveThing(thing, false, true)
                    scratchValue18 = scratchValue18 + 1
                    quest:ResetPlayerCreatureOnlyTarget()
                until scratchValue18 >= 5
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveQuestInfoElement(infoCounter4)
                    quest:RemoveQuestInfoElement(infoCounter)
                    quest:RemoveQuestInfoElement(infoCounter3)
                    quest:ReturnAllConfiscatedItemsToHero()
                    quest:SetHeroWillAsUsable(true)
                    quest:SetTeleportingAsActive(true)
                    -- TODO(native): CCharString__SetFromFormatV();
                    -- TODO(native): CWideString::CWideString(xStack_208,(int)&xStack_354);
                    -- TODO(native): Vector_PushBack(xStack_360,(int)xStack_208);
                    -- TODO(native): GetFormattedString is not a ForgeFSE binding
                    quest:GetFormattedString("TEXT_QST_B17_SCORE", thing)
                    -- TODO(native): CWideString::operator=((CWideString *)&DAT_0143e908,(int)p0);
                    if scratchValue15 < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_HighPrize) then
                        if scratchValue15 < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_MidPrize) then
                            if scratchValue15 < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_LowPrize) then
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_274);
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c8);
                                    if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                        if not quest:IsActiveThreadTerminating() then
                                            -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c4);
                                            goto LAB_00e6894f
                                        end
                                    elseif not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c0);
                                        goto LAB_00e6894f
                                    end
                                    goto FLOW_past_lab_00e6894f
                                    ::LAB_00e6894f::
                                    goto LAB_00e68954
                                    ::FLOW_past_lab_00e6894f::
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2a4);
                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_29c);
                                if quest:GetStateInt("PrizesWon") & 1 == 0 then
                                    if not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_284);
                                        -- TODO(native): *puVar1 = *puVar1 | 1;
                                        goto LAB_00e687e4
                                    end
                                elseif not quest:IsActiveThreadTerminating() then
                                    if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                        if not quest:IsActiveThreadTerminating() then
                                            -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_28c);
                                            goto LAB_00e687e4
                                        end
                                    elseif not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_294);
                                        goto LAB_00e687e4
                                    end
                                end
                                goto FLOW_past_lab_00e687e4
                                ::LAB_00e687e4::
                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_27c);
                                goto LAB_00e68954
                                ::FLOW_past_lab_00e687e4::
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_24c);
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_244);
                            if quest:GetStateInt("PrizesWon") & 2 == 0 then
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2b4);
                                    -- TODO(native): *puVar1 = *puVar1 | 2;
                                    goto LAB_00e685d7
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                    if not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),xStack_234);
                                        goto LAB_00e685d7
                                    end
                                elseif not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_23c);
                                    goto LAB_00e685d7
                                end
                            end
                            goto FLOW_past_lab_00e685d7
                            ::LAB_00e685d7::
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2ac);
                            goto LAB_00e68954
                            ::FLOW_past_lab_00e685d7::
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_200);
                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c8);
                        if quest:GetStateInt("PrizesWon") & 4 == 0 then
                            if not quest:IsActiveThreadTerminating() then
                                -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                -- TODO(native): *puVar1 = *puVar1 | 4;
                                goto LAB_00e68954
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1a0);
                                    goto LAB_00e68395
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1f8);
                                goto LAB_00e68395
                            end
                            goto FLOW_past_lab_00e68395
                            ::LAB_00e68395::
                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1f0);
                            goto LAB_00e68954
                            ::FLOW_past_lab_00e68395::
                        end
                    end
                    goto FLOW_past_lab_00e68954
                    ::LAB_00e68954::
                    -- TODO(native): if ctr_CVar19 == *(self0X18 + 0xf8) then
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): CCharString::CCharString(&xStack_398_2,&xStack_398);
                    -- TODO(native): Game_InitializeArena(*(undefined4 *)(this + 0x14));
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)xStack_360);
                    goto LAB_00e68a20
                    ::FLOW_past_lab_00e68954::
                    ::LAB_00e68ad9::
                    -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)xStack_360);
                end
                ::LAB_00e68ae2::
            end
            ::FLOW_hoist_lab_00e6762e_2::
            break
            ::FLOW_past_lab_00e6762e::
            ::LAB_00e68a3c::
            -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,false);
            resources:DestroyMovie(movie4)
            break
        end
        ::LAB_00e68a20::
        quest:NewScriptFrame(me)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    end
    ::LAB_00e68aeb::
    ::LAB_00e68af4::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    resources:ReleaseResource(resource)
end

-- ChickenMaster.Init (retail 0x00e63560)
function Init(quest, me)
    heroHasPlayed = false
    haveTalked = false
    kickingChickens = false
    ghostChat = false
end

-- ChickenMaster.OnPersist (retail 0x00e641b0)
function OnPersist(quest, me, context)
    quest:SetStateBool("HeroHasPlayed", quest:PersistTransferBool(context, "HeroHasPlayed", quest:GetStateBool("HeroHasPlayed")))
    quest:SetStateBool("HaveTalked", quest:PersistTransferBool(context, "HaveTalked", quest:GetStateBool("HaveTalked")))
    quest:SetStateBool("GhostChat", quest:PersistTransferBool(context, "GhostChat", quest:GetStateBool("GhostChat")))
end

-- ChickenMaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

