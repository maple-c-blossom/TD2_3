#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


#pragma region ゲーム系.h include

#include "Player.h"
#include "Boss.h"

#pragma endregion ゲーム系.h include


namespace MCB
{
	class Scene :public IScene
	{
	private:
		#pragma region 変換行列
		//変換行列
		View matView;
		Projection matProjection;
#pragma endregion 変換行列
		
		#pragma region 各種リソース
		//3Dモデル
		#pragma region 3Dモデル
		Model* BoxModel;
		Model* groundModel;
		Model* skydomeModel;
		Model* handwrModel;

		std::unique_ptr<Model> playerModel;
		std::unique_ptr<Model> pencilEnemyModel;
		std::unique_ptr<Model> WritingModel;
		std::unique_ptr<Model> bossModel;
		std::unique_ptr<Model> nerikesiModel;
		std::unique_ptr<Model> eraseEnemyModel;

		FBXModel* testModel;
#pragma endregion 3Dモデル

		//テクスチャ
		#pragma region テクスチャ
		TextureCell* testTex;
		TextureCell* debugTextTexture;
		TextureCell* zoomTex;
		TextureCell* scopeTex;
		std::array<MCB::TextureCell*, 6> tutorialTexs;
#pragma endregion テクスチャ

		//サウンド
		#pragma region サウンド
		int testSound;
		int test2Sound;

		int volume = 255;
#pragma endregion サウンド

#pragma endregion 各種リソース

		#pragma region 3Dオブジェクト
		SimpleFigure triangle;

		Object3d ground;
		Object3d Skydome;
		Object3d testSpher;

#pragma endregion 3Dオブジェクト

		#pragma region スプライト
		Sprite sprite;

		Sprite zoomSprite;

		Sprite scopeSprite;

		DebugText debugText;

		std::list<std::unique_ptr<Enemy>>enemys;

		Player substie;

		Boss boss;

		std::list<Object3d> walls;

		Timer spownTimer;
#pragma endregion スプライト
#pragma region パーティクル
		Particle testParticle;
		

#pragma endregion
		#pragma region 通常変数
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
		IScene* GetNextScene() override;
		//---------------
		void MatrixUpdate()override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw()override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}