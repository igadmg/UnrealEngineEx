#include "GuessExStatics.h"


namespace XX
{
#if defined(ENGINE_Actor_generated_h) && !defined(GuessEx_ENGINE_Actor_generated_h)
#define GuessEx_ENGINE_Actor_generated_h
	template <typename TActor = AActor>
	const TActor* GetOwningActor(const AActor* Object)
	{
		return Cast<TActor>(Object);
	}
#endif

#if defined(ENGINE_ActorComponent_generated_h) && !defined(GuessEx_ENGINE_ActorComponent_generated_h)
#define GuessEx_ENGINE_ActorComponent_generated_h
	template <typename TActor = AActor>
	TActor* GetOwningActor(const UActorComponent* Object)
	{
		return Cast<TActor>(Object->GetOwner());
	}
#endif

#if defined(ENGINE_CharacterMovementComponent_generated_h) && !defined(GuessEx_ENGINE_CharacterMovementComponent_generated_h)
#define GuessEx_ENGINE_CharacterMovementComponent_generated_h
	template <typename TCharacter = ACharacter>
	TCharacter* GetCharacter(const UCharacterMovementComponent* Object)
	{
		return TValid<TCharacter, ACharacter>::Valid(Object->GetCharacterOwner());
	}
#endif
}
