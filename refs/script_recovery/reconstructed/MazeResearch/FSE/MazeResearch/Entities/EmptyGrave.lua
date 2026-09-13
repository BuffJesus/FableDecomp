local Q = require("MazeResearch.quest")
local A = require("MazeResearch.runtime_adapter")
function Init(quest, me) A.grave_init(quest) end
function Main(quest, me) A.run(Q, "grave", quest, me) end
