//int readCGNS(char*filename,HYBRID_MESH&mesh,vector<string>&bcstring)
//{
//  //  float x[21*17*9],y[21*17*9],z[21*17*9];
//    cgsize_t isize[3][1];//ielem[20*16*8][8];
//    int index_file,index_base,index_zone;
//    cgsize_t irmin,irmax,istart,iend;
//    int nsections,index_sect,nbndry,iparent_flag;
//    cgsize_t iparentdata;
//    char zonename[33],sectionname[33];
//    CGNS_ENUMT(ElementType_t) itype;

///* READ X, Y, Z GRID POINTS FROM CGNS FILE */
///* open CGNS file for read-only */
//    int filetype=-1;
//    if (!cg_is_cgns(filename,&filetype))
//        cout<<"CGNS FILE: type="<<filetype<<endl;
//    else
//        cout<<"NOT CGNS FILE."<<endl;

//    cg_set_file_type(filetype);
//    if (cg_open(filename,CG_MODE_READ,&index_file))
//        cg_error_exit();
//    cout<<"Open CGNS file"<<endl;
///* we know there is only one base (real working code would check!) */
//    int nbases=-1;
//    cg_nbases(index_file,&nbases);
//    if(nbases!=1)
//    {
//        cout<<"The bases is more than 1!---"<<nbases<<endl;
//    }
//    cout<<"Get base number"<<endl;
//    index_base=1;
///* we know there is only one zone (real working code would check!) */
//    index_zone=1;
//    int nzones=-1;
//    cg_nzones(index_file,index_base,&nzones);
//    if(nzones!=1)
//    {
//        cout<<"The zones is more than 1!---"<<nzones<<endl;
//    }
///* get zone size (and name - although not needed here) */
//    cg_zone_read(index_file,index_base,index_zone,zonename,isize[0]);
//    cout<<"read Zone"<<endl;
///* lower range index */
//    irmin=1;
///* upper range index of vertices */
//    irmax=isize[0][0];
//    cout<<"irmax = "<< irmax <<endl;

//    int nNodes=irmax;

//    cgsize_t nprism=isize[1][0];
//    cgsize_t *prism=new cgsize_t[nprism*6];

///* read grid coordinates */
//    cout<<"Starting to read grid coordinates"<<endl;
//    int ncoord=-1;
//    cg_ncoords(index_file,index_base,index_zone,&ncoord);

//    float *x=new float[irmax];
//    float *y=new float[irmax];
//    float *z=new float[irmax];

//    cout<<"Read grid coordinates"<<endl;
//    cg_coord_read(index_file,index_base,index_zone,"CoordinateX",
//                  CGNS_ENUMV(RealSingle),&irmin,&irmax,x);
//    cg_coord_read(index_file,index_base,index_zone,"CoordinateY",
//                  CGNS_ENUMV(RealSingle),&irmin,&irmax,y);
//    cg_coord_read(index_file,index_base,index_zone,"CoordinateZ",
//                  CGNS_ENUMV(RealSingle),&irmin,&irmax,z);

//    cout<<"The number of vertices is: "<<irmax<<" "<<ncoord<<endl;
///* find out how many sections */
//    cg_nsections(index_file,index_base,index_zone,&nsections);
//    printf("\nnumber of sections=%i\n",nsections);
///* read element connectivity */

//    int nTriSum=0;

//    //cgsize_t *triSection[3];
//    cgsize_t **triSection=nullptr;
//    triSection=new cgsize_t*[nsections-1];
//    int *nTriSection=new int[nsections-1];
//    //vector<vector<cgsize_t>>triSection;
//    int count=0;

//    for (index_sect=1; index_sect <= nsections; index_sect++)
//    {
//      cg_section_read(index_file,index_base,index_zone,index_sect,sectionname,
//                      &itype,&istart,&iend,&nbndry,&iparent_flag);


//      printf("\nReading section data...\n");
//      printf("   section name=%s\n",sectionname);
//      printf("   section type=%s\n",ElementTypeName[itype]);
//      printf("   istart,iend=%i, %i\n",(int)istart,(int)iend);
//      if (itype == CGNS_ENUMV(PENTA_6))
//      {
//        printf("   reading element data for this Prism\n");
//        cg_elements_read(index_file,index_base,index_zone,index_sect,prism, \
//                         &iparentdata);
//      }

//      if (itype == CGNS_ENUMV(TRI_3))
//      {
//          int nTri=iend-istart+1;
//          nTriSection[count]=nTri;
//          nTriSum+=nTri;
//          triSection[count]=new cgsize_t[nTri*3];
//          string strName=sectionname;
//          bcstring.push_back(strName);
//          printf("   reading element data for this element\n");
//          cg_elements_read(index_file,index_base,index_zone,index_sect,triSection[count], \
//                  &iparentdata);
//          count++;
//      }

//    }
///* close CGNS file */
//    cg_close(index_file);
//    printf("\nSuccessfully read unstructured grid from file grid_c.cgns\n");

//    mesh.NumNodes=nNodes;
//    mesh.nodes=new Node[nNodes];
//    mesh.NumTetras=nTetras;
//    mesh.pTetras=new TETRAS[nTetras];
//    mesh.NumTris=nTriSum;
//    mesh.pTris=new TRI[nTriSum];

//    for(int i=0;i<nNodes;i++)
//    {
//        mesh.nodes[i].index=i;
//        mesh.nodes[i].coord.x=x[i];
//        mesh.nodes[i].coord.y=y[i];
//        mesh.nodes[i].coord.z=z[i];
//    }
//    for(int i=0;i<nTetras;i++)
//    {
//        mesh.pTetras[i].index=i;
//        mesh.pTetras[i].vertices[0]=tetras[i*4+0]-1;
//        mesh.pTetras[i].vertices[1]=tetras[i*4+1]-1;
//        mesh.pTetras[i].vertices[2]=tetras[i*4+2]-1;
//        mesh.pTetras[i].vertices[3]=tetras[i*4+3]-1;
//    }

//    count=0;
//   // mesh.pTris[i].index=i;
//    for(int j=0;j<nsections-1;j++)
//    {
//        for(int k=0;k<nTriSection[j];k++)
//        {
//            mesh.pTris[count].index=count;
//            mesh.pTris[count].vertices[0]=triSection[j][k*3+0]-1;
//            mesh.pTris[count].vertices[1]=triSection[j][k*3+1]-1;
//            mesh.pTris[count].vertices[2]=triSection[j][k*3+2]-1;
//            mesh.pTris[count].iSurf=j;
//            count++;
//        }
//    }

//    assert(count==nTriSum);


//    return 1;
//}
