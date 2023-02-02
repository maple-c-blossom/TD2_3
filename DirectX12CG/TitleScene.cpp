#include "TitleScene.h"
#include "Scene.h"
using namespace MCB;
using namespace std;
using namespace DirectX;

void MCB::TitleScene::SpriteInit()
{
    titleSprite.InitMatProje();
    titleSprite = titleSprite.CreateSprite();
    titleSprite.tex = title->texture.get();
    pushSpaceSprite.InitMatProje();
    pushSpaceSprite = pushSpaceSprite.CreateSprite();
    pushSpaceSprite.tex = pushSpace->texture.get();
    scopeSprite.InitMatProje();
    scopeSprite = scopeSprite.CreateSprite();
    debugText.Init(debugTextTexture->texture.get());
}

void MCB::TitleScene::ParticleInit()
{
}

MCB::IScene* MCB::TitleScene::GetNextScene()
{
	return new Scene(rootparamsPtr, depth, pipeline);
}

void MCB::TitleScene::MatrixUpdate()
{
    viewCamera->Update();
    Skydome.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    ground.Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    boss->UpdateMatrix(viewCamera);
    for (auto& itr : writing)
    {
        itr->Object3d::Update(*viewCamera->GetView(), *viewCamera->GetProjection());
    }
    for(auto& itr:tutorialBode)itr.Update(*viewCamera->GetView(), *viewCamera->GetProjection(),true);
    //testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    substie->UpdateMatrix(viewCamera);
    for (auto& itr : enemys_6tutorial)
    {
        itr->UpdateMatrix(viewCamera);
    }
    for (auto& itr : enemys_7tutorial)
    {
        itr->UpdateMatrix(viewCamera);
    }
}

