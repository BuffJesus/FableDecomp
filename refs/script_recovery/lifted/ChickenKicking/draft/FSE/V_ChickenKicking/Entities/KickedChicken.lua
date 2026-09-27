-- Generated native draft: KickedChicken. Review coverage report before use.
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
    local bVar1, cVar2, ctr_68, fStack_24, fStack_48, fVar10, fVar9, f_p1, f_stk_1c, f_stk_20, f_stk_2c, f_stk_40, f_stk_44, f_stk_58, f_stk_5c, f_stk_60, f_stk_64, fret_0, native_arg_sequence_1, pCVar3, pCVar4, pRight, pfVar6, pfVar7, piVar8, pi_stk_38, r1, r2, uVar5, xStack_3c, xStack_68, xStack_c
    local alive = true
    r1 = quest:GetThingWithScriptName("ChickenKickingArena")
    pCVar3 = quest:GetThingWithScriptName("FirstLineMarker")
    f_stk_58 = (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("SecondLineMarker")
    f_stk_5c = (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("ThirdLineMarker")
    f_stk_60 = (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FourthLineMarker")
    f_stk_64 = (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("RearRight")
    (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("MidRight")
    (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("MidRight2")
    (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FrontRight")
    (quest:GetDistanceBetweenThings(r1, pCVar3) ^ 2)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FoulLineMarkerA")
    pCVar4 = pCVar3:GetPos()
    -- TODO(native): xStack_3c = *(int **)pCVar4;
    -- TODO(native): pi_stk_38 = *(pCVar4 + 0x4)
    pi_stk_38 = nil --[[unresolved native value]]
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FoulLineMarkerB")
    pCVar4 = pCVar3:GetPos()
    -- TODO(native): fStack_24 = pCVar4.x;
    f_stk_20 = pCVar4.y
    f_stk_1c = pCVar4.z
    pCVar3 = nil
    f_stk_2c = 0.0
    quest:SetStateInt("DistanceBand", 0)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00e64cf3 end
        cVar2 = me:MsgIsKicked()
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            uVar5 = math.random(0, 32767)
            uVar5 = uVar5 & 0x80000003
            if uVar5 < 0 then
                uVar5 = (uVar5 - 1 | 0xfffffffc) + 1
            end
            pRight = tostring(uVar5 + 1)
            xStack_68 = ("SND_MM_CHICKEN_BUKARK_0" .. pRight)
            r2 = quest:PlaySoundOnThing(me, xStack_68)
        end
        pCVar3 = quest:GetHero()
        pCVar4 = pCVar3:GetPos()
        -- TODO(native): xStack_30 = *(undefined1 (*) [4])pCVar4;
        f_stk_2c = pCVar4.y
        -- TODO(native): xStack_30 = *(int **)(pCVar4 + 0x8);
        bVar1 = quest:EntityGetShotStrikePos(me)
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then goto LAB_00e64831 end
            goto LAB_00e64cf3
        end
    until not ((f_stk_20 - f_stk_2c) * (xStack_3c - pCVar3) - (pi_stk_38 - f_stk_2c) * (fStack_24 - pCVar3) <= 0.0)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:SetStateInt("DistanceBand", 0xffffffff)
        goto LAB_00e64831
    end
    goto FLOW_past_lab_00e64831
    ::LAB_00e64831::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        if quest:GetStateInt("DistanceBand") == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            if not (r1 ~= nil and not r1:IsNull()) then
                pfVar6 = {x = 0, y = 0, z = 0}
            else
                pfVar6 = r1:GetPos()
            end
            f_stk_20 = xStack_c._4_4_ - pfVar6.y
            -- TODO(native): fStack_24 = (float)xStack_c._0_4_ - pfVar6.x;
            f_stk_1c = 0.0
            -- TODO(native): C3DVector::GetScaled((C3DVector *)&fStack_24);
            pCVar3 = quest:GetThingWithScriptName("FirstAngleMarker")
            -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
            pCVar3 = quest:GetThingWithScriptName("SecondAngleMarker")
            -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
            pCVar3 = quest:GetThingWithScriptName("ThirdAngleMarker")
            -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
            pCVar3 = quest:GetThingWithScriptName("FourthAngleMarker")
            -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
            quest:SetStateInt("FinalSector", 0)
            ctr_68 = 0x1
            native_arg_sequence_1 = false
            if xStack_3c ~= pi_stk_38 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                piVar8 = xStack_3c + 3
                if piVar8 ~= pi_stk_38 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                repeat
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pfVar6 = {x = 0, y = 0, z = 0}
                    else
                        pfVar6 = r1:GetPos()
                    end
                    -- TODO(native): pfVar7 = (**(piVar8[-3] + 0x18))()
                    pfVar7 = nil --[[unresolved native value]]
                    f_stk_40 = pfVar7[2] - pfVar6.z
                    f_stk_44 = pfVar7[1] - pfVar6.y
                    -- TODO(native): fStack_48 = *pfVar7 - pfVar6.x;
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pfVar6 = {x = 0, y = 0, z = 0}
                    else
                        pfVar6 = r1:GetPos()
                    end
                    pfVar7 = quest:IsXbox()
                    f_stk_2c = pfVar7[1] - pfVar6.y
                    -- TODO(native): xStack_30 = (*pfVar7 - pfVar6.x);
                    f_stk_40 = 0.0
                    if (0.0001 * 0.0001 < fStack_48 * fStack_48 + f_stk_44 * f_stk_44) and (0.0001 * 0.0001 < f_stk_2c * f_stk_2c + pCVar3 * pCVar3) then
                        -- TODO(native): C3DVector::GetScaled((C3DVector *)&fStack_48);
                        -- TODO(native): C3DVector::GetScaled((C3DVector *)xStack_30);
                        -- TODO(native): xStack_18._8_4_ = 1.0;
                        f_p1 = (fret_0 * 0.15915493667125702 * 0.5)
                        -- TODO(native): C3DVector::Rotate((C3DVector *)&fStack_48,(int)&xStack_18,(int)p1);
                        fVar9 = fcos(f_p1 * 6.2831854820251465)
                        if fVar9 < fStack_48 * fStack_24 + f_stk_40 * f_stk_1c + f_stk_44 * f_stk_20 then
                            quest:SetStateInt("FinalSector", ctr_68)
                        end
                    end
                    ctr_68 = ctr_68 + 1
                    piVar8 = piVar8 + 3
                until not (piVar8 ~= pi_stk_38)
            end
            if (r1 ~= nil and not r1:IsNull()) then
                r1:GetPos()
            end
            fVar10 = DistanceCalculator_GetVectorDistance(r1,xStack_c)
            if fVar10 <= f_stk_64 then
                if fVar10 <= f_stk_60 then
                    if fVar10 <= f_stk_5c then
                        if fVar10 <= f_stk_58 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then
                                return
                            end
                            quest:SetStateInt("DistanceBand", 0)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then
                                return
                            end
                            quest:SetStateInt("DistanceBand", 1)
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then
                            return
                        end
                        quest:SetStateInt("DistanceBand", 2)
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then return end
                    quest:SetStateInt("DistanceBand", 3)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then return end
                quest:SetStateInt("DistanceBand", 4)
            end
        end
        quest:SetStateBool("ChickenLanded", true)
        resources:PrepareResource(resources:MemberResource("seh_Chicken"))
        bVar1 = resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            bVar1 = resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
    end
    ::FLOW_past_lab_00e64831::
    ::LAB_00e64cf3::
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

