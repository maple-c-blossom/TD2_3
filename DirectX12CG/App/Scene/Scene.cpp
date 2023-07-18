#include "Scene.h"
#include "TitleScene.h"
#include "Util.h"


using namespace MCB;
using namespace DirectX;

using namespace std;

MCB::Scene::~Scene()
{
    soundManager_->AllDeleteSound();
    WritingEnemy::GetHandWrite()->clear();
    Player::GetCaptureList()->clear();
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

    for (auto& itr : result)
    {
        itr->free = true;
    }

}

#pragma region �ʏ�ϐ��̏�������3D�I�u�W�F�N�g�̏�����
void MCB::Scene::Initialize()
{
    debugCamera.Inilialize();
    mainCamera.Inilialize();
    viewCamera_ = &mainCamera;
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow_->sWINDOW_WIDTH_ / dxWindow_->sWINDOW_HEIGHT_, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    ParticleInit();
    //soundManager.PlaySoundWave(testSound, loopFlag);
    lights_->DefaultLightSet();
    lights_->UpDate();
    Object3d::SetLights(lights_);
    soundManager_->PlaySoundWave(bgm, true);
    soundManager_->SetVolume(10, bgm);
}

void MCB::Scene::Object3DInit()
{

    ground;
    ground.Init();
    ground.model_ = groundModel.get();
    ground.scale_ = { 36,50,43 };
    ground.position_ = { 0,-12,0 };
    ;
    Skydome;
    Skydome.Init();
    Skydome.model_ = skydomeModel.get();
    Skydome.scale_ = { 180,1,180 };
    Skydome.position_ = { 0,-15,0 };

    substie->soundmanager = soundManager_;
    substie->model_ = playerModel.get();
    substie->KneadedEraserModel = nerikesiModel.get();
    substie->scale_ = { 1,1,1 };
    substie->position_ = { 0,0,0 };
    substie->Initialize();
    substie->sphereModel = BossDamegeEffectModelSpher.get();

    Cleaner tempC;
    tempC.Inilialize(creaner.get());
    walls.push_back(std::move(tempC));

    unique_ptr<PencilEnemy,MyDeleter<PencilEnemy>> temp (new PencilEnemy);
    temp->Enemy::Initialize({ 0,0,1 }, { 20,0,10 }, pencilEnemyModel.get(), 0.5f);
    temp->movePoint = { {20,0,10},{ 20,0,20 },{ -20,0,10 } };
    temp->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp));

    unique_ptr<PencilEnemy, MyDeleter<PencilEnemy>> temp2(new PencilEnemy);
    temp2->Enemy::Initialize({ 1,0,0 }, { -20,0,-10 }, pencilEnemyModel.get(), 0.5f);
    temp2->movePoint = { {-20,0,-10},{ -20,0,-20 },{ 20,0,-10 } };
    temp2->SetHandwritingModel(WritingModel.get());
    enemys.push_back(move(temp2));
    spownTimer.Set(30);
    boss->soundmanager = soundManager_;
    boss->Initialize({ 0,0,1 }, { -20,0,0 }, bossModel.get(), pencilEnemyModel.get(), WritingModel.get(), BossDamegeEffectModelStar.get(), BossDamegeEffectModelSpher.get(), BossCover.get(), 1, substie.get());
    boss->shake = mainCamera.GetShakePtr();
    mainCamera.player = substie.get();
    mainCamera.boss = boss.get();
    resultSizeTimer.Set(360);
    
    
    //sphere.Init();
    //sphere.model = BoxModel;
    //sphere.SetCollider(1);
    //sphere.position.z = 20;
}

#pragma endregion �ʏ�ϐ��̏�����

#pragma region �e�탊�\�[�X�̓ǂݍ��݂Ə�����
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
    selectSound = soundManager.LoadWaveSound("Resources\\sound\\se\\select.wav");

#pragma region tutorial
    tutorialTexs[0] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_move.png");
    tutorialTexs[1] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_base.png");
    tutorialTexs[2] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_attack.png");
    tutorialTexs[3] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin1.png");
    tutorialTexs[4] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin2.png");
    tutorialTexs[5] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin3.png");
    tutorialTexs[6] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_spin4.png");
    tutorialTexs[7] = loader->LoadTexture(L"Resources\\ctrlGuide\\ctrlGuide_remove.png");
#pragma endregion

#pragma region result
    result[(int)Result::Clear] = loader->LoadTexture(L"Resources\\result\\clear.png");
    result[(int)Result::GameOver] = loader->LoadTexture(L"Resources\\result\\gameover.png");
    result[(int)Result::Frame] = loader->LoadTexture(L"Resources\\result\\frame.png");
    result[(int)Result::Title] = loader->LoadTexture(L"Resources\\result\\title.png");
    result[(int)Result::Space] = loader->LoadTexture(L"Resources\\result\\space.png");
    result[(int)Result::ABottom] = loader->LoadTexture(L"Resources\\result\\a.png");
    result[(int)Result::Time] = loader->LoadTexture(L"Resources\\result\\time.png");
    result[(int)Result::Rank] = loader->LoadTexture(L"Resources\\result\\rank.png");
    result[(int)Result::ARank] = loader->LoadTexture(L"Resources\\result\\rank_a.png");
    result[(int)Result::BRank] = loader->LoadTexture(L"Resources\\result\\rank_b.png");
    result[(int)Result::CRank] = loader->LoadTexture(L"Resources\\result\\rank_c.png");
    result[(int)Result::SRank] = loader->LoadTexture(L"Resources\\result\\rank_s.png");
