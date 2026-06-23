#include <iostream>
#include "engine.h"

int main()
{
    /*
    std::cout << "Hello, testing. Did Ogre install correctly?" << std::endl;
    std::cout << "catherine was here" << std::endl;
    std::cout << "Hello this is Joe" << std::endl;
    std::cout << "Chels here, just testing" << std::endl;
    */

    // TODO: launch ogre view window here
    Engine engine;

    if (engine.Initialise() == true){
        engine.Run();
    }

    return 0;
}
