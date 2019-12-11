#include "global.h"
#include <cmath>
using namespace std;

double Herfen(double hh,double tt,double nowy) //Binary search in horizontal direction 
{
	double d0,d1,mid;
	int dir;
	Vec pp,ph,pt;
	ph.x=hh;ph.y=nowy;
	pt.x=tt;pt.y=nowy;
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(pt,LineList1);
	if (d0<0)
		dir=1;
	else
		dir=-1;
	while (tt-hh>=prec/10000000)
	{
		mid=(hh+tt)/2;
		pp.x=mid;pp.y=nowy;
		d0=dist_p2LineList(pp,LineList1);
		d1=dist_p2LineList(pp,LineList2);
		if (equ(d1,d0)==1) return mid;
		if (dir==1)
			if (d0>d1)	tt=mid;
			else hh=mid;
		else
			if (d0<d1)	tt=mid;
			else hh=mid;		
	}
	return mid;
}

double Verfen(double hh,double tt,double nowx) //Binary search in vertical direction 
{
	double d0,d1,mid;
	int dir;
	Vec pp,ph,pt;
	ph.x=nowx;ph.y=hh;
	pt.x=nowx;pt.y=tt;
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(pt,LineList1);
	if (d0<0)
		dir=1;
	else
		dir=-1;
	while (tt-hh>=prec/10000000)
	{
        /* printf("%f , %f\n",hh,tt); */
		mid=(hh+tt)/2;
		pp.x=nowx;pp.y=mid;
		d0=dist_p2LineList(pp,LineList1);
		d1=dist_p2LineList(pp,LineList2);
		if (equ(d1,d0)==1) return mid;
		if (dir==1)
			if (d0>d1)	tt=mid;
			else hh=mid;
		else
			if (d0<d1)	tt=mid;
			else hh=mid;		
	}
	return mid;
}

void erfen(Vec &pp,double det) //Binary search to find the accurate position of a CN
{
	Vec ph,pt;
	double d1,d0,dd1,dd2;
	ph.x=pp.x-det;ph.y=pp.y; pt.x=pp.x+det;pt.y=pp.y; 
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(ph,LineList2);
	d1=dist_p2LineList(pt,LineList1)-dist_p2LineList(pt,LineList2);
	dd1=fabs(d0-d1);
	ph.x=pp.x;ph.y=pp.y-det; pt.x=pp.x;pt.y=pp.y+det; 
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(ph,LineList2);
	d1=dist_p2LineList(pt,LineList1)-dist_p2LineList(pt,LineList2);
	dd2=fabs(d0-d1);
	/* printf("%f %f %f\n",dd1,dd2,det); */
	if (dd1>dd2)
		pp.x=Herfen(pp.x-det,pp.x+det,pp.y);
	else
		pp.y=Verfen(pp.y-det,pp.y+det,pp.x);
	return;	
}


void genCenterLine1(){
	iniLineList();
	/* show_linelist(total_LineList,"total_LineList"); */
	/* show_linelist(LineList1,"LineList1"); */
	/* show_linelist(LineList2,"LineList2"); */
	Vec startNode = {(NodeList[startEdge.s].x+NodeList[startEdge.e].x)/2,(NodeList[startEdge.s].y+NodeList[startEdge.e].y)/2};
	Vec endNode   = {(NodeList[endEdge.s].x+NodeList[endEdge.e].x)/2,(NodeList[endEdge.s].y+NodeList[endEdge.e].y)/2};
	dvec currentNode = {startNode,calStartDir(startNode)};
	bool stopFlag = false;
	double dist_currentNode2endEdge;
	double space=prec;
	
	CN.push_back(startNode);
	int id=0;
	while(!stopFlag){
		currentNode = nextNode(currentNode,space);
        erfen(currentNode.p,prec/4);
		dist_currentNode2endEdge = dist_p2Line(currentNode.p,NodeList[endEdge.s],NodeList[endEdge.e]);
		CN.push_back(currentNode.p);
		if(dist_currentNode2endEdge<(1.1*space)){
			stopFlag = true;
		}	
	}
	CN.push_back(endNode);
}

