#include "global.h"
#include <algorithm>
#include <cmath>

int Triid=0;

//Triangulation of polygons
void delaunay(){
	int id=1;
	while(L1.size()>3){
		cutTriangle(enOri);
	}
	auto it=L1.begin();
	int a=*(it);int b=*(++it);int c=*(++it);
	double w=calTriangleWeightByIdx(enOri,a,b,c);
	Triangle tmp(a,b,c,w,Triid++);
	tmp.type=calTriType(a,b,c);
	TML.push_back(tmp);
	updateMap(a,b,c);
}

//Initialization shOriNode 
void inishnOriNode(){
	shnOri.resize(0);
	for(auto it=shOri.begin();it!=shOri.end();it++){
		shnOri.push_back(Node((*it).x,(*it).y));
	}
	iniNodeId(shnOri);
	for(auto it=shnOri.begin();it!=shnOri.end();it++){
		iniNode((*it).id,shnOri);
	}
}

//Initialize polygon node id number
void iniNodeId(vector<Node> &NL){
	int NodeNum = NL.size();
	for(int i=0;i<NodeNum;i++){
		NL[i].id = i;
		NL[i].pre = i-1;
		NL[i].next= i+1;
	}
	NL[0].pre = NodeNum - 1;
	NL[NodeNum-1].next= 0;
}

//Initialize polygon node include node type and weight
void iniNode(int id,vector<Node> &NL){
	int _id,id_;
	_id = NL[id].pre;id_=NL[id].next;
	Vec s = {NL[_id].x,NL[_id].y};
	Vec e = {NL[id].x,NL[id].y};
	Vec p = {NL[id_].x,NL[id_].y};
	bool isConvex1Flag = true;		//falg of if is convex1 
	pl tpse=calpSide(p,s,e);
	if(tpse==l){
		for(int i=0;i<NL.size();i++){
			if(i==_id or i==id or i==id_){
				continue;
			} 
			else{
				Vec tmp = {NL[i].x,NL[i].y};
				if(isInTriangle(tmp,s,e,p)){
					isConvex1Flag = false;
					break;
				}
			} 		
		}
		NL[id].type = isConvex1Flag? convex1:convex2; 
		if (NL[id].type==convex1) {
			NL[id].w=calTriangleWeight(p,e,s);
		}
	}
	else{
		NL[id].type =  groove;
	}
}

//Determine the positional relationship between points and line segments
pl calpSide(Vec p,Vec s,Vec e){
	Vec s_e = sub(e,s);
	Vec s_p = sub(p,s);
	double tmp=cmult(s_e,s_p);
	if(tmp>0) return l;
	if(tmp==0) return o;
	if(tmp<0) return r;
}

//Determine if the point is inside the triangle, and the three points of the triangle are counterclockwise
bool isInTriangle(Vec p,Vec a,Vec b,Vec c){
	pl pab,pbc,pca;
	bool tpab,tpbc,tpca;
	pab=calpSide(p,a,b);pbc=calpSide(p,b,c);pca=calpSide(p,c,a);
	tpab = (pab==r)? false:true;
	tpbc = (pbc==r)? false:true;
	tpca = (pca==r)? false:true;
	return (tpab and tpbc and tpca)? true:false;
}

//Increase shnOri
void increshnOri(double lvt){
	double lgth;
	int n;
	int id=0;
	for(auto it=shnOri.begin();it!=shnOri.end();it++){
		Vec s={(*it).x,(*it).y};
		Vec e;
		if(it==(shnOri.end()-1)){
			e.x=(*(shnOri.begin())).x; e.y=(*(shnOri.begin())).y;
		}
		else{
			e.x=(*(it+1)).x; e.y=(*(it+1)).y;
		}
		enOri.push_back(Node((*it).x,(*it).y,(*it).w,(*it).type,id++));
		lgth = dist_p2p(s,e);
		if(lgth>lvt){
			n = round(lgth/lvt);
			Vec space = nmult(1.0/(n+1),sub(e,s));
			for(int i=0;i<n;i++){
				Vec tmp = add(s,nmult(i+1,space));
				enOri.push_back(Node(tmp.x,tmp.y,-1,groove,id++));
			}
		}
	} 
	iniNodeId(enOri);
	enOriNodeNum = enOri.size();
}

