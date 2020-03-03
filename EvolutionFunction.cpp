#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<math.h>
#include<numeric>
#include "EvolutionFunction.h"

using namespace std;


typedef pair<int, int> pa;
int desorder(const pa &x, const pa &y)
{
    //return x.second > y.second;
    return x.second > y.second;
}


void beginComputeModularity(vector<vector<int> >myCommunity, Graph *graph,ofstream &resultQov, ofstream & resultaverageon, ofstream & resultaverageom, ofstream & resultmaxom)
{
    double CalculateQov(vector<vector<int> >myCommunity, Graph *graph);

    cout<<"正在计算Qov的值...";
    cout<<"myCommunity.size():"<<myCommunity.size()<<endl;
    double Qov=CalculateQov(myCommunity,graph);
    cout<<Qov<<endl;



    //下面计算算法LLPA检测出来的社团的ond和omd
    	map<int,vector<int> > mm;
    	int ond =0, omd = 0, omd1 = 0, maxomd = 0;
    	for (int i = 0; i < myCommunity.size(); i++)
    	{
    	    for (int l = 0; l < myCommunity[i].size(); l++)
            {
                int node = myCommunity[i][l];

                mm[node].push_back(i);
            }

    	}

        vector<pair<int,int> > pair_vec;

    	for (map<int,vector<int> >::const_iterator mm_it = mm.begin(); mm_it != mm.end(); mm_it++)
        {
            vector<int> v = mm_it->second;
            if (v.size() > 1)
            {
                ond = ond + 1;
                omd = omd + v.size();
                if (v.size() > maxomd)
                    maxomd = v.size();

                pair_vec.push_back(make_pair(mm_it->first,v.size()));
            }

        }

        //将pair_vec中的内容按mm_it->second值进行降序排序
        sort(pair_vec.begin(),pair_vec.end(),cmp);

        cout<<endl<<"ond:"<<ond<<endl;
        cout<<"ond/N="<<(double)ond/graph->vcount<<" omd="<<(double)omd/ond<<" max omd="<<maxomd<<endl;


        resultQov<<"Qov"<<endl;
        resultaverageon<<(double)ond/graph->vcount<<endl;
        resultaverageom<<(double)omd/ond<<endl;
        resultmaxom<<maxomd<<endl;

/*
        cout<<endl<<"输出Top 10 om vertexName"<<endl;
        for (vector<pair<int,int> >::const_iterator vt = pair_vec.begin(); vt < pair_vec.begin()+10; vt++)
        {
            pair<int,int> p = *vt;

            cout<<"("<<graph->Nodenames[p.first]<<","<<p.second<<") ";

        }

        cout<<endl<<"写入所有重叠点到文件中..."<<endl;
        resultaverageom<<"vertexName      overlap_membership"<<endl;
        for (vector<pair<int,int> >::const_iterator vt = pair_vec.begin(); vt != pair_vec.end(); vt++)
        {
            pair<int,int> p = *vt;

            resultaverageom<<graph->Nodenames[p.first]<<","<<p.second<<"(";

            vector<int> v = mm[p.first];

            for (vector<int>::iterator vit = v.begin(); vit != v.end(); vit++)
            {
                resultaverageom<<*vit+1<<" ";
            }
            resultaverageom<<")"<<endl;

        }
*/


}