void MCB::TitleScene::Update()
{
    i++;
    if (i % 180 == 0)
    {
        writing.clear();
    }
    
    for (int i = 2; i < tutorialBode.size(); i++)
    {

        if (substie->position.x <= tutorialBode[i].position.x + 30 && substie->position.x >= tutorialBode[i].position.x - 30)
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
                            temp->Initialize({ tutorialBode[i].position.x - 8 + j - 2,tutorialBode[i].position.y,tutorialBode[i].position.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }

                if (substie->GetShard() >= 50)
                {
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
                            temp->Initialize({ tutorialBode[i].position.x - 8 + j - 2,tutorialBode[i].position.y,tutorialBode[i].position.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (substie->GetTrueMake())
                {
                    velocitySum += substie->GetVelocity().V3Len();
                }
                else
                {
                    velocitySum = 0;
                }

                if (velocitySum >= 20)
                {
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
                            temp->Initialize({ tutorialBode[i].position.x - 8 + j - 2,tutorialBode[i].position.y,tutorialBode[i].position.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (substie->GetRotaMode())
                {
                    RotateFrame++;
                }
                if (RotateFrame >= SECOND_FRAME * 5)
                {
                    tutorialSucces |= 0b00100;
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
                        temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                        switch (i + 1)
                        {
                        case 1:
                            temp->position.x = 38.f;
                            temp->position.z = -2.5f;
                            break;
                        case 2:
                            temp->position.x = 45.f;
                            temp->position.z = -2.5f;
                            break;
                        case 3:
                            temp->position.x = 52.f;
                            temp->position.z = -2.5f;
                            break;
                        case 4:
                            temp->position.x = 41.5f;
                            temp->position.z = -15.f;
                            break;
                        case 5:
                            temp->position.x = 48.5f;
                            temp->position.z = -15.0f;
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
                            temp->Initialize({ tutorialBode[i].position.x - 8 + j - 2,tutorialBode[i].position.y,tutorialBode[i].position.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (substie.get()->GetCaptureList()->size() >= 5)
                {
                    tutorialSucces |= 0b00010;
                }
                enemys_7tutorial.clear();
                break;
            case 5:
                enemys_6tutorial.clear();
                if (enemys_7tutorial.size() <= 0)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
                        temp->SetHandwritingModel(WritingModel.get());
                        temp->Initialize({ (float)GetRand(-1,1),0,(float)GetRand(-1,1) }, { (float)GetRand(-4000,4000) / 100,0,(float)GetRand(-3000,3000) / 100 }, pencilEnemyModel.get(), 0.5f);
                        temp->movePoint = { {-20 + temp->position.x,0,20 + temp->position.z},{ 20 + temp->position.x,0,40 + temp->position.z },{ 20 + temp->position.x,0,20 + temp->position.z } };
                        switch (i + 1)
                        {
                        case 1:
                            temp->position.x = 98.f;
                            temp->position.z = -2.5f;
                            break;
                        case 2:
                            temp->position.x = 105.f;
                            temp->position.z = -2.5f;
                            break;
                        case 3:
                            temp->position.x = 112.f;
                            temp->position.z = -2.5f;
                            break;
                        case 4:
                            temp->position.x = 101.5f;
                            temp->position.z = -15.f;
                            break;
                        case 5:
                            temp->position.x = 108.5f;
                            temp->position.z = -15.0f;
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
                            temp->Initialize({ tutorialBode[i].position.x - 8 + j - 2,tutorialBode[i].position.y,tutorialBode[i].position.z - 5 - k * 10 }, WritingModel.get());
                            writing.push_back(move(temp));
                        }
                    }
                }
                if (boss->IsImotal())
                {
                    tutorialSucces |= 0b00001;
                }
                break;
            case 6:
                enemys_7tutorial.clear();
                if ((input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A)) && tutorialSucces == 0b11111)
                {
                    sceneEnd = true;
                }
                break;
            default:
                break;
            }
            break;
        }
        enemys_6tutorial.remove_if([](auto& itr) {return itr->deleteFlag; });
        enemys_7tutorial.remove_if([](auto& itr) {return itr->deleteFlag; });
    }
    substie->Update(false);
    boss->Update(false);
    boss->position = { 90,0,-15 };
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
                itr->position.x = 38.f;
                itr->position.z = -2.5f;
                break;
            case 2:
                itr->position.x = 45.f;
                itr->position.z = -2.5f;
                break;
            case 3:
                itr->position.x = 52.f;
                itr->position.z = -2.5f;
                break;
            case 4:
                itr->position.x = 41.5f;
                itr->position.z = -15.f;
                break;
            case 5:
                itr->position.x = 48.5f;
                itr->position.z = -15.0f;
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
                itr->position.x = 98.f;
                itr->position.z = -2.5f;
                break;
            case 2:
                itr->position.x = 105.f;
                itr->position.z = -2.5f;
                break;
            case 3:
                itr->position.x = 112.f;
                itr->position.z = -2.5f;
                break;
            case 4:
                itr->position.x = 101.5f;
                itr->position.z = -15.f;
                break;
            case 5:
                itr->position.x = 108.5f;
                itr->position.z = -15.0f;
                break;
            default:
                break;
            }
        }
        i++;
    }
    if (substie->position.x < tutorialBode[2].position.x - 30)
    {
        substie->SetShard(0);
    }
    MatrixUpdate();
    if (input->IsKeyTrigger(DIK_P) || input->IsKeyTrigger(DIK_RETURN)  || input->gamePad->IsButtonTrigger(GAMEPAD_START))
    {
        sceneEnd = true;
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
    for (auto& itr : tutorialBode)itr.Draw();
}

void MCB::TitleScene::SpriteDraw()
{
    titleSprite.SpriteDraw(dxWindow->window_width / 2 - (substie->position.x + 210) * 30, dxWindow->window_height / 2 - 80);
    //pushSpaceSprite.SpriteDraw(dxWindow->window_width / 2 - substie->position.x * 30, dxWindow->window_height / 2 + 40 + substie->position.z * 30);
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
    imgui.Begin();
    ImGui::Begin("Debug");
    /*if (ImGui::TreeNode("Tutorial"))
    {
        if (ImGui::TreeNode("Position"))
        {
            ImGui::SliderFloat("x", &tutorialBode.position.x, -100, 100);
            ImGui::SliderFloat("y", &tutorialBode.position.y, -100, 100);
            ImGui::SliderFloat("z", &tutorialBode.position.z, -100, 100);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Scale"))
        {
            ImGui::SliderFloat("x", &tutorialBode.scale.x, 0, 10);
            ImGui::SliderFloat("y", &tutorialBode.scale.y, 0, 10);
            ImGui::SliderFloat("z", &tutorialBode.scale.z, 0, 10);
            ImGui::TreePop();
        }
    }*/
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
            ImGui::Text("shard %f", substie->GetShard(), 0, 20);
            ImGui::SliderFloat("shardCost %f", &substie->shardCost, 0, 20);
            ImGui::SliderFloat("shardRotaCost %f", &substie->shardRotateCost, 0, 20);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::End();
    imgui.End();
}

MCB::TitleScene::TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline)
{
	rootparamsPtr = root;
	this->depth = depth;
    this->pipeline = pipeline;
}

MCB::TitleScene::~TitleScene()
{
    soundManager.AllDeleteSound();
    delete nextScene;
    title->free = true;
    debugTextTexture->free = true;
    pushSpace->free = true;
    scopeTex->free = true;
}

void MCB::TitleScene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 3.0f, -10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    camera.Inilialize();
    viewCamera = &camera;
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
}

void MCB::TitleScene::LoadTexture()
{
    debugTextTexture = loader->LoadTexture(L"Resources\\debugfont.png");
    title = loader->LoadTexture(L"Resources\\text\\title.png");
    pushSpace = loader->LoadTexture(L"Resources\\text\\pushSpace.png");
    scopeTex = loader->LoadTexture(L"Resources\\scope.png");
}

void MCB::TitleScene::LoadSound()
{
    testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
    test2Sound = soundManager.LoadWaveSound("Resources\\fanfare.wav");
    soundManager.SetVolume(100, testSound);
}

void MCB::TitleScene::Object3DInit()
{


    ground;
    ground.Init();
    ground.model = groundModel.get();
    ground.scale = { 181,90,181 };
    ground.position = { 0,-20,0 };

    Skydome;
    Skydome.Init();
    Skydome.model = skydomeModel.get();
    Skydome.scale = { 280,1,280 };
    Skydome.position = { 0,-25,0 };

    testSpher.Init();
    testSpher.model = BoxModel.get();
    testSpher.scale = { 3,3,3 };
    testSpher.position = { 0,4,10 };

    substie->model = playerModel.get();
    substie->KneadedEraserModel = nerikesiModel.get();
    substie->scale = { 1,1,1 };
    substie->position = { -210,0,0 };
    substie->Initialize();

    boss->Initialize({ 0,0,1 }, { -20,0,0 }, bossModel.get(), pencilEnemyModel.get(), WritingModel.get(), BossDamegeEffectModelStar.get(), BossDamegeEffectModelSpher.get(), 1, substie.get());
    boss->shake = camera.GetShakePtr();
    boss->position = { 90,0,-15 };
    //tutorialBode.Init();
    //tutorialBode.model = tutorialBlackBode.get();
    //tutorialBode.scale = { 6,6,6 };
    //tutorialBode.position = { 10,0,0 };
    for (int i = 0; i < 8; i++)
    {
        tutorialBode[i].Init();
        tutorialBode[i].model = tutorialBlackBode.get();
        tutorialBode[i].scale = { 1,1,1 };
        if (i < 2)continue;
        tutorialBode[i].position = { 60.f + 60.f * (i - 2) - 210 + 15,0,8.4f};

    }



    tutorialBode[0].position = { -15.f -210,0,8.4f };
    tutorialBode[1].position = { 15.f - 210,0,8.4f };
    camera.player = substie.get();
}
