#include <cmath>
#include <iostream>
#include <vector>

#include "timer.cpp"

using namespace std;

template <size_t n>
int diagSum(const int (&matrix)[n][n]) {
    int sum = 0;
    size_t pd_ptr = 0;
    size_t sd_ptr = n - 1;
    for (size_t i = 0; i < n; i++) {
        sum += matrix[i][pd_ptr];
        if (pd_ptr != sd_ptr) sum += matrix[i][sd_ptr];
        pd_ptr++;
        sd_ptr--;
    }

    return sum;
}

template <size_t n>
int diag_Sum(const int (&matrix)[n][n]) {
    int sum = {};
    int flat[n * n] = {};
    vector<int> primary_diag{};
    vector<int> secondary_diag{};

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            flat[i * n + j] = matrix[i][j];
        }
    }

    for (size_t i = 0; i < n * n; i = i + (n + 1)) {
        primary_diag.push_back(flat[i]);
    }

    for (size_t i = n * n - n; i > 0; i = i - (n - 1)) {
        secondary_diag.push_back(flat[i]);
    }

    for (auto &i : primary_diag) {
        sum += i;
    }

    for (auto &i : secondary_diag) {
        sum += i;
    }

    if (primary_diag[floor(n / 2)] == secondary_diag[floor(n / 2)])
        sum -= primary_diag[floor(n / 2)];

    return sum--;

    return sum;
}

int main() {
    int mat[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int mat2[6][6] = {{1, 2, 3, 4, 5, 6},       {7, 8, 9, 10, 11, 12},
                      {13, 14, 15, 16, 17, 18}, {19, 20, 21, 22, 23, 24},
                      {25, 26, 27, 28, 29, 30}, {31, 32, 33, 34, 35, 36}};
    cout << "Sum of diagonals: " << diagSum(mat) << endl;
    cout << "Sum of diagonals: " << diagSum(mat2) << "\n\n";

    cout << "Sum of diagonals: " << diag_Sum(mat) << endl;
    cout << "Sum of diagonals: " << diag_Sum(mat2) << endl;

    TimerClass *diagSumTimer1 = new TimerClass{"diagSum(mat)"};
    diagSum(mat);
    delete diagSumTimer1;

    TimerClass *diagSumTimer2 = new TimerClass{"diagSum(mat2)"};
    diagSum(mat2);
    delete diagSumTimer2;

    TimerClass *diag_SumTimer1 = new TimerClass{"diag_Sum(mat)"};
    diag_Sum(mat);
    delete diag_SumTimer1;

    TimerClass *diag_SumTimer2 = new TimerClass{"diag_Sum(mat2)"};
    diag_Sum(mat2);
    delete diag_SumTimer2;

    return 0;
}
