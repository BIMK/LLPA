#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

#include "loadingGraph.h"
//#include "pajek.h"

map<int,set<int> > m;//m�д��ÿ���ڵ��������ŵı��
int overlapnodeNum = 0;

map<int,set<int> > m_LLPA,m_OCG;

set<int> overlap_LLPA, overlap_OCG;

typedef pair<int, int> PAIR1;
int cmpp(const PAIR1 &x, const PAIR1 &y)
{
    return x.second > y.second;
}

typedef pair<pair<int, int>, double> pp;
int desorder(const pp &x, const pp &y)
{
    //return x.second > y.second;
    pair<int,int> p11 = x.first;
    double p12 = x.second;

    pair<int,int> p21 = y.first;
    double p22 = y.second;

    if (p11.second != p21.second)
        return p11.second > p21.second;
    else
        return p12 > p22;
}


void CompareToOCG(string inputFileName, vector<vector<int> > myCommunity, Graph * graph)
{
    double overlapdegree(vector<vector<int> > myCommunity, Graph * graph, set<int> &overlap);
    map<int,double> nodebetweenness(vector<vector<int> > myCommunity, Graph * graph);

    cout<<endl<<endl<<"Ϊ�˺�OCG�㷨���жԱȣ����ڼ����й�ָ��..."<<endl;


    //************************************************LLPA***************************************************


    //ָ��1�������ص���Ķ���
    double odSum = overlapdegree(myCommunity, graph, overlap_LLPA);

//    cout<<endl<<"odSum: "<<odSum<<"  odAverage: "<<odSum/overlapnodeNum<<endl;


    //ָ��2����������ÿ���ڵ�Ľ���
    map<int,double> m_betweenness = nodebetweenness(myCommunity, graph);

/*
    double betSum = 0.0;

    for (map<int,set<int> >::iterator it = m.begin(); it != m.end(); it++)
    {
        set<int> s = it->second;
        if (s.size() > 1)
        {
            int overlapnode = it->first;
            betSum = betSum + m_betweenness[overlapnode];
        }

    }

    cout<<endl<<"overlapBetweennessSum: "<<betSum<<" overlapBetweennessAverage: "<<betSum/overlapnodeNum<<endl;
*/

    //************************************************LLPA***************************************************


    m_LLPA = m;
    m.clear();overlapnodeNum = 0;
    //*************************************************OCG***************************************************

    cout<<endl<<endl<<"Ϊ�˺�LLPA�㷨���жԱȣ����ڼ����й�ָ��..."<<endl;

    vector<vector<int> > myCommunity_OCG;

    std::ifstream resultfile("result_OCG.txt");
    if(resultfile.fail())
	{
		cout<<"��ȡresult_OCG.txtʧ�ܣ���ȷ�����ļ���ȷ!"<<endl;
		exit(0);
	}

	string line;
	while(getline(resultfile ,line ))
	{
	    vector<int>  com;
	    if (!line.empty())
        {
            int splitnum = 0;//��¼line���ж��ٸ��ո�

            //ÿһ�е����һ���ǿո�Ӧ�ðѿո�Ҳȥ��
            if(!line.empty() && line.at(line.length()-2) == ' ')
            {
                line.erase(line.length()-2,1);
            }

            //ÿһ�е�line��������'\n'��β�ģ�����Ҫ��line��'\n'ȥ��
            if(!line.empty() && *line.rend() == '\n')
            {
                line.erase(line.length()-1,1);
            }

            //���ַ���line�����м�Ŀո�ָ���
            for(string :: iterator i = line.begin(); i!=line.end(); i++)
            {
                if(*i == ',' || *i == ' ' || *i == '\t' || *i == '|')
                {
                    *i = '\n';
                    splitnum = splitnum + 1;
                }
            }

            istringstream fields(line);
            string nodename;
            int node_num = 0;

            getline(fields , nodename);

            //node_num = theGlobalgraph.node_name_to_index[nodename];
            //com.push_back(node_num);
            vector<string>::iterator nodeit = find(graph->Nodenames.begin(),graph->Nodenames.end(),nodename);
            if (nodeit != graph->Nodenames.end())
            {
                node_num = (int)(nodeit - graph->Nodenames.begin());
                com.push_back(node_num);
            }

            for (int i = 0 ; i < splitnum ; i++)
            {
                getline(fields , nodename);

                //node_num = theGlobalgraph.node_name_to_index[nodename];
                //com.push_back(node_num);
                vector<string>::iterator nodeit1 = find(graph->Nodenames.begin(),graph->Nodenames.end(),nodename);
                if (nodeit1 != graph->Nodenames.end())
                {
                    node_num = (int)(nodeit1 - graph->Nodenames.begin());
                    com.push_back(node_num);
                }
            }

            myCommunity_OCG.push_back(com);
        }
	}


	vector<int> cs_llpa,cs_ocg;
	for (int i = 0; i < myCommunity.size(); i++)
        cs_llpa.push_back(myCommunity[i].size());
    for (int j = 0; j < myCommunity_OCG.size(); j++)
        cs_ocg.push_back(myCommunity_OCG[j].size());

    sort(cs_llpa.begin(),cs_llpa.end());
    sort(cs_ocg.begin(),cs_ocg.end());

    cout<<endl<<endl<<"LLPA:"<<endl;
    for (int i = cs_llpa.size()-1; i >= 0; i--)
        cout<<cs_llpa[i]<<" ";
    cout<<endl<<endl<<"OCG:"<<endl;
    for (int j = cs_ocg.size()-1; j >= 0; j--)
        cout<<cs_ocg[j]<<" ";
    cout<<endl;

    string result_size1 = "result_communitysize_LLPA.txt",result_size2="result_communitysize_OCG.txt";
    ofstream result_communitysize1,result_communitysize2;
    result_communitysize1.open(result_size1.c_str());
    result_communitysize2.open(result_size2.c_str());
    for (int i = cs_llpa.size()-1; i >= 0; i--)
        result_communitysize1<<cs_llpa[i]<<endl;
    cout<<endl<<endl<<"OCG:"<<endl;
    for (int j = cs_ocg.size()-1; j >= 0; j--)
        result_communitysize2<<cs_ocg[j]<<endl;


    string result_contain = "result_contain.txt";
    ofstream resultcontain;
    resultcontain.open(result_contain.c_str());

    for (int i = 0; i < myCommunity.size(); i++)
    {
        vector<int> com = myCommunity[i];
        vector<vector<int> > result;
        vector<int> ocgcommunityindex;vector<vector<int> > ocgintersect;
        for (int j = 0; j < myCommunity_OCG.size(); j++)
        {
            vector<int> com2 = myCommunity_OCG[j];
            vector<int> cc;
            set_intersection(com.begin(),com.end(),com2.begin(),com2.end(),inserter(cc,cc.begin()));
            if ((double)cc.size()/com2.size() > 0.4)
            {
                ocgcommunityindex.push_back(j);
                result.push_back(com2);
                ocgintersect.push_back(cc);
            }
        }

        if (ocgcommunityindex.size() != 0)
        {
            resultcontain<<"**************************************************************************"<<endl<<endl;
            resultcontain<<"LLPA�ĵ�"<<i+1<<"�����ű�OCG��";
            for (int m =0; m < ocgcommunityindex.size()-1; m++)
            {
                resultcontain<<ocgcommunityindex[m]<<",";
            }
            resultcontain<<ocgcommunityindex[ocgcommunityindex.size()-1]<<"�ָ�"<<endl<<endl;

            resultcontain<<"LLPA:"<<endl<<"��"<<i+1<<"�����Ź���"<<myCommunity[i].size()<<"���ڵ�"<<endl<<endl<<"OCG:(OCG�����Ŵ�С   OCG��LLPA���Ž�����С)"<<endl;

            for (int m =0; m < ocgcommunityindex.size(); m++)
            {
                resultcontain<<ocgcommunityindex[m]<<": "<<myCommunity_OCG[ocgcommunityindex[m]].size()<<"   "<<ocgintersect[m].size()<<endl;
            }
            resultcontain<<endl<<"LLPA("<<i+1<<"): ";
            for (int m = 0; m < myCommunity[i].size(); m++)
                resultcontain<<graph->Nodenames[myCommunity[i][m]]<<" ";

            resultcontain<<endl<<endl;
            for (int m = 0; m < ocgcommunityindex.size(); m++)
            {
                resultcontain<<"OCG("<<ocgcommunityindex[m]<<"): ";
                for (int n = 0; n < myCommunity_OCG[ocgcommunityindex[m]].size(); n++)
                    resultcontain<<myCommunity_OCG[ocgcommunityindex[m]][n]<<" ";
                resultcontain<<endl;
            }

            resultcontain<<endl<<endl;

        }
    }



    //ָ��1�������ص���Ķ���
    double odSum_OCG = overlapdegree(myCommunity_OCG, graph, overlap_OCG);
    m_OCG = m;

//    cout<<endl<<"odSum_OCG: "<<odSum_OCG<<"  odAverage_OCG: "<<odSum_OCG/overlapnodeNum<<endl;


    //ָ��2����������ÿ���ڵ�Ľ���
    map<int,double> m_betweenness_OCG = nodebetweenness(myCommunity_OCG, graph);

/*
    double betSum_OCG = 0.0;

    for (map<int,set<int> >::iterator it = m.begin(); it != m.end(); it++)
    {
        set<int> s = it->second;
        if (s.size() > 1)
        {
            int overlapnode = it->first;
            betSum_OCG = betSum_OCG + m_betweenness_OCG[overlapnode];
        }

    }

    cout<<endl<<"overlapBetweennessSum_OCG: "<<betSum_OCG<<" overlapBetweennessAverage_OCG: "<<betSum_OCG/overlapnodeNum<<endl;
*/

    //*************************************************OCG***************************************************


    //���� overlap_LLPA + overlap_OCG + m_LLPA + m_OCG
    set<int> LLPA_OCG, OCG_LLPA, intersect, intersect_same, intersect_nsame;

    cout<<endl<<endl<<"overlap_LLPA.size():"<<overlap_LLPA.size()<<" overlap_OCG.size():"<<overlap_OCG.size();
    cout<<" m_LLPA.size():"<<m_LLPA.size()<<" m_OCG.size():"<<m_OCG.size()<<endl;

    set_difference(overlap_LLPA.begin(),overlap_LLPA.end(),overlap_OCG.begin(),overlap_OCG.end(),inserter(LLPA_OCG,LLPA_OCG.begin()));
    set_difference(overlap_OCG.begin(),overlap_OCG.end(),overlap_LLPA.begin(),overlap_LLPA.end(),inserter(OCG_LLPA,OCG_LLPA.begin()));
    set_intersection(overlap_LLPA.begin(),overlap_LLPA.end(),overlap_OCG.begin(),overlap_OCG.end(),inserter(intersect,intersect.begin()));

    for (set<int>::iterator ss = intersect.begin(); ss != intersect.end(); ss++)
    {
        set<int> s1 = m_LLPA[*ss]; set<int> s2 = m_OCG[*ss];

        if (s1.size() == s2.size())
            intersect_same.insert(*ss);
        else
            intersect_nsame.insert(*ss);
    }

    string result_LLPA_OCG1 = "result_LLPA_OCG.txt";
    ofstream result_LLPA_OCG;
    result_LLPA_OCG.open(result_LLPA_OCG1.c_str());

    result_LLPA_OCG<<"**********LLPA���ֵ��ص����OCGû�з��ֵ�("<<LLPA_OCG.size()<<"��, ��"<<overlap_LLPA.size()<<"��)**********"<<endl;
    result_LLPA_OCG<<"Nodenames  overlap_membership  nodeDegree  nodeBetweenness"<<endl;
    for (set<int>::iterator sit = LLPA_OCG.begin(); sit != LLPA_OCG.end(); sit++)
    {
        set<int> s = m_LLPA[*sit];
        result_LLPA_OCG<<graph->Nodenames[*sit]<<"   "<<s.size()<<"   "<<graph->node_degree(*sit)<<"   "<<m_betweenness[*sit]<<endl;
    }
    result_LLPA_OCG<<endl<<endl;

    result_LLPA_OCG<<"**********OCG���ֵ��ص����LLPAû�з��ֵ�("<<OCG_LLPA.size()<<"��, ��"<<overlap_OCG.size()<<"��)**********"<<endl;
    result_LLPA_OCG<<"Nodenames  overlap_membership  nodeDegree  nodeBetweenness"<<endl;
    for (set<int>::iterator sit = OCG_LLPA.begin(); sit != OCG_LLPA.end(); sit++)
    {
        set<int> s = m_OCG[*sit];
        result_LLPA_OCG<<graph->Nodenames[*sit]<<"   "<<s.size()<<"   "<<graph->node_degree(*sit)<<"   "<<m_betweenness[*sit]<<endl;
    }
    result_LLPA_OCG<<endl<<endl;

    result_LLPA_OCG<<"**********LLPA��OCG��ͬ���ֵ��ص���(�ص��̶Ȳ�һ��"<<intersect_nsame.size()<<"��, ��"<<intersect.size()<<"��)**********"<<endl;
    result_LLPA_OCG<<"Nodenames  overlap_membership_LLPA  overlap_membership_OCG  nodeDegree  nodeBetweenness"<<endl;
    for (set<int>::iterator sit = intersect_nsame.begin(); sit != intersect_nsame.end(); sit++)
    {
        set<int> s1 = m_LLPA[*sit]; set<int> s2 = m_OCG[*sit];
        result_LLPA_OCG<<graph->Nodenames[*sit]<<"   "<<s1.size()<<"   "<<s2.size()<<"   "<<graph->node_degree(*sit)<<"   "<<m_betweenness[*sit]<<endl;
    }
    result_LLPA_OCG<<endl<<endl;

    result_LLPA_OCG<<"**********LLPA��OCG��ͬ���ֵ��ص���(�ص��̶�һ��"<<intersect_same.size()<<"��, ��"<<intersect.size()<<"��)**********"<<endl;
    result_LLPA_OCG<<"Nodenames  overlap_membership  nodeDegree  nodeBetweenness"<<endl;
    for (set<int>::iterator sit = intersect_same.begin(); sit != intersect_same.end(); sit++)
    {
        set<int> s = m_OCG[*sit];
        result_LLPA_OCG<<graph->Nodenames[*sit]<<"   "<<s.size()<<"   "<<graph->node_degree(*sit)<<"   "<<m_betweenness[*sit]<<endl;
    }



    //�õ�LLPA��Top100��overlap nodes
    vector<pair<pair<int, int>,double > > TopOverlap;//pair<pair<nodesNum, overlapmembership>,betweenness>
    for (map<int,set<int> >::const_iterator mLLPA_it = m_LLPA.begin(); mLLPA_it != m_LLPA.end(); mLLPA_it++)
    {
        set<int> v = mLLPA_it->second;
        if (v.size() > 1)
        {
            TopOverlap.push_back(make_pair(make_pair(mLLPA_it->first,v.size()),m_betweenness[mLLPA_it->first]));
        }

    }
    sort(TopOverlap.begin(),TopOverlap.end(),desorder);

    cout<<endl<<"���Top 600 overlap_membership vertexName"<<endl;
    string result_Topoverlap1 = "result_Topoverlap.txt";
    ofstream result_Topoverlap;
    result_Topoverlap.open(result_Topoverlap1.c_str());

    int index = 1;


    set<int> similarity;

    int ssum = 0;
    set<int> top600;

    if (TopOverlap.size() >= 100)
        for (vector<pair<pair<int,int>, double> >::const_iterator vt = TopOverlap.begin(); vt < TopOverlap.begin()+600; vt++)
        {
            pair<pair<int,int>, double> p = *vt;
            pair<int,int> p11 = p.first;

            set<int> ss_LLPA,ss_OCG,ss_intersect;

            cout<<"("<<graph->Nodenames[p11.first]<<","<<p11.second<<") ";

            if (m_OCG[p11.first].size() > 1)
            {
                ssum = ssum + 1;
                top600.insert(p11.first);
            }


            result_Topoverlap<<"************************************************************************************************************"<<endl;
            result_Topoverlap<<index<<". "<<graph->Nodenames[p11.first]<<"( LLPA om = "<<p11.second<<" , OCG om = "<<m_OCG[p11.first].size()<<" )"<<endl<<endl;

            //LLPA
            result_Topoverlap<<"LLPA( ";
            set<int> s = m_LLPA[p11.first];
            for (set<int>::iterator sit = s.begin(); sit != s.end()--; sit++)
            {
                result_Topoverlap<<*sit+1<<", ";
            }

            result_Topoverlap<<" )"<<endl;

            for (set<int>::iterator sit = s.begin(); sit != s.end(); sit++)
            {
                result_Topoverlap<<"LLPA_"<<*sit + 1<<"( "<<myCommunity[*sit].size()<<" ���ڵ�): ";

                vector<int> vv = myCommunity[*sit];

                for (vector<int>::iterator vvit = vv.begin(); vvit != vv.end(); vvit++)
                {
                    result_Topoverlap<<graph->Nodenames[*vvit]<<" ";

                    ss_LLPA.insert(*vvit);
                }
                result_Topoverlap<<endl;

            }

            //OCG
            result_Topoverlap<<endl;
            result_Topoverlap<<"OCG( ";
            set<int> socg = m_OCG[p11.first];
            for (set<int>::iterator socgit = socg.begin(); socgit != socg.end(); socgit++)
            {
                result_Topoverlap<<*socgit+1<<", ";
            }
            result_Topoverlap<<" )"<<endl;

            for (set<int>::iterator socgit = socg.begin(); socgit != socg.end(); socgit++)
            {
                result_Topoverlap<<"OCG_"<<*socgit + 1<<"( "<<myCommunity_OCG[*socgit].size()<<" ���ڵ�): ";

                vector<int> vv = myCommunity_OCG[*socgit];

                for (vector<int>::iterator vvit = vv.begin(); vvit != vv.end(); vvit++)
                {
                    result_Topoverlap<<graph->Nodenames[*vvit]<<" ";

                    ss_OCG.insert(*vvit);
                }
                result_Topoverlap<<endl;

            }

            set_intersection(ss_LLPA.begin(),ss_LLPA.end(),ss_OCG.begin(),ss_OCG.end(),inserter(ss_intersect,ss_intersect.begin()));
            result_Topoverlap<<endl<<"LLPA��OCG�ҵ����ڽڵ�"<<graph->Nodenames[p11.first]<<"��Χ��������"<<ss_intersect.size()<<"����ͬ�Ľڵ�, ���Ƿֱ��ǣ�"<<endl;

            for (set<int>::iterator ssinit = ss_intersect.begin(); ssinit != ss_intersect.end(); ssinit++)
            {
                result_Topoverlap<<graph->Nodenames[*ssinit]<<" ";
            }
            result_Topoverlap<<endl;

            if ((double)ss_intersect.size()/min(ss_LLPA.size(),ss_OCG.size()) > 0.5)
                similarity.insert(p11.first);

            result_Topoverlap<<"************************************************************************************************************";

            result_Topoverlap<<endl<<endl;

            index++;

        }
    else
        for (vector<pair<pair<int,int>, double> >::const_iterator vt = TopOverlap.begin(); vt < TopOverlap.begin()+TopOverlap.size(); vt++)
        {
            pair<pair<int,int>, double> p = *vt;
            pair<int,int> p11 = p.first;

            cout<<"("<<graph->Nodenames[p11.first]<<","<<p11.second<<") ";

            result_Topoverlap<<index<<". "<<graph->Nodenames[p11.first]<<"( LLPA om = "<<p11.second<<" , OCG om = "<<m_OCG[p11.first].size()<<" )"<<endl;

            //LLPA
            result_Topoverlap<<"LLPA( ";
            set<int> s = m_LLPA[p11.first];
            for (set<int>::iterator sit = s.begin(); sit != s.end(); sit++)
            {
                result_Topoverlap<<*sit+1<<", ";
            }
            result_Topoverlap<<" )"<<endl;

            for (set<int>::iterator sit = s.begin(); sit != s.end(); sit++)
            {
                result_Topoverlap<<"LLPA_"<<*sit + 1<<"( "<<myCommunity[*sit].size()<<" ���ڵ�): ";

                vector<int> vv = myCommunity[*sit];

                for (vector<int>::iterator vvit = vv.begin(); vvit != vv.end(); vvit++)
                {
                    result_Topoverlap<<graph->Nodenames[*vvit]<<" ";
                }
                result_Topoverlap<<endl;

            }

            //OCG
            result_Topoverlap<<endl;
            result_Topoverlap<<"OCG( ";
            set<int> socg = m_OCG[p11.first];
            for (set<int>::iterator socgit = socg.begin(); socgit != socg.end(); socgit++)
            {
                result_Topoverlap<<*socgit+1<<", ";
            }
            result_Topoverlap<<" )"<<endl;

            for (set<int>::iterator socgit = socg.begin(); socgit != socg.end(); socgit++)
            {
                result_Topoverlap<<"OCG"<<*socgit + 1<<"( "<<myCommunity_OCG[*socgit].size()<<" ���ڵ�): ";

                vector<int> vv = myCommunity_OCG[*socgit];

                for (vector<int>::iterator vvit = vv.begin(); vvit != vv.end(); vvit++)
                {
                    result_Topoverlap<<graph->Nodenames[*vvit]<<" ";
                }
                result_Topoverlap<<endl;

            }

            result_Topoverlap<<endl<<endl;

            index++;

        }


        //���ͳ�ƽ��
        result_Topoverlap<<endl<<endl<<"---------------------------------------------------ͳ�ƽ��-------------------------------------------------"<<endl;

        result_Topoverlap<<endl<<"��LLPA�㷨Top100�У���"<<similarity.size()<<"���ڵ㣬���Ǻ�OCG�ҵ��Ľ���д���50%�������ԣ����Ƿֱ��ǣ�"<<endl;

        for (set<int>::iterator sim_it = similarity.begin(); sim_it != similarity.end(); sim_it++)
        {
            result_Topoverlap<<graph->Nodenames[*sim_it]<<" ";
        }

        result_Topoverlap<<endl<<"------------------------------------------------ͳ�ƽ��----------------------------------------------";


        result_Topoverlap<<endl<<endl<<endl<<"top600����"<<ssum<<"���ص����OCG�ҵ���Ҳ���ص���"<<endl;
        for(set<int>::iterator top600it = top600.begin(); top600it != top600.end(); top600it++)
            result_Topoverlap<<*top600it<<" ";
        result_Topoverlap<<endl;

        cout<<endl<<endl<<"Top 100���ڵ�д���ļ����!"<<endl;
        cout<<"Do you want to draw the LLPA or OCG algorithms results? "<<endl;
        string inputstring;
        cin>>inputstring;
        while ( strcmp(inputstring.c_str(),"#"))
        {
            if ( !strcmp(inputstring.c_str(),"LLPA"))
            {
                string inputFileName1 = inputFileName.substr(0,inputFileName.size()-4)+"_LLPA.txt";
                pajek(inputFileName1.c_str(),myCommunity,graph);
            }
            else if (!strcmp(inputstring.c_str(),"OCG"))
            {
                string inputFileName2 = inputFileName.substr(0,inputFileName.size()-4)+"_OCG.txt";
                pajek(inputFileName2.c_str(),myCommunity_OCG,graph);
            }
            cout<<endl<<"Do you want to draw the LLPA or OCG algorithms results? "<<endl;
            cin>>inputstring;
        }


}

