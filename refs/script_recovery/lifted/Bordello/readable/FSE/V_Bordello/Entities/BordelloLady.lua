-- Readable native conversion: BordelloLady. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local goldRequired_, noLongerWorking, haveTalked_, partiedAlready_, name_, walkingDownstairs

-- BordelloLady.Main (retail 0x00e3eb10)
function Main(quest, me)
    local resources = quest:RetailResources()
    local name, isActiveThreadTerminating, predicateResult, predicateResult20, questionAnswer
    local questionAnswer2, getStateString, scratchValue, scratchValue15, getHomePos
    local getNumberOfTimesHeroHasHadSex, scratchValue17
    local ladyHitAfterConversation
    local goldRequired = goldRequired_
    if not quest:NewScriptFrame(me) then return end
    local ladyResource = resources:NewResource()
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local homePosition = me:GetHomePos()
    quest:SetWanderCentrePoint(me, homePosition)
    quest:SetWanderMinDistance(me, 0)
    quest:SetWanderMaxDistance(me, 16.0)
    quest:SetScriptingStateGroup(me, 4)
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    ::LADY_POLL::
    while true do
        if isActiveThreadTerminating then
            resources:ReleaseResource(ladyResource)
            return
        end
        if not (not me:IsTalkedToByHero() or quest:IsReportedOrUnreportedCrimeKnown(quest:GetStateThing("BordelloVillage"))) then break end
        if not noLongerWorking and quest:GetStateBool("BecomeNunnery") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
            noLongerWorking = true
            quest:ClearThingHasInformation(me)
            quest:EntitySetPersonalityOverrideByString(me, "OPINION_PERSONALITY_EX_PROSTITUTE")
        end
        local ladyWasHit = not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL))))
        if not ladyWasHit then
            quest:NewScriptFrame(me);
            isActiveThreadTerminating = quest:IsActiveThreadTerminating();
        else
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
            resources:PrepareResource(resources:MemberResource("seh_Whore"))
            resources:PrepareResource(ladyResource)
            while not resources:TryAcquire(ladyResource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(ladyResource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
            me:ClearCommands()
            me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(ladyResource); return end
            end
            if not quest:IsActiveThreadTerminating() then
                resources:PrepareResource(ladyResource)
            else
                resources:ReleaseResource(ladyResource)
                return
            end
            quest:NewScriptFrame(me)
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
    resources:PrepareResource(resources:MemberResource("seh_Whore"))
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:PrepareResource(ladyResource)
    while not resources:TryAcquire(ladyResource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:ReleaseResource(ladyResource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(ladyResource)
        return
    end
    me:ClearCommands()
    if quest:GetStateBool("BecomeNunnery") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
        resources:ScriptThing(ladyResource)
        if 0.0 < quest:GetHealth(me) then
            getHomePos = GetLHTSTag(quest, me, "HAPPY")
            local hero = quest:GetHero()
            if not me:Speak(hero, getHomePos, 0, false, true, false) then
                goto LAB_00e3f729
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e3f729 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        end
        goto LAB_00e3ffe3
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00e403af
    else
        if quest:GetStateBool("HeroTricking") and helpers.IsHeroWearingBeard(quest, me) and not quest:GetStateBool("PlayerOwned") then
            if not quest:IsActiveThreadTerminating() then
                if 0.0 < quest:GetHealth(me) then
                    getHomePos = GetLHTSTag(quest, me, "MAGICIAN_HINT")
                    local hero2 = quest:GetHero()
                    if not me:Speak(hero2, getHomePos, 0, false, true, false) then
                        goto LAB_00e3f729
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e3f729 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                end
                goto LAB_00e3ffe3
            end
            goto LAB_00e4039f
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
        if not quest:GetStateBool("PlayerOwned") then
            if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
            if not haveTalked_ then
                if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
                if not (quest:GetHealth(me) <= 0.0) then
                    goto FLOW_past_lab_00e3f953
                end
                ::LAB_00e3f953::
                haveTalked_ = true
                goto LAB_00e3f957
                ::FLOW_past_lab_00e3f953::
                getHomePos = GetLHTSTag(quest, me, "PARTY_PAID_INTRODUCTION")
                local hero3 = quest:GetHero()
                if not me:Speak(hero3, getHomePos, 0, false, true, false) then
                    goto LAB_00e3eee8
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00e3f953 end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
                if not partiedAlready_ then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
                    if 0.0 < quest:GetHealth(me) then
                        getHomePos = GetLHTSTag(quest, me, "PARTY_PAID_REMINDER")
                        local hero4 = quest:GetHero()
                        if not me:Speak(hero4, getHomePos, 0, false, true, false) then
                            goto LAB_00e3f729
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e3f729 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
                    if 0.0 < quest:GetHealth(me) then
                        getHomePos = GetLHTSTag(quest, me, "PARTY_AGAIN")
                        local hero5 = quest:GetHero()
                        if not me:Speak(hero5, getHomePos, 0, false, true, false) then
                            goto LAB_00e3eee8
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                    end
                end
                goto LAB_00e3f957
            end
            goto FLOW_past_lab_00e3f957
            ::LAB_00e3f957::
            scratchValue17 = GetLHTSTag(quest, me, "PARTY_PAID_QUESTION")
            quest:GiveHeroYesNoQuestion(scratchValue17, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if not quest:IsActiveThreadTerminating() then
                predicateResult = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if not predicateResult then
                        local getHeroGold = quest:GetHeroGold()
                        if goldRequired <= getHeroGold then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                            if 0.0 < quest:GetHealth(me) then
                                getHomePos = GetLHTSTag(quest, me, "PARTY_PAID_FOLLOW_ME")
                                local hero6 = quest:GetHero()
                                if not me:Speak(hero6, getHomePos, 0, false, true, false) then
                                    goto LAB_00e3eee8
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                            end
                            quest:GiveHeroGold(-goldRequired)
                            resources:AssignResource(resources:MemberResource("seh_Whore"), ladyResource)
                            getStateString = name_
                            scratchValue = "TEXT_CS_B13_SEX_" .. getStateString
                            resources:SetString(resources:MemberStringMap("csargs"), "$SEXTALK", scratchValue)
                            scratchValue = GetLHTSTag(quest, me, "PARTY_PAID_PLEASED")
                            resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", scratchValue)
                            quest:SetCutsceneSkippable(false)
                            if not quest:GetStateBool("HadSex") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                                if getStateString == nil then
                                    goto LAB_00e3fd35
                                else
                                    if getStateString ~= "HEDWIG" then goto LAB_00e3fd35 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                                    scratchValue15 = "CS_BORDELLO_PAYINGFORSEX_HEDWIG"
                                end
                                goto FLOW_past_lab_00e3fd35
                                ::LAB_00e3fd35::
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                                scratchValue15 = "CS_BORDELLO_PAYINGFORSEX"
                                ::FLOW_past_lab_00e3fd35::
                                helpers.PlayCutscene(quest, me, scratchValue15, true)
                                quest:SetStateBool("HadSex", true)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                                if getStateString ~= nil and getStateString == "HEDWIG" then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG", true)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE", true)
                                end
                            end
                            quest:SetCutsceneSkippable(true)
                            partiedAlready_ = true
                            quest:SetStateBool("HeroPartying", true)
                            goto LAB_00e3fd7c
                        end
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(me) then
                                getHomePos = GetLHTSTag(quest, me, "PARTY_PAID_TOO_EXPENSIVE")
                                local hero7 = quest:GetHero()
                                if not me:Speak(hero7, getHomePos, 0, false, true, false) then
                                    goto LAB_00e3f729
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e3f729 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                            end
                            goto LAB_00e3ffe3
                        end
                    end
                    goto LAB_00e3eee8
                end
                goto FLOW_hoist_lab_00e3eee8_1
            end
            ::FLOW_past_lab_00e3f957::
            goto FLOW_hoist_lab_00e3eee8_3
        end
        goto FLOW_hoist_lab_00e3eee8_4
    end
    goto FLOW_past_lab_00e403af
    ::LAB_00e403af::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00e403af::
    goto FLOW_past_lab_00e3eee8
    ::LAB_00e3eee8::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(ladyResource)
    do return end
    ::FLOW_hoist_lab_00e3eee8_1::
    if predicateResult then goto LAB_00e3f729 end
    if 0.0 < quest:GetHealth(me) then
        getHomePos = GetLHTSTag(quest, me, "PARTY_PAID_DECLINED")
        local hero8 = quest:GetHero()
        if not me:Speak(hero8, getHomePos, 0, false, true, false) then
            goto LAB_00e3eee8
        end
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
    end
    ::LAB_00e3ffe3::
    if not walkingDownstairs then
        goto LAB_00e40044
    else
        getHomePos = me:GetHomePos()
        if not (me ~= nil and me:IsDistanceFromPositionOver(getHomePos, 2.0)) then goto LAB_00e40044 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        local p0 = me:GetHomePos()
        me:MoveToPosition(p0, 1.0, ENTITY_MOVE_WALK, false, true)
    end
    goto FLOW_past_lab_00e40044
    ::LAB_00e40044::
    if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
    walkingDownstairs = false
    if not resources:ScriptThing(ladyResource):IsNull() then
        resources:PrepareResource(ladyResource)
    end
    ::FLOW_past_lab_00e40044::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    if not noLongerWorking and quest:GetStateBool("BecomeNunnery") then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
        noLongerWorking = true
        quest:ClearThingHasInformation(me)
        quest:EntitySetPersonalityOverrideByString(me, "OPINION_PERSONALITY_EX_PROSTITUTE")
    end
    ladyHitAfterConversation = false
    if not (me:MsgIsHitByHero()) then
        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e40183_c2 end
        goto FLOW_past_lab_00e40183_c2
    end
    ::LAB_00e40183_c2::
    ladyHitAfterConversation = true
    ::FLOW_past_lab_00e40183_c2::
    if ladyHitAfterConversation then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
        resources:PrepareResource(resources:MemberResource("seh_Whore"))
        resources:PrepareResource(ladyResource)
        while not resources:TryAcquire(ladyResource, me, 4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(ladyResource); return end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(ladyResource); return end
        me:ClearCommands()
        me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(ladyResource); return end
        end
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(ladyResource)
        else
            resources:ReleaseResource(ladyResource)
            return
        end
    end
    quest:NewScriptFrame(me)
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then resources:ReleaseResource(ladyResource); return end
    goto LADY_POLL
    ::FLOW_hoist_lab_00e3eee8_3::
    goto LAB_00e3f729
    ::FLOW_hoist_lab_00e3eee8_4::
    goto FLOW_hoist_lab_00e3f729_1
    ::FLOW_past_lab_00e3eee8::
    goto FLOW_past_lab_00e3f729
    ::LAB_00e3f729::
    goto LAB_00e403af
    ::FLOW_hoist_lab_00e3f729_1::
    if not quest:IsActiveThreadTerminating() then
        resources:ScriptThing(ladyResource)
        if 0.0 < quest:GetHealth(me) then
            getHomePos = GetLHTSTag(quest, me, "PARTY_FREE")
            local hero9 = quest:GetHero()
            if not me:Speak(hero9, getHomePos, 0, false, true, false) then
                goto LAB_00e403af
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e403af end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
        end
        scratchValue17 = GetLHTSTag(quest, me, "PARTY_FREE_QUESTION")
        quest:GiveHeroYesNoQuestion(scratchValue17, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
        while questionAnswer2 < 0 do
            if not quest:NewScriptFrame(me) then goto LAB_00e403af end
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
        end
        if not quest:IsActiveThreadTerminating() then
            predicateResult20 = quest:IsActiveThreadTerminating()
            if questionAnswer2 == 1 then
                if predicateResult20 then goto LAB_00e403af end
                if 0.0 < quest:GetHealth(me) then
                    getHomePos = GetLHTSTag(quest, me, "PARTY_FREE_FOLLOW_ME")
                    local hero10 = quest:GetHero()
                    if not me:Speak(hero10, getHomePos, 0, false, true, false) then
                        goto LAB_00e4039f
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e4039f end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
                end
                resources:AssignResource(resources:MemberResource("seh_Whore"), ladyResource)
                scratchValue15 = "_BOSS"
                getStateString = ("TEXT_CS_B13_SEX_" .. name_) .. scratchValue15
                resources:SetString(resources:MemberStringMap("csargs"), "$SEXTALK", getStateString)
                getStateString = GetLHTSTag(quest, me, "PARTY_FREE_FINISHING_UP")
                resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", getStateString)
                quest:SetCutsceneSkippable(false)
                name = name_
                if name == nil then
                    goto LAB_00e3f48a
                end
                goto FLOW_hoist_lab_00e3f48a_1
            end
            goto FLOW_hoist_lab_00e3f48a_2
        end
    end
    goto FLOW_past_lab_00e3f48a
    ::LAB_00e3f48a::
    if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
    scratchValue15 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE"
    ::LAB_00e3f4a5::
    helpers.PlayCutscene(quest, me, scratchValue15, true)
    quest:SetCutsceneSkippable(true)
    partiedAlready_ = true
    quest:SetStateBool("HeroPartying", true)
    goto LAB_00e3fd7c
    ::FLOW_hoist_lab_00e3f48a_1::
    goto FLOW_hoist_lab_00e3fd7c_1
    ::FLOW_hoist_lab_00e3f48a_2::
    goto FLOW_hoist_lab_00e3fd7c_2
    ::FLOW_past_lab_00e3f48a::
    goto FLOW_past_lab_00e3fd7c
    ::LAB_00e3fd7c::
    getNumberOfTimesHeroHasHadSex = quest:GetNumberOfTimesHeroHasHadSex()
    quest:SetNumberOfTimesHeroHasHadSex(getNumberOfTimesHeroHasHadSex + 1)
    quest:SetHeroAsHavingHadSex(true)
    walkingDownstairs = true
    goto LAB_00e3ffe3
    ::FLOW_hoist_lab_00e3fd7c_1::
    if name ~= "HEDWIG" then goto LAB_00e3f48a end
    if not quest:IsActiveThreadTerminating() then scratchValue15 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG"; goto LAB_00e3f4a5 end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(ladyResource)
    do return end
    ::FLOW_hoist_lab_00e3fd7c_2::
    if not predicateResult20 then
        if 0.0 < quest:GetHealth(me) then
            getHomePos = GetLHTSTag(quest, me, "PARTY_FREE_DECLINED")
            local hero11 = quest:GetHero()
            if not me:Speak(hero11, getHomePos, 0, false, true, false) then
                goto LAB_00e3f729
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e3f729 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        end
        goto LAB_00e3ffe3
    end
    ::FLOW_past_lab_00e3fd7c::
    goto LAB_00e4039f
    ::FLOW_past_lab_00e3f729::
    resources:DestroyMovie(movie)
    resources:ReleaseResource(ladyResource)
    do return end
    ::LAB_00e4039f::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(ladyResource)
end

-- BordelloLady.Init (retail 0x00e3ac70)
function Init(quest, me)
    local this_00
    walkingDownstairs = false
    haveTalked_ = false
    partiedAlready_ = false
    noLongerWorking = false
    this_00 = name_
    local dataString = me:GetDataString()
    name_ = dataString
    if not quest:GetStateBool("BecomeNunnery") then
        quest:SetThingHasInformation(me, true)
    end
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
    goldRequired_ = 0
    if dataString ~= nil and dataString == "POLLY" then
        goldRequired_ = 50
    end
    if dataString ~= nil and dataString == "AMELIA" then
        goldRequired_ = 100
    end
    if dataString ~= nil and dataString == "LUCREZIA" then
        goldRequired_ = 200
    end
    if dataString == nil then
        do goto LAB_00e3ae08 end
    else
        if dataString ~= "SOPHIA" then goto LAB_00e3ae08 end
    end
    goldRequired_ = 1000
    ::LAB_00e3ae08::
    if dataString == nil then
        do
            return
        end
    elseif dataString ~= "HEDWIG" then
        return
    end
    goldRequired_ = 2000
end

-- BordelloLady.OnPersist (retail 0x00e3ba80)
function OnPersist(quest, me, context)
    local partiedAlready = partiedAlready_ or false
    partiedAlready_ = quest:PersistTransferBool(context, "PartiedAlready", partiedAlready)
    local haveTalked = haveTalked_ or false
    haveTalked_ = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
end

-- BordelloLady.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

-- BordelloLady.GetLHTSTag (retail 0x00e403d0)
-- E403D0: bsim names this body NScript::CV_BordelloScript::CBordelloLady::GetLHTSTag (a homologous script member); no PDB name
function GetLHTSTag(quest, me, dialogueSuffix)
    return (("TEXT_QST_B13_" .. name_) .. "_") .. dialogueSuffix
end

