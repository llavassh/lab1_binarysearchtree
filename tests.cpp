
#include "BinarySearchTree.hpp"
#include <iostream>

int passed = 0;
int failed = 0;

#define CHECK(condition) \
    do { \
        if (condition) passed++; \
        else { \
            failed++; \
            std::cout << "Failed: " << #condition << " (line " << __LINE__ << ")\n"; \
        } \
    } while (false)

static bool IsEven(const int& item) { return item % 2 == 0; }
static bool IsOdd(const int& item) { return item % 2 != 0; }
static int Double(const int& item) { return item * 2; }
static int ToZero(const int& item) { return 0; }
static int Sum(const int& a, const int& b) { return a + b; }
static void AddValue(const int& value, DynamicArray<int>& array) {
    array.Add(value);
}

void TestInsert() {
    BinarySearchTree<int> tree;
    CHECK(tree.IsEmpty());
    CHECK(tree.GetSize() == 0);
    CHECK(tree.Insert(10));
    CHECK(tree.Insert(5));
    CHECK(tree.Insert(15));
    CHECK(!tree.Insert(10));
    CHECK(!tree.Insert(5));
    CHECK(tree.GetSize() == 3);
    CHECK(!tree.IsEmpty());
}

void TestFind() {
    BinarySearchTree<int> tree;
    CHECK(!tree.Find(10));
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    CHECK(tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(15));
    CHECK(!tree.Find(7));
    CHECK(!tree.Find(20));
}

void TestRemoveLeaf() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    CHECK(tree.Remove(5));
    CHECK(!tree.Find(5));
    CHECK(tree.Find(10));
    CHECK(tree.Find(15));
    CHECK(tree.GetSize() == 2);
}

void TestRemoveNodeWithLeftChild() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(3);
    CHECK(tree.Remove(5));
    CHECK(!tree.Find(5));
    CHECK(tree.Find(3));
    CHECK(tree.Find(10));
    CHECK(tree.GetSize() == 2);
}

void TestRemoveNodeWithRightChild() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(7);
    CHECK(tree.Remove(5));
    CHECK(!tree.Find(5));
    CHECK(tree.Find(7));
    CHECK(tree.Find(10));
    CHECK(tree.GetSize() == 2);
}

void TestRemoveRootWithOneChild() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    CHECK(tree.Remove(10));
    CHECK(!tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.GetSize() == 1);
}

void TestRemoveRootWithTwoChildren() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);
    CHECK(tree.Remove(10));
    CHECK(!tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(15));
    CHECK(tree.Find(20));
    CHECK(tree.GetSize() == 3);
}

void TestRemoveSuccessorWithRightChild() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);
    tree.Insert(17);
    CHECK(tree.Remove(10));
    CHECK(!tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(15));
    CHECK(tree.Find(17));
    CHECK(tree.Find(20));
    CHECK(tree.GetSize() == 4);
}

void TestRemoveLastElement() {
    BinarySearchTree<int> tree;
    tree.Insert(42);
    CHECK(tree.Remove(42));
    CHECK(tree.IsEmpty());
    CHECK(tree.GetSize() == 0);
}

void TestRemoveNonexistent() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    CHECK(!tree.Remove(20));
    CHECK(!tree.Remove(7));
    CHECK(tree.GetSize() == 2);
    CHECK(tree.Find(10));
    CHECK(tree.Find(5));
}

void TestWhere() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);

    auto even = tree.Where(IsEven);
    CHECK(even.GetSize() == 2);
    CHECK(even.Find(10));
    CHECK(even.Find(20));
    CHECK(!even.Find(5));
    CHECK(!even.Find(15));

    auto odd = tree.Where(IsOdd);
    CHECK(odd.GetSize() == 2);
    CHECK(odd.Find(5));
    CHECK(odd.Find(15));
    CHECK(!odd.Find(10));
    CHECK(!odd.Find(20));

    CHECK(tree.GetSize() == 4);
    CHECK(tree.Find(10));
    CHECK(tree.Find(5));
    CHECK(tree.Find(20));
    CHECK(tree.Find(15));
}

