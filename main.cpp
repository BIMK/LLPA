#include<iostream>
#include<string>
#include<cmath>
#include<set>
#include<map>
#include<assert.h>
#include<algorithm>
#include <iomanip>

#include <time.h>

#include "LabelPropagation.h"
//#include "EvolutionFunction.h"
#include "pajek.h"
//#include "CompareToOCG.h"


using namespace std;


int main(int argc,char *argv[])
{
    //function declaring
    double CalculateQov(vector<vector<int> > myCommunity,Graph *graph,ofstream &resultQov, ofstream & resultaverageon, ofstream & resultaverageom, ofstream & resultmaxom);
    void pajek(string inputFileName, vector<vector<int> >myCommunity, Graph *graph);
    void printCom(vector<vector<int> > myCommunity, Graph * graph, string resultfilename);

    void beginComputeNMI(string outputFileName,vector<vector<int> > Community,Graph*graph,ofstream &outNMI, ofstream & outoverlap);
    pair<double,pair<double,double> > beginComputeFMeasure(vector<vector<int> >myCommunity,string &inputFileName,string &TrueCommunityFileName,Graph *graph,double alpha);
    vector<vector<int> >file2TrueCommunity2(string &TrueCommunity_filename, Graph *graph);

//    parameter error detection, write in the further
    string inputFileName=argv[1];
    string TrueCommunityFileName=argv[2];
    double threshold = atof(argv[3]);
    double delta = atof(argv[4]);
    string resultfilename = argv[5];


/*
//string inputFileName = "PPI_D1.txt";
//string TrueCommunityFileName = "Cmplx-D1.txt";
//double threshold = 0.6;
//string resultfilename = "result.txt";


    //set parameters;
    string inputFileName1[363] = {"PPI_D1.txt","PPI_D1_add_0.100000_0.txt","PPI_D1_add_0.100000_1.txt","PPI_D1_add_0.100000_2.txt",
                                    "PPI_D1_add_0.100000_3.txt","PPI_D1_add_0.100000_4.txt","PPI_D1_add_0.100000_5.txt","PPI_D1_add_0.100000_6.txt",
                                    "PPI_D1_add_0.100000_7.txt","PPI_D1_add_0.100000_8.txt","PPI_D1_add_0.100000_9.txt",
                                    "PPI_D1_add_0.200000_0.txt","PPI_D1_add_0.200000_1.txt","PPI_D1_add_0.200000_2.txt",
                                    "PPI_D1_add_0.200000_3.txt","PPI_D1_add_0.200000_4.txt","PPI_D1_add_0.200000_5.txt",
                                    "PPI_D1_add_0.200000_6.txt","PPI_D1_add_0.200000_7.txt","PPI_D1_add_0.200000_8.txt","PPI_D1_add_0.200000_9.txt",
                                    "PPI_D1_add_0.300000_0.txt","PPI_D1_add_0.300000_1.txt","PPI_D1_add_0.300000_2.txt",
                                    "PPI_D1_add_0.300000_3.txt","PPI_D1_add_0.300000_4.txt","PPI_D1_add_0.300000_5.txt",
                                    "PPI_D1_add_0.300000_6.txt","PPI_D1_add_0.300000_7.txt","PPI_D1_add_0.300000_8.txt","PPI_D1_add_0.300000_9.txt",
                                    "PPI_D1_add_0.400000_0.txt","PPI_D1_add_0.400000_1.txt","PPI_D1_add_0.400000_2.txt",
                                    "PPI_D1_add_0.400000_3.txt","PPI_D1_add_0.400000_4.txt","PPI_D1_add_0.400000_5.txt",
                                    "PPI_D1_add_0.400000_6.txt","PPI_D1_add_0.400000_7.txt","PPI_D1_add_0.400000_8.txt","PPI_D1_add_0.400000_9.txt",
                                    "PPI_D1_add_0.500000_0.txt","PPI_D1_add_0.500000_1.txt","PPI_D1_add_0.500000_2.txt",
                                    "PPI_D1_add_0.500000_3.txt","PPI_D1_add_0.500000_4.txt","PPI_D1_add_0.500000_5.txt",
                                    "PPI_D1_add_0.500000_6.txt","PPI_D1_add_0.500000_7.txt","PPI_D1_add_0.500000_8.txt","PPI_D1_add_0.500000_9.txt",
                                    "PPI_D1_add_0.600000_0.txt","PPI_D1_add_0.600000_1.txt","PPI_D1_add_0.600000_2.txt",
                                    "PPI_D1_add_0.600000_3.txt","PPI_D1_add_0.600000_4.txt","PPI_D1_add_0.600000_5.txt",
                                    "PPI_D1_add_0.600000_6.txt","PPI_D1_add_0.600000_7.txt","PPI_D1_add_0.600000_8.txt","PPI_D1_add_0.600000_9.txt",
                                    "PPI_D1_remove_0.100000_0.txt","PPI_D1_remove_0.100000_1.txt","PPI_D1_remove_0.100000_2.txt",
                                    "PPI_D1_remove_0.100000_3.txt","PPI_D1_remove_0.100000_4.txt","PPI_D1_remove_0.100000_5.txt","PPI_D1_remove_0.100000_6.txt",
                                    "PPI_D1_remove_0.100000_7.txt","PPI_D1_remove_0.100000_8.txt","PPI_D1_remove_0.100000_9.txt",
                                    "PPI_D1_remove_0.200000_0.txt","PPI_D1_remove_0.200000_1.txt","PPI_D1_remove_0.200000_2.txt",
                                    "PPI_D1_remove_0.200000_3.txt","PPI_D1_remove_0.200000_4.txt","PPI_D1_remove_0.200000_5.txt",
                                    "PPI_D1_remove_0.200000_6.txt","PPI_D1_remove_0.200000_7.txt","PPI_D1_remove_0.200000_8.txt","PPI_D1_remove_0.200000_9.txt",
                                    "PPI_D1_remove_0.300000_0.txt","PPI_D1_remove_0.300000_1.txt","PPI_D1_remove_0.300000_2.txt",
                                    "PPI_D1_remove_0.300000_3.txt","PPI_D1_remove_0.300000_4.txt","PPI_D1_remove_0.300000_5.txt",
                                    "PPI_D1_remove_0.300000_6.txt","PPI_D1_remove_0.300000_7.txt","PPI_D1_remove_0.300000_8.txt","PPI_D1_remove_0.300000_9.txt",
                                    "PPI_D1_remove_0.400000_0.txt","PPI_D1_remove_0.400000_1.txt","PPI_D1_remove_0.400000_2.txt",
                                    "PPI_D1_remove_0.400000_3.txt","PPI_D1_remove_0.400000_4.txt","PPI_D1_remove_0.400000_5.txt",
                                    "PPI_D1_remove_0.400000_6.txt","PPI_D1_remove_0.400000_7.txt","PPI_D1_remove_0.400000_8.txt","PPI_D1_remove_0.400000_9.txt",
                                    "PPI_D1_remove_0.500000_0.txt","PPI_D1_remove_0.500000_1.txt","PPI_D1_remove_0.500000_2.txt",
                                    "PPI_D1_remove_0.500000_3.txt","PPI_D1_remove_0.500000_4.txt","PPI_D1_remove_0.500000_5.txt",
                                    "PPI_D1_remove_0.500000_6.txt","PPI_D1_remove_0.500000_7.txt","PPI_D1_remove_0.500000_8.txt","PPI_D1_remove_0.500000_9.txt",
                                    "PPI_D1_remove_0.600000_0.txt","PPI_D1_remove_0.600000_1.txt","PPI_D1_remove_0.600000_2.txt",
                                    "PPI_D1_remove_0.600000_3.txt","PPI_D1_remove_0.600000_4.txt","PPI_D1_remove_0.600000_5.txt",
                                    "PPI_D1_remove_0.600000_6.txt","PPI_D1_remove_0.600000_7.txt","PPI_D1_remove_0.600000_8.txt","PPI_D1_remove_0.600000_9.txt",
                                    "PPI_D2.txt","PPI_D2_add_0.100000_0.txt","PPI_D2_add_0.100000_1.txt","PPI_D2_add_0.100000_2.txt",
                                    "PPI_D2_add_0.100000_3.txt","PPI_D2_add_0.100000_4.txt","PPI_D2_add_0.100000_5.txt","PPI_D2_add_0.100000_6.txt",
                                    "PPI_D2_add_0.100000_7.txt","PPI_D2_add_0.100000_8.txt","PPI_D2_add_0.100000_9.txt",
                                    "PPI_D2_add_0.200000_0.txt","PPI_D2_add_0.200000_1.txt","PPI_D2_add_0.200000_2.txt",
                                    "PPI_D2_add_0.200000_3.txt","PPI_D2_add_0.200000_4.txt","PPI_D2_add_0.200000_5.txt",
                                    "PPI_D2_add_0.200000_6.txt","PPI_D2_add_0.200000_7.txt","PPI_D2_add_0.200000_8.txt","PPI_D2_add_0.200000_9.txt",
                                    "PPI_D2_add_0.300000_0.txt","PPI_D2_add_0.300000_1.txt","PPI_D2_add_0.300000_2.txt",
                                    "PPI_D2_add_0.300000_3.txt","PPI_D2_add_0.300000_4.txt","PPI_D2_add_0.300000_5.txt",
                                    "PPI_D2_add_0.300000_6.txt","PPI_D2_add_0.300000_7.txt","PPI_D2_add_0.300000_8.txt","PPI_D2_add_0.300000_9.txt",
                                    "PPI_D2_add_0.400000_0.txt","PPI_D2_add_0.400000_1.txt","PPI_D2_add_0.400000_2.txt",
                                    "PPI_D2_add_0.400000_3.txt","PPI_D2_add_0.400000_4.txt","PPI_D2_add_0.400000_5.txt",
                                    "PPI_D2_add_0.400000_6.txt","PPI_D2_add_0.400000_7.txt","PPI_D2_add_0.400000_8.txt","PPI_D2_add_0.400000_9.txt",
                                    "PPI_D2_add_0.500000_0.txt","PPI_D2_add_0.500000_1.txt","PPI_D2_add_0.500000_2.txt",
                                    "PPI_D2_add_0.500000_3.txt","PPI_D2_add_0.500000_4.txt","PPI_D2_add_0.500000_5.txt",
                                    "PPI_D2_add_0.500000_6.txt","PPI_D2_add_0.500000_7.txt","PPI_D2_add_0.500000_8.txt","PPI_D2_add_0.500000_9.txt",
                                    "PPI_D2_add_0.600000_0.txt","PPI_D2_add_0.600000_1.txt","PPI_D2_add_0.600000_2.txt",
                                    "PPI_D2_add_0.600000_3.txt","PPI_D2_add_0.600000_4.txt","PPI_D2_add_0.600000_5.txt",
                                    "PPI_D2_add_0.600000_6.txt","PPI_D2_add_0.600000_7.txt","PPI_D2_add_0.600000_8.txt","PPI_D2_add_0.600000_9.txt",
                                    "PPI_D2_remove_0.100000_0.txt","PPI_D2_remove_0.100000_1.txt","PPI_D2_remove_0.100000_2.txt",
                                    "PPI_D2_remove_0.100000_3.txt","PPI_D2_remove_0.100000_4.txt","PPI_D2_remove_0.100000_5.txt","PPI_D2_remove_0.100000_6.txt",
                                    "PPI_D2_remove_0.100000_7.txt","PPI_D2_remove_0.100000_8.txt","PPI_D2_remove_0.100000_9.txt",
                                    "PPI_D2_remove_0.200000_0.txt","PPI_D2_remove_0.200000_1.txt","PPI_D2_remove_0.200000_2.txt",
                                    "PPI_D2_remove_0.200000_3.txt","PPI_D2_remove_0.200000_4.txt","PPI_D2_remove_0.200000_5.txt",
                                    "PPI_D2_remove_0.200000_6.txt","PPI_D2_remove_0.200000_7.txt","PPI_D2_remove_0.200000_8.txt","PPI_D2_remove_0.200000_9.txt",
                                    "PPI_D2_remove_0.300000_0.txt","PPI_D2_remove_0.300000_1.txt","PPI_D2_remove_0.300000_2.txt",
                                    "PPI_D2_remove_0.300000_3.txt","PPI_D2_remove_0.300000_4.txt","PPI_D2_remove_0.300000_5.txt",
                                    "PPI_D2_remove_0.300000_6.txt","PPI_D2_remove_0.300000_7.txt","PPI_D2_remove_0.300000_8.txt","PPI_D2_remove_0.300000_9.txt",
                                    "PPI_D2_remove_0.400000_0.txt","PPI_D2_remove_0.400000_1.txt","PPI_D2_remove_0.400000_2.txt",
                                    "PPI_D2_remove_0.400000_3.txt","PPI_D2_remove_0.400000_4.txt","PPI_D2_remove_0.400000_5.txt",
                                    "PPI_D2_remove_0.400000_6.txt","PPI_D2_remove_0.400000_7.txt","PPI_D2_remove_0.400000_8.txt","PPI_D2_remove_0.400000_9.txt",
                                    "PPI_D2_remove_0.500000_0.txt","PPI_D2_remove_0.500000_1.txt","PPI_D2_remove_0.500000_2.txt",
                                    "PPI_D2_remove_0.500000_3.txt","PPI_D2_remove_0.500000_4.txt","PPI_D2_remove_0.500000_5.txt",
                                    "PPI_D2_remove_0.500000_6.txt","PPI_D2_remove_0.500000_7.txt","PPI_D2_remove_0.500000_8.txt","PPI_D2_remove_0.500000_9.txt",
                                    "PPI_D2_remove_0.600000_0.txt","PPI_D2_remove_0.600000_1.txt","PPI_D2_remove_0.600000_2.txt",
                                    "PPI_D2_remove_0.600000_3.txt","PPI_D2_remove_0.600000_4.txt","PPI_D2_remove_0.600000_5.txt",
                                    "PPI_D2_remove_0.600000_6.txt","PPI_D2_remove_0.600000_7.txt","PPI_D2_remove_0.600000_8.txt","PPI_D2_remove_0.600000_9.txt",
                                    "Y2H_union.txt","Y2H_union_add_0.100000_0.txt","Y2H_union_add_0.100000_1.txt","Y2H_union_add_0.100000_2.txt",
                                    "Y2H_union_add_0.100000_3.txt","Y2H_union_add_0.100000_4.txt","Y2H_union_add_0.100000_5.txt","Y2H_union_add_0.100000_6.txt",
                                    "Y2H_union_add_0.100000_7.txt","Y2H_union_add_0.100000_8.txt","Y2H_union_add_0.100000_9.txt",
                                    "Y2H_union_add_0.200000_0.txt","Y2H_union_add_0.200000_1.txt","Y2H_union_add_0.200000_2.txt",
                                    "Y2H_union_add_0.200000_3.txt","Y2H_union_add_0.200000_4.txt","Y2H_union_add_0.200000_5.txt",
                                    "Y2H_union_add_0.200000_6.txt","Y2H_union_add_0.200000_7.txt","Y2H_union_add_0.200000_8.txt","Y2H_union_add_0.200000_9.txt",
                                    "Y2H_union_add_0.300000_0.txt","Y2H_union_add_0.300000_1.txt","Y2H_union_add_0.300000_2.txt",
                                    "Y2H_union_add_0.300000_3.txt","Y2H_union_add_0.300000_4.txt","Y2H_union_add_0.300000_5.txt",
                                    "Y2H_union_add_0.300000_6.txt","Y2H_union_add_0.300000_7.txt","Y2H_union_add_0.300000_8.txt","Y2H_union_add_0.300000_9.txt",
                                    "Y2H_union_add_0.400000_0.txt","Y2H_union_add_0.400000_1.txt","Y2H_union_add_0.400000_2.txt",
                                    "Y2H_union_add_0.400000_3.txt","Y2H_union_add_0.400000_4.txt","Y2H_union_add_0.400000_5.txt",
                                    "Y2H_union_add_0.400000_6.txt","Y2H_union_add_0.400000_7.txt","Y2H_union_add_0.400000_8.txt","Y2H_union_add_0.400000_9.txt",
                                    "Y2H_union_add_0.500000_0.txt","Y2H_union_add_0.500000_1.txt","Y2H_union_add_0.500000_2.txt",
                                    "Y2H_union_add_0.500000_3.txt","Y2H_union_add_0.500000_4.txt","Y2H_union_add_0.500000_5.txt",
                                    "Y2H_union_add_0.500000_6.txt","Y2H_union_add_0.500000_7.txt","Y2H_union_add_0.500000_8.txt","Y2H_union_add_0.500000_9.txt",
                                    "Y2H_union_add_0.600000_0.txt","Y2H_union_add_0.600000_1.txt","Y2H_union_add_0.600000_2.txt",
                                    "Y2H_union_add_0.600000_3.txt","Y2H_union_add_0.600000_4.txt","Y2H_union_add_0.600000_5.txt",
                                    "Y2H_union_add_0.600000_6.txt","Y2H_union_add_0.600000_7.txt","Y2H_union_add_0.600000_8.txt","Y2H_union_add_0.600000_9.txt",
                                    "Y2H_union_remove_0.100000_0.txt","Y2H_union_remove_0.100000_1.txt","Y2H_union_remove_0.100000_2.txt",
                                    "Y2H_union_remove_0.100000_3.txt","Y2H_union_remove_0.100000_4.txt","Y2H_union_remove_0.100000_5.txt","Y2H_union_remove_0.100000_6.txt",
                                    "Y2H_union_remove_0.100000_7.txt","Y2H_union_remove_0.100000_8.txt","Y2H_union_remove_0.100000_9.txt",
                                    "Y2H_union_remove_0.200000_0.txt","Y2H_union_remove_0.200000_1.txt","Y2H_union_remove_0.200000_2.txt",
                                    "Y2H_union_remove_0.200000_3.txt","Y2H_union_remove_0.200000_4.txt","Y2H_union_remove_0.200000_5.txt",
                                    "Y2H_union_remove_0.200000_6.txt","Y2H_union_remove_0.200000_7.txt","Y2H_union_remove_0.200000_8.txt","Y2H_union_remove_0.200000_9.txt",
                                    "Y2H_union_remove_0.300000_0.txt","Y2H_union_remove_0.300000_1.txt","Y2H_union_remove_0.300000_2.txt",
                                    "Y2H_union_remove_0.300000_3.txt","Y2H_union_remove_0.300000_4.txt","Y2H_union_remove_0.300000_5.txt",
                                    "Y2H_union_remove_0.300000_6.txt","Y2H_union_remove_0.300000_7.txt","Y2H_union_remove_0.300000_8.txt","Y2H_union_remove_0.300000_9.txt",
                                    "Y2H_union_remove_0.400000_0.txt","Y2H_union_remove_0.400000_1.txt","Y2H_union_remove_0.400000_2.txt",
                                    "Y2H_union_remove_0.400000_3.txt","Y2H_union_remove_0.400000_4.txt","Y2H_union_remove_0.400000_5.txt",
                                    "Y2H_union_remove_0.400000_6.txt","Y2H_union_remove_0.400000_7.txt","Y2H_union_remove_0.400000_8.txt","Y2H_union_remove_0.400000_9.txt",
                                    "Y2H_union_remove_0.500000_0.txt","Y2H_union_remove_0.500000_1.txt","Y2H_union_remove_0.500000_2.txt",
                                    "Y2H_union_remove_0.500000_3.txt","Y2H_union_remove_0.500000_4.txt","Y2H_union_remove_0.500000_5.txt",
                                    "Y2H_union_remove_0.500000_6.txt","Y2H_union_remove_0.500000_7.txt","Y2H_union_remove_0.500000_8.txt","Y2H_union_remove_0.500000_9.txt",
                                    "Y2H_union_remove_0.600000_0.txt","Y2H_union_remove_0.600000_1.txt","Y2H_union_remove_0.600000_2.txt",
                                    "Y2H_union_remove_0.600000_3.txt","Y2H_union_remove_0.600000_4.txt","Y2H_union_remove_0.600000_5.txt",
                                    "Y2H_union_remove_0.600000_6.txt","Y2H_union_remove_0.600000_7.txt","Y2H_union_remove_0.600000_8.txt","Y2H_union_remove_0.600000_9.txt"};

    string TrueCommunityFileName1[363] = {"Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt","Cmplx-D1.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt","Cmplx-D2.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt",
                                            "mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt","mips_3_100.txt"};

    double threshold = 0.6;
    double delta;
    string resultfilename = "result.txt";




//string inputFileName1[11] = {"facebook.txt","WikiVote.txt","CAHepTh.txt","anybeat.txt","blogs.txt","blogs2.txt","PGP,txt","protein-protein.txt","word_association.txt","p2p-09.txt"};
//string inputFileName1[1] = {"amazon-0302.txt"};
//double threshold;
//string resultfilename = "result.txt";

    string result_recall = "result_recall.txt";
    string result_precision = "result_precision.txt";
    string result_F1 = "result_F1.txt";
    ofstream resultrecall, resultprecision, resultF1;
    resultrecall.open(result_recall.c_str());
    resultprecision.open(result_precision.c_str());
    resultF1.open(result_F1.c_str());
*/


/*
string s11 = "PPI_D1.txt", s12 = "Cmplx-D1.txt";
string s21 = "PPI_D2.txt", s22 = "Cmplx-D2.txt";
string s31 = "Y2H-union.txt", s32 = "mips_3_100.txt";
Graph *graph1 = new Graph(),*graph2 = new Graph(),*graph3 = new Graph();
graph1->loading(s11);graph2->loading(s21);graph3->loading(s31);

cout<<"111"<<endl;

vector<vector<int> > t1 = file2TrueCommunity2(s12,graph1), t2 = file2TrueCommunity2(s22,graph2), t3 = file2TrueCommunity2(s32,graph3);
ofstream o1,o2,o3;
o1.open(s12.c_str());o2.open(s22.c_str());o3.open(s32.c_str());

cout<<"222"<<endl;

for (int m = 0; m < t1.size(); m++)
{
    vector<int> v = t1[m];
    for(int n = 0; n < v.size(); n++)
        o1<<v[n]<<" ";
    o1<<endl;
}

for (int m = 0; m < t2.size(); m++)
{
    vector<int> v = t2[m];
    for(int n = 0; n < v.size(); n++)
        o2<<v[n]<<" ";
    o2<<endl;
}

for (int m = 0; m < t3.size(); m++)
{
    vector<int> v = t3[m];
    for(int n = 0; n < v.size(); n++)
        o3<<v[n]<<" ";
    o3<<endl;
}
*/



    //vector<vector<double> > abc1(39),abc2(39),abc3(39);
    //string inputFileName,TrueCommunityFileName;
    //int tte = 0,numx=0;
    //for (int k = 1; k < 10; k++)
    //{
        //double sum1,sum2,sum3 = 0.0;

        //for (int times = 0; times < 363; times++)
        //{
           //delta = k * 0.1;

            //string inputFileName = inputFileName1[times];
            //string TrueCommunityFileName = TrueCommunityFileName1[times];



            Graph *graph=new Graph();


            clock_t start,finish;
            double totaltime;
            start = clock();

            cout<<"loading graph..."<<endl;
            graph->loading(inputFileName);
            cout<<"graph.vertex:"<<graph->vcount<<endl;
            cout<<"graph.edges:"<<graph->ecount<<endl;

/*
            Graph * grapha = new Graph();
            if (times < 121)
            {
                string aa = "PPI-D1.txt";
                grapha->loading(aa);
            }
            else if (times < 242)
            {
                string aa = "PPI-D2.txt";
                grapha->loading(aa);
            }
            else if (times < 363)
            {
                string aa = "Y2H-union.txt";
                grapha->loading(aa);
            }
*/

            vector<vector<int> > myCommunity = labelpropagation(graph,threshold);

            //output result community;
            printCom(myCommunity, graph, resultfilename);

            finish = clock();
            totaltime = (double)(finish-start)/CLOCKS_PER_SEC;
            cout<<"程序运行总时间为："<<totaltime<<endl;

            //compute the overlap modularity
            //beginComputeModularity(myCommunity,graph,resultQov,resultaverageon,resultaverageom,resultmaxom);

            //计算NMI
            //beginComputeNMI(TrueCommunityFileName,myCommunity,graph,outNMI,outoverlap);


            /*
            //计算Recall、Precision、F-measure
            pair<double,pair<double,double> > f1 = beginComputeFMeasure(myCommunity,inputFileName,TrueCommunityFileName,graph,delta);

            pair<double,double> p = f1.second;

            cout<<"recall:"<<p.first<<" precision:"<<p.second<<" F1:"<<f1.first<<endl;
            */

/*
            if (times == 0)
            {
                abc1[0].push_back(p.first);
                abc2[0].push_back(p.second);
                abc3[0].push_back(f1.first);
            }
            else if (times > 0 && times < 121)
            {
                sum1 = sum1 + p.first;
                sum2 = sum2 + p.second;
                sum3 = sum3 + f1.first;

                if (times%10 == 0)
                {
                    abc1[times/10].push_back(sum1/10);
                    abc2[times/10].push_back(sum2/10);
                    abc3[times/10].push_back(sum3/10);

                    sum1 = 0.0;sum2 = 0.0; sum3 = 0.0;
                }
            }
            else if (times == 121)
            {
                abc1[13].push_back(p.first);
                abc2[13].push_back(p.second);
                abc3[13].push_back(f1.first);

                numx = 13;
            }
            else if (times > 121 && times < 242)
            {
                tte++;
                sum1 = sum1 + p.first;
                sum2 = sum2 + p.second;
                sum3 = sum3 + f1.first;

                if (tte%10 == 0)
                {
                    abc1[numx+1].push_back(sum1/10);
                    abc2[numx+1].push_back(sum2/10);
                    abc3[++numx].push_back(sum3/10);

                    sum1 = 0.0;sum2 = 0.0; sum3 = 0.0;tte = 0;
                }
            }
            else if (times == 242)
            {
                abc1[numx+1].push_back(p.first);
                abc2[numx+1].push_back(p.second);
                abc3[++numx].push_back(f1.first);
            }
            else if (times > 242 && times < 363)
            {
                tte++;
                sum1 = sum1 + p.first;
                sum2 = sum2 + p.second;
                sum3 = sum3 + f1.first;

                if (tte%10 == 0)
                {
                    abc1[numx+1].push_back(sum1/10);
                    abc2[numx+1].push_back(sum2/10);
                    abc3[++numx].push_back(sum3/10);

                    sum1 = 0.0;sum2 = 0.0; sum3 = 0.0;tte = 0;
                }
            }
*/


//    vector<vector<int> >TrueCommunity = file2TrueCommunity2(TrueCommunityFileName, graph);
//    cout<<"TrueCommunity.size()："<<TrueCommunity.size()<<endl;

            //生成pajek可以读取的.net和.clu文件
            //pajek(inputFileName,myCommunity,graph);


        //}



/*
            string out1 = inputFileName;
            ofstream output1;
            output1.open(out1.c_str());


            for (int tm = 0; tm < graph->Linklist.size(); tm++)
            {
                pair<int,int> p = graph->Linklist[tm];

                if (p.first == p.second)
                {
                    cout<<"自循环错误!"<<endl;
                    return -1;
                }



                output1<<grapha->nodename_to_index[graph->Nodenames[p.first]]<<'\t'<<grapha->nodename_to_index[graph->Nodenames[p.second]]<<endl;
                output1<<grapha->nodename_to_index[graph->Nodenames[p.second]]<<'\t'<<grapha->nodename_to_index[graph->Nodenames[p.first]]<<endl;
            }
*/


    //}

/*
        for (int mm = 0; mm < abc1.size(); mm++)
        {
            for (int nn = 0; nn < 9; nn++)
            {
                resultrecall<<setiosflags(ios::fixed)<<setprecision(4)<<abc1[mm][nn]<<" ";
                resultprecision<<setiosflags(ios::fixed)<<setprecision(4)<<abc2[mm][nn]<<" ";
                resultF1<<setiosflags(ios::fixed)<<setprecision(4)<<abc3[mm][nn]<<" ";
            }
            resultrecall<<endl;
            resultprecision<<endl;
            resultF1<<endl;
        }


        resultrecall.close();
        resultprecision.close();
        resultF1.close();
*/

}

void printCom(vector<vector<int> > myCommunity,Graph * graph, string resultfilename)
{
    //Output
    ofstream myfile;
    myfile.open(resultfilename.c_str());

    int x = 1;

    for (vector<vector<int> >::const_iterator it = myCommunity.begin(); it != myCommunity.end(); it++)
    {

        vector<int> v = *it;

        myfile<<"第"<<x<<"个社团:( "<<v.size()<<" )";

        for (vector<int>::const_iterator i = v.begin(); i != v.end(); i++)
        {
            myfile <<graph->Nodenames[*i]<<" ";
        }

        x = x + 1;

        myfile <<endl<<endl;;
    }
}
