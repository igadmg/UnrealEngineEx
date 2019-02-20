#pragma once


static FORCEINLINE FTransform GetPlaneTransform(const FPlane& Plane)
{
	FVector PlaneAxis[] = {
		FVector::PointPlaneProject(FVector(1, 0, 0), Plane)
		, FVector::PointPlaneProject(FVector(0, 1, 0), Plane)
		, FVector::PointPlaneProject(FVector(0, 0, 1), Plane)
	};

	float PlaneAxisSizes[] = {
		PlaneAxis[0].SizeSquared()
		, PlaneAxis[1].SizeSquared()
		, PlaneAxis[2].SizeSquared()
	};
	int indices[] = { 0, 1, 2 };

	struct FAxisSortBySize {
		float* PlaneAxisSizes;
		FAxisSortBySize(float* PlaneAxisSizes) : PlaneAxisSizes(PlaneAxisSizes) {}
		bool operator()(int a, int b) const { return PlaneAxisSizes[a] > PlaneAxisSizes[b]; }
	};
	Sort(indices, 3, FAxisSortBySize(PlaneAxisSizes));

	if (indices[0] != 0 && indices[1] != 0)
		PlaneAxis[indices[2]] = FVector::CrossProduct(PlaneAxis[2], PlaneAxis[1]);
	if (indices[0] != 1 && indices[1] != 1)
		PlaneAxis[indices[2]] = FVector::CrossProduct(PlaneAxis[0], PlaneAxis[2]);
	if (indices[0] != 2 && indices[1] != 2)
		PlaneAxis[indices[2]] = FVector::CrossProduct(PlaneAxis[0], PlaneAxis[1]);

	PlaneAxis[0].Normalize();
	PlaneAxis[1].Normalize();
	PlaneAxis[2].Normalize();

	return FTransform(PlaneAxis[0], PlaneAxis[1], PlaneAxis[2], FVector::PointPlaneProject(FVector(0, 0, 0), Plane));
}

template <>
class TConstCoordinateFrame<FPlane> : public TConstCoordinateFrame<FTransform>
{
public:
	TConstCoordinateFrame(const FPlane& Object)
		: TConstCoordinateFrame<FTransform>(GetPlaneTransform(Object))
	{
	}
};

template <>
class TCoordinateFrame<FPlane> : public TConstCoordinateFrame<FPlane>
{
public:
	TCoordinateFrame(FPlane& Object)
		: TConstCoordinateFrame<FPlane>(Object)
		, Object(Object)
	{
	}



protected:
	FPlane& Object;
};



template <typename T>
struct FCoordinateFrame<T, typename std::enable_if<std::is_base_of<FPlane, T>::value>::type>
{
	typedef TConstCoordinateFrame<FPlane> ConstReturnType;
	typedef TCoordinateFrame<FPlane> ReturnType;

	static TConstCoordinateFrame<FPlane> MakeConst(const T& Object) { return TConstCoordinateFrame<FPlane>(Object); }
	static TCoordinateFrame<FPlane> Make(T& Object) { return TCoordinateFrame<FPlane>(Object); }
};