void TestWhereEmpty() {
    BinarySearchTree<int> tree;
    auto result = tree.Where(IsEven);
    CHECK(result.IsEmpty());
    CHECK(result.GetSize() == 0);
}

void TestMap() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);

    auto result = tree.Map(Double);
    CHECK(result.GetSize() == 4);
    CHECK(result.Find(20));
    CHECK(result.Find(10));
    CHECK(result.Find(40));
    CHECK(result.Find(30));
    CHECK(tree.GetSize() == 4);
}

void TestMapDuplicates() {
    BinarySearchTree<int> tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    auto result = tree.Map(ToZero);
    CHECK(result.GetSize() == 1);
    CHECK(result.Find(0));
}

void TestReduce() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);
    CHECK(tree.Reduce(0, Sum) == 50);
    CHECK(tree.Reduce(100, Sum) == 150);
    CHECK(tree.Reduce(1, Sum) == 51);
    CHECK(tree.GetSize() == 4);
}

void TestReduceEmpty() {
    BinarySearchTree<int> tree;
    CHECK(tree.Reduce(10, Sum) == 10);
}

void TestIsBalanced() {
    BinarySearchTree<int> tree;
    CHECK(tree.IsBalanced());
    tree.Insert(10);
    CHECK(tree.IsBalanced());
    tree.Insert(5);
    CHECK(tree.IsBalanced());
    tree.Insert(15);
    CHECK(tree.IsBalanced());
    tree.Insert(3);
    CHECK(tree.IsBalanced());
    tree.Insert(20);
    CHECK(tree.IsBalanced());
}

void TestIsNotBalanced() {
    BinarySearchTree<int> tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);
    CHECK(!tree.IsBalanced());
}

void TestBalance() {
    for (size_t count = 0; count <= 20; count++) {
        BinarySearchTree<int> tree;
        for (size_t i = 1; i <= count; i++) tree.Insert(static_cast<int>(i));

        auto result = tree.Balance();
        CHECK(result.GetSize() == count);
        CHECK(result.IsBalanced());

        for (size_t i = 1; i <= count; i++) CHECK(result.Find(static_cast<int>(i)));

        CHECK(tree.GetSize() == count);
        for (size_t i = 1; i <= count; i++) CHECK(tree.Find(static_cast<int>(i)));
    }
}

void TestCreateSubTree() {
    BinarySearchTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3);
    tree.Insert(7);
    tree.Insert(12);
    tree.Insert(18);

    BinarySearchTree<int> rootSubTree = tree.CreateSubTree(10);
    CHECK(rootSubTree.GetSize() == 7);
    CHECK(rootSubTree.Find(10));
    CHECK(rootSubTree.Find(5));
    CHECK(rootSubTree.Find(15));
    CHECK(rootSubTree.Find(3));
    CHECK(rootSubTree.Find(7));
    CHECK(rootSubTree.Find(12));
    CHECK(rootSubTree.Find(18));

    BinarySearchTree<int> subTree = tree.CreateSubTree(5);
    CHECK(subTree.GetSize() == 3);
    CHECK(subTree.Find(5));
    CHECK(subTree.Find(3));
    CHECK(subTree.Find(7));
    CHECK(!subTree.Find(10));
    CHECK(!subTree.Find(15));

    BinarySearchTree<int> emptyTree = tree.CreateSubTree(100);
    CHECK(emptyTree.IsEmpty());
    CHECK(emptyTree.GetSize() == 0);

    subTree.Insert(4);
    subTree.Remove(3);

    CHECK(subTree.Find(4));
    CHECK(!subTree.Find(3));

    CHECK(tree.Find(3));
    CHECK(!tree.Find(4));
    CHECK(tree.GetSize() == 7);
}

