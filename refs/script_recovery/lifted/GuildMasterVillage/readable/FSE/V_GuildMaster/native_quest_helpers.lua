-- Generated from the same native helper bodies as the quest draft.
local GetGuildMasterSpeech
-- E91F20: bsim names this body NScript::CV_GuildMasterScript::GetGuildMasterSpeech (a homologous script member); no PDB name
function GetGuildMasterSpeech(quest, me)
    local predicateResult, scratchValue2, getHeroTitle, sequence1, scratchValue3, scratchValue4
    -- TODO(native): CCharString::CCharString(&xStack_c,other);
    local function ReleaseEverything()
        do return "" end
    end
    local getMasterGameState = quest:GetMasterGameState("PostSavePosition")
    if getMasterGameState < 901 then
        if getMasterGameState == 900 then
            scratchValue3 = "TEXT_QST_081_ARENA"
        elseif getMasterGameState < 701 then
            if getMasterGameState == 700 then
                goto LAB_00e92080
            elseif getMasterGameState < 401 then
                if getMasterGameState == 400 then
                    scratchValue3 = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
                else
                    local switch = getMasterGameState
                    repeat
                        if switch == 100 then
                            scratchValue3 = "TEXT_QST_081_TRAINING"
                            break
                        elseif switch == 150 then
                            scratchValue3 = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
                            break
                        elseif switch == 200 then
                            scratchValue3 = "TEXT_QST_081_WASP_BOSS"
                            break
                        elseif switch == 300 then
                            scratchValue3 = "TEXT_QST_081_MAZE_MEETING"
                        else
                            -- FLOW_native_label_2_c1: (native jump target)
                            do return "" end
                            goto FLOW_after_flow_native_label_2
                        end
                    until true
                end
            else
                local switch4 = getMasterGameState
                repeat
                    if switch4 == 450 then
                        scratchValue3 = "TEXT_QST_081_ORCHARD_FARM"
                        break
                    elseif switch4 == 500 then
                        scratchValue3 = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
                        break
                    elseif switch4 == 550 then
                        scratchValue3 = "TEXT_QST_081_TRADER_ESCORT"
                        break
                    elseif switch4 == 600 then
                        scratchValue2 = quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")
                        predicateResult = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                            goto LAB_00e92080
                        end
                        if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                        scratchValue3 = "TEXT_QST_081_SECOND_MAZE_MEETING"
                    else
                        -- FLOW_native_label_2_c2: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                until true
            end
            goto FLOW_past_lab_00e92080
            ::LAB_00e92080::
            scratchValue3 = "TEXT_QST_081_BANDIT_CAMP"
            ::FLOW_past_lab_00e92080::
        else
            local switch5 = getMasterGameState
            repeat
                if switch5 == 733 then
                    scratchValue3 = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
                    break
                elseif switch5 == 766 then
                    scratchValue3 = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
                    break
                else
                    if switch5 == 800 then
                        scratchValue2 = quest:IsQuestActive("V_TrophyDealer")
                        predicateResult = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            if predicateResult then
                                -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                do return "" end
                                goto FLOW_after_flow_native_label_2
                            end
                            goto FLOW_native_label_1
                        end
                        if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                        scratchValue3 = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                        break
                    elseif switch5 == 850 then
                        goto FLOW_native_label_1
                    elseif switch5 == 856 then
                        scratchValue3 = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
                        break
                    elseif switch5 == 865 then
                        scratchValue3 = "TEXT_QST_081_WHITE_BALVERINE_KHG"
                        break
                    elseif switch5 == 870 then
                        scratchValue3 = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
                        break
                    elseif switch5 == 875 then
                        scratchValue3 = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
                    else
                        -- FLOW_native_label_2_c4: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                    goto FLOW_past_flow_native_label_1
                    ::FLOW_native_label_1::
                    scratchValue3 = "TEXT_QST_081_TROPHY_DEALER"
                    break
                    ::FLOW_past_flow_native_label_1::
                end
            until true
        end
    else
        if getMasterGameState < 1501 then
            if getMasterGameState == 1500 then
                scratchValue3 = "TEXT_QST_081_WIZARD_BATTLE"
            else
                if getMasterGameState < 1217 then
                    if getMasterGameState == 1216 then
                        scratchValue3 = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
                    else
                        local switch6 = getMasterGameState
                        repeat
                            if switch6 == 1000 then
                                scratchValue3 = "TEXT_QST_081_MEET_SISTER"
                                break
                            elseif switch6 == 1050 then
                                scratchValue3 = "TEXT_QST_081_WAITING_FOR_MCC"
                                break
                            elseif switch6 == 1100 then
                                scratchValue3 = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
                                break
                            elseif switch6 == 1200 then
                                scratchValue3 = "TEXT_QST_081_GRAVEYARD"
                            else
                                do return "" end
                                goto FLOW_after_flow_native_label_2_242
                            end
                        until true
                    end
                else
                    local switch7 = getMasterGameState
                    repeat
                        if switch7 == 1233 then
                            scratchValue3 = "TEXT_QST_081_FINALGRAVEYARD"
                            break
                        elseif switch7 == 1250 then
                            scratchValue3 = "TEXT_QST_081_PRISON"
                            break
                        elseif switch7 == 1300 then
                            if not quest:GetMasterGameState("SeenAbbeyMotherAtGuild") then
                                scratchValue2 = quest:IsActiveThreadTerminating()
                                if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                scratchValue3 = "TEXT_QST_081_BEFORE_HOOK_COAST"
                            else
                                scratchValue2 = quest:IsActiveThreadTerminating()
                                if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                scratchValue3 = "TEXT_QST_081_HOOK_COAST"
                            end
                            break
                        elseif switch7 == 1450 then
                            scratchValue3 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                        else
                            -- FLOW_native_label_2: (native jump target)
                            return ""
                        end
                    until true
                end
                ::FLOW_after_flow_native_label_2_242::
            end
        else
            if getMasterGameState < 2301 then
                if getMasterGameState == 2300 then goto LAB_00e923f7 end
                if getMasterGameState < 1701 then
                    if getMasterGameState == 1700 then
                        scratchValue3 = "TEXT_QST_081_JACK_BOSS_FIGHT"
                    elseif getMasterGameState == 1550 then
                        scratchValue3 = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                    else
                        if getMasterGameState ~= 1600 then
                            do return "" end
                            goto FLOW_after_flow_native_label_2_357
                        end
                        scratchValue3 = "TEXT_QST_081_FOCAL_SITES"
                    end
                else
                    if getMasterGameState ~= 2100 then
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue3 = "TEXT_QST_081_SHIP_SUMMONING"
                end
            elseif getMasterGameState == 2400 then
                goto LAB_00e923f7
            elseif getMasterGameState == 2500 then
                -- TODO(native): iVar4 = *(iVar4 + 0x70)
                getHeroTitle = nil --[[unresolved native value]]
                if getHeroTitle == 0 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then
                        ReleaseEverything()
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS"
                elseif getHeroTitle == 1 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                elseif getHeroTitle == 2 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                elseif getHeroTitle == 3 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                elseif getHeroTitle == 4 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                elseif getHeroTitle == 5 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                elseif getHeroTitle == 6 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                else
                    if getHeroTitle ~= 7 then
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                            do return "" end
                            goto FLOW_after_flow_native_label_2_357
                        end
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue3 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                end
            else
                if getMasterGameState ~= 2600 then
                    do return "" end
                    goto FLOW_after_flow_native_label_2_357
                end
                scratchValue3 = "TEXT_QST_081_DRAGON_FIGHT_10"
            end
            goto FLOW_past_lab_00e923f7
            ::LAB_00e923f7::
            scratchValue3 = "TEXT_QST_081_THE_ORACLE"
            ::FLOW_past_lab_00e923f7::
        end
        ::FLOW_after_flow_native_label_2_357::
    end
    ::FLOW_after_flow_native_label_2::
    -- TODO(native): CVar2 = *quest:GetStateString("LastDialogueSaid")
    local scratchValue = nil --[[unresolved native value]]
    sequence1 = false
    sequence1 = scratchValue == in_stack_fffffff0
    if not sequence1 then
        sequence1 = scratchValue ~= nil and in_stack_fffffff0 ~= nil
        if sequence1 then
            -- TODO(native): if *(CVar2 + 4) == *(in_stack_fffffff0 + 4) then
            sequence1 = false
        end
        if sequence1 then
            -- TODO(native): iVar4 = CBasicString<char>::Compare(*(void **)CVar2,*(void **)in_stack_fffffff0);
            sequence1 = getHeroTitle == 0
        end
    end
    if sequence1 then
        scratchValue4 = math.random(0, 32767)
        scratchValue4 = scratchValue4 & 0x80000003
        scratchValue2 = scratchValue4 == 0
        if scratchValue4 < 0 then
            scratchValue2 = (scratchValue4 - 1 | 0xfffffffc) == 0xffffffff
        end
        if scratchValue2 then
            if not quest:GetStateBool("GuildMasterDialogue_2") then
                quest:SetStateBool("GuildMasterDialogue_2", true)
                scratchValue3 = "TEXT_QST_081_INFO_TELEPORTERS"
            elseif not quest:GetStateBool("GuildMasterDialogue_3") then
                quest:SetStateBool("GuildMasterDialogue_3", true)
                scratchValue3 = "TEXT_QST_081_INFO_WEAPONS"
            elseif not quest:GetStateBool("GuildMasterDialogue_4") then
                quest:SetStateBool("GuildMasterDialogue_4", true)
                scratchValue3 = "TEXT_QST_081_INFO_LOG_BOOK"
            else
                getHeroTitle = quest:GetHeroTitle()
                if not ((getHeroTitle ~= 19) or quest:GetStateBool("GuildMasterDialogue_1")) then
                    quest:SetStateBool("GuildMasterDialogue_1", true)
                    scratchValue3 = "TEXT_QST_081_INFO_HERO_TITLE"
                end
            end
        end
    end
    -- TODO(native): quest:SetStateString("LastDialogueSaid", &pcVar7)
    return scratchValue3
end

return {GetGuildMasterSpeech = GetGuildMasterSpeech}
