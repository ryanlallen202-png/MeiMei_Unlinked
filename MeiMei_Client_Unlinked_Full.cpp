// MeiMei_Client_Android.cpp
// Orion Drift Mod - Android ARM64 OpenGL ES 3
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MeiMei", __VA_ARGS__))

namespace ClientState {
    bool MenuOpen = true;
    bool SpeedHack = false;
    float SpeedValue = 2.0f;
    bool ESPBox = false;
}

__attribute__((constructor)) void OnLoad() {
    LOGI("MeiMei Client v5 loaded");
}

extern "C" JNIEXPORT void JNICALL
Java_com_driftrinth_loader_InjectionHook(JNIEnv* env, jobject thiz) {
    LOGI("Injection hook triggered");
    ClientState::MenuOpen = true;
}

void RenderMenu() {
    if (!ClientState::MenuOpen) return;
    
    ImGui::Begin("MeiMei Client v5", &ClientState::MenuOpen);
    ImGui::Text("Orion Drift - Quest Mod");
    ImGui::Separator();
    
    ImGui::Checkbox("Speed Hack", &ClientState::SpeedHack);
    ImGui::SliderFloat("Speed", &ClientState::SpeedValue, 1.0f, 10.0f);
    ImGui::Checkbox("ESP Box", &ClientState::ESPBox);
    
    ImGui::End();
}
