#include <iostream>
#include "Engine.h"

int main()
{
    // create engine object
    Engine engine;
        
    // instanciate context and initialise app
    engine.Initialise();

    engine.Run();

    engine.Shutdown();

    return 0;
}
