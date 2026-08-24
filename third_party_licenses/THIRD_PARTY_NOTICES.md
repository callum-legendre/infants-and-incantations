# Third-Party Notices

*Infants and Incantations* redistributes the components listed below. Their full licence texts are included in this directory and are packaged with the macOS application.

## OGRE

- **Version:** 14.5.2
- **Website:** <https://www.ogre3d.org/>
- **Source:** <https://github.com/OGRECave/ogre/tree/v14.5.2>
- **Licence:** MIT
- **Licence text:** `OGRE-MIT.txt`

## Bullet Physics

Bullet is built as an OGRE dependency and used through the OgreBullet component.

- **Version:** 3.25
- **Website:** <https://pybullet.org/>
- **Source:** <https://github.com/bulletphysics/bullet3>
- **Licence:** zlib
- **Licence text:** `BULLET-ZLIB.txt`

## SDL2 compatibility layer

The application uses the SDL2 API provided by `sdl2-compat`. The exact package version is supplied by the build environment and must be checked when preparing a release.

- **Verified development version:** 2.32.70
- **Website:** <https://www.libsdl.org/>
- **Source:** <https://github.com/libsdl-org/sdl2-compat>
- **Licence:** zlib
- **Licence text:** `SDL2-COMPAT-ZLIB.txt`

## SDL3

The build system packages SDL3 when it is exposed as a shared runtime dependency by the installed SDL compatibility package. The exact package version is supplied by the build environment and must be checked when preparing a release.

- **Verified development version:** 3.4.12
- **Website:** <https://www.libsdl.org/>
- **Source:** <https://github.com/libsdl-org/SDL>
- **Licence:** zlib
- **Licence text:** `SDL3-ZLIB.txt`

## FreeImage

FreeImage is built as a dependency of OGRE's `Codec_FreeImage` plugin.

- **Version:** 3.18.0
- **Website:** <https://freeimage.sourceforge.io/>
- **Source:** <https://sourceforge.net/projects/freeimage/files/Source%20Distribution/3.18.0/>
- **Licence:** FreeImage Public License 1.0
- **Licence text:** `FREEIMAGE.txt`

The project does not modify FreeImage. Its original source is available from the link above under the terms of the FreeImage Public License 1.0.

These notices apply only to the identified third-party components. They do not grant a licence for the original *Infants and Incantations* source code or assets.
