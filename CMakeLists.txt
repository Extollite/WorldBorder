cmake_minimum_required (VERSION 3.8)

# Change to your mod name
project ("WBDLSample")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
add_definitions (-DWIN32_LEAN_AND_MEAN=1)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
    add_compile_options (-Wno-c++2a-extensions -Wno-invalid-offsetof -Wno-deprecated-declarations)
endif ()
add_compile_options (/EHsc /utf-8)
add_definitions (-DWIN32_LEAN_AND_MEAN=1)

# Override it in CMakeSettings.json or command line option
set (MOD_SDK_PATH D:/eZTest)

include_directories (${MOD_SDK_PATH}/Include ${MOD_SDK_PATH}/Include)
link_directories (${MOD_SDK_PATH}/Lib)

find_package (SQLiteCpp CONFIG REQUIRED)
find_package (Boost 1.72.0 REQUIRED)
find_package (sqlite3 CONFIG REQUIRED)
find_package (yaml-cpp CONFIG REQUIRED)

function (import_library name location)
    add_library (${name} UNKNOWN IMPORTED)
    set_target_properties (${name} PROPERTIES IMPORTED_LOCATION ${location})
endfunction ()

import_library (ModLoader ${MOD_SDK_PATH}/Lib/Chakra.lib)
import_library (BedrockServer ${MOD_SDK_PATH}/Lib/bedrock_server_mod.lib)

function (install_pdb name)
    install (FILES $<TARGET_PDB_FILE:${name}> DESTINATION Lib)
endfunction ()

function (def_mod name)
    cmake_parse_arguments(ARG "" "" "LINK;DELAY_LINK" ${ARGN})
    file (GLOB_RECURSE srcs
        CONFIGURE_DEPENDS *.cpp)
    add_library (${name} SHARED ${srcs})
    target_compile_definitions (${name}
        PRIVATE MODNAME=${name})
    target_link_libraries (${name}
        Boost::headers
        yaml-cpp
        ModLoader
        BedrockServer
        Base
        ${ARG_LINK}
        ${ARG_DELAY_LINK})
    install (TARGETS ${name}
        RUNTIME DESTINATION Mods
        ARCHIVE DESTINATION Lib)
    install_pdb (${name})
    set_target_properties (${name}
        PROPERTIES FOLDER Mods)
    if (ARG_DELAY_LINK)
        target_link_libraries (${name} delayimp)
	foreach (target ${ARG_DELAY_LINK})
            target_link_options (${name} PRIVATE /DELAYLOAD:${target}.dll)
        endforeach ()
    endif ()
endfunction ()

# Change the name or append more
add_subdirectory (WorldBorder)
