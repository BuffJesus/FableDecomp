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
    local bVar5, cVar6, c_stk_85, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar10, iVar11, iVar12, iVar13, p0, pCVar7, pcVar9, r1, r2, r3, r4, r5, r6, r7, this_00, uVar4, uVar8, u_stk_8c, xStack_58, xStack_84, xStack_9c, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_9c = resources:NewResource()
        quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
        quest:EntitySetAsToAddToStatChangesWhenHit(me, false)
        quest:SetIsPushableByHero(me, false)
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, false, false)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while true do
            if bVar5 then
                resources:ReleaseResource(xStack_9c)
                return
            end
            bVar5 = me:IsTalkedToByHero()
            if bVar5 then break end
            -- LAB_00e17e8f: (native jump target)
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then
                goto LAB_00e17f13
            else
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00e17f13 end
                end
                goto LAB_00e17f21
            end
            goto FLOW_past_lab_00e17f13
            ::LAB_00e17f13::
            c_stk_85 = 1
            if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21 end
            ::FLOW_past_lab_00e17f13::
            goto FLOW_past_lab_00e17f21
            ::LAB_00e17f21::
            c_stk_85 = 0
            ::FLOW_past_lab_00e17f21::
            if c_stk_85 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        resources:PrepareResource(xStack_9c)
                        bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                        while not bVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e18203 end
                            bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            xStack_58 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_18 = resources:ScriptThing(xStack_9c)
                            pCVar7 = x_stk_18
                            fret_04 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_04 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar11 = 0
                                iVar10 = 0
                                pcVar9 = "TEXT_QST_074_CHIEF_BEEN_ATTACKED"
                                pCVar7 = quest:GetHero()
                                r1 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00e181f6
                                    end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e181f6
                                end
                                goto FLOW_past_lab_00e181f6
                                ::LAB_00e181f6::
                                this_00 = xStack_58
                                goto LAB_00e181fa
                                ::FLOW_past_lab_00e181f6::
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_58)
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                me:SetFriendsWithEverythingFlag(true)
                                resources:PrepareResource(xStack_9c)
                                goto LAB_00e18135
                            end
                        end
                    end
                end
                goto LAB_00e18203
            end
            ::LAB_00e18135::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        end
        ::FLOW_after_lab_00e17e8f::
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            resources:PrepareResource(xStack_9c)
            bVar5 = resources:TryAcquire(xStack_9c, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e18203 end
                bVar5 = resources:TryAcquire(xStack_9c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                xStack_84 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                iVar10 = quest:GetStateInt("BalverineState")
                if iVar10 ~= 2 then
                    if iVar10 == 3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_84
                            goto LAB_00e181fa
                        end
                        x_stk_3c = resources:ScriptThing(xStack_9c)
                        pCVar7 = x_stk_3c
                        fret_00 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar13 = 0
                            iVar12 = 1
                            iVar11 = 0
                            iVar10 = 0
                            pcVar9 = "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK"
                            pCVar7 = quest:GetHero()
                            r2 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                            iVar10 = me:IsPerformingScriptTask()
                            cVar6 = iVar10
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e181ca end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e17bb4: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_84
                                goto LAB_00e181fa
                            end
                        end
                    else
                        if iVar10 == 4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e181ca end
                            x_stk_48 = resources:ScriptThing(xStack_9c)
                            pCVar7 = x_stk_48
                            fret_01 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_01 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar11 = 0
                                iVar10 = 0
                                pcVar9 = "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK"
                                pCVar7 = quest:GetHero()
                                r3 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e181ca end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                end
                                goto LAB_00e17e55
                            end
                            goto LAB_00e17e64
                        end
                        if iVar10 == 6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                x_stk_c = resources:ScriptThing(xStack_9c)
                                pCVar7 = x_stk_c
                                fret_02 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_02 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar9 = "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK"
                                    pCVar7 = quest:GetHero()
                                    r4 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e181ca end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar6 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        -- LAB_00e17d43: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        this_00 = xStack_84
                                        goto LAB_00e181fa
                                    end
                                end
                                goto LAB_00e17e64
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = xStack_84
                            goto LAB_00e181fa
                        end
                        if quest:GetStateInt("BalverineState") == 7 then
                            bVar5 = quest:IsQuestActive("Q_WhiteBalverineWW")
                            if bVar5 then goto LAB_00e17d91 end
                            bVar5 = true
                        else
                            goto LAB_00e17d91
                        end
                        goto FLOW_past_lab_00e17d91
                        ::LAB_00e17d91::
                        bVar5 = false
                        ::FLOW_past_lab_00e17d91::
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e181ca end
                            x_stk_30 = resources:ScriptThing(xStack_9c)
                            pCVar7 = x_stk_30
                            fret_03 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_03 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar11 = 0
                                iVar10 = 0
                                pcVar9 = "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK"
                                pCVar7 = quest:GetHero()
                                r5 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e181ca end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                end
                                goto LAB_00e17e55
                            end
                        end
                    end
                    goto LAB_00e17e64
                end
                goto FLOW_past_lab_00e17e64
                ::LAB_00e17e64::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_84)
                resources:PrepareResource(xStack_9c)
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00e17f13_c1
                end
                goto FLOW_past_lab_00e17f13_c1
                ::LAB_00e17f13_c1::
                c_stk_85 = 1
                if quest:GetStateInt("BalverineState") == 7 then goto LAB_00e17f21_c1 end
                ::FLOW_past_lab_00e17f13_c1::
                goto FLOW_past_lab_00e17f21_c1
                ::LAB_00e17f21_c1::
                c_stk_85 = 0
                ::FLOW_past_lab_00e17f21_c1::
                if c_stk_85 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            resources:PrepareResource(xStack_9c)
                            bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                            while not bVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e18203 end
                                bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                xStack_58 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_18 = resources:ScriptThing(xStack_9c)
                                pCVar7 = x_stk_18
                                fret_04 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_04 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar9 = "TEXT_QST_074_CHIEF_BEEN_ATTACKED"
                                    pCVar7 = quest:GetHero()
                                    r6 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00e181f6_c1
                                        end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar6 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00e181f6_c1
                                    end
                                    goto FLOW_past_lab_00e181f6_c1
                                    ::LAB_00e181f6_c1::
                                    this_00 = xStack_58
                                    goto LAB_00e181fa
                                    ::FLOW_past_lab_00e181f6_c1::
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_58)
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    me:SetFriendsWithEverythingFlag(true)
                                    resources:PrepareResource(xStack_9c)
                                    goto LAB_00e18135_c1
                                end
                            end
                        end
                    end
                    goto LAB_00e18203
                end
                ::LAB_00e18135_c1::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                goto FLOW_after_lab_00e17e8f
                ::FLOW_past_lab_00e17e64::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e181a8: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_84
                    goto LAB_00e181fa
                end
                x_stk_24 = resources:ScriptThing(xStack_9c)
                pCVar7 = x_stk_24
                fret_0 = quest:GetHealth(pCVar7)
                fVar3 = 0.0
                if fret_0 <= fVar3 then goto LAB_00e17e64 end
                iVar13 = 0
                iVar12 = 1
                iVar11 = 0
                iVar10 = 0
                pcVar9 = "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK"
                pCVar7 = quest:GetHero()
                r7 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                iVar10 = me:IsPerformingScriptTask()
                cVar6 = iVar10
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e181ca end
                    iVar10 = me:IsPerformingScriptTask()
                    cVar6 = iVar10
                end
                ::LAB_00e17e55::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then goto LAB_00e17e64 end
                ::LAB_00e181ca::
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_84
                goto LAB_00e181fa
            end
        end
        goto FLOW_past_lab_00e181fa
        ::LAB_00e181fa::
        resources:DestroyMovie(this_00)
        ::FLOW_past_lab_00e181fa::
        ::LAB_00e18203::
        resources:ReleaseResource(xStack_9c)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

