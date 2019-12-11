#include "global.h"
#include <cmath>

//Calculate the point-to-point distance
double dist_p2p(Vec p1,Vec p2){
	return sqrt((pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2)));
}

//Calculate point-to-point distance by node id 
double dist_pidx2pidx(int idx1,int idx2){
	return dist_p2p(NodeList[idx1],NodeList[idx2]);
}

//Vector method calculates the distance from the point to the line segment
double dist_p2Line(Vec p,Vec s,Vec e){
	Vec s_p = sub(p,s);
	Vec s_e = sub(e,s);
	Vec s_c,e_p,c_p;
	double r;
	r = mult(s_p,s_e)/mod_square(s_e);
	
	if(r<=0){
		return mod(s_p);
	} 
	else if(r>=1){
		e_p = sub(s_p,s_e);
		return mod(e_p);
	} 
	else{
		s_c = nmult(r,s_e);
		c_p = sub(s_p,s_c);
		return mod(c_p);
	} 
} 

//The vector method calculates the closest point of the point to the line segment. Returns the nearest point coordinates.
Vec Vec_p2Line(Vec p,Vec s,Vec e){
	Vec s_p = sub(p,s);
	Vec s_e = sub(e,s);
	Vec s_c,e_p,c_p,c;
	double r;
	r = mult(s_p,s_e)/mod_square(s_e);
	if(r<=0){
		return s;
	} 
	else if(r>=1){
		return e;
	} 
	else{
		s_c = nmult(r,s_e);
		c_p = sub(s_p,s_c);
		c = sub(p,c_p);
		return c;
	} 
}

//Calculate the distance from a point to a set of line segments
double dist_p2LineList(Vec p,vector<line> &LineSet){
	double min_dist=MAX;
	double dist;
	for(auto it=LineSet.begin();it!=LineSet.end();it++){
		Vec s={NodeList[(*it).s].x,NodeList[(*it).s].y};
		Vec e={NodeList[(*it).e].x,NodeList[(*it).e].y};
		dist = dist_p2Line(p,s,e);
		if(dist<min_dist) min_dist=dist;
	}
	return min_dist;
}

//Find the position of an edge in the total_LineList 
int findLineIdxInTotaLineList(line l){
	if(abs(l.s-l.e)==1) 
		return min(l.s,l.e);
	else 
		return max(l.s,l.e);
}

//Calculate the distance from the point inside the polygon to the line segment of the polygon. This distance is the internal distance.
double dist_inside_p2Line_inside_ploygon(Vec p,line l){
	Vec lps,lpe;
	lps = NodeList[l.s];
	lpe = NodeList[l.e];
	Vec des;
	Vec tmp;
	des = Vec_p2Line(p,lps,lpe);
	int idx = findLineIdxInTotaLineList(l);
	for(int i=0;i<total_LineList.size();i++){
	    if(i==idx) continue;
		if(isLineCrossLine(p,des,NodeList[total_LineList[i].s],NodeList[total_LineList[i].e])) return MAX;
	}
	return dist_p2p(p,des); 
}

//Calculate the internal distance distance from a point inside a polygon to a group line segment of a polygon 
double dist_inside_p2LineList(Vec p,vector<line> LineSet){
	double min_dist=MAX;
	double dist;
	int idx;
	for(int i=0;i<LineSet.size();i++){
		dist = dist_inside_p2Line_inside_ploygon(p,LineSet[i]);
		if(dist<min_dist){
			min_dist = dist;
		}
	}
	return min_dist;
}

