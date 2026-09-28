-- Readable native conversion: KickedChicken. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- KickedChicken.Main (retail 0x00e64210)
function Main(quest, me)
    local resources = quest:RetailResources()
    local ctr_68, scratchValue3, scratchValue4, f_p1, fret_0, sequence, getPos, getPos2, getPos3
    local scratchValue13, scratchValue14, scratchValue17, vec_30, scratchValue18, scratchValue20
    local chickenKickingArena = quest:GetThingWithScriptName("ChickenKickingArena")
    local getDistanceBetweenThings = quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("FirstLineMarker")) ^ 2
    local getDistanceBetweenThings2 = quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("SecondLineMarker")) ^ 2
    local getDistanceBetweenThings3 = quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("ThirdLineMarker")) ^ 2
    local getDistanceBetweenThings4 = quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("FourthLineMarker")) ^ 2
    quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("RearRight"))
    quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("MidRight"))
    quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("MidRight2"))
    quest:GetDistanceBetweenThings(chickenKickingArena, quest:GetThingWithScriptName("FrontRight"))
    local position = quest:GetThingWithScriptName("FoulLineMarkerA"):GetPos()
    local vec_3c = {x = position.x, y = position.y, z = position.z}
    local position2 = quest:GetThingWithScriptName("FoulLineMarkerB"):GetPos()
    local vec_24 = {x = position2.x, y = position2.y, z = position2.z}
    quest:SetStateInt("DistanceBand", 0)
    repeat
        if not quest:NewScriptFrame(me) then return end
        if me:MsgIsKicked() then
            if quest:IsActiveThreadTerminating() then return end
            scratchValue17 = math.random(0, 32767) & 0x80000003
            if scratchValue17 < 0 then
                scratchValue17 = (scratchValue17 - 1 | 0xfffffffc) + 1
            end
            quest:PlaySoundOnThing(me, "SND_MM_CHICKEN_BUKARK_0" .. tostring(scratchValue17 + 1))
        end
        local position3 = quest:GetHero():GetPos()
        vec_30 = {x = position3.x, y = position3.y, z = position3.z}
        if quest:EntityGetShotStrikePos(me) then
            if not quest:IsActiveThreadTerminating() then goto LAB_00e64831 end
            do return end
        end
    until not ((vec_24.y - vec_30.y) * (vec_3c.x - vec_30.x) - (vec_3c.y - vec_30.y) * (vec_24.x - vec_30.x) <= 0.0)
    if not quest:IsActiveThreadTerminating() then quest:SetStateInt("DistanceBand", 0xffffffff); goto LAB_00e64831 end
    do return end
    ::LAB_00e64831::
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateInt("DistanceBand") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        if not (chickenKickingArena ~= nil and not chickenKickingArena:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = chickenKickingArena:GetPos()
        end
        local scratchValue7 = scratchValue20._4_4_ - getPos.y
        -- TODO(native): fStack_24 = (float)xStack_c._0_4_ - pfVar6.x;
        -- TODO(native): C3DVector::GetScaled((C3DVector *)&fStack_24);
        -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
        -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
        -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
        local fourthAngleMarker = quest:GetThingWithScriptName("FourthAngleMarker")
        -- TODO(native): Vector_PushBack_ScriptThing(&xStack_3c,(int)pCVar3);
        quest:SetStateInt("FinalSector", 0)
        ctr_68 = 1
        sequence = scratchValue18 ~= scratchValue14
        if sequence then
            scratchValue13 = scratchValue18 + 3
            sequence = scratchValue13 ~= scratchValue14
        end
        if sequence then
            repeat
                if not (chickenKickingArena ~= nil and not chickenKickingArena:IsNull()) then
                    getPos2 = {x = 0, y = 0, z = 0}
                else
                    getPos2 = chickenKickingArena:GetPos()
                end
                -- TODO(native): pfVar7 = (**(piVar8[-3] + 0x18))()
    --[[unresolved native value]]
                local scratchValue8 = (nil)[1] - getPos2.y
                -- TODO(native): fStack_48 = *pfVar7 - pfVar6.x;
                if not (chickenKickingArena ~= nil and not chickenKickingArena:IsNull()) then
                    getPos3 = {x = 0, y = 0, z = 0}
                else
                    getPos3 = chickenKickingArena:GetPos()
                end
                local f_stk_2c_2 = quest:IsXbox()[1] - getPos3.y
                -- TODO(native): xStack_30 = (*pfVar7 - pfVar6.x);
                if not ((0.0001 * 0.0001 < scratchValue4 * scratchValue4 + scratchValue8 * scratchValue8) and (0.0001 * 0.0001 < f_stk_2c_2 * f_stk_2c_2 + fourthAngleMarker * fourthAngleMarker)) then ctr_68 = ctr_68 + 1; scratchValue13 = scratchValue13 + 3; goto continue_2 end
                -- TODO(native): C3DVector::GetScaled((C3DVector *)&fStack_48);
                -- TODO(native): C3DVector::GetScaled((C3DVector *)xStack_30);
                -- TODO(native): xStack_18._8_4_ = 1.0;
                f_p1 = fret_0 * 0.15915493667125702 * 0.5
                -- TODO(native): C3DVector::Rotate((C3DVector *)&fStack_48,(int)&xStack_18,(int)p1);
                if fcos(f_p1 * 6.2831854820251465) < scratchValue4 * scratchValue3 + 0.0 * 0.0 + scratchValue8 * scratchValue7 then
                    quest:SetStateInt("FinalSector", ctr_68)
                end
                ctr_68 = ctr_68 + 1
                scratchValue13 = scratchValue13 + 3
                ::continue_2::
            until scratchValue13 == scratchValue14
        end
        if chickenKickingArena ~= nil and not chickenKickingArena:IsNull() then
            chickenKickingArena:GetPos()
        end
        local scratchValue5 = DistanceCalculator_GetVectorDistance(chickenKickingArena,scratchValue20)
        if scratchValue5 <= getDistanceBetweenThings4 then
            if scratchValue5 <= getDistanceBetweenThings3 then
                if scratchValue5 <= getDistanceBetweenThings2 then
                    if scratchValue5 <= getDistanceBetweenThings then
                        if quest:IsActiveThreadTerminating() then return end
                        quest:SetStateInt("DistanceBand", 0)
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        quest:SetStateInt("DistanceBand", 1)
                    end
                else
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateInt("DistanceBand", 2)
                end
            else
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("DistanceBand", 3)
            end
        else
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("DistanceBand", 4)
        end
    end
    quest:SetStateBool("ChickenLanded", true)
    resources:PrepareResource(resources:MemberResource("seh_Chicken"))
    while not resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
end

-- KickedChicken.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- KickedChicken.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- KickedChicken.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

