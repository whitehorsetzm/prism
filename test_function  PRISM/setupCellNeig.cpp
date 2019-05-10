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

    vecRefIntFHash = (int *) malloc(sizeof(int)*nNodes);                     //����С���ֵΪ�±꣬����IDΪֵ���ϸ��£�ÿ�θ��¶���¼��face��next���ʿ����γ���״������¼�����������㡣
    vecInterFaces = (InterFace *) malloc(sizeof(InterFace)*nAllocFaceSize);  //������ÿ�������Ϣ
    ndIFaces = (int *) malloc(sizeof(int)*nAllocNodeFaceSize);               //��������ID
    if (!vecRefIntFHash || !vecInterFaces || !ndIFaces)
    {
        errCode = -1;                                                //�����ڴ�ռ䲢��֤�Ƿ�ɹ�
        goto FAIL;
    }

    ndSize = nNodes;
    clSize = nElems;

    for (i = 0; i < ndSize; i++)
        vecRefIntFHash[i] = -1;
    for (cellIdx = 0; cellIdx < clSize; cellIdx++)
    {
        pElem = &pHexes[cellIdx];
        for (i = 0; i <= 4; i++)                 //��ʼ��neighborsֵΪ-1  BKG_MESH_DIM == 3
            pElem->neighbors[i] = -1;
    }

    for (cellIdx = 0; cellIdx < clSize; cellIdx++)
    {
        if (cellIdx % 1000000 == 0 || cellIdx == clSize - 1)
            printf("%%%.2f.\n", 100.0*(cellIdx)/clSize);             //��������ٷֱȣ�����ʾ0%��99.99%�����for��Ϊ��Ҫ����ѭ��

        pElem = &pHexes[cellIdx];

        for (i = 0; i <= 4; i++) {

            if(sizeof(cf[i])/sizeof(int)==4){
            facNdIdx1 = pElem->vertices[cf[i][0]];
            facNdIdx2 = pElem->vertices[cf[i][1]];                //BKG_MESH_DIM == 3  new BKG_MESH_DIM Ӧ��Ϊ5
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
            faceIt = vecRefIntFHash[minFacNdIdx];      //�õ��Ӧ��neighborsΪfaceIt
            while (faceIt >= 0)                        //���õ�(��С��)��Χ�Ѿ����汻¼������
            {
                if (j >= nAllocNodeFaceSize)           //j��ʲô��     ndIFaces�ռ䲻�� ���·���
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
                ndIFaces[j++] = faceIt;                //ndIFaces�ռ��㹻����˳���Ѿ�¼�����id��������ndIFaces
                                                                                          //class InterFace
                                                                                          //{
                                                                                          //public:
                                                                                          //    int conn[BKG_MESH_DIM];  //��ĵ�ID
                                                                                          //    int lftCell, rgtCell;    //�����ڽ���
                                                                                          //    int hashNxt;             //
                                                                                          //};

                faceIt= vecInterFaces[faceIt].hashNxt;
            }
            ndIFaceSize = j;                      //jΪ��¼�������
                                                           //j�Լ�vecRefIntFHash������Ӧ����Ϊ�˼��ٲ����غ��棬�������vecInterFaces����
                                                           //vecRefIntFHash�д�����Ȼ�͵�ǰ������ϵ
                                                           //

            nCommon = 0;
            for (j = 0;	j < ndIFaceSize; j++) {                 // jΪ��ʱ��С����Χ�Ѿ����������������������������������faceIt Ҳ����һ������ΧһȦ������б����ҳ��غ���
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

            if (nCommon < 4) {//δ�ҵ��غ��棬ѭ�������˳���˵��������δ����������������δ�Ǽǹ�lftCell������еǼǲ���
                /* û���ҵ� */
                if (nFaceSize >= nAllocFaceSize)                      //���ռ䲻��������ռ�
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

                faceAd.conn[0] = facNdIdx1;          //���conn��
                faceAd.conn[1] = facNdIdx2;
                faceAd.conn[2] = facNdIdx3;
                faceAd.conn[3] = facNdIdx4;
                faceAd.lftCell = cellIdx;           //��ǰ���е������������һ���ڽ����ǵ�ǰ��
                faceAd.rgtCell = -1;

                faceAdIdx = nFaceSize;
                faceAd.hashNxt = vecRefIntFHash[minFacNdIdx];     //��һ��faceAdIdx������˴�����һ��faceAD��hasNxtΪ-1
                vecRefIntFHash[minFacNdIdx] = faceAdIdx;          //faceAdIdx������˴�������һ��faceAd��Ϊnext��
                vecInterFaces[nFaceSize++] = faceAd;              //faceAd˳���������
            }



            else {

                if(!(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                     vecInterFaces[ndIFaces[j]].rgtCell < 0))
                    cout<<cellIdx<<endl;

                assert(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                        vecInterFaces[ndIFaces[j]].rgtCell < 0);


                vecInterFaces[ndIFaces[j]].rgtCell = cellIdx;              //��Ȼ���غ��棬˵�������Ѿ��Ǽǹ���˵����������lftCell
                                                                           //��ô��ǰ����Ǹ����rgtCell��

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

                                                            //���������ڵ����neighbors�л��ഢ��Է������±�ΪʲôΪi��k����
                                              //i�ڴ�ѭ���д������ĵ�ǰ�棬k�����ǰ��ĶԵ���
            }                                                         //��cf[4][3]���Կ�������ʱk�����i
                                                                    //��ʱ����rgtCell��������Ȼ���ܱ�֤neighbors[0]��������
                                                                    //lftCell��α�֤��
        }

            else
            {


                facNdIdx1 = pElem->vertices[cf[i][0]];
                facNdIdx2 = pElem->vertices[cf[i][1]];                //BKG_MESH_DIM == 3  new BKG_MESH_DIM Ӧ��Ϊ5
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
                while (faceIt >= 0)                        //���õ�(��С��)�Ѿ��ж�Ӧ��Neighbors�����neighbors�����ڶԽǵ㣩
                {
                    if (j >= nAllocNodeFaceSize)           //j��ʲô��     ndIFaces�ռ䲻�� ���·���
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
                    ndIFaces[j++] = faceIt;                //ndIFaces�ռ��㹻����˳��
                                                                                              //class InterFace
                                                                                              //{
                                                                                              //public:
                                                                                              //    int conn[BKG_MESH_DIM];  //��ĵ�ID
                                                                                              //    int lftCell, rgtCell;    //�����ڽ���
                                                                                              //    int hashNxt;             //
                                                                                              //};

                    faceIt= vecInterFaces[faceIt].hashNxt;  //neighbors��hasnext�����壿
                }
                ndIFaceSize = j;
                                                               //j�Լ�vecRefIntFHash������Ӧ����Ϊ�˼��ٲ����غ��棬�������vecInterFaces����
                                                               //vecRefIntFHash�д�����Ȼ�͵�ǰ������ϵ
                                                               //

                nCommon = 0;
                for (j = 0;	j < ndIFaceSize; j++) {                 //��faceIt Ҳ����һ������ΧһȦ������б����ҳ��غ���
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

                if (nCommon < 3) {//δ�ҵ��غ��棬ѭ�������˳���˵��������δ����������������δ�Ǽǹ�lftCell������еǼǲ���
                    if (nFaceSize >= nAllocFaceSize)                      //���ռ䲻��������ռ�
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

                    faceAd.conn[0] = facNdIdx1;          //���conn��
                    faceAd.conn[1] = facNdIdx2;
                    faceAd.conn[2] = facNdIdx3;
                    faceAd.lftCell = cellIdx;           //��ǰ���е������������һ���ڽ����ǵ�ǰ��
                    faceAd.rgtCell = -1;

                    faceAdIdx = nFaceSize;
                    faceAd.hashNxt = vecRefIntFHash[minFacNdIdx];     //��һ��faceAdIdx������˴�����һ��faceAD��hasNxtΪ-1
                    vecRefIntFHash[minFacNdIdx] = faceAdIdx;          //faceAdIdx������˴�������һ��faceAd��Ϊnext��
                    vecInterFaces[nFaceSize++] = faceAd;              //faceAd˳���������
                }


                                                                               //�ҵ��غ�����break�˳�
                else {

                    if(!(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                         vecInterFaces[ndIFaces[j]].rgtCell < 0))

                    assert(vecInterFaces[ndIFaces[j]].lftCell >= 0 &&
                            vecInterFaces[ndIFaces[j]].rgtCell < 0);


                    vecInterFaces[ndIFaces[j]].rgtCell = cellIdx;              //��Ȼ���غ��棬˵�������Ѿ��Ǽǹ���˵����������lftCell
                                                                               //��ô��ǰ����Ǹ����rgtCell��

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

                                                                //���������ڵ����neighbors�л��ഢ��Է������±�ΪʲôΪi��k����
                                                  //i�ڴ�ѭ���д������ĵ�ǰ�棬k�����ǰ��ĶԵ���
                }                                                         //��cf[4][3]���Կ�������ʱk�����i
                                                                        //��ʱ����rgtCell��������Ȼ���ܱ�֤neighbors[0]��������
                                                                        //lftCell��α�֤��

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