#pragma endregion

}

void MCB::Scene::LoadSound()
{
    bgm = soundManager.LoadWaveSound("Resources\\sound\\bgm\\game.wav");
    clearbgm = soundManager.LoadWaveSound("Resources\\sound\\bgm\\clear.wav");
    gameOverbgm = soundManager.LoadWaveSound("Resources\\sound\\bgm\\gameover.wav");

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

    for(auto& itr: resultSprite)
    {
        itr = make_unique<Sprite>();
        *itr = shardSprite.CreateSprite();
    }
    resultSprite[(int)Result::Clear]->tex = result[(int)Result::Clear]->texture.get();
    resultSprite[(int)Result::GameOver]->tex = result[(int)Result::GameOver]->texture.get();
    resultSprite[(int)Result::Frame]->tex = result[(int)Result::Frame]->texture.get();
    resultSprite[(int)Result::Title]->tex = result[(int)Result::Title]->texture.get();
    resultSprite[(int)Result::Space]->tex = result[(int)Result::Space]->texture.get();
    resultSprite[(int)Result::ABottom]->tex = result[(int)Result::ABottom]->texture.get();
    resultSprite[(int)Result::Time]->tex = result[(int)Result::Time]->texture.get();
    resultSprite[(int)Result::Rank]->tex = result[(int)Result::Rank]->texture.get();
    resultSprite[(int)Result::ARank]->tex = result[(int)Result::ARank]->texture.get();
    resultSprite[(int)Result::BRank]->tex = result[(int)Result::BRank]->texture.get();
    resultSprite[(int)Result::CRank]->tex = result[(int)Result::CRank]->texture.get();
    resultSprite[(int)Result::SRank]->tex = result[(int)Result::SRank]->texture.get();

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

#pragma endregion �e�탊�\�[�X�̓ǂݍ��݂Ə�����

void MCB::Scene::Update()
{

    Enemy::StaticUpdate();
    KneadedEraser::StaticUpdate();
    WritingEnemy::StaticUpdate();
//        if (input->IsKeyTrigger(DIK_SPACE))
//        {
//            sceneEnd = true;
//        }
    if (boss->GetHp() > 0 && substie->GetHp() > 0)
    {
        time++;
        for (auto& itr : walls)
        {
            itr.Update();
        }
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
            if (count < 3)
            {
                unique_ptr<PencilEnemy, MyDeleter<PencilEnemy>> temp(new PencilEnemy);
                temp->SetHandwritingModel(WritingModel.get());
                temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                enemys.push_back(move(temp));
            }
            else if (countErase <= 2)
            {
                unique_ptr<EraserEnemy, MyDeleter<EraserEnemy>> temp(new EraserEnemy);
                temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                temp->handwritingModel = WritingModel.get();
                enemys.push_back(move(temp));
            }
            else if (count >= 5)
            {
                unique_ptr<EraserEnemy, MyDeleter<EraserEnemy>> temp(new EraserEnemy);
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
                    unique_ptr<PencilEnemy, MyDeleter<PencilEnemy>> temp(new PencilEnemy);
                    temp->SetHandwritingModel(WritingModel.get());
                    temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                    temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                    enemys.push_back(move(temp));
                }
                break;
                case 2:
                {
                    unique_ptr<EraserEnemy, MyDeleter<EraserEnemy>> temp(new EraserEnemy);
                    temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                    temp->handwritingModel = WritingModel.get();
                    enemys.push_back(move(temp));
                }
                break;
                default:
                {
                    unique_ptr<EraserEnemy, MyDeleter<EraserEnemy>> temp(new EraserEnemy);
                    temp->Initialize({ 0,0,1 }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, eraseEnemyModel.get(), 0.15f);
                    temp->handwritingModel = WritingModel.get();
                    enemys.push_back(move(temp));
                }
                break;
                }
            }

            spownTimer.Set(30);
        }

        for (auto& itr : enemys)
        {
            itr->Update();
        }

        boss->Update();

    }
    else if (boss->GetHp() <= 0)
    {
        soundManager.StopSoundWave(substie->rotateSound);
        boss->DethUpdate();
        resultSize = sinf(ConvertRadius((float)resultSizeTimer.NowTime() >= 180 ? resultSizeTimer.NowTime()  * -1: resultSizeTimer.NowTime())) * 0.25f + 0.75f;
        resultSizeTimer.SafeUpdate();
        resultSizeTimer.ReSet();
        if (mainCamera.isok)
        {
            if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
            {
                sceneEnd = true;
                soundManager.PlaySoundWave(selectSound);
            }
        }
    }
    else if (substie->GetHp() <= 0)
    {
        soundManager.StopSoundWave(substie->rotateSound);
        substie->DethUpdate();
        resultSize = sinf(ConvertRadius((float)resultSizeTimer.NowTime() >= 180 ? resultSizeTimer.NowTime() * -1 : resultSizeTimer.NowTime())) * 0.25f + 0.75f;
        resultSizeTimer.SafeUpdate();
        resultSizeTimer.ReSet();
        if (mainCamera.isok)
        {
            if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
            {
                sceneEnd = true;
                soundManager.PlaySoundWave(selectSound);
            }
        }
    }
    if (boss->GetHp() <= 0 || substie->GetHp() <= 0)
    {
        soundManager.StopSoundWave(bgm);
    }
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
    //�s��ϊ�
    //MatrixUpdate();


}

