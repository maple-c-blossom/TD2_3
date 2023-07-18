#pragma once
#include <math.h>
#include "Object3d.h"
#include "Timer.h"
#define PI 3.14159265358979323846264338327950288f
namespace MCB
{
	double Lerp(double startPos, double endPos, double maxTime, double time);
	double Lerp(double startPos, double endPos, double time);
	double InQuad(double startPos, double endPos, double maxTime, double time);
	double OutQuad(double startPos, double endPos, double maxTime, double time);
	double InOutQuad(double startPos, double endPos, double maxTime, double time);
	double OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	double OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	double EaseInBack(double startPos, double endPos, double maxTime, double time);
	double EaseInBack2(double startPos, double endPos, double maxTime, double time);
	double EaseOutBounce(double startPos, double endPos, double time, double maxTime);
	int Abs(int num);
	float Abs(float num);
	double Abs(double num);
	float ConvertRadius(float angle);

	static void InitRand() {
		srand((unsigned int)time(nullptr));
	};
	int GetRand(int min, int max);
	template<class T>
	T sign(T x)
	{
		return (x > 0) - (x < 0);
	};

	class Shake
	{
		float shakeRange;
		float shakeStartPos;
		int shakeStartTimeOrigin;
		float shakeRangeOrigin;
		float shakeSign = 1;
		Timer shakeTime;
	public:
		Timer endCount;
		void Setshake(int shakeTime,float endCount,float shakeRange);
		float shakeUpdate();
		float shakeUpdateR();
	};


	class SimpleFigure
	{
	public:
		SimpleFigure();
		Model triangleMaterial;
		Object3d triangle;

		Float3 PointA = { -0.5f,-0.5f,1.0f }; Float3 PointB = { -0.5f,0.5f,1.0f };
		Float3 PointC = { 0.5f,-0.5f,1.0f }; Float4 color = { 1.0f,1.0f,1.0f,1.0f };
		void  DrawTriangle(View view, Projection proj);

	};

	template<typename T = void>
	struct MyDeleter
	{
		constexpr MyDeleter() noexcept = default;

		// 別の Deleter オブジェクトから Deleter オブジェクトを構築します。
		// このコンストラクタは U* が T* に暗黙に変換可能な場合にのみ、オーバーロード解決に参加します。
		template<
			typename U,
			typename std::enable_if<std::is_convertible<U*, T*>::value, std::nullptr_t>::type = nullptr
		>
			MyDeleter(const MyDeleter<U>&) noexcept {}

		void operator()(T* ptr) const {
			delete ptr;
			ptr = nullptr;
		}
	};

	template<typename T>
	struct MyDeleter<T[]>
	{
		constexpr MyDeleter() noexcept = default;

		// 別の Deleter オブジェクトから Deleter オブジェクトを構築します。
		// このコンストラクタは U(*)[] が T(*)[] に暗黙に変換可能な場合にのみ、オーバーロード解決に参加します。
		template<
			typename U,
			typename std::enable_if<std::is_convertible<U(*)[], T(*)[]>::value, std::nullptr_t>::type = nullptr
		>
			MyDeleter(const MyDeleter<U[]>&) noexcept {}

		void operator()(T* ptr) const {
			delete[] ptr;
			for(auto& itr:ptr)itr = nullptr;
		}
	};
}

