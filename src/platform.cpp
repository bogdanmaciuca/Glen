#include "platform.h"
#include <cstdio>
#include "SDL3/SDL.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_sdl3.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

namespace platform {
    namespace {
        SDL_Window*   m_window;
        SDL_GLContext m_gl_context;
        const bool*   m_keys;
        float         m_mouse_x;
        float         m_mouse_y;

        bool m_should_quit = false;
    }

    bool Initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) == false)
            return false;

        m_window = SDL_CreateWindow("Glen", 1024, 768, SDL_WINDOW_OPENGL);
        if (m_window == nullptr)
            return false;

        // GLES version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        // Swapchain buffers
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_gl_context = SDL_GL_CreateContext(m_window);
        if (m_gl_context == nullptr)
            return false;

        // VSYNC
        SDL_GL_SetSwapInterval(1);

        // Keyboard state
        m_keys = SDL_GetKeyboardState(nullptr);

        return true;
    }

    void Release() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DestroyContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void HandleEvents() {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            switch (evt.type) {
                case SDL_EVENT_QUIT:
                    m_should_quit = true;
                    break;
            }
            ImGui_ImplSDL3_ProcessEvent(&evt);
        }
        SDL_GetMouseState(&m_mouse_x, &m_mouse_y);
    }

    void SwapBuffers() {
        SDL_GL_SwapWindow(m_window);
    }

    bool KeyIsDown(GlenKey key) {
        return m_keys[key];
    }

    float GetMouseX() {
        return m_mouse_x;
    }

    float GetMouseY() {
        return m_mouse_y;
    }

    bool ShouldQuit() {
        return m_should_quit;
    }

    SDL_Window* GetWindow() {
        return m_window;
    }

    SDL_GLContext GetGLContext() {
        return m_gl_context;
    }
}

