-- Readable native conversion: BS_Teacher. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BooksNumberWanted = 1248,  -- 16
    BooksNumberAccepted = 1252,  -- 25
    BooksNumberComment = 1256,  -- 35
    BadBooksForHat = 1264,  -- 6
    GoodBooksForHat = 1268,  -- 9
    BooksMoralityReward = 1272,  -- 0.019999999552965164
}

local helpers = require("V_BookCollecting.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local booksAccepted_, badBooksForHat_, goodBooksForHat_, moralityReward, booksWanted_, booksComment_

-- BS_Teacher.Main (retail 0x00e54e90)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, booksDonated, scratchValue17, meControl, speechResult, speechResult2
    local speechResult3, speechResult4, speechResult5, speechResult6, movie, scratchValue25
    local function ReleaseEverything()
        local scratchValue17 = nil --[[reviewed native interface-table staging]]
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    quest:SetThingPersistent(me, true)
    booksDonated = quest:GetStateInt("BooksDonated")
    if booksDonated < 4 and quest:GetStateInt("GossipState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        helpers.AddGossip(quest, me, "Book Collection - Need Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_NEED", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS")
        quest:SetStateInt("GossipState", 1)
    elseif booksDonated < 4 or quest:GetStateInt("GossipState") ~= 1 then
        if 9 < booksDonated and quest:GetStateInt("GossipState") < 3 then
            if quest:IsActiveThreadTerminating() then return end
            helpers.AddGossip(quest, me, "Book Collection - Given Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_GIVEN", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS")
            quest:SetStateInt("GossipState", 3)
        end
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveRumourCategory("Book Collection - Need Books")
        quest:SetStateInt("GossipState", 2)
    end
    while quest:GetStateInt("BooksDonated") < booksAccepted_ do
        if not quest:NewScriptFrame(me) then return end
        if not quest:GetStateBool("HasInformation") or (quest:GetTimeOfDay() < 1616 and 844 < quest:GetTimeOfDay()) then
            if not quest:GetStateBool("HasInformation") and (845 < quest:GetTimeOfDay() and quest:GetTimeOfDay() < 1615) then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetThingHasInformation(me, false, false, true)
                quest:SetStateBool("HasInformation", true)
            end
        else
            if quest:IsActiveThreadTerminating() then return end
            quest:ClearThingHasInformation(me)
            quest:SetStateBool("HasInformation", false)
        end
        if quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
            if quest:IsActiveThreadTerminating() then return end
            while quest:GetMasterGameState("HeroDollsScriptUsingTeacher") do
                if not quest:NewScriptFrame(me) then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("HasInformation", false)
        end
        if not me:IsTalkedToByHero() then goto continue_1 end
        if quest:IsActiveThreadTerminating() then return end
        movie = resources:StartMovie("")
        -- reviewed native interface-pointer staging: CVar12 = *(this + 4)
        scratchValue = nil --[[reviewed native interface-pointer staging]]
        scratchValue25 = scratchValue
        quest:PauseAllNonScriptedEntities(true)
        meControl = resources:MemberResource("seh_me", me)
        resources:PrepareResource(meControl)
        while not resources:TryAcquire(meControl, me, 4) do
            if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if not quest:GetStateBool("DoneIntro") then
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            if not quest:GetMasterGameState("HeroExposedLadyGrey") then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                local scratchValue18 = resources:ScriptThing(resources:MemberResource("seh_me", me))
                if 0.0 < quest:GetHealth(scratchValue18) then
                    local hero = quest:GetHero()
                    speechResult = me:Speak(hero, "TEXT_QST_B16_INTRO", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local scratchValue19 = resources:ScriptThing(resources:MemberResource("seh_me", me))
                if 0.0 < quest:GetHealth(scratchValue19) then
                    local hero2 = quest:GetHero()
                    speechResult2 = me:Speak(hero2, "TEXT_QST_B16_INTRO_B", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            ReleaseEverything2()
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BOOK_COLLECTION", getActiveQuestName, false)
            local pQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "", "")
            quest:SetStateBool("DoneIntro", true)
        elseif quest:GetTimeOfDay() < 845 or 1615 < quest:GetTimeOfDay() then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue20 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            if 0.0 < quest:GetHealth(scratchValue20) then
                -- reviewed native dispatch staging: xStack_7c = *pCVar11;
                local hero3 = quest:GetHero()
                speechResult3 = me:Speak(hero3, "TEXT_QST_B16_BOOK_WRONG_TIME", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                end
                if quest:IsActiveThreadTerminating() then
                    -- reviewed native interface-table staging: iVar8 = *CVar12
                    scratchValue17 = nil --[[reviewed native interface-table staging]]
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
            end
        else
            if quest:GetStateInt("LastBookRequested") < 0 then
                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                local scratchValue21 = resources:ScriptThing(resources:MemberResource("seh_me", me))
                if 0.0 < quest:GetHealth(scratchValue21) then
                    -- reviewed native register-dispatch staging: CVar12 = *pCVar11
                    scratchValue = nil --[[reviewed native register-dispatch staging]]
                    local hero4 = quest:GetHero()
                    speechResult4 = me:Speak(hero4, "TEXT_QST_B16_BOOK_LOOK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            -- reviewed native saved-table staging: iVar8 = *xStack_7c
                            scratchValue17 = nil --[[reviewed native interface-table staging]]
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                end
                LookForBook(quest, me)
                goto LAB_00e55891
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue22 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            if 0.0 < quest:GetHealth(scratchValue22) then
                local hero5 = quest:GetHero()
                speechResult5 = me:Speak(hero5, "TEXT_QST_B16_BOOK_REQUEST_AGAIN", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            scratchValue17 = quest:GetStateInt("LastBookRequested")
            -- reviewed native definition-vector staging: iVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0x94)
            booksDonated = nil --[[reviewed native definition-vector staging]]
            local hero6 = quest:GetHero()
            if quest:IsObjectInThingsPossession(quest:GetStateStringVectorAt("BookDef", scratchValue17), hero6) then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                if AskForBook(quest, me, quest:GetStateInt("LastBookRequested"), "TEXT_QST_B16_BOOK_REFUSE_AGAIN") then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    quest:SetStateInt("LastBookRequested", 0xffffffff)
                else
                    if quest:IsActiveThreadTerminating() then
                        -- reviewed native saved-table staging: iVar8 = *xStack_7c
                        scratchValue17 = nil --[[reviewed native interface-table staging]]
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    LookForBook(quest, me)
                end
                goto LAB_00e55891
            end
            if quest:IsActiveThreadTerminating() then
                -- reviewed native saved-table staging: iVar8 = *xStack_7c
                scratchValue17 = nil --[[reviewed native interface-table staging]]
                ReleaseEverything2(); return
            end
            local scratchValue23 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            if 0.0 < quest:GetHealth(scratchValue23) then
                -- reviewed native register-dispatch staging: CVar12 = *pCVar11
                scratchValue = nil --[[reviewed native register-dispatch staging]]
                local hero7 = quest:GetHero()
                speechResult6 = me:Speak(hero7, "TEXT_QST_B16_BOOK_LOST", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                end
                if quest:IsActiveThreadTerminating() then
                    -- reviewed native saved-table staging: iVar8 = *xStack_7c
                    scratchValue17 = nil --[[reviewed native interface-table staging]]
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
            end
            LookForBook(quest, me)
        end
        ::LAB_00e55891::
        resources:PrepareResource(meControl)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:ClearThingHasInformation(me)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(getActiveQuestName2, false, false, false)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then return end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local this_00 = resources:MemberResource("seh_me", me)
                resources:PrepareResource(this_00)
                while not resources:TryAcquire(this_00, me, 4) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                local scratchValue24 = resources:ScriptThing(resources:MemberResource("seh_me", me))
                if 0.0 < quest:GetHealth(scratchValue24) then
                    local hero8 = quest:GetHero()
                    if not me:Speak(hero8, "TEXT_QST_B16_COMPLETE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                end
                resources:PrepareResource(this_00)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
        else
            if quest:IsActiveThreadTerminating() then return end
            while quest:GetMasterGameState("HeroDollsScriptUsingTeacher") do
                if not quest:NewScriptFrame(me) then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        if not quest:NewScriptFrame(me) then return end
    until false
end

-- BS_Teacher.Init (retail 0x00e54630)
function Init(quest, me)
    badBooksForHat_ = quest:ReadGlobalGameData(SCRIPT_DEF.BadBooksForHat)
    goodBooksForHat_ = quest:ReadGlobalGameData(SCRIPT_DEF.GoodBooksForHat)
    moralityReward = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BooksMoralityReward)))
    booksWanted_ = quest:ReadGlobalGameData(SCRIPT_DEF.BooksNumberWanted)
    booksAccepted_ = quest:ReadGlobalGameData(SCRIPT_DEF.BooksNumberAccepted)
    booksComment_ = quest:ReadGlobalGameData(SCRIPT_DEF.BooksNumberComment)
end

-- BS_Teacher.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BS_Teacher.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

-- BS_Teacher.AskForBook (retail 0x00e55ce0)
-- E55CE0: bsim names this body NScript::CV_BookCollectingScript::CBS_Teacher::AskForBook (a homologous script member); no PDB name
function AskForBook(quest, me, param1, param2)
    local resources = quest:RetailResources()
    local questionAnswer, hero4, pOther, prize, predicateResult, resource, actorMap, actorMap2
    local scratchValue
    local booksWanted = booksWanted_
    local goodBooksForHat = goodBooksForHat_
    local badBooksForHat = badBooksForHat_
    local value = param1
    predicateResult = false
    param1 = "TEXT_QST_B16_OFFER_BOOK_" .. tostring(param1)
    quest:GiveHeroYesNoQuestion(param1, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
    while questionAnswer < 0 do
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
        else
            do return false end
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
        end
    end
    if quest:IsActiveThreadTerminating() then
        return false
    end
    ::FLOW_after_lab_00e55dcf::
    local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if questionAnswer ~= 1 then
        if isActiveThreadTerminating then
            do return false end
        end
        if value < booksWanted then
            if quest:IsActiveThreadTerminating() then
                do return false end;
                goto FLOW_after_lab_00e55dcf
            end
            quest:SetStateInt("LastBookRequested", value)
            resources:MemberResource("seh_me", me)
            local x_stk_1c_1 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            local fret_01 = quest:GetHealth(x_stk_1c_1)
            if fret_01 <= 0.0 then goto LAB_00e566a1 end
            local line = param2
            local uVar12_pushed = 2
            local hero5 = quest:GetHero()
            if not me:Speak(hero5, line, 2, false, true, false) then
                do return false end
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then
                    do return false end;
                    goto FLOW_after_lab_00e55dcf
                end
            end
        else
            if quest:IsActiveThreadTerminating() then
                do return false end;
                goto FLOW_after_lab_00e55dcf
            end
            resources:MemberResource("seh_me", me)
            local x_stk_1c_2 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            local fret_02 = quest:GetHealth(x_stk_1c_2)
            if fret_02 <= 0.0 then goto LAB_00e566a1 end
            prize = "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND"
            local hero = quest:GetHero()
            if not me:Speak(hero, prize, 2, false, true, false) then
                do return false end
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then
                    do return false end;
                    goto FLOW_after_lab_00e55dcf
                end
            end
        end
        if quest:IsActiveThreadTerminating() then
            do return false end;
            goto FLOW_after_lab_00e55dcf
        end
        goto LAB_00e566a1
    end
    if isActiveThreadTerminating then
        do return false end
    end
    quest:SetStateBool("ReadingBook", true)
    quest:TakeObjectFromHero(quest:GetStateStringVectorAt("BookDef", value))
    quest:SetStateFlagVectorAt("BookDonated", value, true)
    quest:SetStateInt("BooksDonated", quest:GetStateInt("BooksDonated") + 1)
    predicateResult = true
    if value < booksWanted then
        if quest:IsActiveThreadTerminating() then
            do return false end;
            goto FLOW_after_lab_00e55dcf
        end
        quest:SetStateInt("GoodBooksDonated", quest:GetStateInt("GoodBooksDonated") + 1)
        resources:MemberResource("seh_me", me)
        local x_stk_1c_3 = resources:ScriptThing(resources:MemberResource("seh_me", me))
        local fret_0 = quest:GetHealth(x_stk_1c_3)
        if 0.0 < fret_0 then
            prize = "TEXT_QST_B16_BOOK_ACCEPTED"
            local hero2 = quest:GetHero()
            if not me:Speak(hero2, prize, 2, false, true, false) then
                do return false end
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then
                    do return false end;
                    goto FLOW_after_lab_00e55dcf
                end
            end
            if quest:IsActiveThreadTerminating() then
                do return false end;
                goto FLOW_after_lab_00e55dcf
            end
        end
        quest:GiveHeroMorality(moralityReward)
        helper_E56D10(quest, me, 44)
    else
        if quest:IsActiveThreadTerminating() then
            do return false end;
            goto FLOW_after_lab_00e55dcf
        end
        resources:MemberResource("seh_me", me)
        local x_stk_1c_4 = resources:ScriptThing(resources:MemberResource("seh_me", me))
        local fret_00 = quest:GetHealth(x_stk_1c_4)
        if 0.0 < fret_00 then
            prize = "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY"
            local hero3 = quest:GetHero()
            if not me:Speak(hero3, prize, 2, false, true, false) then
                do return false end
            end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then
                    do return false end;
                    goto FLOW_after_lab_00e55dcf
                end
            end
            if quest:IsActiveThreadTerminating() then
                do return false end;
                goto FLOW_after_lab_00e55dcf
            end
        end
    end
    pOther = resources:MemberResource("seh_me", me)
    scratchValue = "CS_SCHOOLBOOK_" .. tostring(value)
    resource = resources:NewResource()
    hero4 = quest:GetHero()
    resources:TryAcquire(resource, hero4, 4)
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "Hero", resource)
    resources:SetActor(actorMap2, "Teacher", pOther)
    actorMap = resources:NewStringMap()
    GetBookSpecificArgs(quest, me, value, actorMap)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(scratchValue, actorMap2, actorMap, false, true)
    if not quest:GetStateBool("HatRewarded") then
        if quest:IsActiveThreadTerminating() then
            -- LAB_00e560d5: (native jump target)
            resources:DestroyStringMap(actorMap)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource)
            return false
        end
        if quest:GetStateInt("GoodBooksDonated") < goodBooksForHat then
            if badBooksForHat <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                if quest:IsActiveThreadTerminating() then
                    resources:DestroyStringMap(actorMap)
                    resources:DestroyActorMap(actorMap2)
                    resources:ReleaseResource(resource)
                    do return false end
                end
                resources:ClearStringMap(actorMap)
                resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_EVIL")
                resources:SetActor(actorMap2, "Hero", resource)
                resources:SetActor(actorMap2, "Teacher", pOther)
                resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                quest:SetStateBool("HatRewarded", true)
            end
        else
            if quest:IsActiveThreadTerminating() then
                resources:DestroyStringMap(actorMap)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource)
                do return false end
            end
            resources:ClearStringMap(actorMap)
            resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_GOOD")
            resources:SetActor(actorMap2, "Hero", resource)
            resources:SetActor(actorMap2, "Teacher", pOther)
            resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
            quest:SetStateBool("HatRewarded", true)
        end
    elseif not quest:GetStateBool("KeyRewarded") then
        if quest:IsActiveThreadTerminating() then
            ::LAB_00e560d5_c17::
            resources:DestroyStringMap(actorMap)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource)
            do return false end
            if quest:GetStateInt("GoodBooksDonated") < goodBooksForHat then
                if badBooksForHat <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e560d5_c17 end
                    resources:ClearStringMap(actorMap)
                    resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_EVIL")
                    resources:SetActor(actorMap2, "Hero", resource)
                    resources:SetActor(actorMap2, "Teacher", pOther)
                    resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                    quest:SetStateBool("HatRewarded", true)
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e560d5_c17 end
                resources:ClearStringMap(actorMap)
                resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_GOOD")
                resources:SetActor(actorMap2, "Hero", resource)
                resources:SetActor(actorMap2, "Teacher", pOther)
                resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                quest:SetStateBool("HatRewarded", true)
            end
            goto FLOW_after_lab_00e560d5_216
        end
        if quest:GetStateInt("BooksDonated") == booksAccepted_ then
            if quest:IsActiveThreadTerminating() then
                ::LAB_00e560d5_c18::
                resources:DestroyStringMap(actorMap)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource)
                do return false end
                if quest:GetStateInt("GoodBooksDonated") < goodBooksForHat then
                    if badBooksForHat <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e560d5_c18 end
                        resources:ClearStringMap(actorMap)
                        resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_EVIL")
                        resources:SetActor(actorMap2, "Hero", resource)
                        resources:SetActor(actorMap2, "Teacher", pOther)
                        resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                        quest:SetStateBool("HatRewarded", true)
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e560d5_c18 end
                    resources:ClearStringMap(actorMap)
                    resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_GOOD")
                    resources:SetActor(actorMap2, "Hero", resource)
                    resources:SetActor(actorMap2, "Teacher", pOther)
                    resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                    quest:SetStateBool("HatRewarded", true)
                end
                goto FLOW_after_lab_00e560d5_216
            end
            resources:ClearStringMap(actorMap)
            resources:SetString(actorMap, "$PRIZE", "OBJECT_SILVER_KEY")
            resources:SetActor(actorMap2, "Hero", resource)
            resources:SetActor(actorMap2, "Teacher", pOther)
            resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_KEY", actorMap2, actorMap, false, true)
            quest:SetStateBool("KeyRewarded", true)
        end
    end
    ::FLOW_after_lab_00e560d5_216::
    quest:FadeScreenIn()
    quest:FixMovieSequenceCamera(false)
    resources:DestroyStringMap(actorMap)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
    quest:SetStateBool("ReadingBook", false)
    quest:CreateThread("BookReaction", {args = {value}})  -- native parent-quest worker BookReaction, bound values
    ::LAB_00e566a1::
    return predicateResult
