-- Generated native draft: Gate1GuardOuter. Review coverage report before use.
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
    local bVar4, bVar5, cVar6, c_stk_111, c_stk_145, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, iVar15, iVar16, iVar7, iVar8, i_stk_14c, pCVar10, pCVar11, pCVar9, pQuestName, pcVar14, r1, r10, r11, r12, r13, r2, r3, r4, r5, r6, r7, r8, r9, xStack_10c, xStack_134, xStack_144, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_78, x_stk_84, x_stk_90, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_144 = resources:NewResource()
        resources:PrepareResource(xStack_144)
        bVar4 = resources:TryAcquire(xStack_144, me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d02a56 end
            bVar4 = resources:TryAcquire(xStack_144, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            r1 = quest:GetThingWithScriptName("Gate1GuardInner")
            iVar7 = quest:RegisterTimer()
            i_stk_14c = iVar7
            quest:SetTimer(i_stk_14c, 0)
            c_stk_111 = 0
            c_stk_145 = 0
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                goto LAB_00d01776
            end
            goto FLOW_hoist_lab_00d01776_1
        end
        goto FLOW_hoist_lab_00d01776_2
    end
    goto FLOW_past_lab_00d01776
    ::LAB_00d01776::
    if not quest:GetStateBool("Gate1Open") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            iVar8 = quest:GetTimer(i_stk_14c)
            if iVar8 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d02851 end
                bVar4 = false
                pCVar9 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar9, bVar4)
                iVar8 = quest:AddNewConversation(me, false, false)
                pCVar10 = quest:GetHero()
                quest:AddPersonToConversation(iVar8, pCVar10)
                if c_stk_111 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d02851 end
                    c_stk_111 = 1
                    -- TODO(native): pCVar11 = *(this + 4)
                    pCVar11 = nil --[[unresolved native value]]
                    -- TODO(native): pCVar10 = (**(*pCVar11 + 0x118))(pCVar11)
                    pCVar10 = nil --[[unresolved native value]]
                    quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT1_CALL_OVER_FIRST", me, r1, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d02851 end
                    -- TODO(native): pCVar11 = *(this + 4)
                    pCVar11 = nil --[[unresolved native value]]
                    -- TODO(native): pCVar10 = (**(*pCVar11 + 0x118))(pCVar11)
                    pCVar10 = nil --[[unresolved native value]]
                    quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT1_CALL_OVER_SECOND", me, nil --[[missing]], false)
                end
                quest:SetTimer(i_stk_14c, 10)
            end
            bVar4 = me:IsTalkedToByHero()
            if not bVar4 then goto LAB_00d0274f end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                xStack_134 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                pCVar10 = quest:GetHero()
                bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_SHIRT_BANDITCAMP")
                if bVar4 then
                    goto LAB_00d01abe
                else
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_TROUSERS_BANDITCAMP")
                    if bVar4 then goto LAB_00d01abe end
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_HAT_BANDITCAMP")
                    if bVar4 then goto LAB_00d01abe end
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_BOOTS_BANDITCAMP")
                    if bVar4 then goto LAB_00d01abe end
                    pCVar10 = quest:GetHero()
                    bVar5 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_GLOVES_BANDITCAMP")
                    bVar4 = true
                    if bVar5 then goto LAB_00d01abe end
                end
                goto FLOW_past_lab_00d01abe
                ::LAB_00d01abe::
                bVar4 = false
                ::FLOW_past_lab_00d01abe::
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d0287c end
                    x_stk_3c = resources:ScriptThing(xStack_144)
                    pCVar10 = x_stk_3c
                    fret_0 = quest:GetHealth(pCVar10)
                    fVar3 = 0.0
                    if fVar3 < fret_0 then
                        iVar16 = 0
                        iVar15 = 1
                        iVar8 = 0
                        iVar7 = 0
                        pcVar14 = "TEXT_QST_009_BANDIT1_NO_DISGUISE"
                        pCVar10 = quest:GetHero()
                        r2 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d0287c end
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                        end
                        goto LAB_00d01be9
                    end
                    goto FLOW_hoist_lab_00d01be9_1
                end
                goto FLOW_past_lab_00d01be9
                ::LAB_00d01be9::
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d0287c end
                ::FLOW_hoist_lab_00d01be9_1::
                ::LAB_00d01bf8::
                c_stk_145 = 1
                ::LAB_00d02592::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_134)
                goto LAB_00d0274f
                ::FLOW_past_lab_00d01be9::
                pCVar10 = quest:GetHero()
                bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_SHIRT_BANDITCAMP")
                if bVar4 then
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_TROUSERS_BANDITCAMP")
                    if not bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            x_stk_c = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_c
                            fret_01 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_01 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar8 = 0
                                iVar7 = 0
                                pcVar14 = "TEXT_QST_009_BANDIT1_NO_TROUSERS"
                                pCVar10 = quest:GetHero()
                                r3 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d0287c end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar6 = iVar7
                                end
                                goto LAB_00d01be9
                            end
                            goto LAB_00d01bf8
                        end
                        goto LAB_00d0287c
                    end
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_HAT_BANDITCAMP")
                    if not bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d0287c end
                        x_stk_48 = resources:ScriptThing(xStack_144)
                        pCVar10 = x_stk_48
                        fret_02 = quest:GetHealth(pCVar10)
                        fVar3 = 0.0
                        if fret_02 <= fVar3 then goto LAB_00d01bf8 end
                        iVar16 = 0
                        iVar15 = 1
                        iVar8 = 0
                        iVar7 = 0
                        pcVar14 = "TEXT_QST_009_BANDIT1_NO_HAT"
                        pCVar10 = quest:GetHero()
                        r4 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d0287c end
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d0287c end
                        c_stk_145 = 1
                        goto LAB_00d02592
                    end
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_BOOTS_BANDITCAMP")
                    if not bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            x_stk_78 = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_78
                            fret_03 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_03 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar8 = 0
                                iVar7 = 0
                                pcVar14 = "TEXT_QST_009_BANDIT1_NO_BOOTS"
                                pCVar10 = quest:GetHero()
                                r5 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d0287c end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar6 = iVar7
                                end
                                goto LAB_00d01be9
                            end
                            goto LAB_00d01bf8
                        end
                        goto LAB_00d0287c
                    end
                    pCVar10 = quest:GetHero()
                    bVar4 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_GLOVES_BANDITCAMP")
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar4 then
                        if not bVar5 then
                            x_stk_18 = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_18
                            fret_04 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fret_04 <= fVar3 then goto LAB_00d01bf8 end
                            iVar16 = 0
                            iVar15 = 1
                            iVar8 = 0
                            iVar7 = 0
                            pcVar14 = "TEXT_QST_009_BANDIT1_NO_GLOVES"
                            pCVar10 = quest:GetHero()
                            r6 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d0287c end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                c_stk_145 = 1
                                goto LAB_00d02592
                            end
                        end
                        goto LAB_00d0287c
                    end
                    if not bVar5 then
                        if c_stk_145 == 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d0287c end
                            x_stk_60 = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_60
                            fret_05 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_05 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar8 = 0
                                iVar7 = 0
                                pcVar14 = "TEXT_QST_009_BANDIT1_TRIED_AND_IN"
                                pCVar10 = quest:GetHero()
                                r7 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d0287c end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar6 = iVar7
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d0287c end
                            end
                            goto LAB_00d022ba
                        end
                        goto FLOW_past_lab_00d022ba
                        ::LAB_00d022ba::
                        pCVar10 = quest:GetHero()
                        r8 = quest:GetNearestWithDefName(pCVar10, "VILLAGE_BANDIT_CAMP_MAIN")
                        bVar4 = quest:IsSleepingTime(r8)
                        if not bVar4 then
                            goto LAB_00d023d3
                        end
                        goto FLOW_past_lab_00d023d3
                        ::LAB_00d023d3::
                        x_stk_84 = resources:ScriptThing(xStack_144)
                        pCVar10 = x_stk_84
                        fret_08 = quest:GetHealth(pCVar10)
                        fVar3 = 0.0
                        if fVar3 < fret_08 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar8 = 0
                            iVar7 = 0
                            pcVar14 = "TEXT_QST_009_BANDIT1_GATE_OPEN"
                            pCVar10 = quest:GetHero()
                            r9 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d02870 end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d028a8 end
                        end
                        pCVar10 = quest:GetThingWithScriptName("Gate1")
                        quest:OpenDoor(pCVar10)
                        pQuestName = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05", "BanditCampBoss", "BanditCampEntrance")
                        quest:SetStateBool("Gate1Open", true)
                        quest:GiveHeroExperience(quest:ReadGlobalGameData(0x40))
                        quest:ClearThingHasInformation(me)
                        goto LAB_00d02592
                        ::FLOW_past_lab_00d023d3::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            x_stk_90 = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_90
                            fret_07 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_07 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar8 = 0
                                iVar7 = 0
                                pcVar14 = "TEXT_QST_009_BANDIT1_BANDITS_SLEEPING"
                                pCVar10 = quest:GetHero()
                                r10 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d02870 end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar6 = iVar7
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d028a8 end
                            end
                            goto LAB_00d023d3
                        end
                        ::LAB_00d028a8::
                        goto LAB_00d0287c
                        ::FLOW_past_lab_00d022ba::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            x_stk_30 = resources:ScriptThing(xStack_144)
                            pCVar10 = x_stk_30
                            fret_06 = quest:GetHealth(pCVar10)
                            fVar3 = 0.0
                            if fVar3 < fret_06 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar8 = 0
                                iVar7 = 0
                                pcVar14 = "TEXT_QST_009_BANDIT1_STRAIGHT_IN"
                                pCVar10 = quest:GetHero()
                                r11 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar6 = iVar7
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d0287c end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar6 = iVar7
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d0287c end
                            end
                            goto LAB_00d022ba
                        end
                    end
                    goto LAB_00d0287c
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    x_stk_24 = resources:ScriptThing(xStack_144)
                    pCVar10 = x_stk_24
                    fret_00 = quest:GetHealth(pCVar10)
                    fVar3 = 0.0
                    if fVar3 < fret_00 then
                        iVar16 = 0
                        iVar15 = 1
                        iVar8 = 0
                        iVar7 = 0
                        pcVar14 = "TEXT_QST_009_BANDIT1_NO_SHIRT"
                        pCVar10 = quest:GetHero()
                        r12 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar7 = me:IsPerformingScriptTask()
                        cVar6 = iVar7
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d0287c end
                            iVar7 = me:IsPerformingScriptTask()
                            cVar6 = iVar7
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d0287c end
                    end
                    c_stk_145 = 1
                    goto LAB_00d02592
                end
                -- LAB_00d028b4: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00d02889
            end
            goto LAB_00d02851
        end
        quest:DeregisterTimer(i_stk_14c)
        goto LAB_00d02a4d
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        bVar4 = pCVar10:IsTalkedToByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d02851 end
            xStack_10c = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            x_stk_54 = resources:ScriptThing(xStack_144)
            pCVar10 = x_stk_54
            fret_09 = quest:GetHealth(pCVar10)
            fVar3 = 0.0
            if fVar3 < fret_09 then
                iVar16 = 0
                iVar15 = 1
                iVar8 = 0
                iVar7 = 0
                pcVar14 = "TEXT_QST_009_BANDIT1_ASIDE"
                pCVar10 = quest:GetHero()
                r13 = me:Speak(pCVar10, pcVar14, iVar7, (iVar8 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                iVar7 = me:IsPerformingScriptTask()
                cVar6 = iVar7
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10c)
                        goto LAB_00d02851
                    end
                    iVar7 = me:IsPerformingScriptTask()
                    cVar6 = iVar7
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d028dd: (native jump target)
                    resources:DestroyMovie(xStack_10c)
                    goto LAB_00d02851
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10c)
            iVar7 = i_stk_14c
        end
        goto LAB_00d0274f
    end
    goto FLOW_past_lab_00d0274f
    ::LAB_00d0274f::
    cVar6 = me:MsgIsHitByHero()
    if not cVar6 then
        bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
        if bVar4 then
            bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
            if not bVar4 then goto LAB_00d027e0 end
        end
        bVar4 = false
    else
        goto LAB_00d027e0
    end
    goto FLOW_past_lab_00d027e0
    ::LAB_00d027e0::
    bVar4 = true
    ::FLOW_past_lab_00d027e0::
    if not bVar4 then
        -- TODO(native): if (*(char *)(*(int *)(this + 0x14) + 0x48) == '\0') goto code_r0x00d0283a;
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            pCVar9 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pCVar9)
            resources:PrepareResource(xStack_144)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
            until not (not bVar4)
        end
        goto LAB_00d02851
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00d02851 end
    fret_10 = quest:GetHealth(me)
    if 0.0 < fret_10 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00d02851 end
        iVar8 = quest:AddNewConversation(me, false, false)
        pCVar9 = quest:GetHero()
        quest:AddPersonToConversation(iVar8, pCVar9)
        pCVar9 = quest:GetHero()
        quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT1_ATTACKED_NEW", me, pCVar9, false)
    end
    quest:SetStateBool("AttackedOuterGateGuards", true)
    pCVar9 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pCVar9)
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(xStack_144)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    until not (not bVar4)
    -- LAB_00d02a44: (native jump target)
    quest:DeregisterTimer(i_stk_14c)
    goto LAB_00d02a4d
    ::FLOW_past_lab_00d0274f::
    ::FLOW_hoist_lab_00d01776_1::
    ::LAB_00d02851::
    quest:DeregisterTimer(i_stk_14c)
    ::LAB_00d02a4d::
    ::FLOW_hoist_lab_00d01776_2::
    ::LAB_00d02a56::
    resources:ReleaseResource(xStack_144)
    ::FLOW_past_lab_00d01776::
    do return end
    ::LAB_00d02870::
    ::LAB_00d0287c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d02889::
    resources:DestroyMovie(xStack_134)
    goto LAB_00d02851
    -- TODO(native): code_r0x00d0283a:
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00d02851 end
    goto LAB_00d01776
end

function Init(quest, me)
    __native_entity_state:SetStateInt("AIState", 0)
    if not quest:GetStateBool("Gate1Open") then
        quest:SetThingHasInformation(me, false, true, false)
    end
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
    quest:SetStateBool("AttackedOuterGateGuards", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

