#include "ADXCollider.h"
#include "Object3d.h"
#include "IScene.h"

using namespace MCB;

/*
【接触検知から押し戻しベクトル算出までの流れ】

・空間上の一つの点が二つの図形の中にあるかを調べる（二つの立体図形が同じ点を共有しているかを調べる）
これにより二つの図形が接触しているかを判定する

・当たっていると判定されたらそれぞれのオブジェクトで『衝突点』を求める

・互いのオブジェクトの衝突点の座標の差が押し戻しの方向と強さが入ったベクトルになる
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

//空間上の点をコライダーの中に収めた時の座標
Vector3D ADXCollider::ClosestPoint(Vector3D pos)
{
	Vector3D ret = MCBMatrix::transform(pos, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());
	Vector3D closPos = ret;

	if (colType_ == box)
	{
		if (closPos.vec.x > pos_.vec.x + scale_.vec.x)
		{
			closPos.vec.x = pos_.vec.x + scale_.vec.x;
		}
		else if (closPos.vec.x < pos_.vec.x - scale_.vec.x)
		{
			closPos.vec.x = pos_.vec.x - scale_.vec.x;
		}

		if (closPos.vec.y > pos_.vec.y + scale_.vec.y)
		{
			closPos.vec.y = pos_.vec.y + scale_.vec.y;
		}
		else if (closPos.vec.y < pos_.vec.y - scale_.vec.y)
		{
			closPos.vec.y = pos_.vec.y - scale_.vec.y;
		}

		if (closPos.vec.z > pos_.vec.z + scale_.vec.z)
		{
			closPos.vec.z = pos_.vec.z + scale_.vec.z;
		}
		else if (closPos.vec.z < pos_.vec.z - scale_.vec.z)
		{
			closPos.vec.z = pos_.vec.z - scale_.vec.z;
		}
	}
	else if (colType_ == sphere)
	{
		if ((closPos - pos_).V3Len() > radius_)
		{
			closPos = pos_ + Vector3D::normal(closPos - pos_) * radius_;
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

//空間上の点をコライダーのフチに寄せた時の相対座標
Vector3D ADXCollider::EdgeLocalPoint(Vector3D pos)
{
	return EdgeLocalPoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の相対座標
Vector3D ADXCollider::EdgeLocalPoint(Vector3D pos, Vector3D prePos)
{
	Vector3D ret = MCBMatrix::transform(pos, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());
	ret -= pos_;

	Vector3D prevPos = MCBMatrix::transform(prePos, preMatrix.Inverse());
	prevPos -= pos_;

	if (colType_ == box)
	{
		ret.vec.x /= scale_.vec.x;
		ret.vec.y /= scale_.vec.y;
		ret.vec.z /= scale_.vec.z;

		prevPos.vec.x /= scale_.vec.x;
		prevPos.vec.y /= scale_.vec.y;
		prevPos.vec.z /= scale_.vec.z;

		Vector3D absLocalPos = prevPos;
		if (absLocalPos.vec.x < 0)
		{
			absLocalPos.vec.x = -absLocalPos.vec.x;
		}
		if (absLocalPos.vec.y < 0)
		{
			absLocalPos.vec.y = -absLocalPos.vec.y;
		}
		if (absLocalPos.vec.z < 0)
		{
			absLocalPos.vec.z = -absLocalPos.vec.z;
		}

		if (absLocalPos.vec.z > absLocalPos.vec.x && absLocalPos.vec.z > absLocalPos.vec.y)
		{
			if (ret.vec.z > 0)
			{
				ret.vec.z = 1;
			}
			else
			{
				ret.vec.z = -1;
			}
		}
		else
		{
			if (absLocalPos.vec.x > absLocalPos.vec.y)
			{
				if (ret.vec.x > 0)
				{
					ret.vec.x = 1;
				}
				else
				{
					ret.vec.x = -1;
				}
			}
			else
			{
				if (ret.vec.y > 0)
				{
					ret.vec.y = 1;
				}
				else
				{
					ret.vec.y = -1;
				}
			}
		}

		ret.vec.x *= scale_.vec.x;
		ret.vec.y *= scale_.vec.y;
		ret.vec.z *= scale_.vec.z;
	}
	else if (colType_ == sphere)
	{
		ret = Vector3D::normal(ret) * radius_;
	}
	else
	{
		ret.vec = { 0,0,0 };
	}

	ret += pos_;
	ret = MCBMatrix::transform(ClosestPoint(MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform))), MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse());

	return ret;
}

//空間上の点をコライダーのフチに寄せた時の座標
Vector3D ADXCollider::EdgePoint(Vector3D pos)
{
	return EdgePoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の座標
Vector3D ADXCollider::EdgePoint(Vector3D pos, Vector3D prePos)
{
	Vector3D ret = EdgeLocalPoint(pos, prePos);
	ret = MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	return ret;
}

//相手のコライダーとの衝突点の座標
Vector3D ADXCollider::CollidePoint(Vector3D pos, Vector3D targetColSenter, Vector3D move)
{
	Vector3D ret = EdgeLocalPoint(pos, pos - move);
	Vector3D targetLocalSenter = MCBMatrix::transform(targetColSenter, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform).Inverse()) - pos_;

	if (targetLocalSenter.GetV3Dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = MCBMatrix::transform(ret, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	return ret;
}

//押し返す方向と強さのベクトル
Vector3D ADXCollider::CollideVector(ADXCollider col)
{
	Vector3D ret;

	Vector3D myTranslation = MCBMatrix::transform(pos_, MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform));
	Vector3D myMove = myTranslation - MCBMatrix::transform(pos_, preMatrix);

	Vector3D targetTranslation = MCBMatrix::transform(col.pos_, MCBMatrix::MCBMatrixConvertXMMatrix(col.gameObject->matWorld.matTransform));
	Vector3D targetMove = targetTranslation - MCBMatrix::transform(col.pos_, col.preMatrix);

	Vector3D myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	Vector3D myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	Vector3D pushBackDiff = myPushBack1 - myPushBack2;

	if ((targetTranslation - myTranslation).GetV3Dot(pushBackDiff) > 0)
	{
		ret = myPushBack2;
	}
	else
	{
		ret = myPushBack1;
	}

	if (ret.GetV3Dot(targetTranslation - myTranslation) > 0)
	{
		ret = -ret;
	}

	return ret;
}

//相手のコライダーと重なっているか
bool ADXCollider::IsHit(ADXCollider col)
{
	Vector3D closestVec1 = col.ClosestPoint(ClosestPoint(MCBMatrix::transform(col.pos_, MCBMatrix::MCBMatrixConvertXMMatrix(col.gameObject->matWorld.matTransform))));
	Vector3D closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(MCBMatrix::transform(col.pos_, MCBMatrix::MCBMatrixConvertXMMatrix(col.gameObject->matWorld.matTransform)))));
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

//コライダー同士で押し合う（動かないコライダーにぶつかったら一方的に押される）
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

//先のCollidersUpdateで別のコライダーにぶつかっていたらオブジェクトを押し戻す
void ADXCollider::SendPushBack()
{
	gameObject->position.x += pushBackVector.ConvertXMFloat3().x ;
	gameObject->position.y += pushBackVector.ConvertXMFloat3().y ;
	gameObject->position.z += pushBackVector.ConvertXMFloat3().z ;
	gameObject->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
	preTranslation = gameObject->position;
	preMatrix = MCBMatrix::MCBMatrixConvertXMMatrix(gameObject->matWorld.matTransform);
	pushBackVector.vec = { 0,0,0 };
}

//全てのコライダーで接触判定と押し戻しベクトルの算出を行う
//ゲーム内の全てのコライダーが入った配列を入れて使う
void ADXCollider::CollidersUpdate()
{
	//現在の座標を保存しておく
	std::vector<Vector3D> objsTranslation = {};
	for (auto& itr : Object3d::GetAllObjs())
	{
		objsTranslation.push_back({ itr->position.x,itr->position.y,itr->position.z });
	}

	//すべてのコライダーで移動距離÷(最小絶対半径×0.95)を求め、最も大きい値をtranslateDivNumFに入れる
	float translateDivNumF = 1;
	for (int i = 0; i < cols.size(); i++)
	{
		//ついでにcollideListもこのタイミングでリセット
		cols[i]->collideList.clear();


		Vector3D move = cols[i]->gameObject->position - cols[i]->preTranslation;

		Vector3D scaleX1 = { cols[i]->scale_.vec.x,0,0 };
		Vector3D scaleY1 = { 0,cols[i]->scale_.vec.y,0 };
		Vector3D scaleZ1 = { 0,0,cols[i]->scale_.vec.z };

		float worldScaleX1 = MCBMatrix::transform(scaleX1, cols[i]->gameObject->matWorld.matScale * cols[i]->gameObject->matWorld.matRot).V3Len();
		float worldScaleY1 = MCBMatrix::transform(scaleY1, cols[i]->gameObject->matWorld.matScale * cols[i]->gameObject->matWorld.matRot).V3Len();
		float worldScaleZ1 = MCBMatrix::transform(scaleZ1, cols[i]->gameObject->matWorld.matScale * cols[i]->gameObject->matWorld.matRot).V3Len();

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

	//全てのオブジェクトを移動する前の座標へ移動させる
	for (int i = 0; i < cols.size(); i++)
	{
		cols[i]->gameObject->position = cols[i]->preTranslation.ConvertXMFloat3();
	}

	//行列更新のついでに移動する前の座標を保存
	std::vector<Vector3D> objsPreTranslation = {};
	for (auto& itr:Object3d::GetAllObjs())
	{
		objsPreTranslation.push_back({ itr->position.x,itr->position.y,itr->position.z });
		itr->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
	}

	//少しづつ移動させながら当たり判定と押し戻し処理を行う
	for (int i = 0; i < translateDivNumF; i++)
	{
		//移動
		int j = 0;
		for (auto& itr : Object3d::GetAllObjs())
		{
			Vector3D move = objsTranslation[j] - objsPreTranslation[j];

			itr->position.x += move.vec.x / translateDivNumF;
			itr->position.y += move.vec.y / translateDivNumF;
			itr->position.z += move.vec.z / translateDivNumF;
			itr->Update(*IScene::GetCamera()->GetView(), *IScene::GetCamera()->GetProjection());
			j++;
		}

		//当たり判定と押し戻しベクトルの算出
		for (int j = 0; j < cols.size(); j++)
		{
			for (int k = j + 1; k < cols.size(); k++)
			{
				cols[j]->Collide(cols[k]);
				cols[j]->SendPushBack();
				cols[k]->SendPushBack();
			}
		}

		//押し戻し
		/*
		for (int j = 0; j < cols.size(); j++)
		{
			cols[j]->SendPushBack();
		}
		*/
	}

	cols.clear();
}