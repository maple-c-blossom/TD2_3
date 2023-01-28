#include "ADXCollider.h"
#include "Object3d.h"
#include "IScene.h"

using namespace MCB;

bool ADXCollider::translateDivine = false;
std::vector<collidePattern> ADXCollider::ignoreCollidePatterns = { {1,1},{2,2},{2,3},{3,3},{-1,1} };
std::vector<collidePattern> ADXCollider::ignorePushBackPatterns = {};

/*
�y�ڐG���m���牟���߂��x�N�g���Z�o�܂ł̗���z

�E��ԏ�̈�̓_����̐}�`�̒��ɂ��邩�𒲂ׂ�i��̗��̐}�`�������_�����L���Ă��邩�𒲂ׂ�j
����ɂ���̐}�`���ڐG���Ă��邩�𔻒肷��

�E�������Ă���Ɣ��肳�ꂽ�炻�ꂼ��̃I�u�W�F�N�g�Łw�Փ˓_�x�����߂�

�E�݂��̃I�u�W�F�N�g�̏Փ˓_�̍��W�̍��������߂��̕����Ƌ������������x�N�g���ɂȂ�
*/

std::vector<ADXCollider*> ADXCollider::allColPtr = {};

ADXCollider::ADXCollider(Object3d* obj)
{
	Initialize(obj);
}

void ADXCollider::Initialize(Object3d* obj)
{
	gameObject = obj;
	preTranslation.x = gameObject->position.x;
	preTranslation.y = gameObject->position.y;
	preTranslation.z = gameObject->position.z;
	preMatrix = ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld);
}

void ADXCollider::Update(Object3d* obj)
{
	gameObject = obj;
	allColPtr.push_back(this);
}

//��ԏ�̓_���R���C�_�[�̒��Ɏ��߂����̍��W
ADXVector3 ADXCollider::ClosestPoint(ADXVector3 pos)
{
	ADXVector3 ret = ADXMatrix4::transform(pos, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld).Inverse());
	ADXVector3 closPos = ret;

	if (colType_ == box)
	{
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
	}
	else if (colType_ == sphere)
	{
		if ((closPos - pos_).length() > radius_)
		{
			closPos = pos_ + ADXVector3::normalized(closPos - pos_) * radius_;
		}
	}
	else
	{
		closPos = pos_;
	}

	if ((closPos - ret).length() > 0)
	{
		ret = closPos;
		ret = ADXMatrix4::transform(ret, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld));
	}
	else
	{
		ret = pos;
	}

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
ADXVector3 ADXCollider::EdgeLocalPoint(ADXVector3 pos)
{
	return EdgeLocalPoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
ADXVector3 ADXCollider::EdgeLocalPoint(ADXVector3 pos, ADXVector3 prePos)
{
	ADXVector3 ret = ADXMatrix4::transform(pos, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld).Inverse());
	ret -= pos_;

	ADXVector3 prevPos = ADXMatrix4::transform(prePos, preMatrix.Inverse());
	prevPos -= pos_;

	if (colType_ == box)
	{
		ret.x /= scale_.x;
		ret.y /= scale_.y;
		ret.z /= scale_.z;

		prevPos.x /= scale_.x;
		prevPos.y /= scale_.y;
		prevPos.z /= scale_.z;

		ADXVector3 absLocalPos = prevPos;
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
	}
	else if (colType_ == sphere)
	{
		ret = ADXVector3::normalized(ret) * radius_;
	}
	else
	{
		ret = { 0,0,0 };
	}

	ret += pos_;
	ret = ADXMatrix4::transform(ClosestPoint(ADXMatrix4::transform(ret, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld))), ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld).Inverse());

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos)
{
	return EdgePoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos, ADXVector3 prePos)
{
	ADXVector3 ret = EdgeLocalPoint(pos, prePos);
	ret = ADXMatrix4::transform(ret, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld));
	return ret;
}

//����̃R���C�_�[�Ƃ̏Փ˓_�̍��W
ADXVector3 ADXCollider::CollidePoint(ADXVector3 pos, ADXVector3 targetColSenter, ADXVector3 move)
{
	ADXVector3 ret = EdgeLocalPoint(pos, pos - move);
	ADXVector3 targetLocalSenter = ADXMatrix4::transform(targetColSenter, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld).Inverse()) - pos_;

	if (targetLocalSenter.dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = ADXMatrix4::transform(ret, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld));
	return ret;
}

