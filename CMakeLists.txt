# finds the OGRE library
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# creates the executable file
add_executable(i-squared main.cpp)

# includes and builds third party dependancies
target_link_libraries(i-squared OgreBites)
