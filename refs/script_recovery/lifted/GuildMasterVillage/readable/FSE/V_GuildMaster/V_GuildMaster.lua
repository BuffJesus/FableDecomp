-- Readable native conversion: V_GuildMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_GuildMaster.Main (retail 0x00e90830)
function Main(quest)
    quest:AddEntityBinding("GuildMasterGameFlow", "V_GuildMaster/Entities/GuildMasterGameFlow", 1)
    quest:FinalizeEntityBindings()
end

-- V_GuildMaster.Init (retail 0x00e90780)
function Init(quest)
    quest:SetStateBool("GuildMasterDialogue_0", false)
    quest:SetStateBool("GuildMasterDialogue_1", false)
    quest:SetStateBool("GuildMasterDialogue_2", false)
    quest:SetStateBool("GuildMasterDialogue_3", false)
    quest:SetStateBool("GuildMasterDialogue_4", false)
end

-- V_GuildMaster.GetGuildMasterSpeech (retail 0x00e91f20)
-- E91F20: bsim names this body NScript::CV_GuildMasterScript::GetGuildMasterSpeech (a homologous script member); no PDB name
function GetGuildMasterSpeech(quest)
    local scratchValue2, scratchValue3, sequence, scratchValue4
    -- TODO(native): CCharString::CCharString(&xStack_c,other);
    local function ReleaseEverything()
        do return "" end
    end
    local getMasterGameState = quest:GetMasterGameState("PostSavePosition")
    if getMasterGameState < 901 then
        if getMasterGameState == 900 then
            scratchValue4 = "TEXT_QST_081_ARENA"
        elseif getMasterGameState < 701 then
            if getMasterGameState == 700 then
                goto LAB_00e92080
            elseif getMasterGameState < 401 then
                if getMasterGameState == 400 then
                    scratchValue4 = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
                else
                    repeat
                        if getMasterGameState == 100 then
                            scratchValue4 = "TEXT_QST_081_TRAINING"
                            break
                        elseif getMasterGameState == 150 then
                            scratchValue4 = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
                            break
                        elseif getMasterGameState == 200 then
                            scratchValue4 = "TEXT_QST_081_WASP_BOSS"
                            break
                        elseif getMasterGameState == 300 then
                            scratchValue4 = "TEXT_QST_081_MAZE_MEETING"
                        else
                            -- FLOW_native_label_2_c1: (native jump target)
                            do return "" end
                            goto FLOW_after_flow_native_label_2
                        end
                    until true
                end
            else
                repeat
                    if getMasterGameState == 450 then
                        scratchValue4 = "TEXT_QST_081_ORCHARD_FARM"
                        break
                    elseif getMasterGameState == 500 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
                        break
                    elseif getMasterGameState == 550 then
                        scratchValue4 = "TEXT_QST_081_TRADER_ESCORT"
                        break
                    elseif getMasterGameState == 600 then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp") then
                            if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                            goto LAB_00e92080
                        end
                        if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                        scratchValue4 = "TEXT_QST_081_SECOND_MAZE_MEETING"
                    else
                        -- FLOW_native_label_2_c2: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                until true
            end
            goto FLOW_past_lab_00e92080
            ::LAB_00e92080::
            scratchValue4 = "TEXT_QST_081_BANDIT_CAMP"
            ::FLOW_past_lab_00e92080::
        else
            repeat
                if getMasterGameState == 733 then
                    scratchValue4 = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
                    break
                elseif getMasterGameState == 766 then
                    scratchValue4 = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
                    break
                else
                    if getMasterGameState == 800 then
                        local predicateResult2 = quest:IsActiveThreadTerminating()
                        if quest:IsQuestActive("V_TrophyDealer") then
                            if predicateResult2 then
                                -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                do return "" end
                                goto FLOW_after_flow_native_label_2
                            end
                            goto FLOW_native_label_1
                        end
                        if predicateResult2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                        scratchValue4 = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                        break
                    elseif getMasterGameState == 850 then
                        goto FLOW_native_label_1
                    elseif getMasterGameState == 856 then
                        scratchValue4 = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
                        break
                    elseif getMasterGameState == 865 then
                        scratchValue4 = "TEXT_QST_081_WHITE_BALVERINE_KHG"
                        break
                    elseif getMasterGameState == 870 then
                        scratchValue4 = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
                        break
                    elseif getMasterGameState == 875 then
                        scratchValue4 = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
                    else
                        -- FLOW_native_label_2_c4: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                    goto FLOW_past_flow_native_label_1
                    ::FLOW_native_label_1::
                    scratchValue4 = "TEXT_QST_081_TROPHY_DEALER"
                    break
                    ::FLOW_past_flow_native_label_1::
                end
            until true
        end
    else
        if getMasterGameState < 1501 then
            if getMasterGameState == 1500 then
                scratchValue4 = "TEXT_QST_081_WIZARD_BATTLE"
            else
                if getMasterGameState < 1217 then
                    if getMasterGameState == 1216 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
                    else
                        repeat
                            if getMasterGameState == 1000 then
                                scratchValue4 = "TEXT_QST_081_MEET_SISTER"
                                break
                            elseif getMasterGameState == 1050 then
                                scratchValue4 = "TEXT_QST_081_WAITING_FOR_MCC"
                                break
                            elseif getMasterGameState == 1100 then
                                scratchValue4 = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
                                break
                            elseif getMasterGameState == 1200 then
                                scratchValue4 = "TEXT_QST_081_GRAVEYARD"
                            else
                                do return "" end
                                goto FLOW_after_flow_native_label_2_242
                            end
                        until true
                    end
                else
                    repeat
                        if getMasterGameState == 1233 then
                            scratchValue4 = "TEXT_QST_081_FINALGRAVEYARD"
                            break
                        elseif getMasterGameState == 1250 then
                            scratchValue4 = "TEXT_QST_081_PRISON"
                            break
                        elseif getMasterGameState == 1300 then
                            if not quest:GetMasterGameState("SeenAbbeyMotherAtGuild") then
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                scratchValue4 = "TEXT_QST_081_BEFORE_HOOK_COAST"
                            else
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                                scratchValue4 = "TEXT_QST_081_HOOK_COAST"
                            end
                            break
                        elseif getMasterGameState == 1450 then
                            scratchValue4 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
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
                        scratchValue4 = "TEXT_QST_081_JACK_BOSS_FIGHT"
                    elseif getMasterGameState == 1550 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                    else
                        if getMasterGameState ~= 1600 then
                            do return "" end
                            goto FLOW_after_flow_native_label_2_357
                        end
                        scratchValue4 = "TEXT_QST_081_FOCAL_SITES"
                    end
                else
                    if getMasterGameState ~= 2100 then
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue4 = "TEXT_QST_081_SHIP_SUMMONING"
                end
            elseif getMasterGameState == 2400 then
                goto LAB_00e923f7
            elseif getMasterGameState == 2500 then
                -- TODO(native): iVar4 = *(iVar4 + 0x70)
                scratchValue3 = nil --[[unresolved native value]]
                if scratchValue3 == 0 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto FLOW_after_flow_native_label_2_357 end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS"
                elseif scratchValue3 == 1 then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                elseif scratchValue3 == 2 then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                elseif scratchValue3 == 3 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                elseif scratchValue3 == 4 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                elseif scratchValue3 == 5 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                elseif scratchValue3 == 6 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                else
                    if scratchValue3 ~= 7 then
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                            do return "" end
                            goto FLOW_after_flow_native_label_2_357
                        end
                        do return "" end
                        goto FLOW_after_flow_native_label_2_357
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_357
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                end
            else
                if getMasterGameState ~= 2600 then
                    do return "" end
                    goto FLOW_after_flow_native_label_2_357
                end
                scratchValue4 = "TEXT_QST_081_DRAGON_FIGHT_10"
            end
            goto FLOW_past_lab_00e923f7
            ::LAB_00e923f7::
            scratchValue4 = "TEXT_QST_081_THE_ORACLE"
            ::FLOW_past_lab_00e923f7::
        end
        ::FLOW_after_flow_native_label_2_357::
    end
    ::FLOW_after_flow_native_label_2::
    -- TODO(native): CVar2 = *quest:GetStateString("LastDialogueSaid")
    local scratchValue = nil --[[unresolved native value]]
    sequence = scratchValue == in_stack_fffffff0
    if not sequence then
        sequence = scratchValue ~= nil and in_stack_fffffff0 ~= nil
        if sequence then
            -- TODO(native): if *(CVar2 + 4) == *(in_stack_fffffff0 + 4) then
            sequence = false
        end
        if sequence then
            -- TODO(native): iVar4 = CBasicString<char>::Compare(*(void **)CVar2,*(void **)in_stack_fffffff0);
            sequence = scratchValue3 == 0
        end
    end
    if sequence then
        local scratchValue6 = math.random(0, 32767) & 0x80000003
        scratchValue2 = scratchValue6 == 0
        if scratchValue6 < 0 then
            scratchValue2 = (scratchValue6 - 1 | 0xfffffffc) == 0xffffffff
        end
        if scratchValue2 then
            if not quest:GetStateBool("GuildMasterDialogue_2") then
                quest:SetStateBool("GuildMasterDialogue_2", true)
                scratchValue4 = "TEXT_QST_081_INFO_TELEPORTERS"
            elseif not quest:GetStateBool("GuildMasterDialogue_3") then
                quest:SetStateBool("GuildMasterDialogue_3", true)
                scratchValue4 = "TEXT_QST_081_INFO_WEAPONS"
            elseif not quest:GetStateBool("GuildMasterDialogue_4") then
                quest:SetStateBool("GuildMasterDialogue_4", true)
                scratchValue4 = "TEXT_QST_081_INFO_LOG_BOOK"
            elseif not ((quest:GetHeroTitle() ~= 19) or quest:GetStateBool("GuildMasterDialogue_1")) then
                quest:SetStateBool("GuildMasterDialogue_1", true)
                scratchValue4 = "TEXT_QST_081_INFO_HERO_TITLE"
            end
        end
    end
    -- TODO(native): quest:SetStateString("LastDialogueSaid", &pcVar7)
    return scratchValue4
end

