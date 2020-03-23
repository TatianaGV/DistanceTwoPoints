#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <climits>

using namespace std;

using point = pair<int, int>;

ostream& operator<<(ostream& os, const point& p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

vector<point> read_points(istream& input) {
    int n = 0;
    input >> n;

    vector<point> result;

    for (int i = 0; i < n; ++i) {
        int x = 0, y = 0;
        input >> x >> y;
        result.emplace_back(x, y);
    }

    return result;
}

unsigned distance_squared(const point& lhs, const point& rhs) {
    return (lhs.first-rhs.first)*(lhs.first-rhs.first) +
           (lhs.second-rhs.second)*(lhs.second-rhs.second);
}

void find_minimum_distance_helper(
        vector<point>::const_iterator begin, vector<point>::const_iterator end,
        point& best_1, point& best_2, unsigned& best_d) {

    if (std::distance(begin, end) < 2) { return; }
    if (std::distance(begin, end) == 2) {
        unsigned d = distance_squared(*begin, *(begin+1));
        if (d < best_d) {
            best_d = d;
            best_1 = *begin;
            best_2 = *(begin+1);
        }
        return;
    }

    // разделяем
    auto middle = begin + std::distance(begin, end) / 2;

    // вызываем алгоритм для левой части
    find_minimum_distance_helper(begin, middle, best_1, best_2, best_d);

    // вызываем алгоритм для правой части
    find_minimum_distance_helper(middle, end, best_1, best_2, best_d);

    // властвуем (соединяем результаты двух вызовов)
    vector<point> helper(std::distance(begin, end));
    std::merge(begin, middle, middle, end, helper.begin(), [](auto p1, auto p2) {
        return p1.second == p2.second ? p1.first < p2.first : p1.second < p2.second;
    });


    for (auto it = helper.begin(); it != helper.end(); ++it) {
        // рассматриваем только те точки, которые находятся на расстоянии (middle.x - it.x)^2 < best_d
        if ((middle->first - it->first)*(middle->first - it->first) < best_d) {
            // для каждой точки рассматриваем только 6 её соседей
            for (int i = 1; (i < 7) && (it + i != helper.end()); ++i) {
                unsigned d = distance_squared(*it, *(it+i));
                if (d < best_d) {
                    best_d = d;
                    best_1 = *it;
                    best_2 = *(it+i);
                }
            }
        }
    }

    return;
}

tuple <point, point, unsigned> find_minimum_distance(const vector<point>& vec) {
    point best_lhs;
    point best_rhs;
    unsigned best_d = UINT_MAX;

    find_minimum_distance_helper(vec.cbegin(), vec.cend(), best_lhs, best_rhs, best_d);

    return {best_lhs, best_rhs, best_d};
}

int main() {
    auto vec = read_points(cin);
    sort(vec.begin(), vec.end(), [](auto p1, auto p2) {
        return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first;
    });

    point p1, p2;
    unsigned distance;
    tie(p1, p2, distance) = find_minimum_distance(vec);

    cout << distance << endl;
    cout << "between " << p1 << " and " << p2 << endl;

    return 0;
}