//�����Ԃ������Ƌ����̃x�N�g��
ADXVector3 ADXCollider::CollideVector(ADXCollider col)
{
	ADXVector3 ret;

	ADXVector3 myTranslation = ADXMatrix4::transform(pos_, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld));
	ADXVector3 myMove = myTranslation - ADXMatrix4::transform(pos_, preMatrix);

	ADXVector3 targetTranslation = ADXMatrix4::transform(col.pos_, ADXMatrix4::ConvertToADXMatrix(col.gameObject->matWorld.matWorld));
	ADXVector3 targetMove = targetTranslation - ADXMatrix4::transform(col.pos_, col.preMatrix);

	ADXVector3 myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	ADXVector3 myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	ADXVector3 pushBackDiff = myPushBack1 - myPushBack2;

	if ((targetTranslation - myTranslation).dot(pushBackDiff) > 0)
	{
		ret = myPushBack2;
	}
	else
	{
		ret = myPushBack1;
	}

	if (ret.dot(targetTranslation - myTranslation) > 0)
	{
		ret = -ret;
	}

	return ret;
}

//����̃R���C�_�[�Əd�Ȃ��Ă��邩
bool ADXCollider::IsHit(ADXCollider col)
{
	ADXVector3 closestVec1 = col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, ADXMatrix4::ConvertToADXMatrix(col.gameObject->matWorld.matWorld))));
	ADXVector3 closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, ADXMatrix4::ConvertToADXMatrix(col.gameObject->matWorld.matWorld)))));
	float colPointDiff = (closestVec1 - closestVec2).length();
	if ((closestVec1 - closestVec2).length() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld))));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld)))));
	colPointDiff = (closestVec1 - closestVec2).length();
	if ((closestVec1 - closestVec2).length() <= 0)
	{
		return true;
	}

	return false;
}

//�R���C�_�[���m�ŉ��������i�����Ȃ��R���C�_�[�ɂԂ����������I�ɉ������j
void ADXCollider::Collide(ADXCollider* col)
{
	for (auto& itr : ignoreCollidePatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			return;
		}
	}

	bool executePushBack = true;
	for (auto& itr : ignorePushBackPatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			executePushBack = false;
		}
	}

	if (Object3d::IsValid(gameObject) && Object3d::IsValid(col->gameObject) && IsHit(*col) && enabled && col->enabled && col->gameObject != gameObject)
	{
		if (executePushBack && !isTrigger && !col->isTrigger)
		{
			ADXVector3 myPushBack = CollideVector(*col);
			ADXVector3 targetPushBack = col->CollideVector(*this);

			if (pushable_ && col->pushable_)
			{
				myPushBack = myPushBack * 0.5f;
				targetPushBack = targetPushBack * 0.5f;
			}

			if (pushable_)
			{
				pushBackVector += myPushBack;
			}
			if (col->pushable_)
			{
				col->pushBackVector += targetPushBack;
			}
		}

		collideList.push_back(col);
		col->collideList.push_back(this);

		gameObject->OnColliderHit(this, col);
		col->gameObject->OnColliderHit(col, this);
	}
}

//���CollidersUpdate�ŕʂ̃R���C�_�[�ɂԂ����Ă�����I�u�W�F�N�g�������߂�
void ADXCollider::SendPushBack()
{
	if(gameObject != nullptr)
	{
		gameObject->position.x += pushBackVector.x;
		gameObject->position.y += pushBackVector.y;
		gameObject->position.z += pushBackVector.z;
		gameObject->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
		preTranslation.x = gameObject->position.x;
		preTranslation.y = gameObject->position.y;
		preTranslation.z = gameObject->position.z;
		preMatrix = ADXMatrix4::ConvertToADXMatrix(gameObject->matWorld.matWorld);
		pushBackVector = { 0,0,0 };
	}
}

