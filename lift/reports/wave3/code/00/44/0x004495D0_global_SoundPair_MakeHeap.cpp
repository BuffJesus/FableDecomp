void __fastcall SoundPair_MakeHeap(
    std::pair<unsigned long, CSoundPair>* first,
    std::pair<unsigned long, CSoundPair>* last,
    const CKeyPairCompareLess<unsigned long, CSoundPair>& compare)
{
    if (first != last) {
        int heapDepth = 0;
        int count = static_cast<int>(last - first);

        while (count != 1) {
            count >>= 1;
            ++heapDepth;
        }

        Heap_SiftDown(first, last, 0, heapDepth * 2, compare);
        std::make_heap(first, last, compare);
    }
}