//Initialization L1 L2 L3
void iniL1L2L3(vector<Node> &NL){
	for(auto it=NL.begin();it!=NL.end();it++){
		L1.insert((*it).id);
		if((*it).type==convex1){
			L2.insert((*it).id);
		}
		if((*it).type==groove){
			L3.insert((*it).id);
		}
	}
}

//Cut a Triangle from ploygon
void cutTriangle(vector<Node> &NL){
	int a=NL[*L2.begin()].pre;
	int b=*L2.begin();
	int c=NL[*L2.begin()].next;
	Triangle tmp(a,b,c,NL[b].w,Triid++);
	tmp.type=calTriType(a,b,c);
	TML.push_back(tmp);
	updateMap(a,b,c);
	updateL1L2L3(NL);
}

//Update map after cut a triangle
void updateMap(int a,int b,int c){
	updateEdge(a,b);updateEdge(b,c);updateEdge(c,a);
}

//Update edge after cut a triangle
void updateEdge(int s,int e){
	int key = calKey(s,e);
	auto it=T1.find(key);
	if(it==T1.end()){
		T1.insert(pair<int,int>(key,Triid-1));
		T2.insert(pair<int,int>(key,-1));
	}
	else{
		T2[key]=Triid-1;
	}
	Edge tmpe(s,e);
	tmpe.type=calEdgeType(s,e);
	E.insert(pair<int,Edge>(key,tmpe));
}

//Calculate the type of edge
elabel calEdgeType(int s,int e){
	return isPloygonEdge(s,e)==1? boundary:reuse;
}

//Calculate the type of triangle
tlabel calTriType(int a,int b,int c){
	int ab=isPloygonEdge(a,b);
	int bc=isPloygonEdge(b,c);
	int ca=isPloygonEdge(c,a);
	int tmp=ab+bc+ca;
	//cout<<"a,b,c:"<<a<<" "<<b<<" "<<c<<"ab,bc,ca:"<<ab<<" "<<bc<<" "<<ca<<endl; 
	if(tmp==2) return I;
	if(tmp==1) return II;
	if(tmp==0) return III;
}

//Determine if the edge is a polygon boundary
int isPloygonEdge(int a,int b){
	if(abs(a-b)==1 or (a==0 and b==(enOriNodeNum-1)) or(a==(enOriNodeNum-1) and b==0)){
		return 1;
	}
	else{
		return 0;
	}
}

//Calculate the key value of the edge
int calKey(int s,int e){
	return s<e? (NODENUM*s+e):(NODENUM*e+s);
}

//Remove an element from L2
void removefromL2(int id){
	for(auto it=L2.begin();it!=L2.end();it++){
		if((*it)==id){
			L2.erase(it);
			return;
		}
	}
	cout<<"Not fined in L2:"<<id<<endl;
	system("pause");
}

//Update Convex1 Node
void updateNodeConvex1(vector<Node> &NL,int id){
	int _id,id_;
	_id = NL[id].pre; id_=NL[id].next;
	Vec pre ={NL[_id].x,NL[_id].y};
	Vec cur ={NL[id].x,NL[id].y};
	Vec next={NL[id_].x,NL[id_].y};
	bool isInTriFlag = false;
	for(auto it=L3.begin();it!=L3.end();it++){
		if((*it)==_id or (*it)==id or (*it)==id_){
			continue;
		}
		else{
			Vec tmp={NL[(*it)].x,NL[(*it)].y};
			if(isInTriangle(tmp,pre,cur,next)){
				isInTriFlag = true;
				break;
			}
		}	
	}
	NL[id].type = isInTriFlag==false? convex1:convex2;
	if(NL[id].type==convex1){
		removefromL2(id);
		NL[id].w = calTriangleWeight(pre,cur,next);
		L2.insert(id);
	}
	else{
		removefromL2(id);
	}
}

