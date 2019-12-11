#include "global.h"
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

void rectonly(int effort)
{
    if (effort==0) return;

    int CN_num,unsamexl,i,n=CN.size();
    double oldxl,xl;
    bool samexl=1;
    GLine l1,l2;
    centerNode.assign(CN.begin(),CN.end());
    CN.clear();
    CN_num=1;
    CN.push_back(centerNode[0]);
    for (i=1;i<n;i++)
    {
        /* printf("%d %f %f\n",i,xl,oldxl); */
        oldxl=xl;
        xl=arc_calc(centerNode[i],centerNode[i-1]);
        if (i==1) continue;
        if (arc_equ(oldxl,xl,0.0001))
        {
            if (samexl==0)
            {
                if (unsamexl>5)
                {
                    l1=makeGLine(centerNode[i],centerNode[i-1]);
                    l2=makeGLine(CN[CN_num-1],CN[CN_num-2]);
                    CN_num++;
                    CN.push_back(lineintersect(l1,l2));
                }
                CN_num++;
                CN.push_back(centerNode[i-2]);
            }
            samexl=1;
            unsamexl=0;
        }
        else
        {
            unsamexl++;
            if (samexl==1)
            {
                CN_num++;
                CN.push_back(centerNode[i-1]);
            }
            samexl=0;
        }
    }
    CN_num++;
    CN.push_back(centerNode[n-1]);
    /* for (int i=0;i<CN_num;i++)  decprec(CN[i]); */
    double avelength=0;
    for (int i=1;i<CN_num;i++)
        avelength+=dist_p2p(CN[i],CN[i-1]);
    avelength/=CN_num-1;
    /* printf("ave: %d %f\n",CN_num,avelength); */
    int delnum=10,delflag;
    while (delnum>0)
    {
        delflag=0;
        delnum=0;
        n=CN.size();
        centerNode.assign(CN.begin(),CN.end());
        double avelength=0;
        for (int i=1;i<n;i++)
            avelength+=dist_p2p(CN[i],CN[i-1]);
        avelength/=n-1;
        CN.clear();
        /* printf("ave: %d %f\n",n,avelength); */

        for (int i=0;i<n;i++)
        {
            /* printf("%d %f\n",i,arc_calc(centerNode[i-1],centerNode[i])-arc_calc(centerNode[i],centerNode[i+1])); */
            if (i!=0 && i!=n-1 && delflag==0 && arc_equ(arc_calc(centerNode[i-1],centerNode[i]),arc_calc(centerNode[i],centerNode[i+1]),0.0001*pow(10,effort)))
            {
                delflag=1;
                delnum++;
                continue;
            }
            if (i!=0 && i!=n-1 && centerNode[i].x==centerNode[i+1].x && centerNode[i].y==centerNode[i+1].y)
            {
                delflag=1;
                delnum++;
                continue;
            }

            if (i!=0 && i<n-2 && dist_p2p(centerNode[i],centerNode[i+1])<avelength/(1000/pow(10,effort)))
            {
                l1=makeGLine(centerNode[i],centerNode[i-1]);
                l2=makeGLine(centerNode[i+2],centerNode[i+1]);
                Vec tempoi=lineintersect(l1,l2);
                if (inshotline(centerNode[i-1],centerNode[i],tempoi) && inshotline(centerNode[i+2],centerNode[i+1],tempoi)) 
                {
                    CN.push_back(lineintersect(l1,l2));
                    i+=1;
                    delnum+=2;
                    continue;
                }
                else
                {
                    delnum++;
                    continue;
                }
            }

            delflag=0;
            CN.push_back(centerNode[i]);        
        }
    }
/*     printf("%d\n",CN_num); */
/*     for (i=0;i<CN_num;i++) */
/*         printf("%f %f\n",CN[i].x,CN[i].y); */
}