void beginComputeNMI(string outputFileName,vector<vector<int> > Community,Graph*graph,ofstream &outNMI, ofstream & outoverlap)
{
    //declaring functions
    vector<vector<int> > file2TrueCommunity(const string & outputFileName);
    double CalculateNMI(vector<vector<int> > TrueCommunity , vector<vector<int> > myCommunity, Graph *graph);

    	//Calculate NMI
    	cout<<endl<<"Calculating NMI..."<<endl;
    	double NMI1,NMI2,NMI;
    	vector <vector <int> > TrueCommunity;
        TrueCommunity = file2TrueCommunity(outputFileName);

        vector <vector<int> > myCommunity;
        for (unsigned int i = 0 ; i < Community.size() ; i++)
        {
            vector <int> v = Community[i];
            vector <int> com;
            for (unsigned int j = 0 ; j < v.size() ; j++)
            {
                com.push_back(atoi(graph->Nodenames[v[j]].c_str())); //if string
                //com.push_back(theGlobalgraph.node_index_to_name[v[j]]); if int
            }
            sort(com.begin(),com.end());
            myCommunity.push_back(com);
        }
        cout<<"myCommunity.size():"<<myCommunity.size()<<endl;
        cout<<"TrueCommunity.size():"<<TrueCommunity.size()<<endl;

    	NMI1 = CalculateNMI(TrueCommunity , myCommunity, graph);
    	NMI2 = CalculateNMI(myCommunity , TrueCommunity, graph);
    	NMI =1-(NMI1+NMI2)/2;
    	cout<<"NMI："<<NMI<<endl;

    	outNMI<<NMI<<endl;



    	//下面计算算法LLPA检测出来的社团的ond和omd
    	map<int,vector<int> > mm;
    	int ond =0, omd = 0, omd1 = 0, maxomd = 0;
    	for (int i = 0; i < myCommunity.size(); i++)
    	{
    	    for (int l = 0; l < myCommunity[i].size(); l++)
            {
                int node = myCommunity[i][l];

                mm[node].push_back(i);
            }

    	}

    	for (map<int,vector<int> >::const_iterator mm_it = mm.begin(); mm_it != mm.end(); mm_it++)
        {
            vector<int> v = mm_it->second;
            if (v.size() > 1)
            {
                ond = ond + 1;
                omd = omd + v.size();

                if (v.size() > maxomd)
                    maxomd = v.size();
            }

        }

        cout<<endl<<"ond:"<<ond<<endl;
        cout<<"ond/N="<<(double)ond/graph->vcount<<" average omd="<<(double)omd/ond<<" max omd="<<maxomd<<endl;

        outoverlap<<(double)ond/graph->vcount<<'\t'<<(double)omd/ond<<'\t'<<maxomd<<endl;

}


