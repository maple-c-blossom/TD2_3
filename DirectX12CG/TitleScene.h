#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include "Status.h"
#include "Boss.h"
#include "PencilEnemy.h"
namespace MCB
{
	class TitleScene :public IScene
	{
	private:
#pragma region �ϊ��s��
		//�ϊ��s��
		View matView;
		Projection matProjection;
		Camera camera;
#pragma endregion �ϊ��s��

#pragma region �e�탊�\�[�X
		//3D���f��
#pragma region 3D���f��
		std::unique_ptr<Model> BoxModel;
		std::unique_ptr<Model> groundModel;
		std::unique_ptr<Model> skydomeModel;

		std::unique_ptr<Model> playerModel;
		std::unique_ptr<Model> pencilEnemyModel;
		std::unique_ptr<Model> WritingModel;
		std::unique_ptr<Model> bossModel;
		std::unique_ptr<Model> nerikesiModel;
		std::unique_ptr<Model> eraseEnemyModel;
		std::unique_ptr<Model> BossDamegeEffectModelStar;
		std::unique_ptr<Model> BossDamegeEffectModelSpher;
		std::unique_ptr<Model> tutorialBlackBode;
#pragma endregion 3D���f��

		//�e�N�X�`��
#pragma region �e�N�X�`��
		TextureCell* title;
		TextureCell* debugTextTexture;
		TextureCell* pushSpace;
		TextureCell* scopeTex;
#pragma endregion �e�N�X�`��

		//�T�E���h
#pragma region �T�E���h
		int testSound;
		int test2Sound;

		int volume = 255;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

#pragma region 3D�I�u�W�F�N�g
		SimpleFigure triangle;
		Object3d ground;
		Object3d Skydome;
		Object3d testSpher;
		//Object3d tutorialBode;
		std::list<std::unique_ptr<Handwriting>> writing;
		int i = 0;
		std::array<Object3d, 8> tutorialBode;
		std::unique_ptr <Player> substie = std::make_unique<Player>();
		std::unique_ptr <Boss> boss = std::make_unique<Boss>();
		unsigned short tutorialSucces = 0b00000;
		float velocitySum;
		float RotateFrame;
#pragma endregion 3D�I�u�W�F�N�g

#pragma region �X�v���C�g
		Sprite titleSprite;

		Sprite pushSpaceSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion �X�v���C�g

#pragma region �ʏ�ϐ�
		bool loopFlag = true;
		bool startPositionReset = true;
#pragma endregion �ʏ�ϐ�
	public:

		TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~TitleScene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		void ParticleInit()  override;
		IScene* GetNextScene() override;
		//---------------
		void MatrixUpdate() override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw() override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}

