#include<iostream>
#include "loadingGraph.h"
//#include "WeightGraph.h"

vector<vector<int> > labelpropagation(Graph *graph,double threshold);

int labelfunction(vector<pair<int,double> > link_similarity, Graph *graph,vector<int> label,int linknum);


