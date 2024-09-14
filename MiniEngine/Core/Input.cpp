//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#include "pch.h"
#include "GameCore.h"
#include "Input.h"
#include "DirectXTK/GamePad.h"

#define USE_KEYBOARD_MOUSE
#include "DirectXTK/Keyboard.h"
#include "DirectXTK/Mouse.h"

namespace GameInput
{
    std::unique_ptr<DirectX::GamePad> g_GamePad;
    std::unique_ptr<DirectX::Keyboard> g_Keyboard;
    std::unique_ptr<DirectX::Mouse> g_Mouse;
}

using DirectX::GamePad;
using DirectX::Keyboard;
using DirectX::Mouse;

namespace GameCore
{
    extern HWND g_hWnd;
}

namespace GameInput
{
    void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
    {
#ifdef USE_KEYBOARD_MOUSE
        switch( message )
        {
        case WM_ACTIVATEAPP:
            Keyboard::ProcessMessage(message, wParam, lParam);
            Mouse::ProcessMessage(message, wParam, lParam);
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            Keyboard::ProcessMessage(message, wParam, lParam);
            break;

        case WM_INPUT:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            Mouse::ProcessMessage(message, wParam, lParam);
            break;

        default:
            break;
        }
#endif
    }

    bool s_Buttons[2][GameInput::kNumDigitalInputs];
    float s_HoldDuration[GameInput::kNumDigitalInputs] = { 0.0f };
    float s_Analogs[GameInput::kNumAnalogInputs];
    float s_AnalogsTC[GameInput::kNumAnalogInputs];

#ifdef USE_KEYBOARD_MOUSE
    unsigned char s_DXKeyMapping[GameInput::kNumKeys]; // map DigitalInput enum to DX key codes 

