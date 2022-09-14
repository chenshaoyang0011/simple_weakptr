#include <gtest/gtest.h>
#include "weak_ptr.h"
namespace cik {

TEST(WeakPtrFactoryTest, Basic) {
    int data;
    WeakPtrFactory<int> factory(&data);
    WeakPtr<int> ptr = factory.GetWeakPtr();
    EXPECT_EQ(&data, ptr.get());
}

TEST(WeakPtrFactoryTest, Comparison) {
    int data;
    WeakPtrFactory<int> factory(&data);
    WeakPtr<int> ptr = factory.GetWeakPtr();
    WeakPtr<int> ptr2 = ptr;
    EXPECT_EQ(ptr.get(), ptr2.get());
}

// TEST(WeakPtrFactoryTest, Move) {
//   int data;
//   WeakPtrFactory<int> factory(&data);
//   WeakPtr<int> ptr = factory.GetWeakPtr();
//   WeakPtr<int> ptr2 = factory.GetWeakPtr();
//   WeakPtr<int> ptr3 = std::move(ptr2);
//   EXPECT_NE(ptr.get(), ptr2.get());
//   EXPECT_EQ(ptr.get(), ptr3.get());
// }

TEST(WeakPtrFactoryTest, OutOfScope) {
    WeakPtr<int> ptr;
    EXPECT_EQ(nullptr, ptr.get());
    {
        int data;
        WeakPtrFactory<int> factory(&data);
        ptr = factory.GetWeakPtr();
    }
    EXPECT_EQ(nullptr, ptr.get());
}

} // namespace cik
