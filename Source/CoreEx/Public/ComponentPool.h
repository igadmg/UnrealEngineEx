#pragma once

#include <functional>


struct FComponentPool
{
	template <typename TComponent>
	static void DefaultDestroy(TComponent* Component)
	{
		Component->UnregisterComponent();
		Component->DestroyComponent();
	}
};

template <typename TComponent>
struct TComponentPool
{
	TComponentPool(TArray<TComponent*>& Values)
		: Pool(Values)
		, DestroyFn([](TComponent* c) { FComponentPool::DefaultDestroy(c); })
	{
		Algo::Reverse(Pool);
	}

	TComponentPool(TArray<TComponent*>& Values, std::function<void(TComponent*)> CustomDestroyFn)
		: Pool(Values)
		, DestroyFn([CustomDestroyFn](TComponent* c) { CustomDestroyFn(c); FComponentPool::DefaultDestroy(c); })
	{
		Algo::Reverse(Pool);
	}

	~TComponentPool()
	{
		for (auto Component : Pool)
		{
			if (!IsValid(Component))
				continue;

			DestroyFn(Component);
		}
	}

	TComponent* Take()
	{
		if (Pool.Num() == 0)
			return nullptr;

		auto Component = Pool.Last();
		Pool.SetNum(Pool.Num() - 1);

		return Component;
	}

protected:
	TArray<TComponent*> Pool;
	std::function<void(TComponent*)> DestroyFn;
};
