#include <catch2/catch_all.hpp>

#include "structures/structures.hpp"
#include "timer.hpp"
using namespace Catch;

TEST_CASE("ArrayList") {
    data_structures::list::ArrayList<int> aList{};

    SECTION("Copy") {
        SECTION("Assignment") {
            aList.add(10);
            aList.add(20);
            aList.add(30);

            CAPTURE(aList);
            auto copyList = aList;
            REQUIRE(copyList.size() == 3);
            REQUIRE(copyList.at(0) == 10);
            REQUIRE(copyList.at(1) == 20);
            REQUIRE(copyList.at(2) == 30);

            copyList.at(2) = 40;
            REQUIRE(copyList.at(2) == 40);
            REQUIRE(aList.at(2) == 30);
        }

        SECTION("Construction") {
            aList.add(10);
            aList.add(20);
            aList.add(30);

            CAPTURE(aList);
            auto copyList{aList};
            REQUIRE(copyList.size() == 3);
            REQUIRE(copyList.at(0) == 10);
            REQUIRE(copyList.at(1) == 20);
            REQUIRE(copyList.at(2) == 30);
        }
    }

    SECTION("Move") {
        SECTION("Assignment") {
            aList.add(10);
            aList.add(20);
            aList.add(30);

            CAPTURE(aList);
            auto moveList = std::move(aList);
            REQUIRE(moveList.size() == 3);
            REQUIRE(moveList.at(0) == 10);
            REQUIRE(moveList.at(1) == 20);
            REQUIRE(moveList.at(2) == 30);

            CAPTURE(aList);
            REQUIRE(aList.size() == 0);
            REQUIRE(aList.isEmpty());
        }

        SECTION("Construction") {
            aList.add(10);
            aList.add(20);
            aList.add(30);

            CAPTURE(aList);
            auto moveList{std::move(aList)};
            REQUIRE(moveList.size() == 3);
            REQUIRE(moveList.at(0) == 10);
            REQUIRE(moveList.at(1) == 20);
            REQUIRE(moveList.at(2) == 30);

            CAPTURE(aList);
            REQUIRE(aList.size() == 0);
            REQUIRE(aList.isEmpty());
        }
    }

    SECTION("Add elements") {
        REQUIRE(aList.isEmpty());

        aList.add(1);
        aList.add(2);
        aList.add(3);
        aList.add(4);
        aList.add(5);

        REQUIRE(aList.size() == 5);
        REQUIRE(aList.at(0) == 1);
        REQUIRE(aList.at(1) == 2);
        REQUIRE(aList.at(2) == 3);
        REQUIRE(aList.at(3) == 4);
        REQUIRE(aList.at(4) == 5);
    }

    SECTION("Remove elements") {
        aList.add(1);
        aList.add(2);
        aList.add(3);
        aList.add(4);
        aList.add(5);

        aList.remove(2);  // Remove element at index 2
        aList.remove(2);  // Remove element which was originally at index 3

        REQUIRE(aList.size() == 3);
        REQUIRE(aList.at(0) == 1);
        REQUIRE(aList.at(1) == 2);
        REQUIRE(aList.at(2) == 5);
    }

    SECTION("Throw exception when removing from empty list") { REQUIRE_THROWS_AS(aList.remove(0), std::out_of_range); }

    SECTION("Throw exception when accessing out of bounds") { REQUIRE_THROWS_AS(aList.at(0), std::out_of_range); }
}

