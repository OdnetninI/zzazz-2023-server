# A reimplementation of TheZZAZZGlitch's April Fools Event 2023 server
After the end of the event, most of us were waiting for the server code release. In part because we wanted to understand the 0x07 (MIX) opcode.

Most of the information used to build this server was already acquired by Radixan and me (OdnetninI) during the event.

However, I had some missing fragments. I was able to recover them from the following writeups:
- Muzuwi: https://blog.muzuwi.dev/posts/2023/04/14/fools2023/
- jfb1337: https://github.com/joefarebrother/Fools2023/blob/main/README.md

Here are my writeups regarding this challenge:
- The big challenge writeup is on the way :)
- About the development of the Assembler: https://cohost.org/OdnetninI/post/1441096-finally-i-have-im ( Backup: https://www.edujgh.net/blog.html#zzazz23-comp )

# TheZZAZZGlitch released the code
Finally, TheZZAZZGlitch released the code (https://github.com/zzazzdzz/fools2023 ). I was able to implement the MIX/UNMIX opcodes successfully.

Also, many thanks TheZZAZZGlitch. Not only for recommending this repository for playing the challenge but also for creating it.
Thanks for all these years of challenges, we have learned a lot. We wish you all the best :)

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
- programs: Assembly version of the included programs
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
- [ ] Use TheZZAZZGlitch's source code to finish adjusting the behaviour
- [ ] Filesystem tools for easily inserting, modifing and deleting files
- [ ] GLVM Binaries source code
  + [x] BIOS v1.3
  + [x] Boot 13337
  + [ ] Boot 13338
  + [ ] Boot 13339
  + [x] MATHTEST
  + [x] REPORT03
  + [ ] MIX/UNMIX
- [x] Opcode 0x07 `MIX`
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

# License
This project is under the MIT License (see LICENSE for more information).

