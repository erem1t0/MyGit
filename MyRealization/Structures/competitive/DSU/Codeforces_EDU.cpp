/* # Step 1 */

// Disjoint Sets Union

class DSU
{
private:

    std::vector<int> sz, parent;
    int u, v;

    void mergeDSU()
    {
        u = getParent(u);
        v = getParent(v);

        if (u == v)
            return;

        if (sz[u] < sz[v])
            std::swap(u, v);

        sz[u] += sz[v];
        parent[v] = u;
    }

    void getDSU()
    {
        parent[0] = getParent(u) == getParent(v);
    }
    

    int getParent(int u)
    {
        // Use this if u have cuts or smth related with edges
        // return (u == parent[u] ? u : getParent(parent[u]));

        return (u == parent[u] ? u : parent[u] = getParent(parent[u]));
    }


public:

    DSU(int n)
        : sz(n + 1, 1)
        , parent(n + 1)
    {
        std::iota(all(parent), 0);
    }

    void merge(int u, int v)
    {
        DSU::u = u;
        DSU::v = v;
        mergeDSU();
    }

    bool get(int u, int v)
    {
        DSU::u = u;
        DSU::v = v;
        getDSU();
        return parent[0];
    }


};


// Disjoint Sets Union 2

class DSU
{
private:

    std::vector<int> sz, el_min, el_max, parent;
    int u, v;

    void mergeDSU()
    {
        u = getParent(u);
        v = getParent(v);

        if (u == v)
            return;

        if (sz[u] < sz[v])
            std::swap(u, v);

        sz[u] += sz[v];
        parent[v] = u;
        setmin(el_min[u], el_min[v]);
        setmax(el_max[u], el_max[v]);
    }
    

    int getParent(int u)
    {
        // Use this if u have cuts or smth related with edges
        // return (u == parent[u] ? u : getParent(parent[u]));

        return (u == parent[u] ? u : parent[u] = getParent(parent[u]));
    }


public:

    DSU(int n)
        : sz(n + 1, 1)
        , parent(n + 1)
    {
        std::iota(all(parent), 0);
        el_min = el_max = parent;
    }

    void merge(int u, int v)
    {
        DSU::u = u;
        DSU::v = v;
        mergeDSU();
    }

    std::tuple<int,int,int> get(int u)
    {
        u = getParent(u);
        return { el_min[u],el_max[u],sz[u] };
    }


};

// Experience

class DSU
{
private:

    std::vector<int> exp, sz, parent, add;
    int u, v;


    void mergeDSU()
    {
        u = getParent(u);
        v = getParent(v);

        if (u == v)
            return;

        if (sz[u] < sz[v])
            std::swap(u, v);
        
        add[v] -= add[u];
        parent[v] = u;
        sz[u] += sz[v];
    }
    

    int getParent(int u)
    {
        // Use this if u have cuts or smth related with edges
        return (u == parent[u] ? u : getParent(parent[u]));

        //return (u == parent[u] ? u : parent[u] = getParent(parent[u]));
    }


public:


    DSU(int n)
        : sz(n + 1, 1)
        , add(n + 1)
        , exp(n + 1)
        , parent(n + 1)
    {
        std::iota(all(parent), 0);
    }


    void merge(int u, int v)
    {
        DSU::u = u;
        DSU::v = v;
        mergeDSU();
    }


    int get(int u)
    {
        int answ = 0;
        while (u != parent[u])
        {
            answ += add[u];
            u = parent[u];
        }
        answ += add[u];

        return answ;
    }


    void add_func(int index, int value)
    {
        add[getParent(index)] += value;
    }


};

// Cutting a graph
class DSU
{
private:

    std::vector<int> sz, parent;
    int u, v;


    void mergeDSU()
    {
        u = getParent(u);
        v = getParent(v);

        if (u == v)
            return;

        if (sz[u] < sz[v])
            std::swap(u, v);
        
        parent[v] = u;
        sz[u] += sz[v];
    }
    

    int getParent(int u)
    {
        // Use this if u have cuts or smth related with edges
        return (u == parent[u] ? u : getParent(parent[u]));

        //return (u == parent[u] ? u : parent[u] = getParent(parent[u]));
    }


public:


