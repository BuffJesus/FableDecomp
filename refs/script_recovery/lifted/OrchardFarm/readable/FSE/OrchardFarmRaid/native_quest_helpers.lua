-- Generated from the same native helper bodies as the quest draft.

local TUTORIAL_CATEGORY_COMBAT_MULTIPLIER = 9  -- ETutorialCategory (Ego_r.pdb)
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local string
    local resource = resources:NewResource()
    resources:TryAcquire(resource, hero, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    if quest:IsRegionLoaded("GreatwoodLake") and heroTeam == 0 then
        if not quest:IsActiveThreadTerminating() then string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"; goto LAB_00dd1d98 end
        goto LAB_00dd1d15
    else
        if not quest:IsActiveThreadTerminating() then
            if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), hero, 20.0) then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1d15 end
                if heroTeam ~= 1 then
                    if not quest:IsActiveThreadTerminating() then string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"; goto LAB_00dd1d98 end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00dd1e95
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                goto LAB_00dd1d98
            elseif not quest:IsActiveThreadTerminating() then
                if heroTeam == 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                end
                goto LAB_00dd1d98
            end
        end
        goto FLOW_hoist_lab_00dd1d98_2
    end
    goto FLOW_past_lab_00dd1d15
    ::LAB_00dd1d15::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00dd1d15::
    goto FLOW_past_lab_00dd1d98
    ::LAB_00dd1d98::
    resources:RunMacro(string, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
    if not quest:DisplayTutorial(TUTORIAL_CATEGORY_COMBAT_MULTIPLIER) then quest:SetStateBool("ShownCombatMultiplierTutorial", true); goto FLOW_hoist_lab_00dd1d98_2 end
    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
    while not quest:MsgIsTutorialClickedPast() do
        if not quest:NewScriptFrame(me) then goto LAB_00dd1e70 end
    end
    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
    ::FLOW_hoist_lab_00dd1d98_2::
    ::LAB_00dd1e70::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00dd1d98::
    resources:DestroyMovie(movie)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

function MakeTeamMemberComment(quest, me, commentToMake, speaker, commentType)
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    local timeRemaining = quest:GetTimer(commentTimer)
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
