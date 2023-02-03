#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "Vector3D.h"
#include "Descriptor.h"
#include <vector>
#include "Model.h"
#include "Quaternion.h"
#include "LightGroup.h"
#include "FBXLoader.h"
#include <list>

#include"ADXCollider.h"

namespace MCB
{
    class Object3d
    {
    public:

        //定数バッファ用構造体(行列)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float4 color;
            Float3 cameraPos;

        };
        //---------------------------------

        static LightGroup* lights;

        //行列用定数バッファ
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //行列用定数バッファマップ
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //アフィン変換情報
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
        Float4 color = { 1.0f, 1.0f, 1.0f,1.0f };
        Quaternion quaternion = { 0.f,0.f,0.f,1.f };
        //ワールド行列
        WorldMatrix matWorld = {};

        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {0,0,1};
        float frontAngle = 0;
        bool deleteFlag = false;
        Model* model = nullptr;
        FBXModel* fbxModel = nullptr;

        Object3d();

        ~Object3d();

        //親オブジェクトへのポインタ
        Object3d* parent = nullptr;

        bool trackingFlag = false;

        std::vector<ADXCollider> colliders{};

        void Init();

        void Update(View& view, Projection& projection, bool isBillBord = false);

        void Update(View& view, Projection& projection, Quaternion q, bool isBillBord = false);

        void UpdateData();

        void Draw();

        void Draw(unsigned short int incremant);
        void Draw(ObjectMaterial* material);


        void FbxUpdate(View& view, Projection& projection, bool isBillBord = false);

        void FbxUpdate(View& view, Projection& projection, Quaternion q, bool isBillBord = false);

        void FbxDraw();

        void FbxDraw(unsigned short int incremant);

        void OnColliderHit(ADXCollider* myCol, ADXCollider* col);

        //【ADXEngine由来】静的更新処理
        static void StaticUpdate();

        static void SetLights(LightGroup* light);
        //void CreateModel(const char* fileName);

        //【ADXEngine由来】
        static inline std::vector<Object3d*> GetAllObjs() { return objs; };

        inline int TestCall() { return 1; };

        static inline bool IsValid(Object3d* objPtr)
        {
            return objPtr->deleteFlag == false && objPtr->constMapTranceform != nullptr;
        };

        static inline bool DeleteAllowed(Object3d* objPtr)
        {
            return !IsValid(objPtr) && objPtr->deleteCountDown <= 0;
        }

    private:
        int deleteCountDown = 3;

    private:
        // 【ADXEngine由来】全てのオブジェクトを入れる配列
        static std::vector<Object3d*> allObjPtr;
        // 【ADXEngine由来】全てのオブジェクトが入った配列
        static std::vector<Object3d*> objs;

    protected:
        virtual void UniqueOnColliderHit(ADXCollider* myCol, ADXCollider* col);
    };
}

