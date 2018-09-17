add_library(ModLoader SHARED IMPORTED)

find_library(MODLOADER_LIBRARY server_modloader HINTS "${CMAKE_CURRENT_LIST_DIR}/../../../lib")
set(MODLOADER_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../include/")
set_target_properties(ModLoader PROPERTIES IMPORTED_LOCATION "${MODLOADER_LIBRARY}")
set_target_properties(ModLoader PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MODLOADER_INCLUDE_DIR}")