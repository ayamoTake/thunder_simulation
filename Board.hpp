#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<cmath>
#include<random>
#include<set>

/*
 * Issue: 袋小路になると終わる()
 * */

using namespace std;
struct Index {
    int r, c;
};

inline bool operator<(const Index& a, const Index&b) {
    if (a.r != b.r) return a.r < b.r;
    return a.c < b.c;
}

class Cell {
public:
    bool is_thunder = false;
    int around_ct = 0;
};

class Board {
public:
    vector<vector<Cell>> cells;
    Board(int n, double eta, int seed) : eta(eta), pushed(n, vector<bool>(n, false)), rand_select_gen(seed) { _init(n); }
    Index select();
    bool add_as_thunder(const Index& idx);
    double prob (const Index& idx) const;
    int e(const Index& idx) const;
    int e2(const Index& idx) const;
    void print() const;
    void d_print() const;
    bool ended() const { return will.empty(); }
    int row_size;
    int col_size;
    double eta = 5;
private:
    vector<Index> four_neighbor(const Index& tar) const;
    vector<vector<bool>> pushed;
    set<Index> _init_cell;
    deque<Index> will;
    mutable mt19937 rand_gen;
    mutable mt19937 rand_select_gen;

    bool ok(const Index& tar) const;
    void _init(int n);
};

inline bool Board::ok(const Index& idx) const {
    return idx.r >= 0 && idx.r < row_size
        && idx.c >= 0 && idx.c < col_size;
};

inline vector<Index> Board::four_neighbor(const Index& idx) const {
    vector<Index> ans;
    vector<vector<int>> diff = { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, };
    for (const auto& d : diff) {
        Index tmp = {idx.r + d[0], idx.c + d[1]};
        if (tmp.c == -1) tmp.c = col_size - 1;
        else if (tmp.c == col_size) tmp.c = 0;
        if (ok(tmp)) {
            ans.push_back(tmp);
        }
    }
    return ans;
}

inline bool Board::add_as_thunder(const Index& idx) {
    if (cells[idx.r][idx.c].is_thunder) {
        cerr << "have already been thunder\n";
        cerr << idx.r << ", " << idx.c << endl;
        exit(1);
    }
    cells[idx.r][idx.c].is_thunder = true;
    cells[idx.r][idx.c].around_ct = 4;

    vector<Index> four_neis = four_neighbor(idx);
    shuffle(four_neis.begin(), four_neis.end(), rand_gen);

    for (const Index& nei: four_neis) {
        if (
            cells[nei.r][nei.c].is_thunder 
            || _init_cell.find(nei) != _init_cell.end()
        ) continue;

        cells[nei.r][nei.c].around_ct++;
        if (!pushed[nei.r][nei.c]) {
            if (will.size() >= 10) {
                will.pop_front();
            }
            will.push_back(nei);
            pushed[nei.r][nei.c] = true;
        }
    }

    return idx.r == row_size - 1;
}

inline int Board::e(const Index& idx) const {
    return cells[idx.r][idx.c].around_ct;
}

inline int Board::e2(const Index& idx) const {
    if (!ok(idx)) return 0;
    if (cells[idx.r][idx.c].is_thunder) return 1;
    int sum = 0;
    for (const Index& nei: four_neighbor(idx)) {
        sum += this->e(nei);
    }
    return sum;
}

inline double Board::prob(const Index& idx) const {
    return pow(1 - e2(idx) / 16.0, eta) * 100;
}

inline void Board::_init(int n) {
    row_size = n;
    col_size = n / 9 * 16;
    cells.assign(row_size, vector<Cell>(col_size));

    for (int i = 0; i < col_size; i++) {
        _init_cell.insert({0, i});
    }
    int sx = col_size / 2;
    _init_cell.insert({1, sx});

    for (const Index& idx: _init_cell) {
        add_as_thunder(idx);
    }
}

inline Index Board::select() {
    vector<decltype(will.begin())> target;
    for (auto it = will.begin(); it != will.end(); it++) {
        target.push_back(it);
    }
    int n = target.size();
    vector<double> ps(n);
    for (int i = 0; i < n; i++) {
        ps[i] = prob(*target[i]);
    }
    vector<double> sums(will.size());
    partial_sum(ps.begin(), ps.end(), sums.begin());

    uniform_real_distribution<> dist(0.0, sums[n - 1]);
    int r = dist(rand_select_gen);
    int ans_idx = lower_bound(sums.begin(), sums.end(), r) - sums.begin();

    Index ans = *target[ans_idx];
    /* 
     * erase は線形時間. timestamp を使ったmin-heap でできる??
     * */
    will.erase(target[ans_idx]); 

    return ans;
}

inline void Board::d_print() const {
    cout << "\033[2J";

    double sum = 0;
    set<Index> will_set;
    for ( auto idx : will) {
        will_set.insert(idx);
        double p = prob(idx);
        sum += p;
        // cout << p << endl;
    }
    // cout << "sum: " << sum << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            if (cells[i][j].is_thunder) {
                printf("\033[33m██\033[0m");
            } else if (pushed[i][j]) {
                printf("%02d", (int) (prob({i, j}) / sum * 100));
            } else {
                printf("  ");
            }
        }
        cout << endl;
    }
}

inline void Board::print() const {
    cout << "\033[2J";

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            if (cells[i][j].is_thunder) {
                printf("\033[33m██\033[0m");
            } else {
                printf("  ");
            }
        }
        cout << endl;
    }
}
