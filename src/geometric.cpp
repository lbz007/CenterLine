#include "global.h"
#include <cmath>

//------------------BOX-------------------------
//Returns the boundary of a line segment
boundry cal_line_boundry(Vec s,Vec e){
	double xmin = s.x;
	double xmax = e.x;
	double ymin = s.y;
	double ymax = e.y;
	if(xmin>xmax) swap(xmin,xmax);
	if(ymin>ymax) swap(ymin,ymax);
	return {xmin,xmax,ymin,ymax};
}

//Returns the bounds of a set of line segments
boundry cal_lineset_boundry(vector<line> LineSet){
	double xmin = 1.7e+308;
	double xmax =-1.7e-308;
	double ymin = 1.7e+308;
	double ymax =-1.7e-308;
	for(int i=0;i<LineSet.size();i++){
		if(NodeList[LineSet[i].s].x<xmin) xmin = NodeList[LineSet[i].s].x;
		if(NodeList[LineSet[i].s].y<ymin) ymin = NodeList[LineSet[i].s].y;
		if(NodeList[LineSet[i].s].x>xmax) xmax = NodeList[LineSet[i].s].x;
		if(NodeList[LineSet[i].s].y>ymax) ymax = NodeList[LineSet[i].s].y;
	}
	if(NodeList[LineSet.back().e].x<xmin) xmin = NodeList[LineSet.back().e].x;
	if(NodeList[LineSet.back().e].y<ymin) ymin = NodeList[LineSet.back().e].y;
	if(NodeList[LineSet.back().e].x>xmax) xmax = NodeList[LineSet.back().e].x;
	if(NodeList[LineSet.back().e].y>ymax) ymax = NodeList[LineSet.back().e].y;
	return {xmin,xmax,ymin,ymax};
}

//Determine if the two boxes overlap, the tangency is overlap
bool isBoxCrossBox(boundry b1,boundry b2){
	if(b1.xmin>b2.xmax) {
		return false;
	}
	else if(b1.xmax<b2.xmin){ 
		return false;
	} 
	else if(b1.ymin>b2.ymax){ 
		return false;
	}
	else if(b1.ymax<b2.ymin){
		return false;
	} 
	else 
	{
		return true;
	}
} 

//-----------------------LINE--------------------------
//Determine whether two line segments intersect. Vector method Tangent counts into intersections Partial coincidence also intersects 
bool isLineCrossLine(Vec l1s,Vec l1e,Vec l2s,Vec l2e){
	boundry b1 = cal_line_boundry(l1s,l1e);
	boundry b2 = cal_line_boundry(l2s,l2e);
	if(!isBoxCrossBox(b1,b2)) return false;
	else{
		Vec l1s_e = sub(l1e,l1s);
		Vec l2s_e = sub(l2e,l2s);
		Vec l1s_l2s = sub(l2s,l1s);
		Vec l1s_l2e = sub(l2e,l1s);
		Vec l2s_l1s = sub(l1s,l2s);
		Vec l2s_l1e = sub(l1e,l2s);
		bool flag1 = cmult(l1s_l2s,l1s_e)*cmult(l1s_l2e,l1s_e)<=0 ? true:false;
		bool flag2 = cmult(l2s_l1s,l2s_e)*cmult(l2s_l1e,l2s_e)<=0 ? true:false;
		if(flag1 & flag2) return true;
		else return false; 
	}
}

//Determine if the line segment intersects a set of line segment
bool isLineCrossLineSet(Vec s,Vec e,vector<line> LineSet){
	Vec tmpps,tmppe;
	for(int i=0;i<LineSet.size();i++){
		tmpps = NodeList[LineSet[i].s];
		tmppe = NodeList[LineSet[i].e];
		if(isLineCrossLine(s,e,tmpps,tmppe)) return true;
	} 
	return false;
}

//Determine whether the line segment is inside the polygon. The line segment verified by the midpoint and pass the disjoint verification 
bool isLineInPloygon(Vec p1,Vec p2,vector<line> LineSet){
	Vec tmp = middleNode(p1,p2);
	if(!isPointInPloygon(tmp,LineSet)) return false;
	else if(isLineCrossLineSet(p1,p2,LineSet)) return false;
	else return true;
} 

//According to the known two-point coordinates, the straight line analytical equations of these two points are obtained: a*x+b*y+c = 0  (a >= 0)  
GLine makeGLine(Vec v1,Vec v2){ 
	GLine gl; 
	int sign = 1; 
	gl.a=v2.y-v1.y; 
	if(gl.a<0) { 
		sign = -1; 
		gl.a=sign*gl.a; 
	} 
	gl.b=sign*(v1.x-v2.x); 
	gl.c=sign*(v1.y*v2.x-v1.x*v2.y); 
	return gl; 
}

