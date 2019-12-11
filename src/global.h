#ifndef _GLOBAL_
#define _GLOBAL_ 
#include <string>
#include <iostream>
#include <vector>
#include <set> 
#include <map>

#define MAX 1000000000.0
#define MIN 0.00000000001
#define PI (acos(-1))
#define NODENUM 1000000								/*The maximum number of points in a polygon. 
													Handling polygons that exceed this number of vertices 
													may invalidate the algorithm genCenterLine2.*/

using namespace std;

//---------------------------------------------------struct and enum---------------------------------------------------//
enum plabel{notinied,convex,convex1,convex2,groove};//Polygon vertex type: notinied,convex,convex1,convex2,groove
enum tlabel{I,II,III};      						//Triangle type:I (with 2 edges as boundary edges), II (with 1 edge as boundary edge),III (with 0 edges as boundary edges) 
enum elabel{boundary,reuse};						//Triangle edge type: boundary, internal reuse 
enum pl{l,o,r};										//Point line relationship: online segment left, collinear with line segment, online segment right 

struct Vec{											//A vector,can represent points and vectors
	double x,y;
	Vec(double _x,double _y):x(_x),y(_y){}
	Vec(){}
};

struct Node{										//A node,represent ploygon vertexs
	double x,y;										//Vertex coordinates 
	int id;											//Each vertex has a unique id number, which is the vertex id read in order, and the vertex order is counterclockwise in the polygon boundary 
	double w;										//The triangle weight corresponding to the vertex 
	plabel type; 									//The type of the vertex, convex1,convex2,groove 
	int pre,next;									//Predecessor and successor index
	Node(double _x,double _y){
		x=_x;y=_y;
		id=-1;
		w=-1;
		type=notinied;
		pre=-1;next=-1;
	}
	Node(double _x,double _y,int _id){
		x=_x;y=_y;
		id=_id;
		w=-1;
		type=notinied;
		pre=-1;next=-1;
	}
	Node(double _x,double _y,double _w,plabel _type,int _id){
		x=_x;y=_y;
		id=_id;
		w=_w;
		type=_type;
		pre=-1;next=-1;
	}
	Node(){} 
};
 
struct GLine{										//General equation representing a straight line: ax+by+c=0 a>=0
	double a,b,c;
};

struct Edge{										//The edge of any two vertices of a polygon									
	int s,e;										//The start and end points of the edge, represented by the id of the polygon vertex
	int key;										//Each edge also has a unique id, mapped to a key, and the key is mapped by the two endpoints of the edge, such as (1, 4) and (4, 1) mapped to the same key. 
	elabel type;									//Edge type: boundary£¬reuse 
	Edge(int _s,int _e){
		s=_s;e=_e;
		key= s<e? (NODENUM*s+e):(NODENUM*e+s);
	}
	Edge(){}
};

struct Triangle{									//Represents a triangle
	int a,b,c;										//Index of three vertices
	double w;										//The weight of the triangle, the cos value of the minimum angle of the triangle 
	int id;											//The id of the triangle whose value is the order in which the triangle is cut 
	tlabel type; 									//Type of triangle£ºI,II,III 
	Triangle(int _a,int _b,int _c,double _w,int _id){
		a = _a;b = _b;c = _c;w=_w;id=_id;
	}
	Triangle(){}
};

struct dvec{Vec p;Vec v;};							//A point and its direction 
struct line{int s,e;};   							//A line segment,represented by an index of two vertices
struct LINE{Vec s;Vec e;};							//A line segment, represented by two coordinates
struct boundry{double xmin,xmax,ymin,ymax;}; 		//Bounding box

struct EE{											//A line segment represented by id,and overload operator < with its id
	int s,e,id;
	bool operator<(const EE & e)const{
		return id<e.id;
	}
};

struct seg{											//A line segment represented by id
	int s,e;
	int id;
};

struct P{											//A vertices with coordinates, id number, precursor and successor
	double x,y;
	int pre,next;
	int id;
	P(double _x,double _y,int _id){
		x=_x;y=_y;id=_id;
	} 
};