double CalculateQov(vector<vector<int> > myCommunity, Graph * graph)
{
    //function declaring
    double beta(map<int,set<int> >belongcoefficient, int node1, int node2, int c);
    double beta2(map<int,set<int> >belongcoefficient, int node1, int c);

    double sum=0.0;

    //First, compute the Oi
    set<int> nodeSet;
    map<int,set<int> > belongcoefficient;


//    cout<<"查看myCommunity的值："<<endl;
//    for (int i=0; i<myCommunity.size(); i++)
//    {
//        for (int j=0; j<myCommunity[i].size(); j++)
//            cout<<myCommunity[i][j]<<" ";
//        cout<<endl;
//    }

//    cout<<endl<<"myCommunity.size():"<<myCommunity.size()<<endl;

    for (vector<vector<int> >::const_iterator i=myCommunity.begin(); i!=myCommunity.end(); i++)
    {
        vector<int> com=*i;
        for (vector<int>::const_iterator it=com.begin(); it!=com.end(); it++)
        {
            nodeSet.insert(*it);
            belongcoefficient[*it].insert(i-myCommunity.begin());

        }
    }

    vector<vector<double> > singlecoefficient,averagecoefficient;
    for (int i=0; i<myCommunity.size(); i++)
    {
//        vector<double> v;
        vector<int> com=myCommunity.at(i);

        vector<double> f,avera;
        for (vector<int>::const_iterator it_com = com.begin(); it_com != com.end(); it_com++)
        {
            int comnode = *it_com;
            int comnode_index = int(it_com-com.begin());
            double tt = 1.0/belongcoefficient[comnode].size();
            double tem = 1.0/(1+exp(30-60*tt));

            f.push_back(tem);
        }

//        for (int j=0;j<com.size();j++)
//        {
//            vector<double> v1;
//
//            int node1=com[j];
//            for (int k=j;k<com.size();k++)
//            {
//                int node2=com[k];
////                v1.push_back(beta(belongcoefficient,node1,node2,i));
//                v1.push_back(F[j]*F[k]);
//            }
//            v.push_back(v1);
//
//        }
//        beta1.push_back(v);


        for (int j = 0; j < com.size(); j++)
        {
            int node1 = com[j];
            double sum1 = 0.0;
            for (int k = 0; k < com.size(); k++)
            {
                int node2 = com[k];
                sum1 = sum1 + f[j]*f[k];
            }
            avera.push_back(sum1/graph->vcount);
        }


        singlecoefficient.push_back(f);
        averagecoefficient.push_back(avera);
    }


    for (int i=0; i<myCommunity.size(); i++)

    {
//        if (i%10==0)
//            cout<<"正在计算第"<<i<<"个社团"<<endl;



        vector<double> v=singlecoefficient.at(i);//com这个社团内部每对节点对之间构成的coefficient of belonging;

        vector<double> average_cb = averagecoefficient.at(i);

        vector<int> com=myCommunity.at(i);

//        vector<double> average_cb;double sum1=0.0,beta_average;
//        //计算com中每个节点的平均coefficients of belonging to c
//        for (vector<int>::const_iterator com_it = com.begin(); com_it != com.end(); com_it++)
//        {
//            int index_node = int(com_it-com.begin());
//            int com_node = *com_it;
//
//            for (int x=0;x<index_node;x++)
//                sum1+=v[x][index_node-x];
//            for (int x=index_node;x<v[index_node].size();x++)
//                sum1+=v[index_node][x-index_node];
//            beta_average=sum1/theGlobalgraph.vcount();
//
//            average_cb.push_back(beta_average);
//
//            sum1 = 0.0;
//        }


//        cout<<"v的长度为："<<v.size()<<endl;


        for (vector<int>::const_iterator it1=com.begin(); it1!=com.end(); it1++)
        {
            int node1=*it1;
            int index1=int(it1-com.begin());

            for (vector<int>::const_iterator it2=com.begin(); it2!=com.end(); it2++)
            {
                int node2=*it2;
                int index2=int(it2-com.begin());
                //double beta1=beta(belongcoefficient,node1,node2,i-myCommunity.begin());

                double beta_1;
                beta_1 = v[index1]*v[index2];
//                if (index1<=index2)
//                    beta_1=v[index1][index2-index1];
//                else
//                    beta_1=v[index2][index1-index2];

//                double beta_2,sum1=0.0;
//                for (int x=0;x<index1;x++)
//                    sum1+=v[x][index1-x];
//                for (int x=index1;x<v[index1].size();x++)
//                    sum1+=v[index1][x-index1];
//                beta_2=sum1/theGlobalgraph.vcount();
//
//
//                double beta_3,sum2=0.0;
//                for (int x=0;x<index2;x++)
//                    sum2+=v[x][index2-x];
//                for (int x=index2;x<v[index2].size();x++)
//                    sum2+=v[index2][x-index2];
//                beta_3=sum2/theGlobalgraph.vcount();


                double beta_2 = average_cb[index1],beta_3 = average_cb[index2];

                double adj=0.0;
                if (find(graph->Neighbors[node1].begin(),graph->Neighbors[node1].end(),node2)!=graph->Neighbors[node1].end())
                    adj=1.0;

                sum=sum+beta_1*adj-((beta_2*beta_3*graph->node_degree(node1)*graph->node_degree(node2))/(2*graph->ecount));

            }
        }
    }
    return sum/(2*graph->ecount);

}


double beta(map<int,set<int> >belongcoefficient, int node1, int node2, int c)
{
    //f(x)=60x-30
//    double x,f1,f2;
    double x1,x2,f1,f2,F;

//    for (set<int>::const_iterator i=belongcoefficient[node1].begin();i!=belongcoefficient[node1].end();i++)
//    {
//        cout<<*i<<" ";
//    }
//    cout<<endl;

    if (belongcoefficient[node1].find(c) == belongcoefficient[node1].end() ||
        belongcoefficient[node2].find(c) == belongcoefficient[node2].end())
        return 0.0;
    else
    {
        x1 = 1.0/belongcoefficient[node1].size();
        x2 = 1.0/belongcoefficient[node2].size();
    }
    f1 = 60 * x1 - 30; f2 = 60 * x2 - 30;

    F = 1.0/((1+exp(0-f1))*(1+exp(0-f2)));

    return F;

//    f1=60*x-30;
//    f1=0-f1;
//
//    if (belongcoefficient[node2].find(c)==belongcoefficient[node2].end())
//        x=0.0;
//    else
//        x=1.0/belongcoefficient[node2].size();
//    f2=60*x-30;
//    f2=0-f2;
//
//    double f=(1+exp(f1))*(1+exp(f2));
//
//    return 1.0/f;
}

