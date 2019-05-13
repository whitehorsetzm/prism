#ifndef STRUTS_H
#define STRUTS_H
const int BKG_MESH_DIM=4;
#endif // STRUTS_H
class PRISM
{
public:
     PRISM():index(-1),localID(-1),partMarker(-1){}
    int index;
    int localID;
    int partMarker;
    double neighborsmark[5];
    int vertices[6];
    int neighbors[5];
};
class QUAD
{
public:

    int index;
    int iSurface;
    // int iCell; // the id of the element which contains this facet
    int iCell; // the local id of the element which contains this facet
    // bool bInterface; // true for interface, false for boundary facet.      //not used
    int iSurf; // the geometric surface index
    int iOppoProc; // the processor index of its neighbour
    // int iOppoIndex; // the local index of the facet on its neighboring processor  //not used
//    int addedNodes[NoEdges];
    int vertices[4];
    int partMarker; //the part marker of  the triangle
};

class TRI
{
public:

    int index;
    int iSurface;
    // int iCell; // the id of the element which contains this facet
    int iCell; // the local id of the element which contains this facet
    // bool bInterface; // true for interface, false for boundary facet.      //not used
    int iSurf; // the geometric surface index
    int iOppoProc; // the processor index of its neighbour
    // int iOppoIndex; // the local index of the facet on its neighboring processor  //not used
//    int addedNodes[NoEdges];
    int vertices[3];
    int partMarker; //the part marker of  the triangle
};


class HYBRID_MESH
{
public:
    HYBRID_MESH();
    ~HYBRID_MESH();
public:
    int NumNodes;
    int NumTetras;
    int NumHexes;
    int NumPyra;
    int NumPrsm;

    int NumUniqueSurfFacets;
    int NumUniqueInterfFacets;

    int NumTris;
    int NumQuads;

    PRISM *pHexes;
    TRI *pTris; // triangular boundary facets
    QUAD *pQuads; // quadangular boundary facets

};
class InterFace
{
public:
 int conn[BKG_MESH_DIM];  //面的点ID
 int lftCell, rgtCell;    //左右邻接体
 int hashNxt;             //
};
int findiCellFast(HYBRID_MESH &file);
