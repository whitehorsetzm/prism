//#include "struts.h"

//int constructOneQuad(int *vertices,QUAD& quad)
//{
//    //TRIANGLE triangle;

//    quad.vertices[0]=vertices[0];
//    quad.vertices[1]=vertices[1];
//    quad.vertices[2]=vertices[2];
//    quad.vertices[3]=vertices[3];

//    // triangle.

//    return 1;
//}

//int constructFacets(HYBRID_MESH&mesh, HYBRID_MESH& globalMesh,map<string,int64_t>&tri_globalID)
//{

//    int originNum=mesh.NumQuads;
//    for(int i=0;i<mesh.NumHexes;i++)
//    {
//        for(int j=0;j<6;j++)
//        {
//            mesh.pHexes[i].neighbors[j]=-1;
//        }
//    }
//    cout<<"Hexes Number is: "<<mesh.NumHexes<<endl;

//    setupCellNeig(mesh.NumNodes,mesh.NumHexes,mesh.pHexes);

//    map<string,bool>quaMap;

//    int vertices[4];
//    for(int i=0;i<mesh.NumQuads;i++)
//    {
//        vertices[0]=mesh.pQuads[i].vertices[0];
//        vertices[1]=mesh.pQuads[i].vertices[1];
//        vertices[2]=mesh.pQuads[i].vertices[2];
//        vertices[3]=mesh.pQuads[i].vertices[3];
//        sort(vertices,vertices+4);
//        string temp=IntToString(vertices[0])+"_";
//        temp+=IntToString(vertices[1]);
//        temp+="_";
//        temp+=IntToString(vertices[2]);
//        temp+="_";
//        temp+=IntToString(vertices[3]);
//        quaMap[temp]=true;
//    }



//int quaVID[6][4] = {{0, 1, 5, 4}, {3, 2, 6 ,7}, {5, 6, 7, 4}, {1, 2, 3, 0}, {1, 2, 5, 6}, {0, 3, 7, 4}};


//    vector<QUAD> interFacets;
//    int count=0;
//    int NumQuads=0;
//    for(int i=0;i<mesh.NumHexes;i++)
//    {
//        int forth=-1;
//        for(int j=0;j<6;j++)
//        {
//            if(mesh.pHexes[i].neighbors[j]==-1)  //首先，判断j点所对面是外表面
//            {

//                NumQuads++;
//                forth=j;
//                count=0;

//                int k;
//                for(k=0;k<6;k++)
//                {
//                    for(int l=0;l<4;++l)
//                    {
//                        vertices[l]=mesh.pQuads[i].vertices[quaVID[k][l]];
//                    }
//                    sort(vertices,vertices+4);
//                    int neighborsmark=double(vertices[0]+vertices[1]*2+vertices[2]*3+vertices[3]*4)/10;
//                    if(mesh.pHexes[i].neighborsmark[j]==neighborsmark)
//                    break;

//                }
//                if(k==6)
//                    cout<<"can't find neigbors face!!"<<endl;


//                /*
//                for(int k=0;k<4;k++)
//                {
//                    if(k!=forth)
//                    {
//                        vertices[count]=mesh.pTetras[i].vertices[k];
//                        count++;

//                    }
//                }*/
//                QUAD quadrangle;
//                constructOneQuad(vertices,quadrangle);
//                quadrangle.iCell=i;
//                int globalID=mesh.pHexes[i].index;            //获取全局下标
//                if(globalID<0||globalID>=globalMesh.NumTetras)
//                {
//                    cout<<"Error in finding the gloabl index!"<<endl;
//                }
//                globalID=globalMesh.pHexes[globalID].neighbors[forth];  //在拆分后是外表面，但是拆分前不一定是
//                if(globalID==-1)     // 若==-1，则拆分前也是外表面。
//                {
//                    quadrangle.iOppoProc=-1;
//                }
//                else               //否则不是，则用当前相邻体的ParMarker进行染色，于是一个分块的不同相邻面被染上了不同的颜色
//                {
//                    quadrangle.iOppoProc=globalMesh.pHexes[globalID].partMarker;
//                }
//                interFacets.push_back(quadrangle);
//            }
//            else
//            {
//                //nothing
//            }
//        }
//    }

//   // cout<<"No error ..........................."<<endl;

//    QUAD * facets= new QUAD[mesh.NumQuads];

//    for(int i=0;i<mesh.NumQuads;i++)
//    {
//        facets[i]=mesh.pQuads[i];
//    }
//    count=mesh.NumQuads;

//    mesh.NumQuads=interFacets.size();

//    if(mesh.pQuads!=nullptr)
//    {
//        delete []mesh.pQuads;
//        mesh.pQuads=nullptr;
//    }
//    mesh.pQuads=new QUAD[mesh.NumQuads];
//    for(int i=0;i<count;i++)
//    {
//        mesh.pQuads[i]=facets[i];
//    }
//    if(facets!=nullptr)
//    {
//        delete []facets;
//        facets=nullptr;
//    }

//    //   mesh.pTris=facets;
//    //facets=nullptr;

//    //  count=;
//    for(int i=0;i<interFacets.size();i++)
//    {
//        vertices[0]=interFacets[i].vertices[0];
//        vertices[1]=interFacets[i].vertices[1];
//        vertices[2]=interFacets[i].vertices[2];
//        vertices[3]=interFacets[i].vertices[3];
//        sort(vertices,vertices+4);
//        string temp=IntToString(vertices[0])+"_";
//        temp+=IntToString(vertices[1]);
//        temp+="_";
//        temp+=IntToString(vertices[2]);
//        temp+="_";
//        temp+=IntToString(vertices[3]);
//        if(quaMap.find(temp)!=quaMap.end())
//        {
//            //case : surface facet or interface created in the last partition

//        }
//        else
//        {
//            //case : new interface
//            int globalVertices[4];
//            globalVertices[0]=mesh.nodes[vertices[0]].index;
//            globalVertices[1]=mesh.nodes[vertices[1]].index;
//            globalVertices[2]=mesh.nodes[vertices[2]].index;
//            globalVertices[3]=mesh.nodes[vertices[3]].index;
//            sort(globalVertices,globalVertices+4);
//            string gTemp=IntToString(globalVertices[0])+"_"+IntToString(globalVertices[1])+"_"+IntToString(globalVertices[2])+"_"+IntToString(globalVertices[3]);

//            tri_globalID[gTemp]=-1;//temp value == -1////need gloabl index;

//            mesh.pQuads[count]=interFacets[i];
//            count++;
//        }
//    }

//    assert(count==mesh.NumQuads);

//    cout<<"Counter of the facets is: "<<NumQuads<<endl;
//    cout<<"Surface triangle Number is "<<originNum<<endl;
//    cout<<"All triangles Number is:"<<interFacets.size()<<endl;



//    return 1;
//}
