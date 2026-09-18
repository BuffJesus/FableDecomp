-- Generated native draft: BirdKiller. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar3, cVar4, ctr_90, elem_1, fVar14, fVar2, fret_0, fret_00, fret_01, iVar12, iVar13, iVar6, iVar7, pCVar15, pCVar5, pCVar9, pPosition, pThing, pcVar11, r1, r2, r3, r4, r5, timerId, uVar10, xStack_38, xStack_78, xStack_8c, x_stk_68, x_stk_8, x_stk_c
    local alive = true
    local function __region_LAB_00d4e853_c2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_78)
    end
    local function __cleanup_LAB_00d4e91f()
        quest:PauseAllNonScriptedEntities(false)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(xStack_8c)
    end
    uVar10 = 0
    xStack_8c = resources:NewResource()
    bVar3 = false
    if bVar3 ~= 0 then
    end
    bVar3 = resources:TryAcquire(xStack_8c, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_8c)
            return
        end
        bVar3 = resources:TryAcquire(xStack_8c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        resources:ReleaseResource(xStack_8c)
        return
    end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, true, false)
    me:SetFriendsWithEverythingFlag(me)
    if __native_entity_state:GetStateInt("BirdMode") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d4ef90 end
        x_stk_68 = quest:GetAllThingsWithScriptName("BirdMarker")
        if #x_stk_68 ~= 0 then
            ctr_90 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                bVar3 = false
                elem_1 = x_stk_68[(ctr_90) / 0xc + 1]
                pPosition = elem_1:GetPos()
                r1 = quest:CreateCreature("CREATURE_BIRD_GUILD_SPARROW", pPosition, "KillBird")
                quest:SetThingPersistent(r1, true)
                ctr_90 = ctr_90 + 0xc
                uVar10 = uVar10 + 1
            until not (uVar10 < (#x_stk_68))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d4e0f2: (native jump target)
            resources:ReleaseResource(xStack_8c)
            return
        end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        __native_entity_state:SetStateInt("BirdMode", 2)
    end
    timerId = quest:RegisterTimer()
    ctr_90 = timerId
    quest:SetTimer(timerId, 0xf)
    iVar6 = __native_entity_state:GetStateInt("BirdMode")
    while iVar6 == 2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d4ef87 end
        fVar14 = 5.5
        pCVar5 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar14)
        __native_condition_1 = bVar3
        if __native_condition_1 then
            iVar6 = quest:GetTimer(timerId)
            __native_condition_1 = iVar6 < 1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d4ef87 end
            iVar7 = quest:AddNewConversation(me, false, false)
            pCVar5 = quest:GetHero()
            quest:AddPersonToConversation(iVar7, pCVar5)
            pCVar5 = quest:GetHero()
            quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_HELP", me, pCVar5, false)
            quest:SetTimer(ctr_90, 0xf)
        end
        bVar3 = me:IsTalkedToByHero()
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                xStack_78 = resources:StartMovie("")
                quest:StartMovieSequence()
                pCVar5 = 0x1
                quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                if not __native_entity_state:GetStateBool("HaveChatted") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        __native_entity_state:SetStateBool("HaveChatted", true)
                        x_stk_68 = resources:ScriptThing(xStack_8c)
                        pCVar15 = x_stk_68
                        fret_0 = quest:GetHealth(pCVar15)
                        fVar2 = 0.0
                        x_stk_68 = nil
                        if fVar2 < fret_0 then
                            iVar13 = 0
                            iVar12 = 1
                            iVar7 = 0
                            iVar6 = 0
                            pcVar11 = "TEXT_QST_028_BIRD_KILLER_GREET"
                            pCVar15 = quest:GetHero()
                            r2 = me:Speak(pCVar15, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then __cleanup_LAB_00d4e91f(); return end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4e978 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar6 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar6 == 1 then
                                if not bVar3 then
                                    __native_entity_state:SetStateInt("BirdMode", 1)
                                    bVar3 = false
                                    if bVar3 ~= 0 then
                                    end
                                    iVar7 = 4
                                    pCVar9 = 0
                                    pCVar5 = quest:GetHero()
                                    bVar3 = resources:TryAcquire(pCVar9, pCVar5, iVar7)
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            resources:ReleaseResource(0)
                                            -- LAB_00d4e91f_c2: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(xStack_8c)
                                            return
                                        end
                                        iVar7 = 4
                                        pCVar9 = 0
                                        pCVar5 = quest:GetHero()
                                        bVar3 = resources:TryAcquire(pCVar9, pCVar5, iVar7)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        r1 = resources:NewActorMap()
                                        resources:SetActor(r1, "HERO", 0)
                                        resources:SetActor(r1, "ME", xStack_8c)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", r1, false, true)
                                        resources:DestroyActorMap(r1)
                                        resources:ReleaseResource(0)
                                        __region_LAB_00d4e853_c2(); goto LAB_00d4e87a
                                    end
                                    resources:ReleaseResource(0)
                                end
                                goto LAB_00d4e978
                            end
                            if not bVar3 then
                                x_stk_8 = resources:ScriptThing(xStack_8c)
                                pCVar15 = x_stk_8
                                fret_00 = quest:GetHealth(pCVar15)
                                fVar2 = 0.0
                                if fVar2 < fret_00 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar7 = 0
                                    iVar6 = 0
                                    pcVar11 = "TEXT_QST_028_BIRD_KILLER_REFUSE"
                                    pCVar5 = quest:GetHero()
                                    r3 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(xStack_8c)
                                            return
                                        end
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar4 = iVar6
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d4e9e1_c2 end
                                end
                                __region_LAB_00d4e853_c2()
                                goto LAB_00d4e87a
                            end
                        end
                        ::LAB_00d4e9e1_c2::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_7c)
                        goto FLOW_after_lab_00d4e5e3
                    end
                    ::LAB_00d4e978::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_78)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        -- LAB_00d4e5e3: (native jump target)
                        iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar6 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar6 == 1 then
                                if not bVar3 then
                                    __native_entity_state:SetStateInt("BirdMode", 1)
                                    bVar3 = false
                                    if bVar3 ~= 0 then
                                    end
                                    iVar7 = 4
                                    pCVar9 = 0
                                    pCVar5 = quest:GetHero()
                                    bVar3 = resources:TryAcquire(pCVar9, pCVar5, iVar7)
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            resources:ReleaseResource(0)
                                            __cleanup_LAB_00d4e91f()
                                            return
                                        end
                                        iVar7 = 4
                                        pCVar9 = 0
                                        pCVar5 = quest:GetHero()
                                        bVar3 = resources:TryAcquire(pCVar9, pCVar5, iVar7)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        r1 = resources:NewActorMap()
                                        resources:SetActor(r1, "HERO", 0)
                                        resources:SetActor(r1, "ME", xStack_8c)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", r1, false, true)
                                        resources:DestroyActorMap(r1)
                                        resources:ReleaseResource(0)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_78)
                                        goto LAB_00d4e87a
                                    end
                                    resources:ReleaseResource(0)
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_78)
                                goto FLOW_after_lab_00d4e5e3
                            end
                            if not bVar3 then
                                x_stk_8 = resources:ScriptThing(xStack_8c)
                                pCVar15 = x_stk_8
                                fret_00 = quest:GetHealth(pCVar15)
                                fVar2 = 0.0
                                if fVar2 < fret_00 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar7 = 0
                                    iVar6 = 0
                                    pcVar11 = "TEXT_QST_028_BIRD_KILLER_REFUSE"
                                    pCVar5 = quest:GetHero()
                                    r4 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(xStack_8c)
                                            return
                                        end
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar4 = iVar6
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d4e9e1 end
                                end
                                -- LAB_00d4e853: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_78)
                                goto LAB_00d4e87a
                            end
                        end
                    end
                    ::LAB_00d4e9e1::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_7c)
                end
                ::FLOW_after_lab_00d4e5e3::
            end
            goto LAB_00d4ef87
        end
        ::LAB_00d4e87a::
        timerId = ctr_90
        iVar6 = __native_entity_state:GetStateInt("BirdMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        iVar6 = __native_entity_state:GetStateInt("BirdMode")
        while iVar6 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d4ef87 end
            fVar14 = 5.5
            pCVar5 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar14)
            __native_condition_2 = bVar3
            if __native_condition_2 then
                iVar6 = quest:GetTimer(ctr_90)
                __native_condition_2 = iVar6 < 1
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4ef87 end
                if __native_entity_state:GetStateInt("CurrentBirds") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_ANY", me, pCVar5, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, pCVar5, false)
                end
                quest:SetTimer(ctr_90, 0xf)
            end
            bVar3 = me:IsTalkedToByHero()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4ef87 end
                iVar6 = quest:GetStateInt("CurrentBirdsKilled")
                if iVar6 == 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_ONE", me, pCVar5, false)
                    quest:Pause(1.0)
                    iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xefc)))
                    quest:GiveHeroGold(iVar7)
                elseif iVar6 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_NONE", me, pCVar5, false)
                    quest:Pause(1.0)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_MORE", me, pCVar5, false)
                    quest:Pause(1.0)
                    iVar7 = math.tointeger(math.modf(quest:GetStateInt("CurrentBirdsKilled") * quest:ReadGlobalGameDataFloat(0xefc)))
                    quest:GiveHeroGold(iVar7)
                end
                __native_entity_state:SetStateInt("CurrentBirds", __native_entity_state:GetStateInt("CurrentBirds") + quest:GetStateInt("CurrentBirdsKilled"))
                if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4ef87 end
                    quest:SetStateInt("CurrentBirdsKilled", 0)
                    iVar6 = quest:AddNewConversation(me, false, false)
                    if __native_entity_state:GetStateInt("CurrentBirds") == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4ef87 end
                        xStack_38 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        pCVar15 = 0x1
                        quest:PauseAllNonScriptedEntities((pCVar15 ~= 0))
                        x_stk_c = resources:ScriptThing(xStack_8c)
                        pCVar5 = x_stk_c
                        fret_01 = quest:GetHealth(pCVar5)
                        fVar2 = 0.0
                        if fVar2 < fret_01 then
                            iVar13 = 0
                            iVar12 = 1
                            iVar7 = 0
                            iVar6 = 0
                            pcVar11 = "TEXT_QST_028_BIRD_KILLER_DONE"
                            pCVar5 = quest:GetHero()
                            r5 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_38)
                                    goto LAB_00d4ef87
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_38)
                                goto LAB_00d4ef87
                            end
                        end
                        iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf00)))
                        quest:GiveHeroGold(iVar7)
                        __native_entity_state:SetStateInt("BirdMode", 3)
                        quest:ClearThingHasInformation(me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_38)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4ef87 end
                        pCVar5 = quest:GetHero()
                        quest:AddPersonToConversation(fVar2, pCVar5)
                        pCVar5 = quest:GetHero()
                        quest:AddLineToConversation(false, iVar6, me, pCVar5)
                        quest:Pause(1.0)
                    end
                end
            end
            iVar6 = __native_entity_state:GetStateInt("BirdMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            while not bVar3 do
                fVar14 = 5.5
                pCVar5 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar14)
                __native_condition_3 = bVar3
                if __native_condition_3 then
                    iVar6 = quest:GetTimer(ctr_90)
                    __native_condition_3 = iVar6 < 1
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    iVar7 = quest:AddNewConversation(me, false, false)
                    pCVar5 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar5)
                    pCVar5 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, pCVar5, false)
                    quest:SetTimer(ctr_90, 0xf)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            end
        end
    end
    ::LAB_00d4ef87::
    quest:DeregisterTimer(timerId)
    ::LAB_00d4ef90::
    resources:ReleaseResource(xStack_8c)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HaveChatted", false)
    __native_entity_state:SetStateInt("BirdMode", 0)
    __native_entity_state:SetStateInt("CurrentBirds", 0)
    quest:SetThingPersistent(me, true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

