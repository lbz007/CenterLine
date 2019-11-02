#include "global.h"
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

/* double calcxl(pos a,pos b) */
/* { */
/* 	if (fabs(b.x-a.x)<0.00000001) */
/* 		return 12345.6789; */
/* 	else	return (b.y-a.y)/(b.x-a.x); */
/* } */

bool xlequ(double a,double b)
{
     if (fabs(a-b)<precision/100) return 1;
    /* else return 0; */
     if (fabs(a+2*PI-b)<precision/100) return 1;
     if (fabs(a-2*PI-b)<precision/100) return 1;
     return 0;
}
double calcxl(pos a,pos b)//base point is b
{
    /* double dis=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); */
    /* double si=dis/(a.y-b.y); */
    return atan2((a.y-b.y),(a.x-b.x));
}
lin twopoint(pos p1,pos p2)
{
	lin re;
    double x1=p1.x,y1=p1.y,x2=p2.x,y2=p2.y;
	re.a=y1-y2;
	re.b=x2-x1;
	re.c=x1*y2-x2*y1;
	return re;
}

pos twoline(lin A,lin B)
{
	pos re;
	re.x=(B.b*A.c-A.b*B.c)/(A.b*B.a-A.a*B.b);
	re.y=(B.a*A.c-A.a*B.c)/(A.a*B.b-A.b*B.a);
	return re;
} 

void rectonly()
{
    int unsamexl,i,n=centerNode_num;
    double oldxl,xl;
    bool samexl=1;
    lin l1,l2;
    recNode_num=1;
    recNode.push_back(centerNode[0]);
    for (i=1;i<n;i++)
    {
        printf("%d %f %f\n",i,xl,oldxl);
        oldxl=xl;
        xl=calcxl(centerNode[i],centerNode[i-1]);
        if (i==1) continue;
        if (xlequ(oldxl,xl))
        {
            if (samexl==0)
            {
                if (unsamexl>5)
                {
                   l1=twopoint(centerNode[i],centerNode[i-1]);
                    l2=twopoint(recNode[recNode_num-1],recNode[recNode_num-2]);
                    recNode_num++;
                    recNode.push_back(twoline(l1,l2));
                }
                recNode_num++;
                recNode.push_back(centerNode[i-2]);
            }
            samexl=1;
            unsamexl=0;
        }
        else
        {
            unsamexl++;
            if (samexl==1)
            {
                recNode_num++;
                recNode.push_back(centerNode[i-1]);
            }
            samexl=0;
        }
    }
    recNode_num++;
    recNode.push_back(centerNode[n-1]);
    printf("%d\n",recNode_num);
    for (i=0;i<recNode_num;i++)
        printf("%f %f\n",recNode[i].x,recNode[i].y);
}
