#include "global.h"
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

void initialization(){
	in_mapfile();
	ini_precision();
	cout<<"Input file:"<<mapFile<<"  Precision:"<<precision<<endl;
	cout<<setw(60)<<setfill('.')<<left<<"Begin initialization"<<endl;
	ini_variables();
	ini_LineList1_and_LineList2();
	cout<<setw(60)<<setfill('.')<<left<<"initialization finished"<<endl;
}

void ini_variables(){
	//读文件
	cout<<setw(50)<<setfill('.')<<left<<"Begin ini_variables"<<endl;
	string line;
	ifstream fi(mapFile,std::ios::in);
	while(getline(fi,line)){
		smatch matchResult;
		regex pattern1("V\\s\\d+\\s(-?\\d+(.\\d+)?)\\s+(-?\\d+(.\\d+)?)\\s*");
		regex pattern2("E1\\s(\\d+)\\s(\\d+)\\s*");
		regex pattern3("E2\\s(\\d+)\\s(\\d+)\\s*");
		
		if (regex_match(line, matchResult, pattern1))
		{
			cout << "Find Node:" << line << endl;
			double temp_x,temp_y;
			temp_x = atof(matchResult[1].str().c_str());
			temp_y = atof(matchResult[3].str().c_str());
			NodeList.push_back({temp_x,temp_y});
			if(temp_x<x_min) x_min=temp_x;
			if(temp_x>x_max) x_max=temp_x;
			if(temp_y<y_min) y_min=temp_y;
			if(temp_y>y_max) y_max=temp_y;
		}
		else if (regex_match(line, matchResult, pattern2))
		{
			cout << "Find Start Edge:" << line << endl;
			start_Edge = {atoi(matchResult[1].str().c_str()),atoi(matchResult[2].str().c_str())};
		}
		else if (regex_match(line, matchResult, pattern3))
		{
			cout << "Find  End  Edge:" << line << endl;
			end_Edge = {atoi(matchResult[1].str().c_str()),atoi(matchResult[2].str().c_str())};
		}
	}	
	fi.close();
	Node_num = NodeList.size();
	Edge_num = Node_num;   
	cout<<setw(50)<<setfill('.')<<left<<"ini_variables finished"<<endl;
	
	ofstream fo(pureName+"_debug_file.txt",ios::out);
	fo.close();
}

void swap(int &a,int &b){
	int temp=a;a=b;b=temp;
}

void ini_LineList1_and_LineList2(){
	cout<<setw(50)<<setfill('.')<<left<<"Begin ini_LineList1_and_LineList2"<<endl;
	int index_min;//将索引分为小索引和大索引
	int index_max;
	//初始化总边链表
	for (int i=0;i < Edge_num;i++){
		if (i<(Edge_num-1)){
			total_LineList.push_back({i,i+1});
		}
		if (i==(Edge_num-1)){
			total_LineList.push_back({i,0});
		}
	}
	//查找起始边和终止边的位置
	for (int i=0;i<Edge_num;i++){
		if (((start_Edge.s == total_LineList[i].s) & (start_Edge.e == total_LineList[i].e)) | ((start_Edge.s == total_LineList[i].e) & (start_Edge.e == total_LineList[i].s))){
			index_min = i;
		}
		if (((end_Edge.s == total_LineList[i].s) & (end_Edge.e == total_LineList[i].e)) | ((end_Edge.s == total_LineList[i].e) & (end_Edge.e == total_LineList[i].s))){
			index_max = i;
		}
	}
	if(index_min>index_max)
		swap(index_min,index_max);
	
	//根据索引划分LineList1和LineList2,对LineList1进行反向排序（该操作对棋盘法无意义，对连线法需进行此操作）
	LineList1_num = index_max - index_min - 1;
	LineList2_num = Edge_num - 2 - LineList1_num;
	for (int i = 0; i < LineList1_num; i++){
		LineList1.push_back({total_LineList[index_max - i - 1].e,total_LineList[index_max - i - 1].s});
	}
	for (int i = 0; i < LineList2_num; i++){
		if ((index_max + i + 1) <= (Edge_num - 1)){
			LineList2.push_back({total_LineList[index_max + i + 1].s,total_LineList[index_max + i + 1].e});
		}
		else{
			LineList2.push_back({total_LineList[index_max + i + 1 - Edge_num].s,total_LineList[index_max + i + 1 - Edge_num].e});
		}
	}
	cout<<setw(50)<<setfill('.')<<left<<"ini_LineList1_and_LineList2 finished"<<endl;
}

void ini_precision(){
	cout<<"Please input precision:";
	cin>>precision;
}

void in_mapfile(){
	cout<<"Please input file:";
	cin>>mapFile;
	pureName = mapFile.substr(0, mapFile.rfind("."));
} 
