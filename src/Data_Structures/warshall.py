def war(A):
    n = len(A)
    R = [row[:] for row in A]

    for k in range(n):
        for i in range(n):
            for j in range(n):
                R[i][j] = R[i][j] or (R[i][k] and R[k][j])
    return R


A = [
    [0, 1, 0, 0],  #
    [1, 0, 1, 1],
    [0, 0, 0, 1],
    [0, 0, 0, 0],
]

res = war(A)
for row in res:
    print(row)
