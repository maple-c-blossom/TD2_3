#include "Cleaner.h"
#include "Status.h"
#include "Util.h"

void Cleaner::Inilialize(MCB::Model* model)
{
    position_ = { 20,4,-14 };
    scale_ = { 5.8,4,5.8 };
    rotation_ = { 0,0,0 };
    Init();
    this->model_ = model;
    colliders.push_back(ADXCollider(this));
    colliders.back().colType_ = box;
    moveTime.Set(SECOND_FRAME * 9);
    movePoint[0].vec_ = { -38.f , 0.f , 25.f };
    movePoint[1].vec_ = { 38.f , 0.f , 25.f };
    movePoint[2].vec_ = { -38.f , 0.f , -34.f };
    movePoint[3].vec_ = { 38.f , 0.f , -34.f };
    position_.x = movePoint[0].vec_.x_;
    position_.y = movePoint[0].vec_.y_;
    position_.z = movePoint[0].vec_.z_;
}

void Cleaner::Update()
{
	moveTime.SafeUpdate();
    if (moveTime.NowTime() >= SECOND_FRAME * 2)
    {
        position_.x = Lerp(movePoint[moveindex].vec_.x_, movePoint[moveEndindex].vec_.x_, moveTime.GetEndTime() -  SECOND_FRAME * 2, moveTime.NowTime() - SECOND_FRAME * 2);
        position_.y = Lerp(movePoint[moveindex].vec_.y_, movePoint[moveEndindex].vec_.y_, moveTime.GetEndTime() - SECOND_FRAME * 2, moveTime.NowTime() - SECOND_FRAME * 2);
        position_.z = Lerp(movePoint[moveindex].vec_.z_, movePoint[moveEndindex].vec_.z_, moveTime.GetEndTime() - SECOND_FRAME * 2, moveTime.NowTime() - SECOND_FRAME * 2);
        rotation_.y = ConvertRadius(Lerp(0, 360, moveTime.GetEndTime(), moveTime.NowTime() ));
    }
    if (moveTime.IsEnd())
    {
        moveTime.ReSet();
        moveindex++;
        moveEndindex = moveindex + 1;
        if (movePoint.size() <= moveindex)
        {
            moveindex = 0;
            moveEndindex = 1;
        }
        if (movePoint.size() <= moveEndindex)
        {
            moveEndindex = 0;
        }
    }
}
