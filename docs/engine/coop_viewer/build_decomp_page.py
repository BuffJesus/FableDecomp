#!/usr/bin/env python3
"""Build a self-contained HTML page showing Fable TLC co-op decompilations.

Each function card has a Cleaned / Raw toggle. The 'cleaned' text is authored
here (a readability pass); the 'raw' text is read verbatim from the recovered
.cpp files so it can't drift from the real decompiler output.
"""
import json, os, sys

ROOT = r"D:\Documents\FableTLC"
OUT  = os.path.join(ROOT, "docs", "cut-coop-decompilations.html")

def rel(*p): return os.path.join(ROOT, *p)

W3 = ("lift", "reports", "wave3", "code")

# addr, class, method, tag, raw file path, note(html), cleaned code
FUNCS = {}

def add(section, addr, cls, name, tag, raw_path, note, cleaned):
    FUNCS.setdefault(section, [])
    FUNCS[section].append(dict(addr=addr, cls=cls, name=name, tag=tag,
                               raw_path=raw_path, note=note, cleaned=cleaned.strip("\n")))

# ---------------------------------------------------------------- Player mgr
add("Player manager — the 4-slot core",
    "0x00449D20", "CPlayerManager", "IsMultiplayerGameActive", "near-verbatim",
    rel(*W3, "00", "44", "0x00449D20_CPlayerManager_IsMultiplayerGameActive.cpp"),
    "<b>Not a stub.</b> It walks four player slots and returns true when a seated player that isn't the "
    "main hero is active. The four-player shape is baked into the engine.",
"""// is any non-main player seated and active? (4-slot scan)
bool CPlayerManager::IsMultiplayerGameActive() const
{
    for (int slot = 0; slot <= 3; ++slot)          // four player slots
    {
        for (size_t i = 0; i < Players.size(); ++i)
        {
            if (Players[i]->playerIndex != slot)
                continue;

            CPlayer* p = GetPlayer(slot);
            if (p->playerIndex != PlayerNeutral)   // skip the main hero
            {
                auto* act = GetPlayerActivity(p);  // 0x00487DC0
                if (act && (act->flags & 1) == 0)
                    return true;                   // seated + active
            }
            break;
        }
    }
    return false;
}""")

add("Player manager — the 4-slot core",
    "0x00449B60", "CPlayerManager", "GetMultiplayerColour", "near-verbatim",
    rel(*W3, "00", "44", "0x00449B60_CPlayerManager_GetMultiplayerColour.cpp"),
    "Hard-coded per-player colours — blue / red / cyan / green for players 1-4. Concrete proof the "
    "design expected up to four distinct on-screen players.",
"""// per-player identity colours (RGBA)
CRGBColour CPlayerManager::GetMultiplayerColour(long player) const
{
    switch (player)
    {
    case 0:  return { 0x00, 0x00, 0xFF, 0xFF };   // player 1 - blue
    case 1:  return { 0xFF, 0x00, 0x00, 0xFF };   // player 2 - red
    case 2:  return { 0x00, 0xFF, 0xFF, 0xFF };   // player 3 - cyan
    case 3:  return { 0x00, 0xFF, 0x00, 0xFF };   // player 4 - green
    default: return { 0xFF, 0xFF, 0xFF, 0xFF };   // white
    }
}""")

