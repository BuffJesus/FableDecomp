-- Generated native draft: TalkingTrader1. Review coverage report before use.
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
    local CVar4, bVar6, cVar7, c_stk_d1, dist, fVar3, fret_0, fret_00, fret_01, fret_02, iVar10, iVar15, iVar16, iVar8, i_stk_a8, i_stk_dc, native_arg_sequence_1, native_arg_switch_1, p0, pCVar12, pCVar9, pcVar14, pvVar11, r1, r2, r3, r4, this_00, uStack_b0, uVar13, uVar5, xStack_34, xStack_44, xStack_64, xStack_b4, xStack_c4, xStack_d8, x_stk_18, x_stk_24, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        xStack_c4 = resources:NewResource()
        resources:PrepareResource(xStack_c4)
        bVar6 = resources:TryAcquire(xStack_c4, me, 4)
        while not bVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_c4)
                return
            end
            bVar6 = resources:TryAcquire(xStack_c4, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            -- LAB_00ecbf3a: (native jump target)
            resources:ReleaseResource(xStack_c4)
            return
        end
        r1 = quest:GetThingWithScriptName("TalkingTrader2")
        i_stk_dc = quest:RegisterTimer()
        i_stk_a8 = 10
        if quest:GetStateBool("GotFishingSpotMushroom") then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                quest:DeregisterTimer(i_stk_dc)
                resources:ReleaseResource(xStack_c4)
                return
            end
            quest:RemoveThing(me, false, true)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        CVar4 = 0x0
        while not bVar6 do
            iVar8 = quest:GetTimer(i_stk_dc)
            if iVar8 == 0 then
                dist = 20.0
                pCVar9 = quest:GetHero()
                bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, dist)
                native_arg_sequence_1 = false
                if not bVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    bVar6 = quest:IsDistanceBetweenThingsUnder(me, r1, 10.0)
                    if not bVar6 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00ecc220 end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    iVar8 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(iVar8, r1)
                    native_arg_switch_1 = CVar4
                    repeat
                        if native_arg_switch_1 == 0 then
                            quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERA_CHAT1_10", me, r1, false)
                            quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERB_CHAT1_20", r1, me, false)
                            break
                        else
                            if native_arg_switch_1 == 1 then
                                quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERA_CHAT2_10", pCVar9, nil --[[missing]])
                                quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERB_CHAT2_20", me, nil --[[missing]], false)
                                break
                            else
                                if native_arg_switch_1 == 2 then
                                    quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERA_CHAT3_10", nil --[[missing]], nil --[[missing]])
                                    quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERB_CHAT3_20", me, nil --[[missing]], false)
                                    quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERA_CHAT3_30", me, nil --[[missing]], false)
                                    break
                                else
                                    if native_arg_switch_1 == 3 then
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERA_CHAT4_10", nil --[[missing]], nil --[[missing]])
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_B10_TRADERB_CHAT4_20", me, nil --[[missing]], false)
                                        goto LAB_00ecc1f4
                                    else
                                        if 2 < CVar4 then goto LAB_00ecc1f4 end
                                    end
                                    goto FLOW_past_lab_00ecc1f4
                                    ::LAB_00ecc1f4::
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00ecc928 end
                                    goto LAB_00ecc20b
                                    ::FLOW_past_lab_00ecc1f4::
                                end
                            end
                        end
                    until not (false)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        -- TODO(native): xStack_ac = (CCharString)((int)CVar4 + 1);
                        goto LAB_00ecc20b
                    end
                    goto FLOW_past_lab_00ecc20b
                    ::LAB_00ecc20b::
                    quest:SetTimer(i_stk_dc, 0x10)
                    goto LAB_00ecc220
                    ::FLOW_past_lab_00ecc20b::
                end
                goto LAB_00ecc928
            end
            ::LAB_00ecc220::
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    iVar8 = __native_entity_state:GetStateInt("TimesTalkedTo")
                    if iVar8 == 0 then
                        pcVar14 = "TEXT_QST_B10_TRADERA_INTRO"
                        goto LAB_00ecc2be
                    else
                        if iVar8 == 1 then
                            pcVar14 = "TEXT_QST_B10_TRADERA_REPEATA"
                            goto LAB_00ecc2be
                        end
                        if iVar8 == 2 then
                            -- TODO(native): CCharString::operator=(&xStack_d8,"TEXT_QST_B10_TRADERA_REPEATB_10");
                            __native_entity_state:SetStateInt("TimesTalkedTo", 0xffffffff)
                        end
                    end
                    goto FLOW_past_lab_00ecc2be
                    ::LAB_00ecc2be::
                    xStack_d8 = pcVar14
                    ::FLOW_past_lab_00ecc2be::
                    iVar8 = i_stk_dc
                    __native_entity_state:SetStateInt("TimesTalkedTo", __native_entity_state:GetStateInt("TimesTalkedTo") + 1)
                    iVar10 = quest:GetTimer(i_stk_dc)
                    quest:SetTimer(iVar8, iVar10 + 5)
                    iVar8 = (r1 ~= nil and r1:IsAlive())
                    if not iVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00ecc8f3 end
                        xStack_44 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_c = resources:ScriptThing(xStack_c4)
                        pCVar9 = x_stk_c
                        fret_00 = quest:GetHealth(pCVar9)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar10 = 0
                            iVar8 = 0
                            pcVar14 = "TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10"
                            pCVar9 = quest:GetHero()
                            r2 = me:Speak(pCVar9, pcVar14, iVar8, (iVar10 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar8 = me:IsPerformingScriptTask()
                            cVar7 = iVar8
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_44)
                                    goto LAB_00ecc923
                                end
                                iVar8 = me:IsPerformingScriptTask()
                                cVar7 = iVar8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_44)
                                goto LAB_00ecc8f3
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_44
                        goto LAB_00ecc53c
                    end
                    goto FLOW_past_lab_00ecc53c
                    ::LAB_00ecc53c::
                    resources:DestroyMovie(this_00)
                    goto LAB_00ecc54a
                    ::FLOW_past_lab_00ecc53c::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        xStack_34 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_18 = resources:ScriptThing(xStack_c4)
                        pCVar9 = x_stk_18
                        fret_0 = quest:GetHealth(pCVar9)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar10 = 0
                            iVar8 = 0
                            pvVar11 = xStack_d8
                            pCVar9 = quest:GetHero()
                            r3 = me:Speak(pCVar9, pvVar11, iVar8, (iVar10 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar8 = me:IsPerformingScriptTask()
                            cVar7 = iVar8
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_34)
                                    goto LAB_00ecc923
                                end
                                iVar8 = me:IsPerformingScriptTask()
                                cVar7 = iVar8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_34)
                                goto LAB_00ecc923
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_34
                        goto LAB_00ecc53c
                    end
                    ::LAB_00ecc8f3::
                    goto LAB_00ecc923
                end
                goto FLOW_hoist_lab_00ecc923_1
            end
            goto FLOW_past_lab_00ecc923
            ::LAB_00ecc923::
            goto LAB_00ecc928
            ::FLOW_hoist_lab_00ecc923_1::
            goto FLOW_hoist_lab_00ecc928_1
            ::FLOW_past_lab_00ecc923::
            goto FLOW_past_lab_00ecc928
            ::LAB_00ecc928::
            quest:DeregisterTimer(i_stk_dc)
            goto LAB_00ecc931
            ::FLOW_hoist_lab_00ecc928_1::
            break
            ::FLOW_past_lab_00ecc928::
            ::LAB_00ecc54a::
            -- TODO(native): uStack_b0 = uStack_b0 | 1;
            cVar7 = me:MsgIsHitByHero()
            if not cVar7 then
                uVar13 = uVar5 | 3
                uStack_b0 = uVar13
                cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar7 then
                    uVar13 = uVar5 | 7
                    uStack_b0 = uVar13
                    cVar7 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                    if not cVar7 then goto LAB_00ecc5ce end
                end
                goto LAB_00ecc5ec
            else
                goto LAB_00ecc5ce
            end
            goto FLOW_past_lab_00ecc5ec
            ::LAB_00ecc5ec::
            c_stk_d1 = 0
            ::FLOW_past_lab_00ecc5ec::
            goto FLOW_past_lab_00ecc5ce
            ::LAB_00ecc5ce::
            fret_01 = quest:GetHealth(me)
            c_stk_d1 = 1
            if fret_01 <= 0.0 then goto LAB_00ecc5ec end
            ::FLOW_past_lab_00ecc5ce::
            if (uVar13 & 4) ~= 0 then
                uVar13 = uVar13 & 0xfffffffb
                uStack_b0 = uVar13
            end
            if (uVar13 & 2) ~= 0 then
                uVar13 = uVar13 & 0xfffffffd
                uStack_b0 = uVar13
            end
            if (uVar13 & 1) ~= 0 then
                -- TODO(native): uStack_b0 = uVar13 & 0xfffffffe;
            end
            if c_stk_d1 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00ecc928 end
                if 2 < __native_entity_state:GetStateInt("TimesHit") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00ecc928 end
                    quest:EntitySetAsKillable(me, true, true)
                    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
                end
                pCVar12 = tostring(i_stk_a8)
                pCVar12 = ("TEXT_QST_B10_TRADERA_ONHIT_" .. pCVar12)
                xStack_b4 = pCVar12
                bVar6 = quest:TextEntryExists(xStack_b4)
                if not bVar6 then
                    i_stk_a8 = 10
                    pCVar12 = tostring(10)
                    pCVar12 = ("TEXT_QST_B10_TRADERA_ONHIT_" .. pCVar12)
                    xStack_b4 = pCVar12
                end
                i_stk_a8 = i_stk_a8 + 0xa
                xStack_64 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_24 = resources:ScriptThing(xStack_c4)
                pCVar9 = x_stk_24
                fret_02 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_02 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar10 = 0
                    iVar8 = 0
                    pvVar11 = xStack_b4
                    pCVar9 = quest:GetHero()
                    r4 = me:Speak(pCVar9, pvVar11, iVar8, (iVar10 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar8 = me:IsPerformingScriptTask()
                    cVar7 = iVar8
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00ecc913
                        end
                        iVar8 = me:IsPerformingScriptTask()
                        cVar7 = iVar8
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00ecc913
                    end
                    goto FLOW_past_lab_00ecc913
                    ::LAB_00ecc913::
                    resources:DestroyMovie(xStack_64)
                    goto LAB_00ecc923
                    ::FLOW_past_lab_00ecc913::
                end
                __native_entity_state:SetStateInt("TimesHit", __native_entity_state:GetStateInt("TimesHit") + 1)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_64)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
        end
        quest:DeregisterTimer(i_stk_dc)
        ::LAB_00ecc931::
        resources:ReleaseResource(xStack_c4)
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