Vec getlineprojection(Vec P, Vec A, Vec B){
    Vec v=sub(B,A);
    return add(A,nmult((mult(v,sub(P,A))/mult(v,v)),v));
}

//If two lines l1 (a1*x+b1*y+c1 = 0) and l2(a2*x+b2*y+c2 = 0) intersect, return the intersection point p. If parallel, it will return an invalid value.
Vec lineintersect(GLine l1,GLine l2){ 
	Vec v;
	double d=l1.a*l2.b-l2.a*l1.b; 
	v.x = (l2.c*l1.b-l1.c*l2.b)/d; 
	v.y = (l2.a*l1.c-l1.a*l2.c)/d; 
	return v; 
}

//If the line segments l1 and l2 intersect, return true and the intersection is returned by (inter), otherwise return false
bool SegIntersectSeg(Vec l1s,Vec l1e,Vec l2s,Vec l2e,Vec &inter){
	GLine gl1,gl2;
	gl1 = makeGLine(l1s,l1e);
	gl2 = makeGLine(l2s,l2e);
	if(isLineCrossLine(l1s,l1e,l2s,l2e)){
		inter = lineintersect(gl1,gl2);
		 return true;
	}
	else{
		return false;
	}
}

//--------------------------POINT---------------------------
//Determine if the point is inside a boundry boundary 
bool isPointInBoundry(Vec p,boundry b){
	if(p.x<b.xmin|p.x>b.xmax|p.y<b.ymin|p.y>b.ymax) return false;
	else return true;
} 

//Determine whether the point is on the line segment 
bool isPointOnLine(Vec p,line l){
	Vec s = NodeList[l.s];
	Vec e = NodeList[l.e];
	boundry b = cal_line_boundry(s,e);
	Vec p_s = sub(s,p);
	Vec p_e = sub(e,p);
	if((cmult(p_s,p_e)==0)&(isPointInBoundry(p,b))) return true;
	else return false;
}

//Determine if the point is inside the polygon, including the vertex and the point on the boundary 
bool isPointInPloygon(Vec p, vector<line> LineSet){
	int insec_num = 0;// insect number
	Vec s,e; 
	for(int i=0;i<LineSet.size();i++){
		if(isPointOnLine(p,LineSet[i])) return true;
	}
	for(int i=0;i<LineSet.size();i++){
		s = NodeList[LineSet[i].s];
		e = NodeList[LineSet[i].e];
		if (isRayIntersectsSegment(p,s,e))
			insec_num += 1;
	}
	if(insec_num==0) return false;
	if ((insec_num%2==1)) return true;
	else return false;
}

//Judging whether the horizontal ray passing through a point has an intersection with the line segment. // Judging point, starting point, ending point
bool isRayIntersectsSegment(Vec p, Vec down, Vec top){
	Vec tmp = {1.7e+308,p.y};
	if(down.y==top.y) return false;
	if(down.y>top.y) swap(down,top);
	if(down.y>p.y) return false;
	if(top.y<=p.y) return false; 
	if((down.y==p.y)&(down.x>p.x)) return true; 
	Vec &left = down;
	Vec &right = top;
	if(left.x>right.x) swap(left,right);
	if (right.x<=p.x) return false;
	if(left.x>=p.x) return true; 
	if(isLineCrossLine(top,down,p,tmp)) return true;
	else return false;
}

bool arc_equ(double a,double b,double pre)
{
     if (fabs(a-b)<pre) return 1;
    /* else return 0; */
     if (fabs(a+2*PI-b)<pre) return 1;
     if (fabs(a-2*PI-b)<pre) return 1;
     return 0;
}
double arc_calc(Vec a,Vec b)//base point is b
{
    /* double dis=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); */
    /* double si=dis/(a.y-b.y); */
    return atan2((a.y-b.y),(a.x-b.x));
}

bool equ(double aa,double bb)
{
    return (fabs(aa-bb)<0.00000000001);
}
double arc_power(double a,int x)
{
    double ans=a;
    int in=(int)ans*1000;
    for (int i=1;i<x;i++)
    {
        ans=in/1000.0;
        ans*=a;
        in=(int)ans*1000;
    }
    return ans;
}
double arc_dis(double aa,double bb)
{
    double ans;
    ans=fabs(aa-bb);
    ans=min(ans,fabs(aa+2*PI-bb));
    ans=min(ans,fabs(aa-2*PI-bb));
    return ans;
}
bool inshotline(Vec p1,Vec p2,Vec p3)
{
    if (fabs(p2.x-p1.x)>0.001 && (p3.x-p2.x)*(p2.x-p1.x)<=0.0000000001) return 0;
    if (fabs(p2.y-p1.y)>0.001 && (p3.y-p2.y)*(p2.y-p1.y)<=0.0000000001) return 0;
    return 1;
}

