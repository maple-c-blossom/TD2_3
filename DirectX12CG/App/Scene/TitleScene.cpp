#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{
    titleSprite.InitMatProje();
    titleSprite.CreateSprite();

    pushSpaceSprite.InitMatProje();
    pushSpaceSprite.CreateSprite();
    scopeSprite.InitMatProje();
    scopeSprite.CreateSprite();
    tutorialSkipSprite.CreateSprite();
    tutorialSkipSprite.anchorPoint_ = {1.f,0.f};
    debugText.Init(debugTextTexture->texture.get());
}

void MCB::TitleScene::ParticleInit()
{
}

std::unique_ptr<IScene> MCB::TitleScene::GetNextScene()
{
    return std::move(make_unique<Scene>(rootparamsPtr_, depth_, pipeline_));
}

void MCB::TitleScene::MatrixUpdate()
{
    viewCamera_->Update();
    Skydome.Update();
    ground.Update();
    boss->UpdateMatrix(viewCamera_);
    for (auto& itr : writing)
    {
        itr->Object3d::Update();
    }
    for(auto& itr:tutorialBode)itr.Update(true);
    //testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    substie->UpdateMatrix(viewCamera_);
    for (auto& itr : enemys_6tutorial)
    {
        itr->UpdateMatrix(viewCamera_);
    }
    for (auto& itr : enemys_7tutorial)
    {
        itr->UpdateMatrix(viewCamera_);
    }
}

