HistoryBookcase.Main
00EA7B40 sub esp, 0x1c
00EA7B43 push ebx
00EA7B44 push esi
00EA7B45 mov esi, ecx
00EA7B47 mov eax, dword ptr [esi + 0x14]
00EA7B4A mov cl, byte ptr [eax + 0x49]
00EA7B4D test cl, cl
00EA7B4F push edi
00EA7B50 jne 0xea7ed0
00EA7B56 mov ecx, esi
00EA7B58 call 0xf35b30
00EA7B5D test al, al
00EA7B5F jne 0xea7ee3
00EA7B65 mov ecx, dword ptr [esi + 4]
00EA7B68 mov edx, dword ptr [ecx]
00EA7B6A push 1
00EA7B6C lea edi, [esi + 8]
00EA7B6F push edi
00EA7B70 call dword ptr [edx + 0x6fc]
00EA7B76 push -1
00EA7B78 push 0x1258a64
00EA7B7D lea ecx, [esp + 0x14]
00EA7B81 call 0x99ebf0
00EA7B86 mov ecx, dword ptr [esi + 4]
00EA7B89 mov eax, dword ptr [ecx]
00EA7B8B lea edx, [esp + 0xc]
00EA7B8F push edx
00EA7B90 push edi
00EA7B91 call dword ptr [eax + 0x578]
00EA7B97 lea ecx, [esp + 0xc]
00EA7B9B call 0x99eae0
00EA7BA0 push -1
00EA7BA2 push 0x125d1c8
00EA7BA7 lea ecx, [esp + 0x14]
00EA7BAB call 0x99ebf0
00EA7BB0 mov eax, dword ptr [edi]
00EA7BB2 lea ecx, [esp + 0xc]
00EA7BB6 push ecx
00EA7BB7 mov ecx, edi
00EA7BB9 call dword ptr [eax + 0x50]
00EA7BBC mov bl, al
00EA7BBE neg bl
00EA7BC0 sbb bl, bl
00EA7BC2 lea ecx, [esp + 0xc]
00EA7BC6 inc bl
00EA7BC8 call 0x99eae0
00EA7BCD test bl, bl
00EA7BCF je 0xea7c19
00EA7BD1 mov ecx, dword ptr [esi + 4]
00EA7BD4 mov edx, dword ptr [ecx]
00EA7BD6 call dword ptr [edx + 0x1c]
00EA7BD9 mov ecx, esi
00EA7BDB call 0xf35b30
00EA7BE0 test al, al
00EA7BE2 jne 0xea7ee3
00EA7BE8 push -1
00EA7BEA push 0x125d1c8
00EA7BEF lea ecx, [esp + 0x14]
00EA7BF3 call 0x99ebf0
00EA7BF8 mov eax, dword ptr [edi]
00EA7BFA lea ecx, [esp + 0xc]
00EA7BFE push ecx
00EA7BFF mov ecx, edi
00EA7C01 call dword ptr [eax + 0x50]
00EA7C04 mov bl, al
00EA7C06 neg bl
00EA7C08 sbb bl, bl
00EA7C0A lea ecx, [esp + 0xc]
00EA7C0E inc bl
00EA7C10 call 0x99eae0
00EA7C15 test bl, bl
00EA7C17 jne 0xea7bd1
00EA7C19 mov ecx, esi
00EA7C1B call 0xf35b30
00EA7C20 test al, al
00EA7C22 jne 0xea7ee3
00EA7C28 push -1
00EA7C2A push 0x122d70e
00EA7C2F lea ecx, [esp + 0x1c]
00EA7C33 call 0x99ebf0
00EA7C38 push -1
00EA7C3A push 0x1238fcc
00EA7C3F lea ecx, [esp + 0x18]
00EA7C43 call 0x99ebf0
00EA7C48 push -1
00EA7C4A push 0x12ee578
00EA7C4F lea ecx, [esp + 0x14]
00EA7C53 call 0x99ebf0
00EA7C58 mov ecx, dword ptr [esi + 4]
00EA7C5B mov edx, ecx
00EA7C5D mov ebx, dword ptr [edx]
00EA7C5F lea eax, [esp + 0x14]
00EA7C63 push eax
00EA7C64 lea edx, [esp + 0x14]
00EA7C68 push edx
00EA7C69 mov edx, dword ptr [ecx]
00EA7C6B lea eax, [esp + 0x14]
00EA7C6F push eax
00EA7C70 lea eax, [esp + 0x24]
00EA7C74 push eax
00EA7C75 call dword ptr [edx + 0xa3c]
00EA7C7B mov ecx, dword ptr [esi + 4]
00EA7C7E push eax
00EA7C7F call dword ptr [ebx + 0x4a0]
00EA7C85 lea ecx, [esp + 0x18]
00EA7C89 call 0x99eae0
00EA7C8E lea ecx, [esp + 0xc]
00EA7C92 call 0x99eae0
00EA7C97 lea ecx, [esp + 0x10]
00EA7C9B call 0x99eae0
00EA7CA0 lea ecx, [esp + 0x14]
00EA7CA4 call 0x99eae0
00EA7CA9 mov ecx, dword ptr [esi + 4]
00EA7CAC mov edx, dword ptr [ecx]
00EA7CAE push edi
00EA7CAF call dword ptr [edx + 0x580]
00EA7CB5 push -1
00EA7CB7 push 0x1258a64
00EA7CBC lea ecx, [esp + 0x18]
00EA7CC0 call 0x99ebf0
00EA7CC5 push -1
00EA7CC7 push 0x12ee4b4
00EA7CCC lea ecx, [esp + 0x1c]
00EA7CD0 call 0x99ebf0
00EA7CD5 mov ecx, dword ptr [esi + 4]
00EA7CD8 lea edx, [esp + 0x10]
00EA7CDC push edx
00EA7CDD lea edx, [esp + 0x18]
00EA7CE1 mov eax, ecx
00EA7CE3 mov edi, dword ptr [eax]
00EA7CE5 mov eax, dword ptr [ecx]
00EA7CE7 push edx
00EA7CE8 lea edx, [esp + 0x24]
00EA7CEC push edx
00EA7CED call dword ptr [eax + 0x120]
00EA7CF3 mov ecx, dword ptr [esi + 4]
00EA7CF6 push eax
00EA7CF7 call dword ptr [edi + 0x578]
00EA7CFD lea ecx, [esp + 0x1c]
00EA7D01 call 0x4aa840
00EA7D06 lea ecx, [esp + 0x14]
00EA7D0A call 0x99eae0
00EA7D0F lea ecx, [esp + 0x10]
00EA7D13 call 0x99eae0
00EA7D18 mov eax, dword ptr [esi + 0x14]
00EA7D1B push -1
00EA7D1D push 0x12ee558
00EA7D22 lea ecx, [esp + 0x1c]
00EA7D26 mov byte ptr [eax + 0x49], 1
00EA7D2A call 0x99ebf0
00EA7D2F mov ecx, dword ptr [esi + 4]
00EA7D32 mov edx, dword ptr [ecx]
00EA7D34 lea eax, [esp + 0x14]
00EA7D38 push eax
00EA7D39 call dword ptr [edx + 0x1cc]
00EA7D3F lea ecx, [esp + 0x14]
00EA7D43 call 0x99eae0
00EA7D48 mov ecx, dword ptr [esi + 4]
00EA7D4B mov edx, dword ptr [ecx]
00EA7D4D call dword ptr [edx + 0xa0]
00EA7D53 test al, al
00EA7D55 jne 0xea7d7d
00EA7D57 mov ecx, dword ptr [esi + 4]
00EA7D5A mov eax, dword ptr [ecx]
00EA7D5C call dword ptr [eax + 0x1c]
00EA7D5F mov ecx, esi
00EA7D61 call 0xf35b30
00EA7D66 test al, al
00EA7D68 jne 0xea7ee3
00EA7D6E mov ecx, dword ptr [esi + 4]
00EA7D71 mov edx, dword ptr [ecx]
00EA7D73 call dword ptr [edx + 0xa0]
00EA7D79 test al, al
00EA7D7B je 0xea7d57
00EA7D7D mov ecx, esi
00EA7D7F call 0xf35b30
00EA7D84 test al, al
00EA7D86 jne 0xea7ee3
00EA7D8C push -1
00EA7D8E push 0x12ee538
00EA7D93 lea ecx, [esp + 0x1c]
00EA7D97 call 0x99ebf0
00EA7D9C mov ecx, dword ptr [esi + 4]
00EA7D9F mov eax, dword ptr [ecx]
00EA7DA1 lea edx, [esp + 0x14]
00EA7DA5 push edx
00EA7DA6 call dword ptr [eax + 0x1cc]
00EA7DAC lea ecx, [esp + 0x14]
00EA7DB0 call 0x99eae0
00EA7DB5 mov ecx, dword ptr [esi + 4]
00EA7DB8 mov eax, dword ptr [ecx]
00EA7DBA call dword ptr [eax + 0xa0]
00EA7DC0 test al, al
00EA7DC2 jne 0xea7dea
00EA7DC4 mov ecx, dword ptr [esi + 4]
00EA7DC7 mov edx, dword ptr [ecx]
00EA7DC9 call dword ptr [edx + 0x1c]
00EA7DCC mov ecx, esi
00EA7DCE call 0xf35b30
00EA7DD3 test al, al
00EA7DD5 jne 0xea7ee3
00EA7DDB mov ecx, dword ptr [esi + 4]
00EA7DDE mov eax, dword ptr [ecx]
00EA7DE0 call dword ptr [eax + 0xa0]
00EA7DE6 test al, al
00EA7DE8 je 0xea7dc4
00EA7DEA mov ecx, esi
00EA7DEC call 0xf35b30
00EA7DF1 test al, al
00EA7DF3 jne 0xea7ee3
00EA7DF9 push -1
00EA7DFB push 0x12ee518
00EA7E00 lea ecx, [esp + 0x1c]
00EA7E04 call 0x99ebf0
00EA7E09 mov ecx, dword ptr [esi + 4]
00EA7E0C mov edx, dword ptr [ecx]
00EA7E0E lea eax, [esp + 0x14]
00EA7E12 push eax
00EA7E13 call dword ptr [edx + 0x1cc]
00EA7E19 lea ecx, [esp + 0x14]
00EA7E1D call 0x99eae0
00EA7E22 mov ecx, dword ptr [esi + 4]
00EA7E25 mov edx, dword ptr [ecx]
00EA7E27 call dword ptr [edx + 0xa0]
00EA7E2D test al, al
00EA7E2F jne 0xea7e57
00EA7E31 mov ecx, dword ptr [esi + 4]
00EA7E34 mov eax, dword ptr [ecx]
00EA7E36 call dword ptr [eax + 0x1c]
00EA7E39 mov ecx, esi
00EA7E3B call 0xf35b30
00EA7E40 test al, al
00EA7E42 jne 0xea7ee3
00EA7E48 mov ecx, dword ptr [esi + 4]
00EA7E4B mov edx, dword ptr [ecx]
00EA7E4D call dword ptr [edx + 0xa0]
00EA7E53 test al, al
00EA7E55 je 0xea7e31
00EA7E57 mov ecx, esi
00EA7E59 call 0xf35b30
00EA7E5E test al, al
00EA7E60 jne 0xea7ee3
00EA7E66 push -1
00EA7E68 push 0x12ee4f8
00EA7E6D lea ecx, [esp + 0x1c]
00EA7E71 call 0x99ebf0
00EA7E76 mov ecx, dword ptr [esi + 4]
00EA7E79 mov eax, dword ptr [ecx]
00EA7E7B lea edx, [esp + 0x14]
00EA7E7F push edx
00EA7E80 call dword ptr [eax + 0x1cc]
00EA7E86 lea ecx, [esp + 0x14]
00EA7E8A call 0x99eae0
00EA7E8F mov ecx, dword ptr [esi + 4]
00EA7E92 mov eax, dword ptr [ecx]
00EA7E94 call dword ptr [eax + 0xa0]
00EA7E9A test al, al
00EA7E9C jne 0xea7ec2
00EA7E9E mov edi, edi
00EA7EA0 mov ecx, dword ptr [esi + 4]
00EA7EA3 mov edx, dword ptr [ecx]
00EA7EA5 call dword ptr [edx + 0x1c]
00EA7EA8 mov ecx, esi
00EA7EAA call 0xf35b30
00EA7EAF test al, al
00EA7EB1 jne 0xea7ee3
00EA7EB3 mov ecx, dword ptr [esi + 4]
00EA7EB6 mov eax, dword ptr [ecx]
00EA7EB8 call dword ptr [eax + 0xa0]
00EA7EBE test al, al
00EA7EC0 je 0xea7ea0
00EA7EC2 mov ecx, esi
00EA7EC4 call 0xf35b30
00EA7EC9 test al, al
00EA7ECB jne 0xea7ee3
00EA7ECD lea ecx, [ecx]
00EA7ED0 mov ecx, dword ptr [esi + 4]
00EA7ED3 mov edx, dword ptr [ecx]
00EA7ED5 call dword ptr [edx + 0x1c]
00EA7ED8 mov ecx, esi
00EA7EDA call 0xf35b30
00EA7EDF test al, al
00EA7EE1 je 0xea7ed0
00EA7EE3 pop edi
00EA7EE4 pop esi
00EA7EE5 pop ebx
00EA7EE6 add esp, 0x1c
00EA7EE9 ret 