double overlapdegree(vector<vector<int> > myCommunity, Graph * graph, set<int> &overlap)
{
    for (vector<vector<int> >::iterator vit = myCommunity.begin(); vit != myCommunity.end(); vit++)
    {
        vector <int> v = *vit;
        for (vector<int>::iterator vvit = v.begin(); vvit != v.end(); vvit++)
        {
            m[*vvit].insert(vit - myCommunity.begin());
        }
    }

    double odSum = 0.0;
    for (map<int,set<int> >::iterator mit = m.begin(); mit != m.end(); mit++)
    {
        set<int> v = mit->second;
        if (v.size()>1)
        {
            odSum = odSum + graph->node_degree(mit->first);

            overlapnodeNum = overlapnodeNum + 1;

            overlap.insert(mit->first);
        }
    }

    return odSum;
}

map<int,double> nodebetweenness(vector<vector<int> > myCommunity, Graph * graph)
{
    //��0��graph->vcount-1��ÿ���ڵ�ֱ���ΪԴ�ڵ�

    map<int,double> node_between;

    for (int source = 0; source < graph->vcount; source++)
    {
//        map<int,pair<int,int> > node_distance_shortestNum;

        //������ȱ�����������
        queue<int> q;
        map<int,int> distance, weight, passthrough;

        //��distance��weight��ʼ��
        for (int tt = 0; tt < graph->vcount; tt++)
        {
            distance[tt] = -1;
            weight[tt] = 0;
        }

        q.push(source);
        distance[source] = 0;
        weight[source] = 1;


        while (q.size() > 0)
        {
            int node = q.front();
            q.pop();

            //�ҵ�node������δ���ʹ����ھӣ�����q��
            vector<int> v = graph->Neighbors[node];

//            cout<<"node:"<<node<<" distance[node]:"<<distance[node]<<endl;

            for (vector<int>::iterator i = v.begin(); i != v.end(); i++)
            {
                int temp = *i;

//                cout<<"temp��"<<temp<<"  distance[temp]:"<<distance[temp]<<endl;

                if (distance[temp] < 0)
                {
//                    cout<<"111111"<<endl;
                    q.push(temp);
                    distance[temp] = distance[node] + 1;
                }

                if (distance[temp] == distance[node] + 1)
                {
//                    cout<<"222222"<<endl;
                    weight[temp] = weight[temp] + weight[node];
                }
            }

//            cout<<"whileѭ����Ĺ��̣�"<<endl;
//            for (map<int,int>::iterator t1 = distance.begin(); t1 != distance.end(); t1++)
//            {
//                cout<<"Դ�ڵ�"<<source<<"���ڵ�"<<t1->first<<"�ľ���Ϊ��"<<distance[t1->first]<<" , �ýڵ��ȨֵΪ��"<<weight[t1->first]<<endl;
//            }

        }


        //����distance��weight����vertice betweenness

        //���������������sourceΪԴ�ڵ�ʱ����ܵ����·����Ŀ
        double sumshortest = 0.0;
        for (map<int,int>::iterator mit = weight.begin(); mit != weight.end(); mit++)
        {
            sumshortest = sumshortest + mit->second;
        }
//        cout<<"sumshortest:"<<sumshortest<<endl;


        vector<pair<int,int> > dis;
        for (map<int,int>::iterator mite = distance.begin(); mite != distance.end(); mite++)
        {
            dis.push_back(make_pair(mite->first,mite->second));
        }

        sort(dis.begin(),dis.end(), cmpp);

        for (vector<pair<int,int> >::iterator vpit = dis.begin(); vpit != dis.end(); vpit++)
        {
            pair<int,int> pp = *vpit;
            int vertex = pp.first; int length = pp.second;
//            cout<<"vertex:"<<vertex<<"  length:"<<length;

            //�ҵ�vertex�������ھ��б�source��vertex���·����������Щ�ھ�
            vector<int> nei = graph->Neighbors[vertex];

            int path = 0;

            for (vector<int>::iterator ii = nei.begin(); ii != nei.end(); ii++)
            {
                if (distance[*ii] > length)
                    path = path + ((double)weight[vertex]/weight[*ii])*passthrough[*ii];

//                else if (distance[*ii] > length && weight[*ii] > weight[vertex])
//                    path = path + weight[vertex];
            }

            //path����Ҫ���Ͻڵ�vertex��������·����Ŀ
            path = path + weight[vertex];

//            cout<<"  weight[vertex]:"<<weight[vertex]<<"  path:"<<path<<endl;

            passthrough[vertex] = path;

            if (node_between.find(vertex) == node_between.end())
                node_between[vertex] = (double)path/sumshortest;
            else
            {
                node_between[vertex] = (double)path/sumshortest + node_between[vertex];
            }

        }


//        //���ÿ���ڵ����
//        cout<<endl<<"�Խڵ�"<<source<<"ΪԴ�ڵ㣬���ÿ���ڵ����"<<endl;
//        for (map<int,double>::iterator mit = node_between.begin(); mit != node_between.end(); mit++)
//        {
//            cout<<mit->first<<" "<<mit->second<<endl;
//        }
//        cout<<endl;
//
//
//        char c = getchar();
//        c = getchar();


    }

    string resultDegBet1="output_DegBet.txt";
    ofstream resultDegBet;
    resultDegBet.open(resultDegBet1.c_str());

    resultDegBet<<"NodesName  Degree  Betweenness"<<endl;

    //���ÿ���ڵ����
//    cout<<endl<<endl<<"���ÿ���ڵ����"<<endl;
    for (map<int,double>::iterator mit = node_between.begin(); mit != node_between.end(); mit++)
    {
//        cout<<graph->Nodenames[mit->first]<<" "<<mit->second<<endl;

        resultDegBet<<graph->Nodenames[mit->first]<<" "<<graph->node_degree(mit->first)<<" "<<mit->second<<endl;
    }
//    cout<<endl;
    resultDegBet<<endl;

    return node_between;
}
