-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
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
    local resources = quest:RetailResources()
    local __native_condition_1, __native_condition_2, bVar18, bVar3, cVar4, delay, fVar19, fVar2, f_stk_70, f_stk_74, fret_00, fret_01, fret_02, fret_03, fret_04, fret_06, fret_07, iVar17, iVar20, iVar6, iVar7, native_arg_switch_2, p4, pCVar10, pCVar11, pCVar16, pCVar5, pCVar9, pcVar14, pfVar13, piVar12, pppuVar15, r1, r2, r3, r4, r5, r6, r7, r8, thing, uVar1, xStack_1c0, xStack_1d0, xStack_204, xStack_214, xStack_220, xStack_23c, xStack_250, xStack_28, xStack_38, xStack_48, xStack_54, xStack_6c, xStack_84, xStack_94, xStack_a4, xStack_b0, xStack_c0, xStack_d0, xStack_e0, x_stk_1ec
    local alive = true
    xStack_250 = resources:NewResource()
    bVar3 = false
    if bVar3 ~= 0 then
    end
    bVar3 = resources:TryAcquire(xStack_250, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = resources:TryAcquire(0, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    bVar3 = true
    pCVar5 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, pCVar5, bVar3)
    uVar1 = __native_entity_state:GetStateInt("self_0xc")
    -- TODO(native): thing._4_4_ = uVar1;
    thing = nil
    -- TODO(native): thing._8_4_ = piVar12;
    quest:SetIsPushableByHero(nil --[[missing]], (thing ~= 0))
    r1 = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    iVar6 = quest:RegisterTimer()
    quest:SetTimer(iVar6, 0)
    iVar7 = quest:GetStateInt("TutorialState")
    x_stk_1ec = 0x0
    while iVar7 == 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        iVar6 = iVar6
        if bVar3 then
            quest:DeregisterTimer(iVar6)
            goto FLOW_after_lab_00d5a8cb
        end
        cVar4 = me:IsTalkedToByHero()
        if not cVar4 then
            if (not quest:GetStateBool("EarlyHitWhisper")) or (not quest:GetStateBool("WhisperArrived")) then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c | 1);
                cVar4 = me:MsgIsHitByHero()
                if cVar4 then
                    bVar3 = true
                    goto FLOW_after_lab_00d586db
                end
                bVar3 = false
            else
                -- LAB_00d586db: (native jump target)
                bVar3 = true
            end
            ::FLOW_after_lab_00d586db::
            if (xStack_23c & 1) ~= 0 then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c & 0xfffffffe);
            end
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                iVar6 = iVar6
                if bVar3 then
                    quest:DeregisterTimer(iVar6)
                    goto FLOW_after_lab_00d5a8cb
                end
                xStack_204 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                xStack_220 = resources:ScriptThing(0)
                pCVar5 = xStack_220
                r2 = quest:GetHealth(pCVar5)
                fVar2 = 0.0
                xStack_220 = nil
                if fVar2 < fret_0 then
                    iVar17 = 0
                    iVar20 = 1
                    iVar6 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER"
                    pCVar5 = quest:GetHero()
                    r3 = me:Speak(pCVar5, pcVar14, iVar7, (iVar6 ~= 0), (iVar20 ~= 0), (iVar17 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar4 = iVar7
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_204)
                            goto LAB_00d5933c
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar4 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_38)
                        iVar6 = iVar6
                        quest:DeregisterTimer(iVar6)
                        goto FLOW_after_lab_00d5a8cb
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_204)
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            iVar6 = iVar6
            if bVar3 then
                quest:DeregisterTimer(iVar6)
                goto FLOW_after_lab_00d5a8cb
            end
            quest:SetStateInt("TutorialState", 2)
        end
        fVar19 = 5.5
        pCVar5 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar19)
        __native_condition_2 = bVar3
        if __native_condition_2 then
            iVar7 = quest:GetTimer(iVar6)
            __native_condition_2 = iVar7 < 1
        end
        __native_condition_1 = __native_condition_2
        if __native_condition_1 then
            iVar7 = me:IsPerformingScriptTask()
            __native_condition_1 = not iVar7
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar6)
                goto FLOW_after_lab_00d5a8cb
            end
            iVar6 = quest:AddNewConversation(me, false, false)
            pCVar5 = quest:GetHero()
            quest:AddPersonToConversation(iVar6, pCVar5)
            quest:SetTimer(iVar6, xStack_264)
            if x_stk_1ec == nil then
                bVar3 = false
                pCVar5 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                pCVar5 = quest:GetHero()
                quest:AddLineToConversation(iVar6, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", me, pCVar5, false)
                x_stk_1ec = 0x1
            else
                if x_stk_1ec == 0x1 then
                    bVar3 = false
                    pCVar5 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", me, pCVar5, false)
                end
            end
        end
        iVar7 = quest:GetStateInt("TutorialState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d5a9be: (native jump target)
    else
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(r1)
        cVar4 = quest:GetStateBool("MeleeRepeating")
        while cVar4 == '\x01' do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
            quest:SetStateBool("MeleeRepeatKnown", false)
            xStack_204 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            iVar6 = 4
            pppuVar15 = xStack_204
            pCVar5 = quest:GetHero()
            bVar3 = resources:TryAcquire(pppuVar15, pCVar5, iVar6)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(xStack_94)
                    goto LAB_00d5a8d9
                end
                iVar6 = 4
                pppuVar15 = xStack_204
                pCVar5 = quest:GetHero()
                bVar3 = resources:TryAcquire(pppuVar15, pCVar5, iVar6)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a902: (native jump target)
                resources:ReleaseResource(xStack_94)
                goto LAB_00d5a8d9
            end
            xStack_94 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_94, r1, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(xStack_204)
                    resources:ReleaseResource(xStack_94)
                    goto LAB_00d5a8d9
                end
                bVar3 = resources:TryAcquire(xStack_94, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a8f6: (native jump target)
                resources:ReleaseResource(xStack_204)
                resources:ReleaseResource(xStack_94)
                goto LAB_00d5a8d9
            end
            xStack_54 = resources:NewActorMap()
            resources:SetActor(xStack_54, "HERO", xStack_94)
            resources:SetActor(xStack_54, "TEACHER", 0)
            resources:SetActor(xStack_54, "WHISPER", xStack_204)
            xStack_38 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_ATTACK", xStack_54, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_28)
            resources:DestroyActorMap(xStack_54)
            resources:ReleaseResource(xStack_84)
            resources:ReleaseResource(xStack_d0)
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(true)
            xStack_23c = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0)
            quest:SetStateInt("TutorialState", 3)
            -- TODO(native): CTimer::CTimer((CTimer *)&xStack_260);
            quest:SetTimer(iVar6, xStack_260)
            iVar7 = quest:GetStateInt("GenericTutorialCounter")
            while iVar7 < 7 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                iVar7 = quest:GetTimer(iVar6)
                if iVar7 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_HELP_ATTACK", me, pCVar5, false)
                    quest:SetTimer(iVar6, xStack_260)
                end
                quest:UpdateQuestInfoCounter(xStack_23c, quest:GetStateInt("GenericTutorialCounter"), -1)
                pCVar5 = quest:GetHero()
                fret_00 = quest:GetHealth(pCVar5)
                if fret_00 < quest:ReadGlobalGameData(0xed8) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_HEAL_HERO", me, pCVar5, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
                iVar7 = quest:GetStateInt("GenericTutorialCounter")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(xStack_23c)
            xStack_d0 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            iVar6 = 4
            pCVar16 = xStack_d0
            pCVar5 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar6)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a922
                iVar6 = 4
                pCVar16 = xStack_d0
                pCVar5 = quest:GetHero()
                bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar6)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a922: (native jump target)
                resources:DestroyMovie(xStack_204)
                -- LAB_00d5a9b5: (native jump target)
                goto FLOW_after_lab_00d5a8cb
            end
            xStack_84 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_84, r1, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a916
                bVar3 = resources:TryAcquire(xStack_84, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a916: (native jump target)
                resources:DestroyMovie(xStack_48)
                -- TODO(native): goto LAB_00d5a922
            end
            xStack_6c = resources:NewActorMap()
            resources:SetActor(xStack_6c, "HERO", xStack_204)
            resources:SetActor(xStack_6c, "TEACHER", 0)
            resources:SetActor(xStack_6c, "WHISPER", xStack_48)
            xStack_28 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", xStack_6c, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(xStack_a4)
            resources:DestroyActorMap(xStack_6c)
            resources:ReleaseResource(xStack_e0)
            resources:DestroyMovie(xStack_c0)
            quest:SetStateInt("GenericTutorialCounter", 0)
            bVar3 = quest:IsXbox()
            if not bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    bVar3 = quest:MsgIsGameInfoClickedPast()
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                        bVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_10c);
                        goto LAB_00d5941c
                    end
                end
                -- TODO(native): goto LAB_00d5a9b5
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK")
            bVar3 = quest:MsgIsGameInfoClickedPast()
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                bVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
            ::LAB_00d5941c::
            quest:SetStateInt("TutorialState", 4)
            xStack_23c = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0)
            quest:DisplayQuestInfo(true)
            iVar7 = quest:GetStateInt("GenericTutorialCounter")
            while iVar7 < 5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                quest:UpdateQuestInfoCounter(xStack_23c, quest:GetStateInt("GenericTutorialCounter"), -1)
                pCVar5 = quest:GetHero()
                fret_01 = quest:GetHealth(pCVar5)
                if fret_01 < quest:ReadGlobalGameData(0xed8) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_HEAL_HERO", me, pCVar5, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
                iVar7 = quest:GetStateInt("GenericTutorialCounter")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            pCVar5 = quest:GetThingWithScriptName("SkillApprenticeMarker")
            bVar3 = false
            pCVar9 = pCVar5:GetPos()
            r4 = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", pCVar9, "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(r4, 1)
            xStack_214 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_214, r4, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                bVar3 = resources:TryAcquire(xStack_1c0, r4, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a9a7: (native jump target)
                resources:ReleaseResource(xStack_1c0)
                -- TODO(native): goto LAB_00d5a9b5
            end
            xStack_e0 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            iVar6 = 4
            pCVar16 = xStack_e0
            pCVar5 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar6)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a93f
                iVar6 = 4
                pCVar16 = xStack_e0
                pCVar5 = quest:GetHero()
                bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar6)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a93f: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_e0;
                -- LAB_00d5a99e: (native jump target)
                resources:ReleaseResource(this_01)
                -- TODO(native): goto LAB_00d5a9a7
            end
            xStack_a4 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_a4, r1, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a933
                bVar3 = resources:TryAcquire(xStack_214, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a933: (native jump target)
                resources:ReleaseResource(xStack_214)
                -- TODO(native): goto LAB_00d5a93f
            end
            xStack_b0 = resources:NewActorMap()
            resources:SetActor(xStack_b0, "HERO", xStack_e0)
            resources:SetActor(xStack_b0, "TEACHER", 0)
            resources:SetActor(xStack_b0, "THUNDER", xStack_1c0)
            resources:SetActor(xStack_b0, "WHISPER", xStack_214)
            xStack_48 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BATTLE", xStack_b0, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:ModifyThingHealth(pCVar5, 1000.0, false)
            quest:SetStateInt("TutorialState", 6)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(0)
            resources:DestroyActorMap(xStack_b0)
            resources:ReleaseResource(xStack_94)
            resources:ReleaseResource(xStack_204)
            __native_entity_state:SetStateBool("HeroStanding", true)
            __native_entity_state:SetStateBool("WhisperStanding", true)
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(xStack_23c)
            quest:DisplayQuestInfo(true)
            fVar19 = 1.0
            pCVar5 = quest:GetThingWithScriptName("MeleeOpponent")
            r5 = quest:AddQuestInfoBarHealth(pCVar5, pCVar11, "HUD_WHISPER_ICON", fVar19)
            pCVar5 = quest:GetHero()
            fret_02 = quest:GetHealth(pCVar5)
            f_stk_70 = fret_02
            fret_03 = quest:GetHealth(nil --[[missing]])
            f_stk_74 = fret_03
            cVar4 = __native_entity_state:GetStateBool("HeroStanding")
            while (cVar4 and (__native_entity_state:GetStateBool("WhisperStanding"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                pCVar5 = quest:GetHero()
                fret_04 = quest:GetHealth(pCVar5)
                if quest:ReadGlobalGameData(0xed8) <= fret_04 then
                    pCVar5 = quest:GetThingWithScriptName("MeleeOpponent")
                    r6 = quest:GetHealth(pCVar5)
                    fVar19 = quest:ReadGlobalGameData(0xed8)
                    if fret_05 < fVar19 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            __native_entity_state:SetStateBool("WhisperStanding", false)
                            iVar6 = quest:AddNewConversation(me, false, false)
                            pCVar5 = quest:GetHero()
                            quest:AddPersonToConversation(iVar6, pCVar5)
                            pCVar5 = quest:GetHero()
                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_FIGHT_OVER", me, pCVar5, false)
                            goto FLOW_after_lab_00d59cc6
                        end
                        -- TODO(native): goto LAB_00d5a9a7
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                    __native_entity_state:SetStateBool("HeroStanding", false)
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_FIGHT_OVER", me, pCVar5, false)
                    -- LAB_00d59cc6: (native jump target)
                end
                ::FLOW_after_lab_00d59cc6::
                cVar4 = __native_entity_state:GetStateBool("HeroStanding")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
            quest:ClearAllRumbles()
            pCVar5 = quest:GetHero()
            fret_06 = quest:GetHealth(pCVar5)
            -- TODO(native): xStack_1d4 = (CCharString)(float)fret_06;
            fret_07 = quest:GetHealth(nil --[[missing]])
            -- TODO(native): pfVar13 = *(float **)(DAT_0143e90c + 0xeb4);
            -- TODO(native): xStack_1d4 = (CCharString) (float)(((float10)f_stk_74 - fret_07) - ((float10)f_stk_70 - (float10)(float)xStack_1d4));
            iVar7 = 0
            repeat
                iVar6 = iVar7
                if *pfVar13 < xStack_1d4 ~= (*pfVar13 == xStack_1d4) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                iVar7 = iVar6 + 1
                pfVar13 = pfVar13 + 1
            until not (iVar7 < 7)
            quest:ResetPlayerCreatureOnlyTarget()
            quest:SetStateInt("TutorialState", 7)
            quest:DisplayQuestInfo(false)
            xStack_1d0 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            iVar20 = 4
            pCVar16 = xStack_1d0
            pCVar5 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar20)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a997
                iVar20 = 4
                pCVar16 = xStack_1d0
                pCVar5 = quest:GetHero()
                bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar20)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a997: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_1d0;
                -- TODO(native): goto LAB_00d5a99e
            end
            xStack_1c0 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_1c0, r1, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a98b
                bVar3 = resources:TryAcquire(xStack_84, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a98b: (native jump target)
                resources:ReleaseResource(xStack_84)
                -- TODO(native): goto LAB_00d5a997
            end
            quest:RemoveQuestInfoElement(xStack_23c)
            xStack_220 = resources:NewActorMap()
            resources:SetActor(xStack_220, "HERO", xStack_1d0)
            resources:SetActor(xStack_220, "TEACHER", xStack_214)
            resources:SetActor(xStack_220, "THUNDER", xStack_1d0)
            resources:SetActor(xStack_220, "WHISPER", xStack_84)
            -- TODO(native): Std_Deque_Construct(xStack_1e0);
            native_arg_switch_2 = iVar6
            repeat
                if native_arg_switch_2 == 0 then
                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS"
                    -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_158);
                    break
                else
                    if native_arg_switch_2 == 1 then
                        pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A"
                        -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_118);
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B"
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_150);
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C"
                                -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_1a4);
                                break
                            else
                                if native_arg_switch_2 == 4 then
                                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D"
                                    -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_148);
                                    break
                                else
                                    if native_arg_switch_2 == 5 then
                                        pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E"
                                        -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_110);
                                        break
                                    else
                                        if native_arg_switch_2 == 6 then
                                            pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F"
                                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1e0,&xStack_140);
                                            break
                                        else
                                            goto FLOW_native_label_1
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            until not (false)
            ::FLOW_native_label_1::
            xStack_c0 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): xStack_23c = piVar12;
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if not __native_entity_state:GetStateBool("WhisperStanding") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    -- TODO(native): RunCutsceneMacro_Func(&xStack_138,xStack_220,(void *)0x0,xStack_1e0,false,true);
                    goto LAB_00d5a28a
                end
                -- LAB_00d5a948: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5a96a: (native jump target)
                resources:ReleaseResource(xStack_d0)
                -- TODO(native): LTextTreeWalkThrough__Dtor(xStack_1e0);
                resources:DestroyActorMap(xStack_220)
                -- TODO(native): goto LAB_00d5a98b
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a956: (native jump target)
                -- LAB_00d5a962: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5a96a
            end
            -- TODO(native): RunCutsceneMacro_Func(&xStack_108,xStack_220,(void *)0x0,xStack_1e0,false,true);
            ::LAB_00d5a28a::
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:EntitySetInFaction(pCVar5, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(1)
            bVar18 = true
            bVar3 = false
            pCVar5 = quest:GetThingWithScriptName("MeleeThunder")
            quest:RemoveThing(pCVar5, bVar3, bVar18)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar7 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a956
                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a948
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if iVar7 == 1 then
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a956
                resources:RunMacro("CS_GUILD_MELEE_CONTINUE", xStack_220, false, true)
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a948
                resources:RunMacro("CS_GUILD_MELEE_REPEAT", xStack_220, false, false)
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                pCVar5 = quest:GetHero()
                bVar3 = quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", pCVar5)
                piVar12 = xStack_23c
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        iVar7 = *xStack_23c
                        piVar12 = xStack_23c
                        -- TODO(native): goto LAB_00d5a962
                    end
                    quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
                    piVar12 = xStack_23c
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(xStack_a4)
            -- TODO(native): LTextTreeWalkThrough__Dtor(xStack_1e0);
            resources:DestroyActorMap(xStack_220)
            resources:DestroyMovie(xStack_c0)
            resources:ReleaseResource(xStack_1c0)
            cVar4 = quest:GetStateBool("MeleeOpponentReset")
            while cVar4 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                cVar4 = quest:GetStateBool("MeleeOpponentReset")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
            resources:ReleaseResource(xStack_1d0)
            cVar4 = quest:GetStateBool("MeleeRepeating")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            bVar18 = true
            bVar3 = false
            pCVar10 = quest:GetThingWithScriptName("MeleeOpponent")
            quest:RemoveThing(pCVar10, bVar3, bVar18)
            pCVar10 = quest:GetThingWithScriptName("M_GuildmasterMarker")
            p4 = 1
            iVar17 = 0
            iVar20 = 0
            iVar7 = 0x40400000
            pCVar9 = pCVar10:GetPos()
            me:MoveToPosition(pCVar9, iVar7, iVar20, (iVar17 ~= 0), (p4 ~= 0))
            pCVar10 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
            bVar3 = false
            pCVar9 = pCVar10:GetPos()
            r7 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pCVar9, "MeleeApprentice")
            pCVar10 = quest:GetThingWithScriptName("CombatApprenticeMarker")
            bVar3 = false
            pCVar9 = pCVar10:GetPos()
            r8 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar9, "CombatApprentice")
            delay = 0
            pCVar11 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar11, delay)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "")
        end
        -- LAB_00d5a8cb: (native jump target)
        quest:DeregisterTimer(iVar6)
    end
    ::FLOW_after_lab_00d5a8cb::
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    resources:ReleaseResource(xStack_214)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(iVar6)
    goto LAB_00d5a8d9
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WhisperEarly", false)
    __native_entity_state:SetStateBool("WhisperLate", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

