-- Generated native draft: TalkingTrader2. Review coverage report before use.
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
    local __native_condition_1, bVar5, cVar6, c_stk_a9, ctr_90, fVar3, fret_0, fret_00, fret_01, fret_02, iVar13, iVar14, iVar15, iVar16, iVar9, pCVar10, pCVar7, pcVar12, pvVar8, r1, r2, r3, r4, this_00, uVar11, uVar4, u_stk_94, xStack_34, xStack_44, xStack_5a, xStack_98, xStack_a8, xStack_b0, x_stk_18, x_stk_1a, x_stk_8e, x_stk_c
    local alive = true
    u_stk_94 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_a8 = resources:NewResource()
        resources:PrepareResource(xStack_a8)
        bVar5 = resources:TryAcquire(xStack_a8, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_a8)
                return
            end
            bVar5 = resources:TryAcquire(xStack_a8, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            -- LAB_00eccbf6: (native jump target)
            resources:ReleaseResource(xStack_a8)
            return
        end
        r1 = quest:GetThingWithScriptName("TalkingTrader1")
        ctr_90 = 0xa
        if quest:GetStateBool("GotFishingSpotMushroom") then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_a8)
                return
            end
            quest:RemoveThing(me, false, true)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while not bVar5 do
            cVar6 = me:IsTalkedToByHero()
            if cVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    iVar9 = __native_entity_state:GetStateInt("TimesTalkedTo")
                    if iVar9 == 0 then
                        pcVar12 = "TEXT_QST_B10_TRADERB_INTRO"
                        goto LAB_00ecccaa
                    else
                        if iVar9 == 1 then
                            pcVar12 = "TEXT_QST_B10_TRADERB_REPEATA"
                            goto LAB_00ecccaa
                        end
                        if iVar9 == 2 then
                            -- TODO(native): CCharString::operator=(&xStack_b0,"TEXT_QST_B10_TRADERB_REPEATB_10");
                            __native_entity_state:SetStateInt("TimesTalkedTo", 0xffffffff)
                        end
                    end
                    goto FLOW_past_lab_00ecccaa
                    ::LAB_00ecccaa::
                    xStack_b0 = pcVar12
                    ::FLOW_past_lab_00ecccaa::
                    __native_entity_state:SetStateInt("TimesTalkedTo", __native_entity_state:GetStateInt("TimesTalkedTo") + 1)
                    __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
                    if not __native_condition_1 then
                        cVar6 = (r1 ~= nil and r1:IsAlive())
                        __native_condition_1 = not cVar6
                    end
                    if __native_condition_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ecd272 end
                        xStack_34 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_18 = resources:ScriptThing(xStack_a8)
                        pCVar7 = x_stk_18
                        fret_00 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar15 = 0
                            iVar14 = 1
                            iVar13 = 0
                            iVar9 = 0
                            pcVar12 = "TEXT_QST_B10_TRADERB_OTHERTRADERDEAD_10"
                            pCVar7 = quest:GetHero()
                            r2 = me:Speak(pCVar7, pcVar12, iVar9, (iVar13 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_34)
                                    goto LAB_00ecd29f
                                end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_34)
                                goto LAB_00ecd272
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_34
                        goto LAB_00eccefc
                    end
                    goto FLOW_past_lab_00eccefc
                    ::LAB_00eccefc::
                    resources:DestroyMovie(this_00)
                    goto LAB_00eccf0a
                    ::FLOW_past_lab_00eccefc::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_44 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_c = resources:ScriptThing(xStack_a8)
                        pCVar7 = x_stk_c
                        fret_0 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pvVar8 = xStack_b0
                            iVar9 = quest:GetHero()
                            r3 = me:Speak(iVar9, pvVar8, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_44)
                                    goto LAB_00ecd29f
                                end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_44)
                                goto LAB_00ecd29f
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_44
                        goto LAB_00eccefc
                    end
                    ::LAB_00ecd272::
                end
                goto FLOW_hoist_lab_00ecd29f_1
            end
            goto FLOW_past_lab_00ecd29f
            ::LAB_00ecd29f::
            ::FLOW_hoist_lab_00ecd29f_1::
            break
            ::FLOW_past_lab_00ecd29f::
            ::LAB_00eccf0a::
            uVar4 = u_stk_94
            u_stk_94 = u_stk_94 | 1
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                uVar11 = uVar4 | 3
                u_stk_94 = uVar11
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    uVar11 = uVar4 | 7
                    u_stk_94 = uVar11
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00eccf8f end
                end
                goto LAB_00eccfad
            else
                goto LAB_00eccf8f
            end
            goto FLOW_past_lab_00eccfad
            ::LAB_00eccfad::
            c_stk_a9 = 0
            ::FLOW_past_lab_00eccfad::
            goto FLOW_past_lab_00eccf8f
            ::LAB_00eccf8f::
            fret_01 = quest:GetHealth(me)
            c_stk_a9 = 1
            if fret_01 <= 0.0 then goto LAB_00eccfad end
            ::FLOW_past_lab_00eccf8f::
            if (uVar11 & 4) ~= 0 then
                uVar11 = uVar11 & 0xfffffffb
                u_stk_94 = uVar11
            end
            if (uVar11 & 2) ~= 0 then
                uVar11 = uVar11 & 0xfffffffd
                u_stk_94 = uVar11
            end
            if (uVar11 & 1) ~= 0 then
                u_stk_94 = uVar11 & 0xfffffffe
            end
            if c_stk_a9 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                if 2 < __native_entity_state:GetStateInt("TimesHit") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    quest:EntitySetAsKillable(me, true, true)
                    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
                end
                pCVar10 = tostring(ctr_90)
                pCVar10 = ("TEXT_QST_B10_TRADERB_ONHIT_" .. pCVar10)
                xStack_98 = pCVar10
                bVar5 = quest:TextEntryExists(xStack_98)
                if not bVar5 then
                    ctr_90 = 10
                    pCVar10 = tostring(10)
                    pCVar10 = ("TEXT_QST_B10_TRADERB_ONHIT_" .. pCVar10)
                    xStack_98 = pCVar10
                end
                ctr_90 = ctr_90 + 0xa
                xStack_5a = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_1a = resources:ScriptThing(xStack_a8)
                pCVar7 = x_stk_1a
                fret_02 = quest:GetHealth(pCVar7)
                fVar3 = 0.0
                if fVar3 < fret_02 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pvVar8 = x_stk_8e
                    iVar9 = quest:GetHero()
                    r4 = me:Speak(iVar9, pvVar8, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00ecd292
                        end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00ecd292
                    end
                    goto FLOW_past_lab_00ecd292
                    ::LAB_00ecd292::
                    resources:DestroyMovie(xStack_5a)
                    goto LAB_00ecd29f
                    ::FLOW_past_lab_00ecd292::
                end
                __native_entity_state:SetStateInt("TimesHit", __native_entity_state:GetStateInt("TimesHit") + 1)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_5a)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        end
        resources:ReleaseResource(xStack_a8)
    end
end

function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsAllowedToFollowHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    __native_entity_state:SetStateInt("TimesTalkedTo", 0)
    __native_entity_state:SetStateInt("TimesHit", 0)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

