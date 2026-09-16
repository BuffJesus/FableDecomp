-- Reviewed native Init for Q_GuildTrainingWoodsMelee.ScorpionHome.
-- Main anchors are recorded below; executable conversion remains pending until
-- the creature-list and spawn wrapper signatures are joined to Forge APIs.

local MY_SCRIPT_NAME = "ScorpionHome"

function Init(entity)
    -- Native Init stores these in private entity fields, not Lua quest state.
    -- Keep the readable stub side-effect free until those fields have wrappers.
    return
end

-- Native Main anchors (0x00D67270): HUD_BEETLE_ICON, GuildScorpions,
-- CREATURE_GUILD_STAG_BEETLE, ScorpionSpawn, and Q_GuildTrainingWoodsMelee.
-- The native loop clears the parent ScorpionsAlive byte when fewer than three
-- creatures remain, and may spawn another stag beetle while its counter is > 0.
function Main(entity)
    error(MY_SCRIPT_NAME .. ": native Main pending wrapper ownership review")
end
