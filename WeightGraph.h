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

#ifndef LabelPropgation_h_h
#define LabelPropgation_h_h

class weightGraph
{
public:
    int totalnodes;
    int totallinks;
    vector<string> Nodenames;
    vector<vector<int> > Neighbors;
    map<pair<int,int>,double> Linklist;
    vector<set<int> > node_links;

public:
    weightGraph();
    ~weightGraph();

    void loading(string &inputFileName);
    int node_degree(int nodeNum);
    pair<pair<int,int>,double> GetLink(int edgeNum);
    int find_offset(string nodename);

    int GetVcount(){return totalnodes;}
    int GetEcount(){return totallinks;}

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

