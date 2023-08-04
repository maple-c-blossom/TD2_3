#include "ADXCollider.h"
#include "Object3d.h"
#include "IScene.h"

using namespace MCB;

std::vector<collidePattern> ADXCollider::S_ignoreCollidePatterns = { {1,1},{2,2},{2,3},{3,3},{-1,1} };
std::vector<collidePattern> ADXCollider::S_ignorePushBackPatterns = {};

/*
�y�ڐG���m���牟���߂��x�N�g���Z�o�܂ł̗���z

�E��ԏ�̈�̓_����̐}�`�̒��ɂ��邩�𒲂ׂ�i��̗��̐}�`�������_�����L���Ă��邩�𒲂ׂ�j
����ɂ���̐}�`���ڐG���Ă��邩�𔻒肷��

�E�������Ă���Ɣ��肳�ꂽ�炻�ꂼ��̃I�u�W�F�N�g�Łw�Փ˓_�x�����߂�

�E�݂��̃I�u�W�F�N�g�̏Փ˓_�̍��W�̍��������߂��̕����Ƌ������������x�N�g���ɂȂ�
*/

std::list<ADXCollider*> ADXCollider::S_cols = {};

ADXCollider::ADXCollider(Object3d* obj)
{
	gameObject = obj;
	UniqueInitialize();
}

void ADXCollider::UniqueInitialize()
{
	preTranslation = { gameObject->position_.x,gameObject->position_.y,gameObject->position_.z };
	preMatrix = gameObject->transform.GetMatWorld();
	preMatrixInverse = gameObject->transform.GetMatWorldInverse();
}

void ADXCollider::UniqueUpdate()
{
	S_cols.push_back(this);
}

