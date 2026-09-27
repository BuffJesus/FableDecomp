-- Readable native conversion: BordelloLady. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local self0Xc, name, noLongerWorking, haveTalked, partiedAlready, goldRequired, walkingDownstairs

-- BordelloLady.Main (retail 0x00e3eb10)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, predicateResult, predicateResult22, scratchValue4, health
    local getHealth, questionAnswer, questionAnswer2, getStateString, scratchValue11, line
    local scratchValue13, resource2, pppuVar28, scratchValue16, scratchValue17, getStateInt
    local getStateInt2, resource
    if not quest:NewScriptFrame(me) then return end
    resources:NewResource()
    quest:SetCreatureBrain(nil --[[missing]], "BRAIN_PASSIVE_OVERRIDE")
    getStateInt = self0Xc
    quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])
    quest:SetWanderMinDistance(nil --[[missing]], 0)
    getStateInt2 = self0Xc
    -- TODO(native): xStack_1a0 = *(undefined ***)(this + 0x10);
    if self0Xc ~= nil then
        -- TODO(native): *xStack_1a0 = *xStack_1a0 + 1;
    end
    quest:SetWanderMaxDistance(nil --[[missing]], 16.0)
    -- TODO(native): xStack_1a0 = *(undefined ***)(this + 0x10);
    if self0Xc ~= nil then
        -- TODO(native): *xStack_1a0 = (undefined *)((int)*xStack_1a0 + 1);
    end
    quest:SetScriptingStateGroup(nil --[[missing]], 0)
    predicateResult2 = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult2 then
            return
        end
        scratchValue = not me:IsTalkedToByHero()
        if not scratchValue then
            scratchValue4 = quest:IsReportedOrUnreportedCrimeKnown(nil --[[missing]])
            scratchValue = scratchValue4
        end
        local predicateResult3 = not scratchValue
        if getStateInt & 1 ~= 0 then
            -- TODO(native): xStack_15c = xStack_15c & 0xfffffffe;
        end
        if predicateResult3 then break end
        if not noLongerWorking and quest:GetStateBool("BecomeNunnery") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(self0Xc); return end
            noLongerWorking = true
            quest:ClearThingHasInformation(nil --[[missing]])
            -- TODO(native): EntitySetPersonalityOverride is not a ForgeFSE binding
            quest:EntitySetPersonalityOverride()
        end
        scratchValue17 = getStateInt
        -- TODO(native): xStack_15c = xStack_15c | 2;
        if not me:MsgIsHitByHero() then
            scratchValue16 = scratchValue17 | 6
            getStateInt = scratchValue16
            -- TODO(native): bVar5 = (**(*me + 0xa8))(me,&xStack_120)
    --[[unresolved native value]]
            if nil then
                scratchValue16 = scratchValue17 | 14
                getStateInt = scratchValue16
                -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,&xStack_128)
    --[[unresolved native value]]
                if not nil then goto LAB_00e40183 end
            end
        else
            goto LAB_00e40183
        end
        goto FLOW_past_lab_00e40183
        ::LAB_00e40183::
        ::FLOW_past_lab_00e40183::
        if scratchValue16 & 8 ~= 0 then
            scratchValue16 = scratchValue16 & 0xfffffff7
            getStateInt = scratchValue16
        end
        if scratchValue16 & 4 ~= 0 then
            scratchValue16 = scratchValue16 & 0xfffffffb
            getStateInt = scratchValue16
        end
        if scratchValue16 & 2 ~= 0 then
            -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
        end
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resources:MemberResource("seh_Whore"))
            resources:PrepareResource(self0Xc)
            while not me:AcquireControl(4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(self0Xc); return end
            end
            if not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(self0Xc); return end
                end
                if not quest:IsActiveThreadTerminating() then resources:PrepareResource(self0Xc); goto LAB_00e402f9 end
            end
        end
        resources:ReleaseResource(self0Xc)
        do return end
        ::LAB_00e402f9::
        quest:NewScriptFrame(me)
        predicateResult2 = quest:IsActiveThreadTerminating()
    end
    ::FLOW_after_lab_00e40085::
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(getStateInt2); return end
    resources:PrepareResource(resources:MemberResource("seh_Whore"))
    local movie = resources:StartMovie("")
    quest:StartMovieSequence()
    getStateInt2 = resource2
    quest:PauseAllNonScriptedEntities(true)
    resources:PrepareResource(resource2)
    scratchValue4 = me:AcquireControl(4)
    while not scratchValue4 do
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = me:AcquireControl(8)
        else
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyMovie(movie)
            do return end
            scratchValue4 = me:AcquireControl(8)
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(movie)
        resources:DestroyMovie(movie)
        return
    end
    me:ClearCommands()
    if quest:GetStateBool("BecomeNunnery") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
        resources:ScriptThing(resource2)
        health = quest:GetHealth(nil --[[missing]])
        getHealth = 0.0
        if 0.0 < health then
            line = GetLHTSTag(quest, me, "HAPPY")
            if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3f729 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        end
        goto LAB_00e3ffe3
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00e403af
    else
        if quest:GetStateBool("HeroTricking") and helpers.IsHeroWearingBeard(quest, me) and not quest:GetStateBool("PlayerOwned") then
            if not quest:IsActiveThreadTerminating() then
                health = quest:GetHealth(nil --[[missing]])
                getHealth = 0.0
                if 0.0 < health then
                    line = GetLHTSTag(quest, me, "MAGICIAN_HINT")
                    if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3f729 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                end
                goto LAB_00e3ffe3
            end
            goto LAB_00e4039f
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
        if not quest:GetStateBool("PlayerOwned") then
            if not haveTalked then
                health = quest:GetHealth(nil --[[missing]])
                if health <= 0.0 then
                    goto LAB_00e3f953
                end
                goto FLOW_past_lab_00e3f953
                ::LAB_00e3f953::
                haveTalked = true
                goto LAB_00e3f957
                ::FLOW_past_lab_00e3f953::
                line = GetLHTSTag(quest, me, "PARTY_PAID_INTRODUCTION")
                if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3eee8 end
                if not quest:IsActiveThreadTerminating() then goto LAB_00e3f953 end
            else
                if not partiedAlready then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        line = GetLHTSTag(quest, me, "PARTY_PAID_REMINDER")
                        if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3f729 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                    end
                else
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        line = GetLHTSTag(quest, me, "PARTY_AGAIN")
                        if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3eee8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                    end
                end
                goto LAB_00e3f957
            end
            goto FLOW_past_lab_00e3f957
            ::LAB_00e3f957::
            quest:GiveHeroYesNoQuestion(GetLHTSTag(quest, me, "PARTY_PAID_QUESTION"), "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_PAID_QUESTION")
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e3eee8 end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if not quest:IsActiveThreadTerminating() then
                predicateResult = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if not predicateResult then
                        if goldRequired <= quest:GetHeroGold() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                            health = quest:GetHealth(nil --[[missing]])
                            getHealth = 0.0
                            if 0.0 < health then
                                line = GetLHTSTag(quest, me, "PARTY_PAID_FOLLOW_ME")
                                if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3eee8 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                            end
                            quest:GiveHeroGold(0.0)
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (resources:MemberResource("seh_Whore"),&xStack_1a8);
                            scratchValue11 = "TEXT_CS_B13_SEX_" .. name
                            resources:SetString(resources:MemberStringMap("csargs"), line, scratchValue11)
                            scratchValue11 = GetLHTSTag(quest, me, "PARTY_PAID_PLEASED")
                            resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", scratchValue11)
                            quest:SetCutsceneSkippable(nil --[[missing]])
                            if not quest:GetStateBool("HadSex") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
                                if name == nil then
                                    goto LAB_00e3fd35
                                else
                                    if name ~= "HEDWIG" then goto LAB_00e3fd35 end
                                    scratchValue13 = "CS_BORDELLO_PAYINGFORSEX_HEDWIG"
                                end
                                goto FLOW_past_lab_00e3fd35
                                ::LAB_00e3fd35::
                                scratchValue13 = "CS_BORDELLO_PAYINGFORSEX"
                                ::FLOW_past_lab_00e3fd35::
                                helpers.PlayCutscene(quest, me, scratchValue13, true)
                                quest:SetStateBool("HadSex", true)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
                                if name ~= nil and name == "HEDWIG" then
                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG", true)
                                else
                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE", true)
                                end
                            end
                            quest:SetCutsceneSkippable(nil --[[missing]])
                            partiedAlready = true
                            quest:SetStateBool("HeroPartying", true)
                            goto LAB_00e3fd7c
                        end
                        if not quest:IsActiveThreadTerminating() then
                            health = quest:GetHealth(nil --[[missing]])
                            getHealth = 0.0
                            if 0.0 < health then
                                line = GetLHTSTag(quest, me, "PARTY_PAID_TOO_EXPENSIVE")
                                if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3f729 end
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
    -- TODO(native): ppuVar13 = *piVar1
--[[unresolved native value]]
    ::LAB_00e3f731::
    -- TODO(native): (*(code *)ppuVar13[0x17b])();
    ::FLOW_past_lab_00e403af::
    goto FLOW_past_lab_00e3eee8
    ::LAB_00e3eee8::
    -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();
    resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")
    resources:ReleaseResource(resource2)
    do return end
    ::FLOW_hoist_lab_00e3eee8_1::
    if predicateResult then goto LAB_00e3f729 end
    health = quest:GetHealth(nil --[[missing]])
    getHealth = 0.0
    if 0.0 < health then
        line = GetLHTSTag(quest, me, "PARTY_PAID_DECLINED")
        if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3eee8 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
    end
    ::LAB_00e3ffe3::
    if not walkingDownstairs then
        goto LAB_00e40044
    else
        if not (me ~= nil and me:IsDistanceFromPositionOver(me:GetHomePos(), 2.0)) then goto LAB_00e40044 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        -- TODO(native): p0 = (**(*me + 0x1c))(me,xStack_30)
        local p0 = nil --[[unresolved native value]]
        me:MoveToPosition(p0, 1.0, ENTITY_MOVE_WALK, false, true)
    end
    goto FLOW_past_lab_00e40044
    ::LAB_00e40044::
    if quest:IsActiveThreadTerminating() then goto LAB_00e3f729 end
    walkingDownstairs = false
    if not resources:ScriptThing(resource2):IsNull() then
        resources:PrepareResource(resource)
    end
    ::FLOW_past_lab_00e40044::
    -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();
    resources:ReleaseResource(resource2)
    if not noLongerWorking and quest:GetStateBool("BecomeNunnery") then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
        noLongerWorking = true
        quest:ClearThingHasInformation(nil --[[missing]])
        -- TODO(native): EntitySetPersonalityOverride is not a ForgeFSE binding
        quest:EntitySetPersonalityOverride()
    end
    scratchValue17 = ""
    -- TODO(native): xStack_15c = xStack_15c | 2;
    if not me:MsgIsHitByHero() then
        scratchValue16 = scratchValue17 | 6
        -- TODO(native): bVar5 = (**(*me + 0xa8))(me,&xStack_120)
    --[[unresolved native value]]
        if nil then
            scratchValue16 = scratchValue17 | 14
            -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,&xStack_128)
    --[[unresolved native value]]
            if not nil then goto LAB_00e40183_c2 end
        end
    end
    goto FLOW_past_lab_00e40183_c2
    ::LAB_00e40183_c2::
    ::FLOW_past_lab_00e40183_c2::
    if scratchValue16 & 8 ~= 0 then
        scratchValue16 = scratchValue16 & 0xfffffff7
    end
    if scratchValue16 & 4 ~= 0 then
        scratchValue16 = scratchValue16 & 0xfffffffb
    end
    if scratchValue16 & 2 ~= 0 then
        -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
    end
    if not quest:IsActiveThreadTerminating() then
        resources:PrepareResource(resources:MemberResource("seh_Whore"))
        resources:PrepareResource(resource2)
        while not me:AcquireControl(4) do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
        end
        if not quest:IsActiveThreadTerminating() then
            me:ClearCommands()
            me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
            end
            if not quest:IsActiveThreadTerminating() then resources:PrepareResource(resource2); goto LAB_00e402f9_c2 end
        end
    end
    resources:ReleaseResource(resource2)
    do return end
    ::LAB_00e402f9_c2::
    quest:NewScriptFrame(me)
    goto FLOW_after_lab_00e40085
    ::FLOW_hoist_lab_00e3eee8_3::
    goto LAB_00e3f729
    ::FLOW_hoist_lab_00e3eee8_4::
    goto FLOW_hoist_lab_00e3f729_1
    ::FLOW_past_lab_00e3eee8::
    goto FLOW_past_lab_00e3f729
    ::LAB_00e3f729::
    -- TODO(native): ppuVar13 = *pppuVar28