TEST_CASE("LinkedLists") {
    data_structures::list::LinkedList<int> lList{};
    data_structures::base::list::SinglyLinkedList<int> sList{};
    data_structures::base::list::DoublyLinkedList<int> dList{};

    SECTION("Copy") {
        SECTION("Construction") {
            lList.add(10);
            lList.add(20);
            lList.add(30);

            dList.add(10);
            dList.add(20);
            dList.add(30);

            sList.add(10);
            sList.add(20);
            sList.add(30);

            auto copyLL{lList};
            REQUIRE(copyLL.size() == 3);
            REQUIRE(copyLL.at(0) == 10);
            REQUIRE(copyLL.at(1) == 20);
            REQUIRE(copyLL.at(2) == 30);
            copyLL.at(2) = 40;
            REQUIRE(copyLL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);

            auto copyDL{dList};
            REQUIRE(copyDL.size() == 3);
            REQUIRE(copyDL.at(0) == 10);
            REQUIRE(copyDL.at(1) == 20);
            REQUIRE(copyDL.at(2) == 30);
            copyDL.at(2) = 40;
            REQUIRE(copyDL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);

            auto copySL{sList};
            REQUIRE(copySL.size() == 3);
            REQUIRE(copySL.at(0) == 10);
            REQUIRE(copySL.at(1) == 20);
            REQUIRE(copySL.at(2) == 30);
            copySL.at(2) = 40;
            REQUIRE(copySL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);
        }

        SECTION("Assignment") {
            lList.add(10);
            lList.add(20);
            lList.add(30);

            dList.add(10);
            dList.add(20);
            dList.add(30);

            sList.add(10);
            sList.add(20);
            sList.add(30);

            auto copyLL = lList;
            REQUIRE(copyLL.size() == 3);
            REQUIRE(copyLL.at(0) == 10);
            REQUIRE(copyLL.at(1) == 20);
            REQUIRE(copyLL.at(2) == 30);
            copyLL.at(2) = 40;
            REQUIRE(copyLL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);

            auto copySL = sList;
            REQUIRE(copySL.size() == 3);
            REQUIRE(copySL.at(0) == 10);
            REQUIRE(copySL.at(1) == 20);
            REQUIRE(copySL.at(2) == 30);
            copySL.at(2) = 40;
            REQUIRE(copySL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);

            auto copyDL = dList;
            REQUIRE(copyDL.size() == 3);
            REQUIRE(copyDL.at(0) == 10);
            REQUIRE(copyDL.at(1) == 20);
            REQUIRE(copyDL.at(2) == 30);
            copyDL.at(2) = 40;
            REQUIRE(copyDL.at(2) == 40);
            REQUIRE(lList.at(2) == 30);
        }
    }

    SECTION("Move") {
        SECTION("Construction") {
            lList.add(10);
            lList.add(20);
            lList.add(30);

            dList.add(10);
            dList.add(20);
            dList.add(30);

            sList.add(10);
            sList.add(20);
            sList.add(30);

            auto moveLL{std::move(lList)};
            REQUIRE(moveLL.size() == 3);
            REQUIRE(moveLL.at(0) == 10);
            REQUIRE(moveLL.at(1) == 20);
            REQUIRE(moveLL.at(2) == 30);
            REQUIRE(lList.size() == 0);
            REQUIRE(lList.isEmpty());

            auto moveSL{std::move(sList)};
            REQUIRE(moveSL.size() == 3);
            REQUIRE(moveSL.at(0) == 10);
            REQUIRE(moveSL.at(1) == 20);
            REQUIRE(moveSL.at(2) == 30);
            REQUIRE(sList.size() == 0);
            REQUIRE(sList.isEmpty());

            auto moveDL{std::move(dList)};
            REQUIRE(moveDL.size() == 3);
            REQUIRE(moveDL.at(0) == 10);
            REQUIRE(moveDL.at(1) == 20);
            REQUIRE(moveDL.at(2) == 30);
            REQUIRE(dList.size() == 0);
            REQUIRE(dList.isEmpty());
        }

        SECTION("Assignment") {
            lList.add(10);
            lList.add(20);
            lList.add(30);

            dList.add(10);
            dList.add(20);
            dList.add(30);

            sList.add(10);
            sList.add(20);
            sList.add(30);

            auto moveLL = std::move(lList);
            REQUIRE(moveLL.size() == 3);
            REQUIRE(moveLL.at(0) == 10);
            REQUIRE(moveLL.at(1) == 20);
            REQUIRE(moveLL.at(2) == 30);
            REQUIRE(lList.size() == 0);
            REQUIRE(lList.isEmpty());

            auto moveSL = std::move(sList);
            REQUIRE(moveSL.size() == 3);
            REQUIRE(moveSL.at(0) == 10);
            REQUIRE(moveSL.at(1) == 20);
            REQUIRE(moveSL.at(2) == 30);
            REQUIRE(sList.size() == 0);
            REQUIRE(sList.isEmpty());

            auto moveDL = std::move(dList);
            REQUIRE(moveDL.size() == 3);
            REQUIRE(moveDL.at(0) == 10);
            REQUIRE(moveDL.at(1) == 20);
            REQUIRE(moveDL.at(2) == 30);
            REQUIRE(dList.size() == 0);
            REQUIRE(dList.isEmpty());
        }
    }

    SECTION("Add elements") {
        lList.add(1);
        lList.add(2);
        lList.add(3);
        lList.add(4);
        lList.add(5);

        sList.add(1);
        sList.add(2);
        sList.add(3);
        sList.add(4);
        sList.add(5);

        dList.add(1);
        dList.add(2);
        dList.add(3);
        dList.add(4);
        dList.add(5);

        REQUIRE(lList.size() == 5);
        REQUIRE(sList.size() == 5);
        REQUIRE(dList.size() == 5);

        REQUIRE(lList.at(0) == 1);
        REQUIRE(lList.at(1) == 2);
        REQUIRE(lList.at(2) == 3);
        REQUIRE(lList.at(3) == 4);
        REQUIRE(lList.at(4) == 5);

        REQUIRE(sList.at(0) == 1);
        REQUIRE(sList.at(1) == 2);
        REQUIRE(sList.at(2) == 3);
        REQUIRE(sList.at(3) == 4);
        REQUIRE(sList.at(4) == 5);

        REQUIRE(dList.at(0) == 1);
        REQUIRE(dList.at(1) == 2);
        REQUIRE(dList.at(2) == 3);
        REQUIRE(dList.at(3) == 4);
        REQUIRE(dList.at(4) == 5);
    }

    SECTION("Remove elements") {
        lList.add(1);
        lList.add(2);
        lList.add(3);
        lList.add(4);
        lList.add(5);

        sList.add(1);
        sList.add(2);
        sList.add(3);
        sList.add(4);
        sList.add(5);

        dList.add(1);
        dList.add(2);
        dList.add(3);
        dList.add(4);
        dList.add(5);

        lList.remove(2);
        lList.remove(3);
    }

    SECTION("Throw exception when removing from empty list") {
        REQUIRE_THROWS(lList.remove(0));
        REQUIRE_THROWS(sList.remove(0));
        REQUIRE_THROWS(dList.remove(0));
    }

    SECTION("Throw exception when accessing out of bounds") {
        REQUIRE_THROWS(lList.at(0));
        REQUIRE_THROWS(sList.at(0));
        REQUIRE_THROWS(dList.at(0));
    }

    SECTION("Insert elements") {
        sList.insert(0, 1);
        sList.insert(1, 2);
        sList.insert(2, 3);
        sList.insert(3, 4);
        sList.insert(4, 5);

        dList.insert(0, 1);
        dList.insert(1, 2);
        dList.insert(2, 3);
        dList.insert(3, 4);
        dList.insert(4, 5);

        REQUIRE(sList.size() == 5);
        REQUIRE(dList.size() == 5);

        REQUIRE(sList.at(0) == 1);
        REQUIRE(sList.at(1) == 2);
        REQUIRE(sList.at(2) == 3);
        REQUIRE(sList.at(3) == 4);
        REQUIRE(sList.at(4) == 5);

        REQUIRE(dList.at(0) == 1);
        REQUIRE(dList.at(1) == 2);
        REQUIRE(dList.at(2) == 3);
        REQUIRE(dList.at(3) == 4);
        REQUIRE(dList.at(4) == 5);
    }

    SECTION("Remove elements") {
        sList.add(1);
        sList.add(2);
        sList.add(3);
        sList.add(4);
        sList.add(5);

        dList.add(1);
        dList.add(2);
        dList.add(3);
        dList.add(4);
        dList.add(5);

        sList.remove(2);
        sList.remove(3);

        dList.remove(2);
        dList.remove(3);

        REQUIRE(sList.size() == 3);
        REQUIRE(dList.size() == 3);

        REQUIRE(sList.at(0) == 1);
        REQUIRE(sList.at(1) == 2);
        REQUIRE(sList.at(2) == 4);

        REQUIRE(dList.at(0) == 1);
        REQUIRE(dList.at(1) == 2);
        REQUIRE(dList.at(2) == 4);
    }
}

