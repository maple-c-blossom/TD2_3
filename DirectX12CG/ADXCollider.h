#pragma once
#include "Vector3D.h"
#include "MCBMatrix.h"
#include <vector>

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

class ADXCollider
{
private:
    static std::vector<ADXCollider*> cols;

public:
    static void CollidersUpdate();

protected:
    Vector3D EdgeLocalPoint(Vector3D pos);
    Vector3D EdgeLocalPoint(Vector3D pos, Vector3D prePos);

public:
    bool enabled = true;
    bool isTrigger = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    Object3d* gameObject = nullptr;
    float radius_ = 1;
    Vector3D pos_ = { 0,0,0 };
    Vector3D scale_ = { 1,1,1 };
    std::vector<ADXCollider*> collideList{};
    Vector3D pushBackVector = { 0,0,0 };

    ADXCollider(Object3d* obj);
    void Update(Object3d* obj);
    Vector3D ClosestPoint(Vector3D pos);
    Vector3D EdgePoint(Vector3D pos);
    Vector3D EdgePoint(Vector3D pos, Vector3D prePos);
    Vector3D CollidePoint(Vector3D pos, Vector3D colSenter, Vector3D move);
    Vector3D CollideVector(ADXCollider col);
    
    bool IsHit(ADXCollider col);
    void SendPushBack();
    void Collide(ADXCollider* col);

private:
    Vector3D preTranslation;
    MCBMatrix preMatrix;
};