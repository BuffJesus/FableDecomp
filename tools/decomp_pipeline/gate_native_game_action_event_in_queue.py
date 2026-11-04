"""Fresh VC71 parity gate: CGamePlayerInterface::IsGameActionEventInQueue without the time output (0x00446B10), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@JAAM@Z':0x445cb0,
}
ROOTS=[
 dict(name='is_game_action_event_in_queue',source=R/'rebuild/src/compiled/00/44/CGamePlayerInterface_IsGameActionEventInQueue_00446b10.cpp',symbol='?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z',address=0x446b10,size=37,instructions=13,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-game-action-event-in-queue.json',[],
  'CGamePlayerInterface::IsGameActionEventInQueue without the time output (0x00446B10), exact after relocation')
 print('NATIVE_GAME_ACTION_EVENT_IN_QUEUE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
