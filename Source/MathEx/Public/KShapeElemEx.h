#pragma once

#include "PhysicsEngine/ShapeElem.h"



struct FKShapeElemEx
{
	static EAggCollisionShape::Type GetShapeType(const FKShapeElem& Shape)
	{
		struct FKShapeElemAccessor
		{
			void** __vfptr;
			float RestOffset;
			FName Name;
			EAggCollisionShape::Type ShapeType;
			uint8 bContributeToMass : 1;

#if defined(WITH_PHYSX)
			FPhysxUserData UserData;
#endif // WITH_PHYSX
		};

		return reinterpret_cast<const FKShapeElemAccessor&>(Shape).ShapeType;
	}
};
