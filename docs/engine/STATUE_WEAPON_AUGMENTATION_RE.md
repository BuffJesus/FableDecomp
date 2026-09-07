# Statue hit weapon-augmentation RE handoff

Date: 2026-08-28

## Implementation checkpoint

ForgeFSE now exposes the proposed event-oriented query as
`CScriptThing:MsgGetHitByWeaponAugmentations(hitterScriptName)`. The initial
consumer is `mods/StatueMasterRestoration/Entities/LookoutPointStatue.lua`.
It performs one native event lookup, copies at most 32 validated vector entries,
translates all ten verified bit values, preserves unknown IDs, and returns an
ORed `augmentationMask`. No event/vector pointer crosses into Lua.

Offline build and mock checks pass. Melee behavior is now live-validated in
ForgeTest: Flame returns type `4`/mask `4`, an unaugmented weapon returns an
empty vector/mask zero, same-window lookup repeats, and the event expires after
one yield. Projectile population remains to be validated separately.

## Outcome

ForgeFSE should inspect the augmentation list stored in the **hit event**, not
the hero's presently equipped weapon and not an inventory item reconstructed
from a definition name.  A `CEventHitBy` owns a copied `CHitParameters`; that
payload records both the exact weapon involved in the strike and the
augmentation types which applied to that strike.

This is the correct primitive for targetable Statue Master reconstruction:

```lua
local hit = statue:GetLastHitAugmentations("Hero")
if hit and hit:Has("Flame") then
    -- solve the Fire statue
end
```

The Lua spelling is a proposed ForgeFSE API, not a recovered retail API.

## Retail TLC structures

The PDB-backed `FableWin.exe` and the ported retail type schema agree on these
layouts (32-bit pointers):

```cpp
// sizeof = 0x10
struct NEntityEvents::CEventHitBase {
    void* vftable;                         // +0x00
    // event-base state                    // +0x04
    CCountedPointer<CHitParameters> hit;   // +0x08
};

// Retail VC7.1 sizeof = 0x60
struct CHitParameters {
    // strike flags and vectors            // +0x00 .. +0x2f
    std::map<EDamageAttribute, float> damageMap; // +0x30, 0x0c bytes
    std::vector<EObjectAugmentationType> augmentations; // +0x3c
    CHitLocation* hitLocation;             // +0x48
    CDefPointer<const CMaterialDef> material; // +0x4c
    CIntelligentPointer<CThing> thingHitting; // +0x50
    CIntelligentPointer<CThing> weapon;    // +0x58
};
```

The VC7-era vector at `CHitParameters + 0x3C` is the ordinary three-pointer
layout: begin, end, capacity.  Its elements are 32-bit
`EObjectAugmentationType` values.  A safe read therefore requires:

1. a non-null event and counted `CHitParameters` object;
2. `begin <= end <= capacity`;
3. `(end - begin) % 4 == 0`; and
4. a small defensive element cap before copying into Lua-owned values.

Do not expose native vector pointers or event pointers to Lua; the entity event
queue owns their lifetime.

## Native evidence and addresses

Retail TLC `Fable.exe` image base is `0x00400000`.

| Retail address | Symbol / relevance |
|---|---|
| `0x008D1130` | `CGameScriptThing::MsgIsHitByWithWeapon(hitterName, weaponName)`; 499-byte event-filtering implementation |
| `0x008D47A0` | `CTCEntityEvents::GetEvent<CEventHitBy,...>` basic time-window filter |
| `0x008D4840` | `GetEvent<CEventHitBy,...CIsHitEventHitByThingWithScriptName>` |
| `0x008D49B0` | `GetEvent<CEventHitBy,...CEventFilter_HitEventWithWeapon...>` used by weapon-filtered hit messages |
| `0x007660C0` | `CTCObjectAugmentations::HasAugmentationOfType(EObjectAugmentationType)` |
| `0x00766050` | `CTCObjectAugmentations::HasCombinationOfAugmentations(EObjectAugmentationType)` |
| `0x004AAB90` | base `CScriptThing::MsgIsHitByWithWeapon` forwarding stub (vtable slot 24), byte-matched reconstruction |

PDB-backed `FableWin.exe` cross-checks:

| FableWin address | Symbol |
|---|---|
| `0x02B012D0` | `CGameScriptThing::MsgIsHitByWithWeapon` |
| `0x02B06DA0` | basic time-window `GetEvent<CEventHitBy,...>` |
| `0x02B06F80` | hitter-script-name `GetEvent<CEventHitBy,...>` |
| `0x02B07340` | weapon-filtered `GetEvent<CEventHitBy,...>` |
| `0x025ECCF0` | `CTCObjectAugmentations::HasAugmentationOfType` |
| `0x025EB500` | `CTCObjectAugmentations::GetAugmentations(vector<long>&)` |
| `0x025A7ED0` | `CEventHitBase::GetHitParams() const` |

