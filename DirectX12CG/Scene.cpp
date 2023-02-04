#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"


using namespace MCB;
using namespace DirectX;

using namespace std;

MCB::Scene::~Scene()
{
    soundManager.AllDeleteSound();
    WritingEnemy::GetHandWrite()->clear();
    Player::GetCaptureList()->clear();

    delete nextScene;
    delete testModel;
    //loader->SetDelete(testTex);
    //loader->SetDelete(debugTextTexture);
    //loader->SetDelete(zoomTex);
    //loader->SetDelete(scopeTex);
    shard->free = true;
    debugTextTexture->free = true;
    playerHp->free = true;
    bossHp->free = true;
    for (auto& itr : tutorialTexs)
    {
        itr->free = true;
    }

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
    ground.model = groundModel.get();
    ground.scale = { 36,50,43 };
    ground.position = { 0,-12,0 };
    ;
    Skydome;
    Skydome.Init();
    Skydome.model = skydomeModel.get();
    Skydome.scale = { 180,1,180 };
    Skydome.position = { 0,-15,0 };

    substie->model = playerModel.get();
    substie->KneadedEraserModel = nerikesiModel.get();
    substie->scale = { 1,1,1 };
    substie->position = { 0,0,0 };
    substie->Initialize();

    Object3d wall = Object3d();
    wall.position = { 20,9,-14 };
    wall.scale = { 5.8,10,1.5 };
    wall.rotation = { 0,0,0 };
    wall.Init();
    wall.model = creaner.get();
    walls.push_back(wall);
    walls.back().colliders.push_back(ADXCollider(&walls.back()));
    walls.back().colliders.back().colType_ = box;

    unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
    temp->Enemy::Initialize({ 0,0,1 }, { 20,0,10 }, pencilEnemyModel.get(), 0.5f);
    temp->movePoint = { {20,0,10},{ 20,0,20 },{ -20,0,10 } };
    temp->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp));

    temp = make_unique<PencilEnemy>();
    temp->Enemy::Initialize({ 1,0,0 }, { -20,0,-10 }, pencilEnemyModel.get(), 0.5f);
    temp->movePoint = { {-20,0,-10},{ -20,0,-20 },{ 20,0,-10 } };
    temp->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp));
    spownTimer.Set(30);
    boss->Initialize({ 0,0,1 }, { -20,0,0 }, bossModel.get(), pencilEnemyModel.get(), WritingModel.get(), BossDamegeEffectModelStar.get(), BossDamegeEffectModelSpher.get(), BossCover.get(), 1, substie.get());
    boss->shake = mainCamera.GetShakePtr();
    mainCamera.player = substie.get();
    
    
    
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}

#pragma endregion 通常変数の初期化

#pragma region 各種リソースの読み込みと初期化
void MCB::Scene::LoadModel()
{
	BoxModel = std::make_unique<Model>("Box");

	groundModel = std::make_unique<Model>("note");

	skydomeModel = std::make_unique<Model>("table");

    playerModel = std::make_unique<Model>("player");
    pencilEnemyModel = std::make_unique<Model>("pencil");
    WritingModel = std::make_unique<Model>("Box");
    bossModel = std::make_unique<Model>("boss");
    nerikesiModel = std::make_unique<Model>("nerikeshi");
    creaner = std::make_unique<Model>("cleaner");
    eraseEnemyModel = std::make_unique<Model>("eraser");
    BossDamegeEffectModelStar = std::make_unique<Model>("star");
    BossDamegeEffectModelSpher = std::make_unique<Model>("ball");
    BossCover = std::make_unique<Model>("bossCover");
    //BossCover->texture = loader->LoadTexture(L"Resources\\text\\bossCover\\bossCover.png");
    //testModel = new FBXModel();
    //testModel->Load("testFbx");
    //assert(testModel->textureManager->textures.size() < 20);
    //fbxLoader->LoadModelFromFile("cube");
}

