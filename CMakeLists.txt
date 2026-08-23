cmake_minimum_required(VERSION 3.24)

project(infants-and-incantations LANGUAGES CXX OBJCXX)

find_program(CODESIGN_EXECUTABLE codesign REQUIRED)

# fetch content for building off of most recent Ogre branch
include(FetchContent)

# need SDL2 for handling window and input
find_package(SDL2 CONFIG REQUIRED)

# Homebrew's SDL2 package may be sdl2-compat, which loads SDL3 dynamically at
# runtime rather than exposing it as a linked dependency.
find_package(SDL3 CONFIG QUIET)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# -----------------------------------------------------------------------------
# OGRE
# -----------------------------------------------------------------------------
#

# get most recent ogre branch
FetchContent_Declare(
    ogre
    GIT_REPOSITORY https://github.com/OGRECave/ogre.git
    GIT_TAG v14.5.2
    GIT_SHALLOW true
    GIT_PROGRESS true
)

#
# Build shared OGRE libraries
#
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(OGRE_STATIC OFF CACHE BOOL "" FORCE)
set(OGRE_BITES_STATIC_PLUGINS OFF CACHE BOOL "" FORCE)

#
# Build only the plugins we need
#
set(OGRE_BUILD_RENDERSYSTEM_METAL OFF CACHE BOOL "" FORCE)
set(OGRE_BUILD_RENDERSYSTEM_GL OFF CACHE BOOL "" FORCE)
set(OGRE_BUILD_RENDERSYSTEM_GL3PLUS ON CACHE BOOL "" FORCE)
set(OGRE_BUILD_PLUGIN_FREEIMAGE ON CACHE BOOL "" FORCE)