//��ԏ�̓_���R���C�_�[�̒��Ɏ��߂����̍��W
ADXVector3 ADXCollider::ClosestPoint(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::transform(pos, gameObject->transform.GetMatWorldInverse());
	ADXVector3 closPos = ret;

	switch (colType_)
	{
	case box:
		if (closPos.x > pos_.x + scale_.x)
		{
			closPos.x = pos_.x + scale_.x;
		}
		else if (closPos.x < pos_.x - scale_.x)
		{
			closPos.x = pos_.x - scale_.x;
		}

		if (closPos.y > pos_.y + scale_.y)
		{
			closPos.y = pos_.y + scale_.y;
		}
		else if (closPos.y < pos_.y - scale_.y)
		{
			closPos.y = pos_.y - scale_.y;
		}

		if (closPos.z > pos_.z + scale_.z)
		{
			closPos.z = pos_.z + scale_.z;
		}
		else if (closPos.z < pos_.z - scale_.z)
		{
			closPos.z = pos_.z - scale_.z;
		}
		break;
	case sphere:
		if ((closPos - pos_).Length() > radius_)
		{
			closPos = pos_ + (closPos - pos_).Normalize() * radius_;
		}
		break;
	case plain:
		closPos.y = 0;
		break;
	case quad:
		if (closPos.x > pos_.x + scale_.x)
		{
			closPos.x = pos_.x + scale_.x;
		}
		else if (closPos.x < pos_.x - scale_.x)
		{
			closPos.x = pos_.x - scale_.x;
		}

		if (closPos.z > pos_.z + scale_.z)
		{
			closPos.z = pos_.z + scale_.z;
		}
		else if (closPos.z < pos_.z - scale_.z)
		{
			closPos.z = pos_.z - scale_.z;
		}

		closPos.y = 0;
		break;
	default:
		closPos = pos_;
		break;
	}

	if ((closPos - ret).Length() > 0)
	{
		ret = closPos;
		ret = ADXMatrix4::transform(ret, gameObject->transform.GetMatWorld());
	}
	else
	{
		ret = pos;
	}

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
ADXVector3 ADXCollider::EdgeLocalPoint(const ADXVector3& pos) const
{
	return EdgeLocalPoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
ADXVector3 ADXCollider::EdgeLocalPoint(const ADXVector3& pos, const ADXVector3& prePos) const
{
	ADXVector3 ret = ADXMatrix4::transform(pos, gameObject->transform.GetMatWorldInverse());
	ret -= pos_;

	ADXVector3 prevPos = ADXMatrix4::transform(prePos, preMatrixInverse);
	prevPos -= pos_;

	ADXVector3 absLocalPos{};

	switch (colType_)
	{
	case box:
		ret.x /= scale_.x;
		ret.y /= scale_.y;
		ret.z /= scale_.z;

		prevPos.x /= scale_.x;
		prevPos.y /= scale_.y;
		prevPos.z /= scale_.z;

		absLocalPos = prevPos;
		if (absLocalPos.x < 0)
		{
			absLocalPos.x = -absLocalPos.x;
		}
		if (absLocalPos.y < 0)
		{
			absLocalPos.y = -absLocalPos.y;
		}
		if (absLocalPos.z < 0)
		{
			absLocalPos.z = -absLocalPos.z;
		}

		if (absLocalPos.z > absLocalPos.x && absLocalPos.z > absLocalPos.y)
		{
			if (ret.z > 0)
			{
				ret.z = 1;
			}
			else
			{
				ret.z = -1;
			}
		}
		else
		{
			if (absLocalPos.x > absLocalPos.y)
			{
				if (ret.x > 0)
				{
					ret.x = 1;
				}
				else
				{
					ret.x = -1;
				}
			}
			else
			{
				if (ret.y > 0)
				{
					ret.y = 1;
				}
				else
				{
					ret.y = -1;
				}
			}
		}

		ret.x *= scale_.x;
		ret.y *= scale_.y;
		ret.z *= scale_.z;
		break;
	case sphere:
		ret = ret.Normalize() * radius_;
		break;
	case plain:
		ret.y = 0;
		break;
	case quad:
		if (ret.x > pos_.x + scale_.x)
		{
			ret.x = pos_.x + scale_.x;
		}
		else if (ret.x < pos_.x - scale_.x)
		{
			ret.x = pos_.x - scale_.x;
		}

		if (ret.z > pos_.z + scale_.z)
		{
			ret.z = pos_.z + scale_.z;
		}
		else if (ret.z < pos_.z - scale_.z)
		{
			ret.z = pos_.z - scale_.z;
		}

		ret.y = 0;
		break;
	default:
		ret = { 0,0,0 };
		break;
	}

	ret += pos_;
	ret = ADXMatrix4::transform(ClosestPoint(ADXMatrix4::transform(ret, gameObject->transform.GetMatWorld())), gameObject->transform.GetMatWorldInverse());

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(const ADXVector3& pos)
{
	return EdgePoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(const ADXVector3& pos, const ADXVector3& prePos)
{
	ADXVector3 ret = EdgeLocalPoint(pos, prePos);
	ret = ADXMatrix4::transform(ret, gameObject->transform.GetMatWorld());
	return ret;
}

//����̃R���C�_�[�Ƃ̏Փ˓_�̍��W
ADXVector3 ADXCollider::CollidePoint(const ADXVector3& pos, const ADXVector3& targetColSenter, const ADXVector3& move) const
{
	ADXVector3 ret = EdgeLocalPoint(pos, pos - move);
	ADXVector3 targetLocalSenter = ADXMatrix4::transform(targetColSenter, gameObject->transform.GetMatWorldInverse()) - pos_;

	if (targetLocalSenter.Dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = ADXMatrix4::transform(ret, gameObject->transform.GetMatWorld());
	return ret;
}

//�����Ԃ������Ƌ����̃x�N�g��
ADXVector3 ADXCollider::CollideVector(const ADXCollider& col)
{
	ADXVector3 ret;

	ADXVector3 myTranslation = ADXMatrix4::transform(pos_, gameObject->transform.GetMatWorld());
	ADXVector3 myMove = myTranslation - ADXMatrix4::transform(pos_, preMatrix);

	ADXVector3 targetTranslation = ADXMatrix4::transform(col.pos_, col.gameObject->transform.GetMatWorld());
	ADXVector3 targetMove = targetTranslation - ADXMatrix4::transform(col.pos_, col.preMatrix);

	ADXVector3 myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	ADXVector3 myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	ADXVector3 pushBackDiff = myPushBack1 - myPushBack2;

	if ((targetTranslation - myTranslation).Dot(pushBackDiff) > 0)
	{
		ret = myPushBack2;
	}
	else
	{
		ret = myPushBack1;
	}

	if (ret.Dot(targetTranslation - myTranslation) > 0)
	{
		ret = -ret;
	}

	return ret;
}

//����̃R���C�_�[�Əd�Ȃ��Ă��邩
bool ADXCollider::IsHit(const ADXCollider& col)
{
	ADXVector3 closestVec1 = col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, col.gameObject->transform.GetMatWorld())));
	ADXVector3 closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, col.gameObject->transform.GetMatWorld()))));
	float colPointDiff = (closestVec1 - closestVec2).Length();
	if ((closestVec1 - closestVec2).Length() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, gameObject->transform.GetMatWorld())));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, gameObject->transform.GetMatWorld()))));
	colPointDiff = (closestVec1 - closestVec2).Length();
	if ((closestVec1 - closestVec2).Length() <= 0)
	{
		return true;
	}

	return false;
}

