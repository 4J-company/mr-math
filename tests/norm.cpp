#include <gtest/gtest.h>

#include "mr-math/norm.hpp"
#include "mr-math/packing.hpp"

TEST(NormalPackingTest, Octahedron32) {
	mr::Norm3f n{ 30, 47, 80 };
	mr::PackedNorm32 packed = mr::pack_oct32(n);
	mr::Norm3f unpacked = mr::unpack_oct32(packed);
	EXPECT_TRUE(mr::equal(unpacked, n, 0.01));
}

TEST(NormalPackingTest, Octahedron24) {
	mr::Norm3f n{ 30, 47, 80 };
	mr::PackedNorm24 packed = mr::pack_oct24(n);
	mr::Norm3f unpacked = mr::unpack_oct24(packed);
	EXPECT_TRUE(mr::equal(unpacked, n, 0.01));
}

TEST(NormalPackingTest, Octahedron16) {
	mr::Norm3f n{ 30, 47, 80 };
	mr::PackedNorm16 packed = mr::pack_oct16(n);
	mr::Norm3f unpacked = mr::unpack_oct16(packed);
	EXPECT_TRUE(mr::equal(unpacked, n, 0.1));
}
