#include "src/global.h"
#include <cstdio>

//loadFile
string FileFullName,FilePureName,FileSuffix;		//File name with suffix and file name without suffix 
bool clockwiseFlag;									//The file reads the direction of the polygon vertex, true means counterclockwise, false means clockwise
vector<Vec> Ori; 									//Original polygon vertex 
vector<Vec> shOri;									//Processed polygon vertices
int OriNodeNum;										//Original vertex number
int shOriNodeNum;									//Number of vertices after processing
bool edgeFlag=false; 								//If read start and end edge
Edge startEdge,endEdge; 							//Start edge and end edge 
double PloygonArea;									//Polygon area
double PloygonPerimeter; 							//Polygon perimeter
double Length;										//Estimated centerline length
double Width;										//Estimated cenetrline width
double prec;

//delaunay
vector<Node> shnOri;					//Processed polygon vertices 
vector<Node> enOri;						//Polygon vertxt after insertion point
int enOriNodeNum;						//Polygon vertxt num after insertion point  
set<int> L1;  							//L1 index 
multiset<int,cmpW_enOri> L2; 			//Index of the convex1 vertex stored by its weight 
set<int> L3; 							//Groove vertex index  
vector<Triangle> TML;					//Triangle list 
map<int,Edge> E;						//The key of one edge (the key is calculated as a unique integer through the two indexes of the edge) is mapped to the triangle id number containing the edge, and there must be a triangle in T1.
map<int,int> T1;						//If there are two triangles adjacent to one edge, the second one exists in T2, otherwise the corresponding value in T2 is -1. 
map<int,int> T2;						//Set of triangle II
set<int> Triid_II;						//Center Node
vector<Vec> CN;							//Mark breakpoint position 
vector<int> SEGL;						//Set of triangle I 
vector<int> EndTriid;   				
 
//search
vector<Vec> NodeList;					//Polygon node coordinates
vector<line> total_LineList;			//Total polygon side list
vector<line> LineList1;        			//First group edge
vector<line> LineList2;        			//Second group edge

vector<LLength> LineLength;
vector<Vec> Ori2;
vector<Vec> Ori3;
vector<Vec> centerNode;  
double x_min=-MAX;       				//Ploygon boundary 
double x_max=MAX;
double y_min=-MAX;
double y_max=MAX; 
void decprec(Vec &p);

bool check(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./lineExt boundary_x.txt\n");
        return false;
    }
    // other check...
    return true;
}

int main(int argc , char* argv[]){
    if (!check(argc, argv))
        return -1;

    string Name(argv[1]);
    FileFullName=Name;    
    int idx = FileFullName.find_last_of('_');
    if (idx == -1)
    {
        printf("Invalid file name format\n");
        return -1;
    }

    int EFFORT=2;
    loadFile(); 							//Load file 
	shrinkOri(EFFORT);						//Process orinal node
	
	if(edgeFlag){
//----------------------genCenterLine1---------------------------------------
		genCenterLine1();
        rectonly(EFFORT);
        /* refineCN(); */
		dumpCN(1);
	}
	else{
//----------------------genCenterLine2---------------------------------------
//		inishnOriNode();					 
//		increshnOri(Width);					 
//		iniL1L2L3(enOri);					
//		delaunay();							
//		genCenterLine2();					
//		dumpCN(2);

		
//----------------------genCenterLine3---------------------------------------
		genCenterLine3();
        refineCN(EFFORT);
        dumpCN(2);	
	}
}