add("Player manager — the 4-slot core",
    "0x0044A1A0", "CPlayerManager", "AddPlayerOrAI", "condensed",
    rel(*W3, "00", "44", "0x0044A1A0_global_CPlayerManager_AddPlayerOrAI.cpp"),
    "Seats a new player into the slot vector. Player 4 is given no draw environment (no viewport) while "
    "0-3 get one — read as an AI/non-rendered slot (inference).",
"""// seat a new player (or an AI stand-in) into the slot vector
void CPlayerManager::AddPlayerOrAI(int playerNumber)
{
    if (IsPlayer(playerNumber))
        return;                                     // already seated

    // AI slot gets a JackOfBlades brain; else a real player object
    void* obj = operator new(0x22C);
    obj = obj ? CAIStateGroup_JackOfBladesBase::ctor(obj) : nullptr;
    players.push_back(obj);                          // grow slot vector

    CPlayer* created = players.back();
    CPlayerInit init{};
    init.world          = this->world;
    init.playerNumber   = playerNumber;
    init.playerDefIndex = GetPlayerDefIndexFromNumber(playerNumber);

    bool hasViewport = (playerNumber != 4);          // slot 4 = no viewport
    init.drawEnvironment = hasViewport ? world->DrawGetEnvironment() : nullptr;
    init.playerManager   = this;

    created->PlayerInit(&init);
}""")

# ---------------------------------------------------------------- CoopSpirit
add("The Hero Spirit — CTCCoopSpirit",
    "0x006700F0", "CTCCoopSpirit", "OnCreate", "condensed",
    rel(*W3, "00", "67", "0x006700F0_CTCCoopSpirit_OnCreate.cpp"),
    "Spawn/init: clears the spirit's runtime state and binds its definition. (Def type names in the raw "
    "output are BSim-mislabelled, so they're omitted here rather than asserted.)",
"""// spirit spawn: clear state, bind its definition
void CTCCoopSpirit::OnCreate()
{
    PParticleEmitter = nullptr;
    Master           = nullptr;
    Score            = 0;
    NoMovementTimer  = 0;
    OffscreenCounter = 0;
    MovementVector   = {};

    // resolve this spirit's def from its thing, cache the coop sub-def
    auto* def = CDefinitionManager::GetDef(thing->defIndex);
    def->GetSubDef(&PCoopSpiritDef);
    def->Release();
}""")

add("The Hero Spirit — CTCCoopSpirit",
    "0x006701A0", "CTCCoopSpirit", "UpdateAttractionToMaster", "condensed",
    rel(*W3, "00", "67", "0x006701A0_CTCCoopSpirit_UpdateAttractionToMaster.cpp"),
    "The \"spirit stays near the hero\" logic: past a minimum distance it applies a physics acceleration "
    "pulling the spirit back toward its master. Finished code, not a stub.",
"""// keep the spirit near its master (the main hero)
void CTCCoopSpirit::UpdateAttractionToMaster()
{
    if (Master == nullptr) return;
    if (NoMovementTimer <= idleFrames * kIdleScale) return;  // settled? skip

    C3DVector masterPos = Master->thing->GetPosition();
    C3DVector spiritPos = this->thing->GetPosition();
    C3DVector delta     = masterPos - spiritPos;

    float dist = delta.Length();
    if (dist <= kMinPullDistance) return;            // already close enough

    float pull = min(dist - kMinPullDistance, kMaxPull) * kPullScale;
    C3DVector accel = delta * pull;                  // vector toward master
    this->physics->ApplyAcceleration(&accel);
}""")

add("The Hero Spirit — CTCCoopSpirit",
    "0x00670710", "CTCCoopSpirit", "UpdateScore", "near-verbatim",
    rel(*W3, "00", "67", "0x00670710_CTCCoopSpirit_UpdateScore.cpp"),
    "<b>The takeover mechanic.</b> The headline is score over 19 calling <code>SwapToHero()</code> - "
    "exactly the \"rack up points, take over as the main Hero\" design Molyneux described. The per-slot "
    "loop is bookkeeping around it.",
"""// enough co-op score -> the spirit seizes the main hero slot
void CTCCoopSpirit::UpdateScore()
{
    if (Score > 0x13)                    // score above 19
    {
        for (int slot = 0; slot < 4; ++slot)
        {
            auto* env = CWorld::DrawGetEnvironment(...);
            if ((void*)slot != env)
            {
                CPlayer* p = CPlayerManager::GetPlayer(pm, slot);
                auto* r = ResolvePlayer(p);       // 0x00487DC0
                if (r && r->Field38 < 0)
                    r->scoreIface(0xC7)->value = 0;  // reset per-player marker
            }
        }
        SwapToHero();                    // take over as the main hero
    }
}""")