HistoryBookcase.Init
00EA7B10 ret 

EmptyGrave.Main
00EA8130 sub esp, 0x5c
00EA8133 push ebx
00EA8134 push esi
00EA8135 mov esi, ecx
00EA8137 mov eax, dword ptr [esi + 0x14]
00EA813A mov cl, byte ptr [eax + 0x49]
00EA813D xor ebx, ebx
00EA813F test cl, cl
00EA8141 mov dword ptr [esp + 0x18], ebx
00EA8145 je 0xea8183
00EA8147 mov ecx, esi
00EA8149 call 0xf35b30
00EA814E test al, al
00EA8150 jne 0xea85d1
00EA8156 push -1
00EA8158 push 0x1258a64
00EA815D lea ecx, [esp + 0x14]
00EA8161 call 0x99ebf0
00EA8166 mov ecx, dword ptr [esi + 4]
00EA8169 mov edx, dword ptr [ecx]
00EA816B lea eax, [esp + 0xc]
00EA816F push eax
00EA8170 lea eax, [esi + 8]
00EA8173 push eax
00EA8174 call dword ptr [edx + 0x578]
00EA817A lea ecx, [esp + 0xc]
00EA817E call 0x99eae0
00EA8183 push ebp
00EA8184 push edi
00EA8185 push -1
00EA8187 push 0x12ee628
00EA818C lea ecx, [esp + 0x1c]
00EA8190 call 0x99ebf0
00EA8195 mov ecx, dword ptr [esi + 4]
00EA8198 mov edx, dword ptr [ecx]
00EA819A lea eax, [esp + 0x14]
00EA819E push eax
00EA819F lea eax, [esp + 0x44]
00EA81A3 push eax
00EA81A4 call dword ptr [edx + 0x120]
00EA81AA mov edi, dword ptr [esi + 0x14]
00EA81AD mov ebp, dword ptr [eax + 8]
00EA81B0 mov ecx, dword ptr [eax + 4]
00EA81B3 add eax, 4
00EA81B6 mov eax, dword ptr [edi + 0x54]
00EA81B9 add edi, 0x50
00EA81BC cmp eax, ebp
00EA81BE mov dword ptr [esp + 0x28], ecx
00EA81C2 je 0xea81f4
00EA81C4 test eax, eax
00EA81C6 je 0xea81e4
00EA81C8 dec dword ptr [eax]
00EA81CA mov eax, dword ptr [edi + 4]
00EA81CD cmp dword ptr [eax], 0
00EA81D0 jne 0xea81e4
00EA81D2 mov ecx, dword ptr [eax + 8]
00EA81D5 call dword ptr [eax + 4]
00EA81D8 mov edx, dword ptr [edi + 4]
00EA81DB push edx
00EA81DC call 0xbfe9bc
00EA81E1 add esp, 4
00EA81E4 test ebp, ebp
00EA81E6 mov eax, dword ptr [esp + 0x28]
00EA81EA mov dword ptr [edi], eax
00EA81EC mov dword ptr [edi + 4], ebp
00EA81EF je 0xea81f4
00EA81F1 inc dword ptr [ebp]
00EA81F4 mov eax, dword ptr [esp + 0x48]
00EA81F8 test eax, eax
00EA81FA mov dword ptr [esp + 0x40], 0x1238c8c
00EA8202 je 0xea8222
00EA8204 dec dword ptr [eax]
00EA8206 mov eax, dword ptr [esp + 0x48]
00EA820A cmp dword ptr [eax], 0
00EA820D jne 0xea8222
00EA820F mov ecx, dword ptr [eax + 8]
00EA8212 call dword ptr [eax + 4]
00EA8215 mov ecx, dword ptr [esp + 0x48]
00EA8219 push ecx
00EA821A call 0xbfe9bc
00EA821F add esp, 4
00EA8222 xor eax, eax
00EA8224 lea ecx, [esp + 0x40]
00EA8228 mov dword ptr [esp + 0x44], eax
00EA822C mov dword ptr [esp + 0x48], eax
00EA8230 call 0x99a2e0
00EA8235 lea ecx, [esp + 0x14]
00EA8239 call 0x99eae0
00EA823E mov eax, dword ptr [esi + 0x14]
00EA8241 mov ecx, dword ptr [esi + 4]
00EA8244 mov edx, dword ptr [ecx]
00EA8246 push 1
00EA8248 push 1
00EA824A add eax, 0x4c
00EA824D push eax
00EA824E call dword ptr [edx + 0x714]
00EA8254 mov ecx, esi
00EA8256 call 0xf35b30
00EA825B test al, al
00EA825D jne 0xea85cf
00EA8263 lea edi, [esi + 8]
00EA8266 jmp 0xea8270
00EA8270 push -1
00EA8272 push 0x125d1c8
00EA8277 lea ecx, [esp + 0x1c]
00EA827B call 0x99ebf0
00EA8280 mov edx, dword ptr [edi]
00EA8282 lea eax, [esp + 0x14]
00EA8286 push eax
00EA8287 mov ecx, edi
00EA8289 call dword ptr [edx + 0x50]
00EA828C lea ecx, [esp + 0x14]
00EA8290 mov byte ptr [esp + 0x13], al
00EA8294 call 0x99eae0
00EA8299 mov al, byte ptr [esp + 0x13]
00EA829D test al, al
00EA829F je 0xea8579
00EA82A5 mov ecx, esi
00EA82A7 call 0xf35b30
00EA82AC test al, al
00EA82AE jne 0xea85cf
00EA82B4 mov eax, dword ptr [esi + 0x14]
00EA82B7 mov cl, byte ptr [eax + 0x49]
00EA82BA test cl, cl
00EA82BC je 0xea8579
00EA82C2 mov cl, byte ptr [eax + 0x48]
00EA82C5 test cl, cl
00EA82C7 jne 0xea8579
00EA82CD mov eax, dword ptr [esi + 0x18]
00EA82D0 cmp dword ptr [eax + 4], 0x6a4
00EA82D7 jle 0xea8579
00EA82DD cmp dword ptr [eax + 0x88], 2
00EA82E4 jne 0xea8579
00EA82EA mov ecx, esi
00EA82EC call 0xf35b30
00EA82F1 test al, al
00EA82F3 jne 0xea85cf
00EA82F9 lea ecx, [esp + 0x4c]
00EA82FD call 0x7e72a0
00EA8302 mov ecx, dword ptr [esi + 4]
00EA8305 mov edx, ecx
00EA8307 mov edi, dword ptr [edx]
00EA8309 mov edx, dword ptr [ecx]
00EA830B push 4
00EA830D lea eax, [esp + 0x50]
00EA8311 push eax
00EA8312 call dword ptr [edx + 0x118]
00EA8318 mov ecx, dword ptr [esi + 4]
00EA831B push eax
00EA831C call dword ptr [edi + 0x20]
00EA831F lea ecx, [esp + 0x40]
00EA8323 call 0xcdbf70
00EA8328 push -1
00EA832A push 0x1255174
00EA832F lea ecx, [esp + 0x20]
00EA8333 call 0x99ebf0
00EA8338 lea eax, [esp + 0x4c]
00EA833C push eax
00EA833D lea ecx, [esp + 0x1c]
00EA8341 push ecx
00EA8342 lea ecx, [esp + 0x48]
00EA8346 call 0xcd3d2e
00EA834B mov ecx, eax
00EA834D call 0x8abd10
00EA8352 lea ecx, [esp + 0x18]
00EA8356 call 0x99eae0
00EA835B lea ecx, [esp + 0x5c]
00EA835F call 0x6e7b60
00EA8364 push -1
00EA8366 push 0x122d70e
00EA836B lea ecx, [esp + 0x24]
00EA836F call 0x99ebf0
00EA8374 mov ecx, dword ptr [esi + 4]
00EA8377 mov edx, dword ptr [ecx]
00EA8379 lea eax, [esp + 0x5c]
00EA837D push eax
00EA837E lea eax, [esp + 0x20]
00EA8382 push eax
00EA8383 call dword ptr [edx + 0x5c8]
00EA8389 lea ecx, [esp + 0x1c]
00EA838D call 0x99eae0
00EA8392 mov ebp, dword ptr [esi + 4]
00EA8395 mov edx, dword ptr [ebp]
00EA8398 push 1
00EA839A mov ecx, ebp
00EA839C call dword ptr [edx + 0x5ec]
00EA83A2 mov ecx, dword ptr [esi + 4]
00EA83A5 mov eax, dword ptr [ecx]
00EA83A7 push 1
00EA83A9 call dword ptr [eax + 0x5cc]
00EA83AF push 0x3c
00EA83B1 call 0xbfea1a
00EA83B6 mov edi, eax
00EA83B8 add esp, 4
00EA83BB test edi, edi
00EA83BD je 0xea8415
00EA83BF or ebx, 7
00EA83C2 push -1
00EA83C4 mov dword ptr [esp + 0x24], ebx
00EA83C8 mov ebx, dword ptr [esi + 0x14]
00EA83CB push 0x12ee618
00EA83D0 lea ecx, [esp + 0x40]
00EA83D4 call 0x99ebf0
00EA83D9 push eax
00EA83DA push -1
00EA83DC push 0x12c9934
00EA83E1 lea ecx, [esp + 0x40]
00EA83E5 call 0x99ebf0
00EA83EA mov edx, eax
00EA83EC lea ecx, [esp + 0x34]
00EA83F0 call 0x99f570
00EA83F5 push 0
00EA83F7 push eax
00EA83F8 mov ecx, edi
00EA83FA call 0xcdd450
00EA83FF mov dword ptr [edi + 0x38], ebx
00EA8402 mov ebx, dword ptr [esp + 0x20]
00EA8406 mov dword ptr [edi], 0x12ee458
00EA840C mov dword ptr [edi + 0x34], 0xea85e0
00EA8413 jmp 0xea8417
00EA8415 xor edi, edi
00EA8417 push -1
00EA8419 push 0x122d70e
00EA841E lea ecx, [esp + 0x34]
00EA8422 call 0x99ebf0
00EA8427 mov ecx, dword ptr [esi + 0x14]
00EA842A push eax
00EA842B push edi
00EA842C call 0xcb7e50
00EA8431 lea ecx, [esp + 0x2c]
00EA8435 call 0x99eae0
00EA843A test bl, 4
00EA843D je 0xea844b
00EA843F lea ecx, [esp + 0x30]
00EA8443 and ebx, 0xfffffffb
00EA8446 call 0x99eae0
00EA844B test bl, 2
00EA844E je 0xea845c
00EA8450 lea ecx, [esp + 0x34]
00EA8454 and ebx, 0xfffffffd
00EA8457 call 0x99eae0
00EA845C test bl, 1
00EA845F je 0xea846d
00EA8461 lea ecx, [esp + 0x38]
00EA8465 and ebx, 0xfffffffe
00EA8468 call 0x99eae0
00EA846D mov ecx, dword ptr [esi + 0x18]
00EA8470 mov al, byte ptr [ecx + 0x8c]
00EA8476 test al, al
00EA8478 mov ecx, esi
00EA847A je 0xea84b9
00EA847C call 0xf35b30
00EA8481 test al, al
00EA8483 jne 0xea8598
00EA8489 push -1
00EA848B push 0x12ee5fc
00EA8490 lea ecx, [esp + 0x2c]
00EA8494 call 0x99ebf0
00EA8499 mov edx, dword ptr [esi + 0x14]
00EA849C push 1
00EA849E push 0
00EA84A0 add edx, 0x58
00EA84A3 push 0
00EA84A5 push edx
00EA84A6 lea edx, [esp + 0x50]
00EA84AA lea ecx, [esp + 0x34]
00EA84AE call 0xcbfb7d
00EA84B3 lea ecx, [esp + 0x24]
00EA84B7 jmp 0xea84f4
00EA84B9 call 0xf35b30
00EA84BE test al, al
00EA84C0 jne 0xea85a7
00EA84C6 push -1
00EA84C8 push 0x12ee5dc
00EA84CD lea ecx, [esp + 0x30]
00EA84D1 call 0x99ebf0
00EA84D6 mov eax, dword ptr [esi + 0x14]
00EA84D9 push 1
00EA84DB push 0
00EA84DD push 0
00EA84DF add eax, 0x58
00EA84E2 push eax
00EA84E3 lea edx, [esp + 0x50]
00EA84E7 lea ecx, [esp + 0x38]
00EA84EB call 0xcbfb7d
00EA84F0 lea ecx, [esp + 0x28]
00EA84F4 call 0x99eae0
00EA84F9 mov ecx, dword ptr [esi + 4]
00EA84FC mov edx, dword ptr [ecx]
00EA84FE push 0
00EA8500 call dword ptr [edx + 0x5cc]
00EA8506 mov ecx, dword ptr [esi + 4]
00EA8509 mov eax, dword ptr [ecx]
00EA850B lea edi, [esi + 8]
00EA850E push edi
00EA850F call dword ptr [eax + 0x580]
00EA8515 mov edx, dword ptr [ebp]
00EA8518 push 0
00EA851A mov ecx, ebp
00EA851C call dword ptr [edx + 0x5ec]
00EA8522 lea ecx, [esp + 0x5c]
00EA8526 call 0x6e7b80
00EA852B lea ecx, [esp + 0x40]
00EA852F call 0xcdbfb0
00EA8534 lea ecx, [esp + 0x4c]
00EA8538 call 0x7e74d0
00EA853D mov eax, dword ptr [esi + 0x14]
00EA8540 mov byte ptr [eax + 0x48], 1
00EA8544 mov ecx, dword ptr [esi + 4]
00EA8547 mov edx, dword ptr [ecx]
00EA8549 push edi
00EA854A call dword ptr [edx + 0x580]
00EA8550 mov ecx, dword ptr [esi + 4]
00EA8553 mov edx, dword ptr [ecx]
00EA8555 mov eax, ecx
00EA8557 mov ebp, dword ptr [eax]
00EA8559 push 0
00EA855B lea eax, [esp + 0x40]
00EA855F push eax
00EA8560 call dword ptr [edx + 0xa3c]
00EA8566 mov ecx, dword ptr [esi + 4]
00EA8569 push eax
00EA856A call dword ptr [ebp + 0x464]
00EA8570 lea ecx, [esp + 0x3c]
00EA8574 call 0x99eae0
00EA8579 mov ecx, dword ptr [esi + 4]
00EA857C mov edx, dword ptr [ecx]
00EA857E call dword ptr [edx + 0x1c]
00EA8581 mov ecx, esi
00EA8583 call 0xf35b30
00EA8588 test al, al
00EA858A je 0xea8270
00EA8590 pop edi
00EA8591 pop ebp
00EA8592 pop esi
00EA8593 pop ebx
00EA8594 add esp, 0x5c
00EA8597 ret 
00EA8598 mov eax, dword ptr [ebp]
00EA859B push 0
00EA859D mov ecx, ebp
00EA859F call dword ptr [eax + 0x5ec]
00EA85A5 jmp 0xea85b4
00EA85A7 mov edx, dword ptr [ebp]
00EA85AA push 0
00EA85AC mov ecx, ebp
00EA85AE call dword ptr [edx + 0x5ec]
00EA85B4 lea ecx, [esp + 0x5c]
00EA85B8 call 0x6e7b80
00EA85BD lea ecx, [esp + 0x40]
00EA85C1 call 0xcdbfb0
00EA85C6 lea ecx, [esp + 0x4c]
00EA85CA call 0x7e74d0
00EA85CF pop edi
00EA85D0 pop ebp
00EA85D1 pop esi
00EA85D2 pop ebx
00EA85D3 add esp, 0x5c
00EA85D6 ret 

