-- Readable native conversion: BS_Teacher. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_RANDOM_NO_REPEAT = 2  -- ETextGroupSelectionMethod

local helpers = require("V_BookCollecting.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local booksAccepted, self0X14, badBooksForHat, goodBooksForHat, moralityReward, booksWanted
local booksComment, self0X34

-- BS_Teacher.Main (retail 0x00e54e90)
function Main(quest, me)
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, scratchValue4, scratchValue24, scratchValue25, meControl, hero
    local scratchValue26, speechResult, movie, x_stk_58_1
    local function ReleaseEverything()
        local scratchValue25 = nil --[[unresolved native value]]
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything3()
        resources:DestroyMovie(movie)
    end
    quest:SetThingPersistent(me, true)
    local booksDonated = quest:GetStateInt("BooksDonated")
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
    while quest:GetStateInt("BooksDonated") < booksAccepted do
        if not quest:NewScriptFrame(me) then return end
        if not quest:GetStateBool("HasInformation") or (quest:GetTimeOfDay() < 1616 and 844 < quest:GetTimeOfDay()) then
            if not quest:GetStateBool("HasInformation") and (845 < quest:GetTimeOfDay() and quest:GetTimeOfDay() < 1615) then
                quest:SetThingHasInformation(me, false, false, true)
                quest:SetStateBool("HasInformation", true)
            end
        else
            quest:ClearThingHasInformation(me)
            quest:SetStateBool("HasInformation", false)
        end
        if quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
            if quest:IsActiveThreadTerminating() then return end
            while quest:GetMasterGameState("HeroDollsScriptUsingTeacher") do
                if not quest:NewScriptFrame(me) then return end
            end
            quest:SetStateBool("HasInformation", false)
        end
        if not me:IsTalkedToByHero() then goto continue_1 end
        if quest:IsActiveThreadTerminating() then return end
        movie = resources:StartMovie("")
        -- TODO(native): CVar12 = *(this + 4)
        scratchValue = nil --[[unresolved native value]]
        quest:PauseAllNonScriptedEntities(true)
        meControl = resources:MemberResource("seh_me", me)
        resources:PrepareResource(meControl)
        while not resources:TryAcquire(meControl, me, 4) do
            if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if not quest:GetStateBool("DoneIntro") then
            if not quest:GetMasterGameState("HeroExposedLadyGrey") then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                hero = nil --[[unresolved native value]]
                local fret_00 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_00 then
                    -- TODO(native): CVar2 = *pCVar11
                    scratchValue4 = nil --[[unresolved native value]]
                    scratchValue26 = "TEXT_QST_B16_INTRO"
                    -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                    while nil --[[unresolved native value]] do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)CVar12 + 0x5ec))(0);
                            resources:DestroyMovie(movie)
                            return
                        end
                        -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                end
            else
                -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                hero = nil --[[unresolved native value]]
                local fret_0 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_0 then
                    -- TODO(native): CVar2 = *pCVar11
                    scratchValue4 = nil --[[unresolved native value]]
                    scratchValue26 = "TEXT_QST_B16_INTRO_B"
                    -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                    while nil --[[unresolved native value]] do
                        if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                        -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BOOK_COLLECTION", quest:GetActiveQuestName(), false)
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "", "")
            quest:SetStateBool("DoneIntro", true)
        elseif quest:GetTimeOfDay() < 845 or 1615 < quest:GetTimeOfDay() then
            -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
            hero = nil --[[unresolved native value]]
            local fret_04 = quest:GetHealth(nil --[[missing]])
            x_stk_58_1 = 0
            if 0.0 < fret_04 then
                -- TODO(native): xStack_7c = *pCVar11;
                scratchValue26 = "TEXT_QST_B16_BOOK_WRONG_TIME"
                -- TODO(native): (**(code **)((int)xStack_7c + 0x34))(pCVar9,pcVar13);
                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                while nil --[[unresolved native value]] do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): iVar8 = *CVar12
                    scratchValue25 = nil --[[unresolved native value]]
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
            end
        else
            if quest:GetStateInt("LastBookRequested") < 0 then
                -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                hero = nil --[[unresolved native value]]
                local fret_03 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_03 then
                    -- TODO(native): CVar12 = *pCVar11
                    scratchValue = nil --[[unresolved native value]]
                    scratchValue26 = "TEXT_QST_B16_BOOK_LOOK"
                    -- TODO(native): (**(code **)((int)CVar12 + 0x34))(pCVar9,pcVar13);
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                    while nil --[[unresolved native value]] do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): iVar8 = *xStack_7c
                            scratchValue25 = nil --[[unresolved native value]]
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
                end
                LookForBook(quest, me)
                goto LAB_00e55891
            end
            -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
            hero = nil --[[unresolved native value]]
            local fret_01 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_01 then
                -- TODO(native): CVar2 = *pCVar11
                scratchValue4 = nil --[[unresolved native value]]
                scratchValue26 = "TEXT_QST_B16_BOOK_REQUEST_AGAIN"
                -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                while nil --[[unresolved native value]] do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            -- TODO(native): iVar1 = *(self0X14 + 0x94)
            scratchValue24 = nil --[[unresolved native value]]
            if quest:IsObjectInThingsPossession(scratchValue26, hero_) then
                if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
                if AskForBook(quest, me, quest:GetStateInt("LastBookRequested"), "TEXT_QST_B16_BOOK_REFUSE_AGAIN") then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))();
                        resources:DestroyMovie(movie)
                        return
                    end
                    quest:SetStateInt("LastBookRequested", 0xffffffff)
                else
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): iVar8 = *xStack_7c
                        scratchValue25 = nil --[[unresolved native value]]
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    LookForBook(quest, me)
                end
                goto LAB_00e55891
            end
            if quest:IsActiveThreadTerminating() then
                -- TODO(native): iVar8 = *xStack_7c
                scratchValue25 = nil --[[unresolved native value]]
                ReleaseEverything2(); return
            end
            -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
            hero = nil --[[unresolved native value]]
            local fret_02 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_02 then
                -- TODO(native): CVar12 = *pCVar11
                scratchValue = nil --[[unresolved native value]]
                scratchValue26 = "TEXT_QST_B16_BOOK_LOST"
                -- TODO(native): (**(code **)((int)CVar12 + 0x34))(pCVar9,pcVar13);
                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                while nil --[[unresolved native value]] do
                    if not quest:NewScriptFrame(me) then ReleaseEverything3(); return end
                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): iVar8 = *xStack_7c
                    scratchValue25 = nil --[[unresolved native value]]
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
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
            if me:IsTalkedToByHero() then
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local this_00 = resources:MemberResource("seh_me", me)
                resources:PrepareResource(this_00)
                while not resources:TryAcquire(this_00, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                -- TODO(native): pCVar9 = (**(*this_00 + 0x30))()
                hero = nil --[[unresolved native value]]
                local fret_05 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_05 then
                    speechResult = me:Speak(hero_, "TEXT_QST_B16_COMPLETE", 0.0, x_stk_58_1 ~= 0, false, nil --[[missing]])
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
            while quest:GetMasterGameState("HeroDollsScriptUsingTeacher") do
                if not quest:NewScriptFrame(me) then return end
            end
        end
        if not quest:NewScriptFrame(me) then return end
    until false
end

-- BS_Teacher.Init (retail 0x00e54630)
function Init(quest, me)
    badBooksForHat = quest:ReadGlobalGameData(1264)
    goodBooksForHat = quest:ReadGlobalGameData(1268)
    moralityReward = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(1272)))
    booksWanted = quest:ReadGlobalGameData(1248)
    booksAccepted = quest:ReadGlobalGameData(1252)
    booksComment = quest:ReadGlobalGameData(1256)
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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local questionAnswer, pOther, predicateResult, resource, actorMap, actorMap2, scratchValue
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
            goto FLOW_after_lab_00e55dcf
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
            goto FLOW_after_lab_00e55dcf
        end
        if value < booksWanted then
            if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
            quest:SetStateInt("LastBookRequested", value)
            resources:MemberResource("seh_me", me)
            local fret_01 = quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_me", me)))
            if fret_01 <= 0.0 then goto LAB_00e566a1 end
            me:Speak(hero, param2, 2, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then do return false end; goto FLOW_after_lab_00e55dcf end
            end
        else
            if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
            resources:MemberResource("seh_me", me)
            local fret_02 = quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_me", me)))
            if fret_02 <= 0.0 then goto LAB_00e566a1 end
            me:Speak(hero, "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then do return false end; goto FLOW_after_lab_00e55dcf end
            end
        end
        if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
        goto LAB_00e566a1
    end
    if isActiveThreadTerminating then
        do return false end
        goto FLOW_after_lab_00e55dcf
    end
    quest:SetStateBool("ReadingBook", true)
    quest:TakeObjectFromHero(nil --[[missing]])
    -- TODO(native): *(undefined1 *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + (int)value) = 1;
    quest:SetStateInt("BooksDonated", quest:GetStateInt("BooksDonated") + 1)
    predicateResult = true
    if value < booksWanted then
        if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
        quest:SetStateInt("GoodBooksDonated", quest:GetStateInt("GoodBooksDonated") + 1)
        resources:MemberResource("seh_me", me)
        local fret_0 = quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_me", me)))
        if 0.0 < fret_0 then
            me:Speak(hero, "TEXT_QST_B16_BOOK_ACCEPTED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then do return false end; goto FLOW_after_lab_00e55dcf end
            end
            if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
        end
        quest:GiveHeroMorality(moralityReward)
        helper_E56D10(quest, me, 44)
    else
        if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
        resources:MemberResource("seh_me", me)
        local fret_00 = quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_me", me)))
        if 0.0 < fret_00 then
            me:Speak(hero, "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then do return false end; goto FLOW_after_lab_00e55dcf end
            end
            if quest:IsActiveThreadTerminating() then do return false end; goto FLOW_after_lab_00e55dcf end
        end
    end
    pOther = resources:MemberResource("seh_me", me)
    scratchValue = "CS_SCHOOLBOOK_" .. tostring(value)
    resource = resources:NewResource()
    resources:TryAcquire(resource, hero, 4)
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "Hero", resource)
    -- TODO(native): resources:SetActor(xStack_3c, "Teacher", &pOther)
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
        ::FLOW_after_lab_00e560d5::
        if quest:GetStateInt("GoodBooksDonated") < goodBooksForHat then
            if badBooksForHat <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                if quest:IsActiveThreadTerminating() then
                    resources:DestroyStringMap(actorMap)
                    resources:DestroyActorMap(actorMap2)
                    resources:ReleaseResource(resource)
                    do return false end
                    goto FLOW_after_lab_00e560d5
                end
                -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
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
                goto FLOW_after_lab_00e560d5
            end
            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
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
                    -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
                    resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_EVIL")
                    resources:SetActor(actorMap2, "Hero", resource)
                    resources:SetActor(actorMap2, "Teacher", pOther)
                    resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                    quest:SetStateBool("HatRewarded", true)
                end
            end
            goto FLOW_after_lab_00e560d5_216
        end
        if quest:GetStateInt("BooksDonated") == booksAccepted then
            if quest:IsActiveThreadTerminating() then
                ::LAB_00e560d5_c18::
                resources:DestroyStringMap(actorMap)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource)
                do return false end
                if quest:GetStateInt("GoodBooksDonated") < goodBooksForHat then
                    if badBooksForHat <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e560d5_c18 end
                        -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
                        resources:SetString(actorMap, "$PRIZE", "OBJECT_HERO_HAT_WIZARD_EVIL")
                        resources:SetActor(actorMap2, "Hero", resource)
                        resources:SetActor(actorMap2, "Teacher", pOther)
                        resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", actorMap2, actorMap, false, true)
                        quest:SetStateBool("HatRewarded", true)
                    end
                end
                goto FLOW_after_lab_00e560d5_216
            end
            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
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
    local scratchValue, scratchValue2, scratchValue5, scratchValue6
    local hero = quest:GetHero()
    quest:EntityPostOpinionDeedToRecipient(hero, param2, me)
    scratchValue = 0
    local getThingWithScriptName = quest:GetThingWithScriptName("boy" .. tostring(0))
    while scratchValue5 ~= nil do
        -- TODO(native): cVar3 = (**(*uStack_14 + 0x12c))()
    --[[unresolved native value]]
        if not nil then break end
        quest:EntityPostOpinionDeedToRecipient(hero, param2, getThingWithScriptName)
        scratchValue = scratchValue + 1
        quest:GetThingWithScriptName("boy" .. tostring(scratchValue))
        -- TODO(native): piVar1 = *(pCVar6 + 0x8)
    --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar6 + 0x4)
    --[[unresolved native value]]
        if scratchValue6 ~= nil then
            scratchValue5 = nil
            scratchValue6 = nil
            if nil ~= nil then
                -- TODO(native): *piVar1 = *piVar1 + 1;
            end
        end
    end
    scratchValue2 = 0
    while true do
        quest:GetThingWithScriptName("girl" .. tostring(scratchValue2))
        -- TODO(native): piVar1 = *(pCVar6 + 0x8)
    --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar6 + 0x4)
    --[[unresolved native value]]
        if scratchValue6 ~= nil then
            scratchValue5 = nil
            scratchValue6 = nil
            if nil ~= nil then
                -- TODO(native): *piVar1 = *piVar1 + 1;
            end
        end
        if scratchValue5 == nil then break end
        -- TODO(native): cVar3 = (**(*uStack_14 + 0x12c))()
    --[[unresolved native value]]
        if not nil then break end
        quest:EntityPostOpinionDeedToRecipient(hero, param2, getThingWithScriptName)
        scratchValue2 = scratchValue2 + 1
    end