    DSU(int n)
        : sz(n + 1, 1)
        , parent(n + 1)
    {
        std::iota(all(parent), 0);
    }


    void merge(int u, int v)
    {
        DSU::u = u;
        DSU::v = v;
        mergeDSU();
    }


    bool get(int u, int v)
    {
        return getParent(u) == getParent(v);
    }


};


void solve()
{
    int n, m, k; cin >> n >> m >> k;
    while (m--) {
        int x, y; cin >> x >> y;
    }

    DSU dsu(n);
    const string CUT = "cut", ASK = "ask";
    vector<tuple<string, int, int>> requests;
    while (k--)
    {
        string type; cin >> type;
        int u, v; cin >> u >> v;
        requests.push_back({ type,u,v });
    }

    reverse(all(requests));

    vector<int> answ;
    for (auto& [type, u, v] : requests)
    {
        if (CUT == type)
        {
            dsu.merge(u, v);
        }
        else if (ASK == type)
        {
            answ.push_back(dsu.get(u, v));
        }
        else  throw 1;
    }
    reverse(all(answ));
    for (auto& it : answ)
        cout << (it ? "YES\n" : "NO\n");
}


// Monkeys


class DSU
{
private:
 
    vi sz, parent, time, left, right;
 
 
    int getParent(int u)
    {
        return (u == parent[u] ? u : getParent(parent[u]));
    }
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
        , time(n+1, INF)
    {
        std::iota(all(parent), 0);
    }
 
    void merge(int x, int y, int currTime = INF)
    {
        if (x == y || x == -1 || y == -1)
            return;
 
        x = getParent(x);
        y = getParent(y);
 
        if (x == y)
            return;
 
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        if (x == 1)
        {
            setmin(time[y], currTime);
        }
        else if (y == 1)
        {
            setmin(time[x], currTime);
            std::swap(x, y);
        }
        else {
            setmin(time[y], currTime);
        }
 
        parent[y] = x;
        sz[x] += sz[y];
 
    }
 
    void update(const int n)
    {
        for (int i = 1; i <= n; ++i)
        {
            int curr = i;
            while (curr != parent[curr])
            {
                setmin(time[i], time[curr]);
                curr = parent[curr];
            }
        }
    }
 
    const vi& get()   const
    {
        return time;
    }
 
};
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
 
    vi left(n + 1), right(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> left[i] >> right[i];
 
    vpii queries(m);
    for (auto& [first, second] : queries)
        cin >> first >> second;
    
    vi buffl = left, buffr = right;
 
    for (auto& [monkey, hand] : queries)
        hand == 1 ? left[monkey] = -1 : right[monkey] = -1;
 
    for (int i = 1; i <= n; ++i)
    {
        if (left[i] != -1)
            dsu.merge(i, left[i]);
        if (right[i] != -1)
            dsu.merge(i, right[i]);
    }
 
    reverse(all(queries));
 
    int currTime = m - 1;
    for (auto& [monkey, hand] : queries)
    {
        hand == 1 ? left[monkey] = buffl[monkey] : right[monkey] = buffr[monkey];
        dsu.merge(monkey, hand == 1 ? left[monkey] : right[monkey], currTime);
        --currTime;
    }
 
    dsu.update(n);
 
    vi answ = dsu.get();
    for (int i = 1; i <= n; ++i)
        std::cout << (answ[i] == INF ? -1 : answ[i]) << '\n';
 
}

/* Step 2 */

// People are leaving

class DSU
{
private:
 
    vi sz, parent;
 
 
    int getParent(int x)
    {
        return (x == parent[x] ? x : parent[x] = getParent(parent[x]));
    }
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+2)
    {
        std::iota(all(parent), 0);
    }
 
    void merge(int x)
    {
        int y = x + 1;
        x = getParent(x);
        y = getParent(y);
 
        parent[x] = y;
    }
 
    int get(int x)
    {
        return (getParent(x) == isz(parent) - 1 ? -1 : getParent(x));
    }
 
 
};
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
 
    const char MINUS = '-', GET = '?';
    while (m--)
    {
        char type; cin >> type;
        if (MINUS == type)
        {
            int x; cin >> x;
            dsu.merge(x);
        }
        else if (GET == type)
        {
            int x; cin >> x;
            std::cout << dsu.get(x) << '\n';
        }
        else throw 1;
    }
}