`CEventHitBase::GetHitParams()` returning a const reference and the
`CEventHitBase + 0x08` counted pointer independently support the ownership
model.  `CHitParameters::SetWeapon(CThing&)` is at FableWin `0x02810200`.

## Why existing ForgeFSE calls are insufficient

ForgeFSE already exposes entity vtable slot 24 as:

```cpp
bool MsgIsHitByWithWeapon(
    const CCharString& hitterName,
    const CCharString& weaponName) const;
```

Both strings are **filters**.  The weapon name is not an output parameter and
the method returns only a boolean.  Quest APIs
`GetPreviouslyWieldedMeleeWeaponName` and
`GetPreviouslyWieldedRangedWeaponName` return definition/name state rather
than the hit's augmentation-bearing weapon instance.  They are useful only as
a compatibility fallback and can race with a weapon change after the event.

## Recommended ForgeFSE surface

Add one event-oriented entity query, preserving the same message cursor/time
window semantics as `CGameScriptThing::MsgIsHitByWithWeapon`:

```cpp
struct FseHitAugmentations {
    bool found;
    bool projectile;
    std::vector<std::int32_t> types; // copied before returning to Lua
};

FseHitAugmentations MsgGetHitByWeaponAugmentations(
    const CCharString& hitterScriptName) const;
```

Lua may receive a table such as:

```lua
{
  found = true,
  projectile = false,
  types = { "Flame", "Silver" },
  typeIds = { 4, 2 }
}
```

Prefer stable string names in quest scripts, while retaining numeric IDs as
diagnostic data.  The retail enum values are now data-verified below.

## Verified retail augmentation bit values

`EObjectAugmentationType` is a 32-bit **bitmask enum**, not a sequential enum.
Retail `game.bin` `CAugmentationDef::Type` fields and the native combination
predicate establish this mapping:

| Retail object | Statue clue | Type |
|---|---|---:|
| `OBJECT_SHARPENING_AUGMENTATION` | Steel | `0x00000001` |
| `OBJECT_SILVER_AUGMENTATION` | Silver | `0x00000002` |
| `OBJECT_FLAME_AUGMENTATION` | Fire | `0x00000004` |
| `OBJECT_LIGHTNING_AUGMENTATION` | Lightning | `0x00000008` |
| `OBJECT_PIERCING_AUGMENTATION` | Diamond | `0x00000010` |

The ten retail augmentation definitions appear in object order and decode as:

| Object | `CAugmentationDef` index | Type |
|---|---:|---:|
| Sharpening | 11358 | `0x001` |
| Piercing | 11361 | `0x010` |
| Silver | 11364 | `0x002` |
| Flame | 11367 | `0x004` |
| Hobbe Killer | 11370 | `0x100` |
| Lightning | 11373 | `0x008` |
| Experience | 11376 | `0x080` |
| Health | 11379 | `0x020` |
| Mana | 11382 | `0x040` |
| Bandit Slayer | 11385 | `0x200` |

This is independently consistent with retail
`CTCObjectAugmentations::HasCombinationOfAugmentations @ 0x00766050`: it walks
installed augmentation defs, ORs their `Type` fields, and returns true exactly
when `(combinedTypes & requestedMask) == requestedMask`.

`HasAugmentationOfType @ 0x007660C0` instead compares each installed def's
`CAugmentationDef + 0x2C` type for equality, so use the combination helper—or
explicitly scan the hit vector—when accepting a multi-bit requested mask.

An even smaller puzzle-specific predicate is possible:

```cpp
bool MsgIsHitByWithAugmentation(
    const CCharString& hitterScriptName,
    EObjectAugmentationType requiredType) const;
```

The table-returning form is preferable: one event lookup supports multi-augmented
weapons, debugging, ranged strikes, and later world events without five native
calls consuming or advancing message state unexpectedly.

## Implementation route

1. Start from the retail/PDB decompilation of
   `CGameScriptThing::MsgIsHitByWithWeapon`, retaining its event-time bounds and
   `CIsHitEventHitByThingWithScriptName` filter behavior.
2. Return the matched `CEventHitBy*` internally instead of collapsing directly
   to `bool`.
3. Obtain its `const CHitParameters&` (or safely dereference the counted pointer
   at event `+0x08`).
4. Copy `[hit + 0x3C.begin, hit + 0x3C.end)` immediately.
5. Convert only known enum values to stable names.  Preserve unknown values as
   numeric diagnostics rather than rejecting the hit.
6. Bind the copied result through sol2; never retain engine-owned pointers.

### Recovered message-window behavior

`CGameScriptThing::MsgIsHitByWithWeapon @ 0x008D1130` performs these checks:

1. the wrapped native thing exists;
2. it is not in limbo (`thing + 0x91`, bit 0 clear);
3. it owns the component-vector capability (`thing + 0x28`, bit 2 set);
4. resolve `CTCEntityEvents` interface type `0x42` from the thing's component
   vector map;
5. obtain the script execution context's before/on and after tick boundaries;
6. treat `"NO_WEAPON"` as a special no-weapon predicate, otherwise resolve the
   supplied weapon definition name to its global def index; and
7. scan `CTCEntityEvents` for the first type-1 `CEventHitBy` matching hitter,
   weapon/no-weapon, and the time window.

The weapon-filtered template at `0x008D49B0` walks the event list and retains a
counted reference while testing.  It does **not** erase or consume the event.
Its time test is an open/closed interval: `createdAfter < eventTick <=
createdBeforeOrOn`.  Therefore consumption semantics come from the advancing
script execution window, not mutation of the event queue.

The augmentation table must be returned by the same native lookup which finds
the event.  Do not implement Lua as `MsgIsHitByWithWeapon(...)` followed by a
second `GetLastHitAugmentations(...)`: two lookups can cross a coroutine yield or
script-window update, and the boolean API discards the matched event pointer.

Calling `CTCObjectAugmentations::HasAugmentationOfType` on
`CHitParameters::weapon` is a valid cross-check, but it is not the best primary
answer.  The hit's own augmentation vector is the set actually attached to the
damage event and remains available for projectile strikes where current weapon
state is especially unreliable.

## Statue integration notes

- The two Greatwood objects flanking the Piercing chest are
  `OBJECT_LOOKOUT_POINT_STATUE`; retail gives them no `ScriptName`.  The authored
  event must assign unique script names and an entity script.
- Make the targets targetable but indestructible.  The event script should
  react to a hero-authored weapon hit, inspect the returned augmentation set,
  set one persistent master-quest seal, and debounce repeat hits.
- Required recovered clue mapping remains:
  Flame -> Fire; Extra Damage -> Steel/Sharpening; Silver -> Silver;
  Diamond -> Piercing; Lightning -> Lightning.
- Inventory category names are evidence for the clue mapping, not the runtime
  `EObjectAugmentationType` numeric ABI.

## Verification checklist

Run a small ForgeFSE probe before implementing the full event:

1. plain melee weapon -> found hit, empty augmentation list;
2. one Flame augmentation -> exactly Flame;
3. two different augmentations -> both, with deterministic Lua representation;
4. bow with augmentation -> projectile hit reports the bow's applied type;
5. rapid weapon swap after impact -> result remains the impact weapon's set;
6. non-hero strike -> rejected by the hitter filter;
7. repeated polling without a new hit -> matches existing message cursor rules
   and does not re-award the seal.

Static RE and retail data establish the storage, bit values, and lookup window.
A live probe is still required to validate the new wrapper at the Lua boundary,
especially projectile population and repeat polling across coroutine yields.

### Live probe result (2026-08-29)

The corrected retail bridge captured two independent Flame strikes and two
independent unaugmented strikes from `SCRIPT_NAME_HERO`. Flame returned type ID
`4` and mask `4`; both plain hits returned an empty type list and mask `0`.
Every hit repeated within the same execution window and disappeared after the
next coroutine yield. Positive/negative melee discrimination and the Hero-name
filter are therefore live-validated. Multi-augmentation and projectile cases
remain untested.

## Verified facts (from FINDINGS log)

- **2026-08-28 — Hit-local weapon augmentation inspection.** Authoritative source for a scripted
  statue strike = the matched `CEventHitBy` payload, not the hero's current weapon. `CEventHitBase`
  owns a counted `CHitParameters`; retail 0x60-byte hit parameters store
  `vector<EObjectAugmentationType>` at `+0x3C`, hitter at `+0x50`, striking weapon at `+0x58`.
  `CGameScriptThing::MsgIsHitByWithWeapon` @ 0x008D1130 filters event/time/hitter/weapon but returns
  only a boolean; ForgeFSE sibling `MsgGetHitByWeaponAugmentations` returns names, IDs, ORed mask.
  - `game.bin` bit values: Sharpening/Steel `0x01`, Silver `0x02`, Flame/Fire `0x04`, Lightning
    `0x08`, Piercing/Diamond `0x10`, Health `0x20`, Mana `0x40`, Experience `0x80`, Hobbe Killer
    `0x100`, Bandit Slayer `0x200`. `HasCombinationOfAugmentations @ 0x00766050` = bitmask OR +
    containment test. `GetEvent @ 0x008D49B0`: events are not removed; lookup uses `createdAfter <
    eventTick <= createdBeforeOrOn` with bounds from the script execution context.
  - Lookout Point prototype accepts Piercing/Diamond `0x10`, debounces its namespaced seal, and
    repeats the lookup in-window and after one yield; offline Lua mocks + Release x86 build pass;
    projectile/coroutine-window behavior remain runtime claims.