end

-- BS_Teacher.helper_E56D10 (retail 0x00e56d10)
function helper_E56D10(quest, me, param2)
    local scratchValue, scratchValue4
    local scratchValue7 = param2
    local hero = quest:GetHero()
    quest:EntityPostOpinionDeedToRecipient(hero, scratchValue7, me)
    scratchValue = 0
    local scratchValue9 = "boy" .. tostring(0)
    local getThingWithScriptName4 = quest:GetThingWithScriptName(scratchValue9)
    while getThingWithScriptName4 ~= nil do
        if not (getThingWithScriptName4 ~= nil and getThingWithScriptName4:IsAlive()) then break end
        local getThingWithScriptName = getThingWithScriptName4
        local scratchValue5 = param2
        hero = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(hero, scratchValue5, getThingWithScriptName)
        scratchValue = scratchValue + 1
        scratchValue9 = "boy" .. tostring(scratchValue)
        getThingWithScriptName4 = quest:GetThingWithScriptName(scratchValue9)
    end
    scratchValue4 = 0
    while true do
        scratchValue9 = "girl" .. tostring(scratchValue4)
        getThingWithScriptName4 = quest:GetThingWithScriptName(scratchValue9)
        if not (getThingWithScriptName4 ~= nil and not getThingWithScriptName4:IsNull()) then break end
        if not (getThingWithScriptName4 ~= nil and getThingWithScriptName4:IsAlive()) then break end
        local getThingWithScriptName3 = getThingWithScriptName4
        local scratchValue6 = param2
        hero = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(hero, scratchValue6, getThingWithScriptName3)
        scratchValue4 = scratchValue4 + 1
    end
    getThingWithScriptName4 = nil
