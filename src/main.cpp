/*
* TODO:
* - properly resizing (window and view tab)
* - asset loading (obj)
* - textures (albedo, normal, specular)
*   - only use the provided textures
* - scene hierarchy
*   - add objects/drag and drop them
* - object menu (instead of files menu rn)
*/

#include "SDL3/SDL_opengles2.h"
#include "platform.h"
#include "editor.h"
#include "renderer.h"

int main() {
    platform::Initialize();
    renderer::Initialize();
    editor::Initialize();

    while (!platform::ShouldQuit()) {
        platform::HandleEvents();

        editor::RenderFrame();
        platform::SwapBuffers();
    }

    editor::Release();
    renderer::Release();
    platform::Release();
}

