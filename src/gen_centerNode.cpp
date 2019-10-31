#include "global.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
//template<typename T>
//void swap(T &a, T &b){
//    T temp = a; a = b; b = temp;
//}
void swap(double &a, double &b){
    double temp = a; a = b; b = temp;
}

void swap1(pos &a, pos &b){
	pos temp=a;a=b;b=temp;
}

//计算点到点的距离
double dist_p2p(pos p1,pos p2){
	return sqrt((pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2)));
}

//通过节点序号计算节点距离 
double dist_pidx2pidx(int idx1,int idx2){
	return dist_p2p(NodeList[idx1],NodeList[idx2]);
}

//计算点到线段的距离 
double dist_p2Line(pos p,pos s,pos e){
	double xmin=s.x, xmax=e.x, ymin=s.y, ymax=e.y;
	if(xmin>xmax) swap(xmin,xmax);
	if(ymin>ymax) swap(ymin,ymax);
	pos insect;
	
	if((s.x==e.x)&(s.y==e.y)){
		//线段为点 
		return dist_p2p(p,s);
	} 
	else if(s.x==e.x){            //线段为垂直线段
		if((p.y>ymin)&(p.y<ymax)) //垂交点在线段上 
			return fabs(p.x-s.x);
		else                      //垂交点不在线段上 
			return min(dist_p2p(p,s),dist_p2p(p,e));
	} 
	else if(s.y==e.y){            //线段为水平线段 
		if((p.x>xmin)&(p.x<xmax)) //垂交点在线段上
			return fabs(p.y-s.y);
		else                      //垂交点不在线段上
			return min(dist_p2p(p,s),dist_p2p(p,e));
	}
	else{                        //一般斜线 
		double k,k_;
		k = (e.y-s.y)/(e.x-s.x);
		k_= -1/k;
		double b,b_;
		b = s.y - k*s.x;
		b_= p.y - k_*p.x;
		
		insect = {(b - b_)/(k_ - k),(b*k_ - b_*k) / (k_ - k)};
		
		if((insect.x>xmin)&(insect.x<xmax)) //垂交点在线段上
			{
			return dist_p2p(p,insect);}
		else                                //垂交点不在线段上
		    {
			return min(dist_p2p(p,s),dist_p2p(p,e));}	
	}	
}

//计算点到一组线段的距离 
double dist_p2LineList(pos p,vector<line> LineSet){
	double min_dist=10000;
	double dist;
	for(int i=0;i<LineSet.size();i++){
		dist = dist_p2Line(p,NodeList[LineSet[i].s],NodeList[LineSet[i].e]);
		if(dist<min_dist)
			min_dist = dist;
	}
	return min_dist;
}

//生成下个点
dpos nextNode(dpos p,double Space,vector<pos> &NodeSet){
	dpos nextNode;
	NodeSet.resize(0);
	
	pos tmp;
	double theta0;
	pos v_dir =  p.v;                   
	if(v_dir.y>=0){
		theta0 = acos(v_dir.x);
	}
	else{
		theta0 = 2*PI-acos(v_dir.x);
	}
	
	double theta[11]={-0.78539816, -0.62831853, -0.4712389 , -0.31415927, -0.15707963,
        0.        ,  0.15707963,  0.31415927,  0.4712389 ,  0.62831853, 0.78539816};
    for(int i=0;i<11;i++){
    	tmp = {p.p.x+precision*cos(theta[i]+theta0),p.p.y+precision*sin(theta[i]+theta0)};
    	if(isPointInPloygon(tmp, total_LineList))
    		NodeSet.push_back(tmp);
	}
	
	double eva;
	double eva_min = 10000;
	for(int i=0;i<NodeSet.size();i++){
		eva = cal_Node_evaluation(NodeSet[i],LineList1,LineList2);
		if(eva<eva_min){
			nextNode = {NodeSet[i],{cos(theta[i]+theta0),sin(theta[i]+theta0)}};
			eva_min = eva;
		}
	}
	dump_debug_file(NodeSet,pureName+"_debug_file.txt");
//	cout<<"点为："<<nextNode.p.x<<","<<nextNode.p.y<<endl; 
//	cout<<"点的方向为："<<nextNode.dir<<endl; 
	return nextNode;
}

double cal_Node_evaluation(pos p,vector<line> L1,vector<line> L2){
	double p2L1,p2L2;
	p2L1=dist_p2LineList(p,L1);
	p2L2=dist_p2LineList(p,L2);
	return fabs(p2L1 - p2L2);	
} 

