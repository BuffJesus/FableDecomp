-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local scratchValue, scratchValue2, predicateResult, predicateResult2, scratchValue3
    local scratchValue4, predicateResult3, isDistanceBetweenThingsUnder, predicateResult4
    local predicateResult5, predicateResult6, predicateResult7, scratchValue5, scratchValue6
    local predicateResult8, predicateResult9, scratchValue7, scratchValue8, dist, ePriority
    local scratchValue9, hero, sequence12, sequence21, sequence23, sequence24, hero2, gwl
    local pScriptObject, string, scratchValue10
    local alive = true
    ePriority = 4
    scratchValue9 = resources:NewResource()
    hero2 = quest:GetHero()
    resources:TryAcquire(pScriptObject, hero2, ePriority)
    scratchValue10 = resources:NewActorMap()
    resources:SetActor(scratchValue10, "HERO", scratchValue9)
    scratchValue = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    scratchValue2 = quest:IsRegionLoaded("GreatwoodLake")

    if not scratchValue2 then
        sequence12 = true
    else
        sequence12 = false
    end
    if not sequence12 then
        scratchValue2 = true
        if quest:GetStateInt("HeroTeam") ~= 0 then
            sequence12 = true
        else
            sequence12 = false
        end
    end
    if sequence12 then
        scratchValue2 = false
    end
    if scratchValue2 then
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        if not predicateResult then
            string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"
            -- LAB_00dd1d98_c1: (native jump target)
            resources:RunMacro(string, scratchValue10, false, true)
            quest:FixMovieSequenceCamera(false)
            sequence21 = false
            if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                sequence21 = true
            end
            if not sequence21 then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult2 = not alive
                if predicateResult2 then
                    sequence21 = true
                else
                    sequence21 = false
                end
            end
            if sequence21 then goto LAB_00dd1e70_c1 end
            scratchValue3 = quest:DisplayTutorial(9)
            if scratchValue3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                scratchValue4 = quest:MsgIsTutorialClickedPast()
                while not scratchValue4 do
                    alive = quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                    scratchValue4 = quest:MsgIsTutorialClickedPast()
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            ::LAB_00dd1e70_c1::
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        predicateResult3 = not alive
        if not predicateResult3 then
            dist = 20.0
            hero = quest:GetHero()
            gwl = quest:GetThingWithScriptName("MK_OFI_GWL")
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(gwl, hero, dist)
            if isDistanceBetweenThingsUnder then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult4 = not alive
                if predicateResult4 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d98
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult5 = not alive
                    if not predicateResult5 then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                        goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                predicateResult6 = not alive
                if predicateResult6 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)

                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    sequence23 = true
                else
                    sequence23 = false
                end
                if not sequence23 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult7 = not alive
                    if predicateResult7 then
                        sequence23 = true
                    else
                        sequence23 = false
                    end
                end
                if sequence23 then goto LAB_00dd1e70 end
                scratchValue5 = quest:DisplayTutorial(9)
                if scratchValue5 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    scratchValue6 = quest:MsgIsTutorialClickedPast()
                    while not scratchValue6 do
                        alive = quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        scratchValue6 = quest:MsgIsTutorialClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                predicateResult8 = not alive
                if not predicateResult8 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                    end
                    resources:RunMacro(string, scratchValue10, false, true)
                    quest:FixMovieSequenceCamera(false)
                    sequence24 = false
                    if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                        sequence24 = true
                    end
                    if not sequence24 then
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult9 = not alive
                        if predicateResult9 then
                            sequence24 = true
                        else
                            sequence24 = false
                        end
                    end
                    if sequence24 then goto LAB_00dd1e70 end
                    scratchValue7 = quest:DisplayTutorial(9)
                    if scratchValue7 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        scratchValue8 = quest:MsgIsTutorialClickedPast()
                        while not scratchValue8 do
                            alive = quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                            scratchValue8 = quest:MsgIsTutorialClickedPast()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    end
                    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
                    goto FLOW_after_lab_00dd1d98_127
                end
            end
            ::FLOW_after_lab_00dd1d98_127::
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d98::
    resources:DestroyMovie(xStack_3c)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue10)
    resources:ReleaseResource(xStack_2c)
    return
end

function MakeTeamMemberComment(quest, me, commentToMake, speaker, commentType)
    local timeRemaining = quest:GetTimer(quest:GetStateInt("CommentTimer"))
    local pSpeaker = speaker
    if 0 < timeRemaining then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    local hero = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero)
    hero = quest:GetHero()
    local scratchValue = pSpeaker:GetDataString()
    scratchValue = (quest:GetStateString("TextSystemScriptCode") .. scratchValue)
    scratchValue = (scratchValue .. "_")
    scratchValue = (scratchValue .. commentToMake)
    quest:AddLineToConversation(conversationID, scratchValue, pSpeaker, hero, false)
    return true
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
