#include "iaiEngine.h"

int main()
{
    // create engine object
    Engine engine;
        
    // instanciate context and initialise app
    engine.Initialise();

    engine.Run();

    // rely on the destructor to correctly destroy the engine

    return 0;
}