void MCB::Scene::LoadTexture()
{
	playerHp = loader->LoadTexture(L"Resources\\text\\playerHp.png");
	debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    bossHp = loader->LoadTexture(L"Resources\\text\\bossHp.png");
    shard = loader->LoadTexture(L"Resources\\text\\shard.png");
    tutorialTexs[0] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_move.png");
    tutorialTexs[1] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_base.png");
    tutorialTexs[2] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_attack.png");
    tutorialTexs[3] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin1.png");
    tutorialTexs[4] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin2.png");
    tutorialTexs[5] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin3.png");
    tutorialTexs[6] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin4.png");
    tutorialTexs[7] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_remove.png");
}

void MCB::Scene::LoadSound()
{
	testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::Scene::SpriteInit()
{
    playerHpSprite.InitMatProje();
    playerHpSprite = playerHpSprite.CreateSprite();
    playerHpSprite.anchorPoint = { 0,0 };
    playerHpSprite.tex = playerHp->texture.get();
    bossHpSprite.InitMatProje();
    bossHpSprite = bossHpSprite.CreateSprite();
    bossHpSprite.tex = bossHp->texture.get();
    bossHpSprite.anchorPoint = { 0,0 };
    shardSprite.InitMatProje();
    shardSprite = shardSprite.CreateSprite();
    shardSprite.tex = shard->texture.get();
    shardSprite.anchorPoint = { 0,0 };
    debugText.Init(debugTextTexture->texture.get());
    substie->TutorialInitialize(tutorialTexs[0]->texture.get(), tutorialTexs[1]->texture.get(), tutorialTexs[2]->texture.get(),
        tutorialTexs[3]->texture.get(), tutorialTexs[4]->texture.get(), tutorialTexs[5]->texture.get(), tutorialTexs[6]->texture.get(), tutorialTexs[7]->texture.get());
}

void MCB::Scene::ParticleInit()
{
    //testParticle.Init(testTex);
    //testParticle.position = { 0,0,10 };
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

        substie->Update();
        spownTimer.Update();

        if (spownTimer.IsEnd() && enemys.size() < 6)
        {
            int count = 0;
            int countErase = 0;
            for (auto& itr : enemys)
            {
                if (itr->enemyType == Enemy::Type::Writing)
                {
                    count++;
                }
                else
                {
                    countErase++;
                }
            }
            if (count < 2)
            {
                unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
                temp->SetHandwritingModel(WritingModel.get());
                temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                enemys.push_back(move(temp));
            }
            else if (countErase <= 1)
            {
                unique_ptr<EraserEnemy> temp = make_unique<EraserEnemy>();
                temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                temp->handwritingModel = WritingModel.get();
                enemys.push_back(move(temp));
            }
            else
            {
                switch (GetRand(1, 3))
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
                    temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                    temp->handwritingModel = WritingModel.get();
                    enemys.push_back(move(temp));
                }
                break;
                default:
                {
                    unique_ptr<EraserEnemy> temp = make_unique<EraserEnemy>();
                    temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                    temp->handwritingModel = WritingModel.get();
                    enemys.push_back(move(temp));
                }
                break;
                }
            }

            spownTimer.Set(60);
        }

        for (auto& itr : enemys)
        {
            itr->Update();
        }

        boss->Update();

        lights->UpDate();
        viewCamera->Update();

    CheckAllColision();

    for (int i = 0; i < enemys.size(); i++)
    {
        if (Object3d::DeleteAllowed(enemys[i].get()))
        {
            enemys.erase(enemys.begin() + i);
            i--;
        }
    }
    //行列変換
    //MatrixUpdate();

    if (boss->GetHp() <= 0 || substie->GetHp() <= 0)
    {
        sceneEnd = true;
    }
}

void MCB::Scene::Draw()
{
    //3Dオブジェクト
    Skydome.Draw();
    ground.Draw();
    if (substie->GetVisible())
    {
        substie->Draw();
    }
    for (auto& itr : walls)
    {
        itr.Draw();
    }
    for (auto& itr : enemys)
    {
        itr->Draw();
    }
    boss->Draw();
    WritingEnemy::StaticDraw();
    //human.Draw();
    //testSpher.Draw();
    //testSpher.FbxDraw();
}

