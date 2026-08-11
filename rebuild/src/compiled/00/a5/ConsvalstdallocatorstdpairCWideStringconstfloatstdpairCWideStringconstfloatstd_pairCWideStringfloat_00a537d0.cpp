// std::_Cons_val<allocator<pair<CWideString const,float>>, pair<CWideString const,float>, pair<CWideString,float>>
// Retail 0x00a537d0. Constructs a pair<CWideString const,float> in-place at `dest`
// from source pair `src`: copy-construct the CWideString key, then copy the float.
//
// Layout of pair<CWideString const, float>:
//   +0: CWideString  (a single pointer member -> 4 bytes)
//   +4: float
//
// dest arrives in ecx, src in edx (2-arg __fastcall). The CWideString copy-ctor
// (0x0099ec30) is a real __fastcall: this=dest (ecx untouched), one stack arg = src.

struct CWideString {
	// Copy-ctor at 0x0099ec30. Real member => true __fastcall: this in ecx,
	// `src` pushed as the single stack arg (edx left untouched).
	void CopyCtor(CWideString* src);
	unsigned short* m_buf;
};

struct PairKF {
	CWideString key;
	float val;
};

// _Cons_val: ecx = dest pair, edx = source pair.
void __fastcall Cons_val(PairKF* dest, PairKF* src)
{
	if (dest)
	{
		dest->key.CopyCtor(&src->key);
		dest->val = src->val;
	}
}