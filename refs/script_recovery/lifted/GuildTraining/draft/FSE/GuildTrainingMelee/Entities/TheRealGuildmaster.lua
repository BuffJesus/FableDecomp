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
    local __native_condition_1, __native_condition_2, bVar18, bVar3, cVar4, delay, fVar19, fVar2, f_stk_70, f_stk_74, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, iVar17, iVar20, iVar6, iVar7, ixVar13, native_arg_switch_2, p4, pCVar10, pCVar11, pCVar16, pCVar5, pCVar9, pcVar14, pppuVar15, r1, r2, r3, r4, r5, r6, xStack_1c0, xStack_1d0, xStack_1e0, xStack_204, xStack_214, xStack_220, xStack_23c, xStack_250, xStack_260, xStack_264, xStack_28, xStack_38, xStack_48, xStack_54, xStack_6c, xStack_84, xStack_94, xStack_a4, xStack_b0, xStack_c0, xStack_d0, xStack_e0, x_stk_1ec
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
            resources:ReleaseResource(xStack_250)
            return
        end
        bVar3 = resources:TryAcquire(xStack_250, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    bVar3 = true
    pCVar5 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, pCVar5, bVar3)
    quest:SetIsPushableByHero(me, false)
    r1 = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    iVar6 = quest:RegisterTimer()
    xStack_264 = iVar6
    quest:SetTimer(iVar6, 0)
    iVar7 = quest:GetStateInt("TutorialState")
    x_stk_1ec = 0x0
    while iVar7 == 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
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
                if bVar3 then
                    quest:DeregisterTimer(iVar6)
                    goto FLOW_after_lab_00d5a8cb
                end
                xStack_204 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                xStack_220 = resources:ScriptThing(xStack_250)
                pCVar5 = xStack_220
                fret_0 = quest:GetHealth(pCVar5)
                fVar2 = 0.0
                xStack_220 = nil
                if fVar2 < fret_0 then
                    iVar17 = 0
                    iVar20 = 1
                    iVar6 = 0
                    iVar7 = 0
                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER"
                    pCVar5 = quest:GetHero()
                    r2 = me:Speak(pCVar5, pcVar14, iVar7, (iVar6 ~= 0), (iVar20 ~= 0), (iVar17 ~= 0))
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
                        resources:DestroyMovie(xStack_204)
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
            iVar7 = quest:GetTimer(xStack_264)
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
            quest:SetTimer(xStack_264, 10)
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
        quest:DeregisterTimer(xStack_260)
    else
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(r1)
        cVar4 = quest:GetStateBool("MeleeRepeating")
        while cVar4 do
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
                    resources:ReleaseResource(xStack_204)
                    quest:DeregisterTimer(xStack_264)
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
                resources:ReleaseResource(xStack_204)
                quest:DeregisterTimer(xStack_264)
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
                    resources:ReleaseResource(xStack_94)
                    resources:ReleaseResource(xStack_204)
                    quest:DeregisterTimer(xStack_264)
                    goto LAB_00d5a8d9
                end
                bVar3 = resources:TryAcquire(xStack_94, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a8f6: (native jump target)
                resources:ReleaseResource(xStack_94)
                resources:ReleaseResource(xStack_204)
                quest:DeregisterTimer(xStack_264)
                goto LAB_00d5a8d9
            end
            xStack_54 = resources:NewActorMap()
            resources:SetActor(xStack_54, "HERO", xStack_204)
            resources:SetActor(xStack_54, "TEACHER", xStack_250)
            resources:SetActor(xStack_54, "WHISPER", xStack_94)
            xStack_38 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_ATTACK", xStack_54, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_38)
            resources:DestroyActorMap(xStack_54)
            resources:ReleaseResource(xStack_94)
            resources:ReleaseResource(xStack_204)
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(true)
            xStack_23c = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0)
            quest:SetStateInt("TutorialState", 3)
            xStack_260 = quest:RegisterTimer()
            quest:SetTimer(xStack_260, 0xf)
            iVar7 = quest:GetStateInt("GenericTutorialCounter")
            while iVar7 < 7 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                iVar7 = quest:GetTimer(xStack_260)
                if iVar7 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_HELP_ATTACK", me, pCVar5, false)
                    quest:SetTimer(xStack_260, 0xf)
                end
                quest:UpdateQuestInfoCounter(xStack_23c, quest:GetStateInt("GenericTutorialCounter"), -1)
                pCVar5 = quest:GetHero()
                fret_00 = quest:GetHealth(pCVar5)
                if fret_00 < quest:ReadGlobalGameDataFloat(0xed8) then
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
                resources:DestroyMovie(xStack_d0)
                -- LAB_00d5a9b5: (native jump target)
                quest:DeregisterTimer(xStack_260)
                quest:DeregisterTimer(xStack_260)
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
                resources:DestroyMovie(xStack_84)
                -- TODO(native): goto LAB_00d5a922
            end
            xStack_6c = resources:NewActorMap()
            resources:SetActor(xStack_6c, "HERO", xStack_d0)
            resources:SetActor(xStack_6c, "TEACHER", xStack_250)
            resources:SetActor(xStack_6c, "WHISPER", xStack_84)
            xStack_28 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", xStack_6c, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(xStack_28)
            resources:DestroyActorMap(xStack_6c)
            resources:ReleaseResource(xStack_84)
            resources:DestroyMovie(xStack_d0)
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
                if fret_01 < quest:ReadGlobalGameDataFloat(0xed8) then
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
            r3 = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", pCVar9, "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(r3, 1)
            xStack_214 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            bVar3 = resources:TryAcquire(xStack_214, r3, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                bVar3 = resources:TryAcquire(xStack_214, r3, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a9a7: (native jump target)
                resources:ReleaseResource(xStack_214)
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
                bVar3 = resources:TryAcquire(xStack_a4, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a933: (native jump target)
                resources:ReleaseResource(xStack_a4)
                -- TODO(native): goto LAB_00d5a93f
            end
            xStack_b0 = resources:NewActorMap()
            resources:SetActor(xStack_b0, "HERO", xStack_e0)
            resources:SetActor(xStack_b0, "TEACHER", xStack_250)
            resources:SetActor(xStack_b0, "THUNDER", xStack_214)
            resources:SetActor(xStack_b0, "WHISPER", xStack_a4)
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
            resources:ReleaseResource(xStack_48)
            resources:DestroyActorMap(xStack_b0)
            resources:ReleaseResource(xStack_a4)
            resources:ReleaseResource(xStack_e0)
            __native_entity_state:SetStateBool("HeroStanding", true)
            __native_entity_state:SetStateBool("WhisperStanding", true)
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(xStack_23c)
            quest:DisplayQuestInfo(true)
            fVar19 = 1.0
            pCVar5 = quest:GetThingWithScriptName("MeleeOpponent")
            r4 = quest:AddQuestInfoBarHealth(pCVar5, pCVar11, "HUD_WHISPER_ICON", fVar19)
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
                if quest:ReadGlobalGameDataFloat(0xed8) <= fret_04 then
                    pCVar5 = quest:GetThingWithScriptName("MeleeOpponent")
                    fret_05 = quest:GetHealth(pCVar5)
                    fVar19 = quest:ReadGlobalGameDataFloat(0xed8)
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
            -- TODO(native): CStack_1d4 = (CCharString)(float)fret_06;
            fret_07 = quest:GetHealth(nil --[[missing]])
            ixVar13 = 0
            -- TODO(native): CStack_1d4 = (CCharString) (float)(((float10)f_stk_74 - fret_07) - ((float10)f_stk_70 - (float10)(float)CStack_1d4));
            iVar7 = 0
            repeat
                iVar6 = iVar7
                if quest:ReadGlobalGameDataFloatAt(0xeb4, ixVar13) < CStack_1d4 ~= (quest:ReadGlobalGameDataFloatAt(0xeb4, ixVar13) == CStack_1d4) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                iVar7 = iVar6 + 1
                ixVar13 = ixVar13 + 1
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
                bVar3 = resources:TryAcquire(xStack_1c0, r1, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d5a98b: (native jump target)
                resources:ReleaseResource(xStack_1c0)
                -- TODO(native): goto LAB_00d5a997
            end
            quest:RemoveQuestInfoElement(xStack_23c)
            xStack_220 = resources:NewActorMap()
            resources:SetActor(xStack_220, "HERO", xStack_1d0)
            resources:SetActor(xStack_220, "TEACHER", xStack_250)
            resources:SetActor(xStack_220, "THUNDER", xStack_214)
            resources:SetActor(xStack_220, "WHISPER", xStack_1c0)
            xStack_1e0 = resources:NewStringMap()
            native_arg_switch_2 = iVar6
            repeat
                if native_arg_switch_2 == 0 then
                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS"
                    resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                    break
                else
                    if native_arg_switch_2 == 1 then
                        pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A"
                        resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B"
                            resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C"
                                resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                                break
                            else
                                if native_arg_switch_2 == 4 then
                                    pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D"
                                    resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                                    break
                                else
                                    if native_arg_switch_2 == 5 then
                                        pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E"
                                        resources:SetString(xStack_1e0, "$GRADE", pcVar14)
                                        break
                                    else
                                        if native_arg_switch_2 == 6 then
                                            pcVar14 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F"
                                            resources:SetString(xStack_1e0, "$GRADE", pcVar14)
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
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if not __native_entity_state:GetStateBool("WhisperStanding") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_WON", xStack_220, xStack_1e0, false, true)
                    goto LAB_00d5a28a
                end
                -- LAB_00d5a948: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5a96a: (native jump target)
                resources:ReleaseResource(xStack_c0)
                resources:DestroyStringMap(xStack_1e0)
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
            resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_LOST", xStack_220, xStack_1e0, false, true)
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
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- TODO(native): iVar7 = *xStack_23c
                        iVar7 = nil --[[unresolved native value]]
                        -- TODO(native): goto LAB_00d5a962
                    end
                    quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(xStack_c0)
            resources:DestroyStringMap(xStack_1e0)
            resources:DestroyActorMap(xStack_220)
            resources:DestroyMovie(xStack_1c0)
            resources:ReleaseResource(xStack_1d0)
            cVar4 = quest:GetStateBool("MeleeOpponentReset")
            while not cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
                cVar4 = quest:GetStateBool("MeleeOpponentReset")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then return end  -- TODO(native): goto LAB_00d5a9a7
            resources:ReleaseResource(xStack_214)
            quest:DeregisterTimer(xStack_264)
            iVar6 = xStack_264
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
            r5 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pCVar9, "MeleeApprentice")
            pCVar10 = quest:GetThingWithScriptName("CombatApprenticeMarker")
            bVar3 = false
            pCVar9 = pCVar10:GetPos()
            r6 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar9, "CombatApprentice")
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
    resources:ReleaseResource(xStack_250)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(xStack_264)
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