//double beta2(map<int, set<int> >belongcoefficient, int node2, int c)
//{
//    double sum=0.0;
//    for (int i=0; i<theGlobalgraph.vcount(); i++)
//    {
//        double f=beta(belongcoefficient,node2,i,c);
//        sum+=f;
//    }
//    return sum/theGlobalgraph.vcount();
//}

double CalculateNMI(vector<vector<int> > TrueCommunity , vector<vector<int> > myCommunity , Graph * graph)
{
    double nmi;
    int vertex_num=graph->vcount;

    //下面开始计算NMI值
    int myCommunity_num = myCommunity.size();
    int TrueCommunity_num = TrueCommunity.size();

    //vector <vector <double> > Com11(myCommunity_num),Com10(myCommunity_num),Com01(myCommunity_num),Com00(myCommunity_num);
    vector <vector <double> > P11(myCommunity_num),P10(myCommunity_num),P01(myCommunity_num),P00(myCommunity_num);
    vector <int> intersect;
    int intersect_num;
    for(int i = 0 ; i < myCommunity_num ; i++)
    {
        vector <int> my = myCommunity[i];
        for(int j = 0 ; j < TrueCommunity_num ; j++)
        {
            vector <int> True = TrueCommunity[j];
            set_intersection(my.begin(),my.end(),
                             True.begin(),True.end(),back_inserter(intersect));


            intersect_num = intersect.size();

            /*
            Com11[i].push_back(intersect_num);
            Com10[i].push_back(my.size()-intersect_num);
            Com01[i].push_back(True.size()-intersect_num);
            Com00[i].push_back(vertex_num-Com01[i][j]-Com10[i][j]-Com11[i][j]);

            P11[i].push_back(Com11[i][j]/vertex_num);
            P10[i].push_back(Com10[i][j]/vertex_num);
            P01[i].push_back(Com01[i][j]/vertex_num);
            P00[i].push_back(Com00[i][j]/vertex_num);
            */

            P11[i].push_back((double)intersect_num/vertex_num);
            P10[i].push_back((double)(my.size()-intersect_num)/vertex_num);
            P01[i].push_back((double)(True.size()-intersect_num)/vertex_num);
            P00[i].push_back((double)(vertex_num-my.size()-True.size()+intersect_num)/vertex_num);


            intersect.clear();
        }
    }

    vector <double> min_A;
    double Hk_l,h11,h10,h01,h00,Hkl;
    double templ=0;
    for (int i = 0 ; i < myCommunity_num ; i++)
    {
        double min_k = 1000;
        Hk_l = 0;
        for (int j = 0 ; j < TrueCommunity_num ; j++)
        {

            Hkl = 0; h11 = 0; h10 = 0; h01 = 0; h00 = 0;
            if (P11[i][j] != 0)
            {
                h11 = -P11[i][j] * (log(P11[i][j])/log(2));
                Hkl = Hkl + h11;
            }
            if (P10[i][j] != 0)
            {
                h10 = -P10[i][j] * (log(P10[i][j])/log(2));
                Hkl = Hkl + h10;
            }
            if (P01[i][j] != 0)
            {
                h01 = -P01[i][j] * (log(P01[i][j])/log(2));
                Hkl = Hkl + h01;
            }
            if (P00[i][j] != 0)
            {
                h00 = -P00[i][j] * (log(P00[i][j])/log(2));
                Hkl = Hkl + h00;
            }

            double Py1 = P11[i][j] + P01[i][j];
            double Py0 = P10[i][j] + P00[i][j];
            double Hl = -Py1 * (log(Py1)/log(2)) + (-Py0 * (log(Py0)/log(2)));

            Hk_l = Hkl - Hl;

            if(Hk_l <= min_k && (h00 + h11 > h01 + h10))
            {
                min_k = Hk_l;
                templ = j;
            }
        }

        if (min_k == 1000)
        {
            min_k = 1;
        }
        else
        {
            double Px1 = P11[i][templ] + P10[i][templ];
            double Px0 = P01[i][templ] + P00[i][templ];
            double Hk = -Px1 * (log(Px1)/log(2)) + (-Px0 * (log(Px0)/log(2)));
            min_k = min_k/Hk;
        }
        min_A.push_back(min_k);
    }

    nmi = accumulate(min_A.begin(),min_A.end(),0.0)/myCommunity_num;

    return nmi;
}