//�R���C�_�[���m�ŉ��������i�����Ȃ��R���C�_�[�ɂԂ����������I�ɉ������j
void ADXCollider::Collide(ADXCollider* col)
{
	for (auto& itr : S_ignoreCollidePatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			return;
		}
	}

	bool executePushBack = true;
	for (auto& itr : S_ignorePushBackPatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			executePushBack = false;
		}
	}

	if (enabled && col->enabled && col->gameObject != gameObject && IsHit(*col))
	{
		if (executePushBack && !isTrigger && !col->isTrigger)
		{
			ADXVector3 myPushBack = CollideVector(*col);
			ADXVector3 targetPushBack = col->CollideVector(*this);

			int32_t conditionState[4][3] = { {0,0,0},{1,1,1},{2,2,2},{3,1,2} };

			int32_t pushableCondition = 0;
			int32_t priorityCondition = 0;

			if (pushable_ && col->pushable_)
			{
				pushableCondition = 3;
			}
			else if (pushable_)
			{
				pushableCondition = 1;
			}
			else if (col->pushable_)
			{
				pushableCondition = 2;
			}

			if (pushBackPriority == col->pushBackPriority)
			{
				priorityCondition = 0;
			}
			else if (pushBackPriority < col->pushBackPriority)
			{
				priorityCondition = 1;
			}
			else if (pushBackPriority > col->pushBackPriority)
			{
				priorityCondition = 2;
			}

			int32_t ConditionStateResult = conditionState[pushableCondition][priorityCondition];

			switch (ConditionStateResult)
			{
			case 1:
				pushBackVector += myPushBack;
				break;
			case 2:
				col->pushBackVector += targetPushBack;
				break;
			case 3:
				pushBackVector += myPushBack * 0.5f;
				col->pushBackVector += targetPushBack * 0.5f;
				break;
			default:
				break;
			}
		}

		collideList.push_back(col);
		col->collideList.push_back(this);
	}
}

//���CollidersUpdate�ŕʂ̃R���C�_�[�ɂԂ����Ă�����I�u�W�F�N�g�������߂�
void ADXCollider::SendPushBack()
{
	if (pushable_)
	{
		gameObject->transform.localPosition_ += pushBackVector;
		gameObject->transform.UpdateMatrix();
	}
	preTranslation = gameObject->transform.localPosition_;
	preMatrix = gameObject->transform.GetMatWorld();
	preMatrixInverse = gameObject->transform.GetMatWorldInverse();
	pushBackVector = { 0,0,0 };
}