#
# Build Bullet for the physics engine
#
set(OGRE_BUILD_DEPENDENCIES ON CACHE BOOL "" FORCE)
set(OGRE_BUILD_COMPONENT_BULLET ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(ogre)

# OGRE is an implementation dependency. Its targets are still built when iai
# needs them, but its SDK, samples, and development files are not installed
# alongside the game.
set_property(DIRECTORY "${ogre_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

#
# -----------------------------------------------------------------------------
# Application
# -----------------------------------------------------------------------------
#

add_executable(iai MACOSX_BUNDLE
    src/main.cpp
    src/GameObject.cpp
    src/iaiEngine.cpp
    src/iaiEngineMac.mm
    src/LevelScene.cpp
    src/Player.cpp
    src/PlayerCamera.cpp
    src/GameSession.cpp
)

set_target_properties(iai PROPERTIES
    MACOSX_BUNDLE TRUE
    OUTPUT_NAME iai
)

target_include_directories(iai PRIVATE
    include
)

# link ogre libraries we need and SDL2
target_link_libraries(iai PRIVATE
    SDL2::SDL2
    OgreMain
    OgreBites
    OgreOverlay
    OgreRTShaderSystem
    OgreBullet
)

add_custom_command(TARGET iai POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:iai>/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg"
        "${CMAKE_CURRENT_BINARY_DIR}/resources.cfg"
        "$<TARGET_BUNDLE_CONTENT_DIR:iai>/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_BUNDLE_CONTENT_DIR:iai>/Resources/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/third_party_licenses"
        "$<TARGET_BUNDLE_CONTENT_DIR:iai>/Resources/Licenses"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Model-Sources.txt"
        "$<TARGET_BUNDLE_CONTENT_DIR:iai>/Resources/Licenses/Model-Sources.txt"
)

#
# -----------------------------------------------------------------------------
# Configuration files
# -----------------------------------------------------------------------------
#

set(OGRE_PLUGIN_DIR "${ogre_BINARY_DIR}/lib/macosx")

# Plugin folder for running the app directly from the build tree.
set(PLUGIN_FOLDER "${OGRE_PLUGIN_DIR}")
configure_file(
    plugins.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg
    @ONLY
)

# Plugin folder for the installed app bundle. This is relative to
# iai.app/Contents/Resources/plugins.cfg.
set(PLUGIN_FOLDER "../Frameworks")
configure_file(
    plugins.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/install_plugins.cfg
    @ONLY
)

# Use OGRE media directly from the fetched source tree while developing.
set(OGRE_MAIN_RESOURCE_PATH "${ogre_SOURCE_DIR}/Media/Main")
set(OGRE_RTSS_RESOURCE_PATH "${ogre_SOURCE_DIR}/Media/RTShaderLib")
configure_file(
    resources.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg
    @ONLY
)

# The installed app contains its own copy of the required OGRE media.
set(OGRE_MAIN_RESOURCE_PATH "ogre-media/Main")
set(OGRE_RTSS_RESOURCE_PATH "ogre-media/RTShaderLib")
configure_file(
    resources.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/install_resources.cfg
    @ONLY
)

# configure fix bundle
configure_file(
    ${CMAKE_SOURCE_DIR}/FixBundle.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FixBundle.cmake
    @ONLY
)

#
# -----------------------------------------------------------------------------
# Bundle resources
# -----------------------------------------------------------------------------
#

install(TARGETS iai
    BUNDLE DESTINATION .
)

# The build-tree bundle contains development configuration and may also contain
# runtime logs. Recreate its Resources directory for a clean, portable install.
install(CODE [[
    file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/iai.app/Contents/Resources")
]])

install(DIRECTORY
    ${CMAKE_SOURCE_DIR}/assets
    DESTINATION iai.app/Contents/Resources
    PATTERN ".DS_Store" EXCLUDE
    PATTERN "*.log" EXCLUDE
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/install_resources.cfg
    DESTINATION iai.app/Contents/Resources
    RENAME resources.cfg
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/install_plugins.cfg
    DESTINATION iai.app/Contents/Resources
    RENAME plugins.cfg
)

if(TARGET SDL3::SDL3-shared)
    get_target_property(SDL3_RUNTIME_LIBRARY
        SDL3::SDL3-shared IMPORTED_LOCATION)

    if(NOT SDL3_RUNTIME_LIBRARY)
        get_target_property(SDL3_RUNTIME_LIBRARY
            SDL3::SDL3-shared IMPORTED_LOCATION_RELEASE)
    endif()

    if(SDL3_RUNTIME_LIBRARY)
        get_filename_component(SDL3_RUNTIME_LIBRARY
            "${SDL3_RUNTIME_LIBRARY}" REALPATH)
        install(FILES
            "${SDL3_RUNTIME_LIBRARY}"
            DESTINATION iai.app/Contents/Frameworks
            RENAME libSDL3.dylib
        )
    endif()
endif()

install(DIRECTORY
    ${ogre_SOURCE_DIR}/Media/Main
    ${ogre_SOURCE_DIR}/Media/RTShaderLib
    DESTINATION iai.app/Contents/Resources/ogre-media
)

install(DIRECTORY
    ${CMAKE_SOURCE_DIR}/third_party_licenses/
    DESTINATION iai.app/Contents/Resources/Licenses
)

install(FILES
    ${CMAKE_SOURCE_DIR}/Model-Sources.txt
    DESTINATION iai.app/Contents/Resources/Licenses
)


#
# OGRE runtime plugins
#

install(
    DIRECTORY
        ${OGRE_PLUGIN_DIR}/RenderSystem_GL3Plus.framework
    DESTINATION
        iai.app/Contents/Frameworks
)

install(
    DIRECTORY
        ${OGRE_PLUGIN_DIR}/Codec_FreeImage.framework
    DESTINATION
        iai.app/Contents/Frameworks
)


install(SCRIPT
    ${CMAKE_CURRENT_BINARY_DIR}/FixBundle.cmake
)
# build command
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# cmake --build build
# cmake --install build --prefix install