add("The Hero Spirit — CTCCoopSpirit",
    "0x0066FF20", "CTCCoopSpirit", "SwapToHero", "condensed",
    rel(*W3, "00", "66", "0x0066FF20_CTCCoopSpirit_SwapToHero.cpp"),
    "Performs the swap: reads the spirit's <code>HeroSlot</code>, tells the player manager to switch who's "
    "driving, rebuilds the hero draw data and snaps position to the spirit.",
"""// hand control over: this spirit's player becomes the main hero
void CTCCoopSpirit::SwapToHero()
{
    CIDrawEnvironment* env = world->DrawGetEnvironment();
    auto*     thing = this->thing;
    C3DVector pos   = thing->GetPosition();

    int heroSlot = thing->HeroSlot;
    playerManager->SwapToHero(heroSlot);          // 0x00449D60

    void* def = playerManager->GetSpiritDefName(env);
    displayEngine->AddChild(env, def);            // rebuild draw/def

    CPlayer* p = playerManager->GetPlayer(env);
    p->resolved->SetPosition(pos);                // vtbl slot +0x7C

    thing->FieldB4 = thing->FieldB0;
}""")

add("The Hero Spirit — CTCCoopSpirit",
    "0x0066FB00", "CTCCoopSpirit", "AddExperience", "near-verbatim",
    rel("rebuild", "src", "compiled", "00", "66", "CTCCoopSpirit_AddExperience_0066fb00.cpp"),
    "Trivial - the alter-ego accrues experience. Whole body is a single add.",
"""// alter-ego XP accrual
void CTCCoopSpirit::AddExperience(long amount)
{
    this->experience += amount;
}""")

add("The Hero Spirit — CTCCoopSpirit",
    "0x0062C0E0", "CWorld", "EAMoveSpirit", "condensed",
    rel(*W3, "00", "62", "0x0062C0E0_CWorld_EAMoveSpirit.cpp"),
    "Event-action handler (the \"EA\" prefix): unpacks an (x, y) movement vector from a game-event payload "
    "and applies it to the addressed spirit. This is the piece that would move a remote spirit from a packet.",
"""// game-event handler: nudge a spirit by a movement vector from the payload
void CWorld::EAMoveSpirit(const CGameEvent* event)
{
    void* morph = ResolveMorphHandle(event->morphType);
    if (!MorphHandleValid(morph)) return;

    auto* entry = GetMorphEntry(morph);
    if (entry->spiritEnabledFlag >= 0) return;       // spirit not active

    CTCCoopSpirit* spirit = entry->interface(0xC7);  // coop-spirit iface

    EventPacket buf; buf.copyFrom(event);            // unpack payload
    C3DVector move{};
    buf.read(4, &move.x);
    buf.read(4, &move.y);

    CTCCoopSpirit::ApplyMovementVector(spirit, &move);
}""")

# ---------------------------------------------------------------- Net client
add("Network client — CNetworkClient",
    "0x004AE940", "CNetworkClient", "InitialiseAsLocal", "near-verbatim",
    rel(*W3, "00", "4a", "0x004AE940_CNetworkClient_InitialiseAsLocal.cpp"),
    "Brings the client up and hard-sets <code>Host=false</code> / <code>LocalGame=true</code> every time - "
    "it only ever comes up as a local session. <code>LocalGame</code> is the enable byte at <code>+0x2662</code>.",
"""// bring the client up - always as a LOCAL (non-networked) session
bool CNetworkClient::InitialiseAsLocal(CMainGameComponent* component)
{
    if (InitPlaybackInfo(this) != 1)                 // 0x0099A350
        return false;

    Host          = false;     // never a host
    LocalPlayer   = nullptr;
    HostPlayer    = nullptr;
    LocalFrame    = 0;
    GameComponent = component;
    LocalGame     = true;      // <-- enable byte @ +0x2662, forced local
    FirstTime     = true;
    return true;
}""")

