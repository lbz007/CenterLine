#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "global.h"

void show_result_info();

void dump2file(vector<pos> NodeSet,string fileName){
	ofstream fo(fileName,ios::out);
	 if(!fo){
	 	cout<<"Open Failed..."<<endl;
	 	}
	for(int i=0;i<NodeSet.size();i++){
	 	fo<<setiosflags(ios::showpoint)<<setprecision(16)<<NodeSet[i].x<<" "<<NodeSet[i].y<<endl;
		}
	fo.close();
	cout<<endl<<fileName<<"  output finished."<<endl;
}

void dump_debug_file(vector<pos> NodeSet,string fileName){
	ofstream fo(fileName,ios::app);
	 if(!fo){
	 	cout<<"Open Failed..."<<endl;
	 	}
	fo<<"////"<<endl;
	for(int i=0;i<NodeSet.size();i++){
	 	fo<<setiosflags(ios::showpoint)<<setprecision(6)<<NodeSet[i].x<<" "<<NodeSet[i].y<<endl;
		}
	fo.close();
}

void dump_result(){
	//r表示 输出矩形点文件
	//c表示 输出中点文件
	//i表示 输出内部点文件
	
	string centerNodefileName = "_centerNode.txt";
	
	centerNodefileName = pureName + centerNodefileName;
	
	dump2file(centerNode,centerNodefileName);
	show_result_info();
}

void show_result_info(){
	cout<<endl<<"Result Infomation..."<<endl;
	cout<<"centerNode_num:"<<centerNode_num<<endl;
	cout<<"centerNode:"<<endl;
	for(int i=0;i<centerNode.size();i++){
		cout<<"ID:"<<i<<" ->  "<<centerNode[i].x<<"  "<<centerNode[i].y<<endl;
		//cout<<"ID:"<<i<<" ->  "<<centerNode[i].x<<"  "<<centerNode[i].y<<"  "<<evaluation<<endl;
	}
} 


