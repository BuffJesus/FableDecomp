#include <cstddef>
#include <cstdint>

struct CActiveFile;

struct CActiveFile_Vector12Value
{
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};

struct CActiveFile_Vector12Overlay
{
    CActiveFile_Vector12Value* begin;        // 0x00
    CActiveFile_Vector12Value* end;          // 0x04
    CActiveFile_Vector12Value* capacityEnd;  // 0x08
};

static_assert(sizeof(CActiveFile_Vector12Value) == 0x0C);
static_assert(offsetof(CActiveFile_Vector12Overlay, begin) == 0x00);
static_assert(offsetof(CActiveFile_Vector12Overlay, end) == 0x04);
static_assert(offsetof(CActiveFile_Vector12Overlay, capacityEnd) == 0x08);
static_assert(sizeof(CActiveFile_Vector12Overlay) == 0x0C);

extern "C" int CActiveFile_AssignVector12(
    std::uint32_t count,
    CActiveFile_Vector12Value* begin,
    int endAddress);

namespace CFileInstaller
{
struct CActiveFile
{
    static void __thiscall OnReadFinished(::CActiveFile* self);
};
}

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector12(CActiveFile* self, CActiveFile* other)
{
    if (other != self)
    {
        auto& dst = *reinterpret_cast<CActiveFile_Vector12Overlay*>(self);
        auto& src = *reinterpret_cast<CActiveFile_Vector12Overlay*>(other);

        int srcEndAddress = reinterpret_cast<int>(src.end);
        CActiveFile_Vector12Value* srcBegin = src.begin;
        const std::uint32_t srcCount =
            static_cast<std::uint32_t>((srcEndAddress - reinterpret_cast<int>(srcBegin)) / 0x0C);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<int>(dst.capacityEnd) - reinterpret_cast<int>(dst.begin)) / 0x0C) < srcCount)
        {
            srcEndAddress = CActiveFile_AssignVector12(srcCount, srcBegin, srcEndAddress);
            CFileInstaller::CActiveFile::OnReadFinished(self);
            dst.begin = reinterpret_cast<CActiveFile_Vector12Value*>(srcEndAddress);
            dst.capacityEnd = reinterpret_cast<CActiveFile_Vector12Value*>(srcEndAddress + srcCount * 0x0C);
        }
        else
        {
            const std::uint32_t dstCount =
                static_cast<std::uint32_t>((reinterpret_cast<int>(dst.end) - reinterpret_cast<int>(dst.begin)) / 0x0C);

            if (dstCount < srcCount)
            {
                int countToCopy = static_cast<int>((dstCount * 0x0C) / 0x0C);
                CActiveFile_Vector12Value* dstCurrent = dst.begin;
                CActiveFile_Vector12Value* srcCurrent = srcBegin;

                if (0 < countToCopy)
                {
                    do
                    {
                        countToCopy = countToCopy + -1;
                        dstCurrent->word0 = srcCurrent->word0;
                        dstCurrent->word1 = srcCurrent->word1;
                        dstCurrent->word2 = srcCurrent->word2;
                        dstCurrent = dstCurrent + 1;
                        srcCurrent = srcCurrent + 1;
                    } while (countToCopy != 0);
                }

                std::uint32_t* srcEndDwords = reinterpret_cast<std::uint32_t*>(src.end);
                std::uint32_t* dstWriteDwords = reinterpret_cast<std::uint32_t*>(dst.end);
                std::uint32_t* srcReadDwords = reinterpret_cast<std::uint32_t*>(
                    reinterpret_cast<int>(src.begin) +
                    static_cast<int>(((reinterpret_cast<int>(dst.end) - reinterpret_cast<int>(dst.begin)) / 0x0C) * 0x0C));

                if (srcReadDwords != srcEndDwords)
                {
                    std::uint32_t* dstWriteWord1 = dstWriteDwords + 1;
                    std::uint32_t* srcReadWord1 = srcReadDwords + 1;

                    do
                    {
                        if (dstWriteDwords != nullptr)
                        {
                            *dstWriteDwords = *srcReadDwords;
                            *dstWriteWord1 = *srcReadWord1;
                            dstWriteWord1[1] = srcReadWord1[1];
                        }

                        srcReadDwords = srcReadDwords + 3;
                        dstWriteDwords = dstWriteDwords + 3;
                        dstWriteWord1 = dstWriteWord1 + 3;
                        srcReadWord1 = srcReadWord1 + 3;
                    } while (srcReadDwords != srcEndDwords);
                }
            }
            else
            {
                int remaining = (srcEndAddress - reinterpret_cast<int>(srcBegin)) / 0x0C;
                CActiveFile_Vector12Value* out = dst.begin;

                if (0 < remaining)
                {
                    do
                    {
                        out->word0 = srcBegin->word0;
                        out->word1 = srcBegin->word1;
                        out->word2 = srcBegin->word2;
                        srcBegin = srcBegin + 1;
                        remaining = remaining + -1;
                        out = out + 1;
                    } while (remaining != 0);
                }
            }
        }

        dst.end = reinterpret_cast<CActiveFile_Vector12Value*>(
            reinterpret_cast<int>(dst.begin) + srcCount * 0x0C);
    }

    return self;
}