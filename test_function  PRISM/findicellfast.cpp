#include <set>
#include <map>
#include <string>
#include<iostream>
#include<sstream>
#include"struts.h"
#
//快速找到所有外表面所属的体
using namespace std;
int* cf[5];
int cf1[3] = {0, 1, 2};
int cf2[3] = {3, 4, 5};
int cf3[4] = {0, 1, 4, 3};
int cf4[4] = {1, 2, 5, 4};
int cf5[4] = {0, 2, 5, 3};

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
    cf[0] = cf1;
    cf[1] = cf2;
    cf[2] = cf3;
    cf[3] = cf4;
    cf[4] = cf5;
    int neigbor=-2;
    int count=0;
    set<int>cellIDs;

    map<string,int> trimap;

    int quad[4];
    int tris[3];
    int num=0;
    for(;num<file.NumQuads;num++)
    {
        quad[0]=file.pQuads[num].vertices[0];
        quad[1]=file.pQuads[num].vertices[1];
        quad[2]=file.pQuads[num].vertices[2];
        quad[3]=file.pQuads[num].vertices[3];
        sort(quad,quad+4);
        string temp=IntToString(quad[0])+"_"+IntToString(quad[1])+"_"+IntToString(quad[2])+"_"+IntToString(quad[3]);
        trimap[temp]=num;
    }
    for(;num<file.NumQuads+file.NumTris;num++)
    {
        tris[0]=file.pTris[num].vertices[0];
        tris[1]=file.pTris[num].vertices[1];
        tris[2]=file.pTris[num].vertices[2];
        sort(tris,tris+3);
        string temp=IntToString(tris[0])+"_"+IntToString(tris[1])+"_"+IntToString(tris[2]);
        trimap[temp]=num;
    }


   int neigbormark=-1;
    map<string,int>::iterator mapIter;
    for(int i=0;i<file.NumPrsm;i++)
    {
        neigbormark=-1;
        for(int j=0;j<5;j++)
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
                int k;
                string temp;
                   for (k = 0; k <= 5;k++) {
                 if(k==1||k==0){
                           facNdIdx1 = file.pHexes[i].vertices[cf[k][0]];
                           facNdIdx2 = file.pHexes[i].vertices[cf[k][1]];
                           facNdIdx3 = file.pHexes[i].vertices[cf[k][2]];
                           tris[0]=facNdIdx1;
                           tris[1]=facNdIdx2;
                           tris[2]=facNdIdx3;

                           sort(tris,tris+3);
                           if(double(tris[0]+tris[1]*2+tris[2]*3+tris[3]*4)/10==neigbormark){
                               temp=IntToString(tris[0])+"_"+IntToString(tris[1])+"_"+IntToString(tris[2]);
                               break;
                           }
                       }
                else{
                facNdIdx1 = file.pHexes[i].vertices[cf[k][0]];
                facNdIdx2 = file.pHexes[i].vertices[cf[k][1]];
                facNdIdx3 = file.pHexes[i].vertices[cf[k][2]];
                facNdIdx4 = file.pHexes[i].vertices[cf[k][3]];
                quad[0]=facNdIdx1;
                quad[1]=facNdIdx2;
                quad[2]=facNdIdx3;
                quad[3]=facNdIdx4;

                sort(quad,quad+4);
                if(double(quad[0]+quad[1]*2+quad[2]*3+quad[3]*4)/10==neigbormark)
                    break;
                   }
                }
                   if(i==6)
                       cout<<"error at find face"<<endl;

                temp=IntToString(quad[0])+"_"+IntToString(quad[1])+"_"+IntToString(quad[2])+"_"+IntToString(quad[3]);
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
