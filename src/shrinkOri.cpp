#include "global.h"
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

bool cmplen(LLength aa,LLength bb)
{
    return aa.a<bb.a;
}

int pre(int x)
{
    if (x==0) return OriNodeNum-1;
    else return x-1;
}
int nex(int x)
{
    if (x==OriNodeNum-1) return 0;
    else return x+1;
}

void shrinkOri(int effort)
{
    if (effort==0)
    {
        shOri.assign(Ori.begin(),Ori.end()); 
        return;
    }

    int Node_num2=0,shortnum=0,n=OriNodeNum;
    GLine l1,l2;
    LLength x;
    double totlength=0;

    /* Vec tempoi=Ori[n-1]; */
    /* for (int i=n-1;i>0;i--) */
    /*     Ori[i]=Ori[i-1]; */
    /* Ori[0]=tempoi; */

    for (int i=0;i<n;i++)
    {
        x.a=dist_p2p(Ori[i],Ori[pre(i)]);
        x.b=pre(i);
        LineLength.push_back(x);
        totlength+=x.a;
    }
    sort(LineLength.begin(),LineLength.end(),cmplen);
    /* for (int i=0;i<n;i++) printf("LineLength: %d %.10f\n",i,LineLength[i].a); */

    int numshort=0,ave_lower=0,ave_higher=0;
    double avelength=totlength/n,upave=MAX,downave=-MAX;
    for (int i=0;i<=n;i++)
    {
        if (LineLength[i].a<avelength)
        {
            if (LineLength[i].a>downave) downave=LineLength[i].a;
            ave_lower++;
        }
        else
        {
            if (LineLength[i].a<upave) upave=LineLength[i].a;
            ave_higher++;
        }
        if (LineLength[i].a<Width)
            numshort++;
    }

    double shortline=Width*0.33;
    if (numshort>n-n/5) shortline/=3;        

    for (int j=0;j<=n;j++)
    {
        int i=j%n;
        double disp2p=dist_p2p(Ori[i],Ori[pre(i)]);
        bool flag1=arc_equ(arc_calc(Ori[pre(pre(i))],Ori[pre(i)]),arc_calc(Ori[pre(i)],Ori[i]),1.2);
        bool flag2=arc_equ(arc_calc(Ori[pre(i)],Ori[i]),arc_calc(Ori[i],Ori[nex(i)]),1.2);
        if  (disp2p<shortline-0.0000001 && flag1 && flag2)
        {
            if (shortnum==0)
            {
                Node_num2++;   
                Ori2.push_back(Ori[pre(i)]);
            }
            shortnum++;
        }
        else
        {
            if (shortnum>0 && Node_num2>1)
            {
                l1=makeGLine(Ori[i],Ori[pre(i)]);
                l2=makeGLine(Ori2[Node_num2-1],Ori2[Node_num2-2]);
                Vec tempoi=lineintersect(l1,l2);

                if (inshotline(Ori[i],Ori[pre(i)],tempoi) && inshotline(Ori2[Node_num2-2],Ori2[Node_num2-1],tempoi)) 
                {
                    if (tempoi.x<MAX && tempoi.x>-MAX)
                        Ori2.push_back(tempoi);
                }
                else
                    Ori2.push_back(middleNode(Ori[pre(i)],Ori2[Node_num2-1]));

                Node_num2++;
            }
            Node_num2++;
            Ori2.push_back(Ori[pre(i)]);
            shortnum=0;
        }
        /* printf("%d\n",Node_num2); */
    }
    
    bool delflag=1;
    int revmark=0,delnum=10,nx=0;
    n=Ori2.size();

    while (revmark%2==1 || (nx<20 && delnum>0))
    {
        n=Ori2.size();
        Ori3.clear();
        nx++;
        delflag=1;
        delnum=0;
        for (int i=0;i<n;i++)
        {
            if (delflag==0)
            {
                double chan1=dist_p2p(Ori2[(i+1)%n],Ori2[i])*sqrt(dist_p2p(Ori2[(i+1)%n],Ori2[i]));
                double chan2=arc_dis(arc_calc(Ori2[(i-1+n)%n],Ori2[i]),arc_calc(Ori2[i],Ori2[(i+1)%n])); 
                if (fabs(chan2-PI)<0.1) continue;
                if (chan2>0.2)
                {
                    chan2=(chan2+0.5)/(PI+0.5)*PI;
                    chan2=tan(chan2);
                    if (chan2<0) chan2=tan(PI/2-0.1);
                    chan2*=chan2*chan2*chan2;
                }
                if (chan2<0.1) chan2=tan(chan2);

                if (chan1*chan2<0.006*pow(10,effort)*shortline)
                {
                    delnum++;
                    delflag=1;
                    if (chan2<0.0001*pow(10,effort)) continue;
                    l1=makeGLine(Ori2[i],Ori2[(i-1+n)%n]);
                    l2=makeGLine(Ori2[(i+2)%n],Ori2[(i+1)%n]);
                    Vec tempoi=lineintersect(l1,l2);
                    if (inshotline(Ori2[(i+2)%n],Ori2[(i+1)%n],tempoi) && inshotline(Ori2[(i-1+n)%n],Ori2[i],tempoi))
                        if (tempoi.x<MAX && tempoi.x>-MAX)
                            Ori3.push_back(tempoi);
                    continue;
                }
            }
            delflag=0;
            Ori3.push_back(Ori2[i]);
        }

        Ori2.clear();
        n=Ori3.size();
        if (nx%2==0)
            for (int i=0;i<n;i++)
                Ori2.push_back(Ori3[i]);
        else
        {
            revmark++;
            for (int i=n-1;i>=0;i--)
                Ori2.push_back(Ori3[i]);
        }
    }
    printf("Number of refine iterations: %d\n",nx);
    shOri.clear();
    n=Ori2.size();
    printf("shOriNode number: %d\n",n);
    for (int i=0;i<n;i++)
       shOri.push_back(Ori2[i]);

	n=shOriNodeNum = shOri.size(); 
    int se,ee,ss,es;
    if (edgeFlag)
    {
        n=OriNodeNum;
        for (int i=0;i<shOriNodeNum;i++)
        {
            if (equ(shOri[i].x,Ori[(startEdge.s)%n].x) && equ(shOri[i].y,Ori[(startEdge.s)%n].y))
                ss=i;
            if (equ(shOri[i].x,Ori[(startEdge.e)%n].x) && equ(shOri[i].y,Ori[(startEdge.e)%n].y))
                se=i;
            if (equ(shOri[i].x,Ori[(endEdge.s)%n].x) && equ(shOri[i].y,Ori[(endEdge.s)%n].y))
                es=i;
            if (equ(shOri[i].x,Ori[(endEdge.e)%n].x) && equ(shOri[i].y,Ori[(endEdge.e)%n].y))
                ee=i;
        }
        startEdge.s=ss;startEdge.e=se;
        endEdge.s=es;endEdge.e=ee;
        prec=pow((10*n*totlength)/100000000,2.0/3);
        printf("PREC is: %.10f\n",prec);
    }
} 
