-- Generated from the same native helper bodies as the quest draft.
local helper_E91F20
function helper_E91F20(quest, me)
    local CVar2, bVar3, bVar6, iVar1, iVar4, native_arg_sequence_1, native_arg_switch_3, native_arg_switch_4, native_arg_switch_5, native_arg_switch_6, native_arg_switch_7, pcVar7, uVar5
    local alive = true
    -- TODO(native): CCharString::CCharString(&xStack_c,other);
    local function __region_LAB_00e92061()
        do return "" end
    end
    iVar1 = quest:GetMasterGameState("PostSavePosition")
    if iVar1 < 0x385 then
        if iVar1 == 900 then
            pcVar7 = "TEXT_QST_081_ARENA"
        else
            if iVar1 < 0x2bd then
                if iVar1 == 700 then
                    goto LAB_00e92080
                else
                    if iVar1 < 0x191 then
                        if iVar1 == 400 then
                            pcVar7 = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
                        else
                            native_arg_switch_3 = iVar1
                            repeat
                                if native_arg_switch_3 == 100 then
                                    pcVar7 = "TEXT_QST_081_TRAINING"
                                    break
                                else
                                    if native_arg_switch_3 == 0x96 then
                                        pcVar7 = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
                                        break
                                    else
                                        if native_arg_switch_3 == 200 then
                                            pcVar7 = "TEXT_QST_081_WASP_BOSS"
                                            break
                                        else
                                            if native_arg_switch_3 == 300 then
                                                pcVar7 = "TEXT_QST_081_MAZE_MEETING"
                                            else
                                                -- FLOW_native_label_2_c1: (native jump target)
                                                -- LAB_00e922bb_c1: (native jump target)
                                                -- LAB_00e922c9_c1: (native jump target)
                                                do return "" end
                                                goto FLOW_after_flow_native_label_2
                                            end
                                        end
                                    end
                                end
                            until not (false)
                        end
                    else
                        native_arg_switch_4 = iVar1
                        repeat
                            if native_arg_switch_4 == 0x1c2 then
                                pcVar7 = "TEXT_QST_081_ORCHARD_FARM"
                                break
                            else
                                if native_arg_switch_4 == 500 then
                                    pcVar7 = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
                                    break
                                else
                                    if native_arg_switch_4 == 0x226 then
                                        pcVar7 = "TEXT_QST_081_TRADER_ESCORT"
                                        break
                                    else
                                        if native_arg_switch_4 == 600 then
                                            bVar6 = quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar6 then
                                                if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                                goto LAB_00e92080
                                            end
                                            if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                            pcVar7 = "TEXT_QST_081_SECOND_MAZE_MEETING"
                                        else
                                            -- FLOW_native_label_2_c2: (native jump target)
                                            -- LAB_00e922bb_c2: (native jump target)
                                            -- LAB_00e922c9_c2: (native jump target)
                                            do return "" end
                                            goto FLOW_after_flow_native_label_2
                                        end
                                    end
                                end
                            end
                        until not (false)
                    end
                end
                goto FLOW_past_lab_00e92080
                ::LAB_00e92080::
                pcVar7 = "TEXT_QST_081_BANDIT_CAMP"
                ::FLOW_past_lab_00e92080::
            else
                native_arg_switch_5 = iVar1
                repeat
                    if native_arg_switch_5 == 0x2dd then
                        pcVar7 = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
                        break
                    else
                        if native_arg_switch_5 == 0x2fe then
                            pcVar7 = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
                            break
                        else
                            if native_arg_switch_5 == 800 then
                                bVar6 = quest:IsQuestActive("V_TrophyDealer")
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar6 then
                                    if bVar3 then
                                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                        -- LAB_00e922c9_c3: (native jump target)
                                        do return "" end
                                        goto FLOW_after_flow_native_label_2
                                    end
                                    goto FLOW_native_label_1
                                end
                                if bVar3 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                pcVar7 = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                                break
                            else
                                if native_arg_switch_5 == 0x352 then
                                    goto FLOW_native_label_1
                                else
                                    if native_arg_switch_5 == 0x358 then
                                        pcVar7 = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
                                        break
                                    else
                                        if native_arg_switch_5 == 0x361 then
                                            pcVar7 = "TEXT_QST_081_WHITE_BALVERINE_KHG"
                                            break
                                        else
                                            if native_arg_switch_5 == 0x366 then
                                                pcVar7 = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
                                                break
                                            else
                                                if native_arg_switch_5 == 0x36b then
                                                    pcVar7 = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
                                                else
                                                    -- FLOW_native_label_2_c4: (native jump target)
                                                    -- LAB_00e922bb_c4: (native jump target)
                                                    -- LAB_00e922c9_c4: (native jump target)
                                                    do return "" end
                                                    goto FLOW_after_flow_native_label_2
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                            goto FLOW_past_flow_native_label_1
                            ::FLOW_native_label_1::
                            pcVar7 = "TEXT_QST_081_TROPHY_DEALER"
                            break
                            ::FLOW_past_flow_native_label_1::
                        end
                    end
                until not (false)
            end
        end
    else
        if iVar1 < 0x5dd then
            if iVar1 == 0x5dc then
                pcVar7 = "TEXT_QST_081_WIZARD_BATTLE"
            else
                if iVar1 < 0x4c1 then
                    if iVar1 == 0x4c0 then
                        pcVar7 = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
                    else
                        native_arg_switch_6 = iVar1
                        repeat
                            if native_arg_switch_6 == 1000 then
                                pcVar7 = "TEXT_QST_081_MEET_SISTER"
                                break
                            else
                                if native_arg_switch_6 == 0x41a then
                                    pcVar7 = "TEXT_QST_081_WAITING_FOR_MCC"
                                    break
                                else
                                    if native_arg_switch_6 == 0x44c then
                                        pcVar7 = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
                                        break
                                    else
                                        if native_arg_switch_6 == 0x4b0 then
                                            pcVar7 = "TEXT_QST_081_GRAVEYARD"
                                        else
                                            -- LAB_00e922bb_c5: (native jump target)
                                            -- LAB_00e922c9_c5: (native jump target)
                                            do return "" end
                                            goto FLOW_after_flow_native_label_2_242
                                        end
                                    end
                                end
                            end
                        until not (false)
                    end
                else
                    native_arg_switch_7 = iVar1
                    repeat
                        if native_arg_switch_7 == 0x4d1 then
                            pcVar7 = "TEXT_QST_081_FINALGRAVEYARD"
                            break
                        else
                            if native_arg_switch_7 == 0x4e2 then
                                pcVar7 = "TEXT_QST_081_PRISON"
                                break
                            else
                                if native_arg_switch_7 == 0x514 then
                                    if not quest:GetMasterGameState("SeenAbbeyMotherAtGuild") then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                        pcVar7 = "TEXT_QST_081_BEFORE_HOOK_COAST"
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                        pcVar7 = "TEXT_QST_081_HOOK_COAST"
                                    end
                                    break
                                else
                                    if native_arg_switch_7 == 0x5aa then
                                        pcVar7 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                                    else
                                        -- FLOW_native_label_2: (native jump target)
                                        -- LAB_00e922bb: (native jump target)
                                        -- LAB_00e922c9: (native jump target)
                                        return ""
                                    end
                                end
                            end
                        end
                    until not (false)
                end
                ::FLOW_after_flow_native_label_2_242::
            end
        else
            if iVar1 < 0x8fd then
                if iVar1 == 0x8fc then goto LAB_00e923f7 end
                if iVar1 < 0x6a5 then
                    if iVar1 == 0x6a4 then
                        pcVar7 = "TEXT_QST_081_JACK_BOSS_FIGHT"
                    else
                        if iVar1 == 0x60e then
                            pcVar7 = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                        else
                            if iVar1 ~= 0x640 then
                                -- LAB_00e922bb_c6: (native jump target)
                                -- LAB_00e922c9_c6: (native jump target)
                                do return "" end
                                goto FLOW_after_flow_native_label_2_357
                            end
                            pcVar7 = "TEXT_QST_081_FOCAL_SITES"
                        end
                    end
                else
                    if iVar1 ~= 0x834 then
                        -- LAB_00e922bb_c7: (native jump target)
                        -- LAB_00e922c9_c7: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    pcVar7 = "TEXT_QST_081_SHIP_SUMMONING"
                end
            else
                if iVar1 == 0x960 then
                    goto LAB_00e923f7
                else
                    if iVar1 == 0x9c4 then
                        -- TODO(native): iVar4 = *(iVar4 + 0x70)
                        iVar4 = nil --[[unresolved native value]]
                        if iVar4 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                __region_LAB_00e92061()
                                goto FLOW_after_flow_native_label_2_357
                            end
                            pcVar7 = "TEXT_QST_081_HERO_SOULS"
                        else
                            if iVar4 == 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00e92207: (native jump target)
                                    -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                    do return "" end
                                    goto FLOW_after_flow_native_label_2_357
                                end
                                pcVar7 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                            else
                                if iVar4 == 2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- LAB_00e92048: (native jump target)
                                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                        do return "" end
                                        goto FLOW_after_flow_native_label_2_357
                                    end
                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                                else
                                    if iVar4 == 3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                        pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                                    else
                                        if iVar4 == 4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                            pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                                        else
                                            if iVar4 == 5 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                                pcVar7 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                                            else
                                                if iVar4 == 6 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                                                else
                                                    if iVar4 ~= 7 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then
                                                            -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                                            do return "" end
                                                            goto FLOW_after_flow_native_label_2_357
                                                        end
                                                        -- LAB_00e922c9_c12: (native jump target)
                                                        do return "" end
                                                        goto FLOW_after_flow_native_label_2_357
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then __region_LAB_00e92061(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                                    pcVar7 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    else
                        if iVar1 ~= 0xa28 then
                            -- LAB_00e922bb_c13: (native jump target)
                            -- LAB_00e922c9_c13: (native jump target)
                            do return "" end
                            goto FLOW_after_flow_native_label_2_357
                        end
                        pcVar7 = "TEXT_QST_081_DRAGON_FIGHT_10"
                    end
                end
            end
            goto FLOW_past_lab_00e923f7
            ::LAB_00e923f7::
            pcVar7 = "TEXT_QST_081_THE_ORACLE"
            ::FLOW_past_lab_00e923f7::
        end
        ::FLOW_after_flow_native_label_2_357::
    end
    ::FLOW_after_flow_native_label_2::
    -- TODO(native): CVar2 = *quest:GetStateString("LastDialogueSaid")
    CVar2 = nil --[[unresolved native value]]
    native_arg_sequence_1 = false
    if CVar2 == in_stack_fffffff0 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if not native_arg_sequence_1 then
        if CVar2 ~= nil then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            if in_stack_fffffff0 ~= nil then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            -- TODO(native): if *(CVar2 + 4) == *(in_stack_fffffff0 + 4) then
            if false then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            -- TODO(native): iVar4 = CBasicString<char>::Compare(*(void **)CVar2,*(void **)in_stack_fffffff0);
            if iVar4 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
    end
    if native_arg_sequence_1 then
        uVar5 = math.random(0, 32767)
        uVar5 = uVar5 & 0x80000003
        bVar6 = uVar5 == 0
        if uVar5 < 0 then
            bVar6 = (uVar5 - 1 | 0xfffffffc) == 0xffffffff
        end
        if bVar6 then
            if not quest:GetStateBool("GuildMasterDialogue_2") then
                quest:SetStateBool("GuildMasterDialogue_2", true)
                pcVar7 = "TEXT_QST_081_INFO_TELEPORTERS"
            else
                if not quest:GetStateBool("GuildMasterDialogue_3") then
                    quest:SetStateBool("GuildMasterDialogue_3", true)
                    pcVar7 = "TEXT_QST_081_INFO_WEAPONS"
                else
                    if not quest:GetStateBool("GuildMasterDialogue_4") then
                        quest:SetStateBool("GuildMasterDialogue_4", true)
                        pcVar7 = "TEXT_QST_081_INFO_LOG_BOOK"
                    else
                        iVar4 = quest:GetHeroTitle()
                        if (iVar4 ~= 0x13) or (quest:GetStateBool("GuildMasterDialogue_1")) then goto LAB_00e924b3 end
                        quest:SetStateBool("GuildMasterDialogue_1", true)
                        pcVar7 = "TEXT_QST_081_INFO_HERO_TITLE"
                    end
                end
            end
        end
    end
    ::LAB_00e924b3::
    -- TODO(native): quest:SetStateString("LastDialogueSaid", &pcVar7)
    return pcVar7
end

return {helper_E91F20 = helper_E91F20}
