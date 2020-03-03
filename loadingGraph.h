#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<set>
#include<map>
#include<utility>
#include<cstdlib>
#include<algorithm>
using namespace std;

#ifndef loadingGraph_h_h
#define loadingGraph_h_h

//#ifndef EvolutionFunction_h_h
//#define EvolutionFunction_h_h

class Graph
{
public:
    vector<string> Nodenames;
    vector<vector<int> > Neighbors;
    int vcount,ecount;
    vector<pair<int,int> > Linklist;
    vector<set<int> > node_links;
    map<string,int> nodename_to_index;

public:
    Graph();
    ~Graph();
    void loading(string &inputFileName);
    int node_degree(int nodeNum);
    pair<int,int> GetLink(int edgeNum);
    int find_offset(string nodename);
    int GetVcount(){return vcount;}
    int GetEcount(){return ecount;}
    bool isConnect(int node1,int node2)
    {
        if (find(Neighbors[node1].begin(),Neighbors[node1].end(),node2)!=Neighbors[node1].end())
            return true;
        else
            return false;
    }
    void nodeTolinks();
};

#endif // LOADINGGRAPH_H_H
//#endif
