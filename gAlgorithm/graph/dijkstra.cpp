#include<string>
#include<map>
#include<list>
#include<iostream>
using namespace std;

void dijkstra(map<string, map<string, int>> & graph, string begin, string end) {
    class result_s {
        public:
            string name;
            int distance;
            result_s *parent;
            result_s(string name, int distance, result_s *parent):
                name(name), distance(distance), parent(parent)  { }
    };
    map<string, result_s *> result; // 已访问节点
    list<result_s *>     member;      // 待访问节点
    list<string> path;
    struct result_s *curr;

    int hack_count = 0;

    member.push_back(new result_s(begin, 0, NULL)); // 1. **起点** 插入待处理队列
    while (!member.empty()) {
        // 2. 待处理队列中 找到 开销最小的节点 出列
        list<result_s *>::iterator it_curr = member.begin();
        for (list<result_s *>::iterator it = member.begin(); it != member.end(); it++)
            if ((*it)->distance < (*it_curr)->distance) {
                it_curr = it;
            }
        curr = *it_curr;
        member.erase(it_curr);

        // 3. 遍历节点的邻居
        for(map<string, int>::iterator it = graph[curr->name].begin(); it != graph[curr->name].end(); ++it) {
            int new_distance = it->second + curr->distance;
            if (!result[it->first]) { // 该节点不在result 说明没有被遍历
                // 3.1 如果 邻居不在hash中则插入到hash 和 待处理队列
                result_s * neigh = new result_s(it->first, new_distance, curr);
                member.push_back(neigh);
                result[neigh->name] = neigh;
            } else if (new_distance < result[it->first]->distance)   { // 3.2 检查是否有前往邻居的更短路径
                // 3.3 如果 有更短路径 则更新到该邻居节点在hash的开销 和 最近父节点
                result[it->first]->distance = it->second + curr->distance;
                result[it->first]->parent = curr;
            }
        }
    }

    // 4.
    curr = result[end];
    while(curr->name != begin) {
        path.push_front(curr->name);
        curr = curr->parent;
    }
    path.push_front(begin);

    for (list<string>::iterator it = path.begin(); it != path.end(); ++it) {
        cout << *it << '\n';
    }
}

int main( int argc, char **argv ) {
    map<string, map<string, int>>graph;
    graph["begin"] = map<string, int>();
    graph["A"] = map<string, int>();
    graph["B"] = map<string, int>();
    graph["end"] = map<string, int>();

    graph["begin"]["A"] = 6; graph["begin"]["B"] = 2;

    graph["A"]["end"] = 1;

    graph["B"]["A"] = 3;
    graph["B"]["end"] = 5;

    dijkstra(graph, "begin", "end");
    return 0;
}
