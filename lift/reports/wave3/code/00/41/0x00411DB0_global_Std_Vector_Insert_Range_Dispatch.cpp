struct StdVectorInsertRangeDispatch_Overlay {
    std::byte _pad0[0x04];
    std::pair<float, CSubtitledMessage::CCameraTag>* _Mylast; // +0x04
    std::pair<float, CSubtitledMessage::CCameraTag>* _Myend;  // +0x08
};

static_assert(offsetof(StdVectorInsertRangeDispatch_Overlay, _Mylast) == 0x04);
static_assert(offsetof(StdVectorInsertRangeDispatch_Overlay, _Myend) == 0x08);

struct StdVectorInsertRangeDispatch_ValueTemp {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
    std::uint32_t word3;
    std::uint32_t word4;
    std::uint32_t word5;
    std::uint32_t word6;
};

static_assert(sizeof(StdVectorInsertRangeDispatch_ValueTemp) == 0x1C);

void __thiscall Std_Vector_Insert_Range_Dispatch(
    StdVectorInsertRangeDispatch_Overlay* self,
    std::pair<float, CSubtitledMessage::CCameraTag>* insert_pos,
    unsigned int count,
    std::pair<float, CSubtitledMessage::CCameraTag>* value_src)
{
    using value_type = std::pair<float, CSubtitledMessage::CCameraTag>;

    if (count != 0) {
        if (static_cast<unsigned int>(self->_Myend - self->_Mylast) < count) {
            Std_Vector_Insert_Range(insert_pos, value_src, &value_src, count, 0);
            return;
        }

        StdVectorInsertRangeDispatch_ValueTemp local_value;
        local_value.word0 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x00);
        local_value.word1 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x04);
        local_value.word2 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x08);
        local_value.word3 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x0C);
        local_value.word4 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x10);
        local_value.word5 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x14);
        local_value.word6 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(value_src) + 0x18);

        value_type* const old_last = self->_Mylast;
        const unsigned int tail_count = static_cast<unsigned int>(self->_Mylast - insert_pos);

        if (tail_count <= count) {
            value_src = reinterpret_cast<value_type*>(count - tail_count);

            std::_Fill(
                old_last,
                value_src,
                reinterpret_cast<value_type*>(&local_value));

            int new_last = reinterpret_cast<int>(self->_Mylast) + reinterpret_cast<int>(value_src) * 0x1C;
            self->_Mylast = reinterpret_cast<value_type*>(new_last);

            std::_Copy_impl(insert_pos, old_last, reinterpret_cast<value_type*>(new_last), &value_src);

            self->_Mylast = reinterpret_cast<value_type*>(
                reinterpret_cast<unsigned int>(self->_Mylast) + tail_count * 0x1C);

            std::_Fill(
                reinterpret_cast<CHeroMarriageInfo*>(insert_pos),
                reinterpret_cast<CHeroMarriageInfo*>(old_last),
                reinterpret_cast<CHeroMarriageInfo*>(&local_value));
            return;
        }

        value_src = old_last - count;

        std::_Copy_impl(value_src, old_last, old_last, &value_src);

        self->_Mylast = reinterpret_cast<value_type*>(
            reinterpret_cast<unsigned int>(self->_Mylast) + count * 0x1C);

        std::_Copy_backward(insert_pos, value_src, old_last, &value_src, 0);

        std::_Fill(
            reinterpret_cast<CHeroMarriageInfo*>(insert_pos),
            reinterpret_cast<CHeroMarriageInfo*>(
                reinterpret_cast<std::byte*>(insert_pos) + count * 0x1C),
            reinterpret_cast<CHeroMarriageInfo*>(&local_value));
    }
}