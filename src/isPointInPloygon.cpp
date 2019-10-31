#include "global.h"
#include <vector>

//判断点是否在多边形内
bool isPointInPloygon(pos p, vector<line> LineSet){
	int insec_num = 0;// 交点个数
	pos node1;
	pos node2;
	int index_s,index_e;
	for (int i = 0; i < Edge_num; i++){
		index_s = LineSet[i].s;
		index_e = LineSet[i].e;

		node1 = {NodeList[index_s].x,NodeList[index_s].y};
		node2 = {NodeList[index_e].x,NodeList[index_e].y};
		
		if (((p.x==node1.x)&(p.y==node1.y)) | ((p.x == node2.x)&(p.y == node2.y)))//排除顶点
		{
			return false;
		}
		else if ((node1.x == node2.x)&(p.x==node1.x)&(((p.y>node1.y)&(p.y<node2.y))| ((p.y > node2.y)&(p.y <node1.y))))//排除垂直线段上的点
		{
			return false;
		}
		else if((node1.y == node2.y)&(p.y == node1.y)&(((p.x > node1.x)&(p.x < node2.x)) | ((p.x > node2.x)&(p.y < node1.x))))//排除水平线段的点
		{
			return false;
		}
		else if (((node2.y - node1.y)*(node2.x - p.x)) == ((node2.x - node1.x)*(node2.y - p.y)))//排除斜线上的点
		{
			if(((p.x>node1.x)&(p.x<node2.x))| ((p.x<node1.x)&(p.x>node2.x)))
				return false;
		}
		else if (isRayIntersectsSegment(p, node1, node2))
		{
			insec_num += 1;
		}
	}

	if (insec_num % 2 == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//判断通过一点的水平射线，与线段是否有交点
bool isRayIntersectsSegment(pos p, pos s, pos e) //判断点，边起点，边终点
{
	if (s.y == e.y) //排除与射线平行、重合，线段首尾端点重合的情况
	{
		return false;
	}
	if ((s.y >p.y)&(e.y >p.y))//线段在射线上边
	{
		return false;
	}
	if ((s.y <p.y)&(e.y <p.y))//线段在射线下边
	{
		return false;
	}
	if ((s.x <p.x)&(e.x <p.x))//线段在射线左边
	{
		return false;
	}
	if ((s.y ==p.y)&(e.y >p.y))//交点为下端点，对应spoint
	{
		if (p.x <s.x)
			return true;
		else
			return false;
	}
	if ((e.y ==p.y)&(s.y >p.y))//交点为下端点，对应epoint
	{
		if (p.x < e.x)
			return true;
		else
			return false;
	}
	if ((s.y ==p.y)&(e.y <p.y))//交点为上端点，对应spoint
	{
		return false;
	}
	if ((e.y ==p.y)&(s.y <p.y))//交点为上端点，对应epoint
	{
		return false;
	}
	
	double xseg;
	xseg = e.x - (e.x -s.x)*(e.y -p.y) / (e.y - s.y);//求交点横坐标
	if (xseg <p.x)//交点在射线起点的左侧
	{
		return false;
	}
	return true;
}
