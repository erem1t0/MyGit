import numpy as np
import pandas as pd

# Номер варианта
N = 184

P = [
    [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.819],
    [0.0, 0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.819, 0.0],
    [0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.819],
    [0.0, 0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.819, 0.0],
    [0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.0, 0.819],
    [0.0, 0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.819],
    [0.0, 0.181, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0, 0.0, 0.0, 0.181, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.181, 0.0, 0.0, 0.0, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.181, 0.0, 0.0, 0.819, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
]


def stationary_solve(P):
    A = np.array(P)
    A = A.T - np.eye(12)
    A[0, :] = 1
    B = np.zeros(12)
    B[0] = 1

    return np.linalg.solve(A, B)

np.set_printoptions(precision=5, suppress=True, linewidth=200)

r = stationary_solve(P)
print("|---------------------------------------------------------------|")
print(' Стационарное распределение вероятностей:')
print(r)
if np.linalg.norm(r @ P - r) < 1e-10:
    print(r @ P)
    print("r * P = r")
else:
    print("something not good with r :(")


def dfs(P, v, u, reachable : list, visit : list):
    if visit[v]:
        return
    visit[v] = 1
    reachable[u][v] = 1

    for nxt in range(12):
        if P[v][nxt] != 0:
            dfs(P, nxt, u, reachable, visit)

def get_essentials(P):
    P = np.array(P)

    non_ess = []

    reachable = [[0] * 12 for i in range(12)]
    # Обходим и смотрим, кто откуда достижим
    for i in range(12):
        visit = [0] * 12
        for j in range(12):
            if P[i][j] != 0:
                dfs(P, j, i, reachable, visit)

        
    # Проверяем, есть ли i, j при которых i -> j, но не j -> i
    for i in range(12):
        for j in range(12):
            if reachable[i][j] and not reachable[j][i]:
                non_ess.append(i + 1)
                break

    ess = []
    for i in range(1, 13):
        if i not in non_ess:
            ess.append(i)

    non_ess.sort()
    return ess, non_ess, reachable


ess, non_ess, reachable = get_essentials(P)
print("|---------------------------------------------------------------|")
print(" Существенные состояния:")
print(ess)
print("|---------------------------------------------------------------|")
print(" Несущественные состояния:")
print(non_ess)
print("|---------------------------------------------------------------|")

for e in reachable:
    print(e)