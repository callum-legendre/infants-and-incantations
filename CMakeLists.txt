cmake_minimum_required(VERSION 3.24)

project(infants-and-incantations LANGUAGES CXX OBJCXX)

# fetch content for building off of most recent Ogre branch
include(FetchContent)

# need SDL2 for handling window and input
find_package(SDL2 CONFIG REQUIRED)

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

#
# -----------------------------------------------------------------------------
# Application
# -----------------------------------------------------------------------------
#

add_executable(iai MACOSX_BUNDLE
    src/Main.cpp
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
)

#
# -----------------------------------------------------------------------------
# Configuration files
# -----------------------------------------------------------------------------
#

set(OGRE_MEDIA_DIR "${ogre_SOURCE_DIR}/Media")

# Plugin folder for running the app directly from the build tree.
set(PLUGIN_FOLDER "${CMAKE_BINARY_DIR}/_deps/ogre-build/lib/macosx")
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

set(OGRE_MEDIA_DIR "${ogre_SOURCE_DIR}/Media")
# configure resources
configure_file(
    resources.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg
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

install(DIRECTORY
    ${CMAKE_SOURCE_DIR}/assets
    DESTINATION iai.app/Contents/Resources
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg
    DESTINATION iai.app/Contents/Resources
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/install_plugins.cfg
    DESTINATION iai.app/Contents/Resources
    RENAME plugins.cfg
)


#
# OGRE runtime plugins
#

install(
    DIRECTORY
        ${CMAKE_BINARY_DIR}/_deps/ogre-build/lib/macosx/RenderSystem_GL3Plus.framework
    DESTINATION
        iai.app/Contents/Frameworks
)

install(
    DIRECTORY
        ${CMAKE_BINARY_DIR}/_deps/ogre-build/lib/macosx/Codec_FreeImage.framework
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