TEST_CASE("BinaryTree") {
    SECTION("Add elements") {
        data_structures::tree::LinkedBinaryTree<int> bTree{0};
        auto root = bTree.root();
        bTree.addLeft(root, 1);
        bTree.addRight(root, 2);
        bTree.addLeft(bTree.left(root), 3);
        bTree.addRight(bTree.left(root), 4);
        bTree.addLeft(bTree.right(root), 5);
        bTree.addRight(bTree.right(root), 6);

        REQUIRE(bTree.size() == 7);
    }

    SECTION("Remove elements") {
        data_structures::tree::LinkedBinaryTree<int> bTree{0};
        auto root = bTree.root();
        bTree.addLeft(root, 1);
        bTree.addRight(root, 2);
        bTree.addLeft(bTree.left(root), 3);
        bTree.addRight(bTree.left(root), 4);
        bTree.addLeft(bTree.right(root), 5);
        bTree.addRight(bTree.right(root), 6);

        bTree.remove(bTree.left(root));
        bTree.remove(bTree.right(root));

        REQUIRE(bTree.size() == 5);
    }

    SECTION("Throw exception when removing from empty tree") {
        data_structures::tree::LinkedBinaryTree<int> bTree{0};
        auto root = bTree.root();
        REQUIRE_THROWS(bTree.remove(root));
    }
}

TEST_CASE("Stack") {
    data_structures::base::LinkedStack<int> lStack{};

    SECTION("Copy") {
        SECTION("Construction") {
            lStack.push(10);
            lStack.push(20);
            lStack.push(30);

            auto lCopy{lStack};
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.top() == 30);
            lCopy.pop();
            REQUIRE(lCopy.top() == 20);
            REQUIRE(lStack.top() == 30);
        }

        SECTION("Assignment") {
            lStack.push(10);
            lStack.push(20);
            lStack.push(30);

            auto lCopy = lStack;
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.top() == 30);
            lCopy.pop();
            REQUIRE(lCopy.top() == 20);
            REQUIRE(lStack.top() == 30);
        }
    }

    SECTION("Move") {
        SECTION("Construction") {
            lStack.push(10);
            lStack.push(20);
            lStack.push(30);

            auto lMove{std::move(lStack)};
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.top() == 30);
            REQUIRE(lStack.size() == 0);
            REQUIRE(lStack.isEmpty());
        }

        SECTION("Assignment") {
            lStack.push(10);
            lStack.push(20);
            lStack.push(30);

            auto lMove = std::move(lStack);
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.top() == 30);
            REQUIRE(lStack.size() == 0);
            REQUIRE(lStack.isEmpty());
        }
    }

    SECTION("Push elements") {
        lStack.push(1);
        lStack.push(2);
        lStack.push(3);
        lStack.push(4);
        lStack.push(5);

        REQUIRE(lStack.size() == 5);
        REQUIRE(lStack.top() == 5);
    }

    SECTION("Pop elements") {
        lStack.push(1);
        lStack.push(2);
        lStack.push(3);
        lStack.push(4);
        lStack.push(5);

        lStack.pop();
        lStack.pop();

        REQUIRE(lStack.size() == 3);
        REQUIRE(lStack.top() == 3);
    }

    SECTION("Throw exception when popping from empty stack") { REQUIRE_THROWS(lStack.pop()); }

    SECTION("Throw exception when accessing top of empty stack") { REQUIRE_THROWS(lStack.top()); }
}

