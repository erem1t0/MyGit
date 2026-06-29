#include <vector>
#include <iostream>
using namespace std;

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;

const ll module = 998244353;

class SegmentTree
{
private:

    int sizex, sizey;
    vvll tree;
    vvll a;
    int startx, endx, starty, endy, v_res = 0;
    ll neutral;

    void clear(int vx, int vy)
    {
        tree[vx][vy] = neutral;
    }

    void merge(int targetx, int targety, int sourcex, int sourcey)
    {
        tree[targetx][targety] = (tree[targetx][targety] + tree[sourcex][sourcey]) % module;
    }

    void update_vertex_x(int vx, int v_leftx, int v_rightx, int vy)
    {
        clear(vx, vy);
        merge(vx, vy, v_leftx, vy);
        merge(vx, vy, v_rightx, vy);
    }

    void update_vertex_y(int vy, int v_lefty, int v_righty, int vx)
    {
        clear(vx, vy);
        merge(vx, vy, vx, v_lefty);
        merge(vx, vy, vx, v_righty);
    }

    void build_tree_x(int vx, int leftx, int rightx)
    {
        if (leftx + 1 != rightx)
        {
            int midx = (leftx + rightx) >> 1;
            int v_leftx = (vx << 1), v_rightx = v_leftx + 1;
            build_tree_x(v_leftx, leftx, midx);
            build_tree_x(v_rightx, midx, rightx);
        }  
        build_tree_y(vx, leftx, rightx, 1, 0, sizey);
    }

    void build_tree_y(int vx, int leftx, int rightx, int vy, int lefty, int righty)
    {
        int v_leftx = (vx << 1), v_rightx = v_leftx + 1;
        if (lefty + 1 == righty)
        {
            if (leftx + 1 == rightx)
                tree[vx][vy] = a[leftx][lefty];
            else
                update_vertex_x(vx, v_leftx, v_rightx, vy);
        }
        else {
            int midy = (lefty + righty) >> 1;
            int v_lefty = (vy << 1), v_righty = v_lefty + 1;
            build_tree_y(vx, leftx, rightx, v_lefty, lefty, midy);
            build_tree_y(vx, leftx, rightx, v_righty, midy, righty);
            update_vertex_y(vy, v_lefty, v_righty, vx);
        }
    }

    void get_tree_segx(int vx, int leftx, int rightx)
    {
        if (startx <= leftx && rightx <= endx)
        {
            get_tree_segy(vx, 1, 0, sizey);
        }
        else {
            int midx = (leftx + rightx) >> 1;
            int v_leftx = (vx << 1), v_rightx = v_leftx + 1;
            if(startx < midx)
                get_tree_segx(v_leftx, leftx, midx);
            if(endx > midx)
                get_tree_segx(v_rightx, midx, rightx);
        }
    }

    void get_tree_segy(int vx, int vy, int lefty, int righty)
    {
        if (starty <= lefty && righty <= endy)
        {
            merge(v_res, v_res, vx, vy);
        }
        else {
            int midy = (lefty + righty) >> 1;
            int v_lefty = (vy << 1), v_righty = v_lefty + 1;
            if (starty < midy)
                get_tree_segy(vx, v_lefty, lefty, midy);
            if (endy > midy)
                get_tree_segy(vx, v_righty, midy, righty);
        }
    }


public:


    SegmentTree(int n, int m, ll neutral = 0ll)
        : sizex(n)
        , sizey(m)
        , tree(sizex << 2, vll(sizey << 2, SegmentTree::neutral))
    { }


    void build(const vvll& vec)
    {
        a = vec;
        build_tree_x(1, 0, sizex);
    }


    ll get_sum(int startx, int endx, int starty, int endy)
    {
        SegmentTree::startx = startx;
        SegmentTree::endx = endx + 1;
        SegmentTree::starty = starty;
        SegmentTree::endy = endy + 1;

        clear(v_res, v_res);
        get_tree_segx(1, 0, sizex);

        return tree[v_res][v_res];
    }

    void print()
    {
        for (int i = 0; i < sizex; ++i)
        {
            for (int j = 0; j < sizey; ++j)
                cout << tree[i][j] << ' ';
            cout << '\n';
        }
    }
};