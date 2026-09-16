-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local scratchValue, scratchValue2, predicateResult, predicateResult2
    local isDistanceBetweenThingsUnder, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6, scratchValue3, predicateResult7, scratchValue4, predicateResult8
    local predicateResult9, predicateResult10, predicateResult11, predicateResult12, scratchValue5
    local hero, sequence12, sequence22, hero2, gwl, pScriptObject, scratchValue6
    local alive = true
    scratchValue5 = resources:NewResource()
    hero2 = quest:GetHero()
    resources:TryAcquire(pScriptObject, hero2, 4)
    scratchValue6 = resources:NewActorMap()
    resources:SetActor(scratchValue6, "HERO", scratchValue5)
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
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        predicateResult2 = not alive
        if not predicateResult2 then
            -- TODO(native): local_3c = (CScriptThing *)piVar3;
            hero = quest:GetHero()
            gwl = quest:GetThingWithScriptName("MK_OFI_GWL")
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(gwl, hero, 20.0)
            if isDistanceBetweenThingsUnder then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult3 = not alive
                if predicateResult3 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d15
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult4 = not alive
                    if not predicateResult4 then
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                predicateResult5 = not alive
                if predicateResult5 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                -- LAB_00dd1d98: (native jump target)
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", scratchValue6, false, true)
                quest:FixMovieSequenceCamera(false)

                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    sequence22 = true
                else
                    sequence22 = false
                end
                if not sequence22 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult6 = not alive
                    if predicateResult6 then
                        sequence22 = true
                    else
                        sequence22 = false
                    end
                end
                if sequence22 then goto LAB_00dd1e70 end
                scratchValue3 = quest:DisplayTutorial(9)
                if scratchValue3 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult7 = not alive
                    if predicateResult7 then return end  -- TODO(native): goto LAB_00dd1e53
                    scratchValue4 = quest:MsgIsTutorialClickedPast()
                    while not scratchValue4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult8 = not alive
                        if predicateResult8 then return end  -- TODO(native): goto LAB_00dd1e53
                        scratchValue4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult9 = not alive
                    if predicateResult9 then return end  -- TODO(native): goto LAB_00dd1e3d
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                predicateResult10 = not alive
                if not predicateResult10 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult11 = not alive
                        if predicateResult11 then return end  -- TODO(native): goto LAB_00dd1e3d
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult12 = not alive
                        if predicateResult12 then return end  -- TODO(native): goto LAB_00dd1e53
                    end
                    -- TODO(native): goto LAB_00dd1d98
                end
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d15::
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (&xStack_34);
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue6)
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
