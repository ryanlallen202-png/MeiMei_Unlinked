// MeiMei_Client_Android.cpp
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MeiMei", __VA_ARGS__))

// Self-contained state
namespace ClientState {
    bool MenuOpen = true;
    bool FeatureA = false;
}

// Auto-execute on load
__attribute__((constructor)) void OnLoad() {
    LOGI("MeiMei Client v5 loaded into Orion Drift");
    // Initialization logic hooks go here
}

extern "C" JNIEXPORT void JNICALL
Java_com_driftrinth_loader_InjectionHook(JNIEnv* env, jobject thiz) {
    LOGI("Driftrinth injection hook triggered");
    ClientState::MenuOpen = true;
}

void RenderMenu() {
    if (!ClientState::MenuOpen) return;
    ImGui::Begin("MeiMei Client v5", &ClientState::MenuOpen);
    ImGui::Checkbox("Feature A", &ClientState::FeatureA);
    ImGui::End();
}

void RenderUnlinkedMenu() {
    if (!ClientState::MenuOpen) return;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("MeiMei Client v5", &ClientState::MenuOpen, ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ClientState::AccentColor);
    ImGui::PushStyleColor(ImGuiCol_TabSelected, ClientState::AccentColor);

    if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_None)) {
        
        if (ImGui::BeginTabItem("Combat")) {
            ImGui::Spacing();
            ImGui::Checkbox("Enable Aimbot", &ClientState::Aimbot);
            ImGui::SliderFloat("FOV Radius", &ClientState::AimFov, 1.0f, 90.0f, "%.1f");
            ImGui::Checkbox("Silent Aim", &ClientState::SilentAim);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Visuals")) {
            ImGui::Spacing();
            ImGui::Checkbox("2D Box ESP", &ClientState::EspBox);
            ImGui::Checkbox("Skeleton ESP", &ClientState::EspSkeleton);
            ImGui::SliderFloat("Render Distance", &ClientState::EspDistance, 50.0f, 1000.0f, "%.0f");
            ImGui::Spacing();
            ImGui::Text("Accent Color");
            ImGui::ColorEdit3("##Accent", (float*)&ClientState::AccentColor, ImGuiColorEditFlags_NoInputs);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Movement")) {
            ImGui::Spacing();
            ImGui::Checkbox("Speed Hack", &ClientState::SpeedHack);
            ImGui::SliderFloat("Multiplier", &ClientState::SpeedMultiplier, 1.0f, 5.0f, "%.1fx");
            ImGui::Checkbox("No Clip", &ClientState::NoClip);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    
    ImGui::PopStyleColor(3);
    ImGui::End();
}

HRESULT WINAPI HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!g_Init) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
            pDevice->GetImmediateContext(&pContext);
            
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            
            ImGui_ImplWin32_Init(sd.OutputWindow);
            ImGui_ImplDX11_Init(pDevice, pContext);
            
            g_Init = true;
        }
    }

    if (g_Init) {
        // Toggle menu with INSERT key
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            ClientState::MenuOpen = !ClientState::MenuOpen;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderUnlinkedMenu();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    return S_OK; 
}

DWORD WINAPI SetupThread(LPVOID) {
    while (GetModuleHandleA("d3d11.dll") == nullptr) {
        Sleep(100);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, SetupThread, hModule, 0, nullptr);
    }
    return TRUE;
}
