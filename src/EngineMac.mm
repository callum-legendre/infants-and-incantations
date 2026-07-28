#include "engine.h"

#import <Cocoa/Cocoa.h>
#import <mach-o/dyld.h>

void Engine::createOgreWindow()
{
    SDL_SysWMinfo info{};
    SDL_VERSION(&info.version);

    if (!SDL_GetWindowWMInfo(mWindow, &info)){
        throw std::runtime_error(SDL_GetError());
    }
    
    if (info.subsystem != SDL_SYSWM_COCOA) {
        throw std::runtime_error("SDL window is not backed by Cocoa.");
    }

    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);

    Ogre::NameValuePairList params;
    params["externalWindowHandle"] =
        Ogre::StringConverter::toString(
            reinterpret_cast<uintptr_t>(info.info.cocoa.window));

    mRenderWindow =
        root->createRenderWindow(
            "Infants and Incantations",
            w,
            h,
            false,
            &params);
        
    // debug
    std::cout << "DEBUG: "
    << mRenderWindow->getWidth()
    << " "
    << mRenderWindow->getHeight()
    << std::endl;
    // end debug 

    if (!mRenderWindow)
        throw std::runtime_error("Failed to create Ogre render window.");
}

std::filesystem::path Engine::getResourceDirectory() const
{
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);

    std::string buffer(size, '\0');

    _NSGetExecutablePath(buffer.data(), &size);

    auto exe =
        std::filesystem::canonical(buffer);

    if (exe.parent_path().filename() == "MacOS")
        return exe.parent_path().parent_path() / "Resources";

    return exe.parent_path();
}