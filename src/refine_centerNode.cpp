#include "global.h"
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

bool equ(double a,double b)
{
    if (fabs(a-b)<0.000000001)return 1;
    else return 0;
}
double Herfen(double hh,double tt,double nowy)
{
	double d0,d1,mid;
	int dir;
	pos pp,ph,pt;
	ph.x=hh;ph.y=nowy;
	pt.x=tt;pt.y=nowy;
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(pt,LineList1);
	if (d0<0)
		dir=1;
	else
		dir=-1;
	while (tt-hh>=precision/100000)
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

double Verfen(double hh,double tt,double nowx)
{
	double d0,d1,mid;
	int dir;
	pos pp,ph,pt;
	ph.x=nowx;ph.y=hh;
	pt.x=nowx;pt.y=tt;
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(pt,LineList1);
	if (d0<0)
		dir=1;
	else
		dir=-1;
	while (tt-hh>=precision/100000)
	{
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

void erfen(pos &pp,double det)
{
	pos ph,pt;
	double d1,d0,dd1,dd2;
	ph.x=pp.x-det;ph.y=pp.y; pt.x=pp.x+det;pt.y=pp.y; 
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(ph,LineList2);
	d1=dist_p2LineList(pt,LineList1)-dist_p2LineList(pt,LineList2);
	dd1=fabs(d0-d1);
	ph.x=pp.x;ph.y=pp.y-det; pt.x=pp.x;pt.y=pp.y+det; 
	d0=dist_p2LineList(ph,LineList1)-dist_p2LineList(ph,LineList2);
	d1=dist_p2LineList(pt,LineList1)-dist_p2LineList(pt,LineList2);
	dd2=fabs(d0-d1);
	//printf("%f %f %f\n",dd1,dd2,det);
	if (dd1>dd2)
		pp.x=Herfen(pp.x-det,pp.x+det,pp.y);
	else
		pp.y=Verfen(pp.y-det,pp.y+det,pp.x);
	return;	
}

void refine_centerNode()
{
    int i,n=centerNode.size();
	printf("dfaasfasf\n");
    for (i=0;i<n;i++)
    {
        erfen(centerNode[i],precision/10);    
    }
        
}
