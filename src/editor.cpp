#include "editor.h"
#include "SDL3/SDL_opengles2.h"
#include "../vendor/imgui/imgui_impl_sdl3.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_internal.h"
#include "platform.h"
#include "renderer.h"
#include "camera.h"
#include "gtime.h"

namespace editor {
    namespace {
        ImGuiIO*       m_io        = nullptr;
        ImGuiID        m_dockspace = 0;
        ImGuiViewport* m_viewport  = nullptr;

        float  m_delta_time;
        gtime::Timer  m_frame_timer;
        Camera m_camera;
    }

    void BeginDockspace();
    void EndDockspace();
    void DockBuilder();
    void BeginFrame();
    void EndFrame();
    void DrawViewTab();
    void DrawSceneTab();
    void DrawFilesTab();

    bool Initialize() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO();
        m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL3_InitForOpenGL(platform::GetWindow(), platform::GetGLContext());
        ImGui_ImplOpenGL3_Init("#version 100");
        m_io->IniFilename = nullptr;
        m_io->LogFilename = nullptr;
        m_viewport = ImGui::GetMainViewport();
        return true;
    }

    void Release() {}

    void RenderFrame() {
        BeginFrame();

        DrawViewTab();
        DrawSceneTab();
        DrawFilesTab();

        EndFrame();
    }

    void BeginDockspace() {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        ImGui::SetNextWindowPos(m_viewport->WorkPos);
        ImGui::SetNextWindowSize(m_viewport->WorkSize);
        ImGui::SetNextWindowViewport(m_viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        window_flags |=
            ImGuiWindowFlags_NoTitleBar            |
            ImGuiWindowFlags_NoCollapse            |
            ImGuiWindowFlags_NoResize              |
            ImGuiWindowFlags_NoMove                |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;


        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::DockSpace(m_dockspace);

        ImGui::PopStyleVar(3);
    }

    void EndDockspace() {
        ImGui::End();
    }

    void DockBuilder() {
        static auto first_time = true;
        if (first_time) {
            first_time = false;

            ImGui::DockBuilderRemoveNode(m_dockspace);
            ImGui::DockBuilderAddNode(m_dockspace, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(m_dockspace, m_viewport->Size);

            ImGuiID view_id;
            ImGuiID scene_id = ImGui::DockBuilderSplitNode(
                m_dockspace, ImGuiDir_Left, 0.2f, nullptr, &view_id);
            ImGuiID files_id = ImGui::DockBuilderSplitNode(
                m_dockspace, ImGuiDir_Down, 0.2f, nullptr, &m_dockspace);

            // we now dock our windows into the docking node we made above
            ImGui::DockBuilderDockWindow("View", view_id);
            ImGui::DockBuilderDockWindow("Scene", scene_id);
            ImGui::DockBuilderDockWindow("Files", files_id);
            ImGui::DockBuilderFinish(m_dockspace);
        }
    }

    void BeginFrame() {
        m_delta_time = m_frame_timer.GetTime();
        m_frame_timer.Reset();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        m_dockspace = ImGui::GetID("GlenDockspace");
        BeginDockspace();
        DockBuilder();
    }

    void EndFrame() {
        EndDockspace();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DrawViewTab() {
        // TODO: maybe move to input.h
        static float last_mouse_x = platform::GetMouseX();
        static float last_mouse_y = platform::GetMouseY();
        float mouse_x = platform::GetMouseX();
        float mouse_y = platform::GetMouseY();
        float delta_mouse_x = mouse_x - last_mouse_x;
        float delta_mouse_y = mouse_y - last_mouse_y;
        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;

        if      (platform::KeyIsDown(GLEN_KEY_W)) m_camera.MoveForward(m_delta_time);
        else if (platform::KeyIsDown(GLEN_KEY_S)) m_camera.MoveBackward(m_delta_time);
        if      (platform::KeyIsDown(GLEN_KEY_A)) m_camera.MoveLeft(m_delta_time);
        else if (platform::KeyIsDown(GLEN_KEY_D)) m_camera.MoveRight(m_delta_time);
        m_camera.ProcessMouse(delta_mouse_x, delta_mouse_y);

        renderer::SetCamera(m_camera.GetPos(), m_camera.GetViewMatrix());
        renderer::RenderFrame();

        ImGui::Begin("View");
        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        const float view_width = ImGui::GetContentRegionAvail().x;
        const float view_height = ImGui::GetContentRegionAvail().y;
        renderer::EnsureResized(view_width, view_height);
        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureRef)renderer::GetOutputTexture(), window_pos,
            ImVec2(window_pos.x + view_width, window_pos.y + view_height),
            ImVec2(0, 1), ImVec2(1, 0)
        );
        ImGui::End();
    }

    void DrawSceneTab() {
        ImGui::Begin("Scene");
        ImGui::Text("Hello from Scene!");
        ImGui::End();
    }

    void DrawFilesTab() {
        ImGui::Begin("Files");
        ImGui::Text("Hello from Files!");
        ImGui::End();
    }
}

