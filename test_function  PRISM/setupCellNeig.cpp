#include <iostream>
#include <fstream>
#include <assert.h>
#include"struts.h"
#include<algorithm>
#include<assert.h>
using namespace std;

double MAX_VALUE(double a,double b){
   return a>b?a:b;
}



int readhex(const char *filename,PRISM *pPrism){
           cout<<"readstart"<<endl;
            ifstream file;
            file.open(filename);
            if(file.is_open())
            cout<<"opened"<<endl;
            char word[200];
            string s;
            int n;
            while(file>>s){
                if(s=="CELLS")
                 {
                    cout<<"cells"<<endl;
                    file>>n;
                    cout<<n<<endl;
                    break;
                   }
            }

            file>>s;

                        int point[6];
                        int m;
                        for(int i=0;i<n;++i){
                            file>>m;
                          for(int j=0;j<m;++j){
                                file>>point[j];
                            }
                          for(int j=0;j<m;++j)
                            pPrism[i].vertices[j]=point[j];
                        }
                        cout<<"read succuss"<<endl;
              return 0;
        }   

int setupCellNeig(int nNodes, int nElems, PRISM *pHexes)
{
   ofstream test;
   test.open("test.txt");
   if(!test.is_open())
       return 0;
    int nAllocFaceSize = nNodes * 10, nAllocNodeFaceSize = 20384;
    int nFaceSize = 0;
    int *vecRefIntFHash = NULL;
    InterFace * vecInterFaces = NULL, *vecInterFaces_Temp = NULL;
    int *ndIFaces = NULL, *ndIFaces_Temp = NULL;
    int errCode = 0;
    int cellIdx, faceAdIdx, lftCell, rgtCell;
    int facNdIdx1, facNdIdx2, facNdIdx3,facNdIdx4, minFacNdIdx, faceIt;
    int ndIFaceSize = 0;
    int i, j, k, nCommon;
    InterFace faceAd;

    int* cf[5];
    int cf1[3] = {0, 1, 2};
    int cf2[3] = {3, 4, 5};
    int cf3[4] = {0, 1, 4, 3};
    int cf4[4] = {1, 2, 5, 4};
    int cf5[4] = {0, 2, 5, 3};
    cf[0] = cf1;
    cf[1] = cf2;
    cf[2] = cf3;
    cf[3] = cf4;
    cf[4] = cf5;

    int ndSize, clSize;
    //BKGElem *pElem = NULL, *pLft = NULL, *pRgt = NULL;

    PRISM *pElem = nullptr, *pLft=nullptr, *pRgt=nullptr;

    vecRefIntFHash = (int *) malloc(sizeof(int)*nNodes);                     //以最小点的值为下标，用面ID为值不断更新，每次更新都记录到face的next，故可以形成链状，最后记录的是链表的起点。
    vecInterFaces = (InterFace *) malloc(sizeof(InterFace)*nAllocFaceSize);  //储存着每个面的信息
    ndIFaces = (int *) malloc(sizeof(int)*nAllocNodeFaceSize);               //储存着面ID
    if (!vecRefIntFHash || !vecInterFaces || !ndIFaces)
    {
        errCode = -1;                                                //申请内存空间并验证是否成功
        goto FAIL;
    }

    ndSize = nNodes;
    clSize = nElems;

    for (i = 0; i < ndSize; i++)
        vecRefIntFHash[i] = -1;
    for (cellIdx = 0; cellIdx < clSize; cellIdx++)
    {
        pElem = &pHexes[cellIdx];
        for (i = 0; i <= 4; i++)                 //初始化neighbors值为-1  BKG_MESH_DIM == 3
            pElem->neighbors[i] = -1;
    }

    for (cellIdx = 0; cellIdx < clSize; cellIdx++)
    {
        if (cellIdx % 1000000 == 0 || cellIdx == clSize - 1)
            printf("%%%.2f.\n", 100.0*(cellIdx)/clSize);             //输出工作百分比（仅显示0%和99.99%）这个for内为主要工作循环

        pElem = &pHexes[cellIdx];

        for (i = 0; i <= 4; i++) {

            if(sizeof(cf[i])/sizeof(int)==4){
            facNdIdx1 = pElem->vertices[cf[i][0]];
            facNdIdx2 = pElem->vertices[cf[i][1]];                //BKG_MESH_DIM == 3  new BKG_MESH_DIM 应该为5
            facNdIdx3 = pElem->vertices[cf[i][2]];
            facNdIdx4 = pElem->vertices[cf[i][3]];

       int facevec[4]={facNdIdx1,facNdIdx2,facNdIdx3,facNdIdx4};
       sort(facevec,facevec+4);

            int min=nNodes+10;
            for(int index=0;index<4;++index) {
                 if(min>pElem->vertices[cf[i][index]])
                 min=pElem->vertices[cf[i][index]];
            }
            minFacNdIdx = min;


            j = 0;
            faceIt = vecRefIntFHash[minFacNdIdx];      //该点对应的neighbors为faceIt
            while (faceIt >= 0)                        //若该点(最小点)周围已经有面被录入容器
            {
                if (j >= nAllocNodeFaceSize)           //j是什么？     ndIFaces空间不足 重新分配
                {
                    nAllocNodeFaceSize += MAX_VALUE(100, (int)(nAllocNodeFaceSize * 0.1));
                    ndIFaces_Temp = (int*)realloc(ndIFaces, sizeof(int)*nAllocNodeFaceSize);
                    if (!ndIFaces_Temp)
                    {
                        errCode = -1;
                        goto FAIL;
                    }
                    ndIFaces = ndIFaces_Temp;
                }
                ndIFaces[j++] = faceIt;                //ndIFaces空间足够，则按顺序将已经录入的面id加入数组ndIFaces
                                                                                          //class InterFace
                                                                                          //{
                                                                                          //public:
                                                                                          //    int conn[BKG_MESH_DIM];  //面的点ID
                                                                                          //    int lftCell, rgtCell;    //左右邻接体
                                                                                          //    int hashNxt;             //
                                                                                          //};

                faceIt= vecInterFaces[faceIt].hashNxt;
            }
            ndIFaceSize = j;                      //j为已录入的数量
                                                           //j以及vecRefIntFHash的作用应该是为了加速查找重合面，否则遍历vecInterFaces即可
                                                           //vecRefIntFHash中存的面必然和当前面有联系
                                                           //

            nCommon = 0;
            for (j = 0;	j < ndIFaceSize; j++) {                 // j为暂时最小点周围已经加入容器的面的数量（非最终总数）对faceIt 也就是一个点周围一圈的面进行遍历找出重合面
                nCommon = 0;
                for (k = 0; k < 4; k++)
                    if (vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx1 ||
                            vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx2 ||
                            vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx3||
                        vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx4)
                        nCommon++;

                if (nCommon >= 4)
                {
            //        cout<<"find it !!!!!!!!!"<<endl;
                    break;
                }
            }

            if (nCommon < 4) {//未找到重合面，循环结束退出，说明该面尚未放入容器，该面尚未登记过lftCell。则进行登记操作
                /* 没锟斤拷锟揭碉拷 */
                if (nFaceSize >= nAllocFaceSize)                      //若空间不足则扩大空间
                {
                    nAllocFaceSize += MAX_VALUE(100, (int)(nAllocFaceSize * 0.1));
                    vecInterFaces_Temp = (InterFace*)realloc(vecInterFaces, sizeof(InterFace)*nAllocFaceSize);
                    if (!vecInterFaces_Temp)
                    {
                        errCode = -1;
                        goto FAIL;
                    }
                    vecInterFaces = vecInterFaces_Temp;
                }

                faceAd.conn[0] = facNdIdx1;          //面的conn点
                faceAd.conn[1] = facNdIdx2;
                faceAd.conn[2] = facNdIdx3;
                faceAd.conn[3] = facNdIdx4;
                faceAd.lftCell = cellIdx;           //当前体中的面毫无疑问有一个邻接体是当前体
                faceAd.rgtCell = -1;

                faceAdIdx = nFaceSize;
                faceAd.hashNxt = vecRefIntFHash[minFacNdIdx];     //上一个faceAdIdx被放入此处，第一个faceAD的hasNxt为-1
                vecRefIntFHash[minFacNdIdx] = faceAdIdx;          //faceAdIdx被放入此处，给下一个faceAd作为next。
                vecInterFaces[nFaceSize++] = faceAd;              //faceAd顺序放入容器
            }



            else {

                if(!(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                     vecInterFaces[ndIFaces[j]].rgtCell < 0))
                    cout<<cellIdx<<endl;

                assert(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                        vecInterFaces[ndIFaces[j]].rgtCell < 0);


                vecInterFaces[ndIFaces[j]].rgtCell = cellIdx;              //既然有重合面，说明该面已经登记过，说明该面已有lftCell
                                                                           //那么当前体就是该面的rgtCell。

                lftCell = vecInterFaces[ndIFaces[j]].lftCell;
                rgtCell = vecInterFaces[ndIFaces[j]].rgtCell;
                pLft = &pHexes[lftCell];
                pRgt = &pHexes[rgtCell];                             //

             for(int k=0;k<6;++k){
                   if(pLft->neighbors[k]==-1)
                {
                    pLft->neighbors[k] = rgtCell;
                    pLft->neighborsmark[k]=double((facevec[0]+facevec[1]*2+facevec[2]*3+facevec[3]*4))/10;
                    break;
                }
             }

                pRgt->neighbors[i] = lftCell;
                pRgt->neighborsmark[i]=double((facevec[0]+facevec[1]*2+facevec[2]*3+facevec[3]*4))/10;

                                                            //在两个相邻的体的neighbors中互相储存对方。（下标为什么为i和k？）
                                              //i在此循环中代表该体的当前面，k则代表当前面的对点编号
            }                                                         //由cf[4][3]可以看出，此时k恒等于i
                                                                    //此时对于rgtCell来讲，自然是能保证neighbors[0]不被覆盖
                                                                    //lftCell如何保证？
        }

            else
            {


                facNdIdx1 = pElem->vertices[cf[i][0]];
                facNdIdx2 = pElem->vertices[cf[i][1]];                //BKG_MESH_DIM == 3  new BKG_MESH_DIM 应该为5
                facNdIdx3 = pElem->vertices[cf[i][2]];

                int facevec[3]={facNdIdx1,facNdIdx2,facNdIdx3};
                sort(facevec,facevec+3);

                     int min=nNodes+10;
                     for(int index=0;index<3;++index) {
                          if(min>pElem->vertices[cf[i][index]])
                          min=pElem->vertices[cf[i][index]];
                     }
                     minFacNdIdx = min;

                j = 0;
                faceIt = vecRefIntFHash[minFacNdIdx];
                while (faceIt >= 0)                        //若该点(最小点)已经有对应的Neighbors（面的neighbors储存在对角点）
                {
                    if (j >= nAllocNodeFaceSize)           //j是什么？     ndIFaces空间不足 重新分配
                    {
                        nAllocNodeFaceSize += MAX_VALUE(100, (int)(nAllocNodeFaceSize * 0.1));
                        ndIFaces_Temp = (int*)realloc(ndIFaces, sizeof(int)*nAllocNodeFaceSize);
                        if (!ndIFaces_Temp)
                        {
                            errCode = -1;
                            goto FAIL;
                        }
                        ndIFaces = ndIFaces_Temp;
                    }
                    ndIFaces[j++] = faceIt;                //ndIFaces空间足够，则按顺序将
                                                                                              //class InterFace
                                                                                              //{
                                                                                              //public:
                                                                                              //    int conn[BKG_MESH_DIM];  //面的点ID
                                                                                              //    int lftCell, rgtCell;    //左右邻接体
                                                                                              //    int hashNxt;             //
                                                                                              //};

                    faceIt= vecInterFaces[faceIt].hashNxt;  //neighbors的hasnext的意义？
                }
                ndIFaceSize = j;
                                                               //j以及vecRefIntFHash的作用应该是为了加速查找重合面，否则遍历vecInterFaces即可
                                                               //vecRefIntFHash中存的面必然和当前面有联系
                                                               //

                nCommon = 0;
                for (j = 0;	j < ndIFaceSize; j++) {                 //对faceIt 也就是一个点周围一圈的面进行遍历找出重合面
                    nCommon = 0;
                    for (k = 0; k < 4; k++)
                        if (vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx1 ||
                                vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx2 ||
                                vecInterFaces[ndIFaces[j]].conn[k] == facNdIdx3)
                            nCommon++;
                    if (nCommon >= 3)
                    {
          //              cout<<"find it !!!!!!!!!"<<endl;
                        break;
                    }
                }

                if (nCommon < 3) {//未找到重合面，循环结束退出，说明该面尚未放入容器，该面尚未登记过lftCell。则进行登记操作
                    if (nFaceSize >= nAllocFaceSize)                      //若空间不足则扩大空间
                    {
                        nAllocFaceSize += MAX_VALUE(100, (int)(nAllocFaceSize * 0.1));
                        vecInterFaces_Temp = (InterFace*)realloc(vecInterFaces, sizeof(InterFace)*nAllocFaceSize);
                        if (!vecInterFaces_Temp)
                        {
                            errCode = -1;
                            goto FAIL;
                        }
                        vecInterFaces = vecInterFaces_Temp;
                    }

                    faceAd.conn[0] = facNdIdx1;          //面的conn点
                    faceAd.conn[1] = facNdIdx2;
                    faceAd.conn[2] = facNdIdx3;
                    faceAd.lftCell = cellIdx;           //当前体中的面毫无疑问有一个邻接体是当前体
                    faceAd.rgtCell = -1;

                    faceAdIdx = nFaceSize;
                    faceAd.hashNxt = vecRefIntFHash[minFacNdIdx];     //上一个faceAdIdx被放入此处，第一个faceAD的hasNxt为-1
                    vecRefIntFHash[minFacNdIdx] = faceAdIdx;          //faceAdIdx被放入此处，给下一个faceAd作为next。
                    vecInterFaces[nFaceSize++] = faceAd;              //faceAd顺序放入容器
                }


                                                                               //找到重合面由break退出
                else {

                    if(!(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                         vecInterFaces[ndIFaces[j]].rgtCell < 0))

                    assert(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                            vecInterFaces[ndIFaces[j]].rgtCell < 0);


                    vecInterFaces[ndIFaces[j]].rgtCell = cellIdx;              //既然有重合面，说明该面已经登记过，说明该面已有lftCell
                                                                               //那么当前体就是该面的rgtCell。

                    lftCell = vecInterFaces[ndIFaces[j]].lftCell;
                    rgtCell = vecInterFaces[ndIFaces[j]].rgtCell;
                    pLft = &pHexes[lftCell];
                    pRgt = &pHexes[rgtCell];                             //

                 for(int k=0;k<6;++k){

                       if(pLft->neighbors[k]==-1)
                    {
                        pLft->neighbors[k] = rgtCell;
                        pLft->neighborsmark[k]=double((facevec[0]+facevec[1]*2+facevec[2]*3))/10;
                        break;
                    }
                 }

                    pRgt->neighbors[i] = lftCell;
                    pRgt->neighborsmark[i]=double((facevec[0]+facevec[1]*2+facevec[2]*3))/10;

                                                                //在两个相邻的体的neighbors中互相储存对方。（下标为什么为i和k？）
                                                  //i在此循环中代表该体的当前面，k则代表当前面的对点编号
                }                                                         //由cf[4][3]可以看出，此时k恒等于i
                                                                        //此时对于rgtCell来讲，自然是能保证neighbors[0]不被覆盖
                                                                        //lftCell如何保证？

            }



    }

    }
    goto END;
FAIL:
END:
    if (vecRefIntFHash)
        free(vecRefIntFHash);
    if (vecInterFaces)
        free(vecInterFaces);
    if (ndIFaces)
        free(ndIFaces);

    return errCode; /* S_OK */
}

int main()
{
    PRISM *pHexes;
    pHexes = new PRISM[360190];
    readhex("cylinder_o.vtk",pHexes);
//    for(int i=0;i<360190;++i){
//        for(int j=0;j<6;++j)
//        cout<<pHexes[i].vertices[j]<<" ";
//        cout<<endl;
//    }
    setupCellNeig(205303,360190,pHexes);
        cout<<"test"<<endl;
    ofstream file;
    file.open("test_log.txt");
    if(file.is_open())
    cout<<"test"<<endl;
    for(int i=0;i<360190;++i){
        for(int j=0;j<5;++j)
        {
        file<<pHexes[i].neighbors[j];
        file<<" ";
        }
        file<<endl;
    }
return 0;
}
