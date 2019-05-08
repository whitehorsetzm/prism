//int updateTriIndex(HYBRID_MESH &mesh,map<string,int64_t>&tri_globalID)
//{
//    int vertices[3];
//    map<string,int64_t>::iterator iter;
//    int count=0;
//    for(int i=0;i<mesh.NumTris;i++)
//    {
//        for(int j=0;j<3;j++)
//        {
//            vertices[j]=mesh.pTris[i].vertices[j];
//            vertices[j]=mesh.nodes[vertices[j]].index;
//        }
//        sort(vertices,vertices+3);
//        string temp=IntToString(vertices[0])+"_"+IntToString(vertices[1])+"_"+IntToString(vertices[2]);
//        iter=tri_globalID.find(temp);
//        if(iter==tri_globalID.end())
//        {
//            //not found , which means it is not the new interface
//        }
//        else
//        {
//            //found it
//            mesh.pTris[i].index=iter->second;
//            count++;
//        }

//    }
//    return 1;
//}
