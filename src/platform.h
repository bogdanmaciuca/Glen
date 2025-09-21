#pragma once
#include "int.h"
#include "SDL3/SDL_scancode.h"

struct SDL_Window;
struct SDL_GLContextState;

enum GlenKey {
    GLEN_KEY_A         = SDL_SCANCODE_A,
    GLEN_KEY_B         = SDL_SCANCODE_B,
    GLEN_KEY_C         = SDL_SCANCODE_C,
    GLEN_KEY_D         = SDL_SCANCODE_D,
    GLEN_KEY_E         = SDL_SCANCODE_E,
    GLEN_KEY_F         = SDL_SCANCODE_F,
    GLEN_KEY_G         = SDL_SCANCODE_G,
    GLEN_KEY_H         = SDL_SCANCODE_H,
    GLEN_KEY_I         = SDL_SCANCODE_I,
    GLEN_KEY_J         = SDL_SCANCODE_J,
    GLEN_KEY_K         = SDL_SCANCODE_K,
    GLEN_KEY_L         = SDL_SCANCODE_L,
    GLEN_KEY_M         = SDL_SCANCODE_M,
    GLEN_KEY_N         = SDL_SCANCODE_N,
    GLEN_KEY_O         = SDL_SCANCODE_O,
    GLEN_KEY_P         = SDL_SCANCODE_P,
    GLEN_KEY_Q         = SDL_SCANCODE_Q,
    GLEN_KEY_R         = SDL_SCANCODE_R,
    GLEN_KEY_S         = SDL_SCANCODE_S,
    GLEN_KEY_T         = SDL_SCANCODE_T,
    GLEN_KEY_U         = SDL_SCANCODE_U,
    GLEN_KEY_V         = SDL_SCANCODE_V,
    GLEN_KEY_W         = SDL_SCANCODE_W,
    GLEN_KEY_X         = SDL_SCANCODE_X,
    GLEN_KEY_Y         = SDL_SCANCODE_Y,
    GLEN_KEY_Z         = SDL_SCANCODE_Z,

    GLEN_KEY_1         = SDL_SCANCODE_1,
    GLEN_KEY_2         = SDL_SCANCODE_2,
    GLEN_KEY_3         = SDL_SCANCODE_3,
    GLEN_KEY_4         = SDL_SCANCODE_4,
    GLEN_KEY_5         = SDL_SCANCODE_5,
    GLEN_KEY_6         = SDL_SCANCODE_6,
    GLEN_KEY_7         = SDL_SCANCODE_7,
    GLEN_KEY_8         = SDL_SCANCODE_8,
    GLEN_KEY_9         = SDL_SCANCODE_9,
    GLEN_KEY_0         = SDL_SCANCODE_0,

    GLEN_KEY_RETURN    = SDL_SCANCODE_RETURN,
    GLEN_KEY_ESCAPE    = SDL_SCANCODE_ESCAPE,
    GLEN_KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
    GLEN_KEY_TAB       = SDL_SCANCODE_TAB,
    GLEN_KEY_SPACE     = SDL_SCANCODE_SPACE,
};

namespace platform {
    bool Initialize();
    void Release();
    void HandleEvents();
    void SwapBuffers();

    bool KeyIsDown(GlenKey key);
    float GetMouseX();
    float GetMouseY();

    bool ShouldQuit();
    SDL_Window* GetWindow();
    SDL_GLContextState* GetGLContext();
}

