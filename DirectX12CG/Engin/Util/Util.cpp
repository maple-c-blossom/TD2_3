#include "Util.h"
#include <stdlib.h>
#include <time.h>

using namespace MCB;

double MCB::Lerp(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time + startPos;
}
double MCB::Lerp(double startPos, double endPos, double time)
{
	double change = endPos - startPos;
	return change * time + startPos;
}
double MCB::InQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time * time + startPos;
}
double MCB::OutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
double MCB::InOutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime / 2;
	double change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}

float MCB::clamp(float f)
{
	return (f < 0.0f) ? 0.0f : ((f > 1.0f) ? 1.0f : f);
}

float MCB::clamp(float f, float min, float max)
{
	return (f < min) ? min : ((f > max) ? max : f);
}
double MCB::OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::EaseInBack(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.70158;
	double c3 = c1 + 1;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseInBack2(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.2;
	double c3 = c1 + 0.8;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseOutBounce(double startPos, double endPos, double time, double maxTime)
{
	time /= maxTime;
	double change = endPos - startPos;
	double n1 = 7.5625;
	double d1 = 2.75;

	if (time < 1 / d1)
	{
		return n1 * time * time + startPos + change;
	}
	else if (time < 2 / d1)
	{
		return n1 * (time -= 1.5 / d1) * time + 0.75 + startPos + change;
	}
	else if (time < 2.5 / d1)
	{
		return n1 * (time -= 2.25 / d1) * time + 0.9375 + startPos + change;
	}
	else
	{
		return n1 * (time -= 2.625 / d1) * time + 0.984375 + startPos + change;
	}
}

int MCB::Abs(int num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::Abs(float num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

double MCB::Abs(double num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::ConvertRadius(float angle)
{
	return angle * PI / 180.0f;
}



int MCB::GetRand(int min, int max)
{
	return min + rand() % (max - min);
}



MCB::SimpleFigure::SimpleFigure()
{

	triangle.Init();
	triangle.model_ = &triangleMaterial;
	triangleMaterial.vertices_ = {
		{PointA,{1,1,1},{0,0}},
		{PointB,{1,1,1},{0,0}},
		{PointC,{1,1,1},{0,0}}
	};
	triangleMaterial.SetSizeVB();
	triangleMaterial.material_.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, triangleMaterial.sizeVB_, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
	triangleMaterial.CreateVertexBuffer(triangleMaterial.material_.HeapProp_, D3D12_HEAP_FLAG_NONE, triangleMaterial.material_.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
	triangleMaterial.VertexMaping();
	triangleMaterial.SetVbView();
	triangle.color_ = color;
	triangle.model_->texture_ = triangle.model_->loader_->CreateNoTextureFileIsTexture();
}

void MCB::SimpleFigure::DrawTriangle()
{

	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();

	triangleMaterial.vertices_ = {
		{PointA,{1,1,1},{0,0}},
		{PointB,{1,1,1},{0,0}},
		{PointC,{1,1,1},{0,0}}
	};

	triangleMaterial.VertexMaping();
	triangle.color_ = color;



	triangle.Update();

	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(2, triangleMaterial.material_.constBuffMaterialB1_->GetGPUVirtualAddress());

	//SRVヒープの先頭アドレスを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += triangle.model_->texture_->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);
	//SRVヒープの先頭にあるSRVをパラメータ1番に設定
	dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//頂点データ
	dx12->commandList_->IASetVertexBuffers(0, 1, &triangleMaterial.vbView_);
	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(0, triangle.GetConstBuffTrans()->GetGPUVirtualAddress());
	//描画コマンド
	dx12->commandList_->DrawInstanced((UINT)triangleMaterial.vertices_.size(), 1, 0, 0);
}

void MCB::Shake::Setshake(int shakeTime, float endCount, float shakeRange)
{
	if (shakeTime / 2 < 1) this->shakeTime.Set(1);
	else this->shakeTime.Set(shakeTime / 2);
	this->endCount.Set((int)endCount);
	this->shakeStartTimeOrigin = shakeTime;
	this->shakeRange = shakeRange;
	this->shakeRangeOrigin = shakeRange;
	shakeStartPos = 0;
}

float MCB::Shake::shakeUpdate()
{
	float ans;
	shakeTime.SafeUpdate();
	ans = Lerp(shakeStartPos, shakeRange, shakeTime.GetEndTime(), shakeTime.NowTime());
	if (shakeTime.IsEnd())
	{
		shakeTime.Set(Lerp(shakeStartTimeOrigin, 1, endCount.GetEndTime(), endCount.NowTime()));
		shakeRange = Lerp(shakeRangeOrigin, 0, endCount.GetEndTime(), endCount.NowTime());
		shakeStartPos = ans;
		shakeRange *= -1;
		endCount.SafeUpdate();
	}
	if (endCount.IsEnd())
	{
		return 0;
	}
	return ans;
}

float MCB::Shake::shakeUpdateR()
{
	float ans = 0;
	endCount.SafeUpdate();
	shakeRange = Lerp(shakeRangeOrigin, 0, endCount.GetEndTime(), endCount.NowTime());

	//ans = shakeRange * shakeSign;
	//shakeSign *= -1;
	if (endCount.IsEnd())
	{
		return 0;
	}
	else
	{
		ans = GetRand(-shakeRange * 10000, shakeRange * 10000) / 10000.f;
	}
	return ans;
}