//�S�ẴR���C�_�[�ŐڐG����Ɖ����߂��x�N�g���̎Z�o���s��
//�Q�[�����̑S�ẴR���C�_�[���������z������Ďg��
void ADXCollider::StaticUpdate()
{
	//���݂̍��W��ۑ����Ă���
	std::vector<ADXVector3> objsTranslation = {};
	for (int32_t i = 0; i < Object3d::GetAllObjs().size(); i++)
	{
		objsTranslation.push_back(Object3d::GetAllObjs()[i]->transform.localPosition_);
	}

	//���ׂẴR���C�_�[�ňړ�������(�ŏ���Δ��a�~0.95)�����߁A�ł��傫���l��translateDivNumF�ɓ����
	float translateDivNumF = 1;
	for (auto& colItr : S_cols)
	{
		//���ł�collideList�����̃^�C�~���O�Ń��Z�b�g
		colItr->collideList.clear();


		ADXVector3 move = colItr->gameObject->transform.localPosition_ - colItr->preTranslation;

		ADXVector3 scaleX1 = { colItr->scale_.x,0,0 };
		ADXVector3 scaleY1 = { 0,colItr->scale_.y,0 };
		ADXVector3 scaleZ1 = { 0,0,colItr->scale_.z };

		ADXMatrix4 WorldScalingMat = colItr->gameObject->transform.GetMatScale();
		WorldScalingMat *= colItr->gameObject->transform.GetMatRot();

		float worldScaleX1 = ADXMatrix4::transform(scaleX1, WorldScalingMat).Length();
		float worldScaleY1 = ADXMatrix4::transform(scaleY1, WorldScalingMat).Length();
		float worldScaleZ1 = ADXMatrix4::transform(scaleZ1, WorldScalingMat).Length();

		float minimumWorldRadius1 = 1;

		if (worldScaleX1 < worldScaleY1 && worldScaleX1 < worldScaleZ1 && worldScaleX1 > 0)
		{
			minimumWorldRadius1 = worldScaleX1;
		}
		else if (worldScaleY1 < worldScaleZ1 && worldScaleY1 > 0)
		{
			minimumWorldRadius1 = worldScaleY1;
		}
		else if (worldScaleZ1 > 0)
		{
			minimumWorldRadius1 = worldScaleZ1;
		}

		float moveDivnum1 = move.Length() / (minimumWorldRadius1 * 0.95f);
		if (moveDivnum1 >= translateDivNumF)
		{
			translateDivNumF = moveDivnum1;
		}
	}
	translateDivNumF = ceilf(translateDivNumF);

	//�S�ẴI�u�W�F�N�g���ړ�����O�̍��W�ֈړ�������
	for (auto& colItr : S_cols)
	{
		colItr->gameObject->transform.localPosition_ = colItr->preTranslation;
	}

	//�s��X�V�̂��łɈړ�����O�̍��W��ۑ�
	std::vector<ADXVector3> objsPreTranslation = {};
	for (auto& objItr : Object3d::GetAllObjs())
	{
		objsPreTranslation.push_back(objItr->transform.localPosition_);
		objItr->transform.UpdateMatrix();
	}

	//�����Âړ������Ȃ��瓖���蔻��Ɖ����߂��������s��
	for (int32_t i = 0; i < translateDivNumF; i++)
	{
		//�ړ�
		for (int32_t j = 0; j < Object3d::GetAllObjs().size(); j++)
		{
			ADXVector3 move = objsTranslation[j] - objsPreTranslation[j];

			Object3d::GetAllObjs()[j]->transform.localPosition_ += move / translateDivNumF;
			Object3d::GetAllObjs()[j]->transform.UpdateMatrix();
		}

		//�����蔻��Ɖ����߂��x�N�g���̎Z�o
		for (auto& colItr1 : S_cols)
		{
			for (auto& colItr2 : S_cols)
			{
				if (&colItr1 != &colItr2)
				{
					colItr1->Collide(colItr2);
					colItr1->SendPushBack();
					colItr2->SendPushBack();
				}
			}
		}
	}

	S_cols.clear();
}