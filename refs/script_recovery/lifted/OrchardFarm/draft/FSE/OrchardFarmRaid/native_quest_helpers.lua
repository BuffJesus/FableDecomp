-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local resources = quest:RetailResources()
    local bVar4, dist, ePriority, local_3c, native_arg_sequence_1, pCVar5, pScriptObject, string, xStack_10, xStack_20, xStack_38
    local alive = true
    ePriority = 4
    xStack_10 = resources:NewResource()
    pScriptObject = xStack_10
    pCVar5 = quest:GetHero()
    resources:TryAcquire(pScriptObject, pCVar5, ePriority)
    xStack_38 = resources:NewActorMap()
    resources:SetActor(xStack_38, "HERO", xStack_10)
    xStack_20 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    bVar4 = quest:IsRegionLoaded("GreatwoodLake")
    if (not bVar4) or (quest:GetStateInt("HeroTeam") ~= 0) then
        bVar4 = false
    end
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"
            -- LAB_00dd1d98_c1: (native jump target)
            resources:RunMacro(string, xStack_38, false, true)
            quest:FixMovieSequenceCamera(false)
            native_arg_sequence_1 = false
            if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                native_arg_sequence_1 = true
            end
            if not native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00dd1e70_c1 end
            bVar4 = quest:DisplayTutorial(9)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00dd1e70_c1 end
                bVar4 = quest:MsgIsTutorialClickedPast()
                while not bVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00dd1e70_c1 end
                    bVar4 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00dd1e70_c1 end
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
        bVar4 = not alive
        if not bVar4 then
            dist = 20.0
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, dist)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d98
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                        goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, xStack_38, false, true)
                quest:FixMovieSequenceCamera(false)
                native_arg_sequence_1 = false
                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00dd1e70 end
                bVar4 = quest:DisplayTutorial(9)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00dd1e70 end
                    bVar4 = quest:MsgIsTutorialClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        bVar4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00dd1e70 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                    end
                    resources:RunMacro(string, xStack_38, false, true)
                    quest:FixMovieSequenceCamera(false)
                    native_arg_sequence_1 = false
                    if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                        native_arg_sequence_1 = true
                    end
                    if not native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00dd1e70 end
                    bVar4 = quest:DisplayTutorial(9)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        bVar4 = quest:MsgIsTutorialClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00dd1e70 end
                            bVar4 = quest:MsgIsTutorialClickedPast()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                    end
                    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
                    goto FLOW_after_lab_00dd1d98_109
                end
            end
            ::FLOW_after_lab_00dd1d98_109::
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d98::
    resources:DestroyMovie(xStack_20)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(xStack_38)
    resources:ReleaseResource(xStack_10)
end

function MakeTeamMemberComment(quest, me, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local alive = true
    local iVar1 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
    local pSpeaker = native_arg_speaker
    if 0 < iVar1 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    local conversationID = quest:AddNewConversation(native_arg_speaker, false, false)
    local pCVar2 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, pCVar2)
    pCVar2 = quest:GetHero()
    local pCVar3 = pSpeaker:GetDataString()
    pCVar3 = (quest:GetStateString("TextSystemScriptCode") .. pCVar3)
    pCVar3 = (pCVar3 .. "_")
    pCVar3 = (pCVar3 .. native_arg_comment_to_make)
    quest:AddLineToConversation(conversationID, pCVar3, pSpeaker, pCVar2, false)
    local uVar4 = quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
