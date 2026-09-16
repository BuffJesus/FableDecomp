-- Generated from the same native helper bodies as the quest draft.
local DoMultiplierCutscene, MakeTeamMemberComment
function DoMultiplierCutscene(quest, me)
    local bVar4, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, piVar2, piVar3
    local alive = true
    -- TODO(native): ePriority = 4;
    -- TODO(native): pScriptObject = local_10;
    pCVar5 = quest:GetHero()
    me:AcquireControl(4)
    -- TODO(native): puStack_38 = malloc(0x24);
    -- TODO(native): *puStack_38 = 0;
    -- TODO(native): *(undefined4 *)(puStack_38 + 4) = 0;
    -- TODO(native): *(undefined1 **)(puStack_38 + 8) = puStack_38;
    -- TODO(native): *(undefined1 **)(puStack_38 + 0xc) = puStack_38;
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,&DAT_01255174,-1);
    -- TODO(native): this_00 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&puStack_38,&CStack_40);
    -- TODO(native): CFourierAnalysis::CFourierAnalysis(this_00);
    piVar3 = 0x0
    piVar2 = *(this_00 + 0xc)
    if piVar2 ~= nil then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(this_00 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(this_00 + 0xc))[1])();
            end
        end
        -- TODO(native): *(CScriptThing **)(this_00 + 8) = local_3c;
        -- TODO(native): *(int **)(this_00 + 0xc) = piVar3;
        if piVar3 ~= nil then
            -- TODO(native): *piVar3 = *piVar3 + 1;
        end
    end
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,&DAT_0122d70e,-1);
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(false)
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
            -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL";
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): dist = 20.0;
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, dist)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dd1d15
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL";
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00dd1e95
                end
                -- TODO(native): string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL";
                -- LAB_00dd1d98: (native jump target)
                -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,string,-1);
                -- TODO(native): RunCutsceneMacro_Func(&CStack_40,&puStack_38,(void *)0x0,(void *)0x0,false,true);
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
                        -- TODO(native): string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP";
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                        -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP";
                    end
                    -- TODO(native): goto LAB_00dd1d98
                end
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (&uStack_18);
    ::LAB_00dd1e95::
    -- TODO(native): StdMap_Destroy_API(&puStack_38);
end

function MakeTeamMemberComment(quest, me, native_arg_param_1, native_arg_param_2, native_arg_param_3)
    local alive = true
    local uVar3 = quest:GetStateInt("CommentTimer")
    local iVar1 = quest:GetTimer(nil --[[missing]])
    if 0 < iVar1 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    local uVar2 = quest:AddNewConversation(nil --[[missing]], false, false)
    local uStack_30 = quest:GetHero()
    quest:AddPersonToConversation(nil --[[missing]], uStack_30)
    local uStack_38 = quest:GetHero()
    -- TODO(native): uVar3 = (**(code **)(*(int *)native_arg_param_1 + 0xc))(puVar4,&DAT_01244db4,uVar3,0);
    -- TODO(native): CCharString__AppendData(uVar3);
    -- TODO(native): CCharString__AppendCString(puVar4);
    uVar3 = CCharString__AppendData(puVar5)
    quest:AddLineToConversation(uVar2, uVar3, uStack_38, nil --[[missing]])
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

return {DoMultiplierCutscene = DoMultiplierCutscene, MakeTeamMemberComment = MakeTeamMemberComment}
