#include<iostream>
#include "loadingGraph.h"
using namespace std;

Graph::Graph()
{

}

Graph::~Graph()
{

}

void Graph::loading(string &inputFileName)
{
    pair<string,string> parseLine(string &inputFileName);

    std::ifstream myfile(inputFileName.c_str());
    std::string line;

    set<pair<string,string> > links;
    set<string> Nodenamescopy;
    string t;
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            getline(myfile, line);
            if (!line.empty())
            {
                pair<string,string> p = parseLine(line);
                Nodenamescopy.insert(p.first);Nodenamescopy.insert(p.second);

                if (p.first>p.second)
                {
                    t=p.first;
                    p.first=p.second;
                    p.second=t;
                }
                links.insert(p);
            }

        }
        myfile.close();
    }

    //copy the Nodenamescopy to the Nodenames
    int num = 0;
    for (set<string>::const_iterator it=Nodenamescopy.begin(); it!=Nodenamescopy.end(); it++)
    {
        Nodenames.push_back(*it);
        nodename_to_index[(string)*it] = num++;
    }

    //Get the number of nodes and links
    vcount=Nodenames.size();
    ecount=links.size();

    Neighbors.resize(vcount);

    set<pair<int,int> > Linkcopy;
    //construct the Linklist & Neighbors
    {
        int index1,index2,t;
        for (set<pair<string,string> >::const_iterator linkit=links.begin(); linkit!=links.end(); linkit++)
        {
            pair<string,string> p=*linkit;
            index1=find_offset(p.first);index2=find_offset(p.second);
            if (index1>index2)
            {
                t=index1;
                index1=index2;
                index2=t;
            }
            Linkcopy.insert(make_pair(index1,index2));
            Neighbors[index1].push_back(index2);
            Neighbors[index2].push_back(index1);
        }
    }

    //copy Linkcopy to the Linklist
    for (set<pair<int,int> >::const_iterator it=Linkcopy.begin(); it!=Linkcopy.end(); it++)
    {
        Linklist.push_back(*it);
    }

    nodeTolinks();
}

pair<string,string> parseLine(string &lineorig)
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

	getline(fields, sourceNodeName);

	if(fields.fail())
	{
		//cout<<"读取边文件时，异常!"<<endl;
		//exit(0);

		cout<<"读取边文件，异常!"<<endl;
		exit(0);
	}
	getline(fields, targetNodeName);
	if(fields.fail())
	{
		//cout<<"读取边文件时，异常!"<<endl;
		//exit(0);

		cout<<"孤立点："<<sourceNodeName<<endl;
		targetNodeName = "";
	}

	pair<string,string> p;
	p.first=sourceNodeName;
	p.second=targetNodeName;

	return p;
}

int Graph::find_offset(string nodename)
{
    for (vector<string>::iterator it=Nodenames.begin(); it!=Nodenames.end(); it++)
    {
        if (*it==nodename)
            return int(it-Nodenames.begin());
    }
    return -1;
}

int Graph::node_degree(int node)
{
    return Neighbors[node].size();
}

void Graph::nodeTolinks()
{
    node_links.resize(vcount);

    for (int x=0;x<ecount;x++)
    {
        pair<int,int> p=Linklist[x];
        node_links[p.first].insert(x);
        node_links[p.second].insert(x);
    }

}
