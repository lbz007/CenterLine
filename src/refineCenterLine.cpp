#include <iostream>
#include "global.h"
#include <cmath>
#include <algorithm>
using namespace std;
struct edge
{
    Vec s,e;
    double d;
};
bool para(Vec s1,Vec e1,Vec s2,Vec e2,double pre)
{
    Vec v1,v2;
    double len=dist_p2p(s1,e1);
    v1={(s1.x-e1.x)/len,(s1.y-e1.y)/len};
    len=dist_p2p(s2,e2);
    v2={(s2.x-e2.x)/len,(s2.y-e2.y)/len};
    len=cmult(v1,v2);
    if (fabs(len)<pre) return 1;
    else return 0;

}
bool cmpdis(edge aa,edge bb)
{
    return aa.d<bb.d;
}
void refineCN(int effort)
{
    if (effort==0) return;
    int n=CN.size(),m=shOriNodeNum,numpl;
    vector<edge> pl;
    GLine l1,l2;
    Vec tmp;
    if (edgeFlag==1) 
    {
        SEGL.push_back(0);
        SEGL.push_back(n);
    }
    int numseg=SEGL.size();
    for (int k=0;k<numseg;k+=2)
        for (int i=SEGL[k];i<SEGL[k+1];i++)
        {
            if (i==SEGL[k])
            {
                pl.clear();
                for (int j=0;j<m;j++)
                    if (para(CN[i+1],CN[i],shOri[j],shOri[(j+1)%m],0.2))
                    {
                        double tmpdis=dist_p2Line(CN[i],shOri[j],shOri[(j+1)%m]);
                        tmpdis+=dist_p2Line(CN[i+1],shOri[j],shOri[(j+1)%m]);
                        pl.push_back({shOri[j],shOri[(j+1)%m],tmpdis});
                    }
                numpl=pl.size();
                if (numpl<2) continue;
                sort(pl.begin(),pl.end(),cmpdis);

                if ((pl[0].s.x==pl[1].e.x && pl[0].s.y==pl[1].e.y)||( pl[0].e.x==pl[1].s.x && pl[0].e.y==pl[1].s.y))
                    swap(pl[1],pl[2]);
                if (para(pl[0].s,pl[0].e,pl[1].s,pl[1].e,0.02)==0) continue;
                tmp=getlineprojection(CN[i],middleNode(pl[0].s,pl[1].e),middleNode(pl[0].e,pl[1].s));
                if (dist_p2p(tmp,CN[i])<dist_p2p(CN[i],CN[i+1]))
                    CN[i]=tmp;
                continue;
            }
            if (i==SEGL[k+1]-1)
            {
                pl.clear();
                for (int j=0;j<m;j++)
                    if (para(CN[i-1],CN[i],shOri[j],shOri[(j+1)%m],0.2))
                    {
                        double tmpdis=dist_p2Line(CN[i],shOri[j],shOri[(j+1)%m]);
                        tmpdis+=dist_p2Line(CN[i-1],shOri[j],shOri[(j+1)%m]);
                        pl.push_back({shOri[j],shOri[(j+1)%m],tmpdis});
                    }
                numpl=pl.size();
                /* printf("%d ",numpl); */
                if (numpl<2) continue;
                sort(pl.begin(),pl.end(),cmpdis);

                if ((pl[0].s.x==pl[1].e.x && pl[0].s.y==pl[1].e.y)||( pl[0].e.x==pl[1].s.x && pl[0].e.y==pl[1].s.y))
                    swap(pl[1],pl[2]);
                if (para(pl[0].s,pl[0].e,pl[1].s,pl[1].e,0.02)==0) continue;
                tmp=getlineprojection(CN[i],middleNode(pl[0].s,pl[1].e),middleNode(pl[0].e,pl[1].s));
                if (dist_p2p(tmp,CN[i])<dist_p2p(CN[i],CN[i-1]))
                    CN[i]=tmp;
                continue;
            }

            pl.clear();
            for (int j=0;j<m;j++)
                if (para(CN[i-1],CN[i],shOri[j],shOri[(j+1)%m],0.2))
                {
                    double tmpdis=dist_p2Line(CN[i],shOri[j],shOri[(j+1)%m]);
                    tmpdis+=dist_p2Line(CN[i-1],shOri[j],shOri[(j+1)%m]);
                    pl.push_back({shOri[j],shOri[(j+1)%m],tmpdis});
                }
            numpl=pl.size();
            /* printf("%d ",numpl); */
            if (numpl<2) continue;
            sort(pl.begin(),pl.end(),cmpdis);

            /* printf("pp: %f %f \n",CN[i].x,CN[i].y); */
            /* printf("l1: %f %f %f %f\n",pl[0].s.x,pl[0].s.y,pl[0].e.x,pl[0].e.y); */
            /* printf("l2: %f %f %f %f\n",pl[1].s.x,pl[1].s.y,pl[1].e.x,pl[1].e.y); */

            if ((pl[0].s.x==pl[1].e.x && pl[0].s.y==pl[1].e.y)||( pl[0].e.x==pl[1].s.x && pl[0].e.y==pl[1].s.y))
                swap(pl[1],pl[2]);
            if (para(pl[0].s,pl[0].e,pl[1].s,pl[1].e,0.02)==0) continue;
            l1=makeGLine(middleNode(pl[0].s,pl[1].e),middleNode(pl[0].e,pl[1].s));
            
            pl.clear();
            for (int j=0;j<m;j++)
                if (para(CN[i],CN[i+1],shOri[j],shOri[(j+1)%m],0.3))
                {
                    double tmpdis=dist_p2Line(CN[i],shOri[j],shOri[(j+1)%m]);
                    tmpdis+=dist_p2Line(CN[i+1],shOri[j],shOri[(j+1)%m]);
                    pl.push_back({shOri[j],shOri[(j+1)%m],tmpdis});
                }
            numpl=pl.size();
            /* printf("%d\n",numpl); */
            /* if (numpl<2) continue; */
            sort(pl.begin(),pl.end(),cmpdis);

            /* printf("pp: %f %f \n",CN[i].x,CN[i].y); */
            /* printf("l1: %f %f %f %f\n",pl[0].s.x,pl[0].s.y,pl[0].e.x,pl[0].e.y); */
            /* printf("l2: %f %f %f %f\n",pl[1].s.x,pl[1].s.y,pl[1].e.x,pl[1].e.y); */

            if ((pl[0].s.x==pl[1].e.x && pl[0].s.y==pl[1].e.y)||( pl[0].e.x==pl[1].s.x && pl[0].e.y==pl[1].s.y))
                swap(pl[1],pl[2]);
            if (para(pl[0].s,pl[0].e,pl[1].s,pl[1].e,0.02)==0) continue;
            l2=makeGLine(middleNode(pl[0].s,pl[1].e),middleNode(pl[0].e,pl[1].s));
            
            tmp=lineintersect(l1,l2);
            if (dist_p2p(tmp,CN[i])<min(dist_p2p(CN[i],CN[i-1]),dist_p2p(CN[i],CN[i+1])))
                CN[i]=tmp;
        }
}