end

-- BS_Teacher.GetBookSpecificArgs (retail 0x00e57020)
-- E57020: bsim names this body NScript::CV_BookCollectingScript::CBS_Teacher::GetBookSpecificArgs (a homologous script member); no PDB name
function GetBookSpecificArgs(quest, me)
    local resources = quest:RetailResources()
    local arg, arg13, arg14, arg15, arg3
    if this == 15 then
        if quest:IsActiveThreadTerminating() then return end
        local getMasterGameState = quest:GetMasterGameState("PostSavePosition")
        if getMasterGameState < 1251 then
            arg = "TEXT_CS_B16_BOOK15_90D"
        elseif getMasterGameState < 1701 then
            arg = "TEXT_CS_B16_BOOK15_90A"
        elseif getMasterGameState < 2301 then
            arg = "TEXT_CS_B16_BOOK15_90B"
        elseif getMasterGameState < 2601 then
            arg = "TEXT_CS_B16_BOOK15_90C"
        else
            arg = "TEXT_CS_B16_BOOK15_90B"
        end
        resources:SetString(param_2, "$ARG1", arg)
        return
    end
    if "$ARG1" == 17 then
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetHeroAttractiveness() <= 0.5 then
            arg13 = "TEXT_CS_B16_BOOK17_20A"
        else
            arg13 = "TEXT_CS_B16_BOOK17_20B"
        end
        resources:SetString(param_2, "$ARG1", arg13)
        return
    end
    if "$ARG1" == 19 then
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetMasterGameState("PostSavePosition") < 701 then
            arg14 = "TEXT_CS_B16_BOOK19_80A"
        elseif not quest:GetMasterGameState("BanditCampTwinbladeKilled") then
            arg14 = "TEXT_CS_B16_BOOK19_80B"
        else
            arg14 = "TEXT_CS_B16_BOOK19_80C"
        end
        resources:SetString(param_2, "$ARG1", arg14)
        return
    end
    if "$ARG1" ~= 21 then
        if "$ARG1" == 22 then
            if quest:IsActiveThreadTerminating() then return end
            if 0.5 < quest:GetHeroScariness() then
                arg15 = "TEXT_CS_B16_BOOK22_20B"
            else
                arg15 = "TEXT_CS_B16_BOOK22_20A"
            end
        else
            if "$ARG1" ~= 23 then
                return
            end
            if quest:IsActiveThreadTerminating() then return end
            if -0.5 <= quest:GetHeroAttractiveness() then
                if 0.5 < quest:GetHeroAttractiveness() then
                    arg15 = "TEXT_CS_B16_BOOK23_10C"
                else
                    arg15 = "TEXT_CS_B16_BOOK23_10B"
                end
            else
                arg15 = "TEXT_CS_B16_BOOK23_10A"
            end
        end
        resources:SetString(param_2, "$ARG1", arg15)
        return
    end
    if quest:IsActiveThreadTerminating() then return end
    -- TODO(native): CCharString::CCharString((CCharString *)&this,(CCharString *)(*(int *)(this + 0x18) + 0x54));
    resources:SetString(param_2, "$ARG1", "TEXT_CS_B16_BOOK21_20" .. "$ARG1")
    resources:SetString(param_2, "$ARG2", "TEXT_CS_B16_BOOK21_30" .. "$ARG1")
    if "$ARG1" == 0 then
        goto LAB_00e57345
        goto LAB_00e573aa
    else
        local predicateResult = "$ARG1" == "A"
        -- TODO(native): param_2 = (map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)CONCAT31(param_2._1_3_,cVar2);
        if predicateResult then goto LAB_00e573aa end
        goto LAB_00e57345
    end
    goto FLOW_past_lab_00e573aa
    ::LAB_00e573aa::
    if quest:IsActiveThreadTerminating() then return end
    arg3 = "TEXT_CS_B16_BOOK21_40A"
    ::FLOW_past_lab_00e573aa::
    resources:SetString(param_2, "$ARG3", arg3)
    do return end
    ::LAB_00e57345::
    if quest:IsActiveThreadTerminating() then return end
    resources:SetString(param_2, "$ARG3", "TEXT_CS_B16_BOOK21_40B")