add("Network client — CNetworkClient",
    "0x004AE9D0", "CNetworkClient", "Update", "near-verbatim",
    rel(*W3, "00", "4a", "0x004AE9D0_CNetworkClient_Update.cpp"),
    "The per-frame tick - a no-op unless the enable byte is set. When on, it stashes the frame stamp and "
    "the two checksums that <code>CheckSync</code> later reads.",
"""// per-frame tick - a no-op unless the local-game gate is set
void CNetworkClient::Update(long timeStamp, u32 checksum1, u32 checksum2)
{
    if (LocalGame)                       // enable byte @ +0x2662
    {
        LocalFrame = timeStamp;
        Checksum1  = checksum1;          // stashed for the sync check
        Checksum2  = checksum2;
    }
}""")

add("Network client — CNetworkClient",
    "0x004AEA70", "CNetworkClient", "IsFreeToRender", "near-verbatim",
    rel(*W3, "00", "4a", "0x004AEA70_CNetworkClient_IsFreeToRender.cpp"),
    "Render gate: yields the frame while a server update is pending. Ordinary finished logic.",
"""// ok to render this frame? (yield while a server update is pending)
bool CNetworkClient::IsFreeToRender() const
{
    if (LocalGame)
        return !GameComponent->IsTimeForServerUpdate(LocalFrame);
    return true;
}""")

add("Network client — CNetworkClient",
    "0x004AEBA0", "CNetworkClient", "GetGameEventPackageSet", "near-verbatim",
    rel(*W3, "00", "4a", "0x004AEBA0_CNetworkClient_GetGameEventPackageSet.cpp"),
    "Outbound events - only produced in a local game; otherwise returns nothing. The networked branch is gone.",
"""// outbound events - only produced in a local game
bool CNetworkClient::GetGameEventPackageSet(CGameEventPackageSet* out)
{
    if (LocalGame)
        return GetLocalGameEventPackageSet(out);
    return false;
}""")

add("Network client — CNetworkClient",
    "0x004AEAA0", "CNetworkClient", "GetLocalGameEventPackageSet", "condensed",
    rel(*W3, "00", "4a", "0x004AEAA0_CNetworkClient_GetLocalGameEventPackageSet.cpp"),
    "<b>The producer of the sync event.</b> Once per server-update tick it builds a type-1 event carrying "
    "both world checksums and the frame number - the exact three values <code>CheckSync</code> reads and "
    "then discards.",
"""// build this frame's package: a single "server update" sync event
bool CNetworkClient::GetLocalGameEventPackageSet(CGameEventPackageSet* out)
{
    ServerUpdateEvent ev;
    ev.UpdateReady = GameComponent->IsTimeForServerUpdate(LocalFrame);
    if (!ev.UpdateReady)
        return false;

    ev.Type = 1;                          // <-- type 1 = the sync event
    ev.buf.write(4, &Checksum1);          // world checksum #1
    ev.buf.write(4, &Checksum2);          // world checksum #2
    ev.buf.write(4, &LocalFrame);         // frame number
    ++LocalFrame;

    LocalEventPackage.AddEvent((CGameEvent*)&ev);
    LocalEventPackage.Frame = LocalFrame;
    out->AddPackage(&LocalEventPackage);
    LocalEventPackage.Clear();
    return true;
}""")

