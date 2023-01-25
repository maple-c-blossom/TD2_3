#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"


using namespace MCB;
using namespace DirectX;

using namespace std;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
    delete nextScene;
    delete testModel;
    delete handwrModel;
    //loader->SetDelete(testTex);
    //loader->SetDelete(debugTextTexture);
    //loader->SetDelete(zoomTex);
    //loader->SetDelete(scopeTex);
    testTex->free = true;
    debugTextTexture->free = true;
    zoomTex->free = true;
    scopeTex->free = true;

}

#pragma region 通常変数の初期化と3Dオブジェクトの初期化
void MCB::Scene::Initialize()
{
    debugCamera.Inilialize();
    mainCamera.Inilialize();
    viewCamera = &mainCamera;
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    ParticleInit();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    lights->DefaultLightSet();
    lights->UpDate();
    Object3d::SetLights(lights);

}

void MCB::Scene::Object3DInit()
{

    ground;
    ground.Init();
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,0,0 };
    ;
    Skydome;
    Skydome.Init();
    Skydome.model = skydomeModel;
    Skydome.scale = { 4,4,4 };

    testSpher.Init();
    testSpher.model = BoxModel;
    testSpher.fbxModel = testModel;
    testSpher.scale = {1,1,1};
    testSpher.position = { 0,4,10 };
    testSpher.rotation = { ConvertRadius(90),0,0 };

    substie.model = playerModel.get();
    substie.KneadedEraserModel = nerikesiModel.get();
    substie.scale = { 1,1,1 };
    substie.position = { 0,0,-35 };
    substie.Initialize();

    Object3d wall = Object3d();
    wall.scale = { 10,10,10 };
    wall.Init();
    wall.model = BoxModel;
    walls.push_back(wall);
    walls.back().colliders.push_back(ADXCollider(&walls.back()));
    walls.back().colliders.back().colType_ = box;

    unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
    temp->Enemy::Initialize({ 0,0,1 }, { 20,0,40 }, pencilEnemyModel.get(), 0.5f);
    temp->movePoint = { {20,0,40},{ -20,0,20 },{ -20,0,40 } };
    temp->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp));

    temp = make_unique<PencilEnemy>();
    temp->Enemy::Initialize({ 1,0,0 }, { -20,0,20 }, pencilEnemyModel.get(), 0.5f);
    temp->movePoint = { {-20,0,20},{ 20,0,40 },{ 20,0,20 } };
    temp->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp));
    spownTimer.Set(30);
    boss.Initialize({ 0,0,1 }, { 0,0,0 }, bossModel.get(), pencilEnemyModel.get(), WritingModel.get(), 1, &substie);
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{
	BoxModel = new Model("Box");

	groundModel = new Model("ground");

	skydomeModel = new Model("skydome");
    handwrModel = new Model("Box");

    playerModel = std::make_unique<Model>("player");
    pencilEnemyModel = std::make_unique<Model>("pencil");
    WritingModel = std::make_unique<Model>("Box");
    bossModel = std::make_unique<Model>("boss");
    nerikesiModel = std::make_unique<Model>("nerikeshi");
    //testModel = new FBXModel();
    //testModel->Load("testFbx");
    //assert(testModel->textureManager->textures.size() < 20);
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
	testTex = loader->LoadTexture(L"Resources\\reimu.png");
	debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    zoomTex = loader->LoadTexture(L"Resources\\reticle.png");
    scopeTex = loader->CreateNoTextureFileIsTexture();

}

