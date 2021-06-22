#include "containers.h"

#include "gtest.h"
#include "gmock.h"

namespace containers {

TEST(List_BaseOps, Sample) {
  BiDirectionalList<int> list;
  ASSERT_EQ(list.Size(), 0);
  ASSERT_TRUE(list.IsEmpty());

  list.PushBack(11);

  ASSERT_EQ(list.Size(), 1);
  ASSERT_FALSE(list.IsEmpty());
  ASSERT_EQ(list.Front()->value, 11);
  ASSERT_EQ(list.Back()->value, 11);

  list.PushBack(2222);

  ASSERT_EQ(list.Size(), 2);
  ASSERT_FALSE(list.IsEmpty());
  ASSERT_EQ(list.Front()->value, 11);
  ASSERT_EQ(list.Back()->value, 2222);

  list.PushFront(333333);

  ASSERT_EQ(list.Size(), 3);
  ASSERT_FALSE(list.IsEmpty());
  ASSERT_EQ(list.Front()->value, 333333);
  ASSERT_EQ(list.Back()->value, 2222);

  ASSERT_THAT(list.ToVector(), testing::ElementsAre(333333, 11, 2222));

  list.PopFront();

  ASSERT_EQ(list.Size(), 2);
  ASSERT_FALSE(list.IsEmpty());
  ASSERT_EQ(list.Front()->value, 11);
  ASSERT_EQ(list.Back()->value, 2222);

  list.PopBack();

  ASSERT_EQ(list.Size(), 1);
  ASSERT_FALSE(list.IsEmpty());
  ASSERT_EQ(list.Front()->value, 11);
  ASSERT_EQ(list.Back()->value, 11);

  list.PopFront();

  ASSERT_EQ(list.Size(), 0);
  ASSERT_TRUE(list.IsEmpty());

  ASSERT_THAT(list.ToVector(), testing::ElementsAre());
}

TEST(List_IndexOps, Sample) {
  BiDirectionalList<int> list;

  std::vector<int> values = {11, 2222, 333333, 333333, 2222, 333333};
  for (int value : values) {
    list.PushBack(value);
  }

  ASSERT_EQ(list.Find(11), 0);
  ASSERT_EQ(list.Find(2222), 1);
  ASSERT_EQ(list.Find(333333), 2);
  ASSERT_EQ(list.Find(1234567), -1);

  ASSERT_THAT(list.FindAll(11), testing::ElementsAre(0));
  ASSERT_THAT(list.FindAll(2222), testing::ElementsAre(1, 4));
  ASSERT_THAT(list.FindAll(333333), testing::ElementsAre(2, 3, 5));
  ASSERT_THAT(list.FindAll(1234567), testing::ElementsAre());

  for (int i = 0; i < values.size(); ++i) {
    ASSERT_EQ(list[i]->value, values[i]);
  }
}

TEST(List_InsertErase, Sample) {
  BiDirectionalList<int> list;
  list.PushBack(11);
  list.PushBack(22);

  list.InsertAfter(list[0], 33);
  ASSERT_EQ(list.Size(), 3);
  ASSERT_EQ(list[1]->value, 33);

  list.InsertBefore(list[1], 44);
  ASSERT_EQ(list.Size(), 4);
  ASSERT_EQ(list[1]->value, 44);

  list.Erase(list[1]);
  ASSERT_EQ(list.Size(), 3);
  ASSERT_EQ(list[1]->value, 33);
}

TEST(List_Misc, Sample) {
  BiDirectionalList<int> list1({1, 2, 3, 4});
  BiDirectionalList<int> list2({1, 2, 3, 4});
  BiDirectionalList<int> list3({-11, 22});
  ASSERT_EQ(list1, list2);
  ASSERT_NE(list1, list3);
}

TEST(Container, Sample_Queue) {
  std::vector<int> input = {1, 2, 3, 4, 5};
  std::vector<int> output;
  if (GetVariant() == Variant::kQueue) {
    output = {1, 2, 3, 4, 5};
  } else {
    output = {5, 4, 3, 2, 1};
  }

  Container<int> container;
  ASSERT_EQ(container.Size(), 0);
  ASSERT_TRUE(container.IsEmpty());

  for (int i = 0; i < input.size(); ++i) {
    container.Push(input[i]);
    ASSERT_EQ(container.Size(), i + 1);
    ASSERT_FALSE(container.IsEmpty());
  }

  for (int i = 0; i < input.size(); ++i) {
    ASSERT_EQ(container.Get(), output[i]);
    container.Pop();
    ASSERT_EQ(container.Size(), output.size() - i - 1);
    ASSERT_EQ(container.IsEmpty(), i == input.size() - 1);
  }
}

