#pragma once



namespace ipl
{
	template <typename T>
	T hermite_30(T t) { return 1 - 3*t*t + 2*t*t*t; }
	template <typename T>
	T hermite_31(T t) { return t - 2*t*t + t*t*t; }
	template <typename T>
	T hermite_32(T t) { return - t*t + t*t*t; }
	template <typename T>
	T hermite_33(T t) { return 3*t*t - 2*t*t*t; }



	template <typename T, typename TT>
	T cubic_hermite_spline(T p0, T v0, T p1, T v1, TT t)
	{
		return hermite_30(t)*p0 + hermite_31(t)*v0 + hermite_32(t)*v1 + hermite_33(t)*p1;
	}
}
