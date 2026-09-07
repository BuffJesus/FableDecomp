Quest = nil

local QUEST_NAME = "StatueMaster"

local function angleFractionBetween(fromPos, toPos)
    local dx = toPos.x - fromPos.x
    local dy = toPos.y - fromPos.y
    local angle = math.atan(dx, dy) / (2 * math.pi)
    if angle < 0.0 or angle >= 1.0 then
        angle = math.fmod(angle, 1.0)
        if angle < 0.0 then
            angle = angle + 1.0
        end
    end
    return angle
end

function Init(questObject)
    Quest = questObject
    Quest:Log("StatueMaster: Init phase started.")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("StatueMaster: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("StatueMasterStatue", "StatueMaster/Entities/StatueMasterStatue")
    Quest:AddEntityBinding("StatueMasterCellarDoors", "StatueMaster/Entities/StatueMasterCellarDoors")
    Quest:AddEntityBinding("StatueMasterChest", "StatueMaster/Entities/StatueMasterChest")
    Quest:FinalizeEntityBindings()

    local smNothing = Quest:GetThingWithScriptName("SM_Nothing")
    local smBowerstone = Quest:GetThingWithScriptName("SM_Bowerstone")
    local smGuild = Quest:GetThingWithScriptName("SM_Guild")
    local smGreatwood = Quest:GetThingWithScriptName("SM_Greatwood")
    local smCenter = Quest:GetThingWithScriptName("SM_Center")

    if not smCenter then
        Quest:Log("!!! ERROR: StatueMaster - could not find SM_Center marker. Aborting angle setup.")
        return
    end

    local centerPos = smCenter:GetPos()

    -- Index 0 = Nothing, 1 = Bowerstone, 2 = Guild, 3 = Greatwood - matches
    local angles = {}
    angles[0] = smNothing and angleFractionBetween(centerPos, smNothing:GetPos()) or 0.0
    angles[1] = smBowerstone and angleFractionBetween(centerPos, smBowerstone:GetPos()) or 0.0
    angles[2] = smGuild and angleFractionBetween(centerPos, smGuild:GetPos()) or 0.0
    angles[3] = smGreatwood and angleFractionBetween(centerPos, smGreatwood:GetPos()) or 0.0

    Quest:SetStateString("SM_Angle0", tostring(angles[0]))
    Quest:SetStateString("SM_Angle1", tostring(angles[1]))
    Quest:SetStateString("SM_Angle2", tostring(angles[2]))
    Quest:SetStateString("SM_Angle3", tostring(angles[3]))

    Quest:Log("StatueMaster: Angles computed - Nothing=" .. tostring(angles[0])
        .. " Bowerstone=" .. tostring(angles[1])
        .. " Guild=" .. tostring(angles[2])
        .. " Greatwood=" .. tostring(angles[3]))
end