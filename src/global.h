#ifndef _GLOBAL_
#define _GLOBAL_ 
#include <string>
#include <vector>
#define PRECISION 0.5 //���� 
#define PI 3.141592653589793
using namespace std;

//�ṹ�� 
struct pos{double x,y;}; //���� 
struct dpos{pos p;pos v;};//�����н�����ĵ� �����ʾΪ��λ���� 
struct line{int s,e;};   //�� 
struct lin{double a;double b;double c;};
struct LLength{double a;int b;};

//ϵͳȫ�ֱ��� 
extern string rootDir; //exe�ļ����ڸ�Ŀ¼
extern string mapFile; //������ļ��� 
extern string pureName; //������ļ��� ������׺
//�㷨���ݳ�ʼ��ȫ�ֱ��� 
extern int Node_num;       	//����νڵ���
extern int Node_num2;       	//����νڵ���
extern int recNode_num;
extern vector<pos> NodeList;      	//����νڵ�����
extern vector<pos> NodeList2;      	//����νڵ�����
extern int Edge_num;       	//����α���
extern vector<line> total_LineList;   //�ܶ���α�����
extern line start_Edge;        //��ʼ��
extern line end_Edge;          //��ֹ��
extern int LineList1_num;      //��һ�����Ŀ 
extern vector<line> LineList1;        //��һ��� 
extern int LineList2_num;      //�ڶ������Ŀ 
extern vector<line> LineList2;        //�ڶ���� 
extern vector<LLength> LineLength;

extern double x_min;       	//����α߽� 
extern double x_max;
extern double y_min;
extern double y_max; 
 
extern int centerNode_num;     //�����е����Ŀ 
extern vector<pos> centerNode;        //���ɵ��е� 
extern vector<double> evaluation; //����ֵ 

extern vector<pos> recNode;      	//����νڵ�����
extern double precision;   //������

//��������
void ini_variables();   //��ʼ��ȫ�ֱ��� 
void show_ini_info();         //��ʾ��ʼ����Ϣ
void initialization();        //��ʼ�� 
void ini_LineList1_and_LineList2(); //��ʼ������� 
bool isPointInPloygon(pos, vector<line> );//�жϵ��Ƿ��ڶ������
bool isRayIntersectsSegment(pos, pos, pos);//�ж�ͨ��һ���ˮƽ���ߣ����߶��Ƿ��н���
double cal_Node_evaluation(pos,vector<line> ,vector<line> );//����ڵ�Ĵ���ֵ 
void gen_centerNode();//�����е�ķ���
void dump_result(); //������ 
void dump_debug_file(vector<pos>,string );//���debug��Ϣ
void dump2file(vector<pos> NodeSet,string fileName);
void ini_precision(); //���þ��� 
void in_mapfile();//�����ļ� 
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