# ---------------------------------------------------------------- Protocol
add("Replication protocol — CGameEventPackageSet",
    "0x009F19A0", "CGameEventPackageSet", "CompressIntoBuffer", "condensed",
    rel(*W3, "00", "9f", "0x009F19A0_CGameEventPackageSet_CompressIntoBuffer.cpp"),
    "Packs the whole package-set into a flat byte buffer. \"Compress\" is a misnomer - it's dense "
    "byte-packing, no actual compression. Each event: 15-bit id + high-bit flag, type, length, payload.",
"""// pack a package-set into a flat byte buffer (dense, not compressed)
long CGameEventPackageSet::CompressIntoBuffer(unsigned char* out) const
{
    unsigned char* p = out;
    *p++ = (u8)NoPackages;

    for (each package pkg in Packages[0 .. NoPackages))
    {
        *p++ = (u8)pkg.EventCount;
        write_u32(p, pkg.Unknown0x00); p += 4;

        for (each event ev in pkg.Events[0 .. EventCount))
        {
            u16 hdr = ev.EventId | (ev.HasHighBit ? 0x8000 : 0);
            write_u16(p, hdr);  p += 2;
            *p++ = ev.Type;
            *p++ = ev.PayloadSize;
            memcpy(p, ev.Payload, ev.PayloadSize);   // 0..32 bytes
            p += ev.PayloadSize;
        }
    }
    return (long)(p - out);          // total bytes written
}""")

add("Replication protocol — CGameEventPackageSet",
    "0x009F1AC0", "CGameEventPackageSet", "InitFromCompressedBuffer", "condensed",
    rel(*W3, "00", "9f", "0x009F1AC0_CGameEventPackageSet_InitFromCompressedBuffer.cpp"),
    "The exact inverse. Note: the length byte is read straight back with <b>no bounds check</b> - the event "
    "object only holds 32 payload bytes, so a hostile length would overflow. A modern reader must reject &gt; 32.",
"""// unpack the flat buffer back into a package-set (exact inverse)
void CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* in)
{
    NoPackages = 0;
    u8 packageCount = *in++;

    for (u8 i = 0; i < packageCount; ++i)
    {
        CGameEventPackage pkg;
        u8 eventCount   = *in++;
        pkg.Unknown0x00 = read_u32(in); in += 4;

        for (u8 e = 0; e < eventCount; ++e)
        {
            CGameEvent ev;
            in += ev.InitFromCompressedBuffer(in);   // per-event decode
            pkg.AddEvent(&ev);
        }
        Packages[NoPackages++] = pkg;
    }
}""")

# ---------------------------------------------------------------- CheckSync
add("The gutted part — CheckSync",
    "0x004165E8", "CMainGameComponent", "CheckSync", "near-verbatim",
    rel(*W3, "00", "41", "0x004165E8_CMainGameComponent_CheckSync.cpp"),
    "<b>Reads everything, compares nothing.</b> It pulls the remote checksums + frame and the local world "
    "checksum, then falls off the end: no compare, no desync flag, no report. The decision logic was "
    "stripped out.",
"""// desync detector - reads everything it needs, then compares NOTHING
void CMainGameComponent::CheckSync(CGameEventPackage* package)
{
    u32 checksum1 = 0, checksum2 = 0, frame = 0;

    PeekWorldChecksum();                    // local checksum... discarded

    for (u32 i = 0; i < package->count(); ++i)
    {
        CGameEvent* ev = package->at(i);
        if (ev->Type == 1)                  // the sync event
        {
            MemoryBuffer buf; buf.copyFrom(ev);
            buf.read(4, &checksum1);        // remote checksum #1
            buf.read(4, &checksum2);        // remote checksum #2
            buf.read(4, &frame);            // remote frame
        }
    }
    // no compare, no desync flag, no report - the check was gutted
}""")

# ---------------------------------------------------------------- assemble
SECTION_ORDER = [
    ("Player manager — the 4-slot core",
     "Where multiplayer is switched on and where players get seated. IsMultiplayerGameActive is the "
     "“is co-op running?” predicate people assumed was a stub — it isn't."),
    ("The Hero Spirit — CTCCoopSpirit",
     "The second player's body when they're not the main hero: a spirit that follows the hero, earns a "
     "co-op score, and can seize control once that score is high enough."),
    ("Network client — CNetworkClient",
     "The layer that would have taken it online. In the shipped build it's forced to a local-only session, "
     "but the outbound-event and checksum machinery is all still here."),
    ("Replication protocol — CGameEventPackageSet",
     "The wire format. Pack and unpack are exact inverses, so “what goes over the wire” is fully "
     "recovered: a dense per-frame stream of small event records."),
    ("The gutted part — CheckSync",
     "The desync detector. It gathers every value it would need to catch a desync, then does nothing with "
     "them — the smoking gun for a feature torn down before ship."),
]

