import numpy as np
import pandas as pd
from collections import deque

def create_event_table(lambda_, generator):
    curr_time = 0.0
    come = 1    
    state = 0   

    end_times = [float('inf')]      # Время окончания обработки заявки для текущего прибора (inf если свободен)
    free = [0]                      # Хранит номер обрабатываемой заявки (0, если свободен)

    l_number = []       # Номер события 
    t_sob = []          # Момент наступления события
    l_state = []        # Состояние системы после события
    t_least_min = []    # Минимальное время осовобождения прибора после события(если все свободны -1)

    t_wait = []         # Сколько времени после события до следующей заявки

    l_type = []         # Тип события (1 - пришла ок, 2 - обработана, 3 - отказ)

    l_j = []            # Заявка, которая участвует в событии

    pos = 0
    queue = deque()
    next_task = generator.exponential(scale=1/lambda_)

    t_start = [] # Время начала обслуживания заявки (-1 - отказ)

    l = 1
    while l < 101:
        if len(l_type) > 0 and l_type[-1] == 2 and len(queue) > 0 and free[0] == 0:
            # Добавляем на обслуживание из очереди
            free[0] = queue.popleft()    
            end_times[0] = curr_time + generator.exponential(scale=1/mu)
            t_start[free[0] - 1] = curr_time
            t_least_min[-1] = end_times[0]
            continue

        next_time = next_task
        next_end = end_times[0]

        is_free = (free[0] == 0)
        curr_time = next_time if next_time < next_end else next_end

        # Если заявка придёт раньше, чем освободится какой-то прибор (или все приборы свободны)
        if next_time < next_end:
            l_j.append(come)
            t_start.append(0)
            # Прибор свободен:
            if is_free:
                l_type.append(1)
                free[0] = come   
                end_times[0] = next_time + generator.exponential(scale=1/mu)
                t_start[come - 1] = curr_time
            else:   # Свободных приборов нет - встать в очередь
                # Очередь заполнена
                if len(queue) == m:
                    l_type.append(4)
                    t_start[come - 1] = -1
                    state -= 1 # Компенсируем просто
                else: # Место есть
                    queue.append(come)
                    l_type.append(3) 
            come += 1
            state += 1
            next_task = curr_time + generator.exponential(scale=1/lambda_)

        else:  # Освобождаем прибор
            l_type.append(2)
            l_j.append(free[0])           
            free[0] = 0
            end_times[0] = float('inf')

            state -= 1 

        if next_task == curr_time:
            next_task = curr_time + generator.exponential(scale=1/lambda_)

        pos += 1
        t_wait.append(next_task - curr_time)
        l_number.append(l)
        t_sob.append(curr_time)
        l_state.append(state)
        next_end = min(end_times)
        t_least_min.append(next_end - curr_time if next_end != float('inf') else -1)
        l += 1

    # Всех кто в обслуживании считаем их время окончания. Для всех кто в очереди -> -1
    l = 101
    while True:
        cnt = 0
        # Проверяем, все ли приборы свободны
        for i in range(len(free)):
            if free[i] == 0: cnt += 1
        # Если да + очередь пуста -> останавливаемся
        if cnt == len(free): break

        # Обслужить
        l_number.append(l)
        next_end = end_times[0]

        t_sob.append(next_end)
        t_wait.append(0.0)
        l_j.append(free[0])

        free[0] = 0
        end_times[0] = float('inf')

        l_type.append(2)
        state -= 1
        l_state.append(state)
        l += 1
        next_end = min(end_times)
        t_least_min.append(next_end - curr_time if next_end != float('inf') else -1)

    return pd.DataFrame({"l_number" : l_number, "t_sob" : t_sob, "l_type" : l_type, "l_state" : l_state, "t_least_min" : t_least_min,
                "t_wait" : t_wait, "l_j" : l_j}), t_start

