#include <cassert>
#include "log.h"
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

