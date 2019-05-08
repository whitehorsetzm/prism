//#include "struts.h"
//int     partition(HYBRID_MESH&tetrasfile, HYBRID_MESH *tetrasPart, int nparts, int stride, int offset, int type)
//{

//    HEX *pHexes=tetrasfile.pHexes;

//    idx_t NumVertices=tetrasfile.NumNodes;

//    idx_t NumElement=tetrasfile.NumTetras;

//    idx_t ncommon=3;

//    idx_t NumParts=nparts;

//    idx_t *eptr=new idx_t[NumElement+1];

//    idx_t *eind=new idx_t[NumElement*8];

//    for(int i=0;i<NumElement+1;i++)
//    {
//        eptr[i]=i*8;         //啥意思
//    }

//    for(int i=0;i<NumElement;i++)
//    {
//        for(int j=0;j<8;j++)
//        {
//            eind[i*8+j]=pHexes[i].vertices[j];
//        }
//    }

//    idx_t *epart=new idx_t[NumElement];

//    idx_t *npart=new idx_t[NumVertices];

//    idx_t objvalue=0;

//    idx_t options[40];

//    METIS_SetDefaultOptions(options);

//    options[METIS_OPTION_NUMBERING]=0;


//    if(nparts>1)
//    {
//        int results=METIS_PartMeshDual(&NumElement,&NumVertices,eptr,eind,nullptr,nullptr,&ncommon,&NumParts,nullptr,
//                                       nullptr,&objvalue ,epart,npart);
//    }
//    else
//    {
//        for(int i=0;i<NumElement;i++)
//        {
//            epart[i]=0;
//        }
//        for(int i=0;i<NumVertices;i++)
//        {
//            npart[i]=0;
//        }
//    }

//    for(int i=0;i<NumElement;i++)
//    {
//      //  tetrasfile.pTetras[i].partMarker=epart[i]*stride+offset;//shift partMarker
//        tetrasfile.pHexes[i].partMarker=epart[i];
//        tetrasfile.pHexes[i].index=i;//reset the index
//    }
//    for(int i=0;i<NumVertices;i++)
//    {
//        tetrasfile.nodes[i].partMarker=npart[i];
//    }

//    if(eptr!=nullptr)
//        delete []eptr;
//    eptr=nullptr;
//    if(eind!=nullptr)
//        delete []eind;
//    eind=nullptr;


//    int tetrasID=-1;
//    for(int i=0;i<tetrasfile.NumQuads;i++)
//    {
//        tetrasID=tetrasfile.pQuads[i].iCell;
//        tetrasfile.pQuads[i].partMarker=tetrasfile.pHexes[tetrasID].partMarker; //通过体得到面的PartMarker
//    }

////set the origin partMarker

//    //insert part information to node
//    for(int i=0;i<tetrasfile.NumQuads;i++)
//    {
//        int pointID=-1;


//        int procID=tetrasfile.pHexes[i].partMarker;

//        for(int j=0;j<8;j++)
//        {
//            pointID=tetrasfile.pHexes[i].vertices[j];
//            tetrasfile.nodes[pointID].procs.insert(procID);  //可以用于判断node是否在边界？
//        }
//     //   pointID=tetrasfile.pTetras[i].vertices[0];
//    }

//    //prepare the data to deliver



//    //set<>

//    set<int> *elementPart=new set<int>[nparts];

//    set<int> *facetPart=new set<int>[nparts];

//    int partID=-1;
//    for(int i=0;i<NumElement;i++)
//    {
//        //   tetrasfile.elements[i].partMarker=epart[i];
//        partID=epart[i];
//        elementPart[partID].insert(i);

//    }
//    for(int i=0;i<tetrasfile.NumTris;i++)
//    {
//        partID=tetrasfile.pTris[i].partMarker;
//        facetPart[partID].insert(i);
//    }





//    int tempSize=-1;
//    int tempID=-1;
//    int tempMID=-1;

//    //set<int>pointID;
//    map<int,int>global_to_local;

//    map<int,int>local_to_global;