struct PP{											//A interval represented by two coordinates
	Vec s,e;
	PP(double _sx,double _sy,double _ex,double _ey){
		s.x=_sx;s.y=_sy;e.x=_ex;e.y=_ey;
	}
	PP(){};
}; 



//---------------------------------------------------Global variable---------------------------------------------------// 
//loadFile 
extern string FileFullName,FilePureName,FileSuffix;		//File name with suffix and file name without suffix 
extern bool clockwiseFlag;								//The file reads the direction of the polygon vertex, true means counterclockwise, false means clockwise
extern vector<Vec> Ori;  								//Original polygon vertex
extern vector<Vec> shOri;								//Processed polygon vertices 
extern int OriNodeNum;									//Original vertex number
extern int shOriNodeNum;								//Number of vertices after processing 
extern bool edgeFlag; 									//If read start and end edge
extern Edge startEdge,endEdge;							//Start edge and end edge  
extern double PloygonArea;								//Polygon area
extern double PloygonPerimeter;  						//Polygon perimeter 
extern double Length;									//Estimated centerline length
extern double Width;									//Estimated cenetrline width 
extern double prec;

//genCenterLine1
extern vector<Vec> NodeList;							//Polygon node coordinates
extern vector<line> total_LineList;						//Total polygon side list
extern vector<line> LineList1;        					//First group edge
extern vector<line> LineList2;        					//Second group edge

//delaunay
extern vector<Node> shnOri;								//Processed polygon vertices 
extern vector<Node> enOri;								//Polygon vertxt after insertion point
extern int enOriNodeNum;								//Polygon vertxt num after insertion point  
struct cmpW_enOri{
	bool operator() (const int& n1,const int& n2){
		return (enOri[n1].w>enOri[n2].w);
	}
};

//genCenterLine2
extern set<int> L1;    									//L1 index 
extern multiset<int,cmpW_enOri> L2; 					//Index of the convex1 vertex stored by its weight
extern set<int> L3;    									//Groove vertex index  
extern vector<Triangle> TML;							//Triangle list
extern map<int,Edge> E;									//Mapping from edge key values to edges
extern map<int,int> T1;									//The key of one edge (the key is calculated as a unique integer through the two indexes of the edge) is mapped to the triangle id number containing the edge, and there must be a triangle in T1.
extern map<int,int> T2;									//If there are two triangles adjacent to one edge, the second one exists in T2, otherwise the corresponding value in T2 is -1. 
extern set<int> Triid_II;								//Set of triangle II
extern vector<Vec> CN;									//Center Node
extern vector<int> SEGL;								//Mark breakpoint position 
extern vector<int> EndTriid;   							//Set of triangle I 

 
//---------------------------------------------------Function---------------------------------------------------//
//vec
Vec nmult(double,Vec);  								//Vector number multiplication
Vec add(Vec,Vec);       								//Vector addition
Vec sub(Vec,Vec);       								//Vector subtraction
double mult(Vec,Vec);      								//Vector inner product
double cmult(Vec,Vec);     								//Vector cross multiplication
double mod(Vec);        								//Vector mod
double mod_square(Vec); 								//Vector mod square
Vec uv(Vec v);											//Vector unit vector 
void swap(int &,int &);									//swap int
void swap(double &,double &);							//swap double
void swap(Vec &,Vec &);									//swap Vec
Vec middleNode(Vec,Vec);								//Calculate the midpoint of two points
double min(double,double);								//Maximum of two numbers
double max(double,double);								//Minimum of two numbers

//dist
double dist_p2p(Vec ,Vec );								//Calculate the point-to-point distance
double dist_pidx2pidx(int,int);							//Calculate point-to-point distance by node id
double dist_p2Line(Vec ,Vec ,Vec );						//Vector method calculates the distance from the point to the line segment
Vec Vec_p2Line(Vec ,Vec ,Vec );							//The vector method calculates the closest point of the point to the line segment. Returns the nearest point coordinates.
double dist_p2LineList(Vec,vector<line> &);				//Calculate the distance from a point to a set of line segments
int findLineIdxInTotaLineList(line );					//Find the position of an edge in the total_LineList
double dist_inside_p2Line_inside_ploygon(Vec,line); 	//Calculate the distance from the point inside the polygon to the line segment of the polygon. This distance is the internal distance.
double dist_inside_p2LineList(Vec ,vector<line> );  	//Calculate the internal distance distance from a point inside a polygon to a group line segment of a polygon

