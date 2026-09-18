-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isRegionLoaded, sequence, pScriptObject, string, resource, movie, actorMap
    resource = resources:NewResource()
    resources:TryAcquire(pScriptObject, hero, 4)
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    isRegionLoaded = quest:IsRegionLoaded("GreatwoodLake")
    sequence = not isRegionLoaded
    if not sequence then
        isRegionLoaded = true
        sequence = heroTeam ~= 0
    end
    if sequence then
        isRegionLoaded = false
    end
    if isRegionLoaded then
        if not quest:IsActiveThreadTerminating() then
            resources:RunMacro("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", actorMap, false, true)
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
            if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), hero, 20.0) then
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00dd1d98 end
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
                ::LAB_00dd1d98::
                resources:RunMacro(string, actorMap, false, true)
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
                resources:RunMacro(string, actorMap, false, true)
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
    resources:DestroyMovie(movie)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
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