//    set<int>::iterator setIter;
//    map<int,int>::iterator mapIter;
//    int count=0;
//    for(int i=0;i<nparts;i++)
//    {
//        tempSize=elementPart[i].size();
//        tetrasPart[i].NumHexes=tempSize;
//        tetrasPart[i].pHexes=new HEX[tempSize];

//        count=0;
//        //insert pointID
//        global_to_local.clear();
//        local_to_global.clear();
//        for(setIter=elementPart[i].begin();setIter!=elementPart[i].end();setIter++)
//        {
//            tempID=*setIter;

//            for(int j=0;j<4;j++)
//            {
//                tempMID=tetrasfile.pHexes[tempID].vertices[j];
//                mapIter=global_to_local.find(tempMID);
//                if(mapIter!=global_to_local.end())
//                {
//                    //find it, do nothing
//                }
//                else
//                {
//                    //not found
//                    global_to_local[tempMID]=count;
//                    local_to_global[count]=tempMID;
//                    count++;
//                }
//            }
//        }
//        tetrasPart[i].NumNodes=count;
//        tetrasPart[i].nodes=new Node[tetrasPart[i].NumNodes];

//        for(int j=0;j<tetrasPart[i].NumNodes;j++)
//        {
//            mapIter=local_to_global.find(j);
//            tempMID=mapIter->second;
//            tetrasPart[i].nodes[j]=tetrasfile.nodes[tempMID];
//            tetrasPart[i].nodes[j].localID=j;
//        }

//        count=0;
//        //insert elementID
//        for(setIter=elementPart[i].begin();setIter!=elementPart[i].end();setIter++)
//        {
//            tempID=*setIter;
//            tetrasPart[i].pHexes[count]=tetrasfile.pHexes[tempID];

//            tetrasPart[i].pHexes[count].localID=count;

//            //tetrasPart[i].pTetras[count].index=

//            int tempPartMarker=tetrasPart[i].pHexes[count].partMarker;

//            tetrasPart[i].pHexes[count].partMarker=tempPartMarker*stride+offset;//////////////shift the partMarker


//            for(int j=0;j<8;j++)
//            {
//                tempMID=tetrasPart[i].pHexes[count].vertices[j];
//                tetrasPart[i].pHexes[count].vertices[j]=global_to_local[tempMID];
//            }
//            count++;

//        }
//   //     cout<<"tempSize: "<<tempSize<<" count:"<<count<<endl;
//        assert(count==tempSize);
//        //insert facets
//        tempSize=facetPart[i].size();
//        tetrasPart[i].NumQuads=tempSize;
//        tetrasPart[i].pQuads=new TRI[tetrasPart[i].NumQuads];
//        count=0;
//        for(setIter=facetPart[i].begin();setIter!=facetPart[i].end();setIter++)
//        {
//            tempID=*setIter;
//            tetrasPart[i].pQuads[count]=tetrasfile.pQuads[tempID];
//      //      tetrasPart[i].pTris[count].index=count;///////////////////////////////annotated by zhvliu

//            int tempPartMarker=tetrasPart[i].pQuads[count].partMarker;

//            tetrasPart[i].pQuads[count].partMarker=tempPartMarker*stride+offset;//////////////shift the partMarker

//            for(int j=0;j<4;j++)
//            {
//                tempMID=tetrasPart[i].pQuads[count].vertices[j];
//                tetrasPart[i].pQuads[count].vertices[j]=global_to_local[tempMID];
//            }
//            count++;
//        }
//    }



//    if(elementPart!=nullptr)
//    {
//        delete []elementPart;
//    }
//    if(facetPart!=nullptr)
//    {
//        delete []facetPart;
//    }


//    for(int i=0;i<NumElement;i++)
//    {
//        tetrasfile.pTetras[i].partMarker=epart[i]*stride+offset;//shift partMarker

//      //  tetrasfile.pTetras[i].localID=i;
//      //  tetrasfile.pTetras[i].partMarker=epart[i];
//    }

///*
//    char testName[256];
//    sprintf(testName,"%s%d%s","second",offset,".vtk");
//    writeVTKFile(testName,tetrasfile);


