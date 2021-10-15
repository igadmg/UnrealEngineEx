#pragma once

#include "Containers/CircularQueue.h"

/**
 * Implements a lock-free first-in first-out queue using a circular array with autorewrite.
 *
 * This class is not thread safe. Pushing to Full queue can cahnge Head position.
 *
 * The number of items that can be enqueued is one less than the queue's capacity,
 * because one item will be used for detecting full and empty states.
 *
 * @param ElementType The type of elements held in the queue.
 */
template<typename ElementType> class TCircularAutoQueue
{
public:
	TCircularAutoQueue(uint32 CapacityPlusOne = 8)
		: Queue(CapacityPlusOne)
	{ }


public:
	/**
	 * Gets the number of elements in the queue.
	 *
	 * Can be called from any thread. The result reflects the calling thread's current
	 * view. Since no locking is used, different threads may return different results.
	 *
	 * @return Number of queued elements.
	 */
	uint32 Count() const
	{
		return Queue.Count();
	}

	/**
	 * Removes an item from the front of the queue.
	 *
	 * @param OutElement Will contain the element if the queue is not empty.
	 * @return true if an element has been returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 */
	bool Dequeue(ElementType& OutElement)
	{
		return Queue.Dequeue(OutElement);
	}

	/**
	 * Removes an item from the front of the queue.
	 *
	 * @return true if an element has been removed, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 */
	bool Dequeue()
	{
		return Queue.Dequeue();
	}

	/**
	 * Empties the queue.
	 *
	 * @note To be called only from consumer thread.
	 * @see IsEmpty
	 */
	void Empty()
	{
		Queue.Empty();
	}

	/**
	 * Adds an item to the end of the queue.
	 *
	 * @param Element The element to add.
	 * @return true if the item was added, false if the queue was full.
	 * @note To be called only from producer thread.
	 */
	bool Enqueue(const ElementType& Element)
	{
		if (!Queue.Enqueue(Element))
		{
			Queue.Dequeue();
			return Queue.Enqueue(Element);
		}

		return true;
	}

	/**
	 * Adds an item to the end of the queue.
	 *
	 * @param Element The element to add.
	 * @return true if the item was added, false if the queue was full.
	 * @note To be called only from producer thread.
	 */
	bool Enqueue(ElementType&& Element)
	{
		if (!Queue.Enqueue(Element))
		{
			Queue.Dequeue();
			return Queue.Enqueue(Element);
		}

		return true;
	}

	/**
	 * Checks whether the queue is empty.
	 *
	 * Can be called from any thread. The result reflects the calling thread's current
	 * view. Since no locking is used, different threads may return different results.
	 *
	 * @return true if the queue is empty, false otherwise.
	 * @see Empty, IsFull
	 */
	FORCEINLINE bool IsEmpty() const
	{
		return Queue.IsEmpty();
	}

	/**
	 * Checks whether the queue is full.
	 *
	 * Can be called from any thread. The result reflects the calling thread's current
	 * view. Since no locking is used, different threads may return different results.
	 *
	 * @return true if the queue is full, false otherwise.
	 * @see IsEmpty
	 */
	bool IsFull() const
	{
		return Queue.IsFull();
	}

	/**
	 * Returns the oldest item in the queue without removing it.
	 *
	 * @param OutItem Will contain the item if the queue is not empty.
	 * @return true if an item has been returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 */
	bool Peek(ElementType& OutItem) const
	{
		return Queue.Peek(OutItem);
	}

	/**
	 * Returns the oldest item in the queue without removing it.
	 *
	 * @return an ElementType pointer if an item has been returned, nullptr if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @note The return value is only valid until Dequeue, Empty, or the destructor has been called.
	 */
	const ElementType* Peek() const
	{
		return Queue.Peek();
	}


private:
	TCircularQueue<ElementType> Queue;
};
