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
    tutorialBode.Update(*viewCamera->GetView(), *viewCamera->GetProjection(),true);
    //testSpher.FbxUpdate(*viewCamera->GetView(), *viewCamera->GetProjection(),false);
    substie->UpdateMatrix(viewCamera);

}

void MCB::TitleScene::Update()
{
    substie->Update();

    MatrixUpdate();

    if (input->IsKeyTrigger(DIK_SPACE) || input->gamePad->IsButtonTrigger(GAMEPAD_A))
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
    tutorialBode.Draw();
}

void MCB::TitleScene::SpriteDraw()
{
    titleSprite.SpriteDraw(dxWindow->window_width / 2 - substie->position.x * 30, dxWindow->window_height / 2 - 80);
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
    if (ImGui::TreeNode("Tutorial"))
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
    }
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

    skydomeModel = std::make_unique<Model>("skydome");

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
    ground.scale = { 90,90,90 };
    ground.position = { 0,-20,0 };

    Skydome;
    Skydome.Init();
    Skydome.model = skydomeModel.get();
    Skydome.scale = { 4,4,4 };

    testSpher.Init();
    testSpher.model = BoxModel.get();
    testSpher.scale = { 3,3,3 };
    testSpher.position = { 0,4,10 };

    substie->model = playerModel.get();
    substie->KneadedEraserModel = nerikesiModel.get();
    substie->scale = { 1,1,1 };
    substie->position = { 0,0,0 };
    substie->Initialize();

    tutorialBode.Init();
    tutorialBode.model = tutorialBlackBode.get();
    tutorialBode.scale = { 6,6,6 };
    tutorialBode.position = { 10,0,0 };

    camera.player = substie.get();
}
