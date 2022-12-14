#include "ADXCollider.h"
#include "Object3d.h"

using namespace MCB;

/*
�y�ڐG���m���牟���߂��x�N�g���Z�o�܂ł̗���z

�E��ԏ�̈�̓_����̐}�`�̒��ɂ��邩�𒲂ׂ�i��̗��̐}�`�������_�����L���Ă��邩�𒲂ׂ�j
����ɂ���̐}�`���ڐG���Ă��邩�𔻒肷��

�E�������Ă���Ɣ��肳�ꂽ�炻�ꂼ��̃I�u�W�F�N�g�Łw�Փ˓_�x�����߂�

�E�݂��̃I�u�W�F�N�g�̏Փ˓_�̍��W�̍��������߂��̕����Ƌ������������x�N�g���ɂȂ�
*/

std::vector<ADXCollider*> ADXCollider::cols = {};

ADXCollider::ADXCollider(Object3d* obj)
{
	gameObject = obj;
	preTranslation = gameObject->position;
	preMatrix = MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform);
}

void ADXCollider::Update(Object3d* obj)
{
	gameObject = obj;
	cols.push_back(this);
}

//��ԏ�̓_���R���C�_�[�̒��Ɏ��߂����̍��W
Vector3D ADXCollider::ClosestPoint(Vector3D pos)
{
	Vector3D ret = MCBMatrix::transform(pos, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());
	Vector3D closPos = ret;

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
		if ((closPos - pos_).V3Len() > radius_)
		{
			closPos = pos_ + normal(closPos - pos_) * radius_;
		}
	}
	else
	{
		closPos = pos_;
	}

	if ((closPos - ret).V3Len() > 0)
	{
		ret = closPos;
		ret = MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	}
	else
	{
		ret = pos;
	}

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
Vector3D ADXCollider::EdgeLocalPoint(Vector3D pos)
{
	return EdgeLocalPoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
Vector3D ADXCollider::EdgeLocalPoint(Vector3D pos, Vector3D prePos)
{
	Vector3D ret = MCBMatrix::transform(pos, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());
	ret -= pos_;

	Vector3D prevPos = MCBMatrix::transform(prePos, preMatrix.Inverse());
	prevPos -= pos_;

	if (colType_ == box)
	{
		ret.x /= scale_.x;
		ret.y /= scale_.y;
		ret.z /= scale_.z;

		prevPos.x /= scale_.x;
		prevPos.y /= scale_.y;
		prevPos.z /= scale_.z;

		Vector3D absLocalPos = prevPos;
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
		ret = normal(ret) * radius_;
	}
	else
	{
		ret = { 0,0,0 };
	}

	ret += pos_;
	ret = MCBMatrix::transform(ClosestPoint(MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform))), MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());

	return ret;
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
Vector3D ADXCollider::EdgePoint(Vector3D pos)
{
	return EdgePoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
Vector3D ADXCollider::EdgePoint(Vector3D pos, Vector3D prePos)
{
	Vector3D ret = EdgeLocalPoint(pos, prePos);
	ret = MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	return ret;
}

//����̃R���C�_�[�Ƃ̏Փ˓_�̍��W
Vector3D ADXCollider::CollidePoint(Vector3D pos, Vector3D targetColSenter, Vector3D move)
{
	Vector3D ret = EdgeLocalPoint(pos, pos - move);
	Vector3D targetLocalSenter = MCBMatrix::transform(targetColSenter, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse()) - pos_;

	if (targetLocalSenter.dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	return ret;
}

//�����Ԃ������Ƌ����̃x�N�g��
Vector3D ADXCollider::CollideVector(ADXCollider col)
{
	Vector3D ret;

	Vector3D myTranslation = MCBMatrix::transform(pos_, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	Vector3D myMove = myTranslation - MCBMatrix::transform(pos_, preMatrix);

	Vector3D targetTranslation = MCBMatrix::transform(col.pos_, col.MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	Vector3D targetMove = targetTranslation - MCBMatrix::transform(col.pos_, col.preMatrix);

	Vector3D myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	Vector3D myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	Vector3D pushBackDiff = myPushBack1 - myPushBack2;

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
	Vector3D closestVec1 = col.ClosestPoint(ClosestPoint(MCBMatrix::transform(col.pos_, col.MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform))));
	Vector3D closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(MCBMatrix::transform(col.pos_, col.MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform)))));
	float colPointDiff = (closestVec1 - closestVec2).V3Len();
	if ((closestVec1 - closestVec2).V3Len() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(MCBMatrix::transform(pos_, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform))));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(MCBMatrix::transform(pos_, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform)))));
	colPointDiff = (closestVec1 - closestVec2).V3Len();
	if ((closestVec1 - closestVec2).V3Len() <= 0)
	{
		return true;
	}

	return false;
}

