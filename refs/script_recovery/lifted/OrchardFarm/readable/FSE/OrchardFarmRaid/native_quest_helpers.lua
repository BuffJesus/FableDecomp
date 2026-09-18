-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    local string
    quest:StartCutscene({HERO = hero}, {}, true)
    if quest:IsRegionLoaded("GreatwoodLake") and heroTeam == 0 then
        if not quest:IsActiveThreadTerminating() then
            quest:RunCutscene("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", true, false)
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
            quest:EndCutscene()
            goto FLOW_after_lab_00dd1d98
        end
        quest:EndCutscene()
    else
        if not quest:IsActiveThreadTerminating() then
            if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), hero, 20.0) then
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00dd1d98 end
                if heroTeam ~= 1 then
                    if not quest:IsActiveThreadTerminating() then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                    else
                        quest:EndCutscene()
                        goto LAB_00dd1e95
                    end
                else
                    string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                end
                quest:RunCutscene(string, true, false)
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
                quest:RunCutscene(string, true, false)
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
        quest:EndCutscene()
    end
    ::FLOW_after_lab_00dd1d98::
    ::LAB_00dd1e95::
    quest:EndCutscene()
end

function MakeTeamMemberComment(quest, me, commentToMake, speaker, commentType)
    local timeRemaining
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    timeRemaining = quest:GetTimer(commentTimer)
    local pSpeaker = speaker
    if 0 < timeRemaining then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    quest:AddPersonToConversation(conversationID, hero)
    local getDataString = pSpeaker:GetDataString()
    getDataString = quest:GetStateString("TextSystemScriptCode") .. getDataString
    getDataString = getDataString .. "_"
    getDataString = getDataString .. commentToMake
    quest:AddLineToConversation(conversationID, getDataString, pSpeaker, hero, false)
    local scratchValue = quest:SetTimer(commentTimer, 5)
    return true
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