TEST_CASE("Queue") {
    data_structures::base::LinkedQueue<int> lQueue{};

    SECTION("Copy") {
        SECTION("Construction") {
            lQueue.enqueue(10);
            lQueue.enqueue(20);
            lQueue.enqueue(30);

            auto lCopy{lQueue};
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.front() == 10);
            REQUIRE(lCopy.back() == 30);
            lCopy.dequeue();
            REQUIRE(lCopy.front() == 20);
            REQUIRE(lQueue.front() == 10);
        }

        SECTION("Assignment") {
            lQueue.enqueue(10);
            lQueue.enqueue(20);
            lQueue.enqueue(30);

            auto lCopy = lQueue;
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.front() == 10);
            REQUIRE(lCopy.back() == 30);
            lCopy.dequeue();
            REQUIRE(lCopy.front() == 20);
            REQUIRE(lQueue.front() == 10);
        }
    }

    SECTION("Move") {
        SECTION("Construction") {
            lQueue.enqueue(10);
            lQueue.enqueue(20);
            lQueue.enqueue(30);

            auto lMove{std::move(lQueue)};
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.front() == 10);
            REQUIRE(lMove.back() == 30);
            REQUIRE(lQueue.size() == 0);
            REQUIRE(lQueue.isEmpty());
        }

        SECTION("Assignment") {
            lQueue.enqueue(10);
            lQueue.enqueue(20);
            lQueue.enqueue(30);

            auto lMove = std::move(lQueue);
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.front() == 10);
            REQUIRE(lMove.back() == 30);
            REQUIRE(lQueue.size() == 0);
            REQUIRE(lQueue.isEmpty());
        }
    }

    SECTION("Enqueue elements") {
        lQueue.enqueue(1);
        lQueue.enqueue(2);
        lQueue.enqueue(3);
        lQueue.enqueue(4);
        lQueue.enqueue(5);

        REQUIRE(lQueue.size() == 5);
        REQUIRE(lQueue.front() == 1);
        REQUIRE(lQueue.back() == 5);
    }

    SECTION("Dequeue elements") {
        lQueue.enqueue(1);
        lQueue.enqueue(2);
        lQueue.enqueue(3);
        lQueue.enqueue(4);
        lQueue.enqueue(5);

        lQueue.dequeue();
        lQueue.dequeue();

        REQUIRE(lQueue.size() == 3);
        REQUIRE(lQueue.front() == 3);
        REQUIRE(lQueue.back() == 5);
    }

    SECTION("Throw exception when dequeuing from empty queue") { REQUIRE_THROWS(lQueue.dequeue()); }

    SECTION("Throw exception when accessing front of empty queue") { REQUIRE_THROWS(lQueue.front()); }

    SECTION("Throw exception when accessing back of empty queue") { REQUIRE_THROWS(lQueue.back()); }
}

TEST_CASE("Deque") {
    SECTION("Copy") {
        data_structures::base::LinkedDeque<int> lDeque{};

        SECTION("Construction") {
            lDeque.enqueueFront(10);
            lDeque.enqueueFront(20);
            lDeque.enqueueFront(30);

            auto lCopy{lDeque};
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.front() == 30);
            REQUIRE(lCopy.back() == 10);
            lCopy.dequeueFront();
            REQUIRE(lCopy.front() == 20);
            REQUIRE(lDeque.front() == 30);
        }

        SECTION("Assignment") {
            lDeque.enqueueFront(10);
            lDeque.enqueueFront(20);
            lDeque.enqueueFront(30);

            auto lCopy = lDeque;
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.front() == 30);
            REQUIRE(lCopy.back() == 10);
            lCopy.dequeueFront();
            REQUIRE(lCopy.front() == 20);
            REQUIRE(lDeque.front() == 30);
        }
    }

    SECTION("Move") {
        data_structures::base::LinkedDeque<int> lDeque{};

        SECTION("Construction") {
            lDeque.enqueueFront(10);
            lDeque.enqueueFront(20);
            lDeque.enqueueFront(30);

            auto lMove{std::move(lDeque)};
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.front() == 30);
            REQUIRE(lMove.back() == 10);
            REQUIRE(lDeque.size() == 0);
            REQUIRE(lDeque.isEmpty());
        }

        SECTION("Assignment") {
            lDeque.enqueueFront(10);
            lDeque.enqueueFront(20);
            lDeque.enqueueFront(30);

            auto lMove = std::move(lDeque);
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.front() == 30);
            REQUIRE(lMove.back() == 10);
            REQUIRE(lDeque.size() == 0);
            REQUIRE(lDeque.isEmpty());
        }
    }

    SECTION("enqueue elements") {
        data_structures::base::LinkedDeque<int> lDeque{};

        lDeque.enqueueFront(1);
        lDeque.enqueueFront(2);
        lDeque.enqueueBack(3);
        lDeque.enqueueBack(4);
        lDeque.enqueueFront(5);

        REQUIRE(lDeque.size() == 5);
        REQUIRE(lDeque.front() == 5);
        REQUIRE(lDeque.back() == 4);
    }

    SECTION("dequeue elements") {
        data_structures::base::LinkedDeque<int> lDeque{};

        lDeque.enqueueFront(1);
        lDeque.enqueueFront(2);
        lDeque.enqueueBack(3);
        lDeque.enqueueBack(4);
        lDeque.enqueueFront(5);

        lDeque.dequeueFront();
        lDeque.dequeueBack();

        REQUIRE(lDeque.size() == 3);
        REQUIRE(lDeque.front() == 2);
        REQUIRE(lDeque.back() == 3);
    }

    SECTION("Throw exception when removing from empty deque") {
        data_structures::base::LinkedDeque<int> lDeque{};
        REQUIRE_THROWS(lDeque.dequeueFront());
        REQUIRE_THROWS(lDeque.dequeueBack());
    }

    SECTION("Throw exception when accessing front of empty deque") {
        data_structures::base::LinkedDeque<int> lDeque{};
        REQUIRE_THROWS(lDeque.front());
    }

    SECTION("Throw exception when accessing back of empty deque") {
        data_structures::base::LinkedDeque<int> lDeque{};
        REQUIRE_THROWS(lDeque.back());
    }
}

