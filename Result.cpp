#include "Result.h"
#include "Share.h"

/**�萔**/
//���փ{�^��
static const int NEXT_X = 100;
static const int NEXT_Y = 100;
static const int NEXT_WIDTH = 200;
static const int NEXT_HEIGHT = 200;
//������x�{�^��
static const int RETRY_X = 300;
static const int RETRY_Y = 300;
static const int RETRY_WIDTH = 200;
static const int RETRY_HEIGHT = 200;

/**�`��p�ϐ�**/
static bool onNext;
static bool onRetry;

/**��������p**/
static bool nowNext;
static bool nowRetry;

//�R���X�g���N�^
Result::Result(SceneChanger *changer) 
	:BaseScene(changer)
{}

//�f�X�X�g���N�^
Result::~Result() {
}

void Result::Initialize() {
	//�摜�̃��[�h
	m_resultPic = LoadGraph("pic/Othello/result.png");
	//�����̃��[�h
	m_resultSnd = LoadSoundMem("sound/Othello/result.mp3");
	m_nextSnd = LoadSoundMem("sound/Othello/next.mp3");
	m_retrySnd = LoadSoundMem("sound/Othello/start.mp3");

	//BGM���Đ�
	PlaySoundMem(m_resultSnd , DX_PLAYTYPE_LOOP , true);
}

void Result::Update() {
	//�t���O�̃��Z�b�g
	onNext = false;
	onRetry = false;
	//�}�E�X�ʒu�̎擾
	int mousePosX;
	int mousePosY;
	int mouseInput = GetMouseInput();
	GetMousePoint(&mousePosX , &mousePosY);


	//���փ{�^��
	if (Share::isIn(NEXT_X , NEXT_Y , NEXT_WIDTH , NEXT_HEIGHT , mousePosX , mousePosY)) {
		onNext = true;
		//�N���b�N���ꂽ��
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Menu);
		}
	}

	//������x�{�^��
	if (Share::isIn(RETRY_X, RETRY_Y, RETRY_WIDTH, RETRY_HEIGHT, mousePosX, mousePosY)) {
		onRetry = true;
		//�N���b�N���ꂽ��
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Game);
		}
	}
}

void Result::Draw() {
	//�w�i�̕\��
	DrawExtendGraph(0 , 0 ,WIN_SIZE_X ,WIN_SIZE_Y , m_resultPic , false);
}

void Result::Finalize() {
	deleteMem();
}

void Result::deleteMem() {
	DeleteGraph(m_resultPic);
	DeleteSoundMem(m_resultSnd);
	DeleteSoundMem(m_nextSnd);
	DeleteSoundMem(m_retrySnd);
}