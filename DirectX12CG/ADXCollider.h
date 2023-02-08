#pragma once
#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <vector>
#include <list>


using namespace MCB;

namespace MCB
{
    class Object3d;
}

enum colType
{
    box,
    sphere,
};

struct collidePattern
{
    int layer1;
    int layer2;
};

class ADXCollider
{
private:
    static std::vector<ADXCollider*> allColPtr;

public:
    static void CollidersUpdate();

protected:
    ADXVector3 EdgeLocalPoint(ADXVector3 pos);
    ADXVector3 EdgeLocalPoint(ADXVector3 pos, ADXVector3 prePos);

public:
    bool enabled = true;
    bool isTrigger = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    Object3d* gameObject = nullptr;
    float radius_ = 1;
    ADXVector3 pos_ = { 0,0,0 };
    ADXVector3 scale_ = { 1,1,1 };
    std::vector<ADXCollider*> collideList{};
    ADXVector3 pushBackVector = { 0,0,0 };
    float pushBackPriority = 0;
    int collideLayer = 0;

    ADXCollider(Object3d* obj);
    void Initialize(Object3d* obj);
    void Update(Object3d* obj);
    ADXVector3 ClosestPoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos, ADXVector3 prePos);
    ADXVector3 CollidePoint(ADXVector3 pos, ADXVector3 colSenter, ADXVector3 move);
    ADXVector3 CollideVector(ADXCollider col);
    
    bool IsHit(ADXCollider col);
    void SendPushBack();
    void Collide(ADXCollider* col);

private:
    ADXVector3 preTranslation;
    ADXMatrix4 preMatrix;

private:
    static bool translateDivine;
    static std::vector<collidePattern> ignoreCollidePatterns;
    static std::vector<collidePattern> ignorePushBackPatterns;
};