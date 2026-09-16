# Remaining RockTroll Main integration work

The isolated candidate has root Init/Main/OnPersist, all three parent helpers,
trigger Main and Sparrow Main. RTFE_RockTroll.Main remains absent. The port is
disabled and requires the following verified native behavior to be implemented
and reviewed; this inventory does not certify the omitted code.

The first self/Hero acquisition phases are now emitted separately under Phases,
with explicit continuation callbacks. They do not define actor Main. Native
dispatch and resource-window tests cover retained copies, priority4, fresh Hero
queries, populated failures and cancellation; the host thread adapter and
complete actor integration are still pending.

- **Retained helper arguments and dispatch.** Main EC4A00 constructs the Killed
  helper before self acquisition (stored body EC5010 at EC4A8D), and constructs
  Hit after the exhumation macro (EC4DDF..EC4E7F). Each captures a counted Thing,
  uses a false constructor flag and empty region. Shared dispatch EC52F0 copies
  the stored Thing by value and increments Info before calling the body;
  EC5330 destroys the stored copy separately. Generic Lua arguments and callback
  counters do not establish those lifetimes or scheduler quiescence.
- **Self/Hero resource scopes.** Self acquisition EC4B40/EC4B6B uses priority4;
  Hero acquisition EC4C57/EC4C89 also uses4 and obtains Hero anew for each attempt.
  Preserve the single output resource across failed polls, including populated
  failure output, cancellation and nested cleanup. The checked resource scope
  provides the primitives, but the complete actor control flow is not emitted.
- **Dynamic item definitions (phase recovered, adapter pending).** EC4BAB/EC4BDB convert CDefString fields at
  `*(0143E90C)+730/+734`; AddItemToContainer calls EC4BBB/EC4BEB consume bound
  Troll and each temporary CString before destruction EC4BC5/EC4BF5. Set
  AddedItemsToRockTroll only after both calls (EC4BFD). WithRockTrollRewardsPhase
  now expresses this behavior. AddRockTrollReward has an isolated typed adapter
  proposal with an actual-FSE x86/Lua gate in reward_proposal; host integration
  remains pending. See the candidate's REWARD_STATUS for current source-hash
  correspondence. Tests run the original
  415D70/9D49B0 conversion instructions, including empty-token construction and
  a definition-pointer replacement between calls. Installed names are not copied.
- **Borrowed exhumation movie and actor map (phase recovered).** HERO/TROLL entries are populated
  with native resources (EC4D29/EC4D5C), movie starts at EC4D92, pause(true) is
  EC4DAB, macro CS_ROCKTROLL_EXHUME runs at EC4DD1 with pushed flags0,0,0,1.
  Hit helper registration and PlayedExhumeCutScene=true occur before unpause
  EC4ECB, movie destruction EC4ED5, map destruction EC4EDE and Hero-resource
  destruction EC4EE7. WithRockTrollExhumationPhase now expresses this order and
  the already-played skip. Existing resource APIs match the macro ABI with
  null flag/input maps, setup=false/skippable=true. Original instruction tests
  and an actual-FSE x86/Lua gate in exhumation_proposal cover empty resource
  copies, movie/map lifetimes, errors and macro-time cancellation without an
  invented guard afterward. Thread registration remains a pending adapter.
- **Native resource animations (phase recovered, consumer pending).** SPECIAL_BOAST and SPECIAL_IDLE dispatch
  through resource helper7E73D0 at EC4F16/EC4F4D. One boolean is reread from
  byte01375748; the full seven slots are now witnessed as0,0,0,1,rawbyte,0,0.
  WithRockTrollAnimationPhase preserves CString-before-byte-read order, task
  polling EC4F5F/EC4F7F, cancellation and conditional control release. The
  resource object remains live for the targeting continuation. Native helper
  tests cover empty Data and raw byte0/1/2/255; the staged method still needs
  actual-FSE compilation and binding. Existing speech methods are unchanged.
- **Final targeting.** After releasing self control, fresh Hero queries feed
  EntityForceToLookAtThing EC4FC0 and GiveThingBestEnemyTarget EC4FDA on the bound
  Troll. The actor then frames/checks cancellation until termination and destroys
  its local resource. Retained source/target behavior and errors need tests.

The parent helpers additionally require the proposed **WithCopiedThreadThing**
scope: parent-thread alive condition for Killed, atomic killed-message CString,
nested short-circuit hit/ability CString scopes for Hit, health on the copied
Thing, and health-bar creation storing the full ID before texture destruction.
These requirements are represented in Lua but the host adapter is not installed.
PersistTransferIntDefault, the trigger proposals and scoped region-message
proposal also remain pending host integration.

Original PDB names identify TrollAwake, but the recovered Init/OnPersist do not
write/persist it and the inspected Main/Killed/Hit instructions have not identified
a retail writer at parent+4C. Sparrow reads that field. Do not invent a writer or
initial value; trace remaining native initialization/external ownership before
claiming that interaction complete. Entity lifecycle hooks beyond recovered
Main bodies also need an explicit whole-port audit.

All entry-policy, condition-clone, retained-thread-argument, manager teardown,
save/restore and no-future-dispatch gates remain open. No generated syntax or
offline test result establishes gameplay completeness.