void MCB::Scene::SpriteDraw()
{

    substie->TutorialDraw();
    substie->StatusDraw();

    bossHpSprite.SpriteDraw(10, 60, 290/2,80/2);
    debugText.Print(15 + 290 / 2, 65, 2, "%d", boss->GetHp());

    //debugText.Print(20, 20, 2, "boss:hp %d",boss.GetHp());
    ////debugText.Print(20, 60, 2, "player:hp %d",substie.GetHp());
    //debugText.Print(20, 60, 2, "position x:%fz:%f", substie.position.x,substie.position.z);
    //debugText.Print(20, 100, 2, "player:shard %f", substie.GetShard());
    //debugText.Print(dxWindow->window_width - 300, 20, 2, "Move:WASD");
    //debugText.Print(dxWindow->window_width - 300, 60, 2, "Action:SPACE", substie.GetShard());
    //debugText.Print(dxWindow->window_width - 300, 100, 2, "ActionCost:shard");
    if (boss->GetHp() <= 0)
    {
        debugText.Print(dxWindow->window_width/2 - 100, dxWindow->window_height/2, 4, "GameClear");
    }
    else if (substie->GetHp() <= 0)
    {
        debugText.Print(dxWindow->window_width / 2 - 100, dxWindow->window_height / 2, 4, "GameOver");
    }

    debugText.AllDraw();
}

void MCB::Scene::ParticleDraw()
{
    //testParticle.Draw();
}

void MCB::Scene::CheckAllColision()
{






}

void MCB::Scene::ImGuiUpdate()
{
    imgui.Begin();
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Debug");
    
    if (ImGui::TreeNode("Wall"))
    {
        int i = 0;
        for (auto& itr : walls)
        {
            i++;
            ImGui::Text("Wall%d", i);
            if (ImGui::TreeNode("Position"))
            {
                ImGui::SliderFloat("x", &itr.position.x, -100, 100);
                ImGui::SliderFloat("y", &itr.position.y, -100, 100);
                ImGui::SliderFloat("z", &itr.position.z, -100, 100);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Scale"))
            {
                ImGui::SliderFloat("x", &itr.scale.x, 0, 10);
                ImGui::SliderFloat("y", &itr.scale.y, 0, 10);
                ImGui::SliderFloat("z", &itr.scale.z, 0, 10);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Rotation"))
            {
                ImGui::SliderFloat("x", &itr.rotation.x, 0, 2 * PI);
                ImGui::SliderFloat("y", &itr.rotation.y, 0, 2 * PI);
                ImGui::SliderFloat("z", &itr.rotation.z, 0, 2 * PI);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
        //if (ImGui::Button("Genarate"))
        if (ImGui::TreeNode("Player"))
        {
            if (ImGui::TreeNode("Position"))
            {
                ImGui::Text("X %f", substie->position.x);
                ImGui::Text("Y %f", substie->position.y);
                ImGui::Text("Z %f", substie->position.z);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("shard"))
            {
                ImGui::SliderFloat("shardCost %f", &substie->shardCost,0,20);
                ImGui::SliderFloat("shardRotaCost %f", &substie->shardRotateCost,0,20);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        //{
        //    Object3d wall = Object3d();
        //    wall.scale = { 3,10,1 };
        //    wall.Init();
        //    wall.model = BoxModel;
        //    walls.push_back(wall);
        //    walls.back().colliders.push_back(ADXCollider(&walls.back()));
        //    walls.back().colliders.back().colType_ = box;
        //}
 

    
    ImGui::End();
    imgui.End();
}

void MCB::Scene::MatrixUpdate()
{
    viewCamera->Update();
    Skydome.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    //testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    substie->UpdateMatrix(viewCamera);
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
    boss->UpdateMatrix(viewCamera);
    //testParticle.Updata(matView, matProjection, true);
}

MCB::Scene::Scene(RootParameter* root, Depth* depthptr,PipeLineManager* pipeline)
{
	rootparamsPtr = root;
    depth = depthptr;
    this->pipeline = pipeline;
}