void MCB::Scene::Draw()
{
    //3D�I�u�W�F�N�g
    Skydome.Draw();
    ground.Draw();
    if (substie->GetVisible())
    {
        if (!boss->afterdethDown)
        {
            substie->Draw();
        }
    }
    if (!(substie->GetHp() <= 0))
    {
        if (!boss->afterdethDown)
        {
            for (auto& itr : walls)
            {
                itr.Draw();
            }
            for (auto& itr : enemys)
            {
                itr->Draw();
            }
        }
        boss->Draw();
    }
    WritingEnemy::StaticDraw();
    //human.Draw();
    //testSpher.Draw();
    //testSpher.FbxDraw();
}

void MCB::Scene::SpriteDraw()
{
    if (!(substie->GetHp() <= 0))
    {
        if (!boss->afterdethDown)
        {
            substie->TutorialDraw();
            substie->StatusDraw();

            boss->StatusDraw();
            debugText.Print(150, 40, 0.5f, "%02d:%02d", time / SECOND_FRAME / 60, time / SECOND_FRAME % 60);
        }
    }

    if (mainCamera.isok && boss->GetHp() <= 0 )
    {
        if (!soundPlayed)
        {
            soundManager.PlaySoundWave(clearbgm, true);
            soundManager.SetVolume(10, clearbgm);
            soundPlayed = true;
        }
        resultSprite[(int)Result::Clear]->SpriteDraw(dxWindow->window_width / 2, 90, 576, 60);
        resultSprite[(int)Result::Frame]->SpriteDraw(dxWindow->window_width / 2, dxWindow->window_height / 2, 448 * 2, 446);
        resultSprite[(int)Result::Rank]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2 - 83);
        if (time < SECOND_FRAME * 46)
        {
            resultSprite[(int)Result::SRank]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2 + 43,48 * 1.5f,80 * 1.5f);
        }
        else if (time < SECOND_FRAME * 56)
        {
            resultSprite[(int)Result::ARank]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2 + 43, 48 * 1.5f, 80 * 1.5f);
        }
        else if (time < SECOND_FRAME * 76)
        {
            resultSprite[(int)Result::BRank]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2 + 43, 48 * 1.5f, 80 * 1.5f);
        }
        else
        {
            resultSprite[(int)Result::CRank]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2 + 43, 48 * 1.5f, 80 * 1.5f);
        }
        debugText.Print(dxWindow->window_width / 2 - 224 - (48 * 2) - 24, dxWindow->window_height / 2 + 106, 1, "%02d:%02d", time / SECOND_FRAME / 60, time / SECOND_FRAME % 60);
        resultSprite[(int)Result::Title]->SpriteDraw(dxWindow->window_width / 2 + 224, dxWindow->window_height / 2 - 63,320 * 0.75f,160 * 0.75f);
        resultSprite[(int)Result::Space]->SpriteDraw(dxWindow->window_width / 2 + 224, dxWindow->window_height / 2 + 63, 248 * resultSize, 62 * resultSize);
        resultSprite[(int)Result::ABottom]->SpriteDraw(dxWindow->window_width / 2 + 224, dxWindow->window_height / 2 + 126, 66 * resultSize, 66 * resultSize);
    }
    else if (mainCamera.isok && substie->GetHp() <= 0)
    {
        if (!soundPlayed)
        {
            soundManager.PlaySoundWave(gameOverbgm);
            soundManager.SetVolume(10, gameOverbgm);
            soundPlayed = true;
        }
        resultSprite[(int)Result::GameOver]->SpriteDraw(dxWindow->window_width / 2, 90, 576, 80);
        resultSprite[(int)Result::Frame]->SpriteDraw(dxWindow->window_width / 2, dxWindow->window_height / 2, 448 * 2, 446);
        resultSprite[(int)Result::Title]->SpriteDraw(dxWindow->window_width / 2 - 224, dxWindow->window_height / 2);
        resultSprite[(int)Result::Space]->SpriteDraw(dxWindow->window_width / 2 + 224, dxWindow->window_height / 2 - 63,248 * resultSize,62 * resultSize);
        resultSprite[(int)Result::ABottom]->SpriteDraw(dxWindow->window_width / 2 + 224, dxWindow->window_height / 2 + 63,66 * resultSize,66 * resultSize);
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
        itr.Object3d::Update(*viewCamera->GetView(), *viewCamera->GetProjection());
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
