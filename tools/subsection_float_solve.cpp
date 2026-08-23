// Is the residual in the FLOAT lanes an UNRECOVERED input (the source mesh's
// bounding sphere) or a bug in the port?
//
// For any retail subsection element quadrant with count == 1 the algebra is
// closed: the engine's section sphere for a single instance is exactly that
// instance's own sphere, i.e.
//     centre = objectMatrix.TransformPoint(mesh.sphere.centre)
//     radius = mesh.sphere.radius * instance.scale
// The port already reproduces the integer lanes bit-for-bit, so its remap says
// which source instance sits in that slot. Reading retail's floats back out
// therefore SOLVES for the two unknowns directly:
//     impliedMeshRadius = retailRadius / scale
//     offset            = retailCentre - instancePosition
// If those are constant per collection they are a recoverable authoring input,
// not a defect. If they scatter, the sphere maths in the port is wrong.
#include "forge/stbbake.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static float f32(const uint8_t* p) { float v; std::memcpy(&v, p, 4); return v; }

int main(int argc, char** argv) {
    if (argc < 3) { std::fprintf(stderr, "usage: floatsolve <in.tsv> <out.tsv>\n"); return 2; }
    std::ifstream in(argv[1]);
    FILE* out = std::fopen(argv[2], "w");
    std::fprintf(out, "map\trecordId\tcollectionType\tobjectCount\tscale\timpliedMeshRadius\tdx\tdy\tdz\toursrc\tnearsrc\tnearDist\tourDist\n");
    std::string line; long solved = 0, rowsSeen = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        std::string mp, id, ct, nn, bs, hex;
        std::getline(ls, mp, '\t'); std::getline(ls, id, '\t');
        std::getline(ls, ct, '\t'); std::getline(ls, nn, '\t');
        std::getline(ls, bs, '\t'); std::getline(ls, hex, '\t');
        const int n = std::atoi(nn.c_str());
        std::vector<float> b; { std::istringstream s(bs); double v; while (s >> v) b.push_back(float(v)); }
        std::vector<uint8_t> tbl;
        for (size_t i = 0; i + 1 < hex.size(); i += 2)
            tbl.push_back(uint8_t(std::stoul(hex.substr(i, 2), nullptr, 16)));
        const size_t elems = tbl.size() / 0x50;
        ++rowsSeen;
        // find the T that reproduces the integer lanes (R is irrelevant to them)
        for (int T = 1; T <= 4; ++T) {
            std::vector<forge::stbbake::SubsectionSphere> sph;
            for (int i = 0; i < n; ++i) sph.push_back({b[i*4], b[i*4+1], b[i*4+2], b[i*4+3]});
            forge::stbbake::SubsectionTable t;
            try { t = forge::stbbake::buildSubSectionsAndObjectRemapTable(sph, T); }
            catch (const std::exception&) { continue; }
            if (!t.present || t.elements.size() != elems) continue;
            const auto m = forge::stbbake::serializeSubsectionElements(t.elements);
            bool ok = true;
            for (size_t k = 0; k < elems && ok; ++k)
                for (size_t o = 0x40; o < 0x4c; ++o)
                    if (m[k*0x50+o] != tbl[k*0x50+o]) { ok = false; break; }
            if (!ok) continue;
            for (size_t k = 0; k < elems; ++k) {
                const uint8_t* e = &tbl[k * 0x50];
                for (int q = 0; q < 4; ++q) {
                    if (e[0x40 + q] != 1) continue;
                    const int src = t.remap[e[0x44 + q]];
                    const float px = b[src*4], py = b[src*4+1], pz = b[src*4+2], sc = b[src*4+3];
                    if (sc == 0.0f) continue;
                    const float cx = f32(e + 0x00 + q*4), cy = f32(e + 0x10 + q*4), cz = f32(e + 0x20 + q*4);
                    int nearIdx = -1; double nd = 1e30, od = 0;
                    for (int i = 0; i < n; ++i) {
                        const double d = std::sqrt(double(b[i*4]-cx)*double(b[i*4]-cx)
                                                 + double(b[i*4+1]-cy)*double(b[i*4+1]-cy)
                                                 + double(b[i*4+2]-cz)*double(b[i*4+2]-cz));
                        if (d < nd) { nd = d; nearIdx = i; }
                        if (i == src) od = d;
                    }
                    std::fprintf(out, "%s\t%s\t%s\t%d\t%.9g\t%.9g\t%.9g\t%.9g\t%.9g\t%d\t%d\t%.9g\t%.9g\n",
                        mp.c_str(), id.c_str(), ct.c_str(), n, double(sc),
                        double(f32(e + 0x30 + q*4) / sc),
                        double(f32(e + 0x00 + q*4) - px),
                        double(f32(e + 0x10 + q*4) - py),
                        double(f32(e + 0x20 + q*4) - pz), src, nearIdx, nd, od);
                    ++solved;
                }
            }
            break;
        }
    }
    std::fclose(out);
    std::printf("%ld single-instance quadrants solved from %ld records\n", solved, rowsSeen);
    return 0;
}
