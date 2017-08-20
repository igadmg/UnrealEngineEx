#pragma once



namespace ipl
{
	template <typename T>
	T hermite_50(T t) { return 1 - 10*t*t*t + 15*t*t*t*t - 6*t*t*t*t*t; }
	template <typename T>
	T hermite_51(T t) { return t - 6*t*t*t + 8*t*t*t*t - 3*t*t*t*t*t; }
	template <typename T>
	T hermite_52(T t) { return 0.5*t*t - 1.5*t*t*t + 1.5*t*t*t*t - 0.5*t*t*t*t*t; }
	template <typename T>
	T hermite_53(T t) { return 0.5*t*t*t - t*t*t*t + 0.5*t*t*t*t*t; }
	template <typename T>
	T hermite_54(T t) { return - 4*t*t*t + 7*t*t*t*t - 3*t*t*t*t*t; }
	template <typename T>
	T hermite_55(T t) { return 10*t*t*t - 15*t*t*t*t + 6*t*t*t*t*t; }



	template <typename T, typename TT>
	T quintic_hermite_spline(T p0, T v0, T a0, T p1, T v1, T a1, TT t)
	{
		return hermite_50(t)*p0 + hermite_51(t)*v0 + hermite_52(t)*a0 + hermite_53(t)*a1 + hermite_54(t)*v1 + hermite_55(t)*p1;
	}
}
