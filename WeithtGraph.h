#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<set>
#include<utility>
#include<cstdlib>
#include<algorithm>
using namespace std;

#ifndef LabelPropgation_h_h
#define LabelPropgation_h_h

class weightGraph
{
private:
    size_t totalnodes;
    size_t totallinks;
    set<string> Nodenames;
    vector<vector<size_t> > Neighbors;
    map<pair<size_t,size_t>,double> Linklist;
    vector<set<size_t> > node_links;

public:
    weightGraph();
    ~weightGraph();

    void loading(string &inputFileName);
    size_t node_degree(size_t nodeNum);
    pair<pair<size_t,size_t>,double> GetLink(size_t edgeNum);
    size_t find_offset(string nodename);

    size_t GetVcount(){return vcount;}
    size_t GetEcount(){return ecount;}

    bool isConnect(size_t node1,size_t node2)
    {
        if (find(Neighbors[node1].begin(),Neighbors[node1].end(),node2)!=Neighbors[node1].end())
            return true;
        else
            return false;
    }

    void nodeTolinks();

};

#endif // LOADINGGRAPH_H_H