//geometric
boundry cal_line_boundry(Vec,Vec);						//Returns the boundary of a line segment
boundry cal_lineset_boundry(vector<line> );				//Returns the bounds of a set of line segments	
bool isBoxCrossBox(boundry ,boundry );					//Determine if the two boxes overlap, the tangency is overlap
bool isLineCrossLine(Vec,Vec,Vec,Vec);					//Determine whether two line segments intersect. Vector method Tangent counts into intersections Partial coincidence also intersects
bool isLineCrossLineSet(Vec,Vec,vector<line>);			//Determine if the line segment intersects a set of edges
bool isLineInPloygon(Vec,Vec,vector<line>);				//Determine whether the line segment is inside the polygon. The line segment verified by the midpoint and pass the disjoint verification 
GLine makeGLine(Vec ,Vec );								//According to the known two-point coordinates, the straight line analytical equations of these two points are obtained: a*x+b*y+c = 0  (a >= 0) 
Vec lineintersect(GLine ,GLine );						//If two lines l1 (a1*x+b1*y+c1 = 0) and l2(a2*x+b2*y+c2 = 0) intersect, return the intersection point p. If parallel, it will return an invalid value.
bool SegIntersectSeg(Vec,Vec,Vec,Vec,Vec &);			//If the line segments l1 and l2 intersect, return true and the intersection is returned by (inter), otherwise return false
bool isPointInBoundry(Vec,boundry );					//Determine if the point is inside a boundry boundary 
bool isPointOnLine(Vec ,line );							//Determine whether the point is on the line segment
bool isPointInPloygon(Vec, vector<line>);				//Determine if the point is inside the polygon, including the vertex and the point on the boundary
bool isRayIntersectsSegment(Vec,Vec,Vec);				//Judging whether the horizontal ray passing through a point has an intersection with the line segment. // Judging point, starting point, ending point

//loadFile
void loadFile();										//Load file
void parseFile(string);									//Parse file
void calAreaPerimeter();								//Calculate polygon area and perimeter
void iniNodeId(vector<Node> &);							//Initialize polygon node id number

//shrink
void shrinkOri(int effort);								//Processing original polygons 

//delaunay
void delaunay();										//Triangulation of polygons
void inishnOriNode();									//Initialization shOriNode								
void iniNode(int ,vector<Node> &);						//Initialization a Node
void increshnOri(double );								//Increase shnOri
void iniL1L2L3(vector<Node> &);							//Initialization L1 L2 L3
void cutTriangle(vector<Node> &);						//Cut a Triangle from ploygon
void updateL1L2L3(vector<Node> &);						//Update L1 L2 L3 after cut a triangle
void updateEdge(int ,int );								//Update edge after cut a triangle
void updateMap(int ,int ,int );							//Update map after cut a triangle
void updateNodeConvex1(vector<Node> &,int );			//Update Convex1 Node
void updateNodeConvex2(vector<Node> &,int );			//Update Convex2 Node
void updateNodeGroove(vector<Node> &,int );				//Update groove Node
void removefromL2(int );								//Remove an element from L2

pl calpSide(Vec,Vec,Vec);								//Determine the positional relationship between points and line segments
bool isInTriangle(Vec,Vec,Vec,Vec);						//Determine if the point is inside the triangle, and the three points of the triangle are counterclockwise
int isPloygonEdge(int ,int );							//Determine if the edge is a polygon boundary
double calNodeWeight(vector<Node> &NL,int );			//Calculate the weight of the node
double calTriangleWeight(Vec ,Vec ,Vec );				//Calculate the weight of a triangle
double calTriangleWeightByIdx(vector<Node> &,int ,int ,int );//Calculate the weight of a triangle ,triangle vertex represent by index
tlabel calTriType(int ,int ,int );						//Calculate the type of triangle
elabel calEdgeType(int ,int );							//Calculate the type of edge					
int calKey(int ,int );									//Calculate the key value of the edge

