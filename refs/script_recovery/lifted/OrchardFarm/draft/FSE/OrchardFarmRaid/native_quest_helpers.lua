-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local appuStack_20, bVar4, local_10, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, pScriptObject, puStack_38
    local alive = true
    local_10 = resources:NewResource()
    pCVar5 = quest:GetHero()
    resources:TryAcquire(pScriptObject, pCVar5, 4)
    puStack_38 = resources:NewActorMap()
    resources:SetActor(puStack_38, "HERO", 0x0)
    appuStack_20 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    bVar4 = quest:IsRegionLoaded("GreatwoodLake")
    native_arg_sequence_1 = false
    if not bVar4 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if not native_arg_sequence_1 then
        bVar4 = true
        if quest:GetStateInt("HeroTeam") ~= 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        bVar4 = false
    end
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): local_3c = (CScriptThing *)piVar3;
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, 20.0)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dd1d15
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(appuStack_20)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(appuStack_20)
                    goto LAB_00dd1e95
                end
                -- LAB_00dd1d98: (native jump target)
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", pCVar5, false, true)
                quest:FixMovieSequenceCamera(false)
                native_arg_sequence_2 = false
                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then goto LAB_00dd1e70 end
                bVar4 = quest:DisplayTutorial(9)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                    bVar4 = quest:MsgIsTutorialClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                        bVar4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00dd1e3d
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e3d
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                    end
                    -- TODO(native): goto LAB_00dd1d98
                end
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_20);
    ::LAB_00dd1e95::
    resources:DestroyActorMap(pCVar5)
    resources:ReleaseResource(0x0)
    return extraout_EAX
end

function MakeTeamMemberComment(quest, me, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local pCVar3
    local alive = true
    -- TODO(native): uStack_14 = *(undefined4 *)(this + 0x4c);
    local iVar2 = quest:GetTimer(nil --[[missing]])
    if 0 < iVar2 then
        alive = not quest:IsActiveThreadTerminating()
        return extraout_var << 8
    end
    local conversationID = quest:AddNewConversation(nil --[[missing]], (native_arg_comment_to_make ~= 0), false)
    iVar2 = *piVar1
    pCVar3 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, pCVar3)
    iVar2 = *piVar1
    local r1 = quest:GetHero()
    pCVar3 = 0x0
    local p2 = (**(*native_arg_comment_to_make + 0xc))()
    local pvVar4 = CCharString__AppendData(&stack0xfffffff0,this + 0x84,p2)
    pvVar4 = CCharString__AppendCString(&uStack_14,pvVar4,p2_00)
    local pLine = CCharString__AppendData(&stack0x00000000,pvVar4,"_")
    quest:AddLineToConversation(conversationID, pLine, r1, nil --[[missing]], unaff_retaddr)
    local uVar5 = quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return CONCAT31((int3)(uVar5 >> 8),1)
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
