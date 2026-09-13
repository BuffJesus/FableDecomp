-- Generated native draft: OVI_DeadFather. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    -- TODO(native): iStack_4 = param_1;
    -- TODO(native): ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(param_1 + 8);
    quest:MiniMapAddMarker(nil --[[missing]], "HUD_ORB_QUEST_CORE")
    quest:SetIsPushableByHero(me, false)
end

function Main(quest, me)
    local cVar4, fVar7, pCVar1, pCVar8, piVar6, uVar5
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_10);
        if bVar3 then
        end
        cVar4 = me:AcquireControl(4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db8507 end
            cVar4 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            uVar5 = quest:GetThingWithScriptName("MK_OVID_DAD")
            quest:EntityTeleportToThing(me, uVar5)
            piVar6 = quest:GetThingWithScriptName("MK_OVID_DAD")
            fVar7 = piVar6:GetAngleXY()
            quest:EntitySetFacingAngle(me, fVar7)
            me:PlayLoopingAnimation("CS_DEAD_DAD", -1, false, true, false)
            cVar4 = quest:GetStateBool("DadFound")
            while not cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db8507 end
                cVar4 = quest:GetStateBool("DadFound")
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                quest:MiniMapRemoveMarker(piVar6)
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                until not (alive)
            end
        end
        ::LAB_00db8507::
    end
end

function OnPredicateFail(quest, me)
end

