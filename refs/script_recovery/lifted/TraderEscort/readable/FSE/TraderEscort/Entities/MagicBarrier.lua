-- Readable native conversion: MagicBarrier. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- MagicBarrier.Main (retail 0x00e03f70)
function Main(quest, me)
    local getAngleXY, getAngleXY6, scratchValue, scratchValue2, getAngleXY7, getAngleXY8, sequence
    local sequence22, getPos, getPos2, getPos3, getPos4, getPos5, getPos6, getPos7, getPos8
    local darkwoodRockTroll, thing, thing2, thing3, thing4
    local barrierFX = quest:GetThingWithScriptName("BarrierFX")
    if barrierFX == nil then
        getPos = {x = 0, y = 0, z = 0}
    else
        getAngleXY8 = barrierFX:GetAngleXY()
        if not (barrierFX ~= nil and not barrierFX:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = barrierFX:GetPos()
        end
    end
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(thing3, "NEW_RED_FORCEFIELD_IDLE_01", getPos, "", getAngleXY8, false, false)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos2 = {x = 0, y = 0, z = 0}
    else
        -- TODO(native): xStack_58 = (CCharString)(float)fVar8;
        if not (barrierFX ~= nil and not barrierFX:IsNull()) then
            getPos2 = {x = 0, y = 0, z = 0}
        else
            getPos2 = barrierFX:GetPos()
        end
    end
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(thing4, "NEW_RED_FORCEFIELD_IDLE_02", getPos2, "", getAngleXY7, false, false)
    while not quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046bf end
        -- TODO(native): cVar5 = (**(xStack_30._0_4_ + 0x12c))()
    --[[unresolved native value]]
        sequence = not nil
        if sequence then
            -- TODO(native): cVar5 = (**(xStack_3c._0_4_ + 0x12c))()
    --[[unresolved native value]]
            sequence = not nil
        end
        if sequence then
            if quest:IsActiveThreadTerminating() then goto LAB_00e046bf end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos3 = {x = 0, y = 0, z = 0}
            else
                getAngleXY7 = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos3 = {x = 0, y = 0, z = 0}
                else
                    getPos3 = barrierFX:GetPos()
                end
            end
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(thing2, "NEW_RED_FORCEFIELD_IDLE_01", getPos3, "", getAngleXY7, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if thing3._8_4_ ~= nil then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            thing2 = nil
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos4 = {x = 0, y = 0, z = 0}
            else
                -- TODO(native): xStack_54 = (CCharString)(float)fVar8;
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos4 = {x = 0, y = 0, z = 0}
                else
                    getPos4 = barrierFX:GetPos()
                end
            end
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(thing, "NEW_RED_FORCEFIELD_IDLE_02", getPos4, "", scratchValue2, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if thing4._8_4_ ~= nil then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            thing = nil
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046bf end
    darkwoodRockTroll = quest:GetThingWithScriptName("DarkwoodRockTroll")
    while quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046b6 end
        -- TODO(native): cVar5 = (**(xStack_30._0_4_ + 0x12c))()
    --[[unresolved native value]]
        sequence22 = not nil
        if sequence22 then
            -- TODO(native): cVar5 = (**(xStack_3c._0_4_ + 0x12c))()
    --[[unresolved native value]]
            sequence22 = not nil
        end
        if sequence22 then
            if quest:IsActiveThreadTerminating() then goto LAB_00e046b6 end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos5 = {x = 0, y = 0, z = 0}
            else
                -- TODO(native): xStack_4c = (CCharString)(float)fVar8;
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos5 = {x = 0, y = 0, z = 0}
                else
                    getPos5 = barrierFX:GetPos()
                end
            end
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(thing, "NEW_RED_FORCEFIELD_IDLE_01", getPos5, "", scratchValue, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if thing3._8_4_ ~= nil then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                getPos6 = {x = 0, y = 0, z = 0}
            else
                getAngleXY7 = barrierFX:GetAngleXY()
                if not (barrierFX ~= nil and not barrierFX:IsNull()) then
                    getPos6 = {x = 0, y = 0, z = 0}
                else
                    getPos6 = barrierFX:GetPos()
                end
            end
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(thing2, "NEW_RED_FORCEFIELD_IDLE_02", getPos6, "", getAngleXY7, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
    --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
--[[unresolved native value]]
            if thing4._8_4_ ~= nil then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046b6 end
    quest:RemoveThing(darkwoodRockTroll, thing3, false)
    quest:RemoveThing(barrierFX, thing4, false)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos7 = {x = 0, y = 0, z = 0}
    else
        getPos7 = barrierFX:GetPos()
    end
    getAngleXY = barrierFX:GetAngleXY()
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(thing, "NEW_RED_FORCEFIELD_IDLE_01_OFF", getPos7, "", getAngleXY, false, false)
    if not (barrierFX ~= nil and not barrierFX:IsNull()) then
        getPos8 = {x = 0, y = 0, z = 0}
    else
        getPos8 = barrierFX:GetPos()
    end
    getAngleXY6 = barrierFX:GetAngleXY()
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(thing, "NEW_RED_FORCEFIELD_IDLE_02_OFF", getPos8, "", getAngleXY6, false, false)
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