    class C {
    public:
        C(int x) : x(x) {}
        bool operator==(const C& rhs) {rhs.x == x;}
        bool operator!=(const C& rhs) {rhs.x != x;}

    private:
        int x;
    };

    TEST(t, t) {
        BiDirectionalList<C> l1;
        BiDirectionalList<C> l2;
        C x(2);

        EXPECT_EQ(l1, l2);
        l1.PushBack(x);
        EXPECT_NE(l1, l2);
    }

    TEST(BDList, oper_eq) {
        BiDirectionalList<int> l1;
        BiDirectionalList<int> l2;
        ASSERT_EQ(l1, l2);
        l1.PushBack(1);
        l2.PushBack(2);
        ASSERT_NE(l1, l2);
        l1.PushFront(2);
        ASSERT_NE(l1, l2);
    }

    TEST(BDList, init_op) {
        BiDirectionalList<int> l1;
        BiDirectionalList<int> l2(l1);
        ASSERT_THAT(l1.ToVector(), testing::ElementsAre());
        ASSERT_THAT(l2.ToVector(), testing::ElementsAre());
        BiDirectionalList<int> l3 = std::move(l2);
        ASSERT_THAT(l1.ToVector(), testing::ElementsAre());
        ASSERT_THAT(l2.ToVector(), testing::ElementsAre());
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre());
        l3 = {1, 2, 3, 4, 5};
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        BiDirectionalList<int> l4 = l3;