//    sprintf(testName,"%s%d%s","secondTriangle",offset,".vtk");
//    writeTriangleVTKFile(testName,tetrasfile);

//*/
//    if(epart!=nullptr)
//        delete []epart;
//    epart=nullptr;
//    if(npart!=nullptr)
//        delete []npart;
//    npart=nullptr;

//    map<string,int64_t> qua_globalID;
//    for(int i=0;i<nparts;i++)
//    {
//        constructFacets(tetrasPart[i],tetrasfile,qua_globalID);
//        char name[256];
//        sprintf(name,"%s%d%s","cubeInter",i,".vtk");
//       // writeTriangleVTKFile(name,tetrasPart[i]);
//    }

//    if(type==1)//which means the second partition
//    {
//        int comm_size;
//        MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
//       // map<string,int>tri_globalID;

//        cout<<"Entering type==1"<<endl;

//        int originFacets=tetrasfile.NumTris;
//        int sumFacets=0;
//        for(int i=0;i<nparts;i++)
//        {
//            sumFacets+=tetrasPart[i].NumTris;
//        }
//        cout<<"originFacets: "<<originFacets<<" sumFacets: "<<sumFacets<<endl;
//        int64_t nNewLocalFacets=(sumFacets-originFacets)/2;
//        cout<<"nNewLocalFacets: "<<nNewLocalFacets<<" tri_gloablID.size= "<<qua_globalID.size()<<endl;
//        assert(nNewLocalFacets==qua_globalID.size());

//        int64_t offset = nNewLocalFacets;
//        int64_t nNewGlobalFacets = nNewLocalFacets;
//        if (comm_size > 1)
//        {
//            MPI_Scan(&nNewLocalFacets, &offset, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
//            MPI_Allreduce(&nNewLocalFacets, &nNewGlobalFacets, 1,
//                          MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
//        }
//        offset -= nNewLocalFacets;
//        offset += tetrasfile.NumUniqueSurfFacets + tetrasfile.NumUniqueInterfFacets;
//        printf("tetrasfile.NumUniqueInterfFacets = %d (%lld)\n",
//               tetrasfile.NumUniqueInterfFacets, tetrasfile.NumUniqueInterfFacets);
//        printf("nNewGlobalFacets = %d (%lld)\n", nNewGlobalFacets, nNewGlobalFacets);
//        for (int i = 0; i < nparts; ++i)
//        {
//            tetrasPart[i].NumUniqueSurfFacets = tetrasfile.NumUniqueSurfFacets;
//            tetrasPart[i].NumUniqueInterfFacets = tetrasfile.NumUniqueInterfFacets + nNewGlobalFacets;
//            printf("tetrasPart[%d].NumUniqueInterfFacets = %d (%lld)\n", i,
//                   tetrasPart[i].NumUniqueInterfFacets, tetrasPart[i].NumUniqueInterfFacets);
//        }

//        map<string,int64_t>::iterator mapIter;
//        count=0;
//        for(mapIter=tri_globalID.begin();mapIter!=tri_globalID.end();mapIter++)
//        {
//            mapIter->second=count+offset;
//            count++;
//        }


//        for(int i=0;i<nparts;i++)
//        {
//            updateTriIndex(tetrasPart[i],tri_globalID);
//            findiCellFast(tetrasPart[i]);///////find icell
//        }



//    }

//    /*
//    const int nUniqueFacets = refinedMesh.numOfFacets() - nDuplicatedFacets;
//    int offset = nUniqueFacets;
//    MPI_Scan(&nUniqueFacets, &offset, 1, MPI_INT, MPI_SUM, comm);
//    MPI_Allreduce(const_cast<int*>(&nUniqueFacets), &refinedMesh.NumUniqueFacets,
//                  1, MPI_INT, MPI_SUM, comm);

//    if (rank == 0)
//        printf("##: nTotalUniqueFacets = %d\n", refinedMesh.NumUniqueFacets);

//    offset -= nUniqueFacets;

//    */
//    return 1;
//}