vector <vector <int> > file2TrueCommunity(const string & TrueCommunity_filename)
{
    vector <vector<int> *> TrueCommunity;
    int community_num_max = -1;

    /*
    vector <int> example;
    vector <string> names;
    for (int i = 0 ; i < vertex_num ; i++)
    {
        example.push_back(i);
        names.push_back(ordered_node_names[i]);
    }*/

    //按行读文件
	ifstream read(TrueCommunity_filename.c_str() , ios_base :: in | ios_base :: binary);
    if(read.fail())
	{
		cout<<"读取"<<TrueCommunity_filename<<"失败，请确保该文件正确!"<<endl;
		exit(0);
	}

	string line;
	while(getline(read ,line ))
	{
	    if (!line.empty())
        {
            int splitnum = 0;//记录line中有多少个空格

            //每一行的最后一个是空格，应该把空格也去掉
            if(!line.empty() && line.at(line.length()-2) == ' ')
            {
                line.erase(line.length()-2,1);
            }

            //每一行的line可能是以'\n'结尾的，所以要把line中'\n'去掉
            if(!line.empty() && *line.rend() == '\n')
            {
                line.erase(line.length()-1,1);
            }

            //将字符串line按照中间的空格分隔开
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

            /*
            //找到节点名称为nodename所对应的节点编号
            vector <string> :: const_iterator name_iterator;
            int name_index;
            name_iterator = find(names.begin(),names.end(),nodename);
            name_index = int(name_iterator-names.begin());
            node_num = example[name_index];
            //再把example和names向量中的name_index位置值删除
            example.erase(example.begin()+name_index);
            names.erase(names.begin()+name_index);
            */


            node_num = atoi(nodename.c_str());

            for (int i = 0 ; i < splitnum ; i++)
            {
                string communitynum;
                getline(fields , communitynum);

                //把communitynum转化成数字
                int community_num = atoi(communitynum.c_str());
                if (community_num != 0)
                {
                    if (community_num > community_num_max)
                    {
                        community_num_max = community_num;
                        TrueCommunity.resize(community_num_max,NULL);
                    }

                    if(TrueCommunity[community_num-1] == NULL)
                    {
                        TrueCommunity[community_num-1] = new vector<int>;
                    }

                    TrueCommunity[community_num-1]->push_back(node_num);
                }
            }
        }
    }
    vector <vector <int> > True;

    for (unsigned int i=0;i<TrueCommunity.size();i++)
    {
        vector<int> * TrueCommunityBegin = TrueCommunity[i];
        vector <int> a;
        for (unsigned int j = 0 ; j<TrueCommunityBegin->size(); j++)
        {
            a.push_back(TrueCommunityBegin->at(j));
        }
        sort(a.begin(),a.end());
        True.push_back(a);
    }

    return True;
}

string removeline(string &lineorig)
{
    int i;

    for (i = lineorig.size() - 1; i >= 0; i--)
    {
        //if (lineorig[i] >= '1' && lineorig[i] <= '9' || lineorig[i] >= 'A' && lineorig[i] <= 'Z')
        if (lineorig[i] >= '1' && lineorig[i] <= '9')
            break;
    }

    string line = lineorig.substr(0,i+1);

    return line;
}