void TestThread() {
    BinarySearchTree<int> tree;

    int values[] = {4, 2, 6, 1, 3, 5, 7};
    for (int value : values) {
        tree.Insert(value);
    }

    const std::string orders[] = {
        "KLR", "KRL",
        "LKR", "RKL",
        "LRK", "RLK"
    };

    const int expected[][7] = {
        {4, 2, 1, 3, 6, 5, 7},
        {4, 6, 7, 5, 2, 3, 1},
        {1, 2, 3, 4, 5, 6, 7},
        {7, 6, 5, 4, 3, 2, 1},
        {1, 3, 2, 5, 7, 6, 4},
        {7, 5, 6, 3, 1, 2, 4}
    };

    for (int i = 0; i < 6; ++i) {
        DynamicArray<int> result;

        tree.ThreadAndTraverse(orders[i], AddValue, result);

        CHECK(result.GetSize() == 7);

        for (int j = 0; j < 7; ++j) {
            CHECK(result[j] == expected[i][j]);
        }
    }

    tree.Thread("LKR");

    CHECK(tree.Insert(8));
    CHECK(tree.Find(8));

    DynamicArray<int> resultAfterInsert;
    tree.TraverseThreaded(AddValue, resultAfterInsert);

    CHECK(resultAfterInsert.GetSize() == 0);

    tree.ThreadAndTraverse("LKR", AddValue, resultAfterInsert);

    CHECK(resultAfterInsert.GetSize() == 8);
    CHECK(resultAfterInsert[7] == 8);

    CHECK(tree.Remove(4));
    CHECK(!tree.Find(4));

    DynamicArray<int> resultAfterRemove;
    tree.TraverseThreaded(AddValue, resultAfterRemove);

    CHECK(resultAfterRemove.GetSize() == 0);

    tree.ThreadAndTraverse("LKR", AddValue, resultAfterRemove);

    CHECK(resultAfterRemove.GetSize() == 7);

    int expectedAfterRemove[] = {1, 2, 3, 5, 6, 7, 8};
    for (size_t i = 0; i < 7; ++i) {
        CHECK(resultAfterRemove[i] == expectedAfterRemove[i]);
    }
}

void TestToString() {
    BinarySearchTree<int> tree;
    tree.Insert(2);
    tree.Insert(1);
    tree.Insert(3);

    CHECK(tree.ToString("{K}(L)[R]") == "{2}({1}()[])[{3}()[]]");
}

void TestToStringOrders() {
    BinarySearchTree<int> tree;
    tree.Insert(2);
    tree.Insert(1);
    tree.Insert(3);

    CHECK(tree.ToString("KLR") == "213");
    CHECK(tree.ToString("KRL") == "231");
    CHECK(tree.ToString("LKR") == "123");
    CHECK(tree.ToString("LRK") == "132");
    CHECK(tree.ToString("RKL") == "321");
    CHECK(tree.ToString("RLK") == "312");
}

void TestToStringEdgeCases() {
    BinarySearchTree<int> emptyTree;
    CHECK(emptyTree.ToString("{K}(L)[R]") == "");

    BinarySearchTree<int> leafTree;
    leafTree.Insert(7);
    CHECK(leafTree.ToString("{K}(L)[R]") == "{7}()[]");
}

void TestToStringComplexTree() {
    BinarySearchTree<int> tree;
    tree.Insert(4);
    tree.Insert(2);
    tree.Insert(6);
    tree.Insert(1);
    tree.Insert(3);
    tree.Insert(5);

    CHECK(tree.ToString("{K}(L)[R]") == "{4}({2}({1}()[])[{3}()[]])[{6}({5}()[])[]]");
}

int main() {
    TestInsert();
    TestFind();
    TestRemoveLeaf();
    TestRemoveNodeWithLeftChild();
    TestRemoveNodeWithRightChild();
    TestRemoveRootWithOneChild();
    TestRemoveRootWithTwoChildren();
    TestRemoveSuccessorWithRightChild();
    TestRemoveLastElement();
    TestRemoveNonexistent();
    TestWhere();
    TestWhereEmpty();
    TestMap();
    TestMapDuplicates();
    TestReduce();
    TestReduceEmpty();
    TestIsBalanced();
    TestIsNotBalanced();
    TestBalance();
    TestCreateSubTree();
    TestThread();
    TestToString();
    TestToStringOrders();
    TestToStringEdgeCases();
    TestToStringComplexTree();

    std::cout << "\nPassed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
