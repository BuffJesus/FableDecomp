-- Readable native conversion: ArtifactThief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_LampCost = 3860,  -- 50.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local holdingArtifact, alreadyTalkedTo, notAttacked

-- ArtifactThief.Main (retail 0x00d62480)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult5, predicateResult, predicateResult18, predicateResult25, predicateResult35
    local getStateBool, questionAnswer, addNewConversation, movie, scratchValue, scratchValue24
    local scratchValue25, u_stk_174_1, u_stk_174_2, movie2, movie3, movie4, timerId, movie5, movie6
    local function __region_LAB_00d632c3_c2()
        quest:PauseAllNonScriptedEntities(false)
        movie = movie3
    end
    local function __cleanup_LAB_00d63aa6()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(timerId)
        me:ReleaseControl()
    end
    local function __cleanup_LAB_00d63aab()
        quest:DeregisterTimer(timerId)
        me:ReleaseControl()
    end
    scratchValue25 = 0
    u_stk_174_1 = 0
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(4) then goto LAB_00d63c9f end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        holdingArtifact = true
        alreadyTalkedTo = false
        notAttacked = true
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        getStateBool = holdingArtifact
        repeat
            if not getStateBool or not notAttacked then goto LAB_00d638ec end
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                if not alreadyTalkedTo then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, hero, false)
                end
                quest:SetTimer(timerId, 10)
                scratchValue25 = u_stk_174_1
            end
            scratchValue = scratchValue25 | 1
            u_stk_174_1 = scratchValue
            if me:MsgIsHitByHero() then
                predicateResult5 = true
            else
                scratchValue = scratchValue25 | 3
                u_stk_174_1 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = scratchValue25 | 7
                    u_stk_174_1 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        predicateResult5 = true
                        goto FLOW_after_lab_00d6280c
                    end
                end
                predicateResult5 = false
            end
            ::FLOW_after_lab_00d6280c::
            if scratchValue & 4 ~= 0 then
                scratchValue = scratchValue & 0xfffffffb
                u_stk_174_1 = scratchValue
            end
            if scratchValue & 2 ~= 0 then
                scratchValue = scratchValue & 0xfffffffd
                u_stk_174_1 = scratchValue
            end
            if scratchValue & 1 ~= 0 then
                scratchValue = scratchValue & 0xfffffffe
                u_stk_174_1 = scratchValue
            end
            scratchValue25 = scratchValue
            if predicateResult5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                notAttacked = false
                if not alreadyTalkedTo then
                    movie2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                __cleanup_LAB_00d63aab(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_138;
                else
                    movie6 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                __cleanup_LAB_00d63aab(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_bc;
                end
                quest:EntitySetAsKillable(me, true, true)
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, 1, false, true)
                scratchValue25 = u_stk_174_1
            end
            if not me:IsTalkedToByHero() then getStateBool = holdingArtifact; goto continue_3 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
            if not holdingArtifact then goto LAB_00d638ec end
            if not alreadyTalkedTo then
                alreadyTalkedTo = true
                movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(me) then
                    if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                    if not quest:IsActiveThreadTerminating() then
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(timerId)
                                me:ReleaseControl()
                                do return end
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            predicateResult = quest:IsActiveThreadTerminating()
                            if questionAnswer == 1 then
                                if not predicateResult then
                                    if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost) then
                                        if not quest:IsActiveThreadTerminating() then
                                            if 0.0 < quest:GetHealth(me) then
                                                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                                -- TODO(native): goto LAB_00d632b4_c2
                                            end
                                            __region_LAB_00d632c3_c2(); goto LAB_00d638d8
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        if 0.0 < quest:GetHealth(me) then
                                            if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                        end
                                        quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                        quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                        quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                        quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                        holdingArtifact = false
                                        me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, 0, false, true)
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie3
                                        goto LAB_00d638d8
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d63c96
                            end
                            if not predicateResult then
                                if 0.0 < quest:GetHealth(me) then
                                    if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                    -- LAB_00d632b4_c2: (native jump target)
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                end
                                __region_LAB_00d632c3_c2()
                                goto LAB_00d638d8
                            end
                        end
                        goto FLOW_after_lab_00d62e38
                    end
                else
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            __cleanup_LAB_00d63aa6()
                            do return end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        predicateResult18 = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if not predicateResult18 then
                                if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost) then
                                    if not quest:IsActiveThreadTerminating() then
                                        if 0.0 < quest:GetHealth(me) then
                                            if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                            quest:PauseAllNonScriptedEntities(false)
                                            movie = movie3
                                            goto LAB_00d638d8
                                        end
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie3
                                        goto LAB_00d638d8
                                    end
                                    goto LAB_00d63a7c
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    if 0.0 < quest:GetHealth(me) then
                                        if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                    end
                                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                    quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                    quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                    holdingArtifact = false
                                    me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, 0, false, true)
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie3
                                    goto LAB_00d638d8
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00d63c96
                        end
                        if not predicateResult18 then
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                -- LAB_00d632b4: (native jump target)
                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            goto LAB_00d638d8
                        end
                    end
                end
                ::FLOW_after_lab_00d62e38::
                ::LAB_00d63a7c::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00d63c96
            end
            movie4 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(me) then
                me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
            end
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    __cleanup_LAB_00d63aa6(); do return end
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00d63c96
            end
            predicateResult25 = quest:IsActiveThreadTerminating()
            if questionAnswer ~= 1 then
                if predicateResult25 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
                movie5 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(me) then
                    me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d63c96
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                goto LAB_00d638c8
            end
            if predicateResult25 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00d63c96
            end
            -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
            if false then
                if not quest:IsActiveThreadTerminating() then
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d63c96
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                    quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                    holdingArtifact = false
                    me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, 0, false, true)
                    goto LAB_00d638c8
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00d63c96
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00d63c96
            end
            if 0.0 < quest:GetHealth(me) then
                me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
            end
            ::LAB_00d638c8::
            quest:PauseAllNonScriptedEntities(false)
            movie = movie4
            ::LAB_00d638d8::
            resources:DestroyMovie(movie)
            scratchValue25 = u_stk_174_1
            getStateBool = holdingArtifact
            ::continue_3::
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
        scratchValue24 = scratchValue25 | 8
        u_stk_174_2 = scratchValue24
        if me:MsgIsHitByHero() then
            predicateResult35 = true
        else
            scratchValue24 = scratchValue25 | 24
            u_stk_174_2 = scratchValue24
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue24 = scratchValue25 | 56
                u_stk_174_2 = scratchValue24
                if not me:MsgIsHitByHeroSpecialAbility(me) then
                    predicateResult35 = true
                    goto FLOW_after_lab_00d63b21
                end
            end
            predicateResult35 = false
        end
        ::FLOW_after_lab_00d63b21::
        if scratchValue24 & 32 ~= 0 then
            scratchValue24 = scratchValue24 & 0xffffffdf
            u_stk_174_2 = scratchValue24
        end
        if scratchValue24 & 16 ~= 0 then
            scratchValue24 = scratchValue24 & 0xffffffef
            u_stk_174_2 = scratchValue24
        end
        if scratchValue24 & 8 ~= 0 then
            scratchValue24 = scratchValue24 & 0xfffffff7
            u_stk_174_2 = scratchValue24
        end
        scratchValue25 = scratchValue24
        if predicateResult35 then
            if notAttacked then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, hero, false)
                notAttacked = false
                quest:EntitySetAsKillable(me, true, true)
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, 1, false, true)
                scratchValue25 = u_stk_174_2
        end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end
    ::LAB_00d63c96::
    quest:DeregisterTimer(timerId)
    ::LAB_00d63c9f::
    me:ReleaseControl()
end

-- ArtifactThief.Init (retail 0x00d62410)
function Init(quest, me)
    quest:AddItemToContainer(me, "OBJECT_HAND_LAMP")
end

-- ArtifactThief.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ArtifactThief.OnPredicateFail (retail 0x00d62450)
function OnPredicateFail(quest, me)
end