TEST_CASE("Priority Queue") {
    data_structures::queue::LinkedPriorityQueue<int> lPQueue{};

    SECTION("Copy") {
        SECTION("Construction") {
            lPQueue.insert(100);
            lPQueue.insert(20);
            lPQueue.insert(30);

            // Testing copy construction
            auto lCopy{lPQueue};
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.min() == 20);
            lCopy.removeMin();
            REQUIRE(lCopy.min() == 30);
            REQUIRE(lPQueue.min() == 20);  // Ensure original is unaffected

            // Test that further insertions into the copy don't affect the original
            lCopy.insert(10);
            REQUIRE(lCopy.min() == 10);
            REQUIRE(lPQueue.min() == 20);
        }

        SECTION("Assignment") {
            lPQueue.insert(100);
            lPQueue.insert(20);
            lPQueue.insert(30);

            // Testing copy assignment
            auto lCopy = lPQueue;
            REQUIRE(lCopy.size() == 3);
            REQUIRE(lCopy.min() == 20);
            lCopy.removeMin();
            REQUIRE(lCopy.min() == 30);
            REQUIRE(lPQueue.min() == 20);  // Ensure original is unaffected

            // Ensure that modifying the copy does not affect the original
            lCopy.insert(10);
            REQUIRE(lCopy.min() == 10);
            REQUIRE(lPQueue.min() == 20);
        }
    }

    SECTION("Move") {
        SECTION("Construction") {
            lPQueue.insert(100);
            lPQueue.insert(20);
            lPQueue.insert(30);

            // Testing move construction
            auto lMove{std::move(lPQueue)};
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.min() == 20);
            REQUIRE(lPQueue.size() == 0);  // Ensure original is moved-from
            REQUIRE(lPQueue.isEmpty());

            // Test behavior of moved-from queue
            REQUIRE_THROWS_AS(lPQueue.min(), std::runtime_error);  // Assuming min() throws on empty
        }

        SECTION("Assignment") {
            lPQueue.insert(100);
            lPQueue.insert(20);
            lPQueue.insert(30);

            // Testing move assignment
            auto lMove = std::move(lPQueue);
            REQUIRE(lMove.size() == 3);
            REQUIRE(lMove.min() == 20);
            REQUIRE(lPQueue.size() == 0);  // Ensure original is moved-from
            REQUIRE(lPQueue.isEmpty());

            // Test behavior of moved-from queue
            REQUIRE_THROWS_AS(lPQueue.min(), std::runtime_error);  // Assuming min() throws on empty
        }
    }

    SECTION("Insert elements") {
        lPQueue.insert(100);
        lPQueue.insert(30);
        lPQueue.insert(40);
        lPQueue.insert(20);
        lPQueue.insert(50);

        REQUIRE(lPQueue.size() == 5);
        REQUIRE(lPQueue.min() == 20);

        // Test duplicate insertion
        lPQueue.insert(20);
        REQUIRE(lPQueue.size() == 6);
        REQUIRE(lPQueue.min() == 20);

        // Insert negative values and check min
        lPQueue.insert(-10);
        REQUIRE(lPQueue.size() == 7);
        REQUIRE(lPQueue.min() == -10);
    }

    SECTION("Remove elements") {
        lPQueue.insert(100);
        lPQueue.insert(50);
        lPQueue.insert(20);
        lPQueue.insert(40);
        lPQueue.insert(30);

        REQUIRE(lPQueue.size() == 5);
        REQUIRE(lPQueue.min() == 20);

        lPQueue.removeMin();
        REQUIRE(lPQueue.size() == 4);
        REQUIRE(lPQueue.min() == 30);

        lPQueue.removeMin();
        REQUIRE(lPQueue.size() == 3);
        REQUIRE(lPQueue.min() == 40);

        // Removing until empty
        lPQueue.removeMin();
        lPQueue.removeMin();
        lPQueue.removeMin();
        REQUIRE(lPQueue.size() == 0);
        REQUIRE(lPQueue.isEmpty());
        REQUIRE_THROWS_AS(lPQueue.min(), std::runtime_error);  // Assuming min() throws on empty
    }

    SECTION("Edge Cases") {
        lPQueue.insert(INT_MAX);
        lPQueue.insert(INT_MIN);
        REQUIRE(lPQueue.size() == 2);
        REQUIRE(lPQueue.min() == INT_MIN);
    }

    SECTION("Throw exception when removing from empty priority queue") { REQUIRE_THROWS(lPQueue.removeMin()); }

    SECTION("Throw exception when accessing min of empty priority queue") { REQUIRE_THROWS(lPQueue.min()); }

    SECTION("Throw exception when removing from empty priority queue") { REQUIRE_THROWS(lPQueue.removeMin()); }
}

