#include<iostream>
#include "WeightGraph.h"
using namespace std;

Graph::Graph()
{

}

Graph::~Graph()
{

}

void Graph::loading(string &inputFileName)
{
    pair<pair<string,string>,double> parseLine(string &inputFileName);

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
            pair<pair<string,string>,double> p = parseLine(line);
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
    vcount=Nodenames.size();
    ecount=links.size();

    Neighbors.resize(vcount);

    map<pair<pair<size_t,size_t>,double> > Linkcopy;
    //construct the Linklist & Neighbors
    {
        size_t index1,index2,t;
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
    for (map<pair<pair<size_t,size_t>,double> >::const_iterator it=Linkcopy.begin(); it!=Linkcopy.end(); it++)
    {
        Linklist.push_back(*it);
    }

    nodeTolinks();
}

pair<pair<string,string>,double> parseLine(string &lineorig)
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

size_t Graph::find_offset(string nodename)
{
    for (vector<string>::iterator it=Nodenames.begin(); it!=Nodenames.end(); it++)
    {
        if (*it==nodename)
            return size_t(it-Nodenames.begin());
    }
    return -1;
}

size_t Graph::node_degree(size_t node)
{
    return Neighbors[node].size();
}

void Graph::nodeTolinks()
{
    node_links.resize(vcount);

    for (size_t x=0;x<ecount;x++)
    {
        pair<size_t,size_t> p=Linklist[x];
        node_links[p.first].insert(x);
        node_links[p.second].insert(x);
    }

}