//Initialization edge
void iniLineList(){
	NodeList.assign(shOri.begin(),shOri.end());
	for(int i=0;i<NodeList.size();i++){
		total_LineList.push_back({i,i+1});
	}
	total_LineList[NodeList.size()-1].e=0;
	int index_min,index_max;
	for (int i=0;i<NodeList.size();i++){
		if (((startEdge.s == total_LineList[i].s) & (startEdge.e == total_LineList[i].e)) | ((startEdge.s == total_LineList[i].e) & (startEdge.e == total_LineList[i].s))){
			index_min = i;
		}
		if (((endEdge.s == total_LineList[i].s) & (endEdge.e == total_LineList[i].e)) | ((endEdge.s == total_LineList[i].e) & (endEdge.e == total_LineList[i].s))){
			index_max = i;
		}
	}
	if(index_min>index_max) swap(index_min,index_max);
	
	//partion total edge into LineList1 and LineList2
	int LineList1_num = index_max - index_min - 1;
	int LineList2_num = NodeList.size() - 2 - LineList1_num;
	for (int i = 0; i < LineList1_num; i++){
		LineList1.push_back({total_LineList[index_max - i - 1].e,total_LineList[index_max - i - 1].s});
	}
	for (int i = 0; i < LineList2_num; i++){
		if ((index_max + i + 1) <= (NodeList.size() - 1)){
			LineList2.push_back({total_LineList[index_max + i + 1].s,total_LineList[index_max + i + 1].e});
		}
		else{
			LineList2.push_back({total_LineList[index_max + i + 1 - NodeList.size()].s,total_LineList[index_max + i + 1 - NodeList.size()].e});
		}
	}
}

//Calculate the starting search direction
Vec calStartDir(Vec startNode){
	Vec n1,n2,tmp;
	double k,k_,theta;
	n1 = NodeList[startEdge.s];
	n2 = NodeList[startEdge.e];
	
	if(n1.x==n2.x){
		tmp = {startNode.x+prec,startNode.y};
		if(isPointInPloygon(tmp, total_LineList))
			return{1,0};
		else
			return{-1,0};
	}
	else if(n1.y==n2.y){
		tmp = {startNode.x,startNode.y+prec};
		if(isPointInPloygon(tmp, total_LineList))
			return{0,1};
		else
			return{0,-1};
	}
	else{
		k = (n1.y-n2.y)/(n1.x-n2.x);
	    k_=-1/k;
	    theta = atan(k_);
	    tmp = {startNode.x+prec*cos(theta),startNode.y+prec*sin(theta)};
	    if(isPointInPloygon(tmp, total_LineList))
	    	return{cos(theta),sin(theta)};
	    else 
	    	return{-cos(theta),-sin(theta)};
	}
}

//Search for the next point with method1
dvec nextNode(dvec p,double Space){
		dvec nextNode; 
	Vec v_dir =  p.v;  
	double theta0;                 
	if(v_dir.y>=0) theta0 = acos(v_dir.x);
	else theta0 = 2*PI-acos(v_dir.x);
    /* double theta_near_l1 = theta0 + 0.7853981633974483; */
    /* double theta_near_l2 = theta0 - 0.7853981633974483; */
    double theta_near_l1 = theta0 + PI/2;
    double theta_near_l2 = theta0 - PI/2;
	Vec p_theta_near_l1,p_theta_near_l2;//分别表示距LineList1和LineList2近的点 
	p_theta_near_l1 = {p.p.x+Space*cos(theta_near_l1),p.p.y+Space*sin(theta_near_l1)};
	p_theta_near_l2 = {p.p.x+Space*cos(theta_near_l2),p.p.y+Space*sin(theta_near_l2)};
	double p2L1 = dist_p2LineList(p_theta_near_l1,LineList1);
	double p2L2 = dist_p2LineList(p_theta_near_l2,LineList1);
	if(p2L1>p2L2) swap(theta_near_l1,theta_near_l2); 
	double dir_theta_near_l1_theta_near_l2 = theta_near_l2 - theta_near_l1; 
	double center_theta;
	Vec center_Vec;
	double deviation;
	//deviation = dist_p2p(p_theta_near_l1,p_theta_near_l2);
	double x1,x2,fx1,fx2;
	while(1){
		x1 = theta_near_l1 + 0.382*(theta_near_l2 - theta_near_l1);
		x2 = theta_near_l1 + 0.618*(theta_near_l2 - theta_near_l1);
		p_theta_near_l1 = {p.p.x+Space*cos(theta_near_l1),p.p.y+Space*sin(theta_near_l1)};
		p_theta_near_l2 = {p.p.x+Space*cos(theta_near_l2),p.p.y+Space*sin(theta_near_l2)};
		fx1 = fabs(dist_p2LineList(p_theta_near_l1,LineList1)-dist_p2LineList(p_theta_near_l1,LineList2));
		fx2 = fabs(dist_p2LineList(p_theta_near_l2,LineList1)-dist_p2LineList(p_theta_near_l2,LineList2));

		if(fx1>fx2){
			theta_near_l1 = x1;
			p_theta_near_l1 = {p.p.x+Space*cos(theta_near_l1),p.p.y+Space*sin(theta_near_l1)};
		 } 
		else{
			theta_near_l2 = x2;
			p_theta_near_l2 = {p.p.x+Space*cos(theta_near_l2),p.p.y+Space*sin(theta_near_l2)};
		} 
		deviation = dist_p2p(p_theta_near_l1,p_theta_near_l2);
		center_theta = (theta_near_l1+theta_near_l2)/2.0;
		center_Vec = {p.p.x+Space*cos(center_theta),p.p.y+Space*sin(center_theta)};;
		if(deviation<prec*0.01) return {center_Vec,{cos(center_theta),sin(center_theta)}};
	}
}

