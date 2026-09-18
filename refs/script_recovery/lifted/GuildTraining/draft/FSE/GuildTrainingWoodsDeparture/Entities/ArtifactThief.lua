-- Generated native draft: ArtifactThief. Review coverage report before use.
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
    local __native_condition_1, bVar2, cVar3, dist, fVar1, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, iVar13, iVar14, iVar5, iVar6, p0, pCVar4, pCVar7, pCVar8, pcVar12, r1, r10, r11, r12, r13, r2, r3, r4, r5, r6, r7, r8, r9, uVar10, uVar9, u_stk_174, xStack_128, xStack_138, xStack_148, xStack_158, xStack_170, xStack_184, xStack_188, xStack_94, xStack_a0, xStack_bc, x_stk_18, x_stk_24, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_6c, x_stk_78, x_stk_c
    local alive = true
    local function __region_LAB_00d632c3_c2()
        quest:PauseAllNonScriptedEntities(false)
        pCVar8 = xStack_148
    end
    local function __cleanup_LAB_00d63aa6()
        resources:DestroyMovie(pCVar8)
        quest:DeregisterTimer(xStack_188)
        resources:ReleaseResource(xStack_184)
    end
    local function __cleanup_LAB_00d63aab()
        quest:DeregisterTimer(xStack_188)
        resources:ReleaseResource(xStack_184)
    end
    uVar9 = 0
    u_stk_174 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_184 = resources:NewResource()
    bVar2 = false
    if bVar2 ~= 0 then
    end
    bVar2 = resources:TryAcquire(xStack_184, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d63c9f end
        bVar2 = resources:TryAcquire(xStack_184, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        __native_entity_state:SetStateBool("HoldingArtifact", true)
        __native_entity_state:SetStateBool("AlreadyTalkedTo", false)
        __native_entity_state:SetStateBool("NotAttacked", true)
        xStack_188 = quest:RegisterTimer()
        quest:SetTimer(xStack_188, 0)
        cVar3 = __native_entity_state:GetStateBool("HoldingArtifact")
        repeat
            if (not cVar3) or (not __native_entity_state:GetStateBool("NotAttacked")) then goto LAB_00d638ec end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d63c96 end
            dist = 5.5
            pCVar4 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar4, me, dist)
            __native_condition_1 = bVar2
            if __native_condition_1 then
                iVar5 = quest:GetTimer(xStack_188)
                __native_condition_1 = iVar5 < 1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                iVar6 = quest:AddNewConversation(me, false, false)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(iVar6, pCVar4)
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    bVar2 = false
                    pCVar4 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar4, bVar2)
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                    pCVar4 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, pCVar4, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    bVar2 = false
                    pCVar4 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar4, bVar2)
                    pCVar4 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, pCVar4, false)
                end
                quest:SetTimer(xStack_188, 10)
                uVar9 = u_stk_174
            end
            uVar10 = uVar9 | 1
            u_stk_174 = uVar10
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                -- LAB_00d6280c: (native jump target)
                bVar2 = true
            else
                uVar10 = uVar9 | 3
                u_stk_174 = uVar10
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    uVar10 = uVar9 | 7
                    u_stk_174 = uVar10
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                    if not bVar2 then
                        bVar2 = true
                        goto FLOW_after_lab_00d6280c
                    end
                end
                bVar2 = false
            end
            ::FLOW_after_lab_00d6280c::
            if (uVar10 & 4) ~= 0 then
                uVar10 = uVar10 & 0xfffffffb
                u_stk_174 = uVar10
            end
            if (uVar10 & 2) ~= 0 then
                uVar10 = uVar10 & 0xfffffffd
                u_stk_174 = uVar10
            end
            if (uVar10 & 1) ~= 0 then
                uVar10 = uVar10 & 0xfffffffe
                u_stk_174 = uVar10
            end
            uVar9 = uVar10
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                __native_entity_state:SetStateBool("NotAttacked", false)
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    xStack_138 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_6c = resources:ScriptThing(xStack_184)
                    pCVar4 = x_stk_6c
                    fret_00 = quest:GetHealth(pCVar4)
                    fVar1 = 0.0
                    if fVar1 < fret_00 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT"
                        pCVar4 = quest:GetHero()
                        r1 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_138)
                                __cleanup_LAB_00d63aab(); return
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_138)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_138;
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    xStack_bc = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_78 = resources:ScriptThing(xStack_184)
                    pCVar4 = x_stk_78
                    fret_0 = quest:GetHealth(pCVar4)
                    fVar1 = 0.0
                    if fVar1 < fret_0 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT"
                        pCVar4 = quest:GetHero()
                        r2 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_bc)
                                __cleanup_LAB_00d63aab(); return
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_bc)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_bc;
                end
                quest:EntitySetAsKillable(me, true, true)
                pCVar4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                iVar14 = 1
                iVar13 = 0
                iVar6 = 1
                iVar5 = 0x3f800000
                pCVar7 = pCVar4:GetPos()
                me:MoveToPosition(pCVar7, iVar5, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
                pCVar4 = nil
                uVar9 = u_stk_174
            end
            bVar2 = me:IsTalkedToByHero()
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                if not __native_entity_state:GetStateBool("HoldingArtifact") then goto LAB_00d638ec end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    __native_entity_state:SetStateBool("AlreadyTalkedTo", true)
                    xStack_148 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_54 = resources:ScriptThing(xStack_184)
                    pCVar4 = x_stk_54
                    fret_01 = quest:GetHealth(pCVar4)
                    fVar1 = 0.0
                    if fVar1 < fret_01 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT"
                        pCVar4 = quest:GetHero()
                        r3 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00d63a7c end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar8 = xStack_148
                                    -- LAB_00d63aa6_c2: (native jump target)
                                    resources:DestroyMovie(pCVar8)
                                    -- LAB_00d63aab_c2: (native jump target)
                                    quest:DeregisterTimer(xStack_188)
                                    resources:ReleaseResource(xStack_184)
                                    return
                                end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if iVar5 == 1 then
                                    if not bVar2 then
                                        xStack_170 = quest:GetHeroGold()
                                        if xStack_170 < quest:ReadGlobalGameDataFloat(0xf14) then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if not bVar2 then
                                                x_stk_3c = resources:ScriptThing(xStack_184)
                                                pCVar4 = x_stk_3c
                                                fret_02 = quest:GetHealth(pCVar4)
                                                fVar1 = 0.0
                                                if fVar1 < fret_02 then
                                                    iVar14 = 0
                                                    iVar13 = 1
                                                    iVar6 = 0
                                                    iVar5 = 0
                                                    pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                                                    pCVar4 = quest:GetHero()
                                                    r4 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar3 = iVar5
                                                    while cVar3 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar2 = not alive
                                                        if bVar2 then goto LAB_00d63a7c end
                                                        iVar5 = me:IsPerformingScriptTask()
                                                        cVar3 = iVar5
                                                    end
                                                    -- TODO(native): goto LAB_00d632b4_c2
                                                end
                                                __region_LAB_00d632c3_c2(); goto LAB_00d638d8
                                            end
                                            goto LAB_00d63a7c
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if not bVar2 then
                                            x_stk_24 = resources:ScriptThing(xStack_184)
                                            pCVar4 = x_stk_24
                                            fret_03 = quest:GetHealth(pCVar4)
                                            fVar1 = 0.0
                                            if fVar1 < fret_03 then
                                                iVar14 = 0
                                                iVar13 = 1
                                                iVar6 = 0
                                                iVar5 = 0
                                                pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                                                pCVar4 = quest:GetHero()
                                                r5 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar3 = iVar5
                                                while cVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar2 = not alive
                                                    if bVar2 then goto LAB_00d63a7c end
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar3 = iVar5
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if bVar2 then goto LAB_00d63a7c end
                                            end
                                            quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                            quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                            iVar6 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xf14)))
                                            quest:GiveHeroGold(iVar6)
                                            iVar6 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf14)))
                                            quest:EntityGiveGold(me, iVar6)
                                            __native_entity_state:SetStateBool("HoldingArtifact", false)
                                            pCVar4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                                            iVar14 = 1
                                            iVar13 = 0
                                            iVar6 = 0
                                            iVar5 = 0x3f800000
                                            pCVar7 = pCVar4:GetPos()
                                            me:MoveToPosition(pCVar7, iVar5, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar8 = xStack_148
                                            goto LAB_00d638d8
                                        end
                                    end
                                    -- LAB_00d63a62_c2: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_148)
                                    goto LAB_00d63c96
                                end
                                if not bVar2 then
                                    x_stk_48 = resources:ScriptThing(xStack_184)
                                    pCVar4 = x_stk_48
                                    fret_04 = quest:GetHealth(pCVar4)
                                    fVar1 = 0.0
                                    if fVar1 < fret_04 then
                                        iVar14 = 0
                                        iVar13 = 1
                                        iVar6 = 0
                                        iVar5 = 0
                                        pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO"
                                        pCVar4 = quest:GetHero()
                                        r6 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                        while cVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then goto LAB_00d63a7c end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar3 = iVar5
                                        end
                                        -- LAB_00d632b4_c2: (native jump target)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d63a7c end
                                    end
                                    __region_LAB_00d632c3_c2()
                                    goto LAB_00d638d8
                                end
                            end
                            goto FLOW_after_lab_00d62e38
                        end
                    else
                        -- LAB_00d62e38: (native jump target)
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar5 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar8 = xStack_148
                                __cleanup_LAB_00d63aa6()
                                return
                            end
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if iVar5 == 1 then
                                if not bVar2 then
                                    xStack_170 = quest:GetHeroGold()
                                    if xStack_170 < quest:ReadGlobalGameDataFloat(0xf14) then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if not bVar2 then
                                            x_stk_3c = resources:ScriptThing(xStack_184)
                                            pCVar4 = x_stk_3c
                                            fret_02 = quest:GetHealth(pCVar4)
                                            fVar1 = 0.0
                                            if fVar1 < fret_02 then
                                                iVar14 = 0
                                                iVar13 = 1
                                                iVar6 = 0
                                                iVar5 = 0
                                                pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                                                pCVar4 = quest:GetHero()
                                                r7 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar3 = iVar5
                                                while cVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar2 = not alive
                                                    if bVar2 then goto LAB_00d63a7c end
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar3 = iVar5
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if bVar2 then goto LAB_00d63a7c end
                                                -- LAB_00d632c3_c3: (native jump target)
                                                quest:PauseAllNonScriptedEntities(false)
                                                pCVar8 = xStack_148
                                                goto LAB_00d638d8
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar8 = xStack_148
                                            goto LAB_00d638d8
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        x_stk_24 = resources:ScriptThing(xStack_184)
                                        pCVar4 = x_stk_24
                                        fret_03 = quest:GetHealth(pCVar4)
                                        fVar1 = 0.0
                                        if fVar1 < fret_03 then
                                            iVar14 = 0
                                            iVar13 = 1
                                            iVar6 = 0
                                            iVar5 = 0
                                            pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                                            pCVar4 = quest:GetHero()
                                            r8 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar3 = iVar5
                                            while cVar3 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if bVar2 then goto LAB_00d63a7c end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar3 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then goto LAB_00d63a7c end
                                        end
                                        quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                        quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                        iVar6 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xf14)))
                                        quest:GiveHeroGold(iVar6)
                                        iVar6 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf14)))
                                        quest:EntityGiveGold(me, iVar6)
                                        __native_entity_state:SetStateBool("HoldingArtifact", false)
                                        pCVar4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                                        iVar14 = 1
                                        iVar13 = 0
                                        iVar6 = 0
                                        iVar5 = 0x3f800000
                                        pCVar7 = pCVar4:GetPos()
                                        me:MoveToPosition(pCVar7, iVar5, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar8 = xStack_148
                                        goto LAB_00d638d8
                                    end
                                end
                                -- LAB_00d63a62: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_148)
                                goto LAB_00d63c96
                            end
                            if not bVar2 then
                                x_stk_48 = resources:ScriptThing(xStack_184)
                                pCVar4 = x_stk_48
                                fret_04 = quest:GetHealth(pCVar4)
                                fVar1 = 0.0
                                if fVar1 < fret_04 then
                                    iVar14 = 0
                                    iVar13 = 1
                                    iVar6 = 0
                                    iVar5 = 0
                                    pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO"
                                    pCVar4 = quest:GetHero()
                                    r9 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d63a7c end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    -- LAB_00d632b4: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00d63a7c end
                                end
                                -- LAB_00d632c3: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar8 = xStack_148
                                goto LAB_00d638d8
                            end
                        end
                    end
                    ::FLOW_after_lab_00d62e38::
                    ::LAB_00d63a7c::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_148)
                    goto LAB_00d63c96
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                xStack_158 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                x_stk_c = resources:ScriptThing(xStack_184)
                pCVar4 = x_stk_c
                fret_05 = quest:GetHealth(pCVar4)
                fVar1 = 0.0
                if fVar1 < fret_05 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar6 = 0
                    iVar5 = 0
                    pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN"
                    pCVar4 = quest:GetHero()
                    r10 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar5 = me:IsPerformingScriptTask()
                    cVar3 = iVar5
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            goto LAB_00d63c96
                        end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then goto LAB_00d633f7 end
                    -- LAB_00d635e3: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_158)
                    goto LAB_00d63c96
                end
                ::LAB_00d633f7::
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar5 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        pCVar8 = xStack_158
                        __cleanup_LAB_00d63aa6(); return
                    end
                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_158)
                    goto LAB_00d63c96
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if iVar5 ~= 1 then
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_158)
                        goto LAB_00d63c96
                    end
                    xStack_94 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_a0 = resources:ScriptThing(xStack_184)
                    pCVar4 = xStack_a0
                    fret_08 = quest:GetHealth(pCVar4)
                    fVar1 = 0.0
                    if fVar1 < fret_08 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO"
                        pCVar4 = quest:GetHero()
                        r11 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_94)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_158)
                                goto LAB_00d63c96
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            -- LAB_00d63aec: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_94)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            goto LAB_00d63c96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_94)
                    goto LAB_00d638c8
                end
                if bVar2 then
                    -- LAB_00d63ad2: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_158)
                    goto LAB_00d63c96
                end
                xStack_128 = quest:GetHeroGold()
                -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
                if false then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        x_stk_60 = resources:ScriptThing(xStack_184)
                        pCVar4 = x_stk_60
                        fret_07 = quest:GetHealth(pCVar4)
                        fVar1 = 0.0
                        if fVar1 < fret_07 then
                            iVar14 = 0
                            iVar13 = 1
                            iVar6 = 0
                            iVar5 = 0
                            pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                            pCVar4 = quest:GetHero()
                            r12 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_158)
                                    goto LAB_00d63c96
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar3 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_158)
                                goto LAB_00d63c96
                            end
                        end
                        quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                        quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                        iVar6 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xf14)))
                        quest:GiveHeroGold(iVar6)
                        iVar6 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf14)))
                        quest:EntityGiveGold(me, iVar6)
                        __native_entity_state:SetStateBool("HoldingArtifact", false)
                        pCVar4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                        iVar14 = 1
                        iVar13 = 0
                        iVar6 = 0
                        iVar5 = 0x3f800000
                        pCVar7 = pCVar4:GetPos()
                        me:MoveToPosition(pCVar7, iVar5, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
                        goto LAB_00d638c8
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_158)
                    goto LAB_00d63c96
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_158)
                    goto LAB_00d63c96
                end
                x_stk_18 = resources:ScriptThing(xStack_184)
                pCVar4 = x_stk_18
                fret_06 = quest:GetHealth(pCVar4)
                fVar1 = 0.0
                if fVar1 < fret_06 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar6 = 0
                    iVar5 = 0
                    pcVar12 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                    pCVar4 = quest:GetHero()
                    r13 = me:Speak(pCVar4, pcVar12, iVar5, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar5 = me:IsPerformingScriptTask()
                    cVar3 = iVar5
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            goto LAB_00d63c96
                        end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_158)
                        goto LAB_00d63c96
                    end
                end
                ::LAB_00d638c8::
                quest:PauseAllNonScriptedEntities(false)
                pCVar8 = xStack_158
                ::LAB_00d638d8::
                resources:DestroyMovie(pCVar8)
                uVar9 = u_stk_174
            end
            cVar3 = __native_entity_state:GetStateBool("HoldingArtifact")
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        iVar5 = me:IsPerformingScriptTask()
        cVar3 = iVar5
        while cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d63c96 end
            uVar10 = uVar9 | 8
            u_stk_174 = uVar10
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                -- LAB_00d63b21: (native jump target)
                bVar2 = true
            else
                uVar10 = uVar9 | 0x18
                u_stk_174 = uVar10
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    uVar10 = uVar9 | 0x38
                    u_stk_174 = uVar10
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                    if not bVar2 then
                        bVar2 = true
                        goto FLOW_after_lab_00d63b21
                    end
                end
                bVar2 = false
            end
            ::FLOW_after_lab_00d63b21::
            if (uVar10 & 0x20) ~= 0 then
                uVar10 = uVar10 & 0xffffffdf
                u_stk_174 = uVar10
            end
            if (uVar10 & 0x10) ~= 0 then
                uVar10 = uVar10 & 0xffffffef
                u_stk_174 = uVar10
            end
            if (uVar10 & 8) ~= 0 then
                uVar10 = uVar10 & 0xfffffff7
                u_stk_174 = uVar10
            end
            uVar9 = uVar10
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d63c96 end
                if __native_entity_state:GetStateBool("NotAttacked") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d63c96 end
                    iVar6 = quest:AddNewConversation(me, false, false)
                    pCVar4 = quest:GetHero()
                    quest:AddPersonToConversation(iVar6, pCVar4)
                    pCVar4 = quest:GetHero()
                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, pCVar4, false)
                    __native_entity_state:SetStateBool("NotAttacked", false)
                    quest:EntitySetAsKillable(me, true, true)
                    pCVar4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                    iVar14 = 1
                    iVar13 = 0
                    iVar6 = 1
                    iVar5 = 0x3f800000
                    pCVar7 = pCVar4:GetPos()
                    me:MoveToPosition(pCVar7, iVar5, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
                    uVar9 = u_stk_174
                end
            end
            iVar5 = me:IsPerformingScriptTask()
            cVar3 = iVar5
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
    end
    ::LAB_00d63c96::
    quest:DeregisterTimer(xStack_188)
    ::LAB_00d63c9f::
    resources:ReleaseResource(xStack_184)
end

function Init(quest, me)
    quest:AddItemToContainer(me, "OBJECT_HAND_LAMP")
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

