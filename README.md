# A reimplementation of TheZZAZZGlitch's April Fools Event 2023 server
After the end of the event, most of us were waiting for the server code release. In part because we wanted to understand the 0x07 opcode. But there is no info yet.

Most of the information used to build this server was already acquired by Radixan and me (OdnetninI) during the event.

However, I had some missing fragments. I was able to recover them from the following writeups:
- Muzuwi: https://blog.muzuwi.dev/posts/2023/04/14/fools2023/
- jfb1337: https://github.com/joefarebrother/Fools2023/blob/main/README.md

Also, my writeup is on the way :)

# Directory Structure
- src: server sources
- bios: BIOS files (only 1.3 is known) 
  + It is mapped in memory from 0x0000 to 0x0fff
- boot: Main programs at boot
  + Mapped in memory at 0xf000
- fs: Filesystem of each machine
  + The structure is fs/$server_id/$file_block
  + $server_id is 0, 1, or 2 (see Usage for more information)
  + $file_block is a 1-byte index of the file
- tools: several tools for working with GLVM files
  + assembler: An assembler to compile GLVM ASM into GLVM binary
  + disassembler: A simple tool that dumps a GLVM binary into GLVM ASM

# Compiling
It requires a typical modern C compiler.

```
git clone https://github.com/OdnetninI/zzazz-2023-server.git
cd zzazz-2023-server
make
```

Now the file *zzazz* file should be in the directory.

# Usage
For now, only local mode is available (it means no socket to connect to the internet).

```
./zzazz server_id
```

Servers id are:
- 0 -> Server 13337 GRLTS01
- 1 -> Server 13338 GRLTS02
- 2 -> Server 13339 GRLINFSRV

# Status
- [ ] Filesystem tools for easily inserting, modifing and deleting files
- [ ] GLVM Binaries source code
  + [x] BIOS v1.3
  + [x] Boot 13337
  + [ ] Boot 13338
  + [ ] Boot 13339
  + [ ] MATHTEST
  + [ ] REPORT03
  + [ ] MIX/UNMIX
- [ ] Opcode 0x07 `MIX`
- [ ] NOP*
- [ ] Sockets (For network operation)
- [x] Main functions implemented
- [x] BIOS protection
- [x] Opcodes
- [x] Filesystem
- [x] Boot programs
- [ ] Cleanup
  + [x] Cleanup 1.0
  + [ ] Cleanup 2.0
- [x] Math test data

* There are two NOPs, at the begnining I thought a delay was needed for the timing attack, however, is more probable that one of them is the `UNMIX` opcode.

# License
This project is under the MIT License (see LICENSE for more information).

