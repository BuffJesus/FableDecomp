// Step 2: clone CREATURE_TRADER_01, repoint its Graphic.modelId to our custom
// skinned mesh (MESH_MESHY_HUNTER, graphics.big id 8113), and add it as a NEW
// def entry CREATURE_MESHY_HUNTER into a COPY of game.bin (+names.bin).
//
// MECHANISM (empirically verified, this session):
//   A CREATURE def's base body mesh = its "Graphic" field (forge stream tag
//   2e6b63c8), payload {u32 kind, u32 modelId, u32 zero, f32 scale, u8 flag}.
//   modelId is a graphics.big MBANK_ALLMESHES entry *id* (not index). For
//   CREATURE_TRADER_01 the Graphic tag sits at payload offset 1114; modelId
//   (originally 5149 = MESH_BS_MALE_MIDDLE_UNCLOTHED_01) is at payload offset 1122.
//
// We clone the FULL payload byte-for-byte (so all Components/stat/appearance
// index references stay valid — we only append, never reorder) and overwrite the
// 4 modelId bytes. addEntry + save() rewrites names.bin (new name appended) and
// re-chunks game.bin; every other entry stays semantically identical.
//
// Build: see 02_build.sh   Run: ./add_creature.exe <retail-root> <out-CompiledDefs-dir> <newModelId>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "forge/bin.hpp"

namespace fs = std::filesystem;

static const char* DONOR = "CREATURE_TRADER_01";
static const char* NEWNAME = "CREATURE_MESHY_HUNTER";
// Graphic field payload offsets within the CREATURE_TRADER_01 payload
static const size_t GRAPHIC_TAG_OFF = 1114;     // little-endian c8 63 6b 2e
static const uint32_t GRAPHIC_TAG = 0x2e6b63c8;
static const size_t MODELID_OFF = 1122;         // GRAPHIC_TAG_OFF + 4(tag) + 4(kind)
static const uint32_t ORIG_MODELID = 5149;      // MESH_BS_MALE_MIDDLE_UNCLOTHED_01

static uint32_t rdU32(const std::vector<uint8_t>& d, size_t o) {
    uint32_t v; std::memcpy(&v, d.data() + o, 4); return v;
}
static void wrU32(std::vector<uint8_t>& d, size_t o, uint32_t v) {
    std::memcpy(d.data() + o, &v, 4);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::fprintf(stderr,
            "usage: %s <retail-game-root> <out-CompiledDefs-dir> <newModelId>\n",
            argv[0]);
        return 2;
    }
    const std::string root = argv[1];
    const fs::path outDir = argv[2];
    const uint32_t newModelId = (uint32_t)std::strtoul(argv[3], nullptr, 10);

    const fs::path defsDir = fs::path(root) / "data" / "CompiledDefs";
    std::printf("opening %s\n", defsDir.string().c_str()); std::fflush(stdout);
    auto game = forge::bin::File::open(defsDir / "names.bin", defsDir / "game.bin");
    std::printf("opened, %zu entries\n", game.entries().size()); std::fflush(stdout);

    const forge::bin::Entry* donor = game.find(DONOR);
    std::printf("find(%s) -> %p\n", DONOR, (void*)donor); std::fflush(stdout);
    if (!donor) { std::fprintf(stderr, "ERROR: donor %s not found\n", DONOR); return 1; }
    std::printf("donor->data.size()=%zu\n", donor->data.size()); std::fflush(stdout);
    std::printf("donor->name=%s\n", donor->name.c_str()); std::fflush(stdout);
    if (game.find(NEWNAME)) {
        std::fprintf(stderr, "ERROR: %s already exists in this bin\n", NEWNAME);
        return 1;
    }

    std::vector<uint8_t> data = donor->data;  // full byte-for-byte clone
    std::printf("donor %s: %zu bytes, def=%s\n", DONOR, data.size(),
                donor->definition.c_str());

    // Sanity: the Graphic tag & original modelId must be exactly where we expect.
    if (data.size() < MODELID_OFF + 4) {
        std::fprintf(stderr, "ERROR: payload too small\n"); return 1;
    }
    uint32_t tag = rdU32(data, GRAPHIC_TAG_OFF);
    uint32_t curModel = rdU32(data, MODELID_OFF);
    if (tag != GRAPHIC_TAG) {
        std::fprintf(stderr, "ERROR: Graphic tag mismatch at %zu: got %08x expect %08x\n",
                     GRAPHIC_TAG_OFF, tag, GRAPHIC_TAG);
        return 1;
    }
    if (curModel != ORIG_MODELID) {
        std::fprintf(stderr, "ERROR: modelId mismatch at %zu: got %u expect %u\n",
                     MODELID_OFF, curModel, ORIG_MODELID);
        return 1;
    }
    std::printf("verified Graphic tag @%zu, modelId=%u @%zu\n",
                GRAPHIC_TAG_OFF, curModel, MODELID_OFF);

    wrU32(data, MODELID_OFF, newModelId);
    std::printf("patched modelId -> %u\n", rdU32(data, MODELID_OFF));

    // Snapshot definition BEFORE addEntry (it can realloc entries_ and dangle donor).
    const std::string defType = donor->definition;
    size_t idx = game.addEntry(defType, NEWNAME, std::move(data));
    donor = nullptr;  // invalidated by potential realloc; do not use again
    std::printf("added %s at global index %zu (def=%s)\n",
                NEWNAME, idx, defType.c_str());

    fs::create_directories(outDir);
    game.save(outDir / "names.bin", outDir / "game.bin");
    std::printf("saved -> %s\n", outDir.string().c_str());

    // Reload & confirm.
    auto chk = forge::bin::File::open(outDir / "names.bin", outDir / "game.bin");
    const forge::bin::Entry* ne = chk.find(NEWNAME);
    if (!ne) { std::fprintf(stderr, "ERROR: reload missing %s\n", NEWNAME); return 1; }
    uint32_t rm = rdU32(ne->data, MODELID_OFF);
    std::printf("RELOAD OK: %s present, def=%s, %zu bytes, modelId=%u\n",
                NEWNAME, ne->definition.c_str(), ne->data.size(), rm);
    if (rm != newModelId) { std::fprintf(stderr, "ERROR: modelId not persisted\n"); return 1; }

    // Also confirm the donor is untouched.
    const forge::bin::Entry* d2 = chk.find(DONOR);
    std::printf("donor %s intact: %zu bytes, modelId=%u\n",
                DONOR, d2->data.size(), rdU32(d2->data, MODELID_OFF));
    std::puts("SUCCESS");
    return 0;
}