end

-- BS_Teacher.LookForBook (retail 0x00e57530)
-- E57530: bsim names this body NScript::CV_BookCollectingScript::CBS_Teacher::LookForBook (a homologous script member); no PDB name
function LookForBook(quest, me)
    local booksInGame = quest:GetStateInt("BooksInGame")
    local self_0x14
    local self_0x34
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local ctr_CVar, scratchValue, value, scratchValue6, x_stk_14_1, scratchValue7
    value = 0xffffffff
    ctr_CVar = 0
    if booksInGame < 1 then
        goto LAB_00e57678
    else
        repeat
            if (((ctr_CVar == booksWanted) or (ctr_CVar == booksAccepted)) or (ctr_CVar == booksComment)) and (-1 < value) then
                if quest:IsActiveThreadTerminating() then return end
                break
            end
            -- TODO(native): iVar7 = *(iVar7 + 0x94)
--[[unresolved native value]]
            local isObjectInThingsPossession = quest:IsObjectInThingsPossession(nil --[[missing]], hero)
            if not isObjectInThingsPossession then
                ctr_CVar = ctr_CVar + 1
            else
                if quest:IsActiveThreadTerminating() then return end
                -- TODO(native): if (ctr_CVar13 == quest:GetStateInt("LastBookRequested")) or (*(ctr_CVar13 + *(self_0x14 + 0xa0)) ~= 0) then
                if false then
                    if scratchValue6 ~= 0xffffffff then ctr_CVar = ctr_CVar + 1; goto continue_1 end
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue6 = 0xfffffffe
                    value = 0xfffffffe
                else
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue6 = ctr_CVar
                    value = scratchValue6
                    -- TODO(native): if *(ctr_CVar13 + *(self_0x14 + 0xac)) == 0 then
                end
                ctr_CVar = ctr_CVar + 1
            end
            ::continue_1::
        until ctr_CVar >= booksInGame
        if value ~= 0xfffffffe then goto LAB_00e57678 end
    end
    goto FLOW_past_lab_00e57678
    ::LAB_00e57678::
    if value < booksComment then
        if value ~= 0xffffffff then
            if booksAccepted <= value then
                if quest:IsActiveThreadTerminating() then return end
                resources:MemberResource("seh_me", me)
                -- TODO(native): pCVar9 = (**(self_0x34 + 0x30))()