//genCenterLine1
void iniLineList();										//Initialization edge
void genCenterLine1();									//Centerline engine with known start and end edges
Vec calStartDir(Vec );									//Calculate the starting search direction
double calNodeEvaluation(Vec p,vector<line> &,vector<line> &); //Calculate the weight of the node
dvec nextNode(dvec ,double );							//Search for the next point with method1
dvec nextNode1(dvec,double );							//Search for the next point with method2

//genCenterLine2
void genCenterLine2();									//Centerline engine with unknown start and end edges
void genSeedSeg(int );									//Generate seed segments
void expandNode(int ,int ,int ,vector<Vec> &);			//CenterLine segment growth

//genCenterLine3
void genCenterLine3();									//Centerline engine with unknown start and end edges
void iniTotalP();										//Initialize polygon vertices
void iniDcn();											//Initialization double center point
void iniClist();										//Initialize the center line segment
PP calNodePP(int );										//Calculate the angle bisector of a vertex within a polygon		
double calNW(Vec );										//Calculate the weight of a point
Vec calNearCenterNode(int );							//Calculate the center point near a vertex
double dist_Line2Line(Vec ,Vec ,Vec ,Vec );				//Calculate the distance from the line segment to the line segment
double dist_Line2LineByIdx(int ,int );					//Calculate the distance from the line segment to the line segment by index
bool isLineOvlpLine(Vec ,Vec ,Vec ,Vec );				//Determine whether the line segment overlaps with the line segment
bool isLineNearLine(Vec ,Vec ,Vec ,Vec );				//Determine whether the line segment and the line segment are adjacent
Vec calAngleBisctor(Vec ,Vec ,Vec );					//Calculating angle bisector
void genRlist();										//Remove duplicate center segments
void write2CN();										//Write centerline results to CN(Center Node)
void rfSEG();											//Refine result line segment
void genRlist1();										//Remove duplicate center segments with new method
bool crmLineOvlpLine(Vec,Vec,Vec &,Vec &);				//Parallel segments remove overlapping parts
void show_totalP();										//Print totalP (total ploygon vertex)
void show_dcn();										//Print dcn (center node)
void show_clist();										//Print clist (center line list)
void show_rlist();										//Print rlist (remained center line list)
void show_clistID();									//Print clist ID
void initotalP();

//showIniInfo
void show_vec(Vec &);									//Print a Vec
void show_node(Node &);									//Print a Node
void show_triangle(Triangle );							//Print a Triangle
void show_trianglelist(vector<Triangle> t);				//Print a Triangle list
void show_vector(vector<Vec> &,string );				//Print vector of Vec
void show_vector(vector<Node> &,string );				//Print vector of Node
void show_vector(vector<int> &,string );				//Print vector of int
void show_set(set<int> &,string );						//Print set of int
void show_set(multiset<int,cmpW_enOri> &,string );		//Print multiset of int
void show_map(map<int,int> &,string );					//Print map of <int,int>
void show_map(map<int,Edge> &,string );					//Print map of <int,Edge>
void show_CN();											//Print CN(Center Node)
void show_CNseg(int ,int );								//Print a part of CN(Center Node)
void showIniInfo();										//Print Initialization information
void show_linelist(vector<line> &,string );				//Print line list

//dump
void dumpTriList();										//Write Triangle list to file
void dumpCN(int);										//Write CN(Center Node) to file
void dumpenOri();										//Write enOri boundary info to file
void dumpshOri();										//Write shOri boundary info to file

//lbz
struct LLength{double a;int b;};
extern vector<LLength> LineLength;
extern vector<Vec> Ori2;  
extern vector<Vec> Ori3;  
extern vector<Vec> centerNode;  
extern double x_min;       								//Ploygon boundary 
extern double x_max;
extern double y_min;
extern double y_max; 
double arc_calc(Vec a,Vec b);
bool arc_equ(double a,double b,double pre);
double arc_dis(double aa,double bb);
void dump2file(vector<Vec> NodeSet,string fileName);	//Write a vector<Vec> to a specific file
bool equ(double a,double b);
void rectonly(int effort);
void decprec(Vec &p);
bool inshotline(Vec p1,Vec p2,Vec p3);
void refineCN(int effort);
Vec getlineprojection(Vec P, Vec A, Vec B);

#endif
