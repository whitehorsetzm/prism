#include <set>
#include <map>
#include <string>
#include<iostream>
#include<sstream>
#include"struts.h"

//快速找到所有外表面所属的体
using namespace std;
static int cf[6][4] = {{0, 1, 5, 4}, {3, 2, 6 ,7}, {5, 6, 7, 4}, {1, 2, 3, 0}, {1, 2, 5, 6}, {0, 3, 7, 4}};

string IntToString(int m)
{
    stringstream stream;
    stream<<m;
    string temp;
    stream>>temp;
    return temp;

}

int findiCellFast(HYBRID_MESH &file)
{
    int neigbor=-2;
    int count=0;
    set<int>cellIDs;

    map<string,int> trimap;

    int v[4];
    for(int i=0;i<file.NumQuads;i++)
    {
        v[0]=file.pQuads[i].vertices[0];
        v[1]=file.pQuads[i].vertices[1];
        v[2]=file.pQuads[i].vertices[2];
        v[3]=file.pQuads[i].vertices[3];
        sort(v,v+4);
        string temp=IntToString(v[0])+"_"+IntToString(v[1])+"_"+IntToString(v[2])+"_"+IntToString(v[3]);
        trimap[temp]=i;

    }

   int neigbormark=-1;
    map<string,int>::iterator mapIter;
    for(int i=0;i<file.NumHexes;i++)
    {
        neigbormark=-1;
        for(int j=0;j<6;j++)
        {
            neigbor=file.pHexes[i].neighbors[j];
            neigbormark=file.pHexes[i].neighborsmark[j];

            if(neigbor==-1)
            {
               // count++;

                cellIDs.insert(i);
//                for(int k=0;k<4;k++)
//                {
//                    if(k!=j)
//                    {
//                        v[count]=file.pTetras[i].vertices[k];
//                        count++;
//                    }
//                }
                int facNdIdx1,facNdIdx2,facNdIdx3,facNdIdx4;
                int vec[4];
                   for (int k = 0; k <= 5;k++) {
                facNdIdx1 = file.pHexes[i].vertices[cf[k][0]];
                facNdIdx2 = file.pHexes[i].vertices[cf[k][1]];
                facNdIdx3 = file.pHexes[i].vertices[cf[k][2]];
                facNdIdx4 = file.pHexes[i].vertices[cf[k][3]];
                vec[0]=facNdIdx1;
                vec[1]=facNdIdx2;
                vec[2]=facNdIdx3;
                vec[3]=facNdIdx4;

                sort(vec,vec+4);
                if(double(vec[0]+vec[1]*2+vec[2]*3+vec[3]*4)/10==neigbormark)
                    break;
                   }
                   if(i==6)
                       cout<<"error at find face"<<endl;

                string temp=IntToString(vec[0])+"_"+IntToString(vec[1])+"_"+IntToString(vec[2])+"_"+IntToString(vec[3]);
                mapIter=trimap.find(temp);
                if(mapIter==trimap.end())
                {
                    cout<<"Error in finding iCell!"<<endl;
                }
                 else
                {
                    int triID=mapIter->second;
                    file.pQuads[triID].iCell=i;
                }
            }
        }
    }

    cout<<"Finding iCell finished!"<<endl;
    return 1;
}
