-- Generated native draft: TraderComment. Review coverage report before use.
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
    local bVar3, cVar4, comment_to_make, comment_type, ctr_28, iVar5, i_stk_24, native_arg_sequence_1, p0, puStack_1c, puVar1, puVar2, pu_stk_18, speaker
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    puStack_1c = quest:GetAllThingsWithScriptName("DarkwoodTrader")
    i_stk_24 = quest:ReadGlobalGameData(0xe10)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    while true do
        pu_stk_18 = puVar1
        puVar2 = puStack_1c
        if not (not bVar3) then break end
        iVar5 = puVar1 - puStack_1c >> 0x1f
        ctr_28 = 0
        if (puVar1 - puStack_1c) / 0xc + iVar5 ~= iVar5 then
            iVar5 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                puVar1 = 0
                if bVar3 then goto LAB_00e05536 end
                cVar4 = puStack_1c[(iVar5) / 0xc + 1]:IsAlive()
                native_arg_sequence_1 = false
                if cVar4 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    bVar3 = quest:IsDistanceBetweenThingsUnder(me, (iVar5 + puStack_1c), i_stk_24)
                    if bVar3 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00e05564: (native jump target)
                        return
                    end
                    speaker = puStack_1c[(iVar5) / 0xc + 1]
                    comment_type = 1
                    comment_to_make = me:GetDataString()
                    bVar3 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, comment_to_make, speaker, comment_type)
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:RemoveThing(me, false, true)
                        end
                        return
                    end
                end
                ctr_28 = ctr_28 + 1
                iVar5 = iVar5 + 0xc
            until not (ctr_28 < (#puStack_1c))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        puVar1 = 0
        if bVar3 then goto LAB_00e0557f end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    end
    ::LAB_00e05596::
    do return end
    ::LAB_00e05536::
    goto LAB_00e05596
    ::LAB_00e0557f::
    goto LAB_00e05596
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

