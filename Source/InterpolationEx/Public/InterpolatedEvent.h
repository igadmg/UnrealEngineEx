#pragma once



struct FInterpolatedEvent
{
	float EventTime;
};

enum class EInterpolationStatus : uint8
{
	IT_None,
	IT_Set,
	IT_Interpolated,
	IT_Extrapolated,
};

template <typename T>
struct TEventInterpolator
{
	static void Extrapolate(const T* PrevItems, int32 ItemsCount, float Time, T& Result);
	static void Interpolate(const T& A, const T& B, float Alpha, T& Result);
};

template <typename T, int N, typename Interpolator = TEventInterpolator<T>>
class TInterpolationQueue : public TArray<T, TFixedAllocator<N>>
{
public:
	void Put(const T& Value)
	{
		if (this->Num() == N)
		{
			this->SetNum(N - 1, false);
		}
		
		this->Insert(Value, 0);
	}

	EInterpolationStatus Interpolate(float Time, T& Result) const
	{
		if (this->Num() == 0)
			return EInterpolationStatus::IT_None;

		if (this->Num() == 1)
		{
			Result = this->Last();
			return EInterpolationStatus::IT_Set;
		}

		int PreviousIndex = -1;
		for (int i = 0; i < this->Num(); i++)
		{
			if ((*this)[i].EventTime < Time)
			{
				PreviousIndex = i;
				break;
			}
		}

		if (PreviousIndex == -1)
		{
			Result = this->Last();

			return EInterpolationStatus::IT_Set;
		}

		if (PreviousIndex == 0) // extrapolate
		{
			Interpolator::Extrapolate(this->GetData(), this->Num(), Time, Result);

			return EInterpolationStatus::IT_Extrapolated;
		}		
		
		// interpolate
		{
			const T &FirstState = (*this)[PreviousIndex];
			const T &SecondState = (*this)[PreviousIndex - 1];
			float Alpha = (Time - FirstState.EventTime) / (SecondState.EventTime - FirstState.EventTime);

			Interpolator::Interpolate(FirstState, SecondState, Alpha, Result);

			return EInterpolationStatus::IT_Interpolated;
		}
	}
};
