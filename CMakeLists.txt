include(FetchContent)

cmake_minimum_required(VERSION 4.3.3)

project(infants-and-incantations)

add_executable(iai 
src/main.cpp
src/entity.cpp
src/engine.cpp
)

FetchContent_Declare(
	ogre
	GIT_REPOSITORY https://github.com/OGRECave/ogre
	GIT_TAG v14.5.2
)

FetchContent_MakeAvailable(ogre)

target_link_libraries(iai 
	PRIVATE 
		OgreMain 
		OgreBites
)

target_include_directories(iai PRIVATE include)