    void KbmBuildKeyMapping()
    {
        s_DXKeyMapping[GameInput::kKey_escape] = Keyboard::Keys::Escape;
        s_DXKeyMapping[GameInput::kKey_1] = Keyboard::Keys::D1;
        s_DXKeyMapping[GameInput::kKey_2] = Keyboard::Keys::D2;
        s_DXKeyMapping[GameInput::kKey_3] = Keyboard::Keys::D3;
        s_DXKeyMapping[GameInput::kKey_4] = Keyboard::Keys::D4;
        s_DXKeyMapping[GameInput::kKey_5] = Keyboard::Keys::D5;
        s_DXKeyMapping[GameInput::kKey_6] = Keyboard::Keys::D6;
        s_DXKeyMapping[GameInput::kKey_7] = Keyboard::Keys::D7;
        s_DXKeyMapping[GameInput::kKey_8] = Keyboard::Keys::D8;
        s_DXKeyMapping[GameInput::kKey_9] = Keyboard::Keys::D9;
        s_DXKeyMapping[GameInput::kKey_0] = Keyboard::Keys::D0;
        s_DXKeyMapping[GameInput::kKey_minus] = Keyboard::Keys::OemMinus;
        s_DXKeyMapping[GameInput::kKey_equals] = 13;
        s_DXKeyMapping[GameInput::kKey_back] = Keyboard::Keys::Back;
        s_DXKeyMapping[GameInput::kKey_tab] = Keyboard::Keys::Tab;
        s_DXKeyMapping[GameInput::kKey_q] = Keyboard::Keys::Q;
        s_DXKeyMapping[GameInput::kKey_w] = Keyboard::Keys::W;
        s_DXKeyMapping[GameInput::kKey_e] = Keyboard::Keys::E;
        s_DXKeyMapping[GameInput::kKey_r] = Keyboard::Keys::R;
        s_DXKeyMapping[GameInput::kKey_t] = Keyboard::Keys::T;
        s_DXKeyMapping[GameInput::kKey_y] = Keyboard::Keys::Y;
        s_DXKeyMapping[GameInput::kKey_u] = Keyboard::Keys::U;
        s_DXKeyMapping[GameInput::kKey_i] = Keyboard::Keys::I;
        s_DXKeyMapping[GameInput::kKey_o] = Keyboard::Keys::O;
        s_DXKeyMapping[GameInput::kKey_p] = Keyboard::Keys::P;
        s_DXKeyMapping[GameInput::kKey_lbracket] = Keyboard::Keys::OemOpenBrackets;
        s_DXKeyMapping[GameInput::kKey_rbracket] = Keyboard::Keys::OemCloseBrackets;
        s_DXKeyMapping[GameInput::kKey_return] = Keyboard::Keys::Enter;
        s_DXKeyMapping[GameInput::kKey_lcontrol] = Keyboard::Keys::LeftControl;
        s_DXKeyMapping[GameInput::kKey_a] = Keyboard::Keys::A;
        s_DXKeyMapping[GameInput::kKey_s] = Keyboard::Keys::S;
        s_DXKeyMapping[GameInput::kKey_d] = Keyboard::Keys::D;
        s_DXKeyMapping[GameInput::kKey_f] = Keyboard::Keys::F;
        s_DXKeyMapping[GameInput::kKey_g] = Keyboard::Keys::G;
        s_DXKeyMapping[GameInput::kKey_h] = Keyboard::Keys::H;
        s_DXKeyMapping[GameInput::kKey_j] = Keyboard::Keys::J;
        s_DXKeyMapping[GameInput::kKey_k] = Keyboard::Keys::K;
        s_DXKeyMapping[GameInput::kKey_l] = Keyboard::Keys::L;
        s_DXKeyMapping[GameInput::kKey_semicolon] = Keyboard::Keys::OemSemicolon;
        s_DXKeyMapping[GameInput::kKey_apostrophe] = 40;
        s_DXKeyMapping[GameInput::kKey_grave] = 41;
        s_DXKeyMapping[GameInput::kKey_lshift] = Keyboard::Keys::LeftShift;
        s_DXKeyMapping[GameInput::kKey_backslash] = Keyboard::Keys::OemBackslash;
        s_DXKeyMapping[GameInput::kKey_z] = Keyboard::Keys::Z;
        s_DXKeyMapping[GameInput::kKey_x] = Keyboard::Keys::X;
        s_DXKeyMapping[GameInput::kKey_c] = Keyboard::Keys::C;
        s_DXKeyMapping[GameInput::kKey_v] = Keyboard::Keys::V;
        s_DXKeyMapping[GameInput::kKey_b] = Keyboard::Keys::B;
        s_DXKeyMapping[GameInput::kKey_n] = Keyboard::Keys::N;
        s_DXKeyMapping[GameInput::kKey_m] = Keyboard::Keys::M;
        s_DXKeyMapping[GameInput::kKey_comma] = Keyboard::Keys::OemComma;
        s_DXKeyMapping[GameInput::kKey_period] = Keyboard::Keys::OemPeriod;
        s_DXKeyMapping[GameInput::kKey_slash] = 53;
        s_DXKeyMapping[GameInput::kKey_rshift] = Keyboard::Keys::RightShift;
        s_DXKeyMapping[GameInput::kKey_multiply] = Keyboard::Keys::Multiply;
        s_DXKeyMapping[GameInput::kKey_lalt] = Keyboard::Keys::LeftAlt;
        s_DXKeyMapping[GameInput::kKey_space] = Keyboard::Keys::Space;
        s_DXKeyMapping[GameInput::kKey_capital] = Keyboard::Keys::CapsLock;
        s_DXKeyMapping[GameInput::kKey_f1] = Keyboard::Keys::F1;
        s_DXKeyMapping[GameInput::kKey_f2] = Keyboard::Keys::F2;
        s_DXKeyMapping[GameInput::kKey_f3] = Keyboard::Keys::F3;
        s_DXKeyMapping[GameInput::kKey_f4] = Keyboard::Keys::F4;
        s_DXKeyMapping[GameInput::kKey_f5] = Keyboard::Keys::F5;
        s_DXKeyMapping[GameInput::kKey_f6] = Keyboard::Keys::F6;
        s_DXKeyMapping[GameInput::kKey_f7] = Keyboard::Keys::F7;
        s_DXKeyMapping[GameInput::kKey_f8] = Keyboard::Keys::F8;
        s_DXKeyMapping[GameInput::kKey_f9] = Keyboard::Keys::F9;
        s_DXKeyMapping[GameInput::kKey_f10] = Keyboard::Keys::F10;
        s_DXKeyMapping[GameInput::kKey_numlock] = Keyboard::Keys::NumLock;
        s_DXKeyMapping[GameInput::kKey_scroll] = Keyboard::Keys::Scroll;
        s_DXKeyMapping[GameInput::kKey_numpad7] = Keyboard::Keys::NumPad7;
        s_DXKeyMapping[GameInput::kKey_numpad8] = Keyboard::Keys::NumPad8;
        s_DXKeyMapping[GameInput::kKey_numpad9] = Keyboard::Keys::NumPad9;
        s_DXKeyMapping[GameInput::kKey_subtract] = Keyboard::Keys::Subtract;
        s_DXKeyMapping[GameInput::kKey_numpad4] = Keyboard::Keys::NumPad4;
        s_DXKeyMapping[GameInput::kKey_numpad5] = Keyboard::Keys::NumPad5;
        s_DXKeyMapping[GameInput::kKey_numpad6] = Keyboard::Keys::NumPad6;
        s_DXKeyMapping[GameInput::kKey_add] = Keyboard::Keys::OemPlus;
        s_DXKeyMapping[GameInput::kKey_numpad1] = Keyboard::Keys::NumPad1;
        s_DXKeyMapping[GameInput::kKey_numpad2] = Keyboard::Keys::NumPad2;
        s_DXKeyMapping[GameInput::kKey_numpad3] = Keyboard::Keys::NumPad3;
        s_DXKeyMapping[GameInput::kKey_numpad0] = Keyboard::Keys::NumPad0;
        s_DXKeyMapping[GameInput::kKey_decimal] = Keyboard::Keys::OemPeriod;
        s_DXKeyMapping[GameInput::kKey_f11] = Keyboard::Keys::F11;
        s_DXKeyMapping[GameInput::kKey_f12] = Keyboard::Keys::F12;
        s_DXKeyMapping[GameInput::kKey_numpadenter] = Keyboard::Keys::Enter; // NumPad variant?
        s_DXKeyMapping[GameInput::kKey_rcontrol] = Keyboard::Keys::RightControl;
        s_DXKeyMapping[GameInput::kKey_divide] = Keyboard::Keys::Divide;
        s_DXKeyMapping[GameInput::kKey_sysrq] = 183;
        s_DXKeyMapping[GameInput::kKey_ralt] = Keyboard::Keys::RightAlt;
        s_DXKeyMapping[GameInput::kKey_pause] = Keyboard::Keys::Pause;
        s_DXKeyMapping[GameInput::kKey_home] = Keyboard::Keys::Home;
        s_DXKeyMapping[GameInput::kKey_up] = Keyboard::Keys::Up;
        s_DXKeyMapping[GameInput::kKey_pgup] = Keyboard::Keys::PageUp;
        s_DXKeyMapping[GameInput::kKey_left] = Keyboard::Keys::Left;
        s_DXKeyMapping[GameInput::kKey_right] = Keyboard::Keys::Right;
        s_DXKeyMapping[GameInput::kKey_end] = Keyboard::Keys::End;
        s_DXKeyMapping[GameInput::kKey_down] = Keyboard::Keys::Down;
        s_DXKeyMapping[GameInput::kKey_pgdn] = Keyboard::Keys::PageDown;
        s_DXKeyMapping[GameInput::kKey_insert] = Keyboard::Keys::Insert;
        s_DXKeyMapping[GameInput::kKey_delete] = Keyboard::Keys::Delete;
        s_DXKeyMapping[GameInput::kKey_lwin] = Keyboard::Keys::LeftWindows;
        s_DXKeyMapping[GameInput::kKey_rwin] = Keyboard::Keys::RightWindows;
        s_DXKeyMapping[GameInput::kKey_apps] = Keyboard::Keys::Apps;
    }

