-- Generated native draft: WillWhisper. Review coverage report before use.
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
    local bVar2, bVar3, bVar4, cVar5, pCVar1, pCVar7, pCVar8
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_20);
        if bVar4 then
        end
        cVar5 = me:AcquireControl(4)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d68acf end
            cVar5 = me:AcquireControl(4)
        end
        bVar2 = false
        bVar4 = false
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:EntitySetAsKillable(nil --[[missing]], bVar4)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
            quest:EntitySetInFaction(me, "FACTION_HERO")
            quest:EntitySetAllowBossPhaseChanges(me, false)
            cVar5 = quest:GetStateBool("BanditsAlive")
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d68acf end
                cVar5 = me:MsgIsHitByHero()
                if not cVar5 then
                    cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if cVar5 then
                        bVar2 = true
                        bVar4 = true
                        cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                        if not cVar5 then return end  -- TODO(native): goto LAB_00d689df
                    end
                    bVar2 = true
                    bVar3 = false
                else
                    -- LAB_00d689df: (native jump target)
                    bVar3 = true
                end
                if bVar4 then
                    bVar4 = false
                end
                if bVar2 then
                    bVar2 = false
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d68acf end
                    quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
                    me:SetFriendsWithEverythingFlag(nil --[[missing]])
                end
                if quest:GetStateBool("WhisperAnimate") == '\x01' then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d68acf end
                    quest:SetStateBool("WhisperAnimate", false)
                    me:PlayAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", false, false, false, true, DAT_01375748, false)
                end
                cVar5 = quest:GetStateBool("BanditsAlive")
            end
            alive = not quest:IsActiveThreadTerminating()
        end
        ::LAB_00d68acf::
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