end

-- BS_Teacher.GetBookSpecificArgs (retail 0x00e57020)
-- E57020: bsim names this body NScript::CV_BookCollectingScript::CBS_Teacher::GetBookSpecificArgs (a homologous script member); no PDB name
function GetBookSpecificArgs(quest, me, param1, param)
    local resources = quest:RetailResources()
    local scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue7
    if param1 == 15 then
        if quest:IsActiveThreadTerminating() then return end
        local getMasterGameState = quest:GetMasterGameState("PostSavePosition")
        if getMasterGameState < 1251 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue3 = "TEXT_CS_B16_BOOK15_90D"
        elseif getMasterGameState < 1701 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue3 = "TEXT_CS_B16_BOOK15_90A"
        elseif getMasterGameState < 2301 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue3 = "TEXT_CS_B16_BOOK15_90B"
        elseif getMasterGameState < 2601 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue3 = "TEXT_CS_B16_BOOK15_90C"
        else
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue3 = "TEXT_CS_B16_BOOK15_90B"
        end
        resources:SetString(param, param1, scratchValue3)
        return
    end
    if param1 == 17 then
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetHeroAttractiveness() <= 0.5 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue4 = "TEXT_CS_B16_BOOK17_20A"
        else
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue4 = "TEXT_CS_B16_BOOK17_20B"
        end
        resources:SetString(param, param1, scratchValue4)
        return
    end
    if param1 == 19 then
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetMasterGameState("PostSavePosition") < 701 then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue5 = "TEXT_CS_B16_BOOK19_80A"
        elseif not quest:GetMasterGameState("BanditCampTwinbladeKilled") then
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue5 = "TEXT_CS_B16_BOOK19_80B"
        else
            if quest:IsActiveThreadTerminating() then return end
            param1 = "$ARG1"
            scratchValue5 = "TEXT_CS_B16_BOOK19_80C"
        end
        resources:SetString(param, param1, scratchValue5)
        return
    end
    if param1 ~= 21 then
        if param1 == 22 then
            if quest:IsActiveThreadTerminating() then return end
            if 0.5 < quest:GetHeroScariness() then
                if quest:IsActiveThreadTerminating() then return end
                param1 = "$ARG1"
                scratchValue6 = "TEXT_CS_B16_BOOK22_20B"
            else
                if quest:IsActiveThreadTerminating() then return end
                param1 = "$ARG1"
                scratchValue6 = "TEXT_CS_B16_BOOK22_20A"
            end
        else
            if param1 ~= 23 then
                return
            end
            if quest:IsActiveThreadTerminating() then return end
            local fret_01 = quest:GetHeroAttractiveness()
            if -0.5 <= fret_01 then
                if 0.5 < quest:GetHeroAttractiveness() then
                    if quest:IsActiveThreadTerminating() then return end
                    param1 = "$ARG1"
                    scratchValue6 = "TEXT_CS_B16_BOOK23_10C"
                else
                    if quest:IsActiveThreadTerminating() then return end
                    param1 = "$ARG1"
                    scratchValue6 = "TEXT_CS_B16_BOOK23_10B"
                end
            else
                if quest:IsActiveThreadTerminating() then return end
                param1 = "$ARG1"
                scratchValue6 = "TEXT_CS_B16_BOOK23_10A"
            end
        end
        resources:SetString(param, param1, scratchValue6)
        return
    end
    if quest:IsActiveThreadTerminating() then return end
    param1 = quest:GetMasterGameState("TeddySolution")
    local arg1 = "TEXT_CS_B16_BOOK21_20" .. param1
    local this_00 = param
    resources:SetString(param, "$ARG1", arg1)
    param = "$ARG2"
    local scratchValue = "TEXT_CS_B16_BOOK21_30" .. param1
    resources:SetString(this_00, param, scratchValue)
    if param1 == 0 then
        do goto LAB_00e57345 end
        goto LAB_00e573aa
    else
        local predicateResult = param1 == "A"
        -- reviewed native scratch-byte spill: native_arg_param_2 = (map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)CONCAT31(native_arg_param_2._1_3_,cVar2);
        if predicateResult then goto LAB_00e573aa end
        goto LAB_00e57345
    end
    goto FLOW_past_lab_00e573aa
    ::LAB_00e573aa::
    if quest:IsActiveThreadTerminating() then return end
    param = "$ARG3"
    scratchValue7 = "TEXT_CS_B16_BOOK21_40A"
    ::FLOW_past_lab_00e573aa::
    resources:SetString(this_00, param, scratchValue7)
    do return end
    ::LAB_00e57345::
    if quest:IsActiveThreadTerminating() then return end
    param = "$ARG3"
    resources:SetString(this_00, param, "TEXT_CS_B16_BOOK21_40B")
