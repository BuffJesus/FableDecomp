-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local scratchValue, sequence12, pScriptObject, string, scratchValue8, scratchValue9
    local scratchValue10
    local heroTeam = quest:GetStateInt("HeroTeam")
    scratchValue8 = resources:NewResource()
    resources:TryAcquire(pScriptObject, quest:GetHero(), 4)
    scratchValue10 = resources:NewActorMap()
    resources:SetActor(scratchValue10, "HERO", scratchValue8)
    scratchValue9 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    scratchValue = quest:IsRegionLoaded("GreatwoodLake")
    sequence12 = not scratchValue
    if not sequence12 then
        scratchValue = true
        sequence12 = heroTeam ~= 0
    end
    if sequence12 then
        scratchValue = false
    end
    if scratchValue then
        if not quest:IsActiveThreadTerminating() then
            resources:RunMacro("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", scratchValue10, false, true)
            quest:FixMovieSequenceCamera(false)
            if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
            if quest:DisplayTutorial(9) then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame(me) then goto LAB_00dd1e70_c1 end
                end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            ::LAB_00dd1e70_c1::
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00dd1d98
        end
        quest:PauseAllNonScriptedEntities(false)
    else
        if not quest:IsActiveThreadTerminating() then
            if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), quest:GetHero(), 20.0) then
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00dd1d98 end
                if heroTeam ~= 1 then
                    if not quest:IsActiveThreadTerminating() then string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"; goto LAB_00dd1d98 end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue9)
                    goto LAB_00dd1e95
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue9)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)
                if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                if quest:DisplayTutorial(9) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    while not quest:MsgIsTutorialClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00dd1e70 end
                    end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            elseif not quest:IsActiveThreadTerminating() then
                if heroTeam == 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                end
                resources:RunMacro(string, scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)
                if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                if quest:DisplayTutorial(9) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    while not quest:MsgIsTutorialClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00dd1e70 end
                    end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d98::
    resources:DestroyMovie(scratchValue9)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue10)
    resources:ReleaseResource(scratchValue8)
end

function MakeTeamMemberComment(quest, me, commentToMake, speaker, commentType)
    local commentTimer = quest:GetStateInt("CommentTimer")
    local timeRemaining = quest:GetTimer(commentTimer)
    local pSpeaker = speaker
    if 0 < timeRemaining then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    local hero = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero)
    hero = quest:GetHero()
    local scratchValue = pSpeaker:GetDataString()
    scratchValue = quest:GetStateString("TextSystemScriptCode") .. scratchValue
    scratchValue = scratchValue .. "_"
    scratchValue = scratchValue .. commentToMake
    quest:AddLineToConversation(conversationID, scratchValue, pSpeaker, hero, false)
    local scratchValue2 = quest:SetTimer(commentTimer, 5)
    return true
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
