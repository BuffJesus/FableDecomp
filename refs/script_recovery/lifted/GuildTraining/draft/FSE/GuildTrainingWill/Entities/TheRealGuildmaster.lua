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
    local CVar5, __native_condition_1, __native_condition_2, __native_condition_3, bVar6, bVar8, cVar7, c_stk_22d, delay, dist, fVar4, fret_00, fret_01, iVar10, iVar11, iVar22, i_stk_24, native_arg_switch_5, pCVar12, pCVar13, pCVar15, pCVar23, pCVar9, pcVar17, pfVar16, pppuVar24, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r3, r4, r5, r6, r7, r8, r9, thing, thing_b10, thing_b11, thing_b8, thing_b9, uVar18, uVar19, uVar2, uVar20, uVar21, u_stk_17c, xStack_10, xStack_158, xStack_1d0, xStack_1d4, xStack_1e0, xStack_1e0_2, xStack_1f0, xStack_20, xStack_200, xStack_210, xStack_228, xStack_38, xStack_48, xStack_54, xStack_60_2
    local alive = true
    local function __region_LAB_00d61ad8_c22()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(xStack_158)
    end
    local function __cleanup_LAB_00d61b0a()
        quest:DeregisterTimer(iVar11)
        quest:DeregisterTimer(iVar11)
        resources:ReleaseResource(xStack_158)
    end
    local function __cleanup_LAB_00d61b0a_c22()
        quest:DeregisterTimer(iVar11)
        quest:DeregisterTimer(iVar11)
        resources:ReleaseResource(xStack_158)
    end
    u_stk_17c = 0
    xStack_228 = resources:NewResource()
    bVar6 = false
    if bVar6 ~= 0 then
    end
    bVar6 = resources:TryAcquire(xStack_228, me, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            if false then
                -- TODO(native): (**(code **)((int)xStack_cc + 4))();
            end
            return
        end
        bVar6 = resources:TryAcquire(0, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(me, false, true)
    bVar6 = true
    pCVar9 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, pCVar9, bVar6)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    uVar2 = __native_entity_state:GetStateInt("self_0xc")
    -- TODO(native): thing._4_4_ = uVar2;
    thing = nil
    thing_b8 = piVar1
    thing_b9 = (piVar1 >> 8)
    thing_b10 = (piVar1 >> 0x10)
    thing_b11 = (piVar1 >> 0x18)
    quest:SetIsPushableByHero(nil --[[missing]], (thing ~= 0))
    iVar10 = quest:RegisterTimer()
    iVar11 = quest:RegisterTimer()
    quest:SetTimer(iVar11, 0)
    quest:EntitySetTargetingType(me, 0x1a)
    if not quest:GetStateBool("TestFinished") then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            quest:DeregisterTimer(iVar11)
            quest:DeregisterTimer(iVar11)
            resources:ReleaseResource(0)
            return
        end
        pCVar12 = quest:GetThingWithScriptName("M_WillTeacherStand")
        iVar22 = 1
        uVar18 = 0
        uVar19 = 0
        uVar20 = 0
        uVar21 = 0
        iVar11 = 0
        iVar10 = 0x40400000
        pCVar13 = pCVar12:GetPos()
        me:MoveToPosition(pCVar13, iVar10, iVar11, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
        iVar10 = quest:GetStateInt("TutorialState")
        while iVar10 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d61b69 end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61b69 end
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                    quest:SetStateInt("TutorialState", 2)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                    xStack_1f0 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    pCVar12 = resources:ScriptThing(0)
                    pCVar12 = pCVar12
                    r1 = quest:GetHealth(pCVar12)
                    fVar4 = 0.0
                    if fVar4 < fret_0 then
                        iVar22 = 0
                        uVar18 = 1
                        uVar19 = 0
                        uVar20 = 0
                        uVar21 = 0
                        iVar11 = 0
                        iVar10 = 0
                        pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        pCVar12 = quest:GetHero()
                        r2 = me:Speak(pCVar12, pcVar17, iVar10, (iVar11 ~= 0), CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                        iVar10 = me:IsPerformingScriptTask()
                        cVar7 = iVar10
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                __cleanup_LAB_00d61b0a(); return
                            end
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_1f0)
                            goto LAB_00d61b69
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1f0)
                end
            end
            dist = 5.5
            pCVar9 = quest:GetHero()
            bVar6 = quest:IsDistanceBetweenThingsUnder(pCVar9, me, dist)
            __native_condition_2 = bVar6
            if __native_condition_2 then
                iVar10 = quest:GetTimer(iVar11)
                __native_condition_2 = iVar10 < 1
            end
            __native_condition_1 = __native_condition_2
            if __native_condition_1 then
                iVar10 = me:IsPerformingScriptTask()
                __native_condition_1 = not iVar10
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61b69 end
                iVar11 = quest:AddNewConversation(me, false, false)
                pCVar9 = quest:GetHero()
                quest:AddPersonToConversation(iVar11, pCVar9)
                quest:SetTimer(iVar11, xStack_244)
                if 0 == 0 then
                    bVar6 = false
                    pCVar9 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar9, bVar6)
                    pCVar9 = quest:GetHero()
                    quest:AddLineToConversation(iVar11, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, pCVar9, false)
                    -- LAB_00d5e63f: (native jump target)
                else
                    if 0 == 1 then
                        bVar6 = false
                        pCVar9 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar9, bVar6)
                        pCVar9 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, pCVar9, false)
                        goto FLOW_after_lab_00d5e63f
                    end
                end
                ::FLOW_after_lab_00d5e63f::
                -- TODO(native): xStack_22c = 1 - xStack_22c;
            end
            iVar10 = quest:GetStateInt("TutorialState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                xStack_38 = resources:NewResource()
                bVar6 = false
                if bVar6 ~= 0 then
                end
                iVar11 = 4
                uVar18 = SUB41(xStack_38,0)
                uVar19 = (xStack_38 >> 8)
                uVar20 = (xStack_38 >> 0x10)
                uVar21 = (xStack_38 >> 0x18)
                pCVar12 = quest:GetHero()
                bVar6 = me:AcquireControl(4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        resources:ReleaseResource(xStack_38)
                        goto LAB_00d61b69
                    end
                    iVar11 = 4
                    uVar18 = SUB41(xStack_38,0)
                    uVar19 = (xStack_38 >> 8)
                    uVar20 = (xStack_38 >> 0x10)
                    uVar21 = (xStack_38 >> 0x18)
                    pCVar12 = quest:GetHero()
                    bVar6 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00d60aee: (native jump target)
                    resources:ReleaseResource(xStack_38)
                    goto LAB_00d61b69
                end
                xStack_54 = resources:NewActorMap()
                resources:SetActor(xStack_54, "HERO", xStack_38)
                resources:SetActor(xStack_54, "TEACHER", 0)
                quest:GiveHeroAbility(0xb, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                xStack_20 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_LIGHTNING", xStack_54, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1f0)
                resources:DestroyActorMap(xStack_54)
                resources:DestroyMovie(xStack_10)
                bVar6 = quest:IsXbox()
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61b69 end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61b69 end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61b69 end
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_134);
                    pCVar15 = "TEXT_QST_LOG_COMBAT_USINGSPELLS"
                end
                quest:SetMasterGameState("WillScore", 0)
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_23c);
                quest:SetTimer(iVar11, 0)
                iVar10 = quest:GetStateInt("TutorialState")
                c_stk_22d = 0
                while iVar10 == 2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    CVar5 = ""
                    if bVar6 then goto LAB_00d6138b end
                    while (c_stk_22d == 0 and (quest:GetMasterGameState("WillScore") == 0)) do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        cVar7 = me:IsTalkedToByHero()
                        if cVar7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            c_stk_22d = '\x01'
                        end
                        iVar10 = quest:GetTimer(iVar11)
                        if iVar10 < 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            bVar6 = false
                            pCVar12 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar12, bVar6)
                            quest:SetTimer(iVar11, 2)
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        quest:SetStateInt("TutorialState", 3)
                        xStack_48 = resources:NewResource()
                        bVar6 = false
                        if bVar6 ~= 0 then
                        end
                        iVar11 = 4
                        uVar18 = SUB41(xStack_48,0)
                        uVar19 = (xStack_48 >> 8)
                        uVar20 = (xStack_48 >> 0x10)
                        uVar21 = (xStack_48 >> 0x18)
                        pCVar12 = quest:GetHero()
                        bVar6 = me:AcquireControl(4)
                        while not bVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                resources:ReleaseResource(xStack_48)
                                goto LAB_00d61b69
                            end
                            iVar11 = 4
                            uVar18 = SUB41(xStack_48,0)
                            uVar19 = (xStack_48 >> 8)
                            uVar20 = (xStack_48 >> 0x10)
                            uVar21 = (xStack_48 >> 0x18)
                            pCVar12 = quest:GetHero()
                            bVar6 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00d60aff: (native jump target)
                            resources:ReleaseResource(xStack_48)
                            goto LAB_00d61b69
                        end
                        pCVar12 = resources:NewActorMap()
                        pCVar23 = xStack_48
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1b4,xStack_124);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar14,pCVar23);
                        pppuVar24 = 0
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1b4,xStack_11c);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar14,pppuVar24);
                        xStack_10 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", pCVar12, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:ReleaseResource(xStack_158)
                        resources:DestroyActorMap(pCVar12)
                        resources:ReleaseResource(xStack_1f0)
                    end
                    if c_stk_22d ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        c_stk_22d = bVar6
                        bVar6 = quest:IsXbox()
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d6138b end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d6138b end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                    end
                    iVar10 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d6138b end
                bVar6 = quest:IsXbox()
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6138b end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6138b end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d6138b end
                bVar6 = quest:MsgOnHeroCastSpell()
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6138b end
                    cVar7 = me:IsTalkedToByHero()
                    if cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                        bVar6 = quest:IsXbox()
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d6138b end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d6138b end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6138b end
                    end
                    bVar6 = quest:MsgOnHeroCastSpell()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d6138b end
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_234);
                -- TODO(native): piVar1 = DAT_0143e8f8;
                iVar11 = __ftol2()
                quest:SetTimer(iVar11, xStack_234)
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_22c);
                quest:SetTimer(iVar11, xStack_22c)
                iVar10 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                i_stk_24 = iVar10
                xStack_1d4 = quest:AddQuestInfoTimer(iVar11, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                c_stk_22d = 0
                quest:SetTimer(iVar11, 0)
                iVar22 = quest:GetTimer(iVar11)
                while (0 < iVar22 and (c_stk_22d == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61379 end
                    iVar22 = quest:GetTimer(iVar11)
                    if iVar22 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61379 end
                        bVar6 = false
                        pCVar12 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar12, bVar6)
                        quest:SetTimer(iVar11, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61379 end
                        c_stk_22d = '\x01'
                    end
                    iVar22 = quest:GetHeroWillEnergy()
                    __native_condition_3 = iVar22 == 0
                    if __native_condition_3 then
                        iVar22 = quest:GetTimer(iVar11)
                        __native_condition_3 = iVar22 < 1
                    end
                    if __native_condition_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61379 end
                        iVar11 = quest:AddNewConversation(me, false, false)
                        pCVar12 = quest:GetHero()
                        quest:AddPersonToConversation(iVar11, pCVar12)
                        pCVar12 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, pCVar12, false)
                        quest:SetTimer(iVar11, xStack_22c)
                        iVar10 = i_stk_24
                    end
                    cVar7 = me:IsTalkedToByHero()
                    if cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61379 end
                        bVar6 = quest:IsXbox()
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d61379 end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d61379 end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d61379 end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00d61379 end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61379 end
                    end
                    quest:UpdateQuestInfoCounter(iVar10, quest:GetMasterGameState("WillScore"), -1)
                    iVar22 = quest:GetTimer(iVar11)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                bVar6 = quest:IsHeroControlledByPlayer()
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61379 end
                    bVar6 = quest:IsHeroControlledByPlayer()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61379 end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(iVar10)
                quest:RemoveQuestInfoElement(xStack_1d4)
                quest:SetStateInt("TutorialState", 0)
                xStack_200 = resources:NewResource()
                bVar6 = false
                if bVar6 ~= 0 then
                end
                iVar11 = 4
                uVar18 = 0
                uVar19 = (xStack_200 >> 8)
                uVar20 = (xStack_200 >> 0x10)
                uVar21 = (xStack_200 >> 0x18)
                pCVar12 = quest:GetHero()
                bVar6 = me:AcquireControl(4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d61370 end
                    iVar11 = 4
                    uVar18 = 0
                    uVar19 = (xStack_200 >> 8)
                    uVar20 = (xStack_200 >> 0x10)
                    uVar21 = (xStack_200 >> 0x18)
                    pCVar12 = quest:GetHero()
                    bVar6 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d61370 end
                xStack_210 = resources:NewActorMap()
                xStack_1d0 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(true)
                if c_stk_22d == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6135b end
                    xStack_1d4 = quest:GetMasterGameState("WillScore")
                    -- TODO(native): pfVar16 = *(float **)(DAT_0143e90c + 0xecc);
                    iVar10 = 0
                    repeat
                        iVar11 = iVar10
                        if *pfVar16 < xStack_1d4 ~= (*pfVar16 == xStack_1d4) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d6135b end
                            break
                        end
                        iVar10 = iVar11 + 1
                        pfVar16 = pfVar16 + 1
                    until not (iVar10 < 7)
                    -- TODO(native): Std_Deque_Construct(xStack_1c0);
                    native_arg_switch_5 = iVar11
                    repeat
                        if native_arg_switch_5 == 0 then
                            pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_c0);
                            break
                        else
                            if native_arg_switch_5 == 1 then
                                pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                                -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_138);
                                break
                            else
                                if native_arg_switch_5 == 2 then
                                    pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                                    -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_8c);
                                    break
                                else
                                    if native_arg_switch_5 == 3 then
                                        pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                                        -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_68);
                                        break
                                    else
                                        if native_arg_switch_5 == 4 then
                                            pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                                            -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_128);
                                            break
                                        else
                                            if native_arg_switch_5 == 5 then
                                                pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                                                -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_b8);
                                                break
                                            else
                                                if native_arg_switch_5 == 6 then
                                                    pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                                                    -- TODO(native): pCVar15 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](xStack_1c0,&xStack_130);
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
                    pCVar23 = xStack_200
                    -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_210,xStack_120);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar14,pCVar23);
                    resources:SetActor(xStack_210, "TEACHER", xStack_158)
                    resources:RunMacro("$GRADE", xStack_210, false, false)
                    pCVar12 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar12 ~= 0))
                    -- TODO(native): RunCutsceneMacro_Func(xStack_110,xStack_210,(void *)0x0,xStack_1d0,false,false);
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d61578
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if iVar10 == 1 then
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        r3 = quest:GetThingWithScriptName("HERO")
                        xStack_158 = resources:NewResource()
                        bVar6 = false
                        if bVar6 ~= 0 then
                        end
                        bVar6 = resources:TryAcquire(xStack_158, r3, 4)
                        while not bVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                resources:ReleaseResource(xStack_200)
                                -- LAB_00d61342_c8: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            bVar6 = resources:TryAcquire(xStack_158, r3, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            resources:DestroyMovie(xStack_1d0)
                            -- LAB_00d61578_c9: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        uVar18 = SUB41(&xStack_238_3,0)
                        uVar19 = (&xStack_238_3 >> 8)
                        uVar20 = (&xStack_238_3 >> 0x10)
                        uVar21 = (&xStack_238_3 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,&xStack_a0);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        uVar18 = SUB41(0,0)
                        uVar19 = (0 >> 8)
                        uVar20 = (0 >> 0x10)
                        uVar21 = (0 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,&xStack_f8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        uVar18 = SUB41(xStack_158,0)
                        uVar19 = (xStack_158 >> 8)
                        uVar20 = (xStack_158 >> 0x10)
                        uVar21 = (xStack_158 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,(CCharString *)xStack_1d8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        resources:RunMacro("TEACHER", xStack_210, false, true)
                        bVar6 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                resources:ReleaseResource(xStack_200)
                                -- LAB_00d61342_c10: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then
                            resources:DestroyMovie(xStack_1d0)
                            -- LAB_00d61578_c11: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if iVar10 == 1 then
                            if bVar8 then
                                resources:DestroyMovie(xStack_1d0)
                                -- LAB_00d61578_c12: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            xStack_60_2 = resources:ScriptThing(0)
                            pCVar9 = xStack_60_2
                            r4 = quest:GetHealth(pCVar9)
                            fVar4 = 0.0
                            if fret_03 <= fVar4 then return end  -- TODO(native): goto LAB_00d610d3
                            r5 = me:Speak(me, "CS_GUILD_WILL_CONTINUE", 0x12d1148, false, false, true)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            goto FLOW_native_label_3
                        end
                        if bVar8 then
                            resources:ReleaseResource(xStack_200)
                            -- LAB_00d61342_c13: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        r3 = resources:ScriptThing(0)
                        pCVar9 = r3
                        fret_01 = quest:GetHealth(pCVar9)
                        -- TODO(native): xStack_234._3_1_ = 1;
                        if fret_01 <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        if xStack_234_b3 ~= 0 then
                            r6 = me:Speak(me, "WHISPER", 0x12d1368, false, false, true)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    resources:DestroyMovie(xStack_1d0)
                                    -- LAB_00d61578_c14: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d61578
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                resources:ReleaseResource(xStack_200)
                                -- LAB_00d61342_c15: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                        end
                        bVar8 = false
                        if bVar8 ~= 0 then
                        end
                        resources:DestroyMovie(xStack_1d0)
                    else
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        quest:SetHeroWillEnergyLevel(1.0)
                        uVar18 = SUB41(0,0)
                        uVar19 = (0 >> 8)
                        uVar20 = (0 >> 0x10)
                        uVar21 = (0 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,&xStack_cc);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        resources:RunMacro(xStack_d0, xStack_210, false, false)
                        bVar6 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): LTextTreeWalkThrough__Dtor(xStack_1c0);
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6135b end
                    pCVar12 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar12 ~= 0))
                    uVar18 = SUB41(&xStack_238_3,0)
                    uVar19 = (&xStack_238_3 >> 8)
                    uVar20 = (&xStack_238_3 >> 0x10)
                    uVar21 = (&xStack_238_3 >> 0x18)
                    -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,xStack_d4);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                    uVar18 = SUB41(0,0)
                    uVar19 = (0 >> 8)
                    uVar20 = (0 >> 0x10)
                    uVar21 = (0 >> 0x18)
                    -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,xStack_d0);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                    resources:RunMacro("CS_GUILD_WILL_DISQUALIFIED", xStack_210, false, true)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d6101c end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d6101c end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if iVar10 == 1 then
                        if bVar6 then goto LAB_00d6101c end
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        r7 = quest:GetThingWithScriptName("MeleeApprentice")
                        xStack_1f0 = resources:NewResource()
                        bVar6 = false
                        if bVar6 ~= 0 then
                        end
                        bVar6 = resources:TryAcquire(xStack_1f0, r7, 4)
                        while not bVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                resources:ReleaseResource(xStack_1f0)
                                -- LAB_00d60b31_c17: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            bVar6 = resources:TryAcquire(xStack_1f0, r7, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d61004 end
                        uVar18 = SUB41(&xStack_238_3,0)
                        uVar19 = (&xStack_238_3 >> 8)
                        uVar20 = (&xStack_238_3 >> 0x10)
                        uVar21 = (&xStack_238_3 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,xStack_188);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        uVar18 = SUB41(0,0)
                        uVar19 = (0 >> 8)
                        uVar20 = (0 >> 0x10)
                        uVar21 = (0 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,xStack_180);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        uVar18 = SUB41(xStack_1f0,0)
                        uVar19 = (xStack_1f0 >> 8)
                        uVar20 = (xStack_1f0 >> 0x10)
                        uVar21 = (xStack_1f0 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,&xStack_19c);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", xStack_210, false, true)
                        bVar6 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                resources:ReleaseResource(xStack_1f0)
                                -- LAB_00d60b31_c18: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then goto LAB_00d61004 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if iVar10 == 1 then
                            if bVar8 then goto LAB_00d61004 end
                            xStack_1e0_2 = resources:ScriptThing(0)
                            pCVar9 = xStack_1e0_2
                            r8 = quest:GetHealth(pCVar9)
                            fVar4 = 0.0
                            if fret_02 <= fVar4 then
                                quest:FadeScreenOut(0.5, 0.5)
                                quest:Pause(1.0)
                                quest:PlayAVIMovie("WHISPER")
                                quest:ConfiscateAllHeroWeapons()
                                bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                                if bVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00d61004 end
                                    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                end
                                quest:SetTimeOfDay(11.0)
                                quest:ResetPlayerCreatureCombatMultiplier()
                                quest:SetHeroAsTeenager(false)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                bVar8 = true
                                bVar6 = false
                                pCVar9 = quest:GetThingWithScriptName("MeleeApprentice")
                                quest:RemoveThing(pCVar9, bVar6, bVar8)
                                pCVar9 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
                                bVar6 = false
                                pCVar13 = pCVar9:GetPos()
                                r9 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", pCVar13, "MeleeApprentice")
                                pCVar9 = quest:GetThingWithScriptName("WillApprentice")
                                bVar6 = (pCVar9 ~= nil and pCVar9:IsAlive())
                                if not bVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        pCVar9 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
                                        bVar6 = false
                                        pCVar13 = pCVar9:GetPos()
                                        r10 = quest:CreateCreature("MeleeApprentice", pCVar13, "WillApprentice")
                                        if xStack_60._0_4_ ~= nil then
                                            -- TODO(native): (**(code **)(*xStack_60._0_4_ + 0x118))(0);
                                        end
                                        goto LAB_00d60ef5_c19
                                    end
                                    -- LAB_00d60b19_c19: (native jump target)
                                    resources:ReleaseResource(xStack_1f0)
                                    -- LAB_00d60b31_c19: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00d6135b
                                end
                                ::LAB_00d60ef5_c19::
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
                                -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238_12,(CCharString *)pCVar12);
                                quest:SetPlayerUsingWillDummies(false)
                                quest:SetMasterGameState("HeroTakingGuildTest", false)
                                quest:SetStateInt("TutorialState", 4)
                                quest:SetStateBool("TestFinished", true)
                                uVar18 = 0
                                uVar19 = 0
                                uVar20 = 0
                                uVar21 = 0
                                pCVar15 = quest:GetActiveQuestName()
                                quest:DeactivateQuestLater(pCVar15, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))))
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                until not (not bVar6)
                                goto FLOW_after_lab_00d60be3
                            end
                            r11 = me:Speak(me, "TEXT_QST_LOG_HERO_ATTRIBUTES", 0x12d1148, false, false, true)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            goto FLOW_native_label_4
                        end
                        if bVar8 then
                            resources:ReleaseResource(xStack_1f0)
                            -- LAB_00d60b31_c20: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d6135b
                        end
                        xStack_1e0 = resources:ScriptThing(0)
                        pCVar9 = xStack_1e0
                        fret_00 = quest:GetHealth(pCVar9)
                        CVar5 = ""
                        -- TODO(native): xStack_23c._3_1_ = 1;
                        if fret_00 <= 0.0 then
                            -- TODO(native): xStack_23c = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        if xStack_23c_b3 ~= 0 then
                            r12 = me:Speak(me, "WillApprenticeMarker", 0x12d1368, false, false, true)
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then goto LAB_00d61004 end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                resources:ReleaseResource(xStack_1f0)
                                -- LAB_00d60b31_c21: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                        end
                        bVar8 = false
                        if bVar8 ~= 0 then
                        end
                        resources:ReleaseResource(xStack_200)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        if bVar6 then goto LAB_00d6101c end
                        quest:SetHeroWillEnergyLevel(1.0)
                        uVar18 = SUB41(0,0)
                        uVar19 = (0 >> 8)
                        uVar20 = (0 >> 0x10)
                        uVar21 = (0 >> 0x18)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,xStack_218);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,(void *)CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))));
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", xStack_210, false, false)
                        bVar6 = true
                        quest:PauseAllNonScriptedEntities(false)
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                resources:ReleaseResource(xStack_38)
                resources:DestroyActorMap(xStack_210)
                resources:ReleaseResource(xStack_48)
            until not (bVar6)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:EntitySetTargetingType(me, 0x3a)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetStateInt("TutorialState", 4)
                quest:SetStateBool("TestFinished", true)
                pCVar12 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                iVar22 = 1
                uVar18 = 0
                uVar19 = 0
                uVar20 = 0
                uVar21 = 0
                iVar11 = 0
                iVar10 = 0x3f800000
                pCVar13 = pCVar12:GetPos()
                me:MoveToPosition(pCVar13, iVar10, iVar11, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                pCVar12 = quest:GetThingWithScriptName("WillApprentice")
                bVar6 = (pCVar12 ~= nil and pCVar12:IsAlive())
                if not bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        __cleanup_LAB_00d61b0a_c22()
                        return
                    end
                    pCVar12 = quest:GetThingWithScriptName("WillApprenticeMarker")
                    bVar6 = false
                    uVar18 = SUB41("WillApprentice",0)
                    uVar19 = ("WillApprentice" >> 8)
                    uVar20 = ("WillApprentice" >> 0x10)
                    uVar21 = ("WillApprentice" >> 0x18)
                    pCVar13 = pCVar12:GetPos()
                    r13 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar13, "WillApprentice")
                    if (r13 ~= nil and not r13:IsNull()) then
                        r13:SetToKillOnLevelUnload(0)
                    end
                end
                if quest:GetStateBool("BanditsDefeated") then
                    -- LAB_00d609de_c22: (native jump target)
                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
                    quest:SetPlayerUsingWillDummies(false)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    -- FLOW_native_label_2_c22: (native jump target)
                    if not bVar6 then
                        if not quest:GetStateBool("BanditsDefeated") then
                            u_stk_17c = u_stk_17c | 1
                            bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                            if bVar6 then return end  -- TODO(native): goto LAB_00d6158a_c22
                            bVar6 = true
                        else
                            -- LAB_00d6158a_c22: (native jump target)
                            bVar6 = false
                        end
                        if (u_stk_17c & 1) ~= 0 then
                            u_stk_17c = u_stk_17c & 0xfffffffe
                        end
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00d61b0a_c22(); return end
                            quest:SetStateBool("BanditsDefeated", true)
                        end
                        cVar7 = me:IsTalkedToByHero()
                        if not cVar7 then goto LAB_00d61af3 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            me:ClearCommands()
                            xStack_1f0 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar10 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then return end  -- TODO(native): goto LAB_00d61b53_c22
                                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- LAB_00d61b44_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if iVar10 == 1 then
                                    if not bVar6 then
                                        quest:FadeScreenOut(0.5, 0.5)
                                        quest:Pause(1.0)
                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                        quest:ConfiscateAllHeroWeapons()
                                        bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                                        if bVar6 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then return end  -- TODO(native): goto LAB_00d61b44_c22
                                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                        end
                                        delay = 0
                                        pCVar15 = quest:GetActiveQuestName()
                                        quest:DeactivateQuestLater(pCVar15, delay)
                                        quest:SetTimeOfDay(11.0)
                                        quest:ResetPlayerCreatureCombatMultiplier()
                                        quest:SetHeroAsTeenager(false)
                                        quest:ChangeHeroHealthBy(1000.0, true, false)
                                        bVar8 = true
                                        bVar6 = false
                                        pCVar12 = quest:GetThingWithScriptName("MeleeApprentice")
                                        quest:RemoveThing(pCVar12, bVar6, bVar8)
                                        pCVar12 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
                                        bVar6 = false
                                        uVar18 = SUB41("MeleeApprentice",0)
                                        uVar19 = ("MeleeApprentice" >> 8)
                                        uVar20 = ("MeleeApprentice" >> 0x10)
                                        uVar21 = ("MeleeApprentice" >> 0x18)
                                        pCVar13 = pCVar12:GetPos()
                                        r14 = quest:CreateCreature("MeleeApprenticeMarker", pCVar13, "MeleeApprentice")
                                        __region_LAB_00d61ad8_c22()
                                        goto LAB_00d61af3
                                    end
                                else
                                    if not bVar6 then
                                        xStack_54 = resources:ScriptThing(0)
                                        pCVar12 = xStack_54
                                        r15 = quest:GetHealth(pCVar12)
                                        fVar4 = 0.0
                                        if fVar4 < fret_04 then
                                            iVar22 = 0
                                            uVar18 = 1
                                            uVar19 = 0
                                            uVar20 = 0
                                            uVar21 = 0
                                            iVar11 = 0
                                            iVar10 = 0
                                            pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO"
                                            pCVar12 = quest:GetHero()
                                            r16 = me:Speak(pCVar12, pcVar17, iVar10, (iVar11 ~= 0), CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                                            iVar10 = me:IsPerformingScriptTask()
                                            cVar7 = iVar10
                                            while cVar7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then return end  -- TODO(native): goto LAB_00d61b44_c22
                                                iVar10 = me:IsPerformingScriptTask()
                                                cVar7 = iVar10
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then return end  -- TODO(native): goto LAB_00d61b53_c22
                                        end
                                        pCVar12 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                                        iVar22 = 1
                                        uVar18 = 0
                                        uVar19 = 0
                                        uVar20 = 0
                                        uVar21 = 0
                                        iVar11 = 0
                                        iVar10 = 0x3f800000
                                        pCVar13 = pCVar12:GetPos()
                                        me:MoveToPosition(pCVar13, iVar10, iVar11, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                                        __region_LAB_00d61ad8_c22(); goto LAB_00d61af3
                                    end
                                end
                                -- LAB_00d61b53_c22: (native jump target)
                                -- LAB_00d61b5a_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            resources:ReleaseResource(xStack_1f0)
                            goto LAB_00d61b69
                        end
                    end
                    __cleanup_LAB_00d61b0a_c22(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    quest:ActivateQuest("Q_GuildTrainingWoodsWill")
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
                    quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false)
                    quest:SetQuestCardObjective("GuildWoods", "Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "")
                    -- TODO(native): goto LAB_00d609de_c22
                end
                goto FLOW_after_lab_00d6070e
            end
        end
    else
        -- LAB_00d6070e: (native jump target)
        quest:EntitySetTargetingType(me, 0x3a)
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetStateBool("TestFinished", true)
        pCVar12 = quest:GetThingWithScriptName("M_GuildmasterMarker")
        iVar22 = 1
        uVar18 = 0
        uVar19 = 0
        uVar20 = 0
        uVar21 = 0
        iVar11 = 0
        iVar10 = 0x3f800000
        pCVar13 = pCVar12:GetPos()
        me:MoveToPosition(pCVar13, iVar10, iVar11, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
        pCVar12 = quest:GetThingWithScriptName("WillApprentice")
        bVar6 = (pCVar12 ~= nil and pCVar12:IsAlive())
        if not bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                __cleanup_LAB_00d61b0a()
                return
            end
            pCVar12 = quest:GetThingWithScriptName("WillApprenticeMarker")
            bVar6 = false
            uVar18 = SUB41("WillApprentice",0)
            uVar19 = ("WillApprentice" >> 8)
            uVar20 = ("WillApprentice" >> 0x10)
            uVar21 = ("WillApprentice" >> 0x18)
            pCVar13 = pCVar12:GetPos()
            r17 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar13, "WillApprentice")
            if (r17 ~= nil and not r17:IsNull()) then
                r17:SetToKillOnLevelUnload(0)
            end
        end
        if quest:GetStateBool("BanditsDefeated") then
            -- LAB_00d609de: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            quest:SetPlayerUsingWillDummies(false)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            -- FLOW_native_label_2: (native jump target)
            if not bVar6 then
                if not quest:GetStateBool("BanditsDefeated") then
                    u_stk_17c = u_stk_17c | 1
                    bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                    if bVar6 then
                        bVar6 = false
                        goto FLOW_after_lab_00d6158a
                    end
                    bVar6 = true
                else
                    -- LAB_00d6158a: (native jump target)
                    bVar6 = false
                end
                ::FLOW_after_lab_00d6158a::
                if (u_stk_17c & 1) ~= 0 then
                    u_stk_17c = u_stk_17c & 0xfffffffe
                end
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:DeregisterTimer(iVar11)
                        quest:DeregisterTimer(iVar11)
                        resources:ReleaseResource(xStack_158)
                        return
                    end
                    quest:SetStateBool("BanditsDefeated", true)
                end
                cVar7 = me:IsTalkedToByHero()
                if not cVar7 then goto LAB_00d61af3 end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    me:ClearCommands()
                    xStack_1f0 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00d61b5a_c25: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61b53
                        end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        -- LAB_00d61b44: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if iVar10 == 1 then
                            if not bVar6 then
                                quest:FadeScreenOut(0.5, 0.5)
                                quest:Pause(1.0)
                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                quest:ConfiscateAllHeroWeapons()
                                bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                                if bVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto FLOW_after_lab_00d61b53
                                    end
                                    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                end
                                delay = 0
                                pCVar15 = quest:GetActiveQuestName()
                                quest:DeactivateQuestLater(pCVar15, delay)
                                quest:SetTimeOfDay(11.0)
                                quest:ResetPlayerCreatureCombatMultiplier()
                                quest:SetHeroAsTeenager(false)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                bVar8 = true
                                bVar6 = false
                                pCVar12 = quest:GetThingWithScriptName("MeleeApprentice")
                                quest:RemoveThing(pCVar12, bVar6, bVar8)
                                pCVar12 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
                                bVar6 = false
                                uVar18 = SUB41("MeleeApprentice",0)
                                uVar19 = ("MeleeApprentice" >> 8)
                                uVar20 = ("MeleeApprentice" >> 0x10)
                                uVar21 = ("MeleeApprentice" >> 0x18)
                                pCVar13 = pCVar12:GetPos()
                                r18 = quest:CreateCreature("MeleeApprenticeMarker", pCVar13, "MeleeApprentice")
                                -- LAB_00d61ad8: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(xStack_158)
                                goto LAB_00d61af3
                            end
                        else
                            if not bVar6 then
                                xStack_54 = resources:ScriptThing(0)
                                pCVar12 = xStack_54
                                r19 = quest:GetHealth(pCVar12)
                                fVar4 = 0.0
                                if fVar4 < fret_04 then
                                    iVar22 = 0
                                    uVar18 = 1
                                    uVar19 = 0
                                    uVar20 = 0
                                    uVar21 = 0
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar17 = "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO"
                                    pCVar12 = quest:GetHero()
                                    r20 = me:Speak(pCVar12, pcVar17, iVar10, (iVar11 ~= 0), CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto FLOW_after_lab_00d61b53
                                        end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar7 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00d61b53 end
                                end
                                pCVar12 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                                iVar22 = 1
                                uVar18 = 0
                                uVar19 = 0
                                uVar20 = 0
                                uVar21 = 0
                                iVar11 = 0
                                iVar10 = 0x3f800000
                                pCVar13 = pCVar12:GetPos()
                                me:MoveToPosition(pCVar13, iVar10, iVar11, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))), (iVar22 ~= 0))
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(xStack_158)
                                goto LAB_00d61af3
                            end
                        end
                        ::LAB_00d61b53::
                        -- LAB_00d61b5a: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                    end
                    ::FLOW_after_lab_00d61b53::
                    resources:ReleaseResource(xStack_1f0)
                    goto LAB_00d61b69
                end
            end
            quest:DeregisterTimer(iVar11)
            quest:DeregisterTimer(iVar11)
            resources:ReleaseResource(xStack_158)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            quest:ActivateQuest("Q_GuildTrainingWoodsWill")
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false)
            quest:SetQuestCardObjective("GuildWoods", "Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "")
            -- TODO(native): goto LAB_00d609de
        end
    end
    ::FLOW_after_lab_00d6070e::
    goto LAB_00d61b69
    ::FLOW_native_label_3::
    if not cVar7 then goto LAB_00d610c4 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        resources:ReleaseResource(xStack_200)
        -- LAB_00d61342_c30: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto FLOW_after_lab_00d61578
    end
    iVar10 = me:IsPerformingScriptTask()
    cVar7 = iVar10
    goto FLOW_native_label_3
    ::LAB_00d610c4::
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        -- LAB_00d61563: (native jump target)
        resources:DestroyMovie(xStack_1d0)
        -- LAB_00d61578: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:DestroyMovie(xStack_1d0)
                -- LAB_00d61578_c31: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00d61578
            end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        end
        quest:SetTimeOfDay(11.0)
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:SetHeroAsTeenager(false)
        quest:ChangeHeroHealthBy(1000.0, true, false)
        bVar8 = true
        bVar6 = false
        pCVar9 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(pCVar9, bVar6, bVar8)
        pCVar9 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
        bVar6 = false
        pCVar13 = pCVar9:GetPos()
        r21 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", pCVar13, "MeleeApprentice")
        pCVar9 = quest:GetThingWithScriptName("WillApprentice")
        bVar6 = (pCVar9 ~= nil and pCVar9:IsAlive())
        if bVar6 then
            -- LAB_00d6144d: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238_12,(CCharString *)pCVar12);
            quest:SetPlayerUsingWillDummies(false)
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetStateBool("TestFinished", true)
            uVar18 = 0
            uVar19 = 0
            uVar20 = 0
            uVar21 = 0
            pCVar15 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar15, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))))
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
            until not (not bVar6)
            resources:DestroyMovie(xStack_1d0)
            -- LAB_00d61578_c32: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00d61578
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            pCVar9 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            bVar6 = false
            pCVar13 = pCVar9:GetPos()
            r22 = quest:CreateCreature("WillApprentice", pCVar13, "WillApprenticeMarker")
            if pCVar12._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_60._0_4_ + 0x118))(0);
            end
            -- TODO(native): goto LAB_00d6144d
        end
        -- LAB_00d6132d: (native jump target)
        resources:ReleaseResource(xStack_200)
        -- LAB_00d61342: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00d61578::
    -- TODO(native): LTextTreeWalkThrough__Dtor(xStack_1c0);
    goto LAB_00d6135b
    ::FLOW_native_label_4::
    if not cVar7 then goto LAB_00d60bd4 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        resources:ReleaseResource(xStack_1f0)
        -- LAB_00d60b31_c33: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d6135b
    end
    iVar10 = me:IsPerformingScriptTask()
    cVar7 = iVar10
    goto FLOW_native_label_4
    ::LAB_00d61af3::
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    -- TODO(native): goto FLOW_native_label_2
    ::LAB_00d60bd4::
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        -- LAB_00d60be3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        bVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d61004 end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        end
        quest:SetTimeOfDay(11.0)
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:SetHeroAsTeenager(false)
        quest:ChangeHeroHealthBy(1000.0, true, false)
        bVar8 = true
        bVar6 = false
        pCVar9 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(pCVar9, bVar6, bVar8)
        pCVar9 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
        bVar6 = false
        pCVar13 = pCVar9:GetPos()
        r23 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", pCVar13, "MeleeApprentice")
        pCVar9 = quest:GetThingWithScriptName("WillApprentice")
        bVar6 = (pCVar9 ~= nil and pCVar9:IsAlive())
        if not bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                pCVar9 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
                bVar6 = false
                pCVar13 = pCVar9:GetPos()
                r24 = quest:CreateCreature("MeleeApprentice", pCVar13, "WillApprentice")
                if pCVar12._0_4_ ~= nil then
                    -- TODO(native): (**(code **)(*xStack_60._0_4_ + 0x118))(0);
                end
                goto LAB_00d60ef5
            end
            -- LAB_00d60b19: (native jump target)
            resources:ReleaseResource(xStack_1f0)
            -- LAB_00d60b31: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            goto LAB_00d6135b
        end
        ::LAB_00d60ef5::
        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238_12,(CCharString *)pCVar12);
        quest:SetPlayerUsingWillDummies(false)
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetStateBool("TestFinished", true)
        uVar18 = 0
        uVar19 = 0
        uVar20 = 0
        uVar21 = 0
        pCVar15 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar15, CONCAT13(uVar21,CONCAT12(uVar20,CONCAT11(uVar19,uVar18))))
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
        until not (not bVar6)
    end
    ::FLOW_after_lab_00d60be3::
    ::LAB_00d61004::
    resources:DestroyMovie(xStack_1f0)
    ::LAB_00d6101c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d6135b::
    resources:ReleaseResource(0)
    resources:DestroyActorMap(xStack_210)
    ::LAB_00d61370::
    resources:DestroyMovie(xStack_1f0)
    ::LAB_00d61379::
    ::LAB_00d6138b::
    ::LAB_00d61b69::
    ::LAB_00d61b7b::
    resources:ReleaseResource(0)
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