data_sections = []
missing = []
for title, blurb in SECTION_ORDER:
    fns = []
    for f in FUNCS.get(title, []):
        try:
            with open(f["raw_path"], "r", encoding="utf-8", errors="replace") as fh:
                raw = fh.read()
            # strip leading line-number artefacts? files are plain source; keep as-is
            raw = raw.replace("\r\n", "\n").rstrip("\n")
        except OSError as e:
            missing.append((f["raw_path"], str(e)))
            raw = "// (raw source not found: %s)" % os.path.basename(f["raw_path"])
        fns.append(dict(addr=f["addr"], cls=f["cls"], name=f["name"], tag=f["tag"],
                        note=f["note"], cleaned=f["cleaned"], raw=raw))
    data_sections.append(dict(title=title, blurb=blurb, fns=fns))

DATA_JSON = json.dumps(data_sections, ensure_ascii=False).replace("</", "<\\/")

HTML = r"""<!doctype html>
<html lang="en"><head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Fable TLC — recovered co-op internals</title>
<style>
  :root{
    --ground:#0e1217; --panel:#151b22; --panel-2:#11161c;
    --border:#242d38; --border-2:#1c242d; --text:#cdd6e0; --muted:#7f8b98;
    --gold:#d9a441; --gold-dim:#a9843a;
    --kw:#b48ff0; --type:#5cb8b2; --num:#9ece6a; --str:#e0a577; --comment:#66727f;
    --mono: ui-monospace,"Cascadia Code","JetBrains Mono","SFMono-Regular",Consolas,monospace;
    --sans: ui-sans-serif,system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;
  }
  *{box-sizing:border-box} html,body{margin:0}
  body{
    background:radial-gradient(1100px 460px at 82% -8%,rgba(217,164,65,.06),transparent 60%),var(--ground);
    color:var(--text); font-family:var(--sans);
    padding:clamp(20px,4vw,46px) clamp(14px,4vw,44px) 56px;
  }
  .sheet{max-width:880px;margin:0 auto}
  header.head{border:1px solid var(--border);border-radius:14px;
    background:linear-gradient(180deg,rgba(217,164,65,.05),transparent 55%),var(--panel-2);padding:24px 26px}
  .eyebrow{font-size:11px;letter-spacing:.2em;text-transform:uppercase;color:var(--gold);font-weight:600}
  h1{font-size:clamp(21px,3.4vw,28px);line-height:1.18;margin:9px 0 8px;text-wrap:balance;
    font-weight:650;letter-spacing:-.012em}
  .sub{color:var(--muted);font-size:14.5px;line-height:1.58;max-width:64ch;margin:0}
  .binline{margin-top:16px;font-family:var(--mono);font-size:12px;color:var(--muted);
    display:flex;flex-wrap:wrap;gap:7px 18px}
  .binline b{color:var(--text);font-weight:500}
  h2.sec{font-size:12.5px;letter-spacing:.14em;text-transform:uppercase;color:var(--gold-dim);
    font-weight:600;margin:38px 0 2px;padding-bottom:8px;border-bottom:1px solid var(--border-2)}
  .sec-blurb{color:var(--muted);font-size:13.5px;line-height:1.55;margin:10px 0 0;max-width:64ch}
  .fn{border:1px solid var(--border);border-radius:11px;overflow:hidden;background:var(--panel);margin-top:16px}
  .fn-bar{display:flex;flex-wrap:wrap;align-items:center;gap:9px 12px;padding:11px 15px;
    background:var(--panel-2);border-bottom:1px solid var(--border)}
  .addr{font-family:var(--mono);font-size:12.5px;font-weight:600;color:var(--ground);background:var(--gold);
    padding:3px 9px;border-radius:6px;letter-spacing:.02em;font-variant-numeric:tabular-nums}
  .sym{font-family:var(--mono);font-size:13.5px;color:var(--text);margin-right:auto}
  .sym .cls{color:var(--type)}
  .tag{font-size:10.5px;letter-spacing:.06em;text-transform:uppercase;font-weight:600;padding:2px 8px;
    border-radius:20px;border:1px solid var(--border);color:var(--muted);white-space:nowrap}
  .tag.verbatim{color:#8fce7f;border-color:#2c4030;background:rgba(143,206,127,.06)}
  .tag.condensed{color:var(--gold);border-color:#4a3c1e;background:rgba(217,164,65,.06)}
  .toggle{display:inline-flex;border:1px solid var(--border);border-radius:7px;overflow:hidden}
  .toggle button{font-family:var(--sans);font-size:11.5px;font-weight:600;letter-spacing:.03em;
    padding:4px 11px;background:transparent;color:var(--muted);border:0;cursor:pointer}
  .toggle button+button{border-left:1px solid var(--border)}
  .toggle button.on{background:var(--gold);color:var(--ground)}
  .toggle button:focus-visible{outline:2px solid var(--gold);outline-offset:-2px}
  .code{font-family:var(--mono);font-size:12.75px;line-height:1.62;padding:12px 0 14px;overflow-x:auto}
  .code[hidden]{display:none}
  .row{display:flex;gap:14px;padding:0 16px}
  .row:hover{background:rgba(255,255,255,.018)}
  .gutter{width:2.6ch;flex:none;text-align:right;color:#414c58;user-select:none;
    font-variant-numeric:tabular-nums}
  .src{white-space:pre-wrap;word-break:break-word;margin:0;min-width:0}
  .cm{color:var(--comment);font-style:italic}
  .cm .ad{color:var(--gold);font-style:normal;font-weight:600}
  .kw{color:var(--kw)} .ty{color:var(--type)} .nu{color:var(--num)} .st{color:var(--str)}
  .note{border-top:1px solid var(--border);background:var(--panel-2);padding:11px 15px;
    font-size:12.75px;line-height:1.55;color:#aeb9c5}
  .note b{color:var(--gold);font-weight:600} .note code{font-family:var(--mono);color:var(--text)}
  .rawhint{color:var(--muted);font-style:italic}
  footer.foot{margin-top:34px;color:var(--muted);font-size:12px;line-height:1.55;display:flex;gap:9px;
    align-items:flex-start;border-top:1px solid var(--border-2);padding-top:16px}
  footer.foot .dot{color:var(--gold)}
</style>
</head><body>
<div class="sheet">
  <header class="head">
    <div class="eyebrow">Fable: The Lost Chapters &middot; Static RE</div>
    <h1>The cut co-op “Alter Ego” system, straight from the binary</h1>
    <p class="sub">The functions behind the abandoned multiplayer / local co-op code. Toggle each card
    between a <b>Cleaned</b> readability pass and the <b>Raw</b> decompiler output. Every address is
    unchanged from retail.</p>
    <div class="binline">
      <span>target: <b>retail Fable.exe (Steam)</b></span>
      <span>ImageBase: <b>0x400000</b></span>
      <span>arch: <b>x86-32 PE32</b></span>
    </div>
  </header>
  <main id="fns"></main>
  <footer class="foot">
    <span class="dot">◆</span>
    <span>Reverse-engineered and written up by an AI assistant (Claude) as part of a Fable RE project.
    The <b>Raw</b> tab is the verbatim recovered decompiler output; the <b>Cleaned</b> tab is a
    readability pass over it. Addresses are unchanged from retail.</span>
  </footer>
</div>

<script>
const DATA = __DATA__;

const KW = new Set(("bool void int long short const return if else for while do continue break true false "
 + "auto switch case default struct class size_t unsigned signed char float double new this sizeof each in "
 + "static namespace using template typename operator void nullptr").split(" "));

function esc(s){return s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;");}
function hlComment(c){return esc(c).replace(/0x[0-9A-Fa-f]+/g,m=>'<span class="ad">'+m+'</span>');}
function highlight(code){
  const re=/(\/\/[^\n]*)|("(?:[^"\\]|\\.)*")|(\b0x[0-9A-Fa-f]+\b)|([A-Za-z_][A-Za-z0-9_]*)|(\b\d+\b)/g;
  let out="",last=0,m;
  while((m=re.exec(code))){
    out+=esc(code.slice(last,m.index));
    if(m[1])      out+='<span class="cm">'+hlComment(m[1])+'</span>';
    else if(m[2]) out+='<span class="st">'+esc(m[2])+'</span>';
    else if(m[3]) out+='<span class="nu">'+esc(m[3])+'</span>';
    else if(m[4]){const w=m[4];
      if(KW.has(w))                 out+='<span class="kw">'+w+'</span>';
      else if(/^[CE][A-Z]/.test(w)) out+='<span class="ty">'+w+'</span>';
      else                          out+=esc(w);}
    else if(m[5]) out+='<span class="nu">'+m[5]+'</span>';
    last=re.lastIndex;
  }
  out+=esc(code.slice(last));
  return out;
}
function renderCode(code){
  return highlight(code).split("\n").map((ln,i)=>
    '<div class="row"><span class="gutter">'+(i+1)+'</span><pre class="src">'+(ln||" ")+'</pre></div>'
  ).join("");
}
function tagClass(t){return t==="near-verbatim"?"verbatim":"condensed";}

let uid=0;
const root=document.getElementById("fns");
root.innerHTML=DATA.map(sec=>
  '<h2 class="sec">'+esc(sec.title)+'</h2>'+
  '<p class="sec-blurb">'+esc(sec.blurb)+'</p>'+
  sec.fns.map(fn=>{
    const id="fn"+(uid++);
    return '<section class="fn">'+
      '<div class="fn-bar">'+
        '<span class="addr">'+fn.addr+'</span>'+
        '<span class="sym"><span class="cls">'+fn.cls+'</span>::'+fn.name+'</span>'+
        '<span class="tag '+tagClass(fn.tag)+'">'+fn.tag+'</span>'+
        '<span class="toggle" role="tablist">'+
          '<button class="on" data-t="'+id+'" data-v="clean" role="tab">Cleaned</button>'+
          '<button data-t="'+id+'" data-v="raw" role="tab">Raw</button>'+
        '</span>'+
      '</div>'+
      '<div class="code" id="'+id+'-clean">'+renderCode(fn.cleaned)+'</div>'+
      '<div class="code" id="'+id+'-raw" hidden>'+renderCode(fn.raw)+'</div>'+
      '<div class="note">'+fn.note+'</div>'+
    '</section>';
  }).join("")
).join("");

root.addEventListener("click",e=>{
  const b=e.target.closest("button[data-t]"); if(!b) return;
  const id=b.dataset.t, v=b.dataset.v;
  document.getElementById(id+"-clean").hidden = (v!=="clean");
  document.getElementById(id+"-raw").hidden   = (v!=="raw");
  b.parentElement.querySelectorAll("button").forEach(x=>x.classList.toggle("on",x===b));
});
</script>
</body></html>"""

HTML = HTML.replace("__DATA__", DATA_JSON)

with open(OUT, "w", encoding="utf-8") as fh:
    fh.write(HTML)

print("wrote", OUT, "(%d bytes)" % len(HTML))
print("functions:", sum(len(s["fns"]) for s in data_sections))
if missing:
    print("MISSING RAW FILES:")
    for p, e in missing:
        print("  ", p, "->", e)
else:
    print("all raw sources found")