TEST_CASE("Graph") {
    SECTION("Undirected Graph") {
        SECTION("Edge List Graph") {
            data_structures::graph::undirected::EdgeListUGraph<int, int> uGraph{};

            SECTION("Move") {
                SECTION("Construction") {
                    uGraph.addNode(1);
                    uGraph.addNode(2);
                    uGraph.addNode(3);
                    uGraph.addNode(4);
                    uGraph.addNode(5);

                    auto uMove{std::move(uGraph)};
                    REQUIRE(uMove.size() == 5);
                    REQUIRE(uGraph.size() == 0);
                    REQUIRE(uGraph.isEmpty());
                }

                SECTION("Assignment") {
                    uGraph.addNode(1);
                    uGraph.addNode(2);
                    uGraph.addNode(3);
                    uGraph.addNode(4);
                    uGraph.addNode(5);

                    auto uMove = std::move(uGraph);
                    REQUIRE(uMove.size() == 5);
                    REQUIRE(uGraph.size() == 0);
                    REQUIRE(uGraph.isEmpty());
                }
            }

            SECTION("Copy") {
                SECTION("Construction") {
                    uGraph.addNode(1);
                    uGraph.addNode(2);
                    uGraph.addNode(3);
                    uGraph.addNode(4);
                    uGraph.addNode(5);

                    auto uCopy{uGraph};
                    REQUIRE(uCopy.size() == 5);
                    REQUIRE(uGraph.size() == 5);
                }

                SECTION("Assignment") {
                    uGraph.addNode(1);
                    uGraph.addNode(2);
                    uGraph.addNode(3);
                    uGraph.addNode(4);
                    uGraph.addNode(5);

                    auto uCopy = uGraph;
                    REQUIRE(uCopy.size() == 5);
                    REQUIRE(uGraph.size() == 5);
                }
            }

            SECTION("General Graph Tests") {
                SECTION("Add Nodes") {
                    uGraph.addNode(1);
                    uGraph.addNode(2);
                    uGraph.addNode(3);
                    uGraph.addNode(4);
                    uGraph.addNode(5);

                    REQUIRE(uGraph.size() == 5);
                }

                SECTION("Add Edges") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    REQUIRE(uGraph.size() == 4);
                    REQUIRE(uGraph.numEdges() == 4);
                }

                SECTION("Remove Nodes") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    uGraph.removeNode(n1);
                    uGraph.removeNode(n2);

                    REQUIRE(uGraph.size() == 2);
                }

                SECTION("Remove Edges") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    uGraph.removeEdge(e1);
                    uGraph.removeEdge(e2);

                    REQUIRE(uGraph.numEdges() == 2);
                }
            }

            SECTION("Node Tests") {
                SECTION("Incident Edges") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto n1Node = uGraph.validateNode(n1);
                    auto n2Node = uGraph.validateNode(n2);
                    auto n3Node = uGraph.validateNode(n3);
                    auto n4Node = uGraph.validateNode(n4);

                    auto n1Inc = n1Node->incidentEdges();
                    auto n2Inc = n2Node->incidentEdges();
                    auto n3Inc = n3Node->incidentEdges();
                    auto n4Inc = n4Node->incidentEdges();

                    REQUIRE(n1Inc.size() == 2);
                    REQUIRE(n2Inc.size() == 2);
                    REQUIRE(n3Inc.size() == 3);
                    REQUIRE(n4Inc.size() == 1);
                }

                SECTION("Adjacent Nodes") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto n1Node = uGraph.validateNode(n1);
                    auto n2Node = uGraph.validateNode(n2);
                    auto n3Node = uGraph.validateNode(n3);
                    auto n4Node = uGraph.validateNode(n4);

                    auto n1Adj = n1Node->isAdjacentTo(n2Node);
                    auto n2Adj = n2Node->isAdjacentTo(n3Node);
                    auto n3Adj = n3Node->isAdjacentTo(n4Node);
                    auto n4Adj = n4Node->isAdjacentTo(n1Node);

                    REQUIRE(n1Adj);
                    REQUIRE(n2Adj);
                    REQUIRE(n3Adj);
                    REQUIRE_FALSE(n4Adj);
                }
            }

            SECTION("Edge Tests") {
                SECTION("End Nodes") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto e1Edge = uGraph.validateEdge(e1);
                    auto e2Edge = uGraph.validateEdge(e2);
                    auto e3Edge = uGraph.validateEdge(e3);
                    auto e4Edge = uGraph.validateEdge(e4);

                    auto e1Ends = e1Edge->endNodes();
                    auto e2Ends = e2Edge->endNodes();
                    auto e3Ends = e3Edge->endNodes();
                    auto e4Ends = e4Edge->endNodes();

                    REQUIRE(e1Ends.at(0) == n1);
                    REQUIRE(e1Ends.at(1) == n2);

                    REQUIRE(e2Ends.at(0) == n1);
                    REQUIRE(e2Ends.at(1) == n3);

                    REQUIRE(e3Ends.at(0) == n2);
                    REQUIRE(e3Ends.at(1) == n3);

                    REQUIRE(e4Ends.at(0) == n3);
                    REQUIRE(e4Ends.at(1) == n4);
                }

                SECTION("Incident Nodes") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto e1Edge = uGraph.validateEdge(e1);
                    auto e2Edge = uGraph.validateEdge(e2);
                    auto e3Edge = uGraph.validateEdge(e3);
                    auto e4Edge = uGraph.validateEdge(e4);

                    auto e1Inc = e1Edge->endNodes();
                    auto e2Inc = e2Edge->endNodes();
                    auto e3Inc = e3Edge->endNodes();
                    auto e4Inc = e4Edge->endNodes();

                    REQUIRE(e1Edge->isIncidentOn(n1));
                    REQUIRE(e1Edge->isIncidentOn(n2));

                    REQUIRE(e2Edge->isIncidentOn(n1));
                    REQUIRE(e2Edge->isIncidentOn(n3));

                    REQUIRE(e3Edge->isIncidentOn(n2));
                    REQUIRE(e3Edge->isIncidentOn(n3));

                    REQUIRE(e4Edge->isIncidentOn(n3));
                    REQUIRE(e4Edge->isIncidentOn(n4));

                    REQUIRE_FALSE(e1Edge->isIncidentOn(n3));
                    REQUIRE_FALSE(e1Edge->isIncidentOn(n4));

                    REQUIRE_FALSE(e2Edge->isIncidentOn(n2));
                    REQUIRE_FALSE(e2Edge->isIncidentOn(n4));
                }

                SECTION("Opposite Node") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto e1Edge = uGraph.validateEdge(e1);
                    auto e2Edge = uGraph.validateEdge(e2);
                    auto e3Edge = uGraph.validateEdge(e3);
                    auto e4Edge = uGraph.validateEdge(e4);

                    auto e1Opp = e1Edge->opposite(n1);
                    auto e2Opp = e2Edge->opposite(n1);
                    auto e3Opp = e3Edge->opposite(n2);
                    auto e4Opp = e4Edge->opposite(n3);

                    REQUIRE(e1Opp == n2);
                    REQUIRE(e2Opp == n3);
                    REQUIRE(e3Opp == n3);
                    REQUIRE(e4Opp == n4);
                }

                SECTION("Adjacent Edges") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto e1Edge = uGraph.validateEdge(e1);
                    auto e2Edge = uGraph.validateEdge(e2);
                    auto e3Edge = uGraph.validateEdge(e3);
                    auto e4Edge = uGraph.validateEdge(e4);

                    REQUIRE(e1Edge->isAdjacentTo(e2Edge));
                    REQUIRE(e1Edge->isAdjacentTo(e3Edge));
                    REQUIRE_FALSE(e1Edge->isAdjacentTo(e4Edge));
                }

                SECTION("Edge Weight") {
                    auto n1 = uGraph.addNode(1);
                    auto n2 = uGraph.addNode(2);
                    auto n3 = uGraph.addNode(3);
                    auto n4 = uGraph.addNode(4);

                    auto e1 = uGraph.addEdge(n1, n2, 10);
                    auto e2 = uGraph.addEdge(n1, n3, 20);
                    auto e3 = uGraph.addEdge(n2, n3, 30);
                    auto e4 = uGraph.addEdge(n3, n4, 40);

                    auto e1Edge = uGraph.validateEdge(e1);
                    auto e2Edge = uGraph.validateEdge(e2);
                    auto e3Edge = uGraph.validateEdge(e3);
                    auto e4Edge = uGraph.validateEdge(e4);

                    REQUIRE(**e1Edge == 10);
                    REQUIRE(**e2Edge == 20);
                    REQUIRE(**e3Edge == 30);
                    REQUIRE(**e4Edge == 40);
                }
            }
        }
    }

    SECTION("Directed Graph") {
        SECTION("Edge List Graph") {
            data_structures::graph::directed::EdgeListDGraph<int, int> dGraph{};

            SECTION("Move") {
                SECTION("Construction") {
                    dGraph.addNode(1);
                    dGraph.addNode(2);
                    dGraph.addNode(3);
                    dGraph.addNode(4);
                    dGraph.addNode(5);

                    auto dMove{std::move(dGraph)};
                    REQUIRE(dMove.size() == 5);
                    REQUIRE(dGraph.size() == 0);
                    REQUIRE(dGraph.isEmpty());
                }

                SECTION("Assignment") {
                    dGraph.addNode(1);
                    dGraph.addNode(2);
                    dGraph.addNode(3);
                    dGraph.addNode(4);
                    dGraph.addNode(5);

                    auto dMove = std::move(dGraph);
                    REQUIRE(dMove.size() == 5);
                    REQUIRE(dGraph.size() == 0);
                    REQUIRE(dGraph.isEmpty());
                }
            }

            SECTION("Copy") {
                SECTION("Construction") {
                    dGraph.addNode(1);
                    dGraph.addNode(2);
                    dGraph.addNode(3);
                    dGraph.addNode(4);
                    dGraph.addNode(5);

                    auto dCopy{dGraph};
                    REQUIRE(dCopy.size() == 5);
                    REQUIRE(dGraph.size() == 5);
                }

                SECTION("Assignment") {
                    dGraph.addNode(1);
                    dGraph.addNode(2);
                    dGraph.addNode(3);
                    dGraph.addNode(4);
                    dGraph.addNode(5);

                    auto dCopy = dGraph;
                    REQUIRE(dCopy.size() == 5);
                    REQUIRE(dGraph.size() == 5);
                }
            }

            SECTION("General Graph Tests") {
                SECTION("Add Nodes") {
                    dGraph.addNode(1);
                    dGraph.addNode(2);
                    dGraph.addNode(3);
                    dGraph.addNode(4);
                    dGraph.addNode(5);

                    REQUIRE(dGraph.size() == 5);
                }

                SECTION("Add Edges") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    REQUIRE(dGraph.size() == 4);
                    REQUIRE(dGraph.numEdges() == 4);
                }

                SECTION("Remove Nodes") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    dGraph.removeNode(n1);
                    dGraph.removeNode(n2);

                    REQUIRE(dGraph.size() == 2);
                }

                SECTION("Remove Edges") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    dGraph.removeEdge(e1);
                    dGraph.removeEdge(e2);

                    REQUIRE(dGraph.numEdges() == 2);
                }
            }

            SECTION("Node Tests") {
                SECTION("Incident Edges") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    auto n1Node = dGraph.validateNode(n1);
                    auto n2Node = dGraph.validateNode(n2);
                    auto n3Node = dGraph.validateNode(n3);
                    auto n4Node = dGraph.validateNode(n4);

                    auto n1Inc = n1Node->incidentEdges();
                    auto n2Inc = n2Node->incidentEdges();
                    auto n3Inc = n3Node->incidentEdges();
                    auto n4Inc = n4Node->incidentEdges();

                    REQUIRE(n1Inc.size() == 2);
                    REQUIRE(n2Inc.size() == 2);
                    REQUIRE(n3Inc.size() == 3);
                    REQUIRE(n4Inc.size() == 1);
                }

                SECTION("Adjacent Nodes") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    auto n1Node = dGraph.validateNode(n1);
                    auto n2Node = dGraph.validateNode(n2);
                    auto n3Node = dGraph.validateNode(n3);
                    auto n4Node = dGraph.validateNode(n4);

                    auto n1Adj = n1Node->isAdjacentTo(n2Node);
                    auto n2Adj = n2Node->isAdjacentTo(n3Node);
                    auto n3Adj = n3Node->isAdjacentTo(n4Node);
                }
            }

            SECTION("Edge Tests") {
                SECTION("End Nodes") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    auto e1Edge = dGraph.validateEdge(e1);
                    auto e2Edge = dGraph.validateEdge(e2);
                    auto e3Edge = dGraph.validateEdge(e3);
                    auto e4Edge = dGraph.validateEdge(e4);

                    auto e1End = e1Edge->endNode();
                    auto e1Start = e1Edge->startNode();
                    auto e2End = e2Edge->endNode();
                    auto e2Start = e2Edge->startNode();
                    auto e3End = e3Edge->endNode();
                    auto e3Start = e3Edge->startNode();
                    auto e4End = e4Edge->endNode();
                    auto e4Start = e4Edge->startNode();

                    REQUIRE(e1Start == n1);
                    REQUIRE(e1End == n2);
                    REQUIRE(e2Start == n1);
                    REQUIRE(e2End == n3);
                    REQUIRE(e3Start == n2);
                    REQUIRE(e3End == n3);
                    REQUIRE(e4Start == n3);
                    REQUIRE(e4End == n4);
                }

                SECTION("Incident Nodes") {
                    auto n1 = dGraph.addNode(1);
                    auto n2 = dGraph.addNode(2);
                    auto n3 = dGraph.addNode(3);
                    auto n4 = dGraph.addNode(4);

                    auto e1 = dGraph.addEdge(n1, n2, 10);
                    auto e2 = dGraph.addEdge(n1, n3, 20);
                    auto e3 = dGraph.addEdge(n2, n3, 30);
                    auto e4 = dGraph.addEdge(n3, n4, 40);

                    auto e1Edge = dGraph.validateEdge(e1);
                    auto e2Edge = dGraph.validateEdge(e2);
                    auto e3Edge = dGraph.validateEdge(e3);
                    auto e4Edge = dGraph.validateEdge(e4);

                    REQUIRE(e1Edge->isIncidentOn(n1));
                    REQUIRE(e1Edge->isIncidentOn(n2));
                    REQUIRE_FALSE(e1Edge->isIncidentOn(n3));
                    REQUIRE_FALSE(e1Edge->isIncidentOn(n4));

                    REQUIRE(e2Edge->isIncidentOn(n1));
                    REQUIRE(e2Edge->isIncidentOn(n3));
                    REQUIRE_FALSE(e2Edge->isIncidentOn(n2));
                    REQUIRE_FALSE(e2Edge->isIncidentOn(n4));

                    REQUIRE_FALSE(e3Edge->isIncidentOn(n1));
                    REQUIRE(e3Edge->isIncidentOn(n2));
                    REQUIRE(e3Edge->isIncidentOn(n3));
                    REQUIRE_FALSE(e3Edge->isIncidentOn(n4));

                    REQUIRE_FALSE(e4Edge->isIncidentOn(n1));
                    REQUIRE_FALSE(e4Edge->isIncidentOn(n2));
                    REQUIRE(e4Edge->isIncidentOn(n3));
                    REQUIRE(e4Edge->isIncidentOn(n4));
                }
            }
        }
    }
}

