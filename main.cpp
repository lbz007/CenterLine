#include "src/global.h"
#include <vector>
#include <iostream>

using namespace std;

string rootDir; //exe�ļ����ڸ�Ŀ¼
string mapFile; //������ļ�������׺ 
string pureName; //������ļ��� ������׺ 
//�㷨���ݳ�ʼ��ȫ�ֱ��� 
int Node_num=0;       	//����νڵ���
int Node_num2=0;       	//����νڵ���
int recNode_num=0;
vector<pos> NodeList;      	//����νڵ�����
vector<pos> NodeList2;      	//����νڵ�����
vector<pos> recNode;      	//����νڵ�����
int Edge_num=0;       	//����α���
vector<line> total_LineList;   //�ܶ���α�����
line start_Edge;        //��ʼ��
line end_Edge;          //��ֹ��
int LineList1_num=0;      //��һ�����Ŀ 
vector<line> LineList1;        //��һ��� 
int LineList2_num=0;      //�ڶ������Ŀ 
vector<line> LineList2;        //�ڶ���� 

double x_min=10000;       	//����α߽� 
double x_max=-10000;
double y_min=10000;
double y_max=-10000; 
 
int centerNode_num=0;     //�����е����Ŀ 
vector<pos> centerNode;        //���ɵ��е� 
vector<double> evaluation;   //����ֵ 
vector<LLength> LineLength;

double precision=PRECISION;   //������

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


