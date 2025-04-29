#include <iostream>
#include<queue>

int main() {
    std::priority_queue<int> q;
    q.push(1);
    q.push(2);
    q.push(1);
    for (int e = q.top(); !q.empty(); q.pop(), e = q.top())  {
        std::cout << e << "\n";
    }
}
