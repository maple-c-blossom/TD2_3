#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"

namespace MCB
{
	class TitleScene :public IScene
	{
	private:
#pragma region 変換行列
		//変換行列
		View matView;
		Projection matProjection;
		Camera camera;
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
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* title;
		TextureCell* debugTextTexture;
		TextureCell* pushSpace;
		TextureCell* scopeTex;
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
		std::unique_ptr <Player> substie = std::make_unique<Player>();
		

#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite titleSprite;

		Sprite pushSpaceSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion スプライト

#pragma region 通常変数
		bool loopFlag = true;
		bool startPositionReset = true;
#pragma endregion 通常変数
	public:

		TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~TitleScene();
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
		void MatrixUpdate() override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw() override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}

