-- Generated native draft: Q_BanditCampHoldingScript. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar1, cVar2, native_arg_sequence_1, r1
    local alive = true
    if not quest:GetStateBool("PathReached") then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        bVar1 = quest:IsLevelLoaded("BanditCampPath_1")
        while not bVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            bVar1 = quest:IsLevelLoaded("BanditCampPath_1")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:SetStateBool("PathReached", true)
    end
    bVar1 = quest:IsRegionLoaded("BanditCampPathEntrance")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                r1 = quest:GetThingWithScriptName("OakValeBanditRaidGate")
                native_arg_sequence_1 = false
                if not (r1 ~= nil and not r1:IsNull()) then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    cVar2 = (r1 ~= nil and r1:IsOpenDoor())
                    if not cVar2 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    quest:OpenDoor(r1)
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsRegionLoaded("BanditCampPathEntrance")
    end
end

function Init(quest)
    quest:SetStateBool("PathReached", false)
end

function OnPersist(quest, context)
    local pathReached = quest:GetStateBool("PathReached") or false
    pathReached = quest:PersistTransferBool(context, "PathReached", pathReached)
    quest:SetStateBool("PathReached", pathReached)
end

