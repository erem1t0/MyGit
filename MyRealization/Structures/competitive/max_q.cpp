/*
    Query on Maximum. Working like Query (FIFO) + get-query on current maximum with O(1)

    T pop() -> pop the element (FIFO)
    T get() -> get current maximum in query
    clear() -> clear the query

    Can be changed to Query on Minimum (or other op).

    #include (
        <stack>
        <unility>
        <cstddef>
    )
*/

template<typename T>
struct MaxQ {

    std::stack<std::pair<T, T>> s1, s2;

    void add(const T& val) {
        s1.empty() ? s1.push(std::make_pair(val, val)) : s1.push(std::make_pair(val, max(val, s1.top().second)));
    }

    T pop() {
        if (s2.empty()) {
            if (s1.empty()) return -1;

            T v = s1.top().first;
            s2.push(std::make_pair(v, v));
            s1.pop();

            while (!s1.empty()) {
                T val = s1.top().first;
                s1.pop();
                s2.push(std::make_pair(val, max(val, s2.top().second)));
            }
        }
        T res = s2.top().first;
        s2.pop();
        return res;
    }

    T get() {   const
        if (s1.empty() && s2.empty()) return -1;

        if (s1.empty() || s2.empty())
            return (s1.empty() ? s2.top().second : s1.top().second);
        else
            return max(s1.top().second, s2.top().second);
    }

    std::size_t size()  const
    {
        return s1.size() + s2.size();
    }

    void clear() {
        while (!s1.empty())
            s1.pop();
        while (!s2.empty())
            s2.pop();
    }

};