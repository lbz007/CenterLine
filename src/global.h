#ifndef _GLOBAL_
#define _GLOBAL_ 
#include <string>
#include <vector>
#define PRECISION 0.5 //精度 
#define PI 3.141592653589793
using namespace std;

//结构体 
struct pos{double x,y;}; //坐标 
struct dpos{pos p;pos v;};//带点行进方向的点 方向表示为单位向量 
struct line{int s,e;};   //边 
struct lin{double a;double b;double c;};
struct LLength{double a;int b;};

//系统全局变量 
extern string rootDir; //exe文件所在根目录
extern string mapFile; //输入的文件名 
extern string pureName; //输入的文件名 不带后缀
//算法数据初始化全局变量 
extern int Node_num;       	//多边形节点数
extern int Node_num2;       	//多边形节点数
extern int recNode_num;
extern vector<pos> NodeList;      	//多边形节点坐标
extern vector<pos> NodeList2;      	//多边形节点坐标
extern int Edge_num;       	//多边形边数
extern vector<line> total_LineList;   //总多边形边链表
extern line start_Edge;        //起始边
extern line end_Edge;          //终止边
extern int LineList1_num;      //第一组边数目 
extern vector<line> LineList1;        //第一组边 
extern int LineList2_num;      //第二组边数目 
extern vector<line> LineList2;        //第二组边 
extern vector<LLength> LineLength;

extern double x_min;       	//多边形边界 
extern double x_max;
extern double y_min;
extern double y_max; 
 
extern int centerNode_num;     //生成中点的数目 
extern vector<pos> centerNode;        //生成的中点 
extern vector<double> evaluation; //代价值 

extern vector<pos> recNode;      	//多边形节点坐标
extern double precision;   //网格间距

//函数声明
void ini_variables();   //初始化全局变量 
void show_ini_info();         //显示初始化信息
void initialization();        //初始化 
void ini_LineList1_and_LineList2(); //初始化两组边 
bool isPointInPloygon(pos, vector<line> );//判断点是否在多边形内
bool isRayIntersectsSegment(pos, pos, pos);//判断通过一点的水平射线，与线段是否有交点
double cal_Node_evaluation(pos,vector<line> ,vector<line> );//计算节点的代价值 
void gen_centerNode();//生成中点的方法
void dump_result(); //输出结果 
void dump_debug_file(vector<pos>,string );//输出debug信息
void dump2file(vector<pos> NodeSet,string fileName);
void ini_precision(); //设置精度 
void in_mapfile();//输入文件 
double dist_p2LineList(pos p,vector<line> LineSet);
double dist_p2Line(pos p,pos s,pos e);
double dist_p2p(pos p1,pos p2);
void swap(double &a, double &b);
lin twopoint(pos p1,pos p2);
pos twoline(lin A,lin B);
void refine_centerNode();
void refine_boundary();
void rectonly();
double calcxl(pos a,pos b);

#endif
