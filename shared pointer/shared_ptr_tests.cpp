#include "shared_ptr.h"
#include "containers.h"

#include <vector>

#include "gtest.h"
#include "gmock.h"

namespace {
class Validator {
 public:
  explicit Validator(bool* validity_flag) : validity_flag_(validity_flag) {
    *validity_flag_ = true;
  }
  ~Validator() {
    *validity_flag_ = false;
  }
 private:
  bool* validity_flag_;
};
}  // anonymous namespace

namespace shared_ptr {

TEST(SharedPtr_MemoryMgmt, Sample_RefCounter) {
  bool is_object_valid;
  auto* validator = new Validator(&is_object_valid);
  ASSERT_TRUE(is_object_valid);

  std::vector<SharedPtr<Validator>> pointers;
  pointers.emplace_back(validator);
  ASSERT_TRUE(is_object_valid);

  for (int i = 0; i < 10; ++i) {
    pointers.push_back(pointers.back());
    ASSERT_TRUE(is_object_valid);
  }

  while (pointers.size() > 1) {
    pointers.pop_back();
    ASSERT_TRUE(is_object_valid);
  }

  pointers.pop_back();
  ASSERT_FALSE(is_object_valid);
}

TEST(SharedPtr_MemoryMgmt, Sample_RefCounter_Other) {
    bool is_object_valid;
    auto* validator = new Validator(&is_object_valid);
    ASSERT_TRUE(is_object_valid);
    {
        SharedPtr<Validator> ptr(validator);
        ASSERT_TRUE(is_object_valid);
        ptr.Release();
        SharedPtr<Validator> ptr1(ptr);
        ASSERT_EQ(ptr, nullptr);
        ASSERT_EQ(ptr1, nullptr);
        ASSERT_TRUE(is_object_valid);
    }
    {
        SharedPtr<Validator> ptr1(validator);
        {
            SharedPtr<Validator> ptr2(ptr1);
            {
                SharedPtr<Validator> ptr3(ptr2);
                ptr3.Release();
            }
        }
    }
    {
        SharedPtr<Validator> ptr1(validator);
        {
            SharedPtr<Validator> ptr2(ptr1);
            {
                SharedPtr<Validator> ptr3(ptr2);
                ptr3.Release();
            }
            ptr2.Release();
        }
    }
    {
        SharedPtr<Validator> ptr1(validator);
        {
            SharedPtr<Validator> ptr2(ptr1);
            {
                SharedPtr<Validator> ptr3(ptr2);
                ptr3.Release();
            }
        }
        ptr1.Release();
    }
    ASSERT_TRUE(is_object_valid);
    {
        SharedPtr<Validator> ptr1(validator);
        {
            SharedPtr<Validator> ptr2(std::move(ptr1));
            ptr2.Release();
        }
        ASSERT_TRUE(is_object_valid);
    }
    ASSERT_TRUE(is_object_valid);
    {
        SharedPtr<Validator> ptr1(validator);
        SharedPtr<Validator> ptr2(ptr1);
        SharedPtr<Validator> ptr3(ptr2);
        SharedPtr<Validator> ptr4(ptr3);
        SharedPtr<Validator> ptr5(ptr4);
        {
            SharedPtr<Validator> ptr(std::move(ptr2));
            ptr.Release();
        }
    }
    ASSERT_TRUE(is_object_valid);
    {
        SharedPtr<Validator> ptr1(validator);
        SharedPtr<Validator> ptr2 = ptr1;
        ptr2.Release();
        ptr1 = ptr2;
    }
    ASSERT_TRUE(is_object_valid);
    {
        SharedPtr<Validator> ptr1(validator);
        SharedPtr<Validator> ptr2 = ptr1;
        ptr2.Release();
        ptr1 = std::move(ptr2);
    }
    ASSERT_TRUE(is_object_valid);
}

TEST(SharedPtr_MemoryMgmt, Sample_Release1) {
    int *x = new int(1);
    SharedPtr<int> ptr(x);
    SharedPtr<int> ptr1(ptr);
    ptr1.Release();
    SharedPtr<int> ptr2(ptr1);
    ASSERT_EQ(ptr, x);
    ASSERT_EQ(ptr1, nullptr);
    ASSERT_EQ(ptr2, nullptr);
}

TEST(SharedPtr_MemoryMgmt, Sample_Release) {
  bool is_object_valid;
  auto* validator = new Validator(&is_object_valid);
  ASSERT_TRUE(is_object_valid);
  {
    SharedPtr<Validator> ptr(validator);
    ptr.Release();
    ASSERT_TRUE(is_object_valid);
  }

  ASSERT_TRUE(is_object_valid);
  delete validator;
}

TEST(SharedPtr_Ops, Sample_Basic) {
  SharedPtr<std::vector<int>> shared_ptr(new std::vector<int>(123));

  auto* vector_ptr = shared_ptr.Get();
  ASSERT_EQ(vector_ptr->size(), 123);
  vector_ptr->push_back(123456);
  vector_ptr->push_back(654321);
  ASSERT_EQ(vector_ptr->size(), 125);

  auto& vector_ref = *shared_ptr;
  ASSERT_EQ(vector_ref.size(), 125);
  vector_ref.push_back(123456);
  vector_ref.push_back(654321);
  ASSERT_EQ(vector_ref.size(), 127);

  ASSERT_EQ(shared_ptr->size(), 127);
  shared_ptr->pop_back();
  shared_ptr->pop_back();
  ASSERT_EQ(shared_ptr->size(), 125);
}

TEST(SharedPtr_Ops, Sample_Const) {
  auto* raw_ptr = new std::vector<int>(123);
  const SharedPtr<std::vector<int>> shared_ptr(raw_ptr);

  const auto* vector_ptr = shared_ptr.Get();
  ASSERT_EQ(vector_ptr->size(), 123);
  raw_ptr->push_back(123456);
  raw_ptr->push_back(654321);
  ASSERT_EQ(vector_ptr->size(), 125);

  const auto& vector_ref = *shared_ptr;
  ASSERT_EQ(vector_ref.size(), 125);
  raw_ptr->push_back(123456);
  raw_ptr->push_back(654321);
  ASSERT_EQ(vector_ref.size(), 127);

  ASSERT_EQ(shared_ptr->size(), 127);
  raw_ptr->pop_back();
  raw_ptr->pop_back();
  ASSERT_EQ(shared_ptr->size(), 125);
}

using containers::BiDirectionalList;
using containers::Queue;

TEST(SharedPtr_Test, Constructors_Destructors) {
    {
        SharedPtr<Queue<int>> ptr(new Queue<int>({1, 2, 3, 4}));
    }
    {
        int *ptr = nullptr;
        delete(ptr);
    }
    {
        SharedPtr<Queue<int>> ptr(nullptr);
    }
    {
        SharedPtr<Queue<int>> ptr;
    }
}

}  // namespace shared_ptr
