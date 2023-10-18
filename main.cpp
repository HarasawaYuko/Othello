#include "include.h"
#include "SceneOthello.h"
#include "SceneMgr.h"
#include "Share.h"


/////定数
//ウィンドウの初期位置
#define WIN_POS_X 0
#define WIN_POS_Y 0

namespace {
    static int scene = Scene_Othello;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウモードに設定
    ChangeWindowMode(TRUE);
    // DXライブラリ初期化処理
    if (DxLib_Init() < 0) {
        //エラーなら終了する
        return -1;
    }
    //描画先を裏画面にする
    SetWindowText("GAME APPLICATION");
    SetDrawScreen(DX_SCREEN_BACK);
    SetGraphMode(WIN_SIZE_X , WIN_SIZE_Y , 32);
    SetWindowInitPosition(WIN_POS_X , WIN_POS_Y);
    SetBackgroundColor(255 , 255 , 255);
    ChangeFontType(DX_FONTTYPE_ANTIALIASING);
    ChangeFont("Meiryo UI");

    //xボタンで終了しない
    SetWindowUserCloseEnableFlag(false);

    SetWaitVSyncFlag(0);

    //初期化
    SceneMgr sceneMgr;
    sceneMgr.Initialize();
    Share::Share_Initialize();

    // while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        sceneMgr.Update();
        sceneMgr.Draw();
        if (sceneMgr.fin) break;
    }
    sceneMgr.Finalize();
    DxLib_End();    // DXライブラリ終了処理
    return 0;
}