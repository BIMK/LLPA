#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<cmath>
#include<map>
#include<assert.h>
#include<math.h>
#include "LabelPropagation.h"

#include<time.h>

using namespace std;

static int maxlabel;

vector<vector<int> > labelpropagation(Graph *graph,double threshold)
{
    int maxiteration = 20;

    //declaring functions
    vector<vector<pair<int,double> > > LinkSimilarity(Graph *graph);

    //vector<int> randlinksequence(vector<int> constant);
    vector<int> linksequence(vector<double> averagesimilarity);

    //double thresholdfunction(vector<double> averagesimilarity,vector<pair<int,double> > linksimi,int linknum);

    int labelfunction(vector<pair<int,double> > link_similarity, Graph *graph,vector<int> label,
                         vector<double> averagesimilarity, int linknum);

    int labelfunction1(vector<pair<int,double> > link_similarity, Graph *graph,vector<int> label,
                         vector<double> averagesimilarity, int linknum);

    bool StopCriterion(vector<int> labeloriginal,vector<int> label);

    vector<vector<int> > decode(vector<int> label,Graph *graph,double threshold);

    maxlabel=graph->ecount;

    cout<<"graph->ecount:"<<graph->ecount<<endl;

    //First, calculate the similarity between links

    clock_t start,finish;
    double time;
    start = clock();
    vector<vector<pair<int,double> > > linksimilarity = LinkSimilarity(graph);
    finish  = clock();
    time = (double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"计算边相似度所用时间为："<<time<<endl;

    //calculate the average similarity of each links
    start = clock();
    vector<double> averagesimilarity;
    double sum=0.0;
    for (vector<vector<pair<int,double> > >::const_iterator i=linksimilarity.begin();i!=linksimilarity.end();i++)
    {
        vector<pair<int,double> > v=*i;
        for (vector<pair<int,double> >::const_iterator j=v.begin();j!=v.end();j++)
        {
            pair<int,double> p=*j;
            sum=sum+p.second;
        }

        averagesimilarity.push_back(sum/v.size());
        sum=0.0;
    }
    finish = clock();
    time = (double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"计算平均相似度所用的时间为："<<time<<endl;

    cout<<endl<<"首先查看边的信息："<<endl;
    for (int x=0;x<graph->ecount;x++)
    {
        cout<<graph->Nodenames[graph->Linklist[x].first]<<" "<<graph->Nodenames[graph->Linklist[x].second]<<endl;
    }

    cout<<"linksimilarity结束，查看linksimilarity的值："<<endl;
    for (int x=0;x<linksimilarity.size();x++)
     {
         cout<<"边("<<graph->Nodenames[graph->Linklist[x].first]<<",";
         cout<<graph->Nodenames[graph->Linklist[x].second]<<"):";
         for(int y=0;y<linksimilarity[x].size();y++)
         {
             cout<<"("<<graph->Nodenames[graph->Linklist[linksimilarity[x][y].first].first]<<",";
             cout<<graph->Nodenames[graph->Linklist[linksimilarity[x][y].first].second]<<")_"<<linksimilarity[x][y].second<<" ";
             //cout<<linksimilarity[x][y].first<<"_"<<linksimilarity[x][y].second<<" ";
         }
         cout<<endl;
     }
////
    cout<<endl<<"查看averagesimilarity的值："<<endl;
    for (int x=0;x<averagesimilarity.size();x++)
    {
        cout<<"("<<graph->Nodenames[graph->Linklist[x].first]<<",";
        cout<<graph->Nodenames[graph->Linklist[x].second]<<")_"<<averagesimilarity[x]<<" ";
//        cout<<thresholdfunction(averagesimilarity,linksimilarity[x],x)<<"  ";
    }
    cout<<endl;



    //initialize: allocate each label to nodes;
    vector<int> label,labeloriginal;
    for (int i=0; i<graph->GetEcount(); i++)
    {
        label.push_back(i);
        labeloriginal.push_back(1);

        cout<<i<<"  "<<graph->Nodenames[graph->Linklist[i].first]<<"  "<<graph->Nodenames[graph->Linklist[i].second]<<endl;
    }

    //iterate k times label propagation;  this is the easiest stop criterion condition
    //here we define a stop criterion by ourself: the number of communities and the label in each community don't changed.
    int iteration=0;
    while (!StopCriterion(labeloriginal,label))
    {
        //statistics the number of iteration
        iteration += 1;
        if (iteration>maxiteration)
            break;

        //First of all, generate a rand link sequence;
        //vector<int> randlink=randlinksequence(constant);
        //desend based on averragesimilarity
        vector<int> sequence=linksequence(averagesimilarity);

        //Assignment the label of previous iteration to the labeloriginal;
        labeloriginal=label;

//        cout<<endl<<"查看sequence的值："<<endl;
//        for (int x=0;x<sequence.size();x++)
//            cout<<graph->Nodenames[graph->Linklist[sequence[x]].first]<<" "<<graph->Nodenames[graph->Linklist[sequence[x]].second]<<endl;
//        cout<<endl;

//        cout<<"第38边为："<<graph->Nodenames[graph->Linklist[38].first]<<" "<<graph->Nodenames[graph->Linklist[38].second]<<endl;

        for (vector<int>::const_iterator it=sequence.begin(); it!=sequence.end(); it++)
        {
            //linknum stand for the serial of the link
            int linknum=*it;

            cout<<"正在处理的边为："<<graph->Nodenames[graph->Linklist[linknum].first]<<" "<<graph->Nodenames[graph->Linklist[linknum].second]<<"   ";
//
//            cout<<"查看该边与其它邻接边的相似度："<<endl;
//            for(int y=0;y<linksimilarity[linknum].size();y++)
//            {
//                cout<<"("<<graph->Nodenames[graph->Linklist[linksimilarity[linknum][y].first].first]<<",";
//                cout<<graph->Nodenames[graph->Linklist[linksimilarity[linknum][y].first].second]<<")_"<<linksimilarity[linknum][y].second<<" ";
//            }
//            cout<<endl;

            //compute the number of neighbors of each label,and return the label which contain the max nodes with this label;
            int label_maxnodes= labelfunction(linksimilarity[linknum],graph,label,averagesimilarity,linknum);
//            int label_maxnodes= labelfunction1(linksimilarity[linknum],graph,label,averagesimilarity,linknum,alpha);

            cout<<"返回的标签为："<<label_maxnodes<<endl;
            //update the current link label
            label[linknum]=label_maxnodes;
        }
    }

//    //查看每条边的标签
//    for (int x=0;x<graph->ecount;x++)
//    {
//        cout<<"边("<<graph->Nodenames[graph->Linklist[x].first]<<","<<graph->Nodenames[graph->Linklist[x].second]<<"):"<<label[x]<<endl;
//    }

    //decode: with the same label, belong to one community;
    vector<vector<int> > myCommunity=decode(label,graph,threshold);

    cout<<endl<<"迭代了："<<iteration<<"次"<<endl<<endl;
    return myCommunity;

}

vector<vector<pair<int,double> > > LinkSimilarity(Graph *graph)
{
    //pair<int,double> represent the similarity value(double stype) between the current link and the number of int links;

    //declaring functions
    double setintersection(vector<int> v1,vector<int> v2);
    double setunion(vector<int> v1,vector<int> v2);

    vector<vector<pair<int,double> > > linksimilarity(graph->GetEcount());

    map<pair<int,int>,pair<double,double> > mapnodessimilarity;
    //mapnodessimilarity保存网络中任意一条边的两个节点的相似度
    for(vector<pair<int,int> >::const_iterator it = graph->Linklist.begin();it != graph->Linklist.end(); it++)
    {
        pair<int,int> link = *it;
        int node1 = link.first, node2 = link.second;

        //计算node1和node2的相似度，后面直接利用，避免重复计算
        double intervalue = setintersection(graph->Neighbors.at(node1),graph->Neighbors.at(node2));
        double unionvalue = setunion(graph->Neighbors.at(node1),graph->Neighbors.at(node2));

        mapnodessimilarity[link] = make_pair(intervalue,unionvalue);
    }


    //Get the adj link, defined a node_link means a node correspond to which link
    vector<vector<int> > node_link(graph->GetVcount());
    for(vector<pair<int,int> >::const_iterator ite=graph->Linklist.begin();
         ite!=graph->Linklist.end(); ite++)
     {
         pair<int,int> link=*ite;
         node_link[link.first].push_back(int(ite-graph->Linklist.begin()));
         node_link[link.second].push_back(int(ite-graph->Linklist.begin()));
     }

    int linkindex = 0;
    int anothernode_currentlink, anothernode_anotherlink;
    double simi;

    vector<int>::iterator eraseindex;

    for(vector<pair<int,int> >::const_iterator ite = graph->Linklist.begin();
         ite != graph->Linklist.end(); ite++)
     {
         pair<int,int> currentlink = *ite;// link is the current link we dealing with;

         //cal the similarity between the currentlink and it's adj link through node_link;
         //deal with the first node connect with the currentlink;

         for(vector<int>::const_iterator i = node_link[currentlink.first].begin();
            i != node_link[currentlink.first].end(); i++)
         {
             pair<int,int> anotherlink = graph->Linklist[*i];
             if (*i != linkindex)
             {
                 //indicate the link *i is adj with the current link linkindex;
                 anothernode_currentlink = currentlink.second;
//                 currentnode_currentlink = currentlink.first;
                 if (anotherlink.first == currentlink.first)
                 {
                    anothernode_anotherlink = anotherlink.second;
//                    currentnode_anotherlink = anotherlink.first;
                 }
                 else
                 {
                    anothernode_anotherlink=anotherlink.first;
//                    currentnode_anotherlink=anotherlink.second;
                 }


//                /*自己提出的新的相似度公式*/
                double intervalue += mapnodessimilarity[currentlink].first +
                                    mapnodessimilarity[anotherlink].first;
                double unionvalue += mapnodessimilarity[currentlink].second +
                                    mapnodessimilarity[anotherlink].second;

                intervalue += setintersection(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
                unionvalue += setunion(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
                simi = intervalue/unionvalue;
//                /*以上都是自己设计的相似度*/




////                /*为了比较，我们与普通的相似度进行比较*/
//                double intervalue = setintersection(graph->Neighbors[anothernode_currentlink],
//                                                   graph->Neighbors[anothernode_anotherlink]);
//
//                double unionvalue = setunion(graph->Neighbors[anothernode_currentlink],
//                                             graph->Neighbors[anothernode_anotherlink]);
//
//                simi = intervalue/unionvalue;



//                /*用三个节点的交集除以三个节点的并集*/
//                intervalue += (graph->node_degree(anothernode_currentlink)+graph->node_degree(anothernode_anotherlink))*setintersection(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
//                unionvalue += (graph->node_degree(anothernode_currentlink)+graph->node_degree(anothernode_anotherlink))*setunion(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
//                simi = intervalue/unionvalue;




                linksimilarity[linkindex].push_back(make_pair(*i,simi));
                linksimilarity[*i].push_back(make_pair(linkindex,simi));

              }

         }

         //erase the linkindex from the node_link;
         eraseindex = find(node_link[currentlink.first].begin(),node_link[currentlink.first].end(),linkindex);
         node_link[currentlink.first].erase(eraseindex);

         //deal with the second node connect with the currentlink;
         for(vector<int>::const_iterator i = node_link[currentlink.second].begin();
            i != node_link[currentlink.second].end(); i++)
         {
             pair<int,int> anotherlink = graph->Linklist[*i];

             if (*i != linkindex)
             {
                 //indicate the link *i is adj with the current link linkindex;
                 anothernode_currentlink = currentlink.first;
//                 currentnode_currentlink = currentlink.second;
                 if (anotherlink.first == currentlink.second)
                 {
                    anothernode_anotherlink = anotherlink.second;
//                    currentnode_anotherlink = anotherlink.first;
                 }
                 else
                 {
                    anothernode_anotherlink = anotherlink.first;
//                    currentnode_anotherlink = anotherlink.second;
                 }



                /*自己提出的新的相似度公式*/
                double intervalue = mapnodessimilarity[currentlink].first +
                                    mapnodessimilarity[anotherlink].first;
                double unionvalue = mapnodessimilarity[currentlink].second +
                                    mapnodessimilarity[anotherlink].second;

                intervalue += setintersection(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
                unionvalue += setunion(graph->Neighbors[anothernode_currentlink],graph->Neighbors[anothernode_anotherlink]);
                simi = intervalue/unionvalue;


//                   /*为了比较，我们与普通的相似度进行比较*/
//                double intervalue = setintersection(graph->Neighbors[anothernode_currentlink],
//                                                   graph->Neighbors[anothernode_anotherlink]);
//
//                double unionvalue = setunion(graph->Neighbors[anothernode_currentlink],
//                                             graph->Neighbors[anothernode_anotherlink]);
//
//                simi = intervalue/unionvalue;



                    linksimilarity[linkindex].push_back(make_pair(*i,simi));
                    linksimilarity[*i].push_back(make_pair(linkindex,simi));

                }

            }

         //erase the linkindex from the node_link;
         eraseindex = find(node_link[currentlink.second].begin(),node_link[currentlink.second].end(),linkindex);
         node_link[currentlink.second].erase(eraseindex);

         linkindex++;
     }


     return linksimilarity;
}

double setintersection(vector<int> v1,vector<int> v2)
{
    set<int> s;
    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),inserter(s,s.begin()));
    return double(s.size());
}

double setunion(vector<int> v1,vector<int> v2)
{
    set<int> s;
    set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),inserter(s,s.begin()));
    return double(s.size());
}

//自定义排序函数
bool descend( const pair<int,double> &p1, const pair<int,double> &p2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return p1.second > p2.second;//降序排列
}

vector<int> linksequence(vector<double> averagesimilarity)
{
    vector<int> sequence;

    vector<pair<int,double> > p;

    int index=0;
    for (vector<double>::const_iterator i=averagesimilarity.begin();i!=averagesimilarity.end();i++)
    {
        pair<int,double> p1;
        p1.first=index++;
        p1.second=*i;
        p.push_back(p1);
    }

    //descend sort
    std::sort(p.begin(),p.end(),descend);

    //return the linkindex
    for (vector<pair<int,double> >::const_iterator i=p.begin(); i!=p.end(); i++)
    {
        sequence.push_back((*i).first);
    }

    return sequence;
}

int labelfunction(vector<pair<int,double> > link_similarity, Graph *graph,vector<int> label,
                     vector<double> averagesimilarity, int linknum)
{
    set<int> linkset;
    for (vector<pair<int,double> >::const_iterator vit=link_similarity.begin();vit!=link_similarity.end();vit++)
    {
        pair<int,double> t=*vit;
        linkset.insert(t.first);
    }

        //the set of links with the same label
        map<int,set<int> > label_links;
        for (set<int>::const_iterator it=linkset.begin(); it!=linkset.end(); it++)
        {
            label_links[label[*it]].insert(*it);
        }

       //这个是有问题的：不应该以邻居边的平均相似度作为评判指标
//    //我们以每条边周围邻居边的平均相似度作为标签传播的依据，而不是邻居边的个数
//    map<int,double>label_simi;
//    for (map<int,set<int> >::const_iterator mapit=label_links.begin();mapit!=label_links.end();mapit++)
//    {
//        int label=mapit->first;
//        set<int> neighborlinks=mapit->second;
//        double sum=0.0;
//        for (set<int>::const_iterator setit=neighborlinks.begin();setit!=neighborlinks.end();setit++)
//        {
//            int neighborlinknum=*setit;
//            sum+=averagesimilarity[neighborlinknum];
//        }
////        cout<<"neighborslinks.size()/alpha的值为："<<neighborlinks.size()/alpha<<endl;
//        sum=sum*(neighborlinks.size()/alpha);
//        label_simi[label]=sum;
//    }

//       map<int,double>label_simi;
//       for (map<int,set<int> >::const_iterator mapit=label_links.begin();mapit!=label_links.end();mapit++)
//       {
//           int label=mapit->first;
//           set<int> neighborlinks=mapit->second;
//           double sum=0.0;
//           for (set<int>::const_iterator setit=neighborlinks.begin();setit!=neighborlinks.end();setit++)
//           {
//               int neighborlinknum=*setit;
//               for (vector<pair<int,double> >::const_iterator vit=link_similarity.begin();vit!=link_similarity.end();vit++)
//               {
//                   if((*vit).first==neighborlinknum)
//                   {
//                        sum=sum+(*vit).second;
//                        break;
//                   }
//
//               }
//           }
////           double sum1=pow((double)neighborlinks.size(),alpha);
////           sum=sum/sum1;
//
////            sum=sum*exp(-sum/alpha);
//           label_simi[label]=sum;
//       }

        //按照上面的代码对计算同一标签的相似度之和这个功能进行改进
        map<int,double> label_simi;
        for (vector<pair<int,double> >::const_iterator vit=link_similarity.begin();vit!=link_similarity.end();vit++)
        {
            int linknum = (*vit).first;
            double sim = (*vit).second;
            int linklabel = label[linknum];
            label_simi[linklabel] = label_simi[linklabel] + sim;
        }

//    cout<<"查看label_simi的值："<<endl;
//    for(map<int,double>::const_iterator i=label_simi.begin();i!=label_simi.end();i++)
//    {
//        cout<<i->first<<" "<<i->second<<endl;
//    }
//    cout<<endl;

    double maxvalue=0.0;
    set<int> labelset;
    for (map<int,double>::const_iterator mapit=label_simi.begin(); mapit!=label_simi.end(); mapit++)
    {
        if (mapit->second>=maxvalue)
        {
            if (mapit->second>maxvalue)
            {
                labelset.clear();
                labelset.insert(mapit->first);

            }
            else
                labelset.insert(mapit->first);

            maxvalue = mapit->second;

        }
    }

    //if labelset is empty, we let the current link as a single lable
    if (labelset.empty())
        return maxlabel++;
    else if(labelset.size()==1)
    {
        set<int>::const_iterator i=labelset.begin();
        return *i;
    }
    else
    {
//        //rand select a label, return;
//        srand((unsigned)time(NULL));
//        int randnum=rand() % labelset.size();
//
//        set<int>::iterator i=labelset.begin();
//
//        for (int x=0; x<=randnum&&i!=labelset.end(); x++,i++)
//        {
//            if (x==randnum)
//                return *i;
//        }
//        //选择一个相似度最大的标签返回
//        double similarity_max=0;
//        double averagesimilarity_max=0;
//        int label_return=0;
//
//        for (set<int>::const_iterator labelit=labelset.begin();labelit!=labelset.end();labelit++)
//        {
//            int la=*labelit;
//            set<int> links=label_links[la];
//            assert(!links.empty());
//            for (set<int>::const_iterator linksit=links.begin();linksit!=links.end();linksit++)
//            {
//                for (vector<pair<int,double> >::const_iterator it=link_similarity.begin(); it!=link_similarity.end(); it++)
//                {
//                    pair<int,double> temp=*it;
//                    if (temp.first==*linksit)
//                    {
//                        if (temp.second>=similarity_max)
//                        {
//                            if (averagesimilarity[temp.first]>averagesimilarity_max)
//                            {
//                                similarity_max=temp.second;
//                                averagesimilarity_max=averagesimilarity[temp.first];
//                                label_return=la;
//                                break;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//        return label_return;

        //选择一个averagesimilarity最大的边的标签返回
        //double similarity_max=0.0;
        double averagesimilarity_max=0.0;
        int label_return=0;
        for (set<int>::const_iterator labelit=labelset.begin();labelit!=labelset.end();labelit++)
        {
            int la=*labelit;
            set<int> links=label_links[la];
            //assert(!links.empty());
            for (set<int>::const_iterator linksit=links.begin();linksit!=links.end();linksit++)
            {

                if (averagesimilarity[*linksit]>averagesimilarity_max)
                {
                    averagesimilarity_max=averagesimilarity[*linksit];
                    label_return=la;
                }

//                for (vector<pair<int,double> >::const_iterator it=link_similarity.begin(); it!=link_similarity.end(); it++)
//                {
//                    pair<int,double> temp=*it;
//                    if (temp.first==*linksit)
//                    {
//                        if (temp.second>=similarity_max)
//                        {
//                            if (averagesimilarity[temp.first]>averagesimilarity_max)
//                            {
//                                similarity_max=temp.second;
//                                averagesimilarity_max=averagesimilarity[temp.first];
//                                label_return=la;
//                                break;
//                            }
//                        }
//                    }
//                }
            }
        }
        return label_return;

    }

    return -1;
}

////labelfunction1
//int labelfunction1(vector<pair<int,double> > link_similarity, Graph *graph,vector<int> label,
//                     vector<double> averagesimilarity, int linknum, double alpha)
//{
//
//    return -1;
//}



//stop criterion
bool StopCriterion(vector<int> labeloriginal, vector<int> label)
{
    bool stop=false;

    if (*max_element(labeloriginal.begin(),labeloriginal.end())==1)
        return stop;

    //根据labeloriginal和label解码
    map<int,set<int> > label_linkset_original,label_linkset;
    for (vector<int>::const_iterator i=labeloriginal.begin();i!=labeloriginal.end();i++)
    {
        int currentlabel=*i;
        label_linkset_original[currentlabel].insert(int(i-labeloriginal.begin()));
    }

    for (vector<int>::const_iterator i=label.begin();i!=label.end();i++)
    {
        int currentlabel=*i;
        label_linkset[currentlabel].insert(int(i-label.begin()));
    }

    if (label_linkset_original.size()!=label_linkset.size())
        return stop;
    else
    {
        for (map<int,set<int> >::const_iterator mapit=label_linkset_original.begin();
            mapit!=label_linkset_original.end();mapit++)
        {
            set<int> linksetorighinal=mapit->second;
            set<int> linkset=label_linkset[mapit->first];

//            cout<<endl<<"查看linksetoriginal的值："<<endl;
//            for (set<int>::const_iterator sit=linksetorighinal.begin();sit!=linksetorighinal.end();sit++)
//            {
//                cout<<*sit<<" ";
//            }
//            cout<<endl<<"查看linkset的值："<<endl;
//            for (set<int>::const_iterator sit=linkset.begin();sit!=linkset.end();sit++)
//            {
//                cout<<*sit<<" ";
//            }
//            cout<<endl;

            if (linksetorighinal.size()!=linkset.size())
                return stop;
        }
    }
    stop=true;
    return stop;
}


//decode process
vector<vector<int> > decode(vector<int> label,Graph*graph,double threshold)
{
    vector<int> newlabel=label;

    //vector<set<int> > node_links=graph->nodeTolinks();

//    cout<<"查看node_links的值："<<endl;
//    for (vector<set<int> >::const_iterator i=node_links.begin();i!=node_links.end();i++)
//    {
//        set<int> links=*i;
//        for (set<int>::const_iterator j=links.begin();j!=links.end();j++)
//        {
//            cout<<graph->Nodenames[graph->Linklist[*j].first]<<"_"<<graph->Nodenames[graph->Linklist[*j].second]<<" ";
//        }
//        cout<<endl;
//    }

    //判断每个点周围边的标签
    for (int x=0;x<graph->vcount;x++)
    {
        set<int> links = graph->node_links[x];
        map<int,set<int> > label_to_linkset;

        for (set<int>::const_iterator it=links.begin(); it!=links.end(); it++)
        {
            int lab=label[*it];
            label_to_linkset[lab].insert(*it);
        }

        //取含有最多一样标签的数
        double maxsamelabel=0.0;
        for (map<int,set<int> >::const_iterator i=label_to_linkset.begin();i!=label_to_linkset.end();i++)
        {
            if (i->second.size()>maxsamelabel)
                maxsamelabel=i->second.size();
        }

        //如果拥有这个标签的边数所占的比例小于这个阈值，则将这些边的标签设为单独一个标签
        for (map<int,set<int> >::const_iterator mapit = label_to_linkset.begin(); mapit!=label_to_linkset.end();mapit++)
        {
            set<int> linkset = mapit->second;
            double value1=linkset.size();
            double proportion=value1/maxsamelabel;
            if (proportion<threshold)
            {
                for (set<int>::const_iterator i=linkset.begin();i!=linkset.end();i++)
                {
                    newlabel[*i]=maxlabel++;
                }
            }
        }

    }

    vector<vector<int> > community;
    typedef set<int> linkset;

    map<int,linkset> label_to_linkset;

    for (vector<int>::const_iterator it=newlabel.begin(); it!=newlabel.end(); it++)
    {
        int lab=*it;
        label_to_linkset[lab].insert(int(it-newlabel.begin()));
    }

    //obtain the node set through label_to_linkset
//    int community_num=0;
    for (map<int,linkset>::const_iterator mapit=label_to_linkset.begin(); mapit!=label_to_linkset.end(); mapit++)
    {
        linkset links=mapit->second;
        set <int> linkcom;
        for (linkset::const_iterator it= links.begin(); it!=links.end(); it++)
        {
            int link=*it;
            linkcom.insert((graph->Linklist)[link].first);
            linkcom.insert((graph->Linklist)[link].second);
        }

        vector<int> v;
        for (set<int>::const_iterator setit=linkcom.begin(); setit!=linkcom.end(); setit++)
            v.push_back(*setit);

        if(v.size()>2)
            community.push_back(v);
    }
    return community;
}

//double thresholdfunction(vector<double>averatesimilarity,vector<pair<int,double> > linksimi,int linknum)
//{
//    double s=0.0;
//
//    //f(x)=e(-x);
////    cout<<exp(-s)<<" ";
//    return exp(-s);
//}