void MCB::TitleScene::Update()
{
    Enemy::StaticUpdate();
    KneadedEraser::StaticUpdate();
    WritingEnemy::StaticUpdate();
    i++;
    animTime.Update();
    float tempVec = 0;
    resultSize = sinf(ConvertRadius((float)resultSizeTimer.NowTime() >= 180 ? resultSizeTimer.NowTime() * -1 : resultSizeTimer.NowTime())) * 0.25f + 1.2f;
    resultSizeTimer.SafeUpdate();
    resultSizeTimer.ReSet();
    if (animTime.IsEnd())
    {
        tutorial1AnimNum++;
        if (tutorial1AnimNum >= tutorial1.size())
        {
            tutorial1AnimNum = 0;
        }
        tutorial2AnimNum++;
        if (tutorial2AnimNum >= tutorial2.size())
        {
            tutorial2AnimNum = 0;
        }
        tutorial3AnimNum++;
        if (tutorial3AnimNum >= tutorial3.size())
        {
            tutorial3AnimNum = 0;
        }
        tutorial4AnimNum++;
        if (tutorial4AnimNum >= tutorial4.size())
        {
            tutorial4AnimNum = 0;
        }
        tutorial5AnimNum++;
        if (tutorial5AnimNum >= tutorial5.size())
        {
            tutorial5AnimNum = 0;
        }
        animTime.ReSet();

    }
    if (i % 180 == 0)
    {
        writing.clear();
    }
    

    enemys_6tutorial.remove_if([](auto& itr) {return itr->deleteFlag_; });
    enemys_7tutorial.remove_if([](auto& itr) {return itr->deleteFlag_; });
    substie->Update(false);
    boss->Update(false);
    boss->position_ = { 90,0,-15 };
    for (auto& itr : writing)
    {
        itr->Update();
        for (auto& itr2 : itr->colliders)
        {
            itr2.Update(itr.get());
        }
    }
    int i = 0;
    for (auto& itr : enemys_6tutorial)
    {
        itr->Update(false);
        //for (auto& itr2 : itr->colliders)
        //{
        //    itr2.Update(itr.get());
        //}
        if (itr->GetcapturePtr() == nullptr)
        {
            switch (i + 1)
            {
            case 1:
                itr->position_.x = 38.f;
                itr->position_.z = -2.5f;
                break;
            case 2:
                itr->position_.x = 45.f;
                itr->position_.z = -2.5f;
                break;
            case 3:
                itr->position_.x = 52.f;
                itr->position_.z = -2.5f;
                break;
            case 4:
                itr->position_.x = 41.5f;
                itr->position_.z = -15.f;
                break;
            case 5:
                itr->position_.x = 48.5f;
                itr->position_.z = -15.0f;
                break;
            default:
                break;
            }
        }
        i++;
    }
    i = 0;
    for (auto& itr : enemys_7tutorial)
    {
        itr->Update(false);
        //for (auto& itr2 : itr->colliders)
        //{
        //    itr2.Update(itr.get());
        //}
        if (itr->GetcapturePtr() == nullptr)
        {
            switch (i + 1)
            {
            case 1:
                itr->position_.x = 98.f;
                itr->position_.z = -2.5f;
                break;
            case 2:
                itr->position_.x = 105.f;
                itr->position_.z = -2.5f;
                break;
            case 3:
                itr->position_.x = 112.f;
                itr->position_.z = -2.5f;
                break;
            case 4:
                itr->position_.x = 101.5f;
                itr->position_.z = -15.f;
                break;
            case 5:
                itr->position_.x = 108.5f;
                itr->position_.z = -15.0f;
                break;
            default:
                break;
            }
        }
        i++;
    }
    if (substie->position_.x < tutorialBode[2].position_.x - 30)
    {
        substie->SetShard(1);
    }
    for (int i = 2; i < tutorialBode.size(); i++)
    {

        if (substie->position_.x <= tutorialBode[i].position_.x + 30 && substie->position_.x >= tutorialBode[i].position_.x - 30)
        {
            switch (i - 1)
            {
            case 1:

                if (writing.size() <= 0)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
                            temp->Initialize({ tutorialBode[i].position_.x - 8 + j - 2,tutorialBode[i].position_.y,tutorialBode[i].position_.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }

                if (substie->GetShard() >= 10)
                {
                    if (!(tutorialSucces & 0b10000))
                    {
                        soundManager_->PlaySoundWave(succeseSound);
                    }
                    tutorialSucces |= 0b10000;
                }
                break;

            case 2:
                if (writing.size() <= 0)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
                            temp->Initialize({ tutorialBode[i].position_.x - 8 + j - 2,tutorialBode[i].position_.y,tutorialBode[i].position_.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                 tempVec = substie->GetVelocity().V3Len();
                if (substie->GetTrueMake())
                {
                    velocitySum += tempVec;
                }
                else
                {
                    //velocitySum = 0;
                }

                if (velocitySum >= 3)
                {
                    if (!(tutorialSucces & 0b01000))
                    {
                        soundManager_->PlaySoundWave(succeseSound);
                    }
                    tutorialSucces |= 0b01000;
                }
                break;
            case 3:
                if (writing.size() <= 0)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
                            temp->Initialize({ tutorialBode[i].position_.x - 8 + j - 2,tutorialBode[i].position_.y,tutorialBode[i].position_.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (substie->GetRotaMode())
                {
                    RotateFrame++;
                }
                if (RotateFrame >= SECOND_FRAME * 1)
                {
                    if (!(tutorialSucces & 0b00100))
                    {
                        soundManager_->PlaySoundWave(succeseSound);
                    }
                    tutorialSucces |= 0b00100;
                }
                for (auto& itr : enemys_6tutorial)
                {
                    itr->deleteFlag_ = true;
                }
                enemys_6tutorial.clear();
                break;
            case 4:
                if (enemys_6tutorial.size() <= 0)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
                        temp->SetHandwritingModel(WritingModel.get());
                        temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                        temp->movePoint = { {-20 + temp->position_.x,0,20 + temp->position_.z},{ 20 + temp->position_.x,0,40 + temp->position_.z },{ 20 + temp->position_.x,0,20 + temp->position_.z } };
                        switch (i + 1)
                        {
                        case 1:
                            temp->position_.x = 38.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 2:
                            temp->position_.x = 45.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 3:
                            temp->position_.x = 52.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 4:
                            temp->position_.x = 41.5f;
                            temp->position_.z = -15.f;
                            break;
                        case 5:
                            temp->position_.x = 48.5f;
                            temp->position_.z = -15.0f;
                            break;
                        default:
                            break;
                        }

                        enemys_6tutorial.push_back(move(temp));

                    }
                }
                if (writing.size() <= 0)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
                            temp->Initialize({ tutorialBode[i].position_.x - 8 + j - 2,tutorialBode[i].position_.y,tutorialBode[i].position_.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (substie.get()->GetCaptureList()->size() >= 5)
                {
                    if (!(tutorialSucces & 0b00010))
                    {
                        soundManager_->PlaySoundWave(succeseSound);
                    }
                    tutorialSucces |= 0b00010;
                }

                for (auto& itr : enemys_7tutorial)
                {
                    itr->deleteFlag_ = true;
                }
                break;
            case 5:
                for (auto& itr : enemys_6tutorial)
                {
                    itr->deleteFlag_ = true;
                }
                if (enemys_7tutorial.size() <= 0)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
                        temp->SetHandwritingModel(WritingModel.get());
                        temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                        temp->movePoint = { {-20 + temp->position_.x,0,20 + temp->position_.z},{ 20 + temp->position_.x,0,40 + temp->position_.z },{ 20 + temp->position_.x,0,20 + temp->position_.z } };
                        switch (i + 1)
                        {
                        case 1:
                            temp->position_.x = 98.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 2:
                            temp->position_.x = 105.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 3:
                            temp->position_.x = 112.f;
                            temp->position_.z = -2.5f;
                            break;
                        case 4:
                            temp->position_.x = 101.5f;
                            temp->position_.z = -15.f;
                            break;
                        case 5:
                            temp->position_.x = 108.5f;
                            temp->position_.z = -15.0f;
                            break;
                        default:
                            break;
                        }

                        enemys_7tutorial.push_back(move(temp));

                    }
                }
                if (writing.size() <= 0)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
                            temp->Initialize({ tutorialBode[i].position_.x - 8 + j - 2,tutorialBode[i].position_.y,tutorialBode[i].position_.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (boss->IsImotal())
                {
                    if (!(tutorialSucces & 0b00001))
                    {
                        soundManager_->PlaySoundWave(succeseSound);
                    }
                    tutorialSucces |= 0b00001;
                }
                break;
            case 6:
                for (auto& itr : enemys_7tutorial)
                {
                    itr->deleteFlag_ = true;
                }
                if ((input_->IsKeyTrigger(DIK_SPACE) || input_->gamePad_->IsButtonTrigger(GAMEPAD_A)))
                {
                    sceneEnd_ = true;
                    soundManager_->PlaySoundWave(selectSound);
                }
                break;
            default:
                break;
            }
            break;
        }
    }
    //MatrixUpdate();
    if (input_->IsKeyTrigger(DIK_P) || input_->IsKeyTrigger(DIK_RETURN)  || input_->gamePad_->IsButtonTrigger(GAMEPAD_START))
    {
        sceneEnd_ = true;
        soundManager_->PlaySoundWave(selectSound);
    }
    

}

void MCB::TitleScene::Draw()
{
    //3Dオブジェクト
    Skydome.Draw();
    ground.Draw();
    testSpher.Draw();
    substie->Draw();
    boss->Draw();
    for (auto& itr : writing)
    {
        itr->Draw();
    }
    for (auto& itr : enemys_6tutorial)
    {
        itr->Draw();
    }
    for (auto& itr : enemys_7tutorial)
    {
        itr->Draw();
    }

    tutorialBode[0].Draw(tutorial[0]->texture->incrementNum_);
    tutorialBode[1].Draw(tutorial[1]->texture->incrementNum_);
    if (tutorialSucces & 0b10000)
    {
        tutorialBode[2].Draw(tutorial1Succes[tutorial1AnimNum]->texture->incrementNum_);
    }
    else
    {
        tutorialBode[2].Draw(tutorial1[tutorial1AnimNum]->texture->incrementNum_);
    }

    if (tutorialSucces & 0b01000)
    {
        tutorialBode[3].Draw(tutorial2Succes[tutorial2AnimNum]->texture->incrementNum_);
    }
    else
    {
        tutorialBode[3].Draw(tutorial2[tutorial2AnimNum]->texture->incrementNum_);
    }

    if (tutorialSucces & 0b00100)
    {
        tutorialBode[4].Draw(tutorial3Succes[tutorial3AnimNum]->texture->incrementNum_);
    }
    else
    {
        tutorialBode[4].Draw(tutorial3[tutorial3AnimNum]->texture->incrementNum_);
    }

    if (tutorialSucces & 0b00010)
    {
        tutorialBode[5].Draw(tutorial4Succes[tutorial4AnimNum]->texture->incrementNum_);
    }
    else
    {
        tutorialBode[5].Draw(tutorial4[tutorial4AnimNum]->texture->incrementNum_);
    }

    if (tutorialSucces & 0b00001)
    {
        tutorialBode[6].Draw(tutorial5Succes[tutorial5AnimNum]->texture->incrementNum_);
    }
    else
    {
        tutorialBode[6].Draw(tutorial5[tutorial5AnimNum]->texture->incrementNum_);
    }
    tutorialBode[7].Draw(tutorial[2]->texture->incrementNum_);
  
}

void MCB::TitleScene::SpriteDraw()
{
   titleSprite.SpriteDraw(*title->texture.get(), dxWindow_->sWINDOW_WIDTH_ / 2 - (substie->position_.x + 210) * 30, dxWindow_->sWINDOW_HEIGHT_ / 2 - 80, 512 * resultSize, 128 * resultSize);
    //pushSpaceSprite.SpriteDraw(dxWindow_->sWINDOW_WIDTH_ / 2 - substie->position_.x * 30, dxWindow_->sWINDOW_HEIGHT_ / 2 + 40 + substie->position_.z * 30);
    if (substie->position_.x >= tutorialBode[2].position_.x - 30)
    {
        substie->StatusDraw();
    }

    tutorialSkipSprite.SpriteDraw(*tutorialSkipTex->texture.get(), dxWindow_->sWINDOW_WIDTH_ - 10,10.f,480 * 0.85f, 100 * 0.85f);
    debugText.AllDraw();
}

void MCB::TitleScene::ParticleDraw()
{
}

void MCB::TitleScene::CheckAllColision()
{
}

void MCB::TitleScene::ImGuiUpdate()
{
    imgui_.Begin();
    ImGui::Begin("Debug");
    /*if (ImGui::TreeNode("Tutorial"))
    {
        if (ImGui::TreeNode("position_"))
        {
            ImGui::SliderFloat("x", &tutorialBode.position_.x, -100, 100);
            ImGui::SliderFloat("y", &tutorialBode.position_.y, -100, 100);
            ImGui::SliderFloat("z", &tutorialBode.position_.z, -100, 100);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("scale_"))
        {
            ImGui::SliderFloat("x", &tutorialBode.scale_.x, 0, 10);
            ImGui::SliderFloat("y", &tutorialBode.scale_.y, 0, 10);
            ImGui::SliderFloat("z", &tutorialBode.scale_.z, 0, 10);
            ImGui::TreePop();
        }
    }*/
    if (ImGui::TreeNode("Player"))
    {
        if (ImGui::TreeNode("position_"))
        {
            ImGui::Text("X %f", substie->position_.x);
            ImGui::Text("Y %f", substie->position_.y);
            ImGui::Text("Z %f", substie->position_.z);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("shard"))
        {
            ImGui::Text("shard %f", substie->GetShard(), 0, 20);
            ImGui::SliderFloat("shardCost %f", &substie->shardCost, 0, 20);
            ImGui::SliderFloat("shardRotaCost %f", &substie->shardRotateCost, 0, 20);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::End();
    imgui_.End();
}

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr_ = root;
	this->depth_ = depth;
    this->pipeline_ = pipeline;
}

MCB::TitleScene::~TitleScene()
{
    soundManager_->AllDeleteSound();
    
    title->free = true;
    debugTextTexture->free = true;
    pushSpace->free = true;
    //scopeTex->free = true;
    tutorialSkipTex->free = true;

    for (auto& itr : tutorial)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial1)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial1Succes)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial2)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial2Succes)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial3)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial3Succes)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial4)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial4Succes)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial5)
    {
        itr->free = true;
    }

    for (auto& itr : tutorial5Succes)
    {
        itr->free = true;
    }


}

