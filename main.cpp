#include "include.h"
#include "SceneMgr.h"
#include "Share.h"


//定数
//ウィンドウの初期位置
#define WIN_POS_X 50
#define WIN_POS_Y 50

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウモードに設定
    ChangeWindowMode(TRUE);
    // DXライブラリ初期化処理

    //描画先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);
    //画面等の設定
    SetWindowText("OTELLO");
    SetGraphMode(WIN_SIZE_X , WIN_SIZE_Y , 32);
    SetWindowInitPosition(WIN_POS_X , WIN_POS_Y);
    SetBackgroundColor(255 , 255 , 255);
    ChangeFontType(DX_FONTTYPE_ANTIALIASING);
    ChangeFont("Meiryo UI");

    //xボタンで終了しない
    SetWindowUserCloseEnableFlag(false);
    SetWaitVSyncFlag(0);

    if (DxLib_Init() < 0) {
        //エラーなら終了する
        return -1;
    }

    SceneMgr sceneMgr;//シーン管理クラス

    //初期化
    sceneMgr.Initialize();
    Share::Share_Initialize();

    // while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        sceneMgr.Update();
        sceneMgr.Draw();
        if (sceneMgr.fin) break;
    }
    //終了処理
    sceneMgr.Finalize();
    DxLib_End();
    return 0;
}