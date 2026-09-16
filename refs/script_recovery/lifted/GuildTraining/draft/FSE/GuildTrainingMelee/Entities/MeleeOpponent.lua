-- Generated native draft: MeleeOpponent. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local CVar2, CVar27, bVar4, cVar5, fVar16, fVar17, iVar28, iVar9, pCStack_110, pCVar1, pCVar18, pCVar19, pCVar21, pCVar23, pCVar24, pCVar26, pCVar7, paVar14, pcVar22, piVar6, ppVar10, r1, r2, r3, r4, r5, r6, uVar11, uVar12, uVar13, uVar20, uVar8
    local alive = true
    -- TODO(native): auStack_c4[0] = 0;
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_d8);
    if bVar4 then
    end
    -- TODO(native): pppuVar25 = &ppuStack_d8;
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:EntitySetAsKillable(nil --[[missing]], false)
        cVar5 = quest:GetStateBool("TalkedToWhisper")
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5685e end
            cVar5 = quest:GetStateBool("TalkedToWhisper")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            piVar6 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
            pCVar24 = 0x1
            pcVar22 = 0x0
            pCVar7 = piVar6:GetPos()
            me:MoveToPosition(pCVar7, pCVar18, SUB41(me,0), SUB41(pppuVar25,0))
            bVar4 = me:IsPerformingScriptTask()
            CVar27 = SUB41(unaff_EBX,0)
            if bVar4 then
                repeat
                    CVar27 = SUB41(unaff_EBX,0)
                    if quest:GetStateBool("WhisperStopWalking") == '\x01' then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d5685e end
                    bVar4 = me:IsPerformingScriptTask()
                    CVar27 = SUB41(unaff_EBX,0)
                until not (bVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff1c);
                if bVar4 then
                end
                quest:SetStateBool("WhisperArrived", true)
                quest:EntitySetAsKillable(piVar6, (pcVar22 ~= 0))
                iVar9 = quest:GetStateInt("TutorialState")
                while iVar9 == 1 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d57f5d end
                    cVar5 = me:IsTalkedToByHero()
                    if not cVar5 then
                        cVar5 = me:MsgIsHitByHero()
                        if cVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d57f5d end
                            quest:SetStateBool("EarlyHitWhisper", true)
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d57f5d end
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff04);
                        if bVar4 then
                        end
                        cVar5 = me:AcquireControl(4)
                        while true do
                            uVar20 = SUB41(me,0)
                            if not (not cVar5) then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                return
                            end
                            cVar5 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d56eb7: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            return
                        end
                        -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffef8);
                        fVar16 = quest:GetHealth(nil --[[missing]])
                        fVar17 = _DAT_0122dedc
                        if fVar17 < fVar16 then
                            bVar4 = false
                            pCVar19 = 0x1
                            pCVar18 = 0x0
                            pCVar26 = 0x0
                            pcVar22 = "TEXT_QST_028_WHISPER_MEET"
                            pCVar7 = quest:GetHero()
                            r1 = me:Speak(pCVar7, pcVar22, pCVar26, (pCVar18 ~= 0), (pCVar19 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        return
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d56eb7
                        end
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffef8);
                        if bVar4 then
                        end
                        quest:PauseAllNonScriptedEntities(false)
                    end
                    iVar9 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    CVar2 = __native_entity_state:GetStateBool("RepeatMelee")
                    while CVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d57f5d end
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 ~= 3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d57f5d end
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d57f5d end
                        quest:SetStateBool("MeleeOpponentReset", false)
                        quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK")
                        pCStack_110 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCStack_110)
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(0)
                        -- TODO(native): CTimer::CTimer((CTimer *)&pCStack_110);
                        quest:SetTimer(0xf, pCStack_110)
                        iVar9 = quest:GetStateInt("TutorialState")
                        iVar28 = 0
                        while (iVar9 == 3 and (quest:GetStateInt("GenericTutorialCounter") < 7)) do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                            if not cVar5 then
                                cVar5 = me:MsgIsHitByHero()
                                if cVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    quest:ModifyThingHealth(me, iVar28)
                                    if iVar28 == 0 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        cVar5 = quest:IsXbox()
                                        if not cVar5 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                            pCVar26 = "TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC"
                                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC")
                                            cVar5 = quest:MsgIsGameInfoClickedPast()
                                            while not cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                                cVar5 = quest:MsgIsGameInfoClickedPast()
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                            pCVar26 = "TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP"
                                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP")
                                            cVar5 = quest:MsgIsGameInfoClickedPast()
                                            while not cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                                cVar5 = quest:MsgIsGameInfoClickedPast()
                                            end
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    end
                                    iVar28 = (iVar28 + 1) % 5
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                                iVar9 = quest:GetTimer(0)
                                if iVar9 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    ppVar10 = quest:AddNewConversation(me, false, (pCVar24 ~= 0))
                                    uVar8 = quest:GetHero()
                                    quest:AddPersonToConversation(ppVar10, uVar8)
                                    uVar8 = quest:GetHero()
                                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, uVar8, false)
                                    quest:SetTimer(0xf, uVar8)
                                end
                            end
                            iVar9 = quest:GetTimer(0)
                            if iVar9 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                uVar8 = quest:GetHero()
                                quest:AddPersonToConversation(ppVar10, uVar8)
                                uVar8 = quest:GetHero()
                                quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", me, uVar8, false)
                                quest:SetTimer(0xf, uVar8)
                            end
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d57f7a: (native jump target)
                            return
                        end
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 ~= 4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE")
                        pCStack_110 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCStack_110)
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(0)
                        uVar8 = 0
                        quest:SetTimer(0xf, pCStack_110)
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 == 4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            cVar5 = quest:IsPlayerCreatureBlocking()
                            if not cVar5 then
                                -- LAB_00d57276: (native jump target)
                                bVar4 = false
                            else
                                -- TODO(native): in_stack_ffffff24 = in_stack_ffffff24 | 1;
                                piVar6 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                cVar5 = piVar6:MsgIsHitBy("MeleeOpponent")
                                if not cVar5 then return end  -- TODO(native): goto LAB_00d57276
                                bVar4 = true
                            end
                            if (in_stack_ffffff24 & 1) ~= 0 then
                                -- TODO(native): in_stack_ffffff24 = in_stack_ffffff24 & 0xfffffffe;
                            end
                            if bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                                iVar9 = quest:GetTimer(bVar4)
                                if iVar9 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    ppVar10 = quest:AddNewConversation(me, (uVar8 ~= 0), nil --[[missing]])
                                    uVar11 = quest:GetHero()
                                    quest:AddPersonToConversation(ppVar10, uVar11)
                                    uVar11 = quest:GetHero()
                                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, uVar11, false)
                                    -- LAB_00d57742: (native jump target)
                                    quest:SetTimer(0xf, uVar11)
                                end
                            else
                                piVar6 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                cVar5 = piVar6:MsgIsHitBy("MeleeOpponent")
                                if not cVar5 then
                                    cVar5 = me:MsgIsHitByHero()
                                    if cVar5 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        iVar9 = quest:GetTimer(nil --[[missing]])
                                        if iVar9 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if alive then
                                                ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                                uVar11 = quest:GetHero()
                                                quest:AddPersonToConversation(ppVar10, uVar11)
                                                uVar11 = quest:GetHero()
                                                quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", me, uVar11, false)
                                                -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_78;
                                                -- TODO(native): goto LAB_00d57742
                                            end
                                            -- TODO(native): goto LAB_00d57f7a
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    if iVar28 == 0 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) auStack_c4;
                                            r2 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                            ppVar10 = quest:AddNewConversation(r2, false, false)
                                            uVar11 = quest:GetHero()
                                            quest:AddPersonToConversation(ppVar10, uVar11)
                                            uVar11 = quest:GetHero()
                                            quest:AddLineToConversation(ppVar10, "TEXT_QST_028_MAZE_BLOCK", uVar11, piVar6, false)
                                            cVar5 = quest:IsXbox()
                                            if not cVar5 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    pCVar26 = "TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC"
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")
                                                    cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    while not cVar5 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00d57f71 end
                                                        cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    -- TODO(native): goto LAB_00d5754c
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    paVar14 = "TEXT_QST_028_ONSCREENHELP_BLOCK_HELP"
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP")
                                                    cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    while not cVar5 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00d57f71 end
                                                        cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    -- LAB_00d5754c: (native jump target)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if alive then
                                                        quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                                        iVar28 = (iVar28 + 1) % 5
                                                        goto LAB_00d5775c
                                                    end
                                                end
                                            end
                                            ::LAB_00d57f71::
                                        end
                                        -- TODO(native): goto LAB_00d57f7a
                                    end
                                    uVar12 = rand()
                                    uVar12 = uVar12 & 0x80000001
                                    if uVar12 < 0 then
                                        uVar12 = (uVar12 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar12 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        iVar9 = quest:GetTimer(nil --[[missing]])
                                        if iVar9 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                            ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                            uVar11 = quest:GetHero()
                                            quest:AddPersonToConversation(ppVar10, uVar11)
                                            uVar11 = quest:GetHero()
                                            quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, uVar11, false)
                                            quest:SetTimer(0xf, uVar11)
                                        end
                                    end
                                    iVar28 = (iVar28 + 1) % 5
                                end
                            end
                            ::LAB_00d5775c::
                            iVar9 = quest:GetTimer(nil --[[missing]])
                            if iVar9 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                uVar11 = quest:GetHero()
                                quest:AddPersonToConversation(ppVar10, uVar11)
                                uVar11 = quest:GetHero()
                                quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", me, uVar11, false)
                                quest:SetTimer(0xf, uVar11)
                            end
                            -- TODO(native): iVar3 = DAT_0143e90c;
                            r3 = quest:GetHero()
                            fVar17 = quest:GetHealth(r3)
                            if fVar17 < *(iVar3 + 0xed8) then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                quest:ClearThingBestEnemyTarget(nil --[[missing]])
                            end
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 ~= 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                        quest:SetTimer(uVar8, nil --[[missing]])
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 == 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            cVar5 = me:MsgIsHitByHero()
                            if not cVar5 then
                                cVar5 = quest:IsPlayerCreatureBlocking()
                                if not cVar5 then
                                    -- LAB_00d57af8: (native jump target)
                                    bVar4 = false
                                else
                                    -- TODO(native): in_stack_ffffff24 = in_stack_ffffff24 | 2;
                                    piVar6 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    cVar5 = piVar6:MsgIsHitBy("MeleeOpponent")
                                    if not cVar5 then return end  -- TODO(native): goto LAB_00d57af8
                                    bVar4 = true
                                end
                                if (in_stack_ffffff24 & 2) ~= 0 then
                                    -- TODO(native): in_stack_ffffff24 = in_stack_ffffff24 & 0xfffffffd;
                                end
                                if bVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    iVar9 = quest:GetTimer(bVar4)
                                    if iVar9 < 9 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            uVar12 = rand()
                                            uVar12 = uVar12 & 0x80000001
                                            if uVar12 < 0 then
                                                uVar12 = (uVar12 - 1 | 0xfffffffe) + 1
                                            end
                                            if uVar12 == 1 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                                    uVar8 = quest:GetHero()
                                                    quest:AddPersonToConversation(ppVar10, uVar8)
                                                    uVar8 = quest:GetHero()
                                                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, uVar8, false)
                                                    -- TODO(native): goto LAB_00d57e76
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    r4 = quest:GetThingWithScriptName("MeleeThunder")
                                                    uVar11 = quest:AddNewConversation(r4, false, false)
                                                    uVar8 = quest:GetHero()
                                                    quest:AddPersonToConversation(uVar11, uVar8)
                                                    uVar13 = quest:GetHero()
                                                    ppVar10 = 0x0
                                                    quest:AddLineToConversation(uVar11, "TEXT_QST_028_THUNDER_MELEE_ATTACK", uVar13, piVar6, false)
                                                    -- TODO(native): this = (C3DClothPrimitive *)&stack0xffffff04;
                                                    -- TODO(native): goto LAB_00d57e71
                                                end
                                            end
                                        end
                                        -- TODO(native): goto LAB_00d57f7a
                                    end
                                else
                                    piVar6 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    cVar5 = piVar6:MsgIsHitBy("MeleeOpponent")
                                    if cVar5 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        iVar9 = quest:GetTimer(ppVar10)
                                        if iVar9 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if alive then
                                                uVar12 = rand()
                                                uVar12 = uVar12 & 0x80000001
                                                if uVar12 < 0 then
                                                    uVar12 = (uVar12 - 1 | 0xfffffffe) + 1
                                                end
                                                if uVar12 == 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if alive then
                                                        ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                                        uVar8 = quest:GetHero()
                                                        quest:AddPersonToConversation(ppVar10, uVar8)
                                                        uVar8 = quest:GetHero()
                                                        quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, uVar8, false)
                                                        -- TODO(native): goto LAB_00d57e76
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if alive then
                                                        r5 = quest:GetThingWithScriptName("MeleeThunder")
                                                        uVar11 = quest:AddNewConversation(r5, aCStack_38, false)
                                                        uVar8 = quest:GetHero()
                                                        quest:AddPersonToConversation(uVar11, uVar8)
                                                        uVar13 = quest:GetHero()
                                                        ppVar10 = 0x0
                                                        quest:AddLineToConversation(uVar11, "TEXT_QST_028_THUNDER_MELEE_FINISH", uVar13, piVar6, false)
                                                        -- TODO(native): this = (C3DClothPrimitive *)aaStack_58;
                                                        -- TODO(native): goto LAB_00d57e71
                                                    end
                                                end
                                            end
                                            -- TODO(native): goto LAB_00d57f7a
                                        end
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                iVar9 = quest:GetTimer(ppVar10)
                                if iVar9 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                    uVar12 = rand()
                                    uVar12 = uVar12 & 0x80000001
                                    if uVar12 < 0 then
                                        uVar12 = (uVar12 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar12 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        ppVar10 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                                        uVar8 = quest:GetHero()
                                        quest:AddPersonToConversation(ppVar10, uVar8)
                                        uVar8 = quest:GetHero()
                                        quest:AddLineToConversation(ppVar10, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, uVar8, false)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                                        r6 = quest:GetThingWithScriptName("MeleeThunder")
                                        uVar11 = quest:AddNewConversation(r6, auStack_2c, false)
                                        uVar8 = quest:GetHero()
                                        quest:AddPersonToConversation(uVar11, uVar8)
                                        uVar13 = quest:GetHero()
                                        ppVar10 = 0x0
                                        quest:AddLineToConversation(uVar11, "TEXT_QST_028_THUNDER_MELEE_DEFEND", uVar13, nil --[[missing]], false)
                                        -- TODO(native): this = (C3DClothPrimitive *)aCStack_4c;
                                        -- LAB_00d57e71: (native jump target)
                                    end
                                    -- LAB_00d57e76: (native jump target)
                                    quest:SetTimer(0xf, uVar8)
                                end
                            end
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        iVar9 = quest:GetStateInt("TutorialState")
                        while iVar9 ~= 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            iVar9 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        cVar5 = quest:GetStateBool("MeleeRepeatKnown")
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            cVar5 = quest:GetStateBool("MeleeRepeatKnown")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        if not quest:GetStateBool("MeleeRepeating") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                            __native_entity_state:SetStateBool("RepeatMelee", false)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d57f7a
                        end
                        quest:SetStateBool("MeleeOpponentReset", true)
                        CVar2 = __native_entity_state:GetStateBool("RepeatMelee")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                end
                ::LAB_00d57f5d::
                return
            end
        end
    end
    ::LAB_00d5685e::
end

function Init(quest, me)
    quest:SetStateInt("TutorialState", 1)
    __native_entity_state:SetStateInt("BadHit", 0)
    __native_entity_state:SetStateBool("RepeatMelee", true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

