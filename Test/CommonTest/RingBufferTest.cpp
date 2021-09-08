#include "stdafx.h"

TEST(RingBufferTest, CircularUsage)
{
	CRingBuffer buffer;
	EXPECT_EQ(EC_SUCCESS, buffer.Create(1024));

	LPBYTE pBuffer1 = buffer.Alloc(512);
	LPBYTE pBuffer2 = buffer.Alloc(512);
	EXPECT_EQ(pBuffer1, buffer.Alloc(512));
	buffer.Destroy();
}

TEST(RingBufferTest, ReservedAlloc)
{
	CRingBuffer buffer;
	EXPECT_EQ(EC_SUCCESS, buffer.Create(1024));

	LPBYTE pBuffer1 = buffer.Alloc(512);
	EXPECT_NE(pBuffer1, buffer.Alloc(256, 256));
	EXPECT_NE(pBuffer1, buffer.Alloc(256));
	EXPECT_EQ(pBuffer1, buffer.Alloc(256));
	buffer.Destroy();
}

TEST(RingBufferTest, ReservedAllocReuse)
{
	CRingBuffer buffer;
	EXPECT_EQ(EC_SUCCESS, buffer.Create(1024));

	LPBYTE pBuffer1 = buffer.Alloc(512);
	EXPECT_EQ(pBuffer1, buffer.Alloc(512, 512));
	buffer.Destroy();
}