-- Offline reviewed QS_MeetSister port. Requires WithRetailResources and binding flags.
local reference = require("MeetSister.quest")
local adapter = require("MeetSister.runtime_adapter")
function Init(quest) adapter.run(reference, "init", quest) end
function Main(quest) adapter.run(reference, "main", quest) end
