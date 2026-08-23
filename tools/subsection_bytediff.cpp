// Classified byte diff: FableForge's ported local-detail subsection builder
// (forge::stbbake::buildSubSectionsAndObjectRemapTable, the port of FableWin
// 0x02EDF740 / 0x02EDFB20) replayed over every retail subsection table in the
// oracle, emitting ONE ROW PER RETAIL RECORD so the aggregation can be done
// per map and per failure class instead of by eye.
//
// Per record we search the two UNRECOVERED authoring inputs (leaf threshold
// T in 1..4, assumed mesh sphere radius R over a sweep) and keep the best
// outcome.  Byte ranges scored, per 0x50-byte element:
//    0x00..0x3F  float lanes  centreX[4] centreY[4] centreZ[4] radius[4]
//    0x40..0x43  count[4]
//    0x44..0x47  startIndex[4]
//    0x48..0x4B  childOffset[4]
//    0x4C..0x4F  documented-uninitialised tail, EXCLUDED from the verdict and
//                reported separately.
//
// Build:
//   c++ -std=c++17 -O2 -I D:/Code/FableForge/libs/forgecore/include \
//       tools/subsection_bytediff.cpp D:/Code/FableForge/build/libforgecore.a \
//       -static -o tmp/subsection_diff/bytediff
#include "forge/stbbake.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
struct Row {
    std::string map; long id = 0; int ctype = 0; int n = 0;
    std::vector<float> b;          // B (x,y,z,scale), or final spheres in sphere mode
    bool spheresAreFinal = false;
    std::vector<uint8_t> table;
};

// spheresAreFinal selects the sphere-mode input from
// tools/subsection_spheres.py: column 5 already holds the engine's own
// per-instance bounding spheres, built from the referenced mesh's AUTHORED
// sphere (mesh bank Info origin[0..3]) rather than a swept radius, so
// nothing is fitted.
std::vector<Row> load(const char* p, bool spheresAreFinal) {
    std::ifstream in(p);
    if (!in) { std::fprintf(stderr, "cannot open %s\n", p); std::exit(2); }
    std::vector<Row> rows; std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (first) { first = false; if (line.rfind("map\t", 0) == 0) continue; }
        if (line.empty()) continue;
        std::istringstream ls(line);
        std::string mp, id, ct, nn, bs, hex;
        std::getline(ls, mp, '\t'); std::getline(ls, id, '\t');
        std::getline(ls, ct, '\t'); std::getline(ls, nn, '\t');
        std::getline(ls, bs, '\t'); std::getline(ls, hex, '\t');
        Row r; r.spheresAreFinal = spheresAreFinal; r.map = mp; r.id = std::atol(id.c_str());
        r.ctype = std::atoi(ct.c_str()); r.n = std::atoi(nn.c_str());
        std::istringstream b(bs); double v;
        while (b >> v) r.b.push_back(float(v));
        if (int(r.b.size()) != 4 * r.n) { std::fprintf(stderr, "bad row\n"); std::exit(2); }
        for (size_t i = 0; i + 1 < hex.size(); i += 2)
            r.table.push_back(uint8_t(std::stoul(hex.substr(i, 2), nullptr, 16)));
        if (r.table.empty() || r.table.size() % 0x50) { std::fprintf(stderr, "bad table\n"); std::exit(2); }
        rows.push_back(std::move(r));
    }
    return rows;
}

struct Res {
    int cls = 0;        // 0 count-bad, 1 count-ok/ints-bad, 2 ints-ok/floats-bad, 3 full
    int bestT = -1; float bestR = 0.0f;
    int myElemsAtBestCount = -1;   // element count produced when class==0 (T=4,R=1)
    bool countLane = false, startLane = false, childLane = false;
    int tMaskR1 = 0;    // bit (T-1) set if int lanes match at that T with R=1
    int tMaskAny = 0;   // ... at any R in the sweep
    long floatBytesBad = 0, floatBytesTot = 0;
    long tailNonZeroRetail = 0, tailBytesTot = 0;
};