--[[unresolved native value]]
                local fret_00 = quest:GetHealth(nil --[[missing]])
                if fret_00 <= 0.0 then
                    return
                end
                -- TODO(native): x_stk_14 = *pCVar12
                x_stk_14_1 = nil --[[unresolved native value]]
                scratchValue = "TEXT_QST_B16_BOOK_COMMENT_" .. tostring(value - booksAccepted)
                -- TODO(native): (**(code **)((int)x_stk_14 + 0x34))(pCVar8,pvVar11,uVar15,uVar16,uVar17);
                -- TODO(native): cVar6 = (**(*pCVar12 + 0x68))(pCVar8)
    --[[unresolved native value]]
                if nil == 0 then return end
                repeat
                    if not quest:NewScriptFrame(me) then return end
                    -- TODO(native): cVar6 = (**(*pCVar12 + 0x68))()
    --[[unresolved native value]]
                until not nil
                do return end
                return
            end
            if quest:IsActiveThreadTerminating() then return end
            local x_stk_14_2 = "TEXT_QST_B16_BOOK_REQUEST_" .. tostring(value)
            resources:MemberResource("seh_me", me)
            -- TODO(native): pCVar9 = (**(self_0x34 + 0x30))()
--[[unresolved native value]]
            local fret_01 = quest:GetHealth(nil --[[missing]])
            if 0.0 >= fret_01 then
                AskForBook(quest, me, value, "TEXT_QST_B16_BOOK_REFUSED")
            else
                if not me:Speak(hero, x_stk_14_2, 0, false, true, 0.0 ~= 0) then return end
                if quest:IsActiveThreadTerminating() then return end
                AskForBook(quest, me, scratchValue7, "TEXT_QST_B16_BOOK_REFUSED")
            end
            return
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:MemberResource("seh_me", me)
        -- TODO(native): pCVar9 = (**(self_0x34 + 0x30))()
--[[unresolved native value]]
        local fret_0 = quest:GetHealth(nil --[[missing]])
        if fret_0 <= 0.0 then
            return
        end
        me:Speak(hero, "TEXT_QST_B16_BOOK_NONE_FOUND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, 0.0 ~= 0)
        if not me:IsPerformingScriptTask() then return end
        repeat
            if not quest:NewScriptFrame(me) then return end
        until not me:IsPerformingScriptTask()
        do return end
        return
    end
    ::FLOW_past_lab_00e57678::
    if quest:IsActiveThreadTerminating() then return end
    resources:MemberResource("seh_me", me)
    -- TODO(native): pCVar9 = (**(self_0x34 + 0x30))()
--[[unresolved native value]]
    local fret_02 = quest:GetHealth(nil --[[missing]])
    if fret_02 <= 0.0 then
        return
    end
    if not me:Speak(hero, "TEXT_QST_B16_BOOK_NOT_FOUND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, 0.0 ~= 0) then return end
end

