#include<iostream>
#include <vector>
#include "loadingGraph.h"
//#include "WeightGraph.h"

#ifndef EvolutionFunction_h_h
#define EvolutionFunction_h_h

typedef pair<int, int> PAIR;
int cmp(const PAIR &x, const PAIR &y)
{
    return x.second > y.second;
}

void beginComputeModularity(vector<vector<int> > myCommunity,Graph*graph,ofstream &resultQov, ofstream & resultaverageon, ofstream & resultaverageom, ofstream & resultmaxom);

void beginComputeNMI(string outputFileName,vector<vector<int> > myCommunity,Graph*graph, ofstream &out);

vector<vector<int> > file2TrueCommunity(const string & TrueCommunity_filename);

//vector<vector<int> > file2TrueCommunity2(string & TrueCommunity_filename, Graph *graph);

double CalculateNMI(vector<vector<int> > TrueCommunity , vector<vector<int> > myCommunity);

#endif // LOADINGGRAPH_H_H
