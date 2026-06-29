import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Номер Варианта
N = 184

# Вероятности
p1 = 0.181
q1 = 0.0
p2 = 1 - p1
q2 = 1 - q1

# Вероятности переходов (для удобства)
x1 = p1 * q1
x2 = p1 * q2
x3 = p2 * q1
x4 = p2 * q2
X = [x1, x2, x3, x4]

# Граф состояний цепи Маркова
graph = [[0.0] * 12 for _ in range(12)]

# Записываем состояния
states = dict()

state = 1
for w in range(1, 5):
    for b in range(1, 5):
        if w == b:
            continue

        states[(w, b)] = state
        state += 1

transitions = [[] for _ in range(12)]
# Перебираем все состояния и строим переходы
state = 1
for w in range(1, 5):
    for b in range(1, 5):
        if w == b:
            continue

        # Свободные ящики
        empty = [1,2,3,4]
        empty.remove(w)
        empty.remove(b)

        # x1
        nextw_11 = min(empty)
        empty.remove(nextw_11)
        empty.append(w)
        nextb_11 = min(empty)

        graph[state - 1][states[(nextw_11, nextb_11)] - 1] = x1
        if x1 != 0:
            transitions[state - 1].append(f"({nextw_11},{nextb_11}) [{states[(nextw_11, nextb_11)]}]")

        # x2
        nextw_12 = nextw_11
        nextb_12 = max(empty)

        graph[state - 1][states[(nextw_12, nextb_12)] - 1] = x2
        if x2 != 0:
            transitions[state - 1].append(f"({nextw_12},{nextb_12}) [{states[(nextw_12, nextb_12)]}]")

        # x3
        empty.remove(w)
        empty.append(nextw_11)
        nextw_21 = max(empty)
        empty.append(w)
        empty.remove(nextw_21)
        nextb_21 = min(empty)

        graph[state - 1][states[(nextw_21, nextb_21)] - 1] = x3
        if x3 != 0:
            transitions[state - 1].append(f"({nextw_21},{nextb_21}) [{states[(nextw_21, nextb_21)]}]")

        # x4
        nextw_22 = nextw_21
        nextb_22 = max(empty)

        graph[state - 1][states[(nextw_22, nextb_22)] - 1] = x4
        if x4 != 0:
            transitions[state - 1].append(f"({nextw_22},{nextb_22}) [{states[(nextw_22, nextb_22)]}]")

        state += 1

stat_series = pd.DataFrame({'№ состояния': states.values(), 'Состояние': states.keys(), 'Список возможных'
'состояний на следующем шаге (с ненулевой вероятностью перехода)': transitions})
print("|---------------------------------------------------------------|")
print(' Таблица всех возможных переходов между состояниями:')
print(stat_series)


# Рисуем граф состояний цепи Маркова
gr_draw = nx.DiGraph()
for i in range(12):
    for j in range(12):
        if graph[i][j] != 0:
            gr_draw.add_edge(i + 1, j + 1)

pos = nx.circular_layout(gr_draw)
nx.draw(gr_draw, pos, with_labels=True, node_color='red',  node_size=300, font_size=10, arrows=False, width=0)
nx.draw_networkx_edges(gr_draw, pos, edge_color='black', arrows=True, arrowsize=25, 
                       arrowstyle='->', width=2, connectionstyle='arc3,rad=0.4', alpha=0.6)

plt.show()

print("|---------------------------------------------------------------|")
print(' Матрица переходных вероятностей P:')
for e in graph:
    print(e)
print("|---------------------------------------------------------------|")

np.set_printoptions(precision=5, suppress=True, linewidth=350)

P = np.array(graph)
P_5 = np.linalg.matrix_power(P, 5)
P_10 = np.linalg.matrix_power(P, 10)
P_15 = np.linalg.matrix_power(P, 15)

print(' P(5):')
for e in P_5:
    print(e)
print("|---------------------------------------------------------------|")

print(' P(10):')
for e in P_10:
    print(e)
print("|---------------------------------------------------------------|")

print(' P(15):')
for e in P_15:
    print(e)
print("|---------------------------------------------------------------|")
