#include "src/global.h"
#include <vector>
#include <iostream>

using namespace std;

string rootDir; //exe文件所在根目录
string mapFile; //输入的文件名带后缀 
string pureName; //输入的文件名 不带后缀 
//算法数据初始化全局变量 
int Node_num=0;       	//多边形节点数
int Node_num2=0;       	//多边形节点数
int recNode_num=0;
vector<pos> NodeList;      	//多边形节点坐标
vector<pos> NodeList2;      	//多边形节点坐标
vector<pos> recNode;      	//多边形节点坐标
int Edge_num=0;       	//多边形边数
vector<line> total_LineList;   //总多边形边链表
line start_Edge;        //起始边
line end_Edge;          //终止边
int LineList1_num=0;      //第一组边数目 
vector<line> LineList1;        //第一组边 
int LineList2_num=0;      //第二组边数目 
vector<line> LineList2;        //第二组边 

double x_min=10000;       	//多边形边界 
double x_max=-10000;
double y_min=10000;
double y_max=-10000; 
 
int centerNode_num=0;     //生成中点的数目 
vector<pos> centerNode;        //生成的中点 
vector<double> evaluation;   //代价值 
vector<LLength> LineLength;

double precision=PRECISION;   //网格间距

int main(){
	initialization();
	show_ini_info();
    refine_boundary();
	gen_centerNode(); 
    refine_centerNode();
	dump_result();
    rectonly();
	system("pause");
	return 0;
}


