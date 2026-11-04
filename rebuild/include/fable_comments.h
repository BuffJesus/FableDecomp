#pragma once

// Retail GFRemoveComments @ 0x00A60410. The opening delimiter is passed in
// EDX, the mutable text in ECX, and the closing delimiter on the stack.
void __fastcall GFRemoveComments(char* source, char* opening, char* closing);
