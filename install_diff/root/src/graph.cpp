#include "../include/graph.h"
#include <cstddef>

Graph* Graph::instance = 0;
long int Graph::uid_counter = 1;

long int Graph::uid(){
    return uid_counter++;
}

Graph* Graph::getInstance(){
    if(instance==NULL){
        instance = new Graph();
    }
    return instance;
}

Graph::Graph(){}

void Graph::connect(const long int& uid, const std::pair<double, long int>& edge){
    nodes[uid].push_back(edge);
}

std::vector<std::pair<double, long int> > Graph::get(const long int& uid) const{
    return nodes.at(uid);
}

bool Graph::has(const long int& uid) const{
    return nodes.find(uid)!=nodes.end();
}

void Graph::new_recording(){
    nodes.clear();
}
