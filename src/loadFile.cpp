#include "global.h"
#include <fstream>
#include <regex>
#include <cmath> 
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//Load file
void loadFile(){
    cout<<FileFullName<<endl;
	FileSuffix = FileFullName.substr(FileFullName.find_last_of("_") + 1);
	FilePureName = FileFullName.substr(0, FileFullName.rfind("."));
	cout<<setw(60)<<setfill('.')<<left<<"Begin loading file"<<endl;
	parseFile(FileFullName);
	cout<<setw(60)<<setfill('.')<<left<<"Loading file finished"<<endl;
}

//Parse file
void parseFile(string FileFullName){
	string line;
    int xx,L;
    double tmpx,tmpy;
    char sline[100];
	ifstream fi(FileFullName,std::ios::in);
	while(getline(fi,line)){
        L=line.length();
        for (int i=0;i<L;i++)
            sline[i]=line[i];
        sline[L]='\0';
        if (line[0]=='V')
        {
            sscanf(sline,"V %d %lf %lf",&xx,&tmpx,&tmpy);
            /* printf("lbz:  %d %f %f\n ",xx,tmpx,tmpy); */
            Ori.push_back(Vec(tmpx,tmpy));
            if(tmpx<x_min) x_min=tmpx;
            if(tmpx>x_max) x_max=tmpx;
            if(tmpy<y_min) y_min=tmpy;
            if(tmpy>y_max) y_max=tmpy;
        }
        if (line[0]=='E')
        {
            edgeFlag=1;
            if (line[1]=='1')
                sscanf(sline,"E%d %d %d",&xx,&startEdge.s,&startEdge.e);
            if (line[1]=='2')
                sscanf(sline,"E%d %d %d",&xx,&endEdge.s,&endEdge.e);
        }
    }
	calAreaPerimeter();
	if(clockwiseFlag==false) reverse(Ori.begin(),Ori.end());
	OriNodeNum=Ori.size();
}

//Calculate polygon area and perimeter
void calAreaPerimeter(){
	PloygonArea = 0;
	PloygonPerimeter = 0;
	for(int i=0;i<Ori.size()-1;i++){
		PloygonArea += Ori[i].x*Ori[i+1].y;
		PloygonArea -= Ori[i].y*Ori[i+1].x;
		PloygonPerimeter += sqrt(pow(Ori[i].x-Ori[i+1].x,2)+pow(Ori[i].y-Ori[i+1].y,2));
	} 
	PloygonArea += Ori.back().x*Ori.front().y;
	PloygonArea -= Ori.back().y*Ori.front().x;
	PloygonPerimeter += sqrt(pow(Ori.back().x-Ori.front().x,2)+pow(Ori.back().y-Ori.front().y,2));
	clockwiseFlag = PloygonArea>0? true:false; 
	PloygonArea = PloygonArea>0? PloygonArea/2:-PloygonArea/2;
	Length = PloygonPerimeter/2;
	Width  = PloygonArea/Length;
} 

