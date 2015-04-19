#include <iostream>
#include "world/grid.hpp"
#include "world/cell.hpp"

using namespace std;

class int_cell : public cell {
    public:
    int x;
    int_cell(const int x, const int y) : cell(x, y), x(0) {}
};

int main(int argc, char *argv[]) {
    
    grid<int_cell> g(10, 10);
    
    int i = 0;
    for (grid<int_cell>::iterator it = g.begin();
        it != g.end();
        ++it)

    {
        int n = 0;
        for (grid<int_cell>::neighbour_iterator n_it = g.neighbour_begin(*it);
            n_it != g.neighbour_end(*it);
            ++ n_it)

        {

            ++n;

        }
        ++i;
        cout << n << " ";
        if (i % g.width == 0) {
            cout << endl;
        }
    }
    
    cout << endl;

    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            cout << g.get_distance_to_edge(g[j][i]) << " ";
        }
        cout << endl;
    }

    return 0;
}