//Update Convex2 Node
void updateNodeConvex2(vector<Node> &NL,int id){
	int _id,id_;
	_id = NL[id].pre; id_=NL[id].next;
	Vec pre ={NL[_id].x,NL[_id].y};
	Vec cur ={NL[id].x,NL[id].y};
	Vec next={NL[id_].x,NL[id_].y};
	bool isConvex1 = true;
	for(auto it=L3.begin();it!=L3.end();it++){
		if((*it)==_id or (*it)==id or (*it)==id_){
			continue;
		}
		else{
			Vec tmp={NL[(*it)].x,NL[(*it)].y};
			if(isInTriangle(tmp,pre,cur,next)){
				isConvex1 = false;
				break;
			}
		}	
	}
	NL[id].type = isConvex1==true? convex1:convex2;
	if(NL[id].type==convex1){
		NL[id].w = calTriangleWeight(pre,cur,next);
		L2.insert(id);
	}
}

//Update groove Node
void updateNodeGroove(vector<Node> &NL,int id){
	int _id,id_;
	_id = NL[id].pre; id_=NL[id].next;
	Vec pre ={NL[_id].x,NL[_id].y};
	Vec cur ={NL[id].x,NL[id].y};
	Vec next={NL[id_].x,NL[id_].y};
	if(l==calpSide(next,pre,cur)){
		bool isConvex1=true;
		for(auto it=L3.begin();it!=L3.end();it++){
			if((*it)==_id or (*it)==id or (*it)==id_){
				continue;
			}
			else{
				Vec tmp={NL[(*it)].x,NL[(*it)].y};
				if(isInTriangle(tmp,pre,cur,next)){
					isConvex1 = false;
					break;
				}
			}	
		}
		NL[id].type = isConvex1==true? convex1:convex2;
		if(NL[id].type==convex1){
			NL[id].w = calTriangleWeight(pre,cur,next);
			L2.insert(id);
			L3.erase(id);
		}
		else{
			L3.erase(id);
		}		
	}
}

//Update L1 L2 L3 after cut a triangle
void updateL1L2L3(vector<Node> &NL){
	int cur=*(L2.begin());
	int pre=NL[cur].pre; int ppre=NL[pre].pre;
	int next=NL[cur].next; int nnext=NL[next].next;
	
	NL[pre].next=next;
	NL[next].pre=pre;
	removefromL2(cur);
	L1.erase(cur);
	
	switch(NL[pre].type){
		case(convex1):{
			updateNodeConvex1(NL,pre);
			break;
		}
		case(convex2):{
			updateNodeConvex2(NL,pre);
			break;
		}
		case(groove):{
			updateNodeGroove(NL,pre);
			break;
		} 
	}
	switch(NL[next].type){
		case(convex1):{
			updateNodeConvex1(NL,next);
			break;
		}
		case(convex2):{
			updateNodeConvex2(NL,next);
			break;
		}
		case(groove):{
			updateNodeGroove(NL,next);
			break;
		}
	} 
}

//Calculate the weight of the node
double calNodeWeight(vector<Node> &NL,int id){
	int pre=NL[id].pre;
	int next=NL[id].next;
	Vec a={NL[pre].x,NL[pre].y};
	Vec b={NL[id].x,NL[id].y};
	Vec c={NL[next].x,NL[next].y};
	return calTriangleWeight(a,b,c);
}

//Calculate the weight of a triangle
double calTriangleWeight(Vec a,Vec b,Vec c){
	Vec ab = sub(b,a);
	Vec bc = sub(c,b);
	Vec ca = sub(a,c);
	double la,lb,lc;
	la = mod(bc);
	lb = mod(ca);
	lc = mod(ab);
	double wa,wb,wc;
	wa = (lc*lc+lb*lb-la*la)/(2*lb*lc);
	wb = (la*la+lc*lc-lb*lb)/(2*la*lc);
	wc = (la*la+lb*lb-lc*lc)/(2*la*lb);
	return max(max(wa,wb),wc);
}

//Calculate the weight of a triangle ,triangle vertex represent by index
double calTriangleWeightByIdx(vector<Node> &NL,int _a,int _b,int _c){
	Vec a={NL[_a].x,NL[_a].y};
	Vec b={NL[_b].x,NL[_b].y};
	Vec c={NL[_c].x,NL[_c].y};
	return calTriangleWeight(a,b,c);
}

