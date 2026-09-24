-- Readable native conversion: MagicBarrier. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- MagicBarrier.Main (retail 0x00e03f70)
function Main(quest, me)
    local getAngleXY9, getAngleXY, getAngleXY11, getAngleXY12, sequence, sequence22, getPos, getPos2
    local getPos3, getPos4, getPos5, getPos6, getPos7, getPos8
    local barrierFX = quest:GetThingWithScriptName("BarrierFX")
    if barrierFX == nil then
        getPos = {x = 0, y = 0, z = 0}
    else
        getAngleXY12 = barrierFX:GetAngleXY()
        if not (barrierFX ~= nil and not barrierFX:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = barrierFX:GetPos()
        end
    end
    local scratchValue = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos, getAngleXY12, false)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos2 = {x = 0, y = 0, z = 0}
    else
        getAngleXY11 = barrierFX:GetAngleXY()
        if not (barrierFX ~= nil and not barrierFX:IsNull()) then
            getPos2 = {x = 0, y = 0, z = 0}
        else
            getPos2 = barrierFX:GetPos()
        end
    end
    local scratchValue2 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos2, getAngleXY11, false)
    while not quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046bf end
        -- TODO(native): cVar5 = (**(r2._0_4_ + 0x12c))()
    --[[unresolved native value]]
        sequence = not nil
        if sequence then
            -- TODO(native): cVar5 = (**(r3._0_4_ + 0x12c))()
    --[[unresolved native value]]
            sequence = not nil
        end
        if sequence then
            if quest:IsActiveThreadTerminating() then goto LAB_00e046bf end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos3 = {x = 0, y = 0, z = 0}
            else
                getAngleXY11 = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos3 = {x = 0, y = 0, z = 0}
                else
                    getPos3 = barrierFX:GetPos()
                end
            end
            quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos3, getAngleXY11, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if scratchValue._8_4_ ~= nil then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos4 = {x = 0, y = 0, z = 0}
            else
                getAngleXY = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos4 = {x = 0, y = 0, z = 0}
                else
                    getPos4 = barrierFX:GetPos()
                end
            end
            quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos4, getAngleXY, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if scratchValue2._8_4_ ~= nil then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046bf end
    while quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046b6 end
        -- TODO(native): cVar5 = (**(r2._0_4_ + 0x12c))()
    --[[unresolved native value]]
        sequence22 = not nil
        if sequence22 then
            -- TODO(native): cVar5 = (**(r3._0_4_ + 0x12c))()
    --[[unresolved native value]]
            sequence22 = not nil
        end
        if sequence22 then
            if quest:IsActiveThreadTerminating() then goto LAB_00e046b6 end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos5 = {x = 0, y = 0, z = 0}
            else
                getAngleXY9 = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos5 = {x = 0, y = 0, z = 0}
                else
                    getPos5 = barrierFX:GetPos()
                end
            end
            quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos5, getAngleXY9, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if scratchValue._8_4_ ~= nil then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos6 = {x = 0, y = 0, z = 0}
            else
                getAngleXY11 = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos6 = {x = 0, y = 0, z = 0}
                else
                    getPos6 = barrierFX:GetPos()
                end
            end
            quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos6, getAngleXY11, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if scratchValue2._8_4_ ~= nil then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046b6 end
    quest:RemoveThing(scratchValue, false, true)
    quest:RemoveThing(scratchValue2, false, true)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos7 = {x = 0, y = 0, z = 0}
    else
        getPos7 = barrierFX:GetPos()
    end
    quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01_OFF", getPos7, barrierFX:GetAngleXY(), false)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos8 = {x = 0, y = 0, z = 0}
    else
        getPos8 = barrierFX:GetPos()
    end
    quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02_OFF", getPos8, barrierFX:GetAngleXY(), false)
    quest:RemoveThing(me, false, true)
    ::LAB_00e046b6::
    ::LAB_00e046bf::
end

-- MagicBarrier.Init (retail 0x00e03f30)
function Init(quest, me)
    quest:EntitySetAsDrawable(me, false)
end

-- MagicBarrier.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MagicBarrier.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

