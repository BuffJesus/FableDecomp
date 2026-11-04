-- Readable native conversion: BordelloClient. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local brainState_, nextLine, clientID_, sexChance_

-- BordelloClient.Main (retail 0x00e44ea0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local __push3, clientAtStand, hero3, questionAnswer, getHero, scratchValue15, getDefName
    local scratchValue40, getPos, line, movie, movie2, movie3, scratchValue45, scriptThing3
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local clientStand = quest:GetThingWithScriptName("M_ClientStand")
    local clientSpawn = quest:GetThingWithScriptName("M_ClientSpawn")
    local registeredTimerID = quest:RegisterTimer()
    if me:GetDefName() ~= "CREATURE_BS_VILLAGER_FEMALE" then
        while brainState_ == 1 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                if not (clientStand ~= nil and not clientStand:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = clientStand:GetPos()
                end
                me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(clientStand, me, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                brainState_ = 2
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 3 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                scratchValue40 = "_ENTER_0"
                local stack0xfffffee0 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                local scratchValue11 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue11) then
                    line = stack0xfffffee0
                    local hero = quest:GetHero()
                    if not me:Speak(hero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(registeredTimerID)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(registeredTimerID)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(registeredTimerID)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        quest:SetTimer(registeredTimerID, 120)
        while brainState_ == 2 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            if quest:GetTimer(registeredTimerID) == 0 or quest:GetStateBool("BecomeNunnery") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                brainState_ = 3
            end
            if not me:IsTalkedToByHero() then goto continue_3 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            me:ClearCommands()
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetStateBool("HeroTricking") and helpers.IsHeroWearingBeard(quest, me) then
                if not quest:IsActiveThreadTerminating() then
                    if sexChance_ ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e466b8 end
                        if 3 < nextLine then
                            nextLine = 1
                        end
                        getDefName = tostring(nextLine)
                        scratchValue40 = "_DISGUSTED_0"
                        local scratchValue41 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                        scriptThing3 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scriptThing3) then
                            line = scratchValue41
                            local hero2 = quest:GetHero()
                            if not me:Speak(hero2, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                quest:DeregisterTimer(registeredTimerID)
                                resources:ReleaseResource(resource)
                                return
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    quest:DeregisterTimer(registeredTimerID)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                quest:DeregisterTimer(registeredTimerID)
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
                    scratchValue40 = "_INTERESTED_0"
                    movie3 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                    local scriptThing = resources:ScriptThing(resource)
                    if not (not (0.0 < quest:GetHealth(scriptThing))) then
                        line = movie3
                        hero3 = quest:GetHero()
                        if not me:Speak(hero3, line, 0, false, true, false) then
                            goto FLOW_past_lab_00e45d07
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e45d07 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e45d07 end
                        goto FLOW_past_lab_00e45d07
                    end
                    ::LAB_00e45d07::
                    nextLine = nextLine + 1
                    scratchValue40 = "_SEX_QUESTION"
                    __push3 = ("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40
                    quest:GiveHeroYesNoQuestion(__push3, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e45d07 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not (quest:IsActiveThreadTerminating()) then
                        if questionAnswer == 1 then
                            if 3 < nextLine then
                                nextLine = 1
                            end
                            getDefName = tostring(nextLine)
                            scratchValue40 = "_ACCEPTED_0"
                            local scratchValue43 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                            local scratchValue12 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue12) then
                                line = scratchValue43
                                local hero4 = quest:GetHero()
                                if not me:Speak(hero4, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    quest:DeregisterTimer(registeredTimerID)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        quest:DeregisterTimer(registeredTimerID)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    quest:DeregisterTimer(registeredTimerID)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            nextLine = nextLine + 1
                            quest:GiveHeroGold(200)
                            local __push4 = quest:GetNumberOfTimesHeroHasHadSex()
                            quest:SetNumberOfTimesHeroHasHadSex(__push4 + 1)
                            quest:SetCutsceneSkippable(false)
                            resources:AssignResource(resources:MemberResource("seh_Whore"), resource)
                            quest:SetStateBool("HeroPartying", true)
                            scratchValue40 = "_LEAVE_01"
                            getDefName = ("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40
                            resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", getDefName)
                            helpers.PlayCutscene(quest, me, "CS_BORDELLO_PAIDFORSEX_QUICKIE", true)
                            quest:SetCutsceneSkippable(true)
                            brainState_ = 3
                            quest:SetHeroAsHavingHadGaySex(true)
                        else
                            if 3 < nextLine then
                                nextLine = 1
                            end
                            getDefName = tostring(nextLine)
                            scratchValue40 = "_DECLINED_0"
                            local scratchValue42 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                            scriptThing3 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scriptThing3) then
                                line = scratchValue42
                                local hero5 = quest:GetHero()
                                if not me:Speak(hero5, line, 0, false, true, false) then
                                    goto FLOW_past_lab_00e45d07
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e45d07 end
                                end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e45d07 end
                            end
                            nextLine = nextLine + 1
                        end
                        goto LAB_00e464a0
                    end
                    ::FLOW_past_lab_00e45d07::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(registeredTimerID)
                    resources:ReleaseResource(resource)
                    return
                end
                goto LAB_00e466b8
            end
            goto FLOW_past_lab_00e466b8
            ::LAB_00e466b8::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(registeredTimerID)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e466b8::
            if not (quest:IsActiveThreadTerminating()) then
                goto FLOW_past_lab_00e466db
            end
            ::LAB_00e466db::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(registeredTimerID)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e466db::
            if 3 < nextLine then
                nextLine = 1
            end
            getDefName = tostring(nextLine)
            scratchValue40 = "_BROWSE_0"
            scratchValue45 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
            movie = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(movie) then
                line = scratchValue45
                local hero6 = quest:GetHero()
                if not me:Speak(hero6, line, 0, false, true, false) then
                    goto LAB_00e466b8
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e466b8 end
                end
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
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        getHero = brainState_
        while getHero == 3 do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                if not (clientSpawn ~= nil and not clientSpawn:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = clientSpawn:GetPos()
                end
                me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(clientSpawn, me, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                brainState_ = 4
            end
            if not me:IsTalkedToByHero() then
                getHero = brainState_
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 3 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                scratchValue40 = "_LEAVE_0"
                movie3 = (("TEXT_QST_B13_CLIENT" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                local scratchValue13 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue13) then
                    line = movie3
                    getHero = quest:GetHero()
                    if not me:Speak(getHero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(registeredTimerID)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(registeredTimerID)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(registeredTimerID)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                getHero = brainState_
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        if brainState_ == 4 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            quest:FadeOutAndKillEntity(me, true, 2.0, true)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        else
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
        quest:DeregisterTimer(registeredTimerID)
        resources:ReleaseResource(resource)
        return
    else
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
        clientAtStand = isActiveThreadTerminating
        while not quest:IsActiveThreadTerminating() do
            if not clientAtStand then
                if not me:IsPerformingScriptTask() then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                    if not (clientStand ~= nil and not clientStand:IsNull()) then
                        getPos = {x = 0, y = 0, z = 0}
                    else
                        getPos = clientStand:GetPos()
                    end
                    me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
                end
                if quest:IsDistanceBetweenThingsUnder(me, clientStand, 5.0) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                    clientAtStand = true
                    resources:PrepareResource(resource)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                me:ClearCommands()
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 2 < nextLine then
                    nextLine = 1
                end
                getDefName = tostring(nextLine)
                scratchValue40 = "_CHAT_0"
                local scratchValue44 = (("TEXT_QST_B13_WOMAN" .. tostring(clientID_ + 1)) .. scratchValue40) .. getDefName
                local scratchValue14 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue14) then
                    line = scratchValue44
                    local hero7 = quest:GetHero()
                    if not me:Speak(hero7, line, 0, false, true, false) then
                        goto LAB_00e4568a
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e4568a end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(registeredTimerID)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                nextLine = nextLine + 1
                if clientAtStand then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4568a end
                    goto FLOW_hoist_lab_00e4568a_1
                end
                goto FLOW_past_lab_00e4568a
                ::LAB_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                quest:DeregisterTimer(registeredTimerID)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_hoist_lab_00e4568a_1::
                resources:PrepareResource(resource)
                ::FLOW_past_lab_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            if not (not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL))))) then quest:NewScriptFrame(me); goto continue_8 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            me:ClearCommands()
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registeredTimerID); resources:ReleaseResource(resource); return end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue15 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue15) then
                scratchValue40 = "_ON_HIT"
                line = ("TEXT_QST_B13_WOMAN" .. tostring(clientID_ + 1)) .. scratchValue40
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, line, 0, false, true, false) then
                    goto LAB_00e456ca
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e456ca end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(registeredTimerID)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            if clientAtStand then
                if quest:IsActiveThreadTerminating() then goto LAB_00e456ca end
                goto FLOW_hoist_lab_00e456ca_1
            end
            goto FLOW_past_lab_00e456ca
            ::LAB_00e456ca::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DeregisterTimer(registeredTimerID)
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
        quest:DeregisterTimer(registeredTimerID)
    end
    resources:ReleaseResource(resource)
    do return end
    quest:DeregisterTimer(registeredTimerID)
    resources:ReleaseResource(resource)
end

-- BordelloClient.Init (retail 0x00e3b1a0)
function Init(quest, me)
    local clientInUseIndex, scratchValue
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + 1)
    brainState_ = 1
    local randomChoice = quest:RetailRandModulo(32768)
    scratchValue = 0
    nextLine = 1
    clientID_ = 0
    sexChance_ = randomChoice % 5
    clientInUseIndex = 0
    repeat
        if not quest:GetStateBool("ClientInUse_" .. clientInUseIndex) then
            clientID_ = scratchValue
            quest:SetStateBool("ClientInUse_" .. scratchValue, true)
            break
        end
        scratchValue = scratchValue + 1
        clientInUseIndex = clientInUseIndex + 1
    until not (scratchValue < 3)
    quest:SetThingPersistent(me, true)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
end

-- BordelloClient.OnPersist (retail 0x00e3bb00)
function OnPersist(quest, me, context)
    local brainState = brainState_ or 0
    brainState_ = quest:PersistTransferInt(context, "BrainState", brainState)
    local clientID = clientID_ or 0
    clientID_ = quest:PersistTransferInt(context, "ClientID", clientID)
    local sexChance = sexChance_ or 0
    sexChance_ = quest:PersistTransferInt(context, "SexChance", sexChance)
end

-- BordelloClient.OnPredicateFail (retail 0x00e3b230)
function OnPredicateFail(quest, me)
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") - 1)
    quest:SetStateBool("ClientInUse_" .. (clientID_ or 0), false)
end