--[[unresolved native value]]
    goto LAB_00e3f731
    ::FLOW_hoist_lab_00e3f729_1::
    if not quest:IsActiveThreadTerminating() then
        resources:ScriptThing(resource)
        getHealth = quest:GetHealth(nil --[[missing]])
        if 0.0 < getHealth then
            line = GetLHTSTag(quest, me, "PARTY_FREE")
            if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e403af end
            if quest:IsActiveThreadTerminating() then goto LAB_00e4039f end
        end
        quest:GiveHeroYesNoQuestion(GetLHTSTag(quest, me, "PARTY_FREE_QUESTION"), "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_FREE_QUESTION")
        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
        while questionAnswer2 < 0 do
            if not quest:NewScriptFrame(me) then goto LAB_00e403af end
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
        end
        if not quest:IsActiveThreadTerminating() then
            predicateResult22 = quest:IsActiveThreadTerminating()
            if questionAnswer2 == 1 then
                if predicateResult22 then goto LAB_00e403af end
                getHealth = quest:GetHealth(nil --[[missing]])
                if 0.0 < getHealth then
                    line = GetLHTSTag(quest, me, "PARTY_FREE_FOLLOW_ME")
                    if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e4039f end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e403af end
                end
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (resources:MemberResource("seh_Whore"),&xStack_1a8);
                getStateString = ("TEXT_CS_B13_SEX_" .. name) .. "_BOSS"
                resources:SetString(resources:MemberStringMap("csargs"), "$SEXTALK", getStateString)
                getStateString = GetLHTSTag(quest, me, "PARTY_FREE_FINISHING_UP")
                resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", getStateString)
                quest:SetCutsceneSkippable(true)
                -- TODO(native): CVar3 = *name
    --[[unresolved native value]]
                if nil == nil then
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
    scratchValue13 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE"
    ::LAB_00e3f4a5::
    helpers.PlayCutscene(quest, me, scratchValue13, true)
    quest:SetCutsceneSkippable(nil --[[missing]])
    partiedAlready = true
    quest:SetStateBool("HeroPartying", true)
    goto LAB_00e3fd7c
    ::FLOW_hoist_lab_00e3f48a_1::
    goto FLOW_hoist_lab_00e3fd7c_1
    ::FLOW_hoist_lab_00e3f48a_2::
    goto FLOW_hoist_lab_00e3fd7c_2
    ::FLOW_past_lab_00e3f48a::
    goto FLOW_past_lab_00e3fd7c
    ::LAB_00e3fd7c::
    quest:SetNumberOfTimesHeroHasHadSex(nil --[[missing]])
    quest:SetHeroAsHavingHadSex(true)
    walkingDownstairs = true
    goto LAB_00e3ffe3
    ::FLOW_hoist_lab_00e3fd7c_1::
    if nil ~= "HEDWIG" then goto LAB_00e3f48a end
    if not quest:IsActiveThreadTerminating() then scratchValue13 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG"; goto LAB_00e3f4a5 end
    -- TODO(native): (**(code **)(*piVar1 + 0x5ec))();
    resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")
    resources:ReleaseResource(resource2)
    do return end
    ::FLOW_hoist_lab_00e3fd7c_2::
    if not predicateResult22 then
        health = quest:GetHealth(nil --[[missing]])
        getHealth = 0.0
        if 0.0 < health then
            line = GetLHTSTag(quest, me, "PARTY_FREE_DECLINED")
            if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e3f729 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e3eee8 end
        end
        goto LAB_00e3ffe3
    end
    ::FLOW_past_lab_00e3fd7c::
    goto LAB_00e4039f
    ::FLOW_past_lab_00e3f729::
    resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")
    resources:ReleaseResource(resource2)
    do return end
    ::LAB_00e4039f::
    quest:PauseAllNonScriptedEntities(getHealth ~= 0)
    resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")
    resources:ReleaseResource(resource2)
