
#include <catch2/catch_all.hpp>
#include <vector>

#include "algorithms.hpp"

template <typename Itr>
void resetArray(Itr beginOrigin, Itr endOrigin, Itr beginWorking) {
    std::copy(beginOrigin, endOrigin, beginWorking);
}

TEST_CASE("algorithms") {
    using std::less, std::less_equal, std::greater, std::greater_equal;
    std::vector<int> unSortedIntVector{2, 5, 3, 1, 7, 5, 3, 8, 2, 3};
    const std::vector<int> sortedIntVectorLess{1, 2, 2, 3, 3, 3, 5, 5, 7, 8};
    const std::vector<int> sortedInVectorGreater{8, 7, 5, 5, 3, 3, 2, 2, 1};

    std::vector<double> unSortedDoubleVector{
        9.025270758122742,  7.2202166064981945, 12.63537906137184,  5.415162454873646,  3.6101083032490973,
        10.830324909747292, 1.8050541516245486, 18.050541516245485, 14.440433212996389, 16.24548736462094};
    const std::vector<double> sortedDoubleVectorLess{
        1.8050541516245486, 3.6101083032490973, 5.415162454873646,  7.2202166064981945, 9.025270758122742,
        10.830324909747292, 12.63537906137184,  14.440433212996389, 16.24548736462094,  18.050541516245485};
    const std::vector<double> sortedDoubleVectorGreater{
        18.050541516245485, 16.24548736462094,  14.440433212996389, 12.63537906137184,  10.830324909747292,
        9.025270758122742,  7.2202166064981945, 5.415162454873646,  3.6101083032490973, 1.8050541516245486};

    int originalUnsortedIntCArray[]{2, 5, 3, 1, 7, 5, 3, 8, 2, 3};
    int unsortedIntCArray[]{2, 5, 3, 1, 7, 5, 3, 8, 2, 3};
    const int sortedIntCArrayLess[]{1, 2, 2, 3, 3, 3, 5, 5, 7, 8};
    const int sortedIntCArrayGreater[]{8, 7, 5, 5, 3, 3, 2, 2, 1};

    const auto cIntArraySize = sizeof(unsortedIntCArray) / sizeof(unsortedIntCArray[0]);

    double unsortedDoubleCArray[]{9.025270758122742,  7.2202166064981945, 12.63537906137184,  5.415162454873646,
                                  3.6101083032490973, 10.830324909747292, 1.8050541516245486, 18.050541516245485,
                                  14.440433212996389, 16.24548736462094};
    double sortedDoubleCArrayLess[]{1.8050541516245486, 3.6101083032490973, 5.415162454873646, 7.2202166064981945,
                                    9.025270758122742,  10.830324909747292, 12.63537906137184, 14.440433212996389,
                                    16.24548736462094,  18.050541516245485};
    double sortedDoubleCArrayGreater[]{18.050541516245485, 16.24548736462094, 14.440433212996389, 12.63537906137184,
                                       10.830324909747292, 9.025270758122742, 7.2202166064981945, 5.415162454873646,
                                       3.6101083032490973, 1.8050541516245486};

    SECTION("Sorting") {
        using namespace algorithms::sorting;

        SECTION("BubbleSort") {
            SECTION("Less") {
                SECTION("Int") {
                    SECTION("Works with ranges") {
                        bubble(unSortedIntVector.begin(), unSortedIntVector.end(), less());
                        REQUIRE(unSortedIntVector == sortedIntVectorLess);
                    }

                    SECTION("Works with C style arrays") {
                        bubble(unsortedIntCArray, less());
                        for (size_t i{0}; i < cIntArraySize; i++)
                            REQUIRE(unsortedIntCArray[i] == sortedIntCArrayLess[i]);
                        resetArray(std::begin(originalUnsortedIntCArray), std::end(originalUnsortedIntCArray),
                                   std::begin(unsortedIntCArray));
                    }
                }
            }
        }

        SECTION("InsertionSort") {
            SECTION("Less") {
                SECTION("Int") {
                    SECTION("Works with ranges") {
                        insertion(unSortedIntVector.begin(), unSortedIntVector.end(), less());
                        REQUIRE(unSortedIntVector == sortedIntVectorLess);
                    }

                    SECTION("Works with C style arrays") {
                        insertion(unsortedIntCArray, less());
                        for (size_t i{0}; i < cIntArraySize; i++)
                            REQUIRE(unsortedIntCArray[i] == sortedIntCArrayLess[i]);
                        resetArray(std::begin(originalUnsortedIntCArray), std::end(originalUnsortedIntCArray),
                                   std::begin(unsortedIntCArray));
                    }
                }
            }
        }

        SECTION("MergeSort") {
            SECTION("Less") {
                SECTION("Int") {
                    SECTION("Works with ranges") {
                        merge(unSortedIntVector.begin(), unSortedIntVector.end(), less());
                        REQUIRE(unSortedIntVector == sortedIntVectorLess);
                    }

                    SECTION("Works with C style arrays") {
                        merge(unsortedIntCArray, 0, cIntArraySize - 1, less());
                        for (size_t i{0}; i < cIntArraySize; i++)
                            REQUIRE(unsortedIntCArray[i] == sortedIntCArrayLess[i]);
                        resetArray(std::begin(originalUnsortedIntCArray), std::end(originalUnsortedIntCArray),
                                   std::begin(unsortedIntCArray));
                    }
                }
            }
        }

        SECTION("ShellSort") {
            SECTION("Less") {
                SECTION("Int") {
                    SECTION("Work with ranges") {
                        shell(unSortedIntVector.begin(), unSortedIntVector.end(), less());
                        REQUIRE(unSortedIntVector == sortedIntVectorLess);
                    }

                    SECTION("Works with C style arrays") {
                        shell(unsortedIntCArray, less());
                        for (size_t i{0}; i < 10; i++) REQUIRE(unsortedIntCArray[i] == sortedIntCArrayLess[i]);
                        resetArray(std::begin(originalUnsortedIntCArray), std::end(originalUnsortedIntCArray),
                                   std::begin(unsortedIntCArray));
                    }
                }
            }
        }

        SECTION("QuickSort") {
            SECTION("Less") {
                SECTION("Int") {
                    SECTION("Works with ranges") {
                        quicksort(unSortedIntVector.begin(), unSortedIntVector.end(), less());
                        REQUIRE(unSortedIntVector == sortedIntVectorLess);
                    }

                    SECTION("Works with C style arrays") {
                        quicksort(unsortedIntCArray, 0, cIntArraySize - 1, less());
                        for (size_t i{0}; i < 10; i++) REQUIRE(unsortedIntCArray[i] == sortedIntCArrayLess[i]);
                        resetArray(std::begin(originalUnsortedIntCArray), std::end(originalUnsortedIntCArray),
                                   std::begin(unsortedIntCArray));
                    }
                }
            }
        }
    }

    SECTION("Searching") {}
}
