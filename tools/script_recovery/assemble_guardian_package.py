"""Assemble a new, disabled Guardian package; never installs or overwrites it."""
from pathlib import Path
import shutil

ROOT=Path(__file__).resolve().parents[2]

def assemble():
    target=ROOT/'refs/script_recovery/reconstructed/GuardianTrophyDealerInfo/FSE'
    if target.exists():raise FileExistsError(target)
    module=target/'GuardianTrophyDealerInfo'
    (module/'Entities').mkdir(parents=True)
    reference=ROOT/'refs/script_recovery/guardian_trophy_dealer_info/reference'
    for name in ('quest.lua','runtime_adapter.lua'):
        shutil.copyfile(reference/name,module/name)
    (module/'GuardianTrophyDealerInfo.lua').write_text('''local Q = require("GuardianTrophyDealerInfo.quest")
local A = require("GuardianTrophyDealerInfo.runtime_adapter")
function Init(quest) Q.init(A.new(quest)) end
function Main(quest) Q.main(A.new(quest)) end
function OnPersist(quest, context) Q.persist(A.new(quest), context) end
function WaitForPieceOver(quest) A.run(Q, "waiter", quest) end
''',encoding='utf-8')
    (module/'Entities/GTDI_Maze.lua').write_text('''local Q = require("GuardianTrophyDealerInfo.quest")
local A = require("GuardianTrophyDealerInfo.runtime_adapter")
-- Entity VM owns the angle captured by native Init at entity+1C.
local initial_angle
function Init(quest, me) initial_angle = Q.entity_init(A.new(quest, nil, me)) end
function Main(quest, me)
    assert(initial_angle ~= nil, "GTDI_Maze requires Init before Main")
    A.run(Q, "entity_main", quest, me, initial_angle)
end
''',encoding='utf-8')
    (target/'quests.lua').write_text('-- No legacy registration. This source-only package is disabled.\n',encoding='utf-8')
    (target/'retail_override.lua').write_text('''-- Offline staging; requires the newer source runtime, not installed v23.
RetailOverrides = {
    enabled = false,
    allowUnverifiedDisposable = false,
    disposableSaveAcknowledgement = "",
    entries = {
        {
            nativeName = "QS_GuardianTrophyDealerInfo",
            file = "GuardianTrophyDealerInfo/GuardianTrophyDealerInfo",
            mode = "override", evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true, saveWritesAllowed = true,
            entity_scripts = {
                {name = "GTDI_Maze", file = "GuardianTrophyDealerInfo/Entities/GTDI_Maze", id = 69},
            },
        },
    },
}
''',encoding='utf-8')
    print(target)

if __name__=='__main__':assemble()
