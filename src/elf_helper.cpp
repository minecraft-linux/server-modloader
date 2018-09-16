#include "elf_helper.h"

#include <cstdlib>
#include <elf.h>
#include <modloader/log.h>

using namespace modloader;

typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Dyn Elf_Dyn;

std::vector<std::string> ElfHelper::getDependencies(std::string const &path) {
    Elf_Ehdr header;
    FILE* file = fopen(path.c_str(), "r");
    if (file == nullptr) {
        Log::error("ModLoader", "getModDependencies: failed to open mod");
        return {};
    }
    if (fread(&header, sizeof(Elf_Ehdr), 1, file) != 1) {
        Log::error("ModLoader", "getModDependencies: failed to read header");
        fclose(file);
        return {};
    }

    fseek(file, (long) header.e_phoff, SEEK_SET);

    char phdr[header.e_phentsize * header.e_phnum];
    if (fread(phdr, header.e_phentsize, header.e_phnum, file) != header.e_phnum) {
        Log::error("ModLoader", "getModDependencies: failed to read phnum");
        fclose(file);
        return {};
    }

    // find dynamic
    Elf_Phdr* dynamicEntry = nullptr;
    for (int i = 0; i < header.e_phnum; i++) {
        Elf_Phdr& entry = *((Elf_Phdr*) &phdr[header.e_phentsize * i]);
        if (entry.p_type == PT_DYNAMIC)
            dynamicEntry = &entry;
    }
    if (dynamicEntry == nullptr) {
        Log::error("ModLoader", "getModDependencies: couldn't find PT_DYNAMIC");
        fclose(file);
        return {};
    }
    size_t dynamicDataCount = dynamicEntry->p_filesz / sizeof(Elf_Dyn);
    Elf_Dyn dynamicData[dynamicDataCount];
    fseek(file, (long) dynamicEntry->p_offset, SEEK_SET);
    if (fread(dynamicData, sizeof(Elf_Dyn), dynamicDataCount, file) != dynamicDataCount) {
        Log::error("ModLoader", "getModDependencies: failed to read PT_DYNAMIC");
        fclose(file);
        return {};
    }

    // find strtab
    size_t strtabOff = 0;
    size_t strtabSize = 0;
    for (int i = 0; i < dynamicDataCount; i++) {
        if (dynamicData[i].d_tag == DT_STRTAB) {
            strtabOff = dynamicData[i].d_un.d_val;
        } else if (dynamicData[i].d_tag == DT_STRSZ) {
            strtabSize = dynamicData[i].d_un.d_val;
        }
    }
    if (strtabOff == 0 || strtabSize == 0) {
        Log::error("ModLoader", "getModDependencies: couldn't find strtab");
        fclose(file);
        return {};
    }
    std::vector<char> strtab;
    strtab.resize(strtabSize);
    fseek(file, (long) strtabOff, SEEK_SET);
    if (fread(strtab.data(), 1, strtabSize, file) != strtabSize) {
        Log::error("ModLoader", "getModDependencies: failed to read strtab");
        fclose(file);
        return {};
    }
    std::vector<std::string> ret;
    for (int i = 0; i < dynamicDataCount; i++) {
        if (dynamicData[i].d_tag == DT_NEEDED)
            ret.emplace_back(&strtab[dynamicData[i].d_un.d_val]);
    }
    return ret;
}