// Parking

class DSU
{
private:
 
    vi sz, parent;
 
 
    int getParent(int x)
    {
        return (x == parent[x] ? x : parent[x] = getParent(parent[x]));
    }
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
    {
        std::iota(all(parent), 0);
    }
 
    void merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        parent[y] = x;
        sz[x] += sz[y];
    }
 
    int add(int x)
    {
        x = getParent(x);
        int y = (x + 1 == isz(parent) ? getParent(1) : getParent(x + 1));
        parent[x] = y;
        sz[y] += sz[x];
 
        return x;
    }
 
};
 
 
void solve()
{
    int n; cin >> n;
    DSU dsu(n);
 
    while (n--)
    {
        int place; cin >> place;
        cout << dsu.add(place) << ' ';
    }
 
}

// Bosses
class DSU
{
private:
 
    vi sz, parent;
    vi len;
 
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
        , len(n+1)
    {
        std::iota(all(parent), 0);
    }
 
    // y главнее
    void merge(int x, int y)
    {
        // Два случая
        // Первый когда мы подсоединяем меньшего к большему как надо
        // Второй когда нам нужно подвесить их наоборот
 
        // В первом случае мы соединяем и в длину кладём 1-curr, где curr это len[y]
        // Во втором случае мы увеличиваем len[x] на 1, и отнимаем от len[y] и подвешиваем
        x = getParent(x).first;
        y = getParent(y).first;
 
        if (sz[y] < sz[x]) {
            len[x]++;
            len[y] -= len[x];
            std::swap(x, y);
        }
        else {
            len[x] += 1 - len[y];
        }
 
        sz[y] += sz[x];
        parent[x] = y;
    }
 
    std::pair<int, int> getParent(int x)
    {
        int length = 0;
        while (x != parent[x])
        {
            length += len[x];
            x = parent[x];
        }
        length += len[x];
        return { x, length };
    }
 
    int get(int x)
    {
        return getParent(x).second;
    }
    
};
 
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
    const int MERGE = 1, GET = 2;
    while (m--)
    {
        int type; cin >> type;
        if (MERGE == type)
        {
            int a, b; cin >> a >> b;
            dsu.merge(a, b);
        }
        else if (GET == type)
        {
            int c; cin >> c;
            std::cout << dsu.get(c) << '\n';
        }
        else throw 1;
    }
 
}

// Spannig Tree


class DSU
{
private:
 
    vi sz, parent;
 
 
    int getParent(int x)
    {
        return (x == parent[x] ? x : parent[x] = getParent(parent[x]));
    }
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+2)
    {
        std::iota(all(parent), 0);
    }
 
    void merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        parent[y] = x;
        sz[x] += sz[y];
    }
 
    bool get(int x, int y)
    {
        return getParent(x) == getParent(y);
    }
 
 
};
 
struct Edge
{
    int w, u, v;
 
    bool operator<(const Edge& edge)
    {
        return this->w < edge.w;
    }
 
    friend std::istream& operator>>(std::istream& in, Edge& edge)
    {
        in >> edge.u >> edge.v >> edge.w;
        return in;
    }
};
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
    ll answ = 0;
 
    vector<Edge> edges(m);
    cin >> edges;
    
    sort(all(edges));
 
    for (auto& it : edges)
    {
        int u = it.u;
        int v = it.v;
        int w = it.w;
 
        if (!dsu.get(u, v)) {
            dsu.merge(u, v);
            answ += w;
        }
    }
 
 
    std::cout << answ;
 
}

//   Dense spanning tree


class DSU
{
private:
 
    vi sz, parent, len;
    int n;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : parent[x] = getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
        , len(n+1)
        , n(n)
    {
        std::iota(all(parent), 0);
    }
 
    // len[x] — минимальное количество топлива необходимое чтобы доехать до корня
    bool merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return false;
        