//给线段的一点添加左右邻点
void add_rl_Node(pos p,line l,double step,pos &node1,pos& node2){

	double sx,sy,ex,ey;
	double k,theta;
	sx = NodeList[l.s].x;
	sy = NodeList[l.s].y;
	ex = NodeList[l.e].x;
	ey = NodeList[l.e].y;
	k = (ey-sy)/(ex-sx);
	theta = atan(k);
	if(sx==ex){
		node1 = {sx,p.y-step};
		node2 = {sx,p.y+step};	
	}
	else if(sy==ey){
		node1 = {p.x-step,sy};
		node2 = {p.x+step,sy};
	}
	else{
		node1 = {p.x+step*cos(theta),p.y+step*sin(theta)};
		node2 = {p.x-step*cos(theta),p.y-step*sin(theta)};
	}
} 

//计算起始Node 
pos cal_edge_center_Node(line l){
	double n1x,n1y,n2x,n2y;
	n1x = NodeList[l.s].x;
	n1y = NodeList[l.s].y;
	n2x = NodeList[l.e].x;
	n2y = NodeList[l.e].y;
	pos node1,node2;
	pos temp;
	double step,eva,eva1,eva2;
	step = dist_pidx2pidx(l.s,l.e)/4;
	pos startNode = {(n1x+n2x)/2,(n1y+n2y)/2};
	eva = cal_Node_evaluation(startNode,LineList1,LineList2);
	if(eva<precision){
		return startNode;
	}
	else{
		add_rl_Node(startNode,l,step,node1,node2);
		bool if_precision = false;
		while(!if_precision){
			eva1 = cal_Node_evaluation(node1,LineList1,LineList2);
			eva2 = cal_Node_evaluation(node2,LineList1,LineList2);
			if(eva1<eva2){
				if(eva1<precision){
					startNode = node1;
					if_precision = true;
				}
				else{
					step = step/2;
					temp = node1;
					add_rl_Node(temp,l,step,node1,node2);
				}
			}
			else{
				if(eva2<precision){
					startNode = node2;
					if_precision = true;
				}
				else{
					step = step/2;
					temp = node2;
					add_rl_Node(temp,l,step,node1,node2);
				}
			}
		}
		return startNode;
	}	
}
	
//计算startNode方向
pos  cal_dir_of_startNode(pos &startNode){
	pos n1,n2,tmp;
	double k,k_,theta;
	n1 = NodeList[start_Edge.s];
	n2 = NodeList[start_Edge.e];
	
	if(n1.x==n2.x){
		tmp = {startNode.x+precision,startNode.y};
		if(isPointInPloygon(tmp, total_LineList))
			return{1,0};
		else
			return{-1,0};
	}
	else if(n1.y==n2.y){
		tmp = {startNode.x,startNode.y+precision};
		if(isPointInPloygon(tmp, total_LineList))
			return{0,1};
		else
			return{0,-1};
	}
	else{
		k = (n1.y-n2.y)/(n1.x-n2.x);
	    k_=-1/k;
	    theta = atan(k_);
	    tmp = {startNode.x+precision*cos(theta),startNode.y+precision*sin(theta)};
	    if(isPointInPloygon(tmp, total_LineList))
	    	return{cos(theta),sin(theta)};
	    else 
	    	return{-cos(theta),-sin(theta)};
	}
	}

//生成中点的方法 
void gen_centerNode(){
	pos startNode,endNode;
	startNode = cal_edge_center_Node(start_Edge);
	cout<<endl<<"Center node on start_edge is:("<<startNode.x<<", "<<startNode.y<<")"<<endl;
	endNode = cal_edge_center_Node(end_Edge);
	cout<<"Center node on end_edge is:("<<endNode.x<<", "<<endNode.y<<")"<<endl;
	
	//计算startNode方向
	cout<<"测试cal_dir_of_startNode:"<<endl;
	pos tmp;
	tmp = cal_dir_of_startNode(startNode);
	cout<<"搜索方向向量：("<<tmp.x<<","<<tmp.y<<")"<<endl;
	
	dpos current_node{startNode,cal_dir_of_startNode(startNode)};
	vector<pos> Candidate_Node;//存放当前点的候选邻点 
	
	bool stop_flag = false;
	double dist_current_node2end_edge;
	
	centerNode.push_back(startNode);
	while(!stop_flag){
		current_node = nextNode(current_node,precision,Candidate_Node);
		centerNode.push_back(current_node.p);
		dist_current_node2end_edge = dist_p2Line(current_node.p,NodeList[end_Edge.s],NodeList[end_Edge.e]);
		
		if(dist_current_node2end_edge<(1.1*precision)){
			stop_flag = true;
		}	
	}
	centerNode.push_back(endNode);	
	
	centerNode_num = centerNode.size();
}
