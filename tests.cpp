
#include "BinarySearchTree.hpp"
#include <cassert>
#include <iostream>

int passed = 0;
int failed = 0;

#define CHECK(condition) \
    do { \
        if (condition) { \
            passed++; \
        } else { \
            failed++; \
        } \
    } while (false)

static bool IsEven(const int& item) {
    return item % 2 == 0;
}

static bool IsOdd(const int& item) {
    return item % 2 != 0;
}

static int Double(const int& item) {
    return item * 2;
}

static int Sum(const int& a, const int& b) {
    return a + b;
}

void TestInsert() {
    BinarySearchTree<int> tree;

    CHECK(tree.IsEmpty());
    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(15));
    CHECK(!tree.Insert(10));
    CHECK(tree.GetSize() == 3);
}

void TestFind() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(15));

    CHECK(tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(15));
    CHECK(!tree.Find(11));
}

void TestRemoveLeaf() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(15));

    CHECK(tree.Remove(5));
    CHECK(!tree.Find(5));
    CHECK(tree.Find(10));
    CHECK(tree.Find(15));
    CHECK(tree.GetSize() == 2);
}

void TestRemoveTwoChildren() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(20));
    CHECK(tree.Insert(15));

    CHECK(tree.Remove(10));
    CHECK(!tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(15));
    CHECK(tree.Find(20));
    CHECK(tree.GetSize() == 3);
}

void TestRemoveRootWithOneChild() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));

    CHECK(tree.Remove(10));
    CHECK(!tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.GetSize() == 1);
}

void TestRemoveLastElement() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(42));
    CHECK(tree.Remove(42));

    CHECK(tree.IsEmpty());
    CHECK(tree.GetSize() == 0);
}

void TestWhere() {
    BinarySearchTree<int> tree;

    CHECK(tree.IsEmpty());
    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(20));
    CHECK(tree.Insert(15));
    CHECK(tree.GetSize() == 4);

    auto where = tree.Where(IsEven);
    CHECK(where.Find(10));
    CHECK(where.Find(20));
    CHECK(!where.Find(5));
    CHECK(!where.Find(15));
    CHECK(where.GetSize() == 2);

    auto where2 = tree.Where(IsOdd);
    CHECK(!where2.Find(10));
    CHECK(!where2.Find(20));
    CHECK(where2.Find(5));
    CHECK(where2.Find(15));
    CHECK(where2.GetSize() == 2);
}

void TestMap() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(20));
    CHECK(tree.Insert(15));
    CHECK(tree.GetSize() == 4);

    auto map = tree.Map(Double);
    CHECK(map.Find(10));
    CHECK(map.Find(20));
    CHECK(map.Find(30));
    CHECK(map.Find(40));
    CHECK(map.GetSize() == 4);
}

void TestReduce() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(20));
    CHECK(tree.Insert(15));
    CHECK(tree.GetSize() == 4);

    auto reduce = tree.Reduce(0, Sum);
    CHECK(reduce == 50);
}

void TestBalance() {
    BinarySearchTree<int> tree;

    CHECK(tree.Insert(1));
    CHECK(tree.Insert(2));
    CHECK(tree.Insert(3));
    CHECK(tree.Insert(4));
    CHECK(tree.Insert(5));
    CHECK(!tree.IsBalanced());
    auto result = tree.Balance();
    CHECK(result.IsBalanced());
}

int main() {
    TestInsert();
    TestFind();
    TestRemoveLeaf();
    TestRemoveTwoChildren();
    TestRemoveRootWithOneChild();
    TestRemoveLastElement();
    TestWhere();
    TestMap();
    TestReduce();
    TestBalance();

    std::cout << "Passed tests: " << passed << '\n';
    std::cout << "Failed tests: " << failed << '\n';

    return 0;
}