        if (sz[x] < sz[y])
            std::swap(x, y);
        
        sz[x] += sz[y];
        parent[y] = x;
 
 
        return sz[x] == n;
    }
 
};
 
 
 
void solve()
{
    int n, m; cin >> n >> m;
    vector<tuple<int, int, int>> vec(m);
    for (auto& [w, u, v] : vec)
    {
        cin >> u >> v >> w;
    }
    sort(all(vec));
    ll answ = INFL;
    for (int i = 0; i < m; ++i)
    {
        DSU dsu(n);
        for (int j = i; j < m; ++j)
        {
            if (dsu.merge(get<1>(vec[j]), get<2>(vec[j])))
            {
                setmin(answ, get<0>(vec[j]) - get<0>(vec[i])); 
            }
        }
    }
 
 
    if (answ == INFL)
        std::cout << "NO";
    else {
        std::cout << "YES\n" << answ;
    }
 
}


// No refuel


class DSU
{
private:
 
    vi sz, parent, len;
 
    std::pair<int,int> getParent(int x)
    {
        int length = 0;
        while (x != parent[x])
        {
            setmax(length, len[x]);
            x = parent[x];
        }
        setmax(length, len[x]);
        return { x, length };
    }
 
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
        , len(n+1)
    {
        std::iota(all(parent), 0);
    }
 
    // len[x] — минимальное количество топлива необходимое чтобы доехать до корня
    void merge(int x, int y, int w)
    {
        std::pair<int,int> valx = getParent(x);
        std::pair<int,int> valy = getParent(y);
 
        // Если они в одной группе
        if (valx.first == valy.first)
            return;
 
        
        sz[valx.first] += sz[valy.first];
        parent[valy.first] = valx.first;
        setmax(len[valy.first], w);
    }
    
    int get(int x)
    {
        return getParent(x).second;
    }
 
};
 
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
 
    std::map<std::pair<int, int>, int> mp;
 
    while (m--)
    {
        int u, v, w; cin >> u >> v >> w;
        if (mp.find({ u,v }) != mp.end())
        {
            setmin(mp[{u, v}], w);
        }
        else mp[{u, v}] = w;
    }
 
    vector<tuple<int, int, int>> edges;
    for (auto& [key,val] : mp)
    {
        edges.push_back({ val,key.first,key.second });
    }
 
    sort(all(edges));
 
    for (auto& [w, u, v] : edges)
        dsu.merge(u, v, w);
 
    int answ = 0;
    for (int i = 1; i <= n; ++i)
    {
        setmax(answ, dsu.get(i));
    }
    std::cout << answ;
}

// Oil business

class DSU
{
private:
 
    vi sz, parent;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : parent[x] = getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
    {
        std::iota(all(parent), 0);
    }
 
    // y главнее
    bool merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return false;
 
        sz[x] += sz[y];
        parent[y] = x;
        return true;
    }
    
};
 
 
 
void solve()
{
    int n, m; ll s; cin >> n >> m >> s;
    vector<tuple<int, int, int,int>> vec;
    int index = 1;
    while (m--)
    {
        int u, v, w; cin >> u >> v >> w;
        vec.push_back({ w,u,v,index });
        ++index;
    }
    DSU dsu(n);
 
 
    sort(all(vec), [&](const auto& x, const auto& y) {
 
        return get<0>(x) > get<0>(y);
        });
 
    vector<tuple<int, int, int, int>> edges;
    for (auto& [w, u, v,index] : vec)
    {
        if (!dsu.merge(u, v))
        {
            edges.push_back({ w,u,v,index });
        }
    }
    vi answ;
 
 
    sort(all(edges));
    for (auto& [w, u, v, index] : edges)
    {
        if (s - w >= 0)
        {
            s -= w;
            answ.push_back(index);
        }
        else  break;
    }
 
    sort(all(answ));
 
    std::cout << isz(answ) << '\n';
    for (auto& it : answ)
        std::cout << it << ' ';
 
}

// Bipartite Graph

class DSU
{
private:
 
