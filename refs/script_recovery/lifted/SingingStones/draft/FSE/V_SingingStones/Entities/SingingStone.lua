-- Generated native draft: SingingStone. Review coverage report before use.
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
    local CVar6, CVar7, __native_condition_1, bVar2, ctr_40, ctr_4c, dist, iVar5, iVar8, iVar9, native_arg_switch_2, p0, pCVar3, pCVar4, pRelativeTo, piVar1, r1, xStack_4d, xStack_c
    local alive = true
    CVar6 = 0
    ctr_4c = 0
    r1 = quest:GetNearestWithScriptName(me, "SpeakMarker")
    ctr_40 = 0
    if 0 < quest:GetStateInt("CurrentPlayListIndex") then
        ctr_4c = 0x6c
        goto LAB_00ed33d0
    end
    goto FLOW_past_lab_00ed33d0
    ::LAB_00ed33d0::
    -- TODO(native): if *(__native_entity_state:GetStateInt("self_0x14") + ctr_4c) ~= __native_entity_state:GetStateInt("MyStoneNumber") then goto LAB_00ed34e2 end
    if false then goto LAB_00ed34e2 end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        repeat
            dist = 35.0
            pCVar3 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, dist)
            if bVar2 then
                pCVar4 = me:GetPos()
                bVar2 = quest:IsCameraPosOnScreen(pCVar4)
                if bVar2 then goto LAB_00ed344b end
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00ed3906 end
        until false
    end
    goto LAB_00ed38d9
    ::FLOW_past_lab_00ed33d0::
    ::LAB_00ed3504::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        iVar9 = 0
        CVar7 = CVar6 | 1
        ctr_4c = CVar7
        bVar2 = me:MsgIsHitByHero()
        if bVar2 then
            goto LAB_00ed3597
        else
            CVar7 = CVar6 | 3
            ctr_4c = CVar7
            bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar2 then
                CVar7 = CVar6 | 7
                ctr_4c = CVar7
                bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar2 then goto LAB_00ed3597 end
            end
            -- TODO(native): xStack_4d = '\0';
        end
        goto FLOW_past_lab_00ed3597
        ::LAB_00ed3597::
        -- TODO(native): xStack_4d = '\x01';
        ::FLOW_past_lab_00ed3597::
        if (CVar7 & 4) ~= 0 then
            CVar7 = CVar7 & 0xfffffffb
            ctr_4c = CVar7
        end
        if (CVar7 & 2) ~= 0 then
            CVar7 = CVar7 & 0xfffffffd
            ctr_4c = CVar7
        end
        if (CVar7 & 1) ~= 0 then
            CVar7 = CVar7 & 0xfffffffe
            ctr_4c = CVar7
        end
        CVar6 = CVar7
        if xStack_4d ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00ed38d9 end
            bVar2 = false
            if 0 < quest:GetStateInt("CurrentPlayListIndex") then
                iVar8 = 0x6c
                repeat
                    -- TODO(native): if *(__native_entity_state:GetStateInt("self_0x14") + iVar8) == __native_entity_state:GetStateInt("MyStoneNumber") then
                    if false then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            r1 = nil
                            goto LAB_00ed3906
                        end
                        bVar2 = true
                    end
                    iVar9 = iVar9 + 1
                    iVar8 = iVar8 + 4
                until not (iVar9 < quest:GetStateInt("CurrentPlayListIndex"))
                CVar6 = ctr_4c
                if bVar2 then goto LAB_00ed3899 end
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                r1 = nil
                -- LAB_00ed3977: (native jump target)
                return
            end
            iVar5 = quest:AddNewConversation(r1, true, true)
            pCVar3 = quest:GetHero()
            quest:AddPersonToConversation(iVar5, pCVar3)
            native_arg_switch_2 = __native_entity_state:GetStateInt("MyStoneNumber")
            repeat
                if native_arg_switch_2 == 0 then
                    pCVar3 = quest:GetHero()
                    quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_A", r1, pCVar3, false)
                    break
                else
                    if native_arg_switch_2 == 1 then
                        pCVar3 = quest:GetHero()
                        quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_B", r1, pCVar3, false)
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            pCVar3 = quest:GetHero()
                            quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_C", r1, pCVar3, false)
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                pCVar3 = quest:GetHero()
                                quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_D", r1, pCVar3, false)
                                break
                            else
                                goto FLOW_native_label_1
                            end
                        end
                    end
                end
            until not (false)
            ::FLOW_native_label_1::
            -- TODO(native): *(undefined4 *) (*(int *)(this + 0x14) + 0x6c + *(int *)(*(int *)(this + 0x14) + 0x48) * 4) = *(undefined4 *)(this + 0x1c);
            quest:SetStateInt("CurrentPlayListIndex", quest:GetStateInt("CurrentPlayListIndex") + 1)
            pCVar4 = me:GetPos()
            xStack_c = {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z + 1.0}
            pCVar3 = quest:CreateEffectAtPos("MARKTELEPORTER", xStack_c, 0.0, false)
            -- TODO(native): p0 = *(__native_entity_state:GetStateInt("self_0x14") + 0x80)
            p0 = nil --[[unresolved native value]]
            -- TODO(native): if p0 == *(__native_entity_state:GetStateInt("self_0x14") + 0x84) then
            if false then
                -- TODO(native): std__vector_InsertRange((void *)(*(int *)(this + 0x14) + 0x7c),(int)p0,(int)pCVar3,(int)&xStack_4d,1,1);
            else
                if p0 ~= nil then
                    -- TODO(native): p0[1] = *(undefined4 *)(pCVar3 + 0x4);
                    -- TODO(native): piVar1 = *(pCVar3 + 0x8)
                    piVar1 = nil --[[unresolved native value]]
                    -- TODO(native): p0[2] = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                -- TODO(native): (quest:GetStateListCount("EffectList") * 0xc) = (quest:GetStateListCount("EffectList") * 0xc) + 0xc;
            end
            pCVar3 = nil
            CVar6 = ctr_4c
        end
        ::LAB_00ed3899::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    r1 = nil
    __native_condition_1 = r1 == nil
    if not __native_condition_1 then
        -- TODO(native): *xStack_24 = *xStack_24 + -1;
        -- TODO(native): __native_condition_1 = *r1 ~= 0
        __native_condition_1 = nil --[[unresolved native value]]
    end
    if __native_condition_1 then goto LAB_00ed3906 end
    r1:GetName()
    -- LAB_00ed38fe: (native jump target)
    ::LAB_00ed3906::
    do return end
    ::LAB_00ed344b::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00ed343a: (native jump target)
        return
    end
    pCVar4 = me:GetPos()
    -- TODO(native): xStack_18 = (int *)(pCVar4.z + 1.0);
    -- TODO(native): xStack_18._4_4_ = pCVar4.y;
    -- TODO(native): xStack_18._0_4_ = pCVar4.x;
    pCVar3 = quest:CreateEffectAtPos("MARKTELEPORTER", pCVar4, 0.0, false, pCVar3)
    quest:StateListPush("EffectList", pCVar3)
    ::LAB_00ed34e2::
    ctr_4c = ctr_4c + 4
    ctr_40 = ctr_40 + 1
    if quest:GetStateInt("CurrentPlayListIndex") <= ctr_40 then goto LAB_00ed3504 end
    goto LAB_00ed33d0
    ::LAB_00ed38d9::
    r1 = nil
    goto LAB_00ed3906
end

function Init(quest, me)
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local this_00 = me:GetDataString()
    local iVar4 = parseGameInteger(this_00)
    __native_entity_state:SetStateInt("MyStoneNumber", iVar4)
    local pCVar5 = me:GetDataString()
    pCVar5 = ("M_WispMarker" .. pCVar5)
    local pCVar6 = quest:GetThingWithScriptName(pCVar5)
    __native_entity_state:SetStateThing("MyParticleEmitter", pCVar6)
    pCVar6 = nil
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    local native_arg_switch_1 = __native_entity_state:GetStateInt("MyStoneNumber")
    repeat
        if native_arg_switch_1 == 0 then
            quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_A")
            return
        else
            if native_arg_switch_1 == 1 then
                quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_B")
                return
            else
                if native_arg_switch_1 == 2 then
                    quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_C")
                    return
                else
                    if native_arg_switch_1 == 3 then
                        quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_D")
                    end
                end
            end
        end
    until not (false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

