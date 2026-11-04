#pragma once
// Fixture dependencies only: omit the candidate so the test must link its object.
#include "third_party/zlib121/retail_headers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static int ztest_allocs, ztest_frees, ztest_fail;
extern "C" voidpf zcalloc(voidpf, unsigned items, unsigned size) {
    if (ztest_fail) return 0;
    ++ztest_allocs; return calloc(items, size);
}
extern "C" void zcfree(voidpf, voidpf p) { ++ztest_frees; free(p); }
#ifndef ZTEST_OMIT_INIT
#include "third_party/zlib121/inflateInit__impl.h"
#endif
#ifndef ZTEST_OMIT_INIT2
#include "third_party/zlib121/inflateInit2__impl.h"
#endif
#ifndef ZTEST_OMIT_RESET
#include "third_party/zlib121/inflateReset_impl.h"
#endif
#ifndef ZTEST_OMIT_END
#include "third_party/zlib121/inflateEnd_impl.h"
#endif
#ifndef ZTEST_OMIT_INFLATE
#include "third_party/zlib121/inflate_impl.h"
#endif
#ifndef ZTEST_OMIT_FAST
#include "third_party/zlib121/inffast_impl.h"
#endif
#ifndef ZTEST_OMIT_TABLE
#include "third_party/zlib121/inftrees_impl.h"
#endif
#ifndef ZTEST_OMIT_ADLER
#include "third_party/zlib121/adler32_impl.h"
#endif
#ifndef ZTEST_OMIT_CRC
#include "third_party/zlib121/crc32_impl.h"
#endif
