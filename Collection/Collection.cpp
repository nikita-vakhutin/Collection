#include "stdafx.h"
#include <iostream>
#include <time.h>

#include "BucketList.h"

int main()
{
	clock_t tStart = clock();
	BL_Ctx ctx;
	BL_Init(&ctx);

	uint32_t index = 0;
	BL_Guid guid;
	for (int i = 0; i < 1000000; i++)
	{
		memset(&guid, 0, sizeof(BL_Guid));
		guid.data[12] = index >> 24;
		guid.data[13] = index >> 16;
		guid.data[14] = index >> 8;
		guid.data[15] = index;

		int *data = (int*)malloc(sizeof(int));
		*data = i;
		BL_Insert(&ctx, &guid, data);

		index++;
	}

	memset(&guid, 0, sizeof(BL_Guid));
	guid.data[15] = 20;
	int val = *((int*)BL_Search(&ctx, &guid));

	index = 0;
	for (int i = 0; i < 1000000; i++)
	{
		memset(&guid, 0, sizeof(BL_Guid));
		guid.data[12] = index >> 24;
		guid.data[13] = index >> 16;
		guid.data[14] = index >> 8;
		guid.data[15] = index;

		BL_Erase(&ctx, &guid);

		index++;
	}
	
	BL_Clear(&ctx);

	std::cout << "Execution time: " << clock() - tStart << std::endl;
	system("pause");
    return 0;
}
