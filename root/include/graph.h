#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <utility>
class Graph {
    private:
        std::map<long int, std::vector<std::pair<double, long int> > > nodes;
        static Graph* instance;
        Graph();

    public:
        static long int uid_counter;
        static long int uid();
        static Graph* getInstance();

        void connect(const long int& uid, const std::pair<double, long int>& edge);
        std::vector<std::pair<double, long int> > get(const long int& uid) const;
        bool has(const long int& uid) const;

        void new_recording();
};

#endif /* end of include guard: GRAPH_H */
