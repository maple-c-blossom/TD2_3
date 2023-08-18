#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


#pragma region ゲーム系.h include

#include "Player.h"
#include "Boss.h"
#include "Cleaner.h"

#pragma endregion ゲーム系.h include


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
		#pragma region 変換行列
		//変換行列
		View matView;
		Projection matProjection;
#pragma endregion 変換行列
		
		#pragma region 各種リソース
		//3Dモデル
		#pragma region 3Dモデル
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
#pragma endregion 3Dモデル

		//テクスチャ
		#pragma region テクスチャ
		TextureCell* playerHp;
		TextureCell* debugTextTexture;
		TextureCell* bossHp;
		TextureCell* shard;
		std::array<MCB::TextureCell*, 8> tutorialTexs;
		std::array<MCB::TextureCell*, 12> result;
		std::array< std::unique_ptr<MCB::Sprite>, 12> resultSprite;
#pragma endregion テクスチャ

		//サウンド
		#pragma region サウンド
		int bgm;
		int clearbgm;
		int gameOverbgm;
		int selectSound;
		int volume = 25;
#pragma endregion サウンド

#pragma endregion 各種リソース

		#pragma region 3Dオブジェクト
		SimpleFigure triangle;

		Object3d ground;
		Object3d Skydome;
		Object3d testSpher;

#pragma endregion 3Dオブジェクト

		#pragma region スプライト
		Sprite playerHpSprite;

		Sprite bossHpSprite;

		Sprite shardSprite;

		DebugText debugText;

		std::vector<std::unique_ptr<Enemy,MyDeleter<Enemy>>>enemys;

		std::unique_ptr <Player> substie = std::make_unique<Player>();

		std::unique_ptr <Boss> boss = std::make_unique<Boss>();

		std::vector<Cleaner> walls;

		Timer spownTimer;
#pragma endregion スプライト
#pragma region パーティクル		

#pragma endregion
		#pragma region 通常変数
		int time = 0;
		float resultSize = 1.f;
		bool soundPlayed = false;
		Timer resultSizeTimer;
		bool loopFlag = true;
		bool startPositionReset = true;
		bool ybill = false;
		DebugCamera debugCamera;
		Camera mainCamera;
#pragma endregion 通常変数

	public:
		Scene(RootParameter* root,Depth* depth,PipeLineManager* pipeline);
		~Scene();
		void Initialize() override;

		//各初期化系関数群--------------------
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