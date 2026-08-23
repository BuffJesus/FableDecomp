// Replay FableForge's ported CLocalDetailPrimitiveRepeatedMesh subsection
// builder (forge::stbbake::buildSubSectionsAndObjectRemapTable, the port of
// FableWin 0x02EDF740 / 0x02EDFB20) over every retail subsection table in the
// oracle and report how many it reproduces byte-for-byte.
//
// Two unknown authoring inputs stop this from being a whole-table byte diff:
//   * the leaf threshold T = min(4, max(1, 128 / mesh polyCount)); the polygon
//     count is not in the chunk, so the harness tries T = 1..4 and reports the
//     histogram of which one each retail record needs.
//   * the source mesh bounding sphere; the harness substitutes the placement
//     position for its centre and R * instanceScale for its radius, sweeping R.
// The integer lanes (count[4] @0x40, startIndex[4] @0x44, childOffset[4] @0x48)
// are the algorithm's entire combinatorial output and are insensitive to R, so
// they are scored separately from the float lanes.
//
// Build (adjust the FableForge path if it moved):
//   c++ -std=c++17 -O2 -I D:/Code/FableForge/libs/forgecore/include \
//       tools/subsection_port_check.cpp D:/Code/FableForge/build/libforgecore.a \
//       -static -o subsection_port_check
// Run:
//   python tools/subsection_port_check.py tmp/subsection_oracle/oracle.jsonl p.tsv
//   ./subsection_port_check p.tsv
#include "forge/stbbake.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace {

struct Row {
    int n = 0;
    std::vector<float> b;       // x,y,z,scale per instance
    std::vector<uint8_t> table; // retail bytes, elementCount * 0x50
};

std::vector<Row> load(const char* path) {
    std::ifstream in(path);
    if (!in) { std::fprintf(stderr, "cannot open %s\n", path); std::exit(2); }
    std::vector<Row> rows;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string nStr, bStr, hex;
        std::getline(ls, nStr, '\t');
        std::getline(ls, bStr, '\t');
        std::getline(ls, hex, '\t');
        Row r;
        r.n = std::atoi(nStr.c_str());
        std::istringstream bs(bStr);
        double v;
        while (bs >> v) r.b.push_back(float(v));
        if (int(r.b.size()) != 4 * r.n) {
            std::fprintf(stderr, "row has %zu floats for %d objects\n",
                         r.b.size(), r.n);
            std::exit(2);
        }
        for (size_t i = 0; i + 1 < hex.size(); i += 2)
            r.table.push_back(uint8_t(std::stoul(hex.substr(i, 2), nullptr, 16)));
        if (r.table.empty() || r.table.size() % 0x50 != 0) {
            std::fprintf(stderr, "row table is not a multiple of 0x50\n");
            std::exit(2);
        }
        rows.push_back(std::move(r));
    }
    return rows;
}

// Returns 0 = no T matched, 1 = integer lanes matched, 2 = whole element matched.
int score(const Row& r, float meshRadius, int* usedT) {
    const size_t elems = r.table.size() / 0x50;
    std::vector<forge::stbbake::SubsectionSphere> spheres;
    spheres.reserve(size_t(r.n));
    for (int i = 0; i < r.n; ++i)
        spheres.push_back({r.b[i * 4], r.b[i * 4 + 1], r.b[i * 4 + 2],
                           meshRadius * r.b[i * 4 + 3]});
    int best = 0;
    for (int T = 1; T <= 4; ++T) {
        forge::stbbake::SubsectionTable t;
        try {
            t = forge::stbbake::buildSubSectionsAndObjectRemapTable(spheres, T);
        } catch (const std::exception&) { continue; }
        if (!t.present || t.elements.size() != elems) continue;
        const auto mine = forge::stbbake::serializeSubsectionElements(t.elements);
        bool ints = true, floats = true;
        for (size_t k = 0; k < elems; ++k) {
            for (size_t o = 0x40; o < 0x4c; ++o)
                if (mine[k * 0x50 + o] != r.table[k * 0x50 + o]) ints = false;
            for (size_t o = 0x00; o < 0x40; ++o)
                if (mine[k * 0x50 + o] != r.table[k * 0x50 + o]) floats = false;
        }
        if (!ints) continue;
        if (best < 1) { best = 1; if (usedT) *usedT = T; }
        if (floats) { best = 2; if (usedT) *usedT = T; break; }
    }
    return best;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: subsection_port_check <probe.tsv>\n");
        return 2;
    }
    const auto rows = load(argv[1]);
    std::printf("retail subsection tables: %zu\n", rows.size());

    // Sweep the unknown mesh radius. The integer lanes should be flat across it.
    for (int step = 0; step < 13; ++step) {
        const float R = float(std::pow(10.0, -1.0 + 0.25 * step));
        long ints = 0, whole = 0;
        for (const auto& r : rows) {
            const int s = score(r, R, nullptr);
            if (s >= 1) ++ints;
            if (s >= 2) ++whole;
        }
        std::printf("R=%10.4f  integer lanes %5ld/%zu   whole element %5ld/%zu\n",
                    double(R), ints, rows.size(), whole, rows.size());
    }

    std::map<int, long> tHist;
    long ints = 0;
    for (const auto& r : rows) {
        int T = 0;
        if (score(r, 1.0f, &T) >= 1) { ++ints; ++tHist[T]; }
    }
    std::printf("\nat R=1: integer lanes %ld/%zu; leaf-threshold histogram:\n",
                ints, rows.size());
    for (const auto& kv : tHist)
        std::printf("  T=%d : %ld\n", kv.first, kv.second);
    return ints == long(rows.size()) ? 0 : 1;
}
