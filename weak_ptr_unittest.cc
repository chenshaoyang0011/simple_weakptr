#include <gtest/gtest.h>
#include "weak_ptr.h"
namespace cik {

namespace {

struct Base {
    std::string member;
};
struct Derived : public Base {};

WeakPtr<int> PassThru(WeakPtr<int> ptr) {
    return ptr;
}

} // namespace

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

TEST(WeakPtrFactoryTest, Move) {
    int data;
    WeakPtrFactory<int> factory(&data);
    WeakPtr<int> ptr = factory.GetWeakPtr();
    WeakPtr<int> ptr2 = factory.GetWeakPtr();
    WeakPtr<int> ptr3 = std::move(ptr2);
    EXPECT_NE(ptr.get(), ptr2.get());
    EXPECT_EQ(ptr.get(), ptr3.get());
}

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

TEST(WeakPtrFactoryTest, Multiple) {
    WeakPtr<int> a, b;
    {
        int data;
        WeakPtrFactory<int> factory(&data);
        a = factory.GetWeakPtr();
        b = factory.GetWeakPtr();
        EXPECT_EQ(&data, a.get());
        EXPECT_EQ(&data, b.get());
    }
    EXPECT_EQ(nullptr, a.get());
    EXPECT_EQ(nullptr, b.get());
}

TEST(WeakPtrFactoryTest, MultipleStaged) {
    WeakPtr<int> a;
    {
        int data;
        WeakPtrFactory<int> factory(&data);
        a = factory.GetWeakPtr();
        {
            WeakPtr<int> b = factory.GetWeakPtr();
        }
        EXPECT_NE(nullptr, a.get());
    }
    EXPECT_EQ(nullptr, a.get());
}

TEST(WeakPtrFactoryTest, Dereference) {
    Base data;
    data.member = "123456";
    WeakPtrFactory<Base> factory(&data);
    WeakPtr<Base> ptr = factory.GetWeakPtr();
    EXPECT_EQ(&data, ptr.get());
    EXPECT_EQ(data.member, (*ptr).member);
    EXPECT_EQ(data.member, ptr->member);
}

TEST(WeakPtrFactoryTest, UpCast) {
    Derived data;
    WeakPtrFactory<Derived> factory(&data);
    WeakPtr<Base> ptr = factory.GetWeakPtr();
    ptr = factory.GetWeakPtr();
    EXPECT_EQ(ptr.get(), &data);
}

TEST(WeakPtrTest, ConstructFromNullptr) {
    WeakPtr<int> ptr = PassThru(nullptr);
    EXPECT_EQ(nullptr, ptr.get());
}

TEST(WeakPtrFactoryTest, BooleanTesting) {
    int data;
    WeakPtrFactory<int> factory(&data);

    WeakPtr<int> ptr_to_an_instance = factory.GetWeakPtr();
    EXPECT_TRUE(ptr_to_an_instance);
    EXPECT_FALSE(!ptr_to_an_instance);

    if (ptr_to_an_instance) {
    } else {
        ADD_FAILURE() << "Pointer to an instance should result in true.";
    }

    if (!ptr_to_an_instance) { // check for operator!().
        ADD_FAILURE() << "Pointer to an instance should result in !x being false.";
    }

    WeakPtr<int> null_ptr;
    EXPECT_FALSE(null_ptr);
    EXPECT_TRUE(!null_ptr);

    if (null_ptr) {
        ADD_FAILURE() << "Null pointer should result in false.";
    }

    if (!null_ptr) { // check for operator!().
    } else {
        ADD_FAILURE() << "Null pointer should result in !x being true.";
    }
}

TEST(WeakPtrFactoryTest, ComparisonToNull) {
    int data;
    WeakPtrFactory<int> factory(&data);

    WeakPtr<int> ptr_to_an_instance = factory.GetWeakPtr();
    EXPECT_NE(nullptr, ptr_to_an_instance);
    EXPECT_NE(ptr_to_an_instance, nullptr);

    WeakPtr<int> null_ptr;
    EXPECT_EQ(null_ptr, nullptr);
    EXPECT_EQ(nullptr, null_ptr);
}

} // namespace cik
