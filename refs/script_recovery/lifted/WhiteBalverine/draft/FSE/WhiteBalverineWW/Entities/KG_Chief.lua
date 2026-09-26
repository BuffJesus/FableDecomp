-- Generated native draft: KG_Chief. Review coverage report before use.
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
    local bVar4, bVar5, bVar7, cVar6, fVar2, fret_0, fret_00, fret_01, iVar11, iVar12, iVar13, iVar14, pCVar8, pQuestName, pScriptObject, pcVar10, r1, r2, r3, xStack_34, xStack_48, xStack_78, xStack_84, xStack_94, xStack_9c, xStack_b0, x_stk_18, x_stk_24, x_stk_c
    local alive = true
    local function __cleanup_LAB_00e1a2b3()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_94)
        resources:ReleaseResource(xStack_b0)
    end
    local function __cleanup_LAB_00e1a6d6()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_94)
        resources:ReleaseResource(xStack_b0)
    end
    bVar5 = false
    bVar7 = false
    xStack_9c = pCVar8
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
        quest:EntitySetAsToAddToStatChangesWhenHit(me, false)
        me:SetFriendsWithEverythingFlag(true)
        xStack_b0 = resources:NewResource()
        resources:PrepareResource(xStack_b0)
        bVar4 = resources:TryAcquire(xStack_b0, me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e1a6c6 end
            bVar4 = resources:TryAcquire(xStack_b0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            pQuestName = quest:GetActiveQuestName()
            quest:EntityAttachToScript(me, pQuestName)
            cVar6 = quest:GetStateBool("MissionSucceeded")
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e1a6c6 end
                bVar4 = me:IsTalkedToByHero()
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e1a6c6 end
                    if not quest:GetStateBool("WhiteBalverineAlive") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e1a6c6 end
                        iVar13 = 4
                        xStack_34 = resources:NewResource()
                        pScriptObject = xStack_34
                        pCVar8 = quest:GetHero()
                        resources:TryAcquire(pScriptObject, pCVar8, iVar13)
                        xStack_84 = resources:NewActorMap()
                        resources:SetActor(xStack_84, "HERO", xStack_34)
                        resources:SetActor(xStack_84, "CHIEF", xStack_b0)
                        xStack_78 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_WBK_CHIEF4", xStack_84, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:SetStateBool("MissionSucceeded", true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_78)
                        resources:DestroyActorMap(xStack_84)
                        resources:ReleaseResource(xStack_34)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e1a6c6 end
                        xStack_94 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not __native_entity_state:GetStateBool("InitialChatted") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                __cleanup_LAB_00e1a2b3()
                                return
                            end
                            __native_entity_state:SetStateBool("InitialChatted", false)
                            x_stk_c = resources:ScriptThing(xStack_b0)
                            pCVar8 = x_stk_c
                            fret_0 = quest:GetHealth(pCVar8)
                            fVar2 = 0.0
                            if fVar2 < fret_0 then
                                iVar14 = 0
                                iVar12 = 1
                                iVar13 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST"
                                pCVar8 = quest:GetHero()
                                r1 = me:Speak(pCVar8, pcVar10, iVar11, (iVar13 ~= 0), (iVar12 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then __cleanup_LAB_00e1a6d6(); return end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_94)
                                    resources:ReleaseResource(xStack_b0)
                                    return
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                __cleanup_LAB_00e1a6d6()
                                return
                            end
                            x_stk_24 = resources:ScriptThing(xStack_b0)
                            pCVar8 = x_stk_24
                            fret_00 = quest:GetHealth(pCVar8)
                            fVar2 = 0.0
                            if fVar2 < fret_00 then
                                iVar14 = 0
                                iVar12 = 1
                                iVar13 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_074_CHIEF_BALVERINE_GONE_SECOND"
                                pCVar8 = quest:GetHero()
                                r2 = me:Speak(pCVar8, pcVar10, iVar11, (iVar13 ~= 0), (iVar12 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then __cleanup_LAB_00e1a2b3(); return end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_94)
                                    resources:ReleaseResource(xStack_b0)
                                    return
                                end
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_94)
                    end
                end
                pCVar8 = xStack_9c
                bVar4 = me:MsgIsHitByHero()
                if bVar4 then
                    goto LAB_00e1a451
                else
                    bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar5 then
                        bVar5 = true
                        bVar7 = true
                        bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar4 then goto LAB_00e1a451 end
                    end
                    bVar5 = true
                    bVar4 = false
                end
                goto FLOW_past_lab_00e1a451
                ::LAB_00e1a451::
                bVar4 = true
                ::FLOW_past_lab_00e1a451::
                if bVar7 then
                    bVar7 = false
                end
                if bVar5 then
                    bVar5 = false
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e1a6c6 end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e1a6c6 end
                    resources:PrepareResource(xStack_b0)
                    bVar4 = resources:TryAcquire(xStack_b0, pCVar8, 4)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e1a6c6 end
                        bVar4 = resources:TryAcquire(xStack_b0, pCVar8, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e1a6c6 end
                    xStack_48 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_18 = resources:ScriptThing(xStack_b0)
                    pCVar8 = x_stk_18
                    fret_01 = quest:GetHealth(pCVar8)
                    fVar2 = 0.0
                    if fVar2 < fret_01 then
                        iVar14 = 0
                        iVar12 = 1
                        iVar13 = 0
                        iVar11 = 0
                        pcVar10 = "TEXT_QST_074_CHIEF_BEEN_ATTACKED"
                        pCVar8 = quest:GetHero()
                        r3 = me:Speak(pCVar8, pcVar10, iVar11, (iVar13 ~= 0), (iVar12 ~= 0), (iVar14 ~= 0))
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_48)
                                resources:ReleaseResource(xStack_b0)
                                return
                            end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_48)
                            resources:ReleaseResource(xStack_b0)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_48)
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e1a6c6 end
                    me:SetFriendsWithEverythingFlag(xStack_9c)
                end
                cVar6 = quest:GetStateBool("MissionSucceeded")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if not bVar7 then
                quest:ClearThingHasInformation(me)
            end
        end
        ::LAB_00e1a6c6::
        resources:ReleaseResource(xStack_b0)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("InitialChatted", false)
    quest:SetThingHasInformation(me, false, true, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

