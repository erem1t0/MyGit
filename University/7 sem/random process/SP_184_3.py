import numpy as np
import pandas as pd

def create_event_table(dT, n, mu, generator):
    curr_time = 0.0
    come = 1    
    state = 0   

    end_times = [float('inf')] * n # Время окончания обработки заявки для текущего прибора (inf если свободен)
    free = [0] * n                 # Хранит номер обрабатываемой заявки (0, если свободен)

    l_number = []       # Номер события 
    t_sob = []          # Момент наступления события
    l_state = []        # Состояние системы после события
    t_least_min = []    # Минимальное время осовобождения прибора после события(если все свободны -1)

    t_wait = []         # Сколько времени после события до следующей заявки

    l_type = []         # Тип события (1 - пришла ок, 2 - обработана, 3 - отказ)

    l_j = []            # Заявка, которая участвует в событии
    l_k = []            # Прибор, который обрабатывает заявку в событии (-1 если отказ)

    for l in range(1, 101):
        next_time = dT * come 
        next_end = min(end_times) 
        next_end_index = end_times.index(next_end) if next_end != float('inf') else -1 

        is_free = 0 in free
        curr_time = next_time if next_time < next_end else next_end

        # Если заявка придёт раньше, чем освободится какой-то прибор (или все приборы свободны)
        if next_time < next_end:
            l_j.append(come)

            # Свободный прибор есть
            if is_free:
                l_type.append(1)
                free_device = free.index(0) 
                free[free_device] = come    
                end_times[free_device] = next_time + generator.exponential(scale=1/mu)

                l_k.append(free_device + 1)
                state += 1 
            else:   # Свободных приборов нет - ОТКАЗ
                l_type.append(3) 
                l_k.append(-1)
            come += 1

            t_wait.append(dT*come - curr_time)
        else:  # Освобождаем прибор
            t_wait.append(next_time - curr_time)
            l_type.append(2)
            
            l_j.append(free[next_end_index])
            l_k.append(next_end_index + 1)            
            free[next_end_index] = 0
            end_times[next_end_index] = float('inf')

            state -= 1 

        l_number.append(l)
        t_sob.append(curr_time)
        l_state.append(state)
        next_end = min(end_times)
        t_least_min.append(next_end - curr_time if next_end != float('inf') else -1)

    l = 101
    while True:
        cnt = 0
        # Проверяем, все ли приборы свободны
        for i in range(len(free)):
            if free[i] == 0: cnt += 1
        # Если да, то останавливаемся
        if cnt == len(free): break

        # Добавить событие вида 2 для всех заявок которые ещё работают
        l_number.append(l)
        next_end = min(end_times)
        next_end_index = end_times.index(next_end)

        t_sob.append(next_end)
        t_wait.append(0.0)

        l_j.append(free[next_end_index])
        l_k.append(next_end_index)
        free[next_end_index] = 0
        end_times[next_end_index] = float('inf')
        

        state -= 1
        l_state.append(state)
        l_type.append(2)
        l += 1

        next_end = min(end_times)
        t_least_min.append(next_end - curr_time if next_end != float('inf') else -1)

    return pd.DataFrame({"l_number" : l_number, "t_sob" : t_sob, "l_type" : l_type, "l_state" : l_state, "t_least_min" : t_least_min,
                             "t_wait" : t_wait, "l_j" : l_j, "l_k" : l_k})

def create_task_table(data : pd.DataFrame):
    
    j_number = []   # номер заявки j
    t_come = []     # момент времени появления заявки в CMO
    t_time = []     # время обслуживания заявки (если отказ, то 0)
    t_end = []      # момент окончания обслуживания заявки (если отказ, то t_end = t_come)
    k_number = []   # номер прибора, который обслуживал заявку (если отказ, то -2)
    
    for _, row in data.iterrows():
        if row["l_type"] == 1:
            j_number.append(row["l_j"])
            t_come.append(row["t_sob"])
            k_number.append(row["l_k"])

            t_time.append(0.0)
            t_end.append(0.0)
        elif row["l_type"] == 2:
            idx = j_number.index(row["l_j"])

            t_end[idx] = row["t_sob"]
            t_time[idx] = t_end[idx] - t_come[idx]
        else:
            j_number.append(row["l_j"])
            t_come.append(row["t_sob"])
            t_time.append(0.0)
            t_end.append(row["t_sob"])
            k_number.append(-2)

    return pd.DataFrame({"j_number" : j_number, "t_come" : t_come, "t_time" : t_time, "t_end" : t_end, "k_number" : k_number})

def create_device_table(data : pd.DataFrame, total_time):
    k_number = []
    N_k = []
    t_busy = []
    t_free = []
    t_koeff = []
    l = 0
    for _, row in data.iterrows():
        if row["k_number"] == -2:
            continue
        
        if row["k_number"] not in k_number:
            k_number.append(row["k_number"])
            N_k.append(0)
            t_busy.append(0.0)
            t_free.append(0.0)
            t_koeff.append(0.0)
        idx = k_number.index(row["k_number"])
        N_k[idx] += 1
        t_busy[idx] += min(total_time, row["t_end"]) - row["t_come"]

    for i in range(len(t_free)):
        t_free[i] = total_time - t_busy[i]
        t_koeff[i] = t_free[i] / total_time

    k_number.append(0)
    N_k.append(sum(N_k))
    t_busy.append(np.mean(t_busy))
    t_free.append(np.mean(t_free))
    t_koeff.append(np.mean(t_koeff))

    return pd.DataFrame({"k_number" : k_number, "N_k" : N_k, "t_busy" : t_busy, "t_free" : t_free, "t_koeff" : t_koeff})

def create_states_table(data: pd.DataFrame):
    r_count = [0] * (n + 1)
    r_freq = [0.0] * (n + 1)
    r_time = [0.0] * (n + 1)
    r_delta = [0.0] * (n + 1)
    prev = 0.0
    curr = 0.0
    st = 0

    for i, row in data.iterrows():
            curr = row["t_sob"]
            r_count[st] += 1
            r_time[st] += curr - prev
            st = int(row["l_state"])
            prev = curr

    st = int(data["l_state"][99])
    r_count[st] += 1
    r_count[0] -= 1

    for i in range(n+1):
        r_freq[i] = r_count[i] / 100
        r_delta[i] = r_time[i] / sum(r_time)

    r_count.append(sum(r_count))
    r_freq.append(sum(r_freq))
    r_time.append(sum(r_time))
    r_delta.append(sum(r_delta))

    return pd.DataFrame({"r_count" : r_count, "r_freq" : r_freq, "r_time" : r_time, "r_delta" : r_delta})

if __name__ == "__main__":
        
    # Номер варианта
    N = 184

    dT = 0.043
    n = 13
    mu = 1.551

    generator = np.random.default_rng(N)

    event_table = create_event_table(dT, n, mu, generator)
    task_table = create_task_table(event_table)
    device_table = create_device_table(task_table, event_table['t_sob'][99])
    states_table = create_states_table(event_table[0:100])

    print("\n-----------------ТАБЛИЦА СОБЫТИЙ-----------------\n")
    print(event_table)

    print("\n-----------------ТАБЛИЦА ЗАЯВОК-----------------\n")
    print(task_table)

    print("\n-----------------ТАБЛИЦА ПРИБОРОВ-----------------\n")
    print(device_table)

    print("\n-----------------ТАБЛИЦА СОСТОЯНИЙ-----------------\n")
    print(states_table)