    void KbmZeroInputs()
    {
        for (uint32_t i = 0; i < kNumKeys; ++i)
        {
            s_Buttons[0][i] = false;
        }

        s_Buttons[0][kMouseLeft]    = false;
        s_Buttons[0][kMouseMiddle]  = false;
        s_Buttons[0][kMouseRight]   = false;
        s_Buttons[0][kMouseButton1] = false;
        s_Buttons[0][kMouseButton2] = false;
        s_Analogs[kAnalogMouseX] = 0.0f;
        s_Analogs[kAnalogMouseY] = 0.0f;
        s_Analogs[kAnalogMouseScroll] = 0.0f;
    }

    void KbmInitialize()
    {
        KbmBuildKeyMapping();

        g_Keyboard = std::make_unique<DirectX::Keyboard>();
        g_Mouse = std::make_unique<DirectX::Mouse>();

        g_Mouse->SetWindow(GameCore::g_hWnd);
        g_Mouse->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
        ASSERT(g_Mouse->IsConnected());

        KbmZeroInputs();
    }

    void KbmUpdate()
    {
        HWND foreground = GetForegroundWindow();
        bool visible = IsWindowVisible(foreground) != 0;

        if (foreground != GameCore::g_hWnd || !visible)
        {
            KbmZeroInputs();
        }
        else
        {
            auto kbState = g_Keyboard->GetState();

            for (uint32_t i = 0; i < kNumKeys; ++i)
            {
                s_Buttons[0][i] = kbState.IsKeyDown((Keyboard::Keys)s_DXKeyMapping[i]);
            }

            auto mState = g_Mouse->GetState();

            s_Buttons[0][kMouseLeft]    = mState.leftButton;
            s_Buttons[0][kMouseMiddle]  = mState.middleButton;
            s_Buttons[0][kMouseRight]   = mState.rightButton;
            s_Buttons[0][kMouseButton1] = mState.xButton1;
            s_Buttons[0][kMouseButton2] = mState.xButton2;

            s_Analogs[kAnalogMouseX] = mState.x / 512.0f;
            s_Analogs[kAnalogMouseY] = -mState.y / 512.0f;

            if (mState.scrollWheelValue > 0)
                s_Analogs[kAnalogMouseScroll] = 1.0f;
            else if (mState.scrollWheelValue < 0)
                Utility::Printf("Scroll down\n");
            else
                s_Analogs[kAnalogMouseScroll] = 0.0f;

            Mouse::Get().ResetScrollWheelValue();
        }
    }

#endif

}

