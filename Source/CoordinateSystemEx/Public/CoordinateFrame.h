#include <type_traits>

/**
That file should be included after all include files in cpp. Btter include it in .final.h

 Coordinate Frame functions provide some generic way to get World and Relative positions, rotations and transforms
 of different UE objects.

 Can be used by simply making Cooridante frame from any object (if it is supported)

 auto cf = MakeCoordinateFrame(MyActor);
 cf.GetWorldLocation();
 cf.SetWorldLocation(NewLocation);

 or

 auto cf = MakeCoordinateFrame(MyActorComponent);
 cf.GetWorldLocation();
 cf.GetRelativeLocation();
 cf.SetWorldLocation(NewLocation);

 etc.

 Check CoordinateFrame.0.h for available functions.
 Every included file implements CoordinateFrame for specific object type.

 Should be extended...
*/

#include "CoordinateFrame.0.h"

#include "CoordinateFrame.FTransform.h"
#include "CoordinateFrame.FPlane.h"

#ifdef ENGINE_Actor_generated_h
#include "CoordinateFrame.AActor.h"
#endif

#ifdef ENGINE_ActorComponent_generated_h
#include "CoordinateFrame.UActorComponent.h"
#endif

#ifdef ENGINE_SceneComponent_generated_h
#include "CoordinateFrame.USceneComponent.h"
#endif

#ifdef ENGINE_SplineComponent_generated_h
#include "CoordinateFrame.USplineComponent.h"
#endif

#ifdef ENGINE_StaticMeshSocket_generated_h
#include "CoordinateFrame.UStaticMeshSocket.h"
#endif
