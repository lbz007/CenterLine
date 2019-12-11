#include "global.h"
#include <fstream>
#include <iomanip>
#include <cstring>

//Write Triangle list to file
void dumpTriList(){
	string file=FilePureName+"_tri.txt";
	ofstream fo(file,ios::out);
	for(int i=0;i<TML.size();i++){
		fo<<TML[i].a<<" "<<TML[i].b<<" "<<TML[i].c<<endl;
	}
	fo.close();
}

//Write CN(Center Node) to file
void dumpCN(int mode){
	string file = "centerline_"+FileSuffix;
	if(mode==1){
		ofstream fo(file,ios::out);
		for(int i=0;i<CN.size();i++){
            fo<<fixed<<setprecision(2)<<CN[i].x<<" "<<fixed<<setprecision(2)<<CN[i].y<<endl;
		}
		fo.close();
        cout<<endl<<"Generate ./"<<file<<" OK!"<<endl;
	}
	if(mode==2){
		ofstream fo(file,ios::out);
		int segnum=SEGL.size()/2;
		for(int i=0;i<segnum;i++){
			int l=SEGL[2*i];int r=SEGL[2*i+1];
			for(int j=l;j<r;j++){
				fo<<fixed<<setprecision(2)<<CN[j].x<<" "<<fixed<<setprecision(2)<<CN[j].y<<endl;
			}
			if(i!=(segnum-1))
				fo<<endl;
		}
		fo.close();
        cout<<endl<<"Generate ./"<<file<<" OK!"<<endl;
	}	
}

//Write shOri boundary info to file
void dumpshOri(){
	string file=FilePureName+"_sh.txt";
	ofstream fo(file,ios::out);
	for(int i=0;i<shOri.size();i++){
		fo<<"V "<<i<<" "<<shOri[i].x<<" "<<shOri[i].y<<endl;
	}
	fo.close();
}

//Write a vector<Vec> to a specific file
void dump2file(vector<Vec> NodeSet,string fileName){
	ofstream fo(fileName,ios::out);
	 if(!fo){
	 	cout<<"Open Failed..."<<endl;
	 	}
	for(int i=0;i<NodeSet.size();i++){
	 	fo<<setiosflags(ios::showpoint)<<setprecision(16)<<NodeSet[i].x<<" "<<NodeSet[i].y<<endl;
		}
	fo.close();
	cout<<endl<<fileName<<" output finished."<<endl;
}

//Write enOri boundary info to file
void dumpenOri(){
	string file=FilePureName+"_en.txt";
	ofstream fo(file,ios::out);
	for(int i=0;i<enOri.size();i++){
		fo<<"V "<<i<<" "<<enOri[i].x<<" "<<enOri[i].y<<endl;
	}
	fo.close();
}