//�S�ẴR���C�_�[�ŐڐG����Ɖ����߂��x�N�g���̎Z�o���s��
//�Q�[�����̑S�ẴR���C�_�[���������z������Ďg��
void ADXCollider::CollidersUpdate()
{
	for (int i = 0; i < allColPtr.size(); i++)
	{
		allColPtr[i]->collideList.clear();
	}

	if (translateDivine)
	{
		//���݂̍��W��ۑ����Ă���
		std::vector<ADXVector3> objsTranslation = {};
		for (auto& itr : Object3d::GetAllObjs())
		{
			objsTranslation.push_back({ itr->position.x,itr->position.y,itr->position.z });
		}

		//���ׂẴR���C�_�[�ňړ�������(�ŏ���Δ��a�~0.95)�����߁A�ł��傫���l��translateDivNumF�ɓ����
		float translateDivNumF = 1;
		for (int i = 0; i < allColPtr.size(); i++)
		{
			ADXVector3 move;
			move.x = allColPtr[i]->gameObject->position.x - allColPtr[i]->preTranslation.x;
			move.y = allColPtr[i]->gameObject->position.y - allColPtr[i]->preTranslation.y;
			move.z = allColPtr[i]->gameObject->position.z - allColPtr[i]->preTranslation.z;

			ADXVector3 scaleX1 = { allColPtr[i]->scale_.x,0,0 };
			ADXVector3 scaleY1 = { 0,allColPtr[i]->scale_.y,0 };
			ADXVector3 scaleZ1 = { 0,0,allColPtr[i]->scale_.z };

			float worldScaleX1 = ADXMatrix4::transform(scaleX1, ADXMatrix4::ConvertToADXMatrix(allColPtr[i]->gameObject->matWorld.matScale * allColPtr[i]->gameObject->matWorld.matRot)).length();
			float worldScaleY1 = ADXMatrix4::transform(scaleY1, ADXMatrix4::ConvertToADXMatrix(allColPtr[i]->gameObject->matWorld.matScale * allColPtr[i]->gameObject->matWorld.matRot)).length();
			float worldScaleZ1 = ADXMatrix4::transform(scaleZ1, ADXMatrix4::ConvertToADXMatrix(allColPtr[i]->gameObject->matWorld.matScale * allColPtr[i]->gameObject->matWorld.matRot)).length();

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

			float moveDivnum1 = move.length() / (minimumWorldRadius1 * 0.95);
			if (moveDivnum1 >= translateDivNumF)
			{
				translateDivNumF = moveDivnum1;
			}
		}
		translateDivNumF = ceilf(translateDivNumF);

		//�S�ẴI�u�W�F�N�g���ړ�����O�̍��W�ֈړ�������
		for (int i = 0; i < allColPtr.size(); i++)
		{
			allColPtr[i]->gameObject->position.x = allColPtr[i]->preTranslation.x;
			allColPtr[i]->gameObject->position.y = allColPtr[i]->preTranslation.y;
			allColPtr[i]->gameObject->position.z = allColPtr[i]->preTranslation.z;
		}

		//�s��X�V�̂��łɈړ�����O�̍��W��ۑ�
		std::vector<ADXVector3> objsPreTranslation = {};
		for (auto& itr : Object3d::GetAllObjs())
		{
			objsPreTranslation.push_back({ itr->position.x,itr->position.y,itr->position.z });
			itr->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
		}

		//�����Âړ������Ȃ��瓖���蔻��Ɖ����߂��������s��
		for (int i = 0; i < translateDivNumF; i++)
		{
			//�ړ�
			int j = 0;
			for (auto& itr : Object3d::GetAllObjs())
			{
				ADXVector3 move = objsTranslation[j] - objsPreTranslation[j];

				itr->position.x += move.x / translateDivNumF;
				itr->position.y += move.y / translateDivNumF;
				itr->position.z += move.z / translateDivNumF;
				itr->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
				j++;
			}

			//�����蔻��Ɖ����߂��x�N�g���̎Z�o
			for (int j = 0; j < allColPtr.size(); j++)
			{
				for (int k = j + 1; k < allColPtr.size(); k++)
				{
					allColPtr[j]->Collide(allColPtr[k]);
					allColPtr[j]->SendPushBack();
					allColPtr[k]->SendPushBack();
				}
			}
		}
	}
	else
	{
		//�����蔻��Ɖ����߂��x�N�g���̎Z�o
		for (int i = 0; i < allColPtr.size(); i++)
		{
			for (int j = i + 1; j < allColPtr.size(); j++)
			{
				allColPtr[i]->Collide(allColPtr[j]);
				allColPtr[i]->SendPushBack();
				allColPtr[j]->SendPushBack();
			}
		}
	}

	allColPtr.clear();
}