    vi sz, parent;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(2 * n + 2, 1)
        , parent(2 * n + 2)
    {
        std::iota(all(parent), 0);
    }
 
    
    void merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return;
 
        if (sz[x] < sz[y])
            std::swap(x, y);
        
        sz[x] += sz[y];
        parent[y] = x;
    }
 
 
    bool get(int x, int y)
    {
        return getParent(x) == getParent(y);
    }
 
};
 
 
 
void solve()
{
    int n, m; cin >> n >> m;
    int shift = 0;
    DSU dsu(n);
 
    const int MERGE = 0, GET = 1;
    while (m--)
    {
        int type, a, b; cin >> type >> a >> b;
        int x = (a + shift) % n;
        int y = (b + shift) % n;
 
        if (MERGE == type)
        {
            dsu.merge(x, n + y);
            dsu.merge(n + x, y);
        }
        else if (GET == type)
        {
            bool answ = dsu.get(x, y);
            if (answ)
                shift = (shift + 1) % n;
            std::cout << (answ ? "YES\n" : "NO\n");
        }
        else throw 1;
    }
 
}


// First Non-Bipartite Edge


class DSU
{
private:
 
    vi sz, parent, len;
 
 
    std::pair<int,int> getParent(int x)
    {
        if (x == parent[x])
            return { x,0 };
        std::pair<int, int> val = getParent(parent[x]);
        parent[x] = val.first;
        len[x] = (len[x] + val.second) % 2;
        return { parent[x], len[x] };
    }
 
public:
 
    DSU(const int n)
        : sz(n+1,1)
        , parent(n+1)
        , len(n+1)
    {
        std::iota(all(parent), 0);
    }
 
    bool merge(int x, int y)
    {
        std::pair<int, int> valX = getParent(x);
        std::pair<int, int> valY = getParent(y);
 
        if (valX.first == valY.first)
        {
            return valX.second != valY.second;
        }
 
        if (sz[valX.first] < sz[valY.first])
            std::swap(valX, valY);
 
 
        parent[valY.first] = valX.first;
        len[valY.first] = (1 + valX.second + valY.second) % 2;
        sz[valX.first] += sz[valY.first];
 
        return  1;
    }
 
   
 
};
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
    int answ{-1};
 
    for (int i = 1; i <= m; ++i)
    {
        int u, v; cin >> u >> v;
        if (!dsu.merge(u, v) && answ == -1)
        {
            answ = i;
        }
    }
 
    std::cout << answ;
 
 
 
}


/* # Step 3 */

// DSU with rollback


class DSU
{
private:
 
    vi sz, parent, next;
    int numSets;
    vector<tuple<pair<int, int>, pair<int, int>, int>> versions;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(n + 1, 1)
        , parent(n + 1)
        , numSets(n)
    {
        std::iota(all(parent), 0);
    }
 
    
    int merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return numSets;
 
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        versions.push_back({ {sz[x],x},{parent[y],y},numSets });
 
        sz[x] += sz[y];
        parent[y] = x;
        return --numSets;
    }
 
 
    int rollback(int version)
    {
        while (isz(versions) != version)
        {
            tuple<pair<int, int>, pair<int, int>, int> last = versions.back();
            sz[get<0>(last).second] = get<0>(last).first;
            parent[get<1>(last).second] = get<1>(last).first;
            numSets = get<2>(last);
            versions.pop_back();
        }
 
        return numSets;
    }
    
    int getVersion()    const
    {
        return isz(versions);
    }
 
};
 
 
 
void solve()
{
    int n, m; cin >> n >> m;
    DSU dsu(n);
 
    std::vector<int> versions;
    while (m--)
    {
        string type; cin >> type;
        if (type[0] == 'u')
        {
            int u, v; cin >> u >> v;
            std::cout << dsu.merge(u, v) << '\n';
        }
        else if (type[0] == 'p')
        {
            versions.push_back(dsu.getVersion());
        }
        else if (type[0] == 'r')
        {
            std::cout << dsu.rollback(versions.back()) << '\n';
            versions.pop_back();
        }
        else throw 1;
    }
 
}


// Number of Connected Components on Segments