void MCB::TitleScene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 3.0f, -10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow_->sWINDOW_WIDTH_ / dxWindow_->sWINDOW_HEIGHT_, 0.1f, 4000.0f);
    camera.Inilialize();
    viewCamera_ = &camera;
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();
    ParticleInit();
    //soundManager_->PlaySoundWave(testSound, loopFlag);
    lights_->DefaultLightSet();
    lights_->UpDate();
    Object3d::SetLights(lights_);
    animTime.Set(30);
    soundManager_->PlaySoundWave(bgm, true);
    soundManager_->SetVolume(10, bgm);
    resultSizeTimer.Set(360);
}

void MCB::TitleScene::LoadModel()
{
    BoxModel = std::make_unique<Model>("Box");

    groundModel = std::make_unique<Model>("note");

    skydomeModel = std::make_unique<Model>("table");

    playerModel = std::make_unique<Model>("player");
    pencilEnemyModel = std::make_unique<Model>("pencil");
    WritingModel = std::make_unique<Model>("Box");
    bossModel = std::make_unique<Model>("boss");
    nerikesiModel = std::make_unique<Model>("nerikeshi");
    eraseEnemyModel = std::make_unique<Model>("eraser");
    BossDamegeEffectModelStar = std::make_unique<Model>("star");
    tutorialBlackBode = std::make_unique<Model>("blackboard");
    BossDamegeEffectModelSpher = std::make_unique<Model>("ball");
    BossCover = std::make_unique<Model>("bossCover");
    //BossCover->texture = loader->LoadTexture(L"Resources\\text\\bossCover\\bossCover.png");
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture = loader_->LoadTexture(L"Resources\\debugfont.png");
    title = loader_->LoadTexture(L"Resources\\text\\title.png");
    pushSpace = loader_->LoadTexture(L"Resources\\text\\pushSpace.png");
    //scopeTex = loader->LoadTexture(L"Resources\\scope.png");

    tutorial[0] = loader_->LoadTexture(L"Resources\\tutorial\\title_sousa.png");
    tutorialSkipTex = loader_->LoadTexture(L"Resources\\tutorial\\skip_1.png");
    tutorial[1] = loader_->LoadTexture(L"Resources\\tutorial\\title_yazirushi.png");
    tutorial[2] = loader_->LoadTexture(L"Resources\\tutorial\\start.png");
#pragma region tutorial1
    tutorial1[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1\\tutorial1_1.png");
    tutorial1[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1\\tutorial1_2.png");
    tutorial1[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1\\tutorial1_3.png");
    tutorial1[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1\\tutorial1_4.png");
    tutorial1[4] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1\\tutorial1_5.png");

    tutorial1Succes[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1Succes\\tutorial1_maru_1.png");
    tutorial1Succes[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1Succes\\tutorial1_maru_2.png");
    tutorial1Succes[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1Succes\\tutorial1_maru_3.png");
    tutorial1Succes[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1Succes\\tutorial1_maru_4.png");
    tutorial1Succes[4] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial1Succes\\tutorial1_maru_5.png");
#pragma endregion

#pragma region tutorial2
    tutorial2[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2\\tutorial2_1.png");
    tutorial2[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2\\tutorial2_2.png");
    tutorial2[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2\\tutorial2_3.png");
    tutorial2[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2\\tutorial2_4.png");
    tutorial2[4] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2\\tutorial2_5.png");

    tutorial2Succes[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2Succes\\tutorial2_maru_1.png");
    tutorial2Succes[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2Succes\\tutorial2_maru_2.png");
    tutorial2Succes[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2Succes\\tutorial2_maru_3.png");
    tutorial2Succes[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2Succes\\tutorial2_maru_4.png");
    tutorial2Succes[4] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial2Succes\\tutorial2_maru_5.png");
#pragma endregion

#pragma region tutorial3
    tutorial3[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3\\tutorial3_1.png");
    tutorial3[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3\\tutorial3_2.png");
    tutorial3[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3\\tutorial3_3.png");
    tutorial3[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3\\tutorial3_4.png");


    tutorial3Succes[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3Succes\\tutorial3_maru_1.png");
    tutorial3Succes[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3Succes\\tutorial3_maru_2.png");
    tutorial3Succes[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3Succes\\tutorial3_maru_3.png");
    tutorial3Succes[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial3Succes\\tutorial3_maru_4.png");

#pragma endregion

#pragma region tutorial4
    tutorial4[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4\\tutorial4_1.png");
    tutorial4[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4\\tutorial4_2.png");
    tutorial4[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4\\tutorial4_3.png");


    tutorial4Succes[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4Succes\\tutorial4_maru_1.png");
    tutorial4Succes[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4Succes\\tutorial4_maru_2.png");
    tutorial4Succes[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial4Succes\\tutorial4_maru_3.png");

#pragma endregion

#pragma region tutorial5
    tutorial5[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5\\tutorial5_1.png");
    tutorial5[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5\\tutorial5_2.png");
    tutorial5[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5\\tutorial5_3.png");
    tutorial5[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5\\tutorial5_3.png");


    tutorial5Succes[0] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5Succes\\tutorial5_maru_1.png");
    tutorial5Succes[1] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5Succes\\tutorial5_maru_2.png");
    tutorial5Succes[2] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5Succes\\tutorial5_maru_3.png");
    tutorial5Succes[3] = loader_->LoadTexture(L"Resources\\tutorial\\tutorial5Succes\\tutorial5_maru_3.png");

#pragma endregion
}

void MCB::TitleScene::LoadSound()
{
    bgm = soundManager_->LoadWaveSound("Resources\\sound\\bgm\\title.wav");
    succeseSound = soundManager_->LoadWaveSound("Resources\\sound\\se\\pinpon.wav");
    selectSound = soundManager_->LoadWaveSound("Resources\\sound\\se\\select.wav");

}

void MCB::TitleScene::Object3DInit()
{


    ground;
    ground.Init();
    ground.model_ = groundModel.get();
    ground.scale_ = { 181,90,181 };
    ground.position_ = { 0,-20,0 };

    Skydome;
    Skydome.Init();
    Skydome.model_ = skydomeModel.get();
    Skydome.scale_ = { 280,1,280 };
    Skydome.position_ = { 0,-25,0 };

    testSpher.Init();
    testSpher.model_ = BoxModel.get();
    testSpher.scale_ = { 3,3,3 };
    testSpher.position_ = { 0,4,10 };


    substie->soundmanager = soundManager_;
    substie->model_ = playerModel.get();
    substie->KneadedEraserModel = nerikesiModel.get();
    substie->scale_ = { 1,1,1 };
    substie->position_ = { -210,0,0 };
    substie->Initialize();

    boss->soundmanager = soundManager_;
    boss->Initialize({ 0,0,1 }, { -20,0,0 }, bossModel.get(), pencilEnemyModel.get(), WritingModel.get(), BossDamegeEffectModelStar.get(), BossDamegeEffectModelSpher.get(), BossCover.get(), 1, substie.get());
    boss->shake = camera.GetShakePtr();
    boss->position_ = { 90,0,-15 };
    //tutorialBode.Init();
    //tutorialBode.model_ = tutorialBlackBode.get();
    //tutorialBode.scale_ = { 6,6,6 };
    //tutorialBode.position_ = { 10,0,0 };
    for (int i = 0; i < 8; i++)
    {
        tutorialBode[i].Init();
        tutorialBode[i].model_ = tutorialBlackBode.get();
        tutorialBode[i].scale_ = { 1,1,1 };
        if (i < 2)continue;
        tutorialBode[i].position_ = { 60.f + 60.f * (i - 2) - 210 + 15,0,8.4f};

    }



    tutorialBode[0].position_ = { -15.f -210,0,8.4f };
    tutorialBode[1].position_ = { 15.f - 210,0,8.4f };
    camera.player = substie.get();
    camera.boss = boss.get();
}
