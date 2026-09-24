-- Readable native conversion: MagicBarrier. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- MagicBarrier.Main (retail 0x00e03f70)
function Main(quest, me)
    local getAngleXY9, getAngleXY, getAngleXY11, getAngleXY12, getPos, getPos2, getPos3, getPos4
    local getPos5, getPos6, getPos7, getPos8, createEffectAtPos, createEffectAtPos2
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
    createEffectAtPos = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos, getAngleXY12, false)
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
    createEffectAtPos2 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos2, getAngleXY11, false)
    while not quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046bf end
        if not (createEffectAtPos ~= nil and createEffectAtPos:IsAlive()) and not (createEffectAtPos2 ~= nil and createEffectAtPos2:IsAlive()) then
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
            createEffectAtPos = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos3, getAngleXY11, false)
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
            createEffectAtPos2 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos4, getAngleXY, false)
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046bf end
    while quest:GetStateBool("TradersShouldBeScared") do
        if not quest:NewScriptFrame(me) then goto LAB_00e046b6 end
        if not (createEffectAtPos ~= nil and createEffectAtPos:IsAlive()) and not (createEffectAtPos2 ~= nil and createEffectAtPos2:IsAlive()) then
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
            createEffectAtPos = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", getPos5, getAngleXY9, false)
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
            createEffectAtPos2 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", getPos6, getAngleXY11, false)
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e046b6 end
    quest:RemoveThing(createEffectAtPos, false, true)
    quest:RemoveThing(createEffectAtPos2, false, true)
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

