#include "DxLib.h"
#include "Test.h"

namespace {
    constexpr auto WINDOW_TITLE = _T("Spaceship Operator");
    constexpr auto SCREEN_WIDTH = 1280;
    constexpr auto SCREEN_HEIGHT = 720;
    constexpr auto SCREEN_DEPTH = 32;

    const auto strColor = GetColor(255, 255, 255);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    auto window_mode = FALSE;

#ifdef _DEBUG
    window_mode = TRUE;
#endif

    SetMainWindowText(WINDOW_TITLE);

    ChangeWindowMode(window_mode);

    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

    if (DxLib_Init() == -1) {
        return -1;
    }

    constexpr auto test = _T("Test");
#ifdef DEFAULT_OPERATOR
    auto test00 = Test{};
    auto test01 = Test{ 5, 10.0f, test };
    auto test02 = Test{ test01 };
    auto test03 = Test{ 5, 15.0f, test };
    auto test04 = Test{ 5, 10.0f, _T("West") };
#else
    constexpr auto nan = std::numeric_limits<float>::quiet_NaN();
    auto test00 = Test{};
    auto test01 = Test{ 5, nan, test };
    auto test02 = Test{ test01 };
    auto test03 = Test{ 5, 10.0f, test };
    auto test04 = Test{ 5, 10.0f, _T("West") };
#endif

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() != -1) {
        if (1 == CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        ClearDrawScreen();

        if (test01 == test02) {
            DrawString(10, 10, _T("test01 ‚Æ test02 ‚Í“¯‚¶’l"), strColor);
        }

        if (test01 < test03) {
            DrawString(10, 30, _T("test03 ‚Í test01 ‚æ‚è‘å‚«‚¢"), strColor);
        }

        if (test01 < test04) {
            DrawString(10, 50, _T("test04 ‚Í test01 ‚æ‚è‘å‚«‚¢"), strColor);
        }

        auto diff = test03 <=> test04;

        if (diff == 0) {
            DrawString(10, 70, _T("test03 ‚Æ test04 ‚Í“¯‚¶’l"), strColor);
        }
        else if (diff < 0) {
            DrawString(10, 70, _T("test03 ‚Í test04 ‚æ‚è¬‚³‚¢"), strColor);
        }
        else if (diff > 0) {
            DrawString(10, 70, _T("test03 ‚Í test04 ‚æ‚è‘å‚«‚¢"), strColor);
        }


        ScreenFlip();
    }

    DxLib_End();

    return 0;
}
