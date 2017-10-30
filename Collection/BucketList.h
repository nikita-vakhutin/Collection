#pragma once

#include "stdafx.h"
#include <stdint.h>
#include <stdlib.h>

#define BUCKETS_COUNT 65536

typedef struct BL_GuidTag
{
	union
	{
		uint8_t data[16];

		struct
		{
			uint32_t data1;
			uint16_t data2;
			uint16_t data3;
			uint8_t data4[8];
		};

		struct
		{
			uint64_t n1;
			uint64_t n2;
		};
	};
} BL_Guid;

typedef struct BL_ItemTag
{
	BL_Guid key;
	void* data;
	struct BL_ItemTag* next;
} BL_Item;

typedef struct BL_CtxTag
{
	BL_Item** buckets;
} BL_Ctx;

void BL_Init(BL_Ctx *ctx);
void BL_Clear(BL_Ctx *ctx);
// Returns 1 in the case of success, 0 otherwise.
int BL_Insert(BL_Ctx *ctx, const BL_Guid *key, void *data);
void* BL_Search(BL_Ctx *ctx, const BL_Guid *key);
// Returns 1 in the case of success, 0 otherwise.
int BL_Erase(BL_Ctx *ctx, const BL_Guid *key);