//�R���C�_�[���m�ŉ��������i�����Ȃ��R���C�_�[�ɂԂ����������I�ɉ������j
void ADXCollider::Collide(ADXCollider* col)
{
	if (IsHit(*col) && enabled && col->enabled && col->gameObject != gameObject)
	{
		if (!isTrigger && !col->isTrigger)
		{
			Vector3D myPushBack = CollideVector(*col);
			Vector3D targetPushBack = col->CollideVector(*this);

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
	}
}

//���CollidersUpdate�ŕʂ̃R���C�_�[�ɂԂ����Ă�����I�u�W�F�N�g�������߂�
void ADXCollider::SendPushBack()
{
	gameObject->position += pushBackVector;
	gameObject->transform.UpdateMatrix();
	preTranslation = gameObject->position;
	preMatrix = MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform);
	pushBackVector = { 0,0,0 };
}

//�S�ẴR���C�_�[�ŐڐG����Ɖ����߂��x�N�g���̎Z�o���s��
//�Q�[�����̑S�ẴR���C�_�[���������z������Ďg��
void ADXCollider::CollidersUpdate()
{
	//���݂̍��W��ۑ����Ă���
	std::vector<Vector3D> objsTranslation = {};
	for (int i = 0; i < Object3d::GetAllObjs().size(); i++)
	{
		objsTranslation.push_back(Object3d::GetAllObjs()[i]->position);
	}

	//���ׂẴR���C�_�[�ňړ�������(�ŏ���Δ��a�~0.95)�����߁A�ł��傫���l��translateDivNumF�ɓ����
	float translateDivNumF = 1;
	for (int i = 0; i < cols.size(); i++)
	{
		//���ł�collideList�����̃^�C�~���O�Ń��Z�b�g
		cols[i]->collideList.clear();


		Vector3D move = cols[i]->gameObject->position - cols[i]->preTranslation;

		Vector3D scaleX1 = { cols[i]->scale_.x,0,0 };
		Vector3D scaleY1 = { 0,cols[i]->scale_.y,0 };
		Vector3D scaleZ1 = { 0,0,cols[i]->scale_.z };

		float worldScaleX1 = MCBMatrix::transform(scaleX1, cols[i]->gameObject->transform.matScale_ * cols[i]->gameObject->transform.matRot_).V3Len();
		float worldScaleY1 = MCBMatrix::transform(scaleY1, cols[i]->gameObject->transform.matScale_ * cols[i]->gameObject->transform.matRot_).V3Len();
		float worldScaleZ1 = MCBMatrix::transform(scaleZ1, cols[i]->gameObject->transform.matScale_ * cols[i]->gameObject->transform.matRot_).V3Len();

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

		float moveDivnum1 = move.V3Len() / (minimumWorldRadius1 * 0.95);
		if (moveDivnum1 >= translateDivNumF)
		{
			translateDivNumF = moveDivnum1;
		}
	}
	translateDivNumF = ceilf(translateDivNumF);

	//�S�ẴI�u�W�F�N�g���ړ�����O�̍��W�ֈړ�������
	for (int i = 0; i < cols.size(); i++)
	{
		cols[i]->gameObject->position = cols[i]->preTranslation;
	}

	//�s��X�V�̂��łɈړ�����O�̍��W��ۑ�
	std::vector<Vector3D> objsPreTranslation = {};
	for (int i = 0; i < Object3d::GetAllObjs().size(); i++)
	{
		objsPreTranslation.push_back(Object3d::GetAllObjs()[i]->position);
		Object3d::GetAllObjs()[i]->transform.UpdateMatrix();
	}

	//�����Âړ������Ȃ��瓖���蔻��Ɖ����߂��������s��
	for (int i = 0; i < translateDivNumF; i++)
	{
		//�ړ�
		for (int j = 0; j < Object3d::GetAllObjs().size(); j++)
		{
			Vector3D move = objsTranslation[j] - objsPreTranslation[j];

			Object3d::GetAllObjs()[j]->position += move / translateDivNumF;
			Object3d::GetAllObjs()[j]->transform.UpdateMatrix();
		}

		//�����蔻��Ɖ����߂��x�N�g���̎Z�o
		for (int j = 0; j < cols.size(); j++)
		{
			for (int k = j + 1; k < cols.size(); k++)
			{
				cols[j]->Collide(cols[k]);
				cols[j]->SendPushBack();
				cols[k]->SendPushBack();
			}
		}

		//�����߂�
		/*
		for (int j = 0; j < cols.size(); j++)
		{
			cols[j]->SendPushBack();
		}
		*/
	}

	cols.clear();
}