/* dvec nextNode(dvec p,double Space){ */
/* 	dvec nextNode; */ 
/* 	Vec v_dir =  p.v; */  
/* 	double theta0; */                 
/* 	if(v_dir.y>=0) theta0 = acos(v_dir.x); */
/* 	else theta0 = 2*PI-acos(v_dir.x); */
/*     double theta_near_l1 = theta0 + 0.7853981633974483; */
/*     double theta_near_l2 = theta0 - 0.7853981633974483; */
/* 	Vec p_theta_near_l1,p_theta_near_l2;//分别表示距LineList1和LineList2近的点 */ 
/* 	p_theta_near_l1 = {p.p.x+Space*cos(theta_near_l1),p.p.y+Space*sin(theta_near_l1)}; */
/* 	p_theta_near_l2 = {p.p.x+Space*cos(theta_near_l2),p.p.y+Space*sin(theta_near_l2)}; */
/* 	double p2L1 = dist_p2LineList(p_theta_near_l1,LineList1); */
/* 	double p2L2 = dist_p2LineList(p_theta_near_l2,LineList2); */
/* 	if(p2L1>p2L2) swap(theta_near_l1,theta_near_l2); */ 
/* 	double dir_theta_near_l1_theta_near_l2 = theta_near_l2 - theta_near_l1; */ 
/* 	double center_theta; */
/* 	Vec center_Vec; */
/* 	double deviation; */
/* 	while(1){ */
/* 		center_theta = (theta_near_l1+theta_near_l2)/2; */
/* 		center_Vec = {p.p.x+Space*cos(center_theta),p.p.y+Space*sin(center_theta)}; */
/* 		p2L1 = dist_p2LineList(center_Vec,LineList1); */
/* 		p2L2 = dist_p2LineList(center_Vec,LineList2); */
/* 		if(p2L1<p2L2){ */
/* 			theta_near_l1 = center_theta; */
/* 			p_theta_near_l1 = {p.p.x+Space*cos(theta_near_l1),p.p.y+Space*sin(theta_near_l1)}; */
/* 		 } */ 
/* 		else{ */
/* 			theta_near_l2 = center_theta; */
/* 			p_theta_near_l2 = {p.p.x+Space*cos(theta_near_l2),p.p.y+Space*sin(theta_near_l2)}; */
/* 		} */ 
/* 		deviation = dist_p2p(p_theta_near_l1,p_theta_near_l2); */
/* 		if(deviation<prec) return {center_Vec,{cos(center_theta),sin(center_theta)}}; */
/* 	} */
/* } */

//Calculate the weight of the node
double calNodeEvaluation(Vec p,vector<line> &L1,vector<line> &L2){
	double p2L1,p2L2;
	p2L1=dist_p2LineList(p,L1);
	p2L2=dist_p2LineList(p,L2);
	return fabs(p2L1 - p2L2);	
} 

//Search for the next point with method2
dvec nextNode1(dvec p,double Space){
	dvec nextNode;
	vector<Vec> NodeSet;

	Vec tmp;
	double theta0;
	Vec v_dir =  p.v;                   
	if(v_dir.y>=0){
		theta0 = acos(v_dir.x);
	}
	else{
		theta0 = 2*PI-acos(v_dir.x);
	}
	
	double theta[11]={-0.78539816, -0.62831853, -0.4712389 , -0.31415927, -0.15707963,
        0.        ,  0.15707963,  0.31415927,  0.4712389 ,  0.62831853,
        0.78539816};
    for(int i=0;i<11;i++){
    	tmp = {p.p.x+prec*cos(theta[i]+theta0),p.p.y+prec*sin(theta[i]+theta0)};
    	if(isPointInPloygon(tmp, total_LineList))
    		NodeSet.push_back(tmp);
	}
	
	double eva;
	double eva_min = MAX;
	for(int i=0;i<NodeSet.size();i++){
		eva = calNodeEvaluation(NodeSet[i],LineList1,LineList2);
		if(eva<eva_min){
			nextNode = {NodeSet[i],{cos(theta[i]+theta0),sin(theta[i]+theta0)}};
			eva_min = eva;
		}
	}

    erfen(nextNode.p,prec/4);
    /* printf("pioi: %f %f\n",nextNode.p.x,nextNode.p.y); */
	return nextNode;
}