EmptyGrave.Init
00EA80F0 push ecx
00EA80F1 push esi
00EA80F2 push -1
00EA80F4 mov esi, ecx
00EA80F6 push 0x12ee5d4
00EA80FB lea ecx, [esp + 0xc]
00EA80FF call 0x99ebf0
00EA8104 mov ecx, dword ptr [esi + 0x14]
00EA8107 lea eax, [esp + 4]
00EA810B push eax
00EA810C add ecx, 0x58
00EA810F call 0x8adf10
00EA8114 lea ecx, [esp + 4]
00EA8118 mov byte ptr [eax], 0
00EA811B call 0x99eae0
00EA8120 pop esi
00EA8121 pop ecx
00EA8122 ret 

MazeResearch.UNLIMBO
00EA85E0 push ecx
00EA85E1 push ebx
00EA85E2 push esi
00EA85E3 push edi
00EA85E4 push -1
00EA85E6 mov esi, ecx
00EA85E8 push 0x12ee5d4
00EA85ED lea ecx, [esp + 0x14]
00EA85F1 call 0x99ebf0
00EA85F6 lea eax, [esp + 0xc]
00EA85FA lea edi, [esi + 0x58]
00EA85FD push eax
00EA85FE mov ecx, edi
00EA8600 call 0x8adf10
00EA8605 cmp byte ptr [eax], 0
00EA8608 lea ecx, [esp + 0xc]
00EA860C sete bl
00EA860F call 0x99eae0
00EA8614 test bl, bl
00EA8616 je 0xea865a
00EA8618 mov ecx, dword ptr [esi + 0x40]
00EA861B mov edx, dword ptr [ecx]
00EA861D call dword ptr [edx + 0x1c]
00EA8620 mov ecx, esi
00EA8622 call 0xcb7940
00EA8627 test al, al
00EA8629 jne 0xea868a
00EA862B push -1
00EA862D push 0x12ee5d4
00EA8632 lea ecx, [esp + 0x14]
00EA8636 call 0x99ebf0
00EA863B lea eax, [esp + 0xc]
00EA863F push eax
00EA8640 mov ecx, edi
00EA8642 call 0x8adf10
00EA8647 cmp byte ptr [eax], 0
00EA864A lea ecx, [esp + 0xc]
00EA864E sete bl
00EA8651 call 0x99eae0
00EA8656 test bl, bl
00EA8658 jne 0xea8618
00EA865A mov ecx, esi
00EA865C call 0xcb7940
00EA8661 test al, al
00EA8663 jne 0xea868a
00EA8665 mov ecx, dword ptr [esi + 0x40]
00EA8668 mov edx, dword ptr [ecx]
00EA866A push 1
00EA866C push 0
00EA866E lea edi, [esi + 0x4c]
00EA8671 push edi
00EA8672 call dword ptr [edx + 0x714]
00EA8678 mov esi, dword ptr [esi + 0x40]
00EA867B mov eax, dword ptr [esi]
00EA867D push 1
00EA867F push 0
00EA8681 push edi
00EA8682 mov ecx, esi
00EA8684 call dword ptr [eax + 0x7f8]
00EA868A pop edi
00EA868B pop esi
00EA868C pop ebx
00EA868D pop ecx
00EA868E ret 

MazeResearch.OnPersist
00EA80A0 push ecx
00EA80A1 push esi
00EA80A2 push edi
00EA80A3 mov edi, dword ptr [esp + 0x10]
00EA80A7 mov esi, ecx
00EA80A9 lea eax, [esp + 0xb]
00EA80AD push eax
00EA80AE lea ecx, [esi + 0x48]
00EA80B1 push ecx
00EA80B2 push 0x12ee5c8
00EA80B7 mov ecx, edi
00EA80B9 mov byte ptr [esp + 0x17], 0
00EA80BE call 0x4045c0
00EA80C3 lea edx, [esp + 0x10]
00EA80C7 push edx
00EA80C8 add esi, 0x49
00EA80CB push esi
00EA80CC push 0x12ee5bc
00EA80D1 mov ecx, edi
00EA80D3 mov byte ptr [esp + 0x1c], 0
00EA80D8 call 0x4045c0
00EA80DD pop edi
00EA80DE pop esi
00EA80DF pop ecx
00EA80E0 ret 4
