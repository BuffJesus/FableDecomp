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
    local resources = quest:RetailResources()
    local bVar2, bVar3, bVar4, cVar1, p0, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        bVar2 = resources:TryAcquire(xStack_20, me, 4)
        while not bVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d68acf end
            bVar2 = resources:TryAcquire(xStack_20, me, 4)
        end
        bVar4 = false
        bVar2 = false
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:EntitySetAsKillable(me, false, false)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
            quest:EntitySetInFaction(me, "FACTION_HERO")
            quest:EntitySetAllowBossPhaseChanges(me, false)
            cVar1 = quest:GetStateBool("BanditsAlive")
            while cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d68acf end
                bVar3 = me:MsgIsHitByHero()
                if bVar3 then
                    goto LAB_00d689df
                else
                    bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar4 then
                        bVar4 = true
                        bVar2 = true
                        bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar3 then goto LAB_00d689df end
                    end
                    bVar4 = true
                    bVar3 = false
                end
                goto FLOW_past_lab_00d689df
                ::LAB_00d689df::
                bVar3 = true
                ::FLOW_past_lab_00d689df::
                if bVar2 then
                    bVar2 = false
                end
                if bVar4 then
                    bVar4 = false
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d68acf end
                    quest:EntitySetInFaction(me, "FACTION_HERO")
                    me:SetFriendsWithEverythingFlag(true)
                end
                if quest:GetStateBool("WhisperAnimate") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d68acf end
                    quest:SetStateBool("WhisperAnimate", false)
                    me:PlayAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", false, false, false, true, true, false, false)
                end
                cVar1 = quest:GetStateBool("BanditsAlive")
            end
            alive = not quest:IsActiveThreadTerminating()
        end
        ::LAB_00d68acf::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

