#include "global.h"
#include <vector>

//�жϵ��Ƿ��ڶ������
bool isPointInPloygon(pos p, vector<line> LineSet){
	int insec_num = 0;// �������
	pos node1;
	pos node2;
	int index_s,index_e;
	for (int i = 0; i < Edge_num; i++){
		index_s = LineSet[i].s;
		index_e = LineSet[i].e;

		node1 = {NodeList[index_s].x,NodeList[index_s].y};
		node2 = {NodeList[index_e].x,NodeList[index_e].y};
		
		if (((p.x==node1.x)&(p.y==node1.y)) | ((p.x == node2.x)&(p.y == node2.y)))//�ų�����
		{
			return false;
		}
		else if ((node1.x == node2.x)&(p.x==node1.x)&(((p.y>node1.y)&(p.y<node2.y))| ((p.y > node2.y)&(p.y <node1.y))))//�ų���ֱ�߶��ϵĵ�
		{
			return false;
		}
		else if((node1.y == node2.y)&(p.y == node1.y)&(((p.x > node1.x)&(p.x < node2.x)) | ((p.x > node2.x)&(p.y < node1.x))))//�ų�ˮƽ�߶εĵ�
		{
			return false;
		}
		else if (((node2.y - node1.y)*(node2.x - p.x)) == ((node2.x - node1.x)*(node2.y - p.y)))//�ų�б���ϵĵ�
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
//�ж�ͨ��һ���ˮƽ���ߣ����߶��Ƿ��н���
bool isRayIntersectsSegment(pos p, pos s, pos e) //�жϵ㣬����㣬���յ�
{
	if (s.y == e.y) //�ų�������ƽ�С��غϣ��߶���β�˵��غϵ����
	{
		return false;
	}
	if ((s.y >p.y)&(e.y >p.y))//�߶��������ϱ�
	{
		return false;
	}
	if ((s.y <p.y)&(e.y <p.y))//�߶��������±�
	{
		return false;
	}
	if ((s.x <p.x)&(e.x <p.x))//�߶����������
	{
		return false;
	}
	if ((s.y ==p.y)&(e.y >p.y))//����Ϊ�¶˵㣬��Ӧspoint
	{
		if (p.x <s.x)
			return true;
		else
			return false;
	}
	if ((e.y ==p.y)&(s.y >p.y))//����Ϊ�¶˵㣬��Ӧepoint
	{
		if (p.x < e.x)
			return true;
		else
			return false;
	}
	if ((s.y ==p.y)&(e.y <p.y))//����Ϊ�϶˵㣬��Ӧspoint
	{
		return false;
	}
	if ((e.y ==p.y)&(s.y <p.y))//����Ϊ�϶˵㣬��Ӧepoint
	{
		return false;
	}
	
	double xseg;
	xseg = e.x - (e.x -s.x)*(e.y -p.y) / (e.y - s.y);//�󽻵������
	if (xseg <p.x)//�����������������
	{
		return false;
	}
	return true;
}
