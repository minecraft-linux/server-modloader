#include <modloader/loader.h>
#include "loader.h"

#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <modloader/log.h>
#include "elf_helper.h"

using namespace modloader;

static ModLoaderImpl loaderImpl;

extern "C" {

const char *modloader_version() {
    return ModLoader::getVersion();
}

void modloader_add_lib_search_path(const char* path) {
    loaderImpl.addLibSearchDir(path);
}

void *modloader_load_mod(const char* path) {
    return loaderImpl.loadMod(path);
}

void modloader_load_mods_from_directory(const char* path) {
    loaderImpl.loadModsFromDirectory(path);
}

void modloader_iterate_mods(modloader_foreach_fn cb, void* userdata) {
    for (void* v : loaderImpl.mods)
        cb(v, userdata);
}

}

const char *ModLoader::getVersion() {
    return MODLOADER_VERSION;
}

void ModLoader::addLibSearchDir(std::string const &path) {
    loaderImpl.addLibSearchDir(path);
}

void* ModLoader::loadMod(std::string const &path) {
    return loaderImpl.loadMod(path);
}

void ModLoader::loadModsFromDirectory(std::string const &path) {
    loaderImpl.loadModsFromDirectory(path);
}

void ModLoader::forEachMod(std::function<void(void *)> cb) {
    for (void* v : loaderImpl.mods)
        cb(v);
}

std::string ModLoaderImpl::findLib(std::string const &name) {
    for (std::string const& dir : libDirs) {
        std::string fullPath = dir;
        fullPath += '/';
        fullPath += name;
        if (access(fullPath.c_str(), R_OK) == 0)
            return fullPath;
    }
    return std::string();
}

void *ModLoaderImpl::loadLib(std::string const &path) {
    auto e = knownLoadedLibs.find(path);
    if (e != knownLoadedLibs.end())
        return e->second;

    auto iof = path.rfind('/');
    std::string fullPath = path;
    if (iof == std::string::npos)
        fullPath = findLib(path);

    if (!fullPath.empty()) {
        Log::info("ModLoader", "Loading library: %s", fullPath.c_str());

        auto deps = ElfHelper::getDependencies(fullPath);
        for (std::string const& dep : deps)
            loadLib(dep);
    } else {
        fullPath = path;
    }

    void* ret = dlopen(fullPath.c_str(), RTLD_NOW);
    if (!ret) {
        Log::error("ModLoader", "Failed loading library %s: %s", fullPath.c_str(), dlerror());
        return nullptr;
    }
    std::string filename = iof != std::string::npos ? path.substr(iof + 1) : path;
    knownLoadedLibs[filename] = ret;
    return ret;
}

void *ModLoaderImpl::loadMod(std::string const &path) {
    void* ret = loadLib(path);
    mods.insert(ret);
    return ret;
}

void ModLoaderImpl::loadModsFromDirectory(std::string const &path) {
    Log::info("ModLoader", "Loading mods from directory: %s", path.c_str());
    DIR* dir = opendir(path.c_str());
    dirent* ent;
    if (dir == nullptr) {
        Log::info("ModLoader", "Directory does not exist");
        return;
    }
    int errors = 0;
    while ((ent = readdir(dir)) != nullptr) {
        if (ent->d_name[0] == '.')
            continue;
        std::string fileName(ent->d_name);
        size_t len = fileName.length();

        if(loadMod(fileName) == nullptr){
            errors++;
        }
    }
    closedir(dir);
    Log::info("ModLoader", "Loaded %li mods", mods.size() - errors);
    if(errors > 0){
        Log::error("ModLoader", "%d mods failed to load, aborting", errors);
        exit(1);
    }
}
