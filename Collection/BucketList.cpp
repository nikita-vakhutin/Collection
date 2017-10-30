#include "stdafx.h"
#include "BucketList.h"

uint16_t GetHash(const BL_Guid *key)
{
	uint16_t hash = 0;
	for (int i = 0; i < 8; ++i)
	{
		hash ^= ((uint16_t*)key)[i];
	}

	return hash;
}

void BL_Init(BL_Ctx *ctx)
{
	ctx->buckets = (BL_Item**)malloc(BUCKETS_COUNT * sizeof(BL_Item*));
	memset(ctx->buckets, 0, BUCKETS_COUNT * sizeof(BL_Item*));
}

void BL_Clear(BL_Ctx *ctx)
{
	for (size_t i = 0; i < BUCKETS_COUNT; ++i)
	{
		BL_Item* item = ctx->buckets[i];
		if (item != NULL)
		{
			// Clear whole list in each bucket.
			while (item->next != NULL)
			{
				BL_Item* ptr = item->next;
				item->next = ptr->next;

				free(ptr->data);
				free(ptr);
			}

			free(item->data);
			free(item);
			ctx->buckets[i] = NULL;
		}
	}
	free(ctx->buckets);
}

int BL_Insert(BL_Ctx *ctx, const BL_Guid *key, void *data)
{
	if (BL_Search(ctx, key) != NULL)
	{
		return 0;
	}

	BL_Item* newItem = (BL_Item*)malloc(sizeof(BL_Item));
	newItem->key = *key;
	newItem->data = data;
	newItem->next = NULL;

	uint16_t index = GetHash(key);
	BL_Item* item = ctx->buckets[index];

	if (item == NULL)
	{
		// If bucket is empty.
		ctx->buckets[index] = newItem;
		return 1;
	}

	// Bucket is not empty.
	// Insert item to the end of the list.
	while (item->next != NULL)
	{
		item = item->next;
	}
	item->next = newItem;

	return 1;
}

void* BL_Search(BL_Ctx *ctx, const BL_Guid *key)
{
	BL_Item* item = ctx->buckets[GetHash(key)];

	while (item != NULL)
	{
		if (memcmp(key, &item->key, sizeof(BL_Guid)) == 0)
		{
			// Item found.
			return item->data;
		}

		item = item->next;
	}

	// Item not found.
	return NULL;
}

int BL_Erase(BL_Ctx *ctx, const BL_Guid *key)
{
	uint16_t index = GetHash(key);
	BL_Item* item = ctx->buckets[index];

	while (item != NULL)
	{
		if (memcmp(key, &item->key, sizeof(BL_Guid)) == 0)
		{
			// Item found.
			free(item->data);

			if (item->next != NULL)
			{
				// Item in the middle of the list.
				item->data = item->next->data;
				item->key = item->next->key;

				BL_Item *nextPtr = item->next;
				item->next = nextPtr->next;
				free(nextPtr);
			}
			else
			{
				// Item in the end of the list.
				BL_Item* ptr = ctx->buckets[index];
				if (ptr == item)
				{
					// Only item in list.
					free(item);
					ctx->buckets[index] = NULL;
					return 1;
				}

				// Looking for ptr to last item in list.
				while (ptr->next != item)
				{
					ptr = ptr->next;
				}
				free(ptr->next);
				ptr->next = NULL;
			}

			return 1;
		}

		item = item->next;
	}

	return 0;
}