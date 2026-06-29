class SD
{
private:
    const int gsize = 256;
    std::vector<int> a;
    std::vector<long long> s;

    void buildGroup(int g)
    {
        const int begin = g * gsize;
        const int end = std::min(begin + gsize, (int)a.size());
        //  [begin, end)
        for (int i = begin; i < end; ++i)
            s[g] += a[i];
    }

public:


    SD(const std::vector<int>& vec)
    {
        build(vec);
    }

    void build(const std::vector<int>& vec)
    {
        a = vec;
        const int n = (int)vec.size();
        const int nG = (n + gsize - 1) / gsize;         // count of Groups
        s.assign(nG, 0);
        for (int g = 0; g < nG; ++g)
            buildGroup(g);
    }

    void set(int index, int value)
    {
        s[index / gsize] += value - a[index];
        a[index] = value;
    }

    ll get_sum(int left, int right)
    {
        //  [left, right]
        ll res = 0;
        int gl = left / gsize;
        int gr = right / gsize;
        if (gl == gr)
        {
            for (int i = left; i <= right; ++i)
                res += a[i];
        }
        else
        {
            for (int i = left; i < (gl + 1) * gsize; ++i)
                res += a[i];
            for (int g = gl + 1; g < gr; ++g)
                res += s[g];
            for (int i = gr * gsize; i <= right; ++i)
                res += a[i];
        }
        return res;
    }

};