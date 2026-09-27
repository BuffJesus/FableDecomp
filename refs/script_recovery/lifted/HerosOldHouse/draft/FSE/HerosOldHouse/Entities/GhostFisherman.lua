-- Generated native draft: GhostFisherman. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, always_update, bVar5, cVar6, c_stk_dd, fVar15, fVar2, fret_0, fret_00, fret_01, iVar17, iVar19, iVar8, iVar9, pCVar10, pCVar12, pCVar13, pCVar7, pPosition, pScriptObject, pcVar14, r1, r2, r3, r4, r5, timerId, xStack_100, xStack_118, xStack_128, xStack_12c, xStack_3c, xStack_f0, x_stk_18, x_stk_c, x_stk_d4
    local alive = true
    xStack_128 = resources:NewResource()
    resources:PrepareResource(xStack_128)
    bVar5 = resources:TryAcquire(xStack_128, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:ReleaseResource(xStack_128)
            return
        end
        bVar5 = resources:TryAcquire(xStack_128, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        resources:ReleaseResource(xStack_128)
        return
    end
    r1 = quest:GetThingWithScriptName("HiddenBooty")
    timerId = quest:RegisterTimer()
    xStack_12c = timerId
    quest:SetTimer(timerId, 0)
    if not quest:GetStateBool("WifeAttacked") then
        while not __native_entity_state:GetStateBool("GhostGoing") do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d8bc9c end
            if not __native_entity_state:GetStateBool("HaveTalked") then
                fVar15 = 5.5
                pCVar7 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(pCVar7, me, fVar15)
                __native_condition_1 = bVar5
                if __native_condition_1 then
                    iVar8 = quest:GetTimer(timerId)
                    __native_condition_1 = iVar8 < 1
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8bc9c end
                    iVar9 = quest:AddNewConversation(me, false, false)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar9, pCVar7)
                    pCVar7 = quest:GetHero()
                    quest:AddLineToConversation(iVar9, "TEXT_QST_032_GHOST_FISHERMAN_HELP", me, pCVar7, false)
                    quest:SetTimer(xStack_12c, 0xf)
                end
            end
            __native_condition_2 = not quest:GetStateBool("Helping")
            if __native_condition_2 then
                bVar5 = quest:IsDiggingSpotEnabled(r1)
                __native_condition_2 = not bVar5
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d8bc9c end
                quest:SetStateBool("MissionAborted", true)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
            c_stk_dd = me:IsTalkedToByHero()
            if c_stk_dd then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    iVar9 = 4
                    xStack_100 = resources:NewResource()
                    pScriptObject = xStack_100
                    pCVar10 = quest:GetHero()
                    resources:TryAcquire(pScriptObject, pCVar10, iVar9)
                    xStack_118 = resources:NewActorMap()
                    resources:SetActor(xStack_118, "HERO", xStack_100)
                    resources:SetActor(xStack_118, "GHOST", xStack_128)
                    xStack_f0 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_SETUP", xStack_118, false, false)
                    if not __native_entity_state:GetStateBool("HaveTalked") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            __native_entity_state:SetStateBool("HaveTalked", true)
                            resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_1", xStack_118, false, true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar8 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_f0)
                                    resources:DestroyActorMap(xStack_118)
                                    resources:ReleaseResource(xStack_100)
                                    quest:DeregisterTimer(xStack_12c)
                                    goto LAB_00d8bca5
                                end
                                iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if iVar8 == 1 then
                                    if bVar5 then goto LAB_00d8bc59 end
                                    quest:SetStateBool("Helping", true)
                                    resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_2", xStack_118, false, true)
                                    bVar5 = false
                                    pCVar12 = quest:GetActiveQuestName()
                                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", pCVar12, bVar5)
                                    pCVar13 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(pCVar13, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")
                                    quest:MiniMapRemoveMarker(me)
                                    pCVar10 = quest:GetThingWithScriptName("FishermansWife")
                                    quest:MiniMapAddMarker(pCVar10, "HUD_ORB_QUEST_VIGNETTE")
                                else
                                    if bVar5 then goto LAB_00d8bb8e end
                                    resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_REFUSAL", xStack_118, false, true)
                                end
                                goto LAB_00d8bac9
                            end
                        end
                        goto LAB_00d8bb8e
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            if quest:GetStateBool("Helping") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d8bb8e end
                                bVar5 = quest:IsDiggingSpotEnabled(r1)
                                if bVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00d8bc59 end
                                    x_stk_c = resources:ScriptThing(xStack_128)
                                    pCVar10 = x_stk_c
                                    fret_0 = quest:GetHealth(pCVar10)
                                    fVar2 = 0.0
                                    if fVar2 < fret_0 then
                                        iVar19 = 0
                                        iVar17 = 1
                                        iVar9 = 0
                                        iVar8 = 0
                                        pcVar14 = "TEXT_QST_032_GHOST_FISHERMAN_WHERE"
                                        pCVar10 = quest:GetHero()
                                        r2 = me:Speak(pCVar10, pcVar14, iVar8, (iVar9 ~= 0), (iVar17 ~= 0), (iVar19 ~= 0))
                                        iVar8 = me:IsPerformingScriptTask()
                                        cVar6 = iVar8
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(xStack_f0)
                                                resources:DestroyActorMap(xStack_118)
                                                resources:ReleaseResource(xStack_100)
                                                quest:DeregisterTimer(xStack_12c)
                                                goto LAB_00d8bca5
                                            end
                                            iVar8 = me:IsPerformingScriptTask()
                                            cVar6 = iVar8
                                        end
                                        goto LAB_00d8bab8
                                    end
                                    goto FLOW_hoist_lab_00d8bab8_1
                                end
                                goto FLOW_hoist_lab_00d8bab8_2
                            end
                            goto FLOW_hoist_lab_00d8bab8_3
                        end
                        goto FLOW_hoist_lab_00d8bab8_4
                    end
                    goto FLOW_past_lab_00d8bab8
                    ::LAB_00d8bab8::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8bc59 end
                    ::FLOW_hoist_lab_00d8bab8_1::
                    goto LAB_00d8bac9
                    ::FLOW_hoist_lab_00d8bab8_2::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8bb8e end
                    if quest:GetStateBool("Helped") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d8bc59 end
                        resources:RunMacro("CS_GHOSTFISH_FISH_SUCCESS", xStack_118, false, true)
                        pCVar13 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar13, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_04", "", "OakBay")
                        __native_entity_state:SetStateBool("GhostGoing", true)
                        goto LAB_00d8bac9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d8bb8e end
                    x_stk_18 = resources:ScriptThing(xStack_128)
                    pCVar10 = x_stk_18
                    fret_00 = quest:GetHealth(pCVar10)
                    fVar2 = 0.0
                    if fVar2 < fret_00 then
                        iVar19 = 0
                        iVar17 = 1
                        iVar9 = 0
                        iVar8 = 0
                        pcVar14 = "TEXT_QST_032_GHOST_FISHERMAN_COMPLAINT"
                        pCVar10 = quest:GetHero()
                        r3 = me:Speak(pCVar10, pcVar14, iVar8, (iVar9 ~= 0), (iVar17 ~= 0), (iVar19 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar6 = iVar8
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d8bc59 end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar6 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00d8b75b: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d8bc64
                        end
                    end
                    goto LAB_00d8bac9
                    ::FLOW_hoist_lab_00d8bab8_3::
                    goto FLOW_hoist_lab_00d8bac9_1
                    ::FLOW_hoist_lab_00d8bab8_4::
                    goto FLOW_hoist_lab_00d8bac9_2
                    ::FLOW_past_lab_00d8bab8::
                    goto FLOW_past_lab_00d8bb8e
                    ::LAB_00d8bb8e::
                    quest:PauseAllNonScriptedEntities(false)
                    ::FLOW_past_lab_00d8bb8e::
                    goto FLOW_past_lab_00d8bac9
                    ::LAB_00d8bac9::
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_f0)
                    resources:DestroyActorMap(xStack_118)
                    resources:ReleaseResource(xStack_100)
                    goto LAB_00d8bb7b
                    ::FLOW_hoist_lab_00d8bac9_1::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar8 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_f0)
                                -- LAB_00d8bc2d: (native jump target)
                                resources:DestroyActorMap(xStack_118)
                                resources:ReleaseResource(xStack_100)
                                quest:DeregisterTimer(xStack_12c)
                                goto LAB_00d8bca5
                            end
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if iVar8 == 1 then
                                if bVar5 then goto LAB_00d8bb8e end
                                quest:SetStateBool("Helping", true)
                                resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_2", xStack_118, false, true)
                                bVar5 = false
                                pCVar12 = quest:GetActiveQuestName()
                                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", pCVar12, bVar5)
                                pCVar13 = quest:GetActiveQuestName()
                                quest:SetQuestCardObjective(pCVar13, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")
                                quest:MiniMapRemoveMarker(me)
                                pCVar10 = quest:GetThingWithScriptName("FishermansWife")
                                quest:MiniMapAddMarker(pCVar10, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if bVar5 then goto LAB_00d8bc59 end
                                xStack_3c = resources:ScriptThing(xStack_128)
                                pCVar10 = xStack_3c
                                fret_01 = quest:GetHealth(pCVar10)
                                fVar2 = 0.0
                                if fVar2 < fret_01 then
                                    iVar19 = 0
                                    iVar17 = 1
                                    iVar9 = 0
                                    iVar8 = 0
                                    pcVar14 = "TEXT_QST_032_GHOST_FISHERMAN_REFUSAL"
                                    pCVar10 = quest:GetHero()
                                    r4 = me:Speak(pCVar10, pcVar14, iVar8, (iVar9 ~= 0), (iVar17 ~= 0), (iVar19 ~= 0))
                                    iVar8 = me:IsPerformingScriptTask()
                                    cVar6 = iVar8
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00d8bc59 end
                                        iVar8 = me:IsPerformingScriptTask()
                                        cVar6 = iVar8
                                    end
                                    goto LAB_00d8bab8
                                end
                            end
                            goto LAB_00d8bac9
                        end
                    end
                    ::FLOW_hoist_lab_00d8bac9_2::
                    goto LAB_00d8bc59
                    ::FLOW_past_lab_00d8bac9::
                    goto FLOW_past_lab_00d8bc59
                    ::LAB_00d8bc59::
                    quest:PauseAllNonScriptedEntities(false)
                    ::FLOW_past_lab_00d8bc59::
                    ::LAB_00d8bc64::
                    resources:DestroyMovie(xStack_f0)
                    resources:DestroyActorMap(xStack_118)
                    resources:ReleaseResource(xStack_100)
                end
                goto LAB_00d8bc9c
            end
            ::LAB_00d8bb7b::
            timerId = xStack_12c
            if quest:GetStateBool("WifeAttacked") then break end
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        if not quest:GetStateBool("WifeAttacked") then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                quest:RemoveThing(me, false, true)
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                always_update = false
                bVar5 = false
                fVar15 = 0.0
                pPosition = pCVar10:GetPos()
                r5 = quest:CreateEffectAtPos("Ghost_Appear_01", pPosition, fVar15, bVar5)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
        end
    end
    ::LAB_00d8bc9c::
    quest:DeregisterTimer(xStack_12c)
    ::LAB_00d8bca5::
    resources:ReleaseResource(xStack_128)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HaveTalked", false)
    __native_entity_state:SetStateBool("GhostGoing", false)
    quest:EntitySetAsAbleToWalkThroughSolidObjects(me, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetTargetingType(me, 2)
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_INANIMATE_EVIL_HIGH")
    quest:SetThingHasInformation(me, false, true, false)
    if not quest:GetStateBool("Helping") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

function OnPersist(quest, me, context)
    local haveTalked = quest:GetStateBool("HaveTalked") or false
    haveTalked = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
    quest:SetStateBool("HaveTalked", haveTalked)
end

function OnPredicateFail(quest, me)
end

