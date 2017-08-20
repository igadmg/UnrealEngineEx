#pragma once



namespace ipl
{
	template <typename T, typename TT>
	T linear_spline(T p0, T p1, TT t)
	{
		return p0 + (p1 - p0) * t;
	}
}