vector<vector<int> >file2TrueCommunity2(string &TrueCommunity_filename, Graph *graph)
{

    cout<<endl<<"file2TrueCommunity2函数进来了"<<endl;


    vector<vector<int> > TrueCommunity;

    //按行读文件
	ifstream read(TrueCommunity_filename.c_str() , ios_base :: in | ios_base :: binary);
    if(read.fail())
	{
		cout<<"读取"<<TrueCommunity_filename<<"失败，请确保该文件正确!"<<endl;
		exit(0);
	}

	string line;
	while(getline(read ,line ))
	{
	    if (!line.empty())
        {
            int splitnum = 0;//记录line中有多少个空格

            //每一行的最后一个是空格，应该把空格也去掉
            if(!line.empty() && line.at(line.length()-2) == ' '|| line.at(line.length()-2) == '\t')
            {
                line.erase(line.length()-2,1);
            }

            //去掉line末尾的所有空格
            line = removeline(line);

            //将字符串line按照中间的空格分隔开
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

            //getline(fields , nodename);

            /*
            //找到节点名称为nodename所对应的节点编号
            vector <string> :: const_iterator name_iterator;
            int name_index;
            name_iterator = find(names.begin(),names.end(),nodename);
            name_index = int(name_iterator-names.begin());
            node_num = example[name_index];
            //再把example和names向量中的name_index位置值删除
            example.erase(example.begin()+name_index);
            names.erase(names.begin()+name_index);
            */


            vector<int> v;

            for (int i = 0 ; i < splitnum + 1 ; i++)
            {
                getline(fields , nodename);

                if (find(graph->Nodenames.begin(),graph->Nodenames.end(),nodename) == graph->Nodenames.end())
                    v.push_back(-1);
                else
                    v.push_back(graph->nodename_to_index[nodename]);

            }

            sort(v.begin(),v.end());
            TrueCommunity.push_back(v);

        }
	}

    return TrueCommunity;
}

const int maxn=1000000;
bool arr[maxn+1]={false};
vector<int> produce_prim_number()
{
        vector<int> prim;
        prim.push_back(2);
        int i,j;
        for(i=3;i*i<=maxn;i+=2)
        {
                if(!arr[i])
                {
                        prim.push_back(i);
                        for(j=i*i;j<=maxn;j+=i)
                                arr[j]=true;
                }
        }
        while(i<maxn)
        {
                if(!arr[i])
                        prim.push_back(i);
                i+=2;
        }
        return prim;
}
//计算n!中素数因子p的指数
int cal(int x,int p)
{
        int ans=0;
        long long rec=p;
        while(x>=rec)
        {
                ans+=x/rec;
                rec*=p;
        }
        return ans;
}
//计算n的k次方对m取模，二分法
int pow(long long n,int k,int M)
{
        long long ans=1;
        while(k)
        {
                if(k&1)
                {
                        ans=(ans*n)%M;
                }
                n=(n*n)%M;
                k>>=1;
        }
        return ans;
}
//计算C（n，m）
int func(int n,int m)
{
        const int M=10007;
        vector<int> prim=produce_prim_number();
        long long ans=1;
        int num;
        for(int i=0;i<prim.size()&&prim[i]<=n;++i)
        {
                num=cal(n,prim[i])-cal(m,prim[i])-cal(n-m,prim[i]);
                ans=(ans*pow(prim[i],num,M))%M;
        }
        return ans;
}

