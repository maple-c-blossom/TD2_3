#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


#pragma region �Q�[���n.h include

#include "Player.h"
#include "Boss.h"
#include "Cleaner.h"

#pragma endregion �Q�[���n.h include


namespace MCB
{
	class Scene :public IScene
	{
	private:

		enum class Result
		{
			Clear,
			GameOver,
			Frame,
			Title,
			Space,
			ABottom,
			Time,
			Rank,
			ARank,
			BRank,
			CRank,
			SRank
		};
		#pragma region �ϊ��s��
		//�ϊ��s��
		View matView;
		Projection matProjection;
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
		std::unique_ptr<Model> BossCover;
		std::unique_ptr<Model> creaner;

		//FBXModel* testModel;
#pragma endregion 3D���f��

		//�e�N�X�`��
		#pragma region �e�N�X�`��
		TextureCell* playerHp;
		TextureCell* debugTextTexture;
		TextureCell* bossHp;
		TextureCell* shard;
		std::array<MCB::TextureCell*, 8> tutorialTexs;
		std::array<MCB::TextureCell*, 12> result;
		std::array< std::unique_ptr<MCB::Sprite>, 12> resultSprite;
#pragma endregion �e�N�X�`��

		//�T�E���h
		#pragma region �T�E���h
		int bgm;
		int clearbgm;
		int gameOverbgm;
		int selectSound;
		int volume = 25;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

		#pragma region 3D�I�u�W�F�N�g
		SimpleFigure triangle;

		Object3d ground;
		Object3d Skydome;
		Object3d testSpher;

#pragma endregion 3D�I�u�W�F�N�g

		#pragma region �X�v���C�g
		Sprite playerHpSprite;

		Sprite bossHpSprite;

		Sprite shardSprite;

		DebugText debugText;

		std::vector<std::unique_ptr<Enemy,MyDeleter<Enemy>>>enemys;

		std::unique_ptr <Player> substie = std::make_unique<Player>();

		std::unique_ptr <Boss> boss = std::make_unique<Boss>();

		std::vector<Cleaner> walls;

		Timer spownTimer;
#pragma endregion �X�v���C�g
#pragma region �p�[�e�B�N��		

#pragma endregion
		#pragma region �ʏ�ϐ�
		int time = 0;
		float resultSize = 1.f;
		bool soundPlayed = false;
		Timer resultSizeTimer;
		bool loopFlag = true;
		bool startPositionReset = true;
		bool ybill = false;
		DebugCamera debugCamera;
		Camera mainCamera;
#pragma endregion �ʏ�ϐ�

	public:
		Scene(RootParameter* root,Depth* depth,PipeLineManager* pipeline);
		~Scene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		void ParticleInit()  override;
		std::unique_ptr<IScene> GetNextScene() override;
		//---------------
		void MatrixUpdate()override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw()override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;


		void PostEffectDraw()override;
	};
}