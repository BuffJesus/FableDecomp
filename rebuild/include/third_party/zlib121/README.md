# Retail zlib 1.2.1

Upstream: https://zlib.net/fossils/zlib-1.2.1.tar.gz (17 November 2003).
License: the original notice is retained in zlib.h and each upstream header.

Headers are unmodified upstream files. *_impl.h files are explicitly marked C++
adaptations: K&R signatures become ANSI signatures, the local identifier `this`
becomes `here`, and void-pointer assignments to internal_state get C++ casts.
The inflate translation unit is separated into entry points for per-address
parity and behavior verification. Fixed tables and window logic stay with inflate.
No modern zlib substitutions or retail binary data are included.