class DSU
{
private:
 
    vi sz, parent, next;
    int numCC;
    vector<tuple<pair<int, int>, pair<int, int>, int>> versions;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(n + 1, 1)
        , parent(n + 1)
        , numCC(n)
    {
        std::iota(all(parent), 0);
    }
 
    
    void merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return;
 
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        versions.push_back({ {sz[x],x},{parent[y],y},numCC });
 
        sz[x] += sz[y];
        parent[y] = x;
        --numCC;
    }
 
 
    int rollback(int version)
    {
        while (isz(versions) != version)
        {
            tuple<pair<int, int>, pair<int, int>, int> last = versions.back();
            sz[get<0>(last).second] = get<0>(last).first;
            parent[get<1>(last).second] = get<1>(last).first;
            numCC = get<2>(last);
            versions.pop_back();
        }
 
        return numCC;
    }
 
 
    int getVersion()    const
    {
        return isz(versions);
    }
 
    int getAnsw()   const
    {
        return numCC;
    }
 
};
 
 
 
void solve()
{
    // Ответы буду записывать просто в мапу чтобы не хранить номера запросов
    map<pair<int, int>, int> answ;
    // Вводим рёбра
    int n, m; cin >> n >> m;
    vector<pair<int, int>> edges(m + 1);
    edges.push_back({ 0,0 });
    for (int i = 1; i <= m; ++i)
        cin >> edges[i].first >> edges[i].second;
    int k; cin >> k;
    // Ввели запросы
    vector<pair<int, int>> queries;
    for (int i = 0; i < k; ++i)
    {
        int l, r; cin >> l >> r;
        queries.push_back({ l,r });
    }
    // Запомним порядок
    const vector<pair<int, int>> remember = queries;
    // Посортили по левой границе
    sort(all(queries));
    // Теперь разбиваем на группы по sqrt(m)
    int gsize = (int)ceil(sqrt((m + 1) / 2.0));
    const int nG = (m + 1) / gsize + 1;
 
    int index = 0;
    // Начинаем обрабатывать группы
    for (int g = 0; g < nG; ++g)
    {
        const int start = g * gsize;
        const int end = min(m + 1, start + gsize);
 
        // easy запросы — у которых правая граница внутри блока
        vector<pair<int, int>> easy;
        // hard запросы — остальные
        vector<pair<int, int>> hard;
        while (index < k && queries[index].first < end)
        {
            if (queries[index].second < end)
                easy.push_back(queries[index]);
            else 
                hard.push_back(queries[index]);
            ++index;
        }
        // Сортим по правой границе
        sort(all(hard), [&](const auto& x, const auto& y) {
            return x.second < y.second;
            });
        // Поехали обрабатывать запросы
        // Начнём с простых 
        for (auto& [li, ri] : easy)
        {
            // Если запрос уже обрабатывали то ничего не надо
            if (answ.find({ li,ri }) != answ.end())
                continue;
            // Просто для каждого запроса создали DSU и втупую собрали ответ
            DSU dsu(n);
            for (int i = li; i <= ri; ++i)
                dsu.merge(edges[i].first, edges[i].second);
            answ[{li, ri}] = dsu.getAnsw();
        }
        // Теперь к сложным запросам
        // Создаём DSU с откатами
        DSU dsu(n);
        int right = end;
        for (auto& [li, ri] : hard)
        {
            if (answ.find({ li,ri }) != answ.end())
                continue;
            // Собираем все рёбра начиная с того где закончили до ri
            for (int i = right; i <= ri; ++i)
                dsu.merge(edges[i].first, edges[i].second);
            // Точка отката
            const int persist = dsu.getVersion();
            // Теперь идём влево с начала некст группы до li и получаем ответ
            for (int i = end - 1; i >= li; --i)
                dsu.merge(edges[i].first, edges[i].second);
            answ[{li, ri}] = dsu.getAnsw();
            // Теперь откатываем левую границу
            dsu.rollback(persist);
            // Двигаем границу для мержа
            right = ri + 1;
        }
 
    }
 
    // Выводим ответ
    for (auto& [l, r] : remember)
    {
        std::cout << answ[{l, r}] << '\n';
    }
}


