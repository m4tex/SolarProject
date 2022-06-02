#include "window.cpp"
#include "engine.cpp"

int main() {
    HWND mainHwnd;
    Window::Create(&mainHwnd);
    Engine::Start(mainHwnd);
    SetWindowTextA(mainHwnd, "WOOOOOOOOOOOO");
    return 0;
}