def create_task_table(data : pd.DataFrame, t_start, total_time):
    
    # t_start Время начала обслуживания заявки
    j_number = []   # номер заявки j
    t_come = []     # момент времени появления заявки в CMO
    t_time = []     # время обслуживания заявки (если отказ, то 0)
    t_end = []      # момент окончания обслуживания заявки (если отказ, то t_end = t_come)

    t_q_time = [] # Время пребывания в очереди
    t_queue = []    # Номер места в очереди заявки (0 - сразу, -1 - отказ)

    for _, row in data.iterrows():
        type_ = row["l_type"]
        if type_ == 1 or type_ == 3:
            j_number.append(row["l_j"])
            t_come.append(row["t_sob"])
            t_time.append(0.0)
            #t_queue.append(max(0, row["l_state"] - 1))
            t_queue.append(0 if type_ == 1 else max(0, row["l_state"] - 1)) # 1 - без очереди
            t_end.append(0.0)
            st = int(row["l_j"] - 1)
            #t_q_time.append(max(total_time, t_start[st]) - t_come[st])
            t_q_time.append(0.0 if type_ == 1 else t_start[st] - t_come[st]) # 1 - без очереди
            if t_q_time[-1] < 0:
                t_q_time[-1] = total_time - t_come[st]
        elif row["l_type"] == 2:
            idx = j_number.index(row["l_j"])
            t_end[idx] = row["t_sob"]
            t_time[idx] = min(total_time, t_end[idx]) - t_start[idx]
        elif row["l_type"] == 4:
            j_number.append(row["l_j"])
            t_come.append(row["t_sob"])
            t_end.append(t_come[-1])
            t_time.append(0.0)
            t_q_time.append(0.0)
            t_queue.append(-1)
            

    return pd.DataFrame({"j_number" : j_number, "t_come" : t_come, "t_queue" : t_queue, "t_q_time" : t_q_time, "t_start" : t_start, 
                         "t_time" : t_time,  "t_end" : t_end})

def create_states_table(data: pd.DataFrame, m, r):
    r_count = [0] * (m + 2)
    r_freq = [0.0] * (m + 2)
    r_time = [0.0] * (m + 2)
    r_delta = [0.0] * (m + 2)
    prev = 0.0
    curr = 0.0
    st = 0

    for _, row in data.iterrows():
        curr = row["t_sob"]
        r_count[st] += 1
        r_time[st] += curr - prev

        st = int(row['l_state'])
        prev = curr

    st = int(data["l_state"][99])
    r_count[st] += 1
    r_count[0] -= 1

    for i in range(len(r_count)):
        r_freq[i] = r_count[i] / 100
        r_delta[i] = r_time[i] / sum(r_time)

    r_count.append(sum(r_count))
    r_freq.append(sum(r_freq))
    r_time.append(sum(r_time))
    r_delta.append(sum(r_delta))
    r.append(sum(r))

    return pd.DataFrame({"r" : r, "r_count" : r_count, "r_freq" : r_freq, "r_time" : r_time, "r_delta" : r_delta})

def calc_r_k(lambda_, mu, m):
    rho = lambda_ / mu
    r = [0] * (m + 2)
    
    r[0] = (1 - rho) / (1 - rho**(m + 2)) if rho != 1 else 1 / (m + 2)
    for i in range(1, m + 2):
        r[i] = r[0] * rho**i
    return r

if __name__ == "__main__":
        
    # Номер варианта
    N = 184

    m = 12
    lambda_ = 2.464
    mu = 1.551

    generator = np.random.default_rng(N)

    r = calc_r_k(lambda_, mu, m)

    event_table, t_start = create_event_table(lambda_, generator)
    task_table = create_task_table(event_table, t_start, event_table["t_sob"][99])
    states_table = create_states_table(event_table[0:100], m, r)

    print("\n-----------------ТАБЛИЦА СОБЫТИЙ-----------------\n")
    print(event_table)

    print("\n-----------------ТАБЛИЦА ЗАЯВОК-----------------\n")
    print(task_table)

    print("\n-----------------ТАБЛИЦА СОСТОЯНИЙ-----------------\n")
    print(states_table)