end

-- BS_Teacher.LookForBook (retail 0x00e57530)
-- E57530: bsim names this body NScript::CV_BookCollectingScript::CBS_Teacher::LookForBook (a homologous script member); no PDB name
function LookForBook(quest, me)
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, ctr_CVar, native_scalar_18, speechResult, value, scratchValue10
    local x_stk_14_1
    local booksInGame = quest:GetStateInt("BooksInGame")
    local booksAccepted = booksAccepted_
    local booksComment = booksComment_
    value = 0xffffffff
    ctr_CVar = 0
    scratchValue10 = 0
    native_scalar_18 = 0xffffffff
    if booksInGame < 1 then
        goto LAB_00e57678
    else
        repeat
            if (((ctr_CVar == booksWanted_) or (ctr_CVar == booksAccepted)) or (ctr_CVar == booksComment)) and (-1 < ((value + 2147483648) % 4294967296 - 2147483648)) then
                if quest:IsActiveThreadTerminating() then return end
                break
            end
            local hero = quest:GetHero()
            value = native_scalar_18
            if not quest:IsObjectInThingsPossession(quest:GetStateStringVectorAt("BookDef", ctr_CVar), hero) then
                ctr_CVar = ctr_CVar + 1
            else
                if quest:IsActiveThreadTerminating() then return end
                if ctr_CVar == quest:GetStateInt("LastBookRequested") or (quest:GetStateFlagVectorAt("BookDonated", ctr_CVar) and 1 or 0) ~= 0 then
                    value = native_scalar_18
                    if native_scalar_18 ~= 0xffffffff then ctr_CVar = ctr_CVar + 1; goto continue_1 end
                    if quest:IsActiveThreadTerminating() then return end
                    native_scalar_18 = 0xfffffffe
                    value = 0xfffffffe
                else
                    if quest:IsActiveThreadTerminating() then return end
                    isActiveThreadTerminating = scratchValue10 == 0
                    if isActiveThreadTerminating then
                        native_scalar_18 = ctr_CVar
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    end
                    if isActiveThreadTerminating then
                        return
                    end
                    value = native_scalar_18
                    if (quest:GetStateFlagVectorAt("BookOwned", ctr_CVar) and 1 or 0) == 0 then
                        if quest:IsActiveThreadTerminating() then return end
                        quest:SetStateFlagVectorAt("BookOwned", ctr_CVar, true)
                        scratchValue10 = 1
                    end
                end
                ctr_CVar = ctr_CVar + 1
            end
            ::continue_1::
        until not (ctr_CVar < booksInGame)
        if value ~= 0xfffffffe then goto LAB_00e57678 end
    end
    goto FLOW_past_lab_00e57678
    ::LAB_00e57678::
    if ((value + 2147483648) % 4294967296 - 2147483648) < booksComment then
        if value ~= 0xffffffff then
            if booksAccepted <= ((value + 2147483648) % 4294967296 - 2147483648) then
                if quest:IsActiveThreadTerminating() then return end
                resources:MemberResource("seh_me", me)
                local scratchValue6 = resources:ScriptThing(resources:MemberResource("seh_me", me))
                if quest:GetHealth(scratchValue6) <= 0.0 then
                    return
                end
                x_stk_14_1 = nil --[[reviewed native dispatch staging]]
                local line = "TEXT_QST_B16_BOOK_COMMENT_" .. tostring(((value + 2147483648) % 4294967296 - 2147483648) - booksAccepted)
                local hero2 = quest:GetHero()
                me:Speak(hero2, line, 0, false, true, false)
                if not resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me)) then quest:IsActiveThreadTerminating(); return end
                repeat
                    if not quest:NewScriptFrame(me) then return end
                until not resources:IsPerformingScriptTask(resources:MemberResource("seh_me", me))
                quest:IsActiveThreadTerminating()
                do return end
                quest:IsActiveThreadTerminating()
                return
            end
            if quest:IsActiveThreadTerminating() then return end
            local x_stk_14_2 = "TEXT_QST_B16_BOOK_REQUEST_" .. tostring((value + 2147483648) % 4294967296 - 2147483648)
            resources:MemberResource("seh_me", me)
            local scratchValue7 = resources:ScriptThing(resources:MemberResource("seh_me", me))
            if not (0.0 < quest:GetHealth(scratchValue7)) then
                AskForBook(quest, me, value, "TEXT_QST_B16_BOOK_REFUSED")
            else
                local hero3 = quest:GetHero()
                if not me:Speak(hero3, x_stk_14_2, 0, false, true, 0.0 ~= 0) then
                    return
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then return end
                end
                value = native_scalar_18
                if quest:IsActiveThreadTerminating() then return end
                AskForBook(quest, me, value, "TEXT_QST_B16_BOOK_REFUSED")
            end
            return
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:MemberResource("seh_me", me)
        local scratchValue8 = resources:ScriptThing(resources:MemberResource("seh_me", me))
        if quest:GetHealth(scratchValue8) <= 0.0 then
            return
        end
        local hero4 = quest:GetHero()
        speechResult = me:Speak(hero4, "TEXT_QST_B16_BOOK_NONE_FOUND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
        if not me:IsPerformingScriptTask() then quest:IsActiveThreadTerminating(); return end
        repeat
            if not quest:NewScriptFrame(me) then return end
        until not me:IsPerformingScriptTask()
        quest:IsActiveThreadTerminating()
        do return end
        quest:IsActiveThreadTerminating()
        return
    end
    ::FLOW_past_lab_00e57678::
    if quest:IsActiveThreadTerminating() then return end
    resources:MemberResource("seh_me", me)
    local scratchValue9 = resources:ScriptThing(resources:MemberResource("seh_me", me))
    if quest:GetHealth(scratchValue9) <= 0.0 then
        return
    end
    local hero5 = quest:GetHero()
    if not me:Speak(hero5, "TEXT_QST_B16_BOOK_NOT_FOUND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then
        return
    end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then return end
    end
    quest:IsActiveThreadTerminating()
end