TEST_CASE("Heap") {
    SECTION("Min Heap") {
        data_structures::heap::ArrayMinHeap<int> mHeap{};

        SECTION("General Tests") {
            SECTION("Insert Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                REQUIRE(mHeap.size() == 5);
                REQUIRE(mHeap.extreme() == 10);
            }

            SECTION("Remove Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 3);
                REQUIRE(mHeap.extreme() == 30);
            }
        }

        SECTION("Robustness") {
            SECTION("Insert and Remove Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 0);
                REQUIRE_THROWS(mHeap.extreme());
            }

            SECTION("Insert and Remove Elements in Random Order") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.insert(5);
                mHeap.extractExtreme();
                mHeap.insert(100);
                mHeap.extractExtreme();
                mHeap.insert(15);
                mHeap.extractExtreme();
                mHeap.insert(25);
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 4);
                REQUIRE(mHeap.extreme() == 30);
            }
        }
    }

    SECTION("Max Heap") {
        SECTION("General Tests") {
            data_structures::heap::ArrayMaxHeap<int> mHeap{};

            SECTION("Insert Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                REQUIRE(mHeap.size() == 5);
                REQUIRE(mHeap.extreme() == 50);
            }

            SECTION("Remove Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 3);
                REQUIRE(mHeap.extreme() == 30);
            }
        }

        SECTION("Robustness") {
            data_structures::heap::ArrayMaxHeap<int> mHeap{};

            SECTION("Insert and Remove Elements") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 0);
                REQUIRE_THROWS(mHeap.extreme());
            }

            SECTION("Insert and Remove Elements in Random Order") {
                mHeap.insert(10);
                mHeap.insert(20);
                mHeap.insert(30);
                mHeap.insert(40);
                mHeap.insert(50);

                mHeap.extractExtreme();
                mHeap.insert(5);
                mHeap.extractExtreme();
                mHeap.insert(100);
                mHeap.extractExtreme();
                mHeap.insert(15);
                mHeap.extractExtreme();
                mHeap.insert(25);
                mHeap.extractExtreme();

                REQUIRE(mHeap.size() == 4);
                REQUIRE(mHeap.extreme() == 20);
            }
        }
    }
}
