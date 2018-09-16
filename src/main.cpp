#include <modloader/loader.h>

using namespace modloader;

__attribute__((constructor))
static void modloader_init() {
    ModLoader::addLibSearchDir("mods");
    ModLoader::loadModsFromDirectory("mods");
}