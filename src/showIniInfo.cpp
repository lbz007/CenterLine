#include "global.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

//Print Initialization information
void showIniInfo(){
	cout<<endl;
	cout<<"Initialization Infomation..."<<endl;
	cout<<"OriNodeNum:"<<OriNodeNum<<endl;
	if(edgeFlag){
		cout<<"startEdge:"<<startEdge.s<<"---"<<startEdge.e<<"   endEdge:"<<endEdge.s<<"---"<<endEdge.e<<endl;
	}
	else{
		cout<<"No start and end edge"<<endl;
	}
	if(clockwiseFlag){
		cout<<"Counterclockwise "<<endl;
	}
	else{
		cout<<"Clockwise "<<endl;
	}
	cout<<"Area："<<PloygonArea<<" perimeter："<<PloygonPerimeter<<"估计线长: "<<Length<<" 估计宽度"<<Width<<endl; 
	cout<<endl<<"Node Infomation..."<<endl;
    show_vector(Ori,"Ori");
}  

//Print a Vec
void show_vec(Vec &v){
	cout<<"Vec: ("<<v.x<<","<<v.y<<")"<<endl; 
}     

//Print a Node
void show_node(Node &n){
	cout<<"Node id:"<<n.id<<" Pos: ("<<n.x<<","<<n.y<<")"<<" Weight:"<<n.w<<"  "<<acos(n.w)/PI*180<<"度"<<" pre:"<<n.pre<<" next:"<<n.next;
	switch(n.type){
		case(notinied):{
			cout<<"   notinied"<<endl;
			break;
		}
		case(convex):{
			cout<<"   convex"<<endl;
			break;
		}
		case(convex1):{
			cout<<"   convex1"<<endl;
			break;
		}
		case(convex2):{
			cout<<"   convex2"<<endl;
			break;
		}
		case(groove):{
			cout<<"   groove"<<endl;
			break;
		}
		default:{
			cout<<"   notinied"<<endl;
			break;
		}
	}	
}

//Print a Triangle
void show_triangle(Triangle t){
	cout<<"Tri "<<"id:"<<t.id<<" ("<<t.a<<","<<t.b<<","<<t.c<<")";
	switch(t.type){
		case(I):{
			cout<<" type: I"<<endl;
			break;
		}
		case(II):{
			cout<<" type: II"<<endl;
			break;
		}
		case(III):{
			cout<<" type: III"<<endl;
			break;
		}
	}
}

//Print a Triangle list
void show_trianglelist(vector<Triangle> tl){
	for(int i=0;i<tl.size();i++){
		show_triangle(tl[i]);
	}
}

//Print vector of Vec
void show_vector(vector<Vec> &v,string s){
	int id=0;
	cout<<s<<" size:"<<v.size()<<endl;
	for(auto it=v.begin();it!=v.end();it++){
		cout<<s<<"["<<id++<<"]:"<<(*it).x<<","<<(*it).y<<endl;
	}
	cout<<endl;
}

//Print vector of Node
void show_vector(vector<Node> &v,string s){
	int id=0;
	cout<<s<<" size:"<<v.size()<<endl;
	for(auto it=v.begin();it!=v.end();it++){
		cout<<s<<"["<<id++<<"]:";
		show_node((*it));
	}
	cout<<endl;
}

//Print vector of int
void show_vector(vector<int> &v,string s){
	int id=0;
	cout<<s<<" size:"<<v.size()<<endl;
	for(auto it=v.begin();it!=v.end();it++){
		cout<<s<<"["<<id++<<"]:"<<(*it)<<endl;
	}
	cout<<endl;
}

//Print set of int
void show_set(set<int> &s,string st){
	int id=0;
	cout<<st<<" size:"<<s.size()<<endl;
	for(auto it=s.begin();it!=s.end();it++){
		cout<<st<<"["<<id++<<"]:"<<(*it);
		show_node(enOri[(*it)]);
	}
	cout<<endl;
}

//Print multiset of int
void show_set(multiset<int,cmpW_enOri> &s,string st){
	int id=0;
	cout<<st<<" size:"<<s.size()<<endl;
	for(auto it=s.begin();it!=s.end();it++){
		cout<<st<<"["<<id++<<"]:"<<(*it);
		show_node(enOri[(*it)]);
	}
	cout<<endl;
}

//Print map of <int,int>
void show_map(map<int,Edge> &m,string s){
	auto it=m.begin();
	while(it!=m.end()){
		cout<<s<<" key:"<<(*it).first<<" Edge:"<<(*it).second.s<<"->"<<(*it).second.e;
		switch((*it).second.type){
			case(boundary):{
				cout<<" type: boundary"<<endl; 
				break;
			}
			case(reuse):{
				cout<<" type: reuse"<<endl; 
				break;
			}
		}
		it++;
	}
}

//Print map of <int,Edge>
void show_map(map<int,int> &m,string s){
	auto it=m.begin();
	while(it!=m.end()){
		cout<<s<<" key:"<<(*it).first<<" Edge:"<<E[(*it).first].s<<"->"<<E[(*it).first].e<<" Triiid:"<<(*it).second<<endl;
		//show_triangle(TML[(*it).second]);
		it++;
	}
}

//Print a part of CN(Center Node)
void show_CNseg(int l,int r){
	for(int i=l;i<r;i++){
		cout<<"CN["<<i<<"]:"<<"( "<<CN[i].x<<","<<CN[i].y<<" )"<<endl;
	}
	cout<<endl;
}

//Print CN(Center Node)
void show_CN(){
	int num=0;
	int segnum=SEGL.size()/2;
	cout<<endl<<"total seg num:"<<segnum<<endl;
	for(int i=0;i<segnum;i++){
		cout<<"seg "<<num++<<":"<<endl;
		show_CNseg(SEGL[2*i],SEGL[2*i+1]);
	} 
}

//Print line list
void show_linelist(vector<line> &l,string s){
	int id=0;
	cout<<s<<" size:"<<l.size()<<endl;
	for(auto it=l.begin();it!=l.end();it++){
		cout<<s<<"["<<id++<<"]:"<<(*it).s<<"->"<<(*it).e<<endl;
	}
	cout<<endl;
} 
