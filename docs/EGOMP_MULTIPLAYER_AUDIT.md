# EgoMP multiplayer audit

Source: [`98thrxse/egomp`](https://github.com/98thrxse/egomp) at commit
[`816e58f9a0acd099ac8289b6a125bbf95c590b4b`](https://github.com/98thrxse/egomp/commit/816e58f9a0acd099ac8289b6a125bbf95c590b4b),
dated 2026-05-19.

## Verdict

EgoMP is useful to the future multiplayer effort as a feasibility proof and
an engine-integration map. It demonstrates that TLC can create extra
`CPlayer`/`CThingPlayerCreature` pairs, drive remote creatures through
movement and facing seams, and order a join around the retail region loader.
Those are important unknowns that no longer need to be treated as purely
theoretical.

It is not a production network architecture to adopt. EgoMP is a
client-authoritative four-slot listen-server relay with no protocol
versioning, sender validation, interpolation, gameplay-state replication,
persistence, or security. Its most useful contribution is the set of engine
seams and lifecycle constraints it exposes.

This audit is the sidecar-prototype counterpart to
[`COOP_REVIVAL.md`](COOP_REVIVAL.md), which documents TLC's separate dormant
`CNetworkClient`/`CGameEventPackageSet` machinery. The two systems must not be
treated as one implementation: EgoMP does not use or validate the retail
network gate, package protocol, `CheckSync`, or multiplayer-active path.

## Useful engine evidence

| Area | Evidence and value |
|---|---|
| Lifecycle | Networking attaches after `CMainGameComponent::PostInit`, polls after each game `Update`, and disconnects before retail `Shutdown`. |
| Remote players | Extra engine `CPlayer` slots and `CThingPlayerCreature` instances are used to represent peers. A `network ID -> engine player ID -> creature` adapter is viable. |
| Motion | [`ResolveMovementAcceleration` and `ResolveFacingDirection`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetPlayerManager/NetPlayerManagerMotion.cpp) are promising simulation seams. Their hook locations should be independently verified before use. |
| Region join | The joining client requests the region containing the host, waits for `RegionLoadStatus == NOT_LOADING_REGION`, and only then announces its creature. This proves a world-ready barrier is required. |
| Initial roster | A new peer receives the existing player set, showing the minimum shape of a join-in-progress player snapshot. |

EgoMP forcibly keeps the retail `IsMultiplayerGameActive` result false because
enabling the residual native behavior crashes. That is useful negative
evidence: the reconstruction needs its own multiplayer layer rather than a
simple dormant-feature switch.

## Protocol actually demonstrated

The transport is SLikeNet/RakNet with one host and at most three clients.
There are seven application messages:

| Message | Purpose |
|---|---|
| Connection notification | Announces a newly accepted peer. |
| Create local player | Assigns the joining client's network identity. |
| Create player | Announces one remote player and its initial creature definition/position. |
| Create players | Sends the existing player roster to a joining peer. |
| Destroy player | Removes a disconnected remote player. |
| Movement | Sends network ID, position, and movement acceleration. |
| Rotation | Sends network ID, up vector, and forward vector. |

Control and lifecycle messages use reliable ordered delivery. Movement and
rotation use unreliable sequenced delivery. The host distributes membership
and relays motion, but clients author their own transforms. The claimed
`networkId` in a motion packet is not checked against the packet sender.

The relevant source evidence is:

- [`NetworkMessages.h`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Network/NetworkMessages.h)
- [`NetworkDispatcher.cpp`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Network/NetworkDispatcher.cpp)
- [`NetPlayerManagerLifecycle.cpp`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetPlayerManager/NetPlayerManagerLifecycle.cpp)
- [`NetPlayerManagerMotion.cpp`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetPlayerManager/NetPlayerManagerMotion.cpp)
- [`NetMainGameComponent.cpp`](https://github.com/98thrxse/egomp/blob/816e58f9a0acd099ac8289b6a125bbf95c590b4b/Core/Multiplayer/Fable/NetMainGameComponent/NetMainGameComponent.cpp)

## Boundaries and hazards

- There is no application protocol version, executable/content fingerprint,
  feature negotiation, or schema evolution.
- Packet reads, roster counts, definition indices, vector values, and claimed
  player identities are not validated.
- There are no timestamps or simulation ticks, interpolation, extrapolation,
  prediction, rollback, rate control, or authoritative correction.
- Remote position is hard-snapped when squared drift exceeds `1`; otherwise
  only movement acceleration is injected.
- The global creature movement/facing callback maps can run callbacks in the
  wrong creature update context and more than once per frame.
- Initial teleport-to-host is the only region synchronization. Later region
  transitions, doors, cutscenes, and cross-region interest are unsynchronized.
- There is no health, combat, animation, equipment, inventory, quests,
  NPC/AI, dialogue, interactable, economy, weather, or world-object
  replication.
- There is no authoritative world snapshot, save integration, reconnect,
  host migration, authentication, encryption, malformed-packet defense, or
  NAT/discovery flow.
- Raw definition global indices assume identical game content without checking
  it.
- A constant region-load callback key can be overwritten by overlapping
  joins.
- Hook installation and object creation from `DllMain` rely on work that is
  unsafe under the Windows loader lock.

## Recommended adoption path

1. Independently recover stable player-manager, creature, physics,
   main-component lifecycle, and region-load interfaces.
2. Prove two local player creatures offline in one process, including separate
   control, region loading, and clean teardown.
3. Add a replication registry with stable entity IDs and generation counters;
   keep engine player IDs as an adapter detail.
4. Define a versioned wire envelope with magic, length/type, session ID,
   sender, tick/sequence, and executable/content compatibility hashes.
5. Build deterministic loopback transport and automated two-peer tests before
   real UDP.
6. Make the host authoritative over ownership and simulation. Clients send
   input/intent; the host validates and emits snapshots.
7. Add snapshot buffering, interpolation, correction, ownership checks, and
   rate limits for the first ghost-player milestone.
8. Implement region transitions as a state machine with request, loading,
   ready acknowledgment, activation barrier, timeout, and interest set.
9. Expand replication in layers: animation/actions, combat/health,
   interactables, NPC ownership, then inventory/quests/persistence.
10. Add join snapshots, reconnect, authentication, packet fuzzing, and only
    then internet discovery and NAT traversal.

The modern `GameEvent` codec under `rebuild/modern/multiplayer` may eventually
serve as one reliable event payload. It does not itself supply packet framing,
identity, authority, transport, or synchronization.

## Licensing boundary

EgoMP's project code is GPLv3. Treat its observed behavior, addresses, object
relationships, and hook locations as factual leads, then verify them against
retail bytes, Ghidra, and the Ego PDB and implement independently. Do not copy
its source or bundled SLikeNet binary into this reconstruction without an
explicit compatible licensing decision and a separate transport
license/security review.
