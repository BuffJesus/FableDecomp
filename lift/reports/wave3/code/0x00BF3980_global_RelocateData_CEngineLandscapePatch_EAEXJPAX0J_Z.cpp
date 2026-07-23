#include <cstddef>
#include <cstdint>

class CLandscapeLayerMesh;
class CBankFileAsyncData;
enum EThreadedFilePriority : int;

struct CEngineLandscapePatch_RelocateData_Overlay
{
    std::byte pad_0000[0x08];
    CLandscapeLayerMesh* field_0008;
    CLandscapeLayerMesh* field_000C;
    std::byte pad_0010[0x04];
    CBankFileAsyncData* field_0014;
};

static_assert(offsetof(CEngineLandscapePatch_RelocateData_Overlay, field_0008) == 0x08);
static_assert(offsetof(CEngineLandscapePatch_RelocateData_Overlay, field_000C) == 0x0C);
static_assert(offsetof(CEngineLandscapePatch_RelocateData_Overlay, field_0014) == 0x14);

struct CLandscapeLayerMesh_RelocateData_Overlay
{
    std::byte pad_0000[0x3C];
    CLandscapeLayerMesh* field_003C;
};

static_assert(offsetof(CLandscapeLayerMesh_RelocateData_Overlay, field_003C) == 0x3C);

class CBankFileAsyncData
{
public:
    static void ChangeLoadingPriority(CBankFileAsyncData* pAsyncData, EThreadedFilePriority priority);
};

class CLandscapeLayerMesh
{
public:
    static void OnPostRelocation(CLandscapeLayerMesh* pMesh);
};

void CEngineLandscapePatch::RelocateData(long /*unused*/, void* /*unusedBase*/, void* relocatedPtr, long newBase)
{
    auto* const self = reinterpret_cast<CEngineLandscapePatch_RelocateData_Overlay*>(this);
    EThreadedFilePriority unaff_ESI;

    CLandscapeLayerMesh* current = self->field_000C;
    if (relocatedPtr == current) {
        if (current != nullptr) {
            self->field_000C = reinterpret_cast<CLandscapeLayerMesh*>(
                reinterpret_cast<std::intptr_t>(current) +
                (newBase - reinterpret_cast<std::intptr_t>(relocatedPtr)));
            return;
        }
    }
    else {
        auto* asyncData = self->field_0014;
        if (relocatedPtr == asyncData) {
            if (asyncData != nullptr) {
                self->field_0014 = reinterpret_cast<CBankFileAsyncData*>(
                    reinterpret_cast<std::intptr_t>(asyncData) +
                    (newBase - reinterpret_cast<std::intptr_t>(relocatedPtr)));
            }

            CBankFileAsyncData::ChangeLoadingPriority(self->field_0014, unaff_ESI);
            return;
        }

        CLandscapeLayerMesh* previous = nullptr;
        current = self->field_0008;
        if (current != nullptr) {
            while (relocatedPtr != current) {
                auto* const currentOverlay =
                    reinterpret_cast<CLandscapeLayerMesh_RelocateData_Overlay*>(current);
                previous = current;
                current = currentOverlay->field_003C;
                if (current == nullptr) {
                    return;
                }
            }

            if (current != nullptr) {
                current = reinterpret_cast<CLandscapeLayerMesh*>(
                    reinterpret_cast<std::intptr_t>(current) +
                    (newBase - reinterpret_cast<std::intptr_t>(relocatedPtr)));
            }

            if (previous != nullptr) {
                auto* const previousOverlay =
                    reinterpret_cast<CLandscapeLayerMesh_RelocateData_Overlay*>(previous);
                previousOverlay->field_003C = current;
                CLandscapeLayerMesh::OnPostRelocation(current);
                return;
            }

            self->field_0008 = current;
            CLandscapeLayerMesh::OnPostRelocation(current);
        }
    }
}