void MCB::Scene::LoadSound()
{
	testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::Scene::SpriteInit()
{
    sprite.InitMatProje();
    sprite = sprite.CreateSprite();
    zoomSprite.InitMatProje();
    zoomSprite = zoomSprite.CreateSprite();
    scopeSprite.InitMatProje();
    scopeSprite = scopeSprite.CreateSprite();
    debugText.Init(debugTextTexture->texture.get());

}

void MCB::Scene::ParticleInit()
{
    testParticle.Init(testTex);
    testParticle.position = { 0,0,10 };
    //testParticle.rotation.x = ConvertRadius(-90);
}

IScene* MCB::Scene::GetNextScene()
{
    return new TitleScene(rootparamsPtr,depth,pipeline);
}

#pragma endregion 各種リソースの読み込みと初期化

void MCB::Scene::Update()
{
    Enemy::StaticUpdate();
    KneadedEraser::StaticUpdate();
    WritingEnemy::StaticUpdate();
//        if (input->IsKeyTrigger(DIK_SPACE))
//        {
//            sceneEnd = true;
//        }

        substie.Update();
        spownTimer.Update();

        if (spownTimer.IsEnd() && enemys.size() < 6)
        {
            switch (GetRand(1,3))
            {
            case 1:
            {
                unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
                temp->SetHandwritingModel(WritingModel.get());
                temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                enemys.push_back(move(temp));
            }
            break;
            case 2:
            {
                unique_ptr<EraserEnemy> temp = make_unique<EraserEnemy>();
                temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.25f);
                temp->handwritingModel = WritingModel.get();
                enemys.push_back(move(temp));
            }
                break;
            default:
            {
                unique_ptr<EraserEnemy> temp = make_unique<EraserEnemy>();
                temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.25f);
                temp->handwritingModel = WritingModel.get();
                enemys.push_back(move(temp));
            }
                break;
            }


            spownTimer.Set(60);
        }

        for (auto& itr : enemys)
        {
            itr->Update();
        }

        boss.Update();

        lights->UpDate();
        viewCamera->Update();

    CheckAllColision();
    enemys.remove_if([](auto& itr)
        {
            return !Object3d::IsValid(itr.get());
        });
    //行列変換
    MatrixUpdate();

    if (boss.GetHp() <= 0 || substie.GetHp() <= 0)
    {
        sceneEnd = true;
    }
}

void MCB::Scene::Draw()
{
    //3Dオブジェクト
    Skydome.Draw();
    ground.Draw();
    if (substie.GetVisible())
    {
        substie.Draw();
    }
    for (auto& itr : walls)
    {
        itr.Draw();
    }
    for (auto& itr : enemys)
    {
        itr->Draw();
    }
    boss.Draw();
    WritingEnemy::StaticDraw();
    //human.Draw();
    //testSpher.Draw();
    //testSpher.FbxDraw();
}

void MCB::Scene::SpriteDraw()
{
    debugText.Print(20, 20, 2, "boss:hp %d",boss.GetHp());
    debugText.Print(20, 60, 2, "player:hp %d",substie.GetHp());
    debugText.Print(20, 100, 2, "player:shard %f", substie.GetShard());
    debugText.Print(dxWindow->window_width - 300, 20, 2, "Move:WASD");
    debugText.Print(dxWindow->window_width - 300, 60, 2, "Action:SPACE", substie.GetShard());
    debugText.Print(dxWindow->window_width - 300, 100, 2, "ActionCost:shard");
    if (boss.GetHp() <= 0)
    {
        debugText.Print(dxWindow->window_width/2 - 100, dxWindow->window_height/2, 4, "GameClear");
    }
    else if (substie.GetHp() <= 0)
    {
        debugText.Print(dxWindow->window_width / 2 - 100, dxWindow->window_height / 2, 4, "GameOver");
    }

    debugText.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    testParticle.Draw();
}

void MCB::Scene::CheckAllColision()
{






}

void MCB::Scene::ImGuiUpdate()
{
    imgui.Begin();
    ImGui::ShowDemoWindow();
    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView(ybill);
    viewCamera->MatrixUpdate();
    Skydome.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    //testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    testParticle.Update(*viewCamera->GetView(), *viewCamera->GetProjection(), true);
    substie.UpdateMatrix(viewCamera);
    for (auto& itr : enemys)
    {
        itr->UpdateMatrix(viewCamera);
    }
    for (auto& itr : walls)
    {
        itr.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
        for (auto& colItr : itr.colliders)
        {
            colItr.Update(&itr);
        }
    }
    boss.UpdateMatrix(viewCamera);
    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr,PipeLineManager* pipeline)
{
	rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}

