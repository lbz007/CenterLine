#include <iostream>
#include "global.h"
#include <iomanip>
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
   
bool cmplen(LLength aa,LLength bb)
{
    return aa.a<bb.a;
}
void refine_boundary()
{
    int shortnum=0,n=Node_num;
    lin l1,l2;
    LLength x;
    double totlength=0;
    for (int i=1;i<n;i++)
    {
        x.a=dist_p2p(NodeList[i],NodeList[i-1]);
        x.b=i-1;
        LineLength.push_back(x);
        totlength+=x.a;
    }
    x.a=dist_p2p(NodeList[n-1],NodeList[0]);
    x.b=n-1;
    LineLength.push_back(x);
    totlength+=x.a;
    sort(LineLength.begin(),LineLength.end(),cmplen);
    /* for (int i=0;i<n;i++) printf("LineLength: %d %.10f\n",i,LineLength[i].a); */

    double shortline=-1,olddet,det=0,th=0.5;
    while (shortline==-1 && th<5)
    {
        th+=1;
        for (int i=1;i<n-4;i++)
        {
            if (det!=0) olddet=det;
            det=LineLength[i].a-LineLength[i-1].a;
            /* printf("%d %f %f\n",i,det,LineLength[i].a); */
            /* if (i==1||olddet==0) continue; */
            if (det>LineLength[i-1].a/th)
            {
                int aa=LineLength[i].b;
                if (fabs(calcxl(NodeList[aa],NodeList[aa-1])-calcxl(NodeList[aa],NodeList[aa+1]))<0.1) continue;
                if (dist_p2p(NodeList[aa],NodeList[aa-1])<totlength/n/10 && dist_p2p(NodeList[aa+2],NodeList[aa+1])<totlength/n/10) continue;
                aa++;
                if (fabs(calcxl(NodeList[aa],NodeList[aa-1])-calcxl(NodeList[aa],NodeList[aa+1]))<0.1) continue;
                shortline=LineLength[i].a;
                break;
            }
        }
    }
    if (shortline==-1) 
        shortline=x_max-x_min+y_max-y_min;

    printf("shortLINE: %f\n",shortline);
    for (int i=1;i<n;i++)
    {
        printf("%d\n",shortnum);
        if (dist_p2p(NodeList[i],NodeList[i-1])<shortline)
        {
            if (shortnum==0)
            {
                Node_num2++;   
                NodeList2.push_back(NodeList[i-1]);
            }
            shortnum++;
        }
        else
        {
            if (shortnum>5)
            {
                l1=twopoint(NodeList[i],NodeList[i-1]);
                l2=twopoint(NodeList2[Node_num2-1],NodeList2[Node_num2-2]);
                Node_num2++;
                NodeList2.push_back(twoline(l1,l2));
            }
            Node_num2++;
            NodeList2.push_back(NodeList[i-1]);
            shortnum=0;
        }
    }
    Node_num2++;
    NodeList2.push_back(NodeList[n-1]);
    string refinedNodefileName = "_refined.txt";
	refinedNodefileName = pureName + refinedNodefileName;
    dump2file(NodeList2,refinedNodefileName);
    for (int i=0;i<Node_num2;i++)
        printf("%f %f\n",NodeList2[i].x,NodeList2[i].y);
}