end

-- BordelloLady.Init (retail 0x00e3ac70)
function Init(quest, me)
    -- TODO(native): puStack_1c = v_stk_4;
    walkingDownstairs = false
    haveTalked = false
    partiedAlready = false
    noLongerWorking = false
    local dataString = me:GetDataString()
    name = dataString
    if not quest:GetStateBool("BecomeNunnery") then
        quest:SetThingHasInformation(me, true)
    end
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
    goldRequired = 0
    if dataString ~= nil and dataString == "POLLY" then
        goldRequired = 50
    end
    if dataString ~= nil and dataString == "AMELIA" then
        goldRequired = 100
    end
    if dataString ~= nil and dataString == "LUCREZIA" then
        goldRequired = 200
    end
    if not ((dataString == nil) or (dataString ~= "SOPHIA")) then
        goldRequired = 1000
    end
    if dataString == nil then
        return
    elseif dataString ~= "HEDWIG" then
        return
    end
    goldRequired = 2000
end

-- BordelloLady.OnPersist (retail 0x00e3ba80)
function OnPersist(quest, me, context)
    quest:SetStateBool("PartiedAlready", quest:PersistTransferBool(context, "PartiedAlready", quest:GetStateBool("PartiedAlready")))
    quest:SetStateBool("HaveTalked", quest:PersistTransferBool(context, "HaveTalked", quest:GetStateBool("HaveTalked")))
end

-- BordelloLady.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

-- BordelloLady.GetLHTSTag (retail 0x00e403d0)
-- E403D0: bsim names this body NScript::CV_BordelloScript::CBordelloLady::GetLHTSTag (a homologous script member); no PDB name
function GetLHTSTag(quest, me, dialogueSuffix)
    return (("TEXT_QST_B13_" .. name) .. "_") .. dialogueSuffix
end

