-- Readable native conversion: BordelloClient. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local clientID, brainState, nextLine, sexChance

-- BordelloClient.Main (retail 0x00e44ea0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local questionAnswer, timerId, getDefName, scriptThing, scratchValue, scriptThing2
    local stack0xfffffee0, resource2, movie, scratchValue39, movie2, scratchValue41, movie3, line
    local scratchValue43, line2
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local clientSpawn = quest:GetThingWithScriptName("M_ClientSpawn")
    timerId = quest:RegisterTimer()
    if not ((me:GetDefName() == "CREATURE_BS_VILLAGER_FEMALE") and 1 or 0) then
        while brainState == 1 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                if not (resource ~= nil and not resources:ScriptThing(resource):IsNull()) then
                    scriptThing = {x = 0, y = 0, z = 0}
                else
                    scriptThing = resources:ScriptThing(resource):GetPos()
                end
                me:MoveToPosition(scriptThing, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(0, me, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                brainState = 2
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 3 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                stack0xfffffee0 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_ENTER_0") .. getDefName
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, line2, 0, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        quest:SetTimer(timerId, 120)
        while brainState == 2 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            if quest:GetTimer(timerId) == 0 or quest:GetStateBool("BecomeNunnery") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                brainState = 3
            end
            if not me:IsTalkedToByHero() then goto continue_3 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            me:ClearCommands()
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetStateBool("HeroTricking") and helpers.IsHeroWearingBeard(quest, me) then
                if not quest:IsActiveThreadTerminating() then
                    if sexChance ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e466b8 end
                        if 3 < nextLine then
                            nextLine = 1
                        end
                        getDefName = tostring(nextLine)
                        local line3 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_DISGUSTED_0") .. getDefName
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, line3, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource)
                                return
                            end
                        end
                        goto LAB_00e46498
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e466db end
                    if 3 < nextLine then
                        nextLine = 1
                    end
                    getDefName = tostring(nextLine)
                    movie3 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_INTERESTED_0") .. getDefName
                    if 0.0 >= quest:GetHealth(resources:ScriptThing(resource)) then goto LAB_00e45d07 end
                    if not me:Speak(hero, movie3, 0, false, true, false) then goto LAB_00e46644 end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e45d07 end
                    goto FLOW_past_lab_00e45d07
                    ::LAB_00e45d07::
                    nextLine = nextLine + 1
                    quest:GiveHeroYesNoQuestion(("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_SEX_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e46644 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if predicateResult then
                                -- TODO(native): (**(code **)(*unaff_EBX + 0x5ec))();
                                resources:DestroyMovie(movie2)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if 3 < nextLine then
                                nextLine = 1
                            end
                            getDefName = tostring(nextLine)
                            local line4 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_ACCEPTED_0") .. getDefName
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                me:Speak(hero, line4, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        -- TODO(native): (**(code **)(*unaff_EBP + 0x5ec))();
                                        resources:DestroyMovie(movie3)
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    -- TODO(native): (**(code **)(*unaff_EBP + 0x5ec))();
                                    resources:DestroyMovie(movie3)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            nextLine = nextLine + 1
                            quest:GiveHeroGold(200)
                            quest:SetNumberOfTimesHeroHasHadSex(quest:GetNumberOfTimesHeroHasHadSex())
                            quest:SetCutsceneSkippable(false)
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (resources:MemberResource("seh_Whore"),&xStack_120);
                            quest:SetStateBool("HeroPartying", true)
                            getDefName = ("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_LEAVE_01"
                            resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", getDefName)
                            helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAIDFORSEX_QUICKIE", true)
                            quest:SetCutsceneSkippable(true)
                            brainState = 3
                            quest:SetHeroAsHavingHadGaySex(true)
                        else
                            if predicateResult then goto LAB_00e46644 end
                            if 3 < nextLine then
                                nextLine = 1
                            end
                            getDefName = tostring(nextLine)
                            local line5 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_DECLINED_0") .. getDefName
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                if not me:Speak(hero, line5, 0, false, true, false) then goto LAB_00e46644 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e46644 end
                            end
                            nextLine = nextLine + 1
                        end
                        goto LAB_00e464a0
                    end
                    ::FLOW_past_lab_00e45d07::
                    ::LAB_00e46644::
                    -- TODO(native): (**(code **)(*unaff_EBX + 0x5ec))();
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                goto LAB_00e466b8
            end
            goto FLOW_past_lab_00e466b8
            ::LAB_00e466b8::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e466b8::
            if quest:IsActiveThreadTerminating() then goto LAB_00e466db end
            goto FLOW_past_lab_00e466db
            ::LAB_00e466db::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e466db::
            if 3 < nextLine then
                nextLine = 1
            end
            getDefName = tostring(nextLine)
            line = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_BROWSE_0") .. getDefName
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e466b8 end
                if quest:IsActiveThreadTerminating() then goto LAB_00e466db end
            end
            ::LAB_00e46498::
            nextLine = nextLine + 1
            ::LAB_00e464a0::
            resources:PrepareResource(resources:MemberResource("seh_Whore"))
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            ::continue_3::
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        timerId = brainState
        while timerId == 3 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            timerId = me:IsPerformingScriptTask()
            if not timerId then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                if movie3 == nil then
                    scratchValue = {x = 0, y = 0, z = 0}
                else
                    -- TODO(native): puVar8 = (**(*xStack_e4 + 0x18))()
                    scratchValue = nil --[[unresolved native value]]
                end
                me:MoveToPosition(scratchValue, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(clientSpawn, me, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                brainState = 4
            end
            if not me:IsTalkedToByHero() then
                timerId = brainState
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 3 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                movie3 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID + 1)) .. "_LEAVE_0") .. getDefName
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, line, 0, false, true, false)
                    timerId = me:IsPerformingScriptTask()
                    while timerId do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            timerId = me:IsPerformingScriptTask()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                            timerId = me:IsPerformingScriptTask()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                timerId = brainState
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        if brainState == 4 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            quest:FadeOutAndKillEntity(me, true, 2.0, true)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        else
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
        return
    else
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        while not quest:IsActiveThreadTerminating() do
            if not isActiveThreadTerminating then
                if not me:IsPerformingScriptTask() then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    if not (resource ~= nil and not resources:ScriptThing(resource):IsNull()) then
                        scriptThing2 = {x = 0, y = 0, z = 0}
                    else
                        scriptThing2 = resources:ScriptThing(resource):GetPos()
                    end
                    me:MoveToPosition(scriptThing2, 1.0, ENTITY_MOVE_WALK, false, true)
                end
                if quest:IsDistanceBetweenThingsUnder(me, 0, 5.0) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    -- TODO(native): xStack_e0_b3 = '\x01';
                    resources:PrepareResource(resource)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 2 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                scratchValue41 = (("TEXT_QST_B13_WOMAN" .. tostring(clientID + 1)) .. "_CHAT_0") .. getDefName
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    if not me:Speak(hero, movie3, 0, false, true, false) then goto LAB_00e4568a end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                if scratchValue43 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4568a end
                    goto FLOW_hoist_lab_00e4568a_1
                end
                goto FLOW_past_lab_00e4568a
                ::LAB_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_hoist_lab_00e4568a_1::
                resources:PrepareResource(resource)
                ::FLOW_past_lab_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            if not me:MsgIsHitByHero() then
                -- TODO(native): bVar4 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                if nil then
                    -- TODO(native): bVar4 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                    if not nil then goto LAB_00e453d2 end
                end
                -- TODO(native): xStack_c4_b3 = '\0';
            else
                goto LAB_00e453d2
            end
            goto FLOW_past_lab_00e453d2
            ::LAB_00e453d2::
            -- TODO(native): xStack_c4_b3 = '\x01';
            ::FLOW_past_lab_00e453d2::
            if scratchValue39 == 0 then quest:NewScriptFrame(me); goto continue_8 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            me:ClearCommands()
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, ("TEXT_QST_B13_WOMAN" .. tostring(clientID + 1)) .. "_ON_HIT", 0, false, true, false) then goto LAB_00e456ca end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            if scratchValue43 ~= 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e456ca end
                goto FLOW_hoist_lab_00e456ca_1
            end
            goto FLOW_past_lab_00e456ca
            ::LAB_00e456ca::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_hoist_lab_00e456ca_1::
            resources:PrepareResource(resource)
            ::FLOW_past_lab_00e456ca::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:NewScriptFrame(me)
            ::continue_8::
        end
        quest:DeregisterTimer(timerId)
    end
    resources:ReleaseResource(resource)
    do return end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- BordelloClient.Init (retail 0x00e3b1a0)
function Init(quest, me)
    local clientInUseIndex, scratchValue
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + 1)
    brainState = 1
    scratchValue = 0
    nextLine = 1
    clientID = 0
    sexChance = math.random(0, 32767) % 5
    clientInUseIndex = 0
    repeat
        if not quest:GetStateBool("ClientInUse_" .. clientInUseIndex) then
            clientID = scratchValue
            quest:SetStateBool("ClientInUse_" .. scratchValue, true)
            break
        end
        scratchValue = scratchValue + 1
        clientInUseIndex = clientInUseIndex + 1
    until scratchValue >= 3
    quest:SetThingPersistent(me, true)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
end

-- BordelloClient.OnPersist (retail 0x00e3bb00)
function OnPersist(quest, me, context)
    quest:SetStateInt("BrainState", quest:PersistTransferInt(context, "BrainState", quest:GetStateInt("BrainState") or 0))
    quest:SetStateInt("ClientID", quest:PersistTransferInt(context, "ClientID", quest:GetStateInt("ClientID") or 0))
    quest:SetStateInt("SexChance", quest:PersistTransferInt(context, "SexChance", quest:GetStateInt("SexChance") or 0))
end

-- BordelloClient.OnPredicateFail (retail 0x00e3b230)
function OnPredicateFail(quest, me)
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") - 1)
    -- TODO(native): *(undefined1 *)(*(int *)(this + 0x20) + 0x58 + *(int *)(this + 0x14)) = 0;
end

