#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;
struct Index {
    int r, c;
};

class Cell {
public:
    bool is_thunder = false;
    int e = 0;
    int prob = 0;
};

class Board {
public:
    vector<vector<Cell>> cells;
    Board(int n) : comp(this), _Q(comp), pushed(n, vector<bool>(n, false)) { _init(n); }
    void add_as_thunder(const Index& idx);
    Index select();
    int prob (const Index& idx) const;
    void print() const;
    bool ended() const { return _Q.empty(); }
private:
    struct Comp {
        Board* parent;
        Comp (Board* p) : parent(p) {}
        bool operator()(const Index& a, const Index& b) const {
            return parent->prob(a) > parent->prob(b);
        }
    };
    Comp comp;
    priority_queue<Index, vector<Index>, Comp> _Q;
    vector<Index> four_neighbor(const Index& tar) const;
    vector<vector<bool>> pushed;
    bool ok(const Index& tar) const;
    void _init(int n);
};

inline bool Board::ok(const Index& idx) const {
    return idx.r >= 0 && idx.r < cells.size()
        && idx.c >= 0 && idx.c < cells.size();
};

inline vector<Index> Board::four_neighbor(const Index& idx) const {
    vector<Index> ans;
    vector<vector<int>> diff = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, };
    for (const auto& d : diff) {
        Index tmp = {idx.r + d[0], idx.c + d[1]};
        if (ok(tmp)) {
            ans.push_back(tmp);
        }
    }
    return ans;
}

inline void Board::add_as_thunder(const Index& idx) {
    if (cells[idx.r][idx.c].is_thunder 
        // && !(idx.r == 1 && idx.c == cells.size() / 2)
    ) {
        cerr << "have already been thunder\n";
        cerr << idx.r << ", " << idx.c << endl;
        exit(1);
    }
    cells[idx.r][idx.c].e = 4;
    cells[idx.r][idx.c].is_thunder = true;
    for (const Index& nei: four_neighbor(idx)) {
        if (
            !cells[nei.r][nei.c].is_thunder 
            && nei.r != 0 && !(nei.r == 1 && nei.c == cells.size() / 2)
            && !pushed[nei.r][nei.c]
        ) {
            cells[nei.r][nei.c].e++;
            _Q.push(nei);
            pushed[nei.r][nei.c] = true;
        }
    }
}

inline int Board::prob(const Index& idx) const {
    int sum = 0;
    for (auto nei: four_neighbor(idx)) {
        sum += cells[nei.r][nei.c].e;
    }
    return sum;
}

inline void Board::_init(int n) {
    cells.assign(n, vector<Cell>(n));
    for (int i = 0; i < n; i++) {
        add_as_thunder({0, i});
    }
    int sx = n / 2;
    add_as_thunder({1, sx});
}

inline Index Board::select() {
    Index ans = _Q.top();
    _Q.pop();
    return ans;
}

inline void Board::print() const {
    cout << "\033[2J";

    auto q = _Q;
    for (int i = 0; i < q.size(); i++) {
        Index idx = q.top(); q.pop();
        cout << "(" << idx.r << ", " << idx.c << ") ";
    }
    cout << endl;

    for (const auto& r: cells) {
        for (const auto& c: r) {
            if (c.is_thunder) {
                cout << "\033[33m" << "██" << "\033[0m";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}