void GameInput::Initialize()
{
    ZeroMemory(s_Buttons, sizeof(s_Buttons) );
    ZeroMemory(s_Analogs, sizeof(s_Analogs) );

    g_GamePad = std::make_unique<DirectX::GamePad>();

#ifdef USE_KEYBOARD_MOUSE
    KbmInitialize();
#endif
}

void GameInput::Shutdown()
{
    g_GamePad.reset();

#ifdef USE_KEYBOARD_MOUSE
    g_Keyboard.reset();
    g_Mouse.reset();
#endif
}

void GameInput::Update( float frameDelta )
{
    memcpy(s_Buttons[1], s_Buttons[0], sizeof(s_Buttons[0]));
    memset(s_Buttons[0], 0, sizeof(s_Buttons[0]));
    memset(s_Analogs, 0, sizeof(s_Analogs));

    auto state = g_GamePad->GetState(0);

    if (state.IsConnected())
    {
        s_Buttons[0][kDPadUp] = state.dpad.up;
        s_Buttons[0][kDPadDown] = state.dpad.down;
        s_Buttons[0][kDPadLeft] = state.dpad.left;
        s_Buttons[0][kDPadRight] = state.dpad.right;
        s_Buttons[0][kStartButton] = state.buttons.start;
        s_Buttons[0][kBackButton] = state.buttons.back;
        s_Buttons[0][kLThumbClick] = state.buttons.leftStick;
        s_Buttons[0][kRThumbClick] = state.buttons.rightStick;
        s_Buttons[0][kLShoulder] = state.buttons.leftShoulder;
        s_Buttons[0][kRShoulder] = state.buttons.rightShoulder;
        s_Buttons[0][kAButton] = state.buttons.a;
        s_Buttons[0][kBButton] = state.buttons.b;
        s_Buttons[0][kXButton] = state.buttons.x;
        s_Buttons[0][kYButton] = state.buttons.y;

        s_Analogs[kAnalogLeftTrigger]   = state.triggers.left;
        s_Analogs[kAnalogRightTrigger]  = state.triggers.right;
        s_Analogs[kAnalogLeftStickX]    = state.thumbSticks.leftX;
        s_Analogs[kAnalogLeftStickY]    = state.thumbSticks.leftY;
        s_Analogs[kAnalogRightStickX]   = state.thumbSticks.rightX;
        s_Analogs[kAnalogRightStickY]   = state.thumbSticks.rightY;
    }

#ifdef USE_KEYBOARD_MOUSE
    KbmUpdate();
#endif

    // Update time duration for buttons pressed
    for (uint32_t i = 0; i < kNumDigitalInputs; ++i)
    {
        if (s_Buttons[0][i])
        {
            if (!s_Buttons[1][i])
                s_HoldDuration[i] = 0.0f;
            else
                s_HoldDuration[i] += frameDelta;
        }
    }

    for (uint32_t i = 0; i < kNumAnalogInputs; ++i)
    {
        s_AnalogsTC[i] = s_Analogs[i] * frameDelta;
    }

}

bool GameInput::IsAnyPressed( void )
{
    return s_Buttons[0] != 0;
}

bool GameInput::IsPressed( DigitalInput di )
{
    return s_Buttons[0][di];
}

bool GameInput::IsFirstPressed( DigitalInput di )
{
    return s_Buttons[0][di] && !s_Buttons[1][di];
}

bool GameInput::IsReleased( DigitalInput di )
{
    return !s_Buttons[0][di];
}

bool GameInput::IsFirstReleased( DigitalInput di )
{
    return !s_Buttons[0][di] && s_Buttons[1][di];
}

float GameInput::GetDurationPressed( DigitalInput di )
{
    return s_HoldDuration[di];
}

float GameInput::GetAnalogInput( AnalogInput ai )
{
    return s_Analogs[ai];
}

float GameInput::GetTimeCorrectedAnalogInput( AnalogInput ai )
{
    return s_AnalogsTC[ai];
}
