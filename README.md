# ModLoader

This is a tool to load mods onto the Linux version of [Bedrock Dedicated Server](https://minecraft.net/download/server/bedrock).

## Usage
- Get `libserver_modloader.so`, either from [releases](https://github.com/minecraft-linux/server-modloader/releases) or [building your own](#compiling-from-source)
- Put your compiled mods in a folder called `mods/` next to your moddable BDS binary
- `LD_PRELOAD=path/to/libserver_modloader.so ./bedrock_server_symbols_moddable.debug`

## Compiling from source
1. `git clone https://github.com/minecraft-linux/server-modloader.git --recursive`
2. `cd server-modloader`
3. `mkdir build && cd build`
4. `cmake ..`
5. `make`

You now have a `libserver_modloader.so` binary.

## Getting mods to work on newer versions (1.16+)
Newer versions of BDS don't have exported symbols anymore, so a little hacking is necessary to modify the binary and make the symbols linkable.

Using [LIEF](https://github.com/lief-project/lief), you can create a patched, moddable BDS from `bedrock_server_symbols.debug` using the following Python script:

```py
import lief
import sys

lib_symbols = lief.parse("bedrock_server_symbols.debug")
for s in filter(lambda e: e.exported, lib_symbols.static_symbols):
    lib_symbols.add_dynamic_symbol(s)
lib_symbols.write("bedrock_server_symbols_patched.debug")
```
