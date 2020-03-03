#include<iostream>
#include "WeightGraph.h"
using namespace std;

weightGraph::weightGraph()
{

}

weightGraph::~weightGraph()
{

}

void weightGraph::loading(string &inputFileName)
{
    pair<pair<string,string>,double> parseWeightLine(string &inputFileName);

    std::ifstream myfile(inputFileName.c_str());
    std::string line;

    set<pair<pair<string,string>,double> > links;
    set<string> Nodenamescopy;
    string t;
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            getline(myfile, line);
            pair<pair<string,string>,double> p = parseWeightLine(line);
            Nodenamescopy.insert(p.first.first);Nodenamescopy.insert(p.first.second);

            if (p.first.first>p.first.second)
            {
                t=p.first.first;
                p.first.first=p.first.second;
                p.first.second=t;
            }
            links.insert(p);
        }
        myfile.close();
    }

    //copy the Nodenamescopy to the Nodenames
    for (set<string>::const_iterator it=Nodenamescopy.begin(); it!=Nodenamescopy.end(); it++)
    {
        Nodenames.push_back(*it);
    }

    //Get the number of nodes and links
    totalnodes=Nodenames.size();
    totallinks=links.size();

    Neighbors.resize(totalnodes);

    map<pair<int,int>,double> Linkcopy;
    //construct the Linklist & Neighbors
    {
        int index1,index2,t;
        for (set<pair<pair<string,string>,double> >::const_iterator linkit=links.begin(); linkit!=links.end(); linkit++)
        {
            pair<pair<string,string>,double> p=*linkit;
            index1=find_offset(p.first.first);index2=find_offset(p.first.second);
            if (index1>index2)
            {
                t=index1;
                index1=index2;
                index2=t;
            }
            //Linkcopy.insert(make_pair(make_pair(index1,index2),p.second);
            Linkcopy[make_pair(index1,index2)]=p.second;
            Neighbors[index1].push_back(index2);
            Neighbors[index2].push_back(index1);
        }
    }

    //copy Linkcopy to the Linklist
    for (map<pair<int,int>,double>::const_iterator it=Linkcopy.begin(); it!=Linkcopy.end(); it++)
    {
        //Linklist.push_back(*it);

        pair<int,int> p = it->first;
        Linklist[p]=it->second;
    }

    nodeTolinks();
}

pair<pair<string,string>,double> parseWeightLine(string &lineorig)
{
	string line(lineorig);

	for(string :: iterator i = line.begin(); i!=line.end(); i++)
	{
		if(*i == ',' || *i == ' ' || *i == '\t' || *i == '|')
			*i='\n';
	}

	istringstream fields(line);

	string sourceNodeName;
	string targetNodeName;
	string weight;

	getline(fields, sourceNodeName);

	if(fields.fail())
	{
		cout<<"读取边文件时，异常!"<<endl;
		exit(0);
	}
	getline(fields, targetNodeName);
	if(fields.fail())
	{
		cout<<"读取边文件时，异常!"<<endl;
		exit(0);
	}
	getline(fields, weight);
	if (fields.fail())
    {
        cout<<"读取边文件时，异常！"<<endl;
        exit(0);
    }

	pair<pair<string,string>,double> p;
	p.first.first=sourceNodeName;
	p.first.second=targetNodeName;
    p.second=atof(weight.c_str());
	return p;
}

int weightGraph::find_offset(string nodename)
{
    for (vector<string>::iterator it=Nodenames.begin(); it!=Nodenames.end(); it++)
    {
        if (*it==nodename)
            return int(it-Nodenames.begin());
    }
    return -1;
}

int weightGraph::node_degree(int node)
{
    return Neighbors[node].size();
}

void weightGraph::nodeTolinks()
{
    node_links.resize(totalnodes);

//    for (int x=0;x<ecount;x++)
//    {
//        pair<int,int> p=Linklist[x];
//        node_links[p.first].insert(x);
//        node_links[p.second].insert(x);
//    }
    int linkindex=0;
    for (map<pair<int,int>,double>::const_iterator mapit=Linklist.begin();mapit!=Linklist.end();mapit++)
    {
        pair<int,int> p = mapit->first;
        node_links[p.first].insert(linkindex);
        node_links[p.second].insert(linkindex++);
    }

}