pair<double,pair<double,double> > beginComputeFMeasure(vector<vector<int> >myCommunity,string &inputFileName,string &TrueCommunityFileName,Graph *graph, double alpha)
{
    vector<vector<int> >TrueCommunity = file2TrueCommunity2(TrueCommunityFileName, graph);
/*
    cout<<"myCommunity.size():"<<myCommunity.size()<<" TrueCommunity.size():"<<TrueCommunity.size()<<endl;
    for (int i = 0; i < myCommunity.size(); i++)
    {
        for (int j = 0; j < myCommunity[i].size(); j++)
            cout<<myCommunity[i][j]<<" ";
        cout<<endl;
    }
    for (int i = 0; i < TrueCommunity.size(); i++)
    {
        for (int j = 0; j < TrueCommunity[i].size(); j++)
            cout<<TrueCommunity[i][j]<<" ";
        cout<<endl;
    }
    char c=getchar();
    c=getchar();
*/

    double tp = 0.0, fn = 0.0, fp;

    for (int i = 0; i < myCommunity.size(); i++)
    {
        int match = 0;
        vector<int> my = myCommunity[i];
        for (int j = 0; j < TrueCommunity.size(); j++)
        {
            vector<int> truec = TrueCommunity[j];

            vector<int> intersection;
            set_intersection(my.begin(),my.end(),truec.begin(),truec.end(),back_inserter(intersection));

            double d = (double)(intersection.size()*intersection.size());

            if (d/(my.size()*truec.size()) > 0.8)
            {
                cout<<"i:"<<i+1<<"  j:"<<j+1<<"  比例:"<<d/(my.size()*truec.size())<<endl;
            }

            if (d/(my.size()*truec.size()) > alpha)
            {
                match = 1;
                break;
            }

        }

        if (match == 1)
            tp = tp + 1;
    }


    for (int i = 0; i < TrueCommunity.size(); i++)
    {
        int match = 0;
        vector<int> truec = TrueCommunity[i];
        for (int j = 0; j < myCommunity.size(); j++)
        {
            vector<int> my = myCommunity[j];

            vector<int> intersection;
            set_intersection(my.begin(),my.end(),truec.begin(),truec.end(),back_inserter(intersection));

            double d = (double)(intersection.size()*intersection.size());
            if (d/(my.size()*truec.size()) > alpha)
            {
                match = 1;
                break;
            }

        }

        if (match == 0)
            fn = fn + 1;
    }

    fp = myCommunity.size() - tp;


//    //p-value
//    double tp = 0.0, fn = 0.0, fp;
//
//    for (int i = 0; i < myCommunity.size(); i++)
//    {
//        int match = 0;
//        vector<int> my = myCommunity[i];
//
//        double pvalue = 1.0;
//        for (int j = 0; j < TrueCommunity.size(); j++)
//        {
//            vector<int> truec = TrueCommunity[j];
//
//            vector<int> intersection;
//            set_intersection(my.begin(),my.end(),truec.begin(),truec.end(),back_inserter(intersection));
//
//            for (int h = 0; h < intersection.size(); h++)
//            {
//                pvalue = pvalue - func(truec.size(),h)*func(graph->vcount-truec.size(),my.size()-h)*func(graph->vcount,my.size());
//            }
//            if (pvalue < 0.05)
//            {
//                match = 1;
//                break;
//            }
//
//        }
//
//        if (match == 1)
//            tp = tp + 1;
//    }
//
//
//    for (int i = 0; i < TrueCommunity.size(); i++)
//    {
//        int match = 0;
//        vector<int> truec = TrueCommunity[i];
//
//        double pvalue = 1.0;
//        for (int j = 0; j < myCommunity.size(); j++)
//        {
//            vector<int> my = myCommunity[j];
//
//            vector<int> intersection;
//            set_intersection(my.begin(),my.end(),truec.begin(),truec.end(),back_inserter(intersection));
//
//            for (int h = 0; h < intersection.size(); h++)
//            {
//                pvalue = pvalue - func(truec.size(),h)*func(graph->vcount-truec.size(),my.size()-h)*func(graph->vcount,my.size());
//            }
//            if (pvalue < 0.05)
//            {
//                match = 1;
//                break;
//            }
//
//        }
//
//        if (match == 0)
//            fn = fn + 1;
//    }
//
//    fp = myCommunity.size() - tp;
//
    cout<<"tp:"<<tp<<" fn:"<<fn<<" fp:"<<fp<<endl;




    double recall = (double)(tp)/(tp + fn);
    if (tp + fn == 0)
        recall = 0.0;

    double precision = (double)(tp)/(tp + fp);
    if (tp + fp == 0)
        precision = 0.0;

    double f1 = (double)(2*recall*precision)/(recall + precision);
    if (recall + precision == 0)
        f1 = 0.0;

    pair<double,pair<double,double> > p;

    pair<double,double> pp;
    pp.first = recall;
    pp.second = precision;
    p.first = f1;
    p.second = pp;

    cout<<endl<<"recall:"<<recall<<endl;
    cout<<"precision:"<<precision<<endl;
    cout<<"f1:"<<f1<<endl;



    set<int> coverage;
    map<int,int> mapsizetonum1;
    double average_size = 0;
    for (int i = 0; i < myCommunity.size(); i++)
    {
        mapsizetonum1[myCommunity[i].size()]++;
//        if (mapsizetonum1.find(myCommunity[i].size()/10) == mapsizetonum1.end())
//            mapsizetonum1[myCommunity[i].size()/10] = 1;
//        else
//            mapsizetonum1[myCommunity[i].size()/10]++;

        average_size += myCommunity[i].size();
        for (int j = 0; j < myCommunity[i].size(); j++)
        {
            coverage.insert(myCommunity[i][j]);
        }

    }

    map<int,int> truemapsizetonum1;
    for (int i = 0; i < TrueCommunity.size(); i++)
    {
        truemapsizetonum1[TrueCommunity[i].size()]++;

    }

//    map<int,double> mapsizetonum;
//    for (map<int,int>::iterator mit = mapsizetonum1.begin(); mit != mapsizetonum1.end(); mit++)
//    {
//        int nums = mit->second;
//        mapsizetonum[mit->first] = (double)nums/myCommunity.size();
//    }


    map<int,set<int> > mapmyoverlap;
    for (int i = 0; i < myCommunity.size(); i++)
    {
        for (int j = 0; j < myCommunity[i].size(); j++)
        {
            mapmyoverlap[myCommunity[i][j]].insert(i);
        }
    }
    vector<pair<int,int> > myoverlap,trueoverlap;
    for (map<int,set<int> >::const_iterator it = mapmyoverlap.begin(); it != mapmyoverlap.end(); it++)
    {
        if (it->first != -1 && it->second.size() > 1)
        {
            myoverlap.push_back(make_pair(it->first,it->second.size()));
        }

    }

    sort(myoverlap.begin(),myoverlap.end(),desorder);

    map<int,set<int> > mapoverlap;
    for (int i = 0; i < TrueCommunity.size(); i++)
    {
        for (int j = 0; j < TrueCommunity[i].size(); j++)
        {
            mapoverlap[TrueCommunity[i][j]].insert(i);
        }
    }

    for (map<int,set<int> >::const_iterator it = mapoverlap.begin(); it != mapoverlap.end(); it++)
    {
        if (it->first != -1 && it->second.size() > 1)
        {
            trueoverlap.push_back(make_pair(it->first,it->second.size()));
        }

    }

    sort(trueoverlap.begin(),trueoverlap.end(),desorder);



    /*统计predicted和truecommunity信息*/
    char res[100];
    string result_information = "result_information_";
    sprintf(res,"%s%s",result_information.c_str(),inputFileName.c_str());
    ofstream resultinformation;
    resultinformation.open(res);
    resultinformation<<"算法: LLPA"<<endl;
    resultinformation<<"graph protein number:"<<graph->vcount<<"  graph interactions number:"<<graph->ecount<<endl;
    resultinformation<<"predicted clusters number:"<<myCommunity.size()<<"  true clusters number:"<<TrueCommunity.size()<<endl;
    resultinformation<<"average cluster size:"<<average_size/myCommunity.size()<<endl;
    resultinformation<<"coverage:"<<coverage.size()<<endl;

    resultinformation<<endl<<endl<<"-----------------------cluster size distributions-------------------------"<<endl;
    for (map<int,int>::const_iterator it = mapsizetonum1.begin(); it != mapsizetonum1.end(); it++)
    {
        //if (it->first != 0)
            resultinformation<<it->first<<" "<<it->second<<endl;
    }
    resultinformation<<endl<<endl<<"-----------------------cluster size distributions-------------------------"<<endl;

    resultinformation<<endl<<endl<<"-----------------------true cluster size distributions-------------------------"<<endl;
    for (map<int,int>::const_iterator it = truemapsizetonum1.begin(); it != truemapsizetonum1.end(); it++)
    {
        //if (it->first != 0)
            resultinformation<<it->first<<" "<<it->second<<endl;
    }
    resultinformation<<endl<<endl<<"-----------------------true cluster size distributions-------------------------"<<endl;

    resultinformation<<endl<<endl<<"-----------------------overlap information-----------------------"<<endl;
    resultinformation<<"predicted clusters overlap number:"<<myoverlap.size()<<"  true clusters overlap number:"<<trueoverlap.size()<<endl;
    resultinformation<<"predicted overlapped proteins:"<<endl;
    for (vector<pair<int,int> >::const_iterator it = myoverlap.begin(); it != myoverlap.end(); it++)
    {
        pair<int,int> p = *it;
        resultinformation<<graph->Nodenames[p.first]<<" ("<<p.second<<")"<<endl;
    }
    resultinformation<<"true overlapped proteins:"<<endl;
    for (vector<pair<int,int> >::const_iterator it = trueoverlap.begin(); it != trueoverlap.end(); it++)
    {
        pair<int,int> p = *it;
        resultinformation<<graph->Nodenames[p.first]<<" ("<<p.second<<")"<<endl;
    }
    resultinformation<<endl<<"-----------------------overlap information-----------------------"<<endl;

    return p;

}



