CActiveFile* __thiscall CActiveFile_AssignUndefinedVector24(CActiveFile* self, CActiveFile* other)
{
    struct Vector24Overlay
    {
        std::uint32_t* begin; // 0x00
        std::uint32_t* end;   // 0x04
        std::uint32_t* cap;   // 0x08
    };

    static_assert(offsetof(Vector24Overlay, begin) == 0x00);
    static_assert(offsetof(Vector24Overlay, end) == 0x04);
    static_assert(offsetof(Vector24Overlay, cap) == 0x08);
    static_assert(sizeof(Vector24Overlay) == 0x0C);

    if (other != self)
    {
        auto& dst = *reinterpret_cast<Vector24Overlay*>(self);
        auto& src = *reinterpret_cast<Vector24Overlay*>(other);

        const std::int32_t src_bytes = static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(src.end) - reinterpret_cast<std::uintptr_t>(src.begin));
        std::uint32_t* src_cursor = src.begin;
        const std::uint32_t count = static_cast<std::uint32_t>(src_bytes / 0x18);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(dst.cap) - reinterpret_cast<std::uintptr_t>(dst.begin)) / 0x18) < count)
        {
            const auto new_begin = reinterpret_cast<std::uint32_t*>(
                CActiveFile_AssignVector24(count, src_cursor, reinterpret_cast<int>(src.end)));
            CFileInstaller::CActiveFile::OnReadFinished(self);
            dst.begin = new_begin;
            dst.cap = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(new_begin) + count * 0x18);
        }
        else
        {
            const std::uint32_t dst_size = static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(dst.end) - reinterpret_cast<std::uintptr_t>(dst.begin)) / 0x18);

            if (dst_size < count)
            {
                std::int32_t remaining = static_cast<std::int32_t>((dst_size * 0x18) / 0x18);
                std::uint32_t* dst_cursor = dst.begin;

                if (0 < remaining)
                {
                    do
                    {
                        --remaining;

                        std::uint32_t* copy_src = src_cursor;
                        std::uint32_t* copy_dst = dst_cursor;
                        for (int i = 6; i != 0; --i)
                        {
                            *copy_dst = *copy_src;
                            ++copy_src;
                            ++copy_dst;
                        }

                        dst_cursor += 6;
                        src_cursor += 6;
                    } while (remaining != 0);
                }

                std::uint32_t* src_end = src.end;
                std::uint32_t* append_dst = dst.end;
                for (std::uint32_t* append_src = reinterpret_cast<std::uint32_t*>(
                         reinterpret_cast<std::uintptr_t>(src.begin) +
                         ((reinterpret_cast<std::uintptr_t>(append_dst) - reinterpret_cast<std::uintptr_t>(dst.begin)) / 0x18) * 0x18);
                     append_src != src_end;
                     append_src += 6)
                {
                    if (append_dst != nullptr)
                    {
                        std::uint32_t* copy_src = append_src;
                        std::uint32_t* copy_dst = append_dst;
                        for (int i = 6; i != 0; --i)
                        {
                            *copy_dst = *copy_src;
                            ++copy_src;
                            ++copy_dst;
                        }
                    }

                    append_dst += 6;
                }
            }
            else
            {
                std::int32_t remaining = src_bytes / 0x18;
                auto copy_src_bytes = reinterpret_cast<std::uint8_t*>(src_cursor);
                std::uint32_t* copy_dst = dst.begin;

                if (0 < remaining)
                {
                    do
                    {
                        --remaining;

                        std::uint32_t* copy_src = reinterpret_cast<std::uint32_t*>(copy_src_bytes);
                        std::uint32_t* copy_out = copy_dst;
                        for (int i = 6; i != 0; --i)
                        {
                            *copy_out = *copy_src;
                            ++copy_src;
                            ++copy_out;
                        }

                        copy_src_bytes += 0x18;
                        copy_dst += 6;
                    } while (remaining != 0);
                }
            }
        }

        dst.end = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(dst.begin) + count * 0x18);
    }

    return self;
}