// Dynamic Connectivity Offline
 
class DSU
{
private:
 
    vi sz, parent, next;
    int numCC;
    vector<tuple<pair<int, int>, pair<int, int>, int>> versions;
 
    int getParent(int x)
    {
        return x == parent[x] ? x : getParent(parent[x]);
    }
 
 
public:
 
    DSU(const int n)
        : sz(n + 1, 1)
        , parent(n + 1)
        , numCC(n)
    {
        std::iota(all(parent), 0);
    }
 
    
    void merge(int x, int y)
    {
        x = getParent(x);
        y = getParent(y);
        if (x == y)
            return;
 
        if (sz[x] < sz[y])
            std::swap(x, y);
 
        versions.push_back({ {sz[x],x},{parent[y],y},numCC });
 
        sz[x] += sz[y];
        parent[y] = x;
        --numCC;
    }
 
 
    int rollback(int version)
    {
        while (isz(versions) != version)
        {
            tuple<pair<int, int>, pair<int, int>, int> last = versions.back();
            sz[get<0>(last).second] = get<0>(last).first;
            parent[get<1>(last).second] = get<1>(last).first;
            numCC = get<2>(last);
            versions.pop_back();
        }
 
        return numCC;
    }
 
 
    int getVersion()    const
    {
        return isz(versions);
    }
 
    int getAnsw()   const
    {
        return numCC;
    }
 
 
    void SegTree(const vector <tuple<int, int, int, int>>& edges, const vector<int>& gets, int l, int r)
    {
        if (l + 1 == r)
        {
            const int persist = this->getVersion();
            auto x = lower_bound(all(edges), std::tuple<int, int, int, int>(0, 0, l, 0));
            if (x != edges.end() && get<2>(*x) <= l && get<3>(*x) >= r)
                this->merge(get<0>(*x), get<1>(*x));
                
            
            auto y = lower_bound(all(gets), l);
            if(y != gets.end() && *y == l)
                    std::cout << this->numCC << '\n';
                
            this->rollback(persist);
        }
        else {
            const int persist = this->getVersion();
            vector<int> gets2;
            vector<tuple<int, int, int, int>> edges2;
            for (auto& it : edges)
            {
                if (l >= get<2>(it) && r <= get<3>(it))
                    this->merge(get<0>(it), get<1>(it));
                else if(!(l >= get<3>(it) || r <= get<2>(it)))
                    edges2.push_back(it);
            }
 
            for (auto& t : gets)
                if (t >= l && t < r)
                    gets2.push_back(t);
 
            int mid = (l + r) >> 1;
            SegTree(edges2, gets2, l, mid);
            SegTree(edges2, gets2, mid, r);
 
            this->rollback(persist);
        }
    }
 
};
 
 
void solve()
{
    int n, m; cin >> n >> m;
    if (!m)
        return;
    vector<int> gets;
    vector<tuple<int, int, int>> adds;
    vector<tuple<int, int, int, int>> edges;
    for (int i = 0; i < m; ++i)
    {
        char type; cin >> type;
        if (type == '?')
            gets.push_back(i);
        else if (type == '+') {
            int u, v; cin >> u >> v;
            if (u > v)
                std::swap(u, v);
            adds.push_back({ u,v,i });
        }
        else if (type == '-')
        {
            int u, v; cin >> u >> v;
            if (u > v)
                std::swap(u, v);
            
            for (auto& it : adds)
            {
                if (u == get<0>(it) && v == get<1>(it))
                {
                    if (get<2>(it) >= i)
                        break;
                    edges.push_back({ u,v,get<2>(it),i + 1 });
                    it = std::tuple<int, int, int>{ 0,0,0 };
                    break;
                }
            }
        }
        else throw 1;
    }
 
    
    for (auto& q1 : adds)
    {
        if (q1 == std::tuple<int, int, int>{0, 0, 0})
            continue;
 
        edges.push_back({ get<0>(q1),get<1>(q1),get<2>(q1),m});
    }
    DSU dsu(n);
    dsu.SegTree(edges, gets, 0, m);
 
}
