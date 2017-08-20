#pragma once

#include "interpolated_value.h"



template <typename T>
struct TInterpolatedValue : ipl::interpolated_value<T, float>
{
public:
	T MeanValue() const
	{
		size_t icount = count();
		T MeanValue = T();

		for (int i = 0; i < icount; i++)
		{
			MeanValue += getValue(i) / icount;
		}

		return MeanValue;
	}
};

template <typename T>
struct TInterpolatedAngleValue : ipl::interpolated_angle_value<T, float>
{
public:
	T MeanValue() const
	{
		size_t icount = count();
		if (icount == 0)
			return 0;

		T MeanDeltaValue = T();

		for (int i = 0; i < icount; i++)
		{
			MeanDeltaValue += lastDelta(i) / icount;
		}

		T MeanValue = getValue(0) + MeanDeltaValue;
		return std::fmod(std::fmod(MeanValue, max_value_ - min_value_) + max_value_ - min_value_ - min_value_, max_value_ - min_value_) + min_value_;
	}
};
