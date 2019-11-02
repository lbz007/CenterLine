#include <iostream>
#include "global.h"
#include <iomanip>
#include <cstdio>
#include <algorithm>
using namespace std;

void show_ini_info(){
	cout<<endl;
	cout<<"Initialization Infomation..."<<endl;
	cout<<"Node_num:"<<Node_num<<"  Edge_num:"<<Edge_num<<endl;
	cout<<"Start_Edge:"<<start_Edge.s<<"---"<<start_Edge.e<<"   End_Edge:"<<end_Edge.s<<"---"<<end_Edge.e<<endl;
	cout<<"x_min:"<<x_min<<",  x_max:"<<x_max<<endl;
	cout<<"y_min:"<<y_min<<",  y_max:"<<y_max<<endl;
	cout<<"precision:"<<precision<<endl;
	cout<<endl<<"Node Infomation..."<<endl;
	for (int i = 0; i < Node_num; i++){
		cout << "NodeList[" << i << "]:" << "(" << NodeList[i].x << "," << NodeList[i].y << ")" << endl;
	}

    /* freopen("NodeList.txt","w",stdout); */
    /* for (int i = 0; i < Node_num; i++){ */
		/* cout << NodeList[i].x << " " << NodeList[i].y << endl; */
    /* } */
    /* fclose(stdout); */
    

	cout<<endl << "Edge Infomation..." << endl;
	for (int i = 0; i < Edge_num; i++){
		cout << "total_LineList[" << i << "]:" << total_LineList[i].s << "->" << total_LineList[i].e << endl;
	}
	cout<<endl<<"LineList1 Infomation..."<<endl;
	cout<<"LineList1_num:"<<LineList1_num<<endl;
	for (int i = 0; i < LineList1_num; i++){
		cout << "LineList1[" << i << "]:" << LineList1[i].s << "->" << LineList1[i].e << endl;
	}
	cout<<endl<<"LineList2 Infomation..."<<endl;
	cout<<"LineList2_num:"<<LineList2_num<<endl;
	for (int i = 0; i < LineList2_num; i++){
		cout << "LineList2[" << i << "]:" << LineList2[i].s << "->" << LineList2[i].e << endl;
	}
}       