Res judge(const Row& r, const std::vector<float>& Rs) {
    const size_t elems = r.table.size() / 0x50;
    Res best;
    best.tailBytesTot = long(elems) * 4;
    for (size_t k = 0; k < elems; ++k)
        for (size_t o = 0x4c; o < 0x50; ++o)
            if (r.table[k * 0x50 + o]) ++best.tailNonZeroRetail;
    const std::vector<float> single{1.0f};
    for (float R : (r.spheresAreFinal ? single : Rs)) {
        std::vector<forge::stbbake::SubsectionSphere> sph;
        sph.reserve(size_t(r.n));
        for (int i = 0; i < r.n; ++i)
            sph.push_back(r.spheresAreFinal
                ? forge::stbbake::SubsectionSphere{r.b[i*4], r.b[i*4+1], r.b[i*4+2], r.b[i*4+3]}
                : forge::stbbake::SubsectionSphere{r.b[i*4], r.b[i*4+1], r.b[i*4+2], R * r.b[i*4+3]});
        for (int T = 1; T <= 4; ++T) {
            forge::stbbake::SubsectionTable t;
            try { t = forge::stbbake::buildSubSectionsAndObjectRemapTable(sph, T); }
            catch (const std::exception&) { continue; }
            if (best.myElemsAtBestCount < 0)
                best.myElemsAtBestCount = t.present ? int(t.elements.size()) : 0;
            if (!t.present || t.elements.size() != elems) continue;
            const auto m = forge::stbbake::serializeSubsectionElements(t.elements);
            bool cL = true, sL = true, hL = true; long fbad = 0;
            for (size_t k = 0; k < elems; ++k) {
                for (size_t o = 0x40; o < 0x44; ++o) if (m[k*0x50+o] != r.table[k*0x50+o]) cL = false;
                for (size_t o = 0x44; o < 0x48; ++o) if (m[k*0x50+o] != r.table[k*0x50+o]) sL = false;
                for (size_t o = 0x48; o < 0x4c; ++o) if (m[k*0x50+o] != r.table[k*0x50+o]) hL = false;
                for (size_t o = 0x00; o < 0x40; ++o) if (m[k*0x50+o] != r.table[k*0x50+o]) ++fbad;
            }
            int cls = 1;
            if (cL && sL && hL) {
                cls = (fbad == 0) ? 3 : 2;
                best.tMaskAny |= 1 << (T - 1);
                if (R == 1.0f) best.tMaskR1 |= 1 << (T - 1);
            }
            if (cls > best.cls) {
                best.cls = cls; best.bestT = T; best.bestR = R;
                best.countLane = cL; best.startLane = sL; best.childLane = hL;
                best.floatBytesBad = fbad; best.floatBytesTot = long(elems) * 0x40;
            }
        }
    }
    if (best.cls == 0) { best.floatBytesTot = long(elems) * 0x40; best.floatBytesBad = best.floatBytesTot; }
    return best;
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: bytediff <in.tsv> <out.tsv> [--spheres]\n");
        return 2;
    }
    bool spheresAreFinal = false;
    for (int i = 3; i < argc; ++i)
        if (std::string(argv[i]) == "--spheres") spheresAreFinal = true;
    const auto rows = load(argv[1], spheresAreFinal);
    std::vector<float> Rs;
    for (int s = 0; s < 13; ++s) Rs.push_back(float(std::pow(10.0, -1.0 + 0.25 * s)));
    // put R=1 first so the T histogram is reported at the canonical radius
    Rs.insert(Rs.begin(), 1.0f);
    FILE* out = std::fopen(argv[2], "w");
    std::fprintf(out, "map\trecordId\tcollectionType\tobjectCount\tretailElems\tportElems\t"
                      "class\tbestT\tbestR\tcountLane\tstartLane\tchildLane\t"
                      "floatBytesBad\tfloatBytesTot\ttailNonZero\ttailTot\ttMaskR1\ttMaskAny\n");
    for (const auto& r : rows) {
        const Res v = judge(r, Rs);
        std::fprintf(out, "%s\t%ld\t%d\t%d\t%zu\t%d\t%d\t%d\t%g\t%d\t%d\t%d\t%ld\t%ld\t%ld\t%ld\t%d\t%d\n",
            r.map.c_str(), r.id, r.ctype, r.n, r.table.size() / 0x50,
            v.myElemsAtBestCount, v.cls, v.bestT, double(v.bestR),
            int(v.countLane), int(v.startLane), int(v.childLane),
            v.floatBytesBad, v.floatBytesTot, v.tailNonZeroRetail, v.tailBytesTot,
            v.tMaskR1, v.tMaskAny);
    }
    std::fclose(out);
    std::printf("wrote %zu rows to %s\n", rows.size(), argv[2]);
    return 0;
}