        l3 = l3;
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        l3 = std::move(l3);
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));

        BiDirectionalList<int> l;
        std::initializer_list<int> el = {1, 2, 3, 4};
        l = el;
    }

    TEST(BDList, BasicOperations) {
        BiDirectionalList<int> l;
        ASSERT_EQ(l.Size(), 0);
        ASSERT_TRUE(l.IsEmpty());

        l.PushFront(0);
        ASSERT_EQ(l.Size(), 1);
        ASSERT_FALSE(l.IsEmpty());
        ASSERT_EQ(l.Front()->value, 0);
        ASSERT_EQ(l.Back()->value, 0);

        l.PushFront(1);
        ASSERT_EQ(l.Size(), 2);
        ASSERT_EQ(l.Front()->value, 1);
        ASSERT_EQ(l.Back()->value, 0);

        l.PushBack(1);
        ASSERT_EQ(l.Size(), 3);
        ASSERT_EQ(l.Front()->value, 1);
        ASSERT_EQ(l.Back()->value, 1);
        ASSERT_THAT(l.ToVector(), testing::ElementsAre(1, 0, 1));

        l.PopFront();
        ASSERT_EQ(l.Size(), 2);
        ASSERT_EQ(l.Front()->value, 0);
        ASSERT_EQ(l.Back()->value, 1);
        ASSERT_THAT(l.ToVector(), testing::ElementsAre(0, 1));

        l.PopBack();
        ASSERT_EQ(l.Size(), 1);
        ASSERT_EQ(l.Front()->value, 0);
        ASSERT_EQ(l.Back()->value, 0);
        ASSERT_THAT(l.ToVector(), testing::ElementsAre(0));

        l.PopBack();
        ASSERT_EQ(l.Size(), 0);
        ASSERT_TRUE(l.ToVector().empty());
        ASSERT_TRUE(l.IsEmpty());
    }

    TEST(BDList, Constructors) {
        BiDirectionalList<int> l1;
        ASSERT_THAT(l1.ToVector(), testing::ElementsAre());
        BiDirectionalList<int> l2({1, 2, 3, 4, 5});
        ASSERT_THAT(l2.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        BiDirectionalList<int> l3(l2);
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
    }

    TEST(BDList, OtherMethods) {
        BiDirectionalList<int> l({1, 2, 3, 4, 5});
        ASSERT_EQ(l.Find(1), 0);
        ASSERT_EQ(l.Find(2), 1);
        ASSERT_EQ(l.Find(3), 2);
        ASSERT_EQ(l.Find(4), 3);
        ASSERT_EQ(l.Find(5), 4);
        ASSERT_EQ(l.Size(), 5);

        l.InsertAfter(l[0], 5);
        ASSERT_THAT(l.ToVector(), testing::ElementsAre(1, 5, 2, 3, 4, 5));
        ASSERT_EQ(l.Size(), 6);

        l.Erase(l[1]);
        l.Erase(l[1]);
        l.Erase(l[1]);
        ASSERT_EQ(l.Size(), 3);

        ASSERT_THAT(l.ToVector(), testing::ElementsAre(1, 4, 5));
        l.InsertAfter(l[2], 25);
        l.InsertBefore(l[0], 25);
        ASSERT_EQ(l.Size(), 5);

        ASSERT_THAT(l.ToVector(), testing::ElementsAre(25, 1, 4, 5, 25));
        ASSERT_THAT(l.FindAll(25), testing::ElementsAre(0, 4));
    }

    TEST(BDList, MoveSemanticsAndOperators) {
        BiDirectionalList<int> l1({1, 2, 3, 4, 5});
        BiDirectionalList<int> l2 = l1;
        ASSERT_THAT(l1.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        ASSERT_THAT(l2.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));

        BiDirectionalList<int> l3(std::move(l1));
        ASSERT_THAT(l3.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        ASSERT_TRUE(l1.IsEmpty());

        BiDirectionalList<int> l4 = std::move(l2);
        ASSERT_THAT(l4.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        ASSERT_TRUE(l2.IsEmpty());

        ASSERT_EQ(l3, l4);
        ASSERT_NE(l2, l4);
    }

    TEST(BDList, Const) {
        const BiDirectionalList<int> l({1, 2, 3, 4, 5});
        ASSERT_EQ(l.Size(), 5);
        ASSERT_FALSE(l.IsEmpty());
        ASSERT_TRUE(l.Front()->value == 1);
        ASSERT_TRUE(l.Back()->value == 5);
        ASSERT_THAT(l.ToVector(), testing::ElementsAre(1, 2, 3, 4, 5));
        const BiDirectionalList<int> l2({1, 2, 3, 4, 5});
        const BiDirectionalList<int> l3({1, 2, 3, 4});

        ASSERT_EQ(l, l2);
        ASSERT_NE(l, l3);
    }

    TEST(QTests, BasicOperations1) {
        Queue<int> q1;
        ASSERT_TRUE(q1.IsEmpty());
        ASSERT_EQ(q1.Size(), 0);

        Queue<int> q2({1, 2, 3, 4, 5});
        ASSERT_EQ(q2.Size(), 5);
        ASSERT_FALSE(q2.IsEmpty());

        std::vector<int> output;
        output.push_back(q2.Get());

        q2.Pop();
        ASSERT_EQ(q2.Size(), 4);

        while (!q2.IsEmpty()) {
            output.push_back(q2.Get());
            q2.Pop();
        }

        ASSERT_THAT(output, testing::ElementsAre(5, 4, 3, 2, 1));

        output.clear();
        q2.Push(1);
        ASSERT_EQ(q2.Size(), 1);
        q2.Push(2);

        while (!q2.IsEmpty()) {
            output.push_back(q2.Get());
            q2.Pop();
        }

        ASSERT_THAT(output, testing::ElementsAre(1, 2));

        Queue<int> q3;
        Queue<int> q4({2, 5, 10, 11});

        ASSERT_EQ(q2, q3);
        ASSERT_NE(q2, q4);
    }

    TEST(SomeTest, t) {
        Queue<int> q;
        q.Push(1);
        q.Push(2);

        int val1 = q.Get();

    }

    TEST(Container, Sample_Queue1) {
        std::vector<int> input = {1, 2, 3, 4, 5};
        std::vector<int> output;
        if (GetVariant() == Variant::kQueue) {
            output = {1, 2, 3, 4, 5};
        } else {
            output = {5, 4, 3, 2, 1};
        }

        Container<int> container;
        ASSERT_EQ(container.Size(), 0);
        ASSERT_TRUE(container.IsEmpty());

        for (int i = 0; i < input.size(); ++i) {
            container.Push(input[i]);
            ASSERT_EQ(container.Size(), i + 1);
            ASSERT_FALSE(container.IsEmpty());
        }

        for (int i = 0; i < input.size(); ++i) {
            ASSERT_EQ(container.Get(), output[i]);
            container.Pop();
            ASSERT_EQ(container.Size(), output.size() - i - 1);
            ASSERT_EQ(container.IsEmpty(), i == input.size() - 1);
        }
    }

    TEST(other, const_tests) {
        Queue<int> q({2, 3, 4, 5});
        const Queue<int> q1 = q;
        ASSERT_EQ(q1.Get(), 5);
    }

    TEST(Queue_Test, Constructors) {
        std::function<bool(Queue<int>, const std::vector<int> &)> Equal =
                [](Queue<int> q, const std::vector<int> &vec) {
                    std::vector<int> outp;
                    while (!q.IsEmpty()) {
                        outp.push_back(q.Get());
                        q.Pop();
                    }
                    std::reverse(outp.begin(), outp.end());
                    if (vec.size() == outp.size()) {
                        for (int i = 0; i < vec.size(); i++)
                            if (vec[i] != outp[i]) {
                                return false;
                            }
                        return true;
                    }
                    return false;
                };

        Queue<int> q1({1, 2, 3, 4, 5});
        ASSERT_TRUE(Equal(q1, {1, 2, 3, 4, 5}));
        Queue<int> q2 = q1;
        ASSERT_TRUE(Equal(q2, {1, 2, 3, 4, 5}));
        Queue<int> q3 = std::move(q1);
        ASSERT_TRUE(q1.IsEmpty());
        ASSERT_TRUE(Equal(q3, {1, 2, 3, 4, 5}));
        Queue<int> q4(std::move(q3));
        ASSERT_TRUE(q3.IsEmpty());
        ASSERT_TRUE(Equal(q4, {1, 2, 3, 4, 5}));
        Queue<int> q5(q4);
        ASSERT_TRUE(Equal(q5, {1, 2, 3, 4, 5}));
        q4 = q4;
        ASSERT_TRUE(Equal(q4, {1, 2, 3, 4, 5}));
        q5 = std::move(q5);
        ASSERT_TRUE(Equal(q5, {1, 2, 3, 4, 5}));
        Queue<int> q6;
        ASSERT_TRUE(Equal(q6, {}));
        Queue<int> q7(std::move(q7));
        ASSERT_TRUE(Equal(q7, {}));
    }

    TEST(Queue_Test, PushPop) {
        Queue<int> q;
        q.Push(1);
        ASSERT_EQ(q.Get(), 1);
        q.Push(2);
        ASSERT_EQ(q.Get(), 1);
        q.Pop();
        ASSERT_EQ(q.Get(), 2);
        q.Pop();
        ASSERT_TRUE(q.IsEmpty());
    }

}  // namespace containers
