#include "global.h"
#include <cmath>
#include <fstream> 
#include <algorithm>
using namespace std;

vector<P> totalP;					//Total polygon vertices 
vector<P> dcn; 	 					//Double center point
vector<seg> clist; 					//Center line seg 
set<int> clistID;  					//Center line id 
vector<seg> rlist; 					//Remained center line segment

//Centerline engine with unknown start and end edges
void genCenterLine3(){
	iniTotalP();					
	iniDcn();						 
	iniClist();						
	/* genRlist(); */				
    genRlist1();
    /* refineCN(); */
	/* dumpCN(2); */

}
bool cmpx(const Vec &v1,const Vec &v2){
	return v1.x<v2.x;
}
bool cmpy(const Vec &v1,const Vec &v2){
	return v1.y<v2.y;
}
//Refine segment
void rfSEG(){
	//delete small segment
	double lvt = 0.5*Width;
	vector<int> segl;
	vector<Vec> cn;
	for(int i=0;i<SEGL.size()/2;i++){
		double lgth=0;
		int l = SEGL[2*i];
		int r = SEGL[2*i+1];
		if(r-l==2){
			Vec a = CN[l];
			Vec b = CN[l+1];
			lgth = dist_p2p(a,b);
			if(lgth<lvt){
				continue;
			}
			else{
				segl.push_back(cn.size()); 
				cn.insert(cn.end(),CN.begin()+l,CN.begin()+r);
				segl.push_back(cn.size());
			}
		}
		else{
			segl.push_back(cn.size()); 
			cn.insert(cn.end(),CN.begin()+l,CN.begin()+r);
			segl.push_back(cn.size());
		}
	}
	SEGL.resize(0);
	SEGL.assign(segl.begin(),segl.end());
	CN.resize(0);
	CN.assign(cn.begin(),cn.end());
	
	//expand segment terminals
	for(int i=0;i<SEGL.size()/2;i++){
		double lgth=0;
		int l = SEGL[2*i];
		int r = SEGL[2*i+1];
		
		double lw = 1.5*Width;
		Vec a = CN[l];
		Vec a_pre = CN[l+1];
		Vec b = CN[r-1];
		Vec b_pre = CN[r-2];
		Vec a_pre_a = uv(sub(a,a_pre));
		Vec b_pre_b = uv(sub(b,b_pre));
		Vec a_shot_seg = add(a,nmult(lw,a_pre_a));
		Vec b_shot_seg = add(b,nmult(lw,b_pre_b));
		Vec tmpea,tmpeb;
		Vec mida,midb;
		Vec resa,resb;
		double mina = MAX; 
		double minb = MAX;
		bool aflag = false;
		bool bflag = false;
		for(int i=0;i<total_LineList.size();i++){
			Vec t1 = {totalP[total_LineList[i].s].x,totalP[total_LineList[i].s].y};
			Vec t2 = {totalP[total_LineList[i].e].x,totalP[total_LineList[i].e].y};
			if(SegIntersectSeg(a,a_shot_seg,t1,t2,tmpea)){
				mida = middleNode(t1,t2);
				double dt = dist_p2p(mida,tmpea);
				if(dt<0.05*Width){
					aflag = true;
					double da = dist_p2p(a,tmpea);
					if(da<mina){
						mina = da;
						resa = mida;
					}
				}	
			}
			if(SegIntersectSeg(b,b_shot_seg,t1,t2,tmpeb)){
				midb = middleNode(t1,t2);
				double dt = dist_p2p(midb,tmpeb);
				if(dt<0.05*Width){
					bflag = true;
					double da = dist_p2p(b,tmpeb);
					if(da<minb){
						minb = da;
						resb = midb;
					}
				}
			}
		} 
		if(aflag){
			CN[l] = resa;
			//cout<<"a expand"<<endl;
		}
		if(bflag){
			CN[r-1] = resb;
			//cout<<"b expand"<<endl;
		}
	}
	
	//merge segment
	if(SEGL.size()>=4){
		vector<Vec> tcn;
		vector<int> tsegl;
		Vec sa,ea,mid;
		sa = CN[SEGL[0]];
		ea = CN[SEGL[SEGL.size()-2]];
		double dist = dist_p2p(sa,ea);
		if(dist<0.15*Width){
			mid = middleNode(sa,ea);
			vector<Vec> sseg;
			vector<Vec> eseg;
			vector<Vec> nseg;
			sseg.assign(CN.begin()+SEGL[0],CN.begin()+SEGL[1]);
			eseg.assign(CN.begin()+SEGL[SEGL.size()-2],CN.begin()+SEGL[SEGL.size()-1]);
			reverse(sseg.begin(),sseg.end());
			sseg.pop_back();
			sseg.push_back(mid);
			eseg.erase(eseg.begin());
			nseg.insert(nseg.end(),sseg.begin(),sseg.end());
			nseg.insert(nseg.end(),eseg.begin(),eseg.end());
			tsegl.push_back(tcn.size());
			tcn.insert(tcn.end(),nseg.begin(),nseg.end());
			tsegl.push_back(tcn.size());
			
			for(int i=1;i<SEGL.size()/2-1;i++){
				int l = SEGL[2*i];
				int r = SEGL[2*i+1];
				vector<Vec> tmp;
				tmp.resize(0);
				tmp.assign(CN.begin()+l,CN.begin()+r);
				
				tsegl.push_back(tcn.size());
				tcn.insert(tcn.end(),tmp.begin(),tmp.end());
				tsegl.push_back(tcn.size());
			}
			SEGL.resize(0);
			SEGL.assign(tsegl.begin(),tsegl.end());
			CN.resize(0);
			CN.assign(tcn.begin(),tcn.end());
		} 		
	}	
    //merge seg enhance
	if(SEGL.size()>=4){
		vector<Vec> tcn;
		vector<int> tsegl;
		Vec sa,ea,mid;
		sa = CN[SEGL[0]];
		ea = CN[SEGL[SEGL.size()-2]];
		double dist = dist_p2p(sa,ea);
		bool nearest_flag = true;

		for(int i=SEGL[0];i<SEGL[1]-1;i++){
			Vec a = CN[i];
			Vec b = CN[i+1];
			double d = dist_p2Line(ea,a,b);
			if(d<dist){
				nearest_flag = false;
				break;
			}
		}
		if(nearest_flag){
			mid = middleNode(sa,ea);
			vector<Vec> sseg;
			vector<Vec> eseg;
			vector<Vec> nseg;
			sseg.assign(CN.begin()+SEGL[0],CN.begin()+SEGL[1]);
			eseg.assign(CN.begin()+SEGL[SEGL.size()-2],CN.begin()+SEGL[SEGL.size()-1]);
			reverse(sseg.begin(),sseg.end());
			sseg.push_back(mid);
			nseg.insert(nseg.end(),sseg.begin(),sseg.end());
			nseg.insert(nseg.end(),eseg.begin(),eseg.end());
			tsegl.push_back(tcn.size());
			tcn.insert(tcn.end(),nseg.begin(),nseg.end());
			tsegl.push_back(tcn.size());

			for(int i=1;i<SEGL.size()/2-1;i++){
				int l = SEGL[2*i];
				int r = SEGL[2*i+1];
				vector<Vec> tmp;
				tmp.resize(0);
				tmp.assign(CN.begin()+l,CN.begin()+r);

				tsegl.push_back(tcn.size());
				tcn.insert(tcn.end(),tmp.begin(),tmp.end());
				tsegl.push_back(tcn.size());
			}
			SEGL.resize(0);
			SEGL.assign(tsegl.begin(),tsegl.end());
			CN.resize(0);
			CN.assign(tcn.begin(),tcn.end());
		}
	}
}

//Write center point to CN 
void write2CN(){
	vector<int> partion;
	CN.push_back({dcn[rlist[0].s].x,dcn[rlist[0].s].y}); 					//第一段头点加入 
	for(int i=1;i<rlist.size();i++){
		if((rlist[i].id-1)==rlist[i-1].id){
			CN.push_back({dcn[rlist[i].s].x,dcn[rlist[i].s].y});			//段连号，该段头点加入 
		}
		else{
			Vec cur = {dcn[rlist[i].s].x,dcn[rlist[i].s].y};				//段不连号，判断是否可连接，若可连接 也将该段头点加入 
			Vec pre = {dcn[rlist[i-1].e].x,dcn[rlist[i-1].e].y};
			double dist = dist_p2p(cur,pre);           						 
			if(dist<0.1*Width){												//小于阈值 认为可连段
				CN.push_back({dcn[rlist[i-1].e].x,dcn[rlist[i-1].e].y});
				CN.push_back({dcn[rlist[i].s].x,dcn[rlist[i].s].y});
			} 
			else{
			CN.push_back({dcn[rlist[i-1].e].x,dcn[rlist[i-1].e].y});		//不连号、也不可连接 将上一段未插入的尾点插入 记录分界位置 并将新段段首加入 
			partion.push_back(CN.size());
			CN.push_back({dcn[rlist[i].s].x,dcn[rlist[i].s].y});
			}
		}
	}
	CN.push_back({dcn[rlist.back().e].x,dcn[rlist.back().e].y}); 
	
	SEGL.push_back(0);
	for(int i=0;i<partion.size();i++){
		SEGL.push_back(partion[i]);
		SEGL.push_back(partion[i]);
	}
	SEGL.push_back(CN.size());
    rfSEG();
}

//Remove duplicate center segments  
void genRlist(){
	double lvt=0.2*Width;
	int s,e; double dist;
	vector<int> ertmp;
	int id=0;
	Vec src_s,src_e,cmp_s,cmp_e;
	while(!clistID.empty()){
		ertmp.resize(0);
		src_s.x = dcn[clist[*(clistID.begin())].s].x;
		src_s.y = dcn[clist[*(clistID.begin())].s].y;
		src_e.x = dcn[clist[*(clistID.begin())].e].x;
		src_e.y = dcn[clist[*(clistID.begin())].e].y;
		rlist.push_back(clist[*(clistID.begin())]);
		clistID.erase(clistID.begin());
		for(auto it=clistID.begin();it!=clistID.end();it++){
			cmp_s.x=dcn[clist[(*it)].s].x;
			cmp_s.y=dcn[clist[(*it)].s].y;
			cmp_e.x=dcn[clist[(*it)].e].x;
			cmp_e.y=dcn[clist[(*it)].e].y;
			if(isLineNearLine(src_s,src_e,cmp_s,cmp_e)){
				ertmp.push_back((*it));
			}
		} 
		for(int i=0;i<ertmp.size();i++){
			clistID.erase(ertmp[i]);
		}
	}
    write2CN();
} 

//Parallel segments remove overlapping parts 
bool crmLineOvlpLine(Vec src_s,Vec src_e,Vec &cmp_s,Vec &cmp_e){
	double min_src,max_src,min_cmp,max_cmp;
	Vec *src_min;
	Vec *src_max;
	Vec *cmp_min;
	Vec *cmp_max;
	
	Vec src_se = sub(src_e,src_s);
	Vec cmp_se = sub(cmp_e,cmp_s);
	
	if(fabs(src_se.x)>fabs(src_se.y)){
		if(src_s.x<src_e.x){
			min_src = src_s.x;
			max_src = src_e.x;
			src_min = &src_s;
			src_max = &src_e;
		}
		else{
			min_src = src_e.x;
			max_src = src_s.x;
			src_min = &src_e;
			src_max = &src_s;			
		}
		if(cmp_s.x<cmp_e.x){
			min_cmp = cmp_s.x;
			max_cmp = cmp_e.x;
			cmp_min = &cmp_s;
			cmp_max = &cmp_e;
		}
		else{
			min_cmp = cmp_e.x;
			max_cmp = cmp_s.x;
			cmp_min = &cmp_e;
			cmp_max = &cmp_s;			
		}
	}
	else{
		if(src_s.y<src_e.y){
			min_src = src_s.y;
			max_src = src_e.y;
			src_min = &src_s;
			src_max = &src_e;
		}
		else{
			min_src = src_e.y;
			max_src = src_s.y;
			src_min = &src_e;
			src_max = &src_s;			
		}
		if(cmp_s.y<cmp_e.y){
			min_cmp = cmp_s.y;
			max_cmp = cmp_e.y;
			cmp_min = &cmp_s;
			cmp_max = &cmp_e;
		}
		else{
			min_cmp = cmp_e.y;
			max_cmp = cmp_s.y;
			cmp_min = &cmp_e;
			cmp_max = &cmp_s;			
		}
	}
	
	double lvt = 0.1*Width;
	double lvt1= 0.7*Width;
	if((max_cmp-min_cmp)-(max_src-min_src)>lvt1){
		//cout<<"cmp is longer"<<endl;
		bool minNearFlag,maxNearFlag;
		minNearFlag = fabs(min_src-min_cmp)<lvt? true:false;
		maxNearFlag = fabs(max_src-max_cmp)<lvt? true:false;
		if(minNearFlag and !maxNearFlag){
			//cout<<"cmp before cut："<<cmp_min->x<<" "<<cmp_min->y<<"->"<<cmp_max->x<<" "<<cmp_max->y<<endl;
			*cmp_min = *src_max;
			//cout<<"cmp after cut："<<cmp_min->x<<" "<<cmp_min->y<<"->"<<cmp_max->x<<" "<<cmp_max->y<<endl;
			return false;
		}
		else if(maxNearFlag and !minNearFlag){
			//cout<<"cmp before cut："<<cmp_max->x<<" "<<cmp_max->y<<"->"<<cmp_min->x<<" "<<cmp_min->y<<endl;
			*cmp_max = *src_min;
			//cout<<"cmp after cut："<<cmp_max->x<<" "<<cmp_max->y<<"->"<<cmp_min->x<<" "<<cmp_min->y<<endl;
			return false;
		}
		else{
			//cout<<"not cut"<<endl; 
			return true;
		}	
	}
	//cout<<"not cut"<<endl; 
	return true;
}

struct S{
	Vec s,e;
};

//Remove duplicate center segments with new method
void genRlist1(){
	vector<S> elist;			
	set<int> elistID;		
	int id=0;
	for(auto it=clistID.begin();it!=clistID.end();it++){
		Vec s = {dcn[clist[(*it)].s].x,dcn[clist[(*it)].s].y};
		Vec e = {dcn[clist[(*it)].e].x,dcn[clist[(*it)].e].y};
		S t = {s,e};
		elist.push_back(t);
		elistID.insert(id++);
	}  
	
	vector<int> trlist;
	double lvt=0.2*Width;
	vector<int> ertmp;
	int idx=0;
	Vec src_s,src_e,cmp_s,cmp_e;
	while(!elistID.empty()){
		ertmp.resize(0);
		src_s = elist[*(elistID.begin())].s;
		src_e = elist[*(elistID.begin())].e;
		trlist.push_back(*(elistID.begin()));
		elistID.erase(elistID.begin());
		for(auto it=elistID.begin();it!=elistID.end();it++){
			cmp_s = elist[*(it)].s;
			cmp_e = elist[*(it)].e; 
			if(isLineNearLine(src_s,src_e,cmp_s,cmp_e)){
				if(crmLineOvlpLine(src_s,src_e,elist[*(it)].s,elist[*(it)].e)){
					ertmp.push_back((*it));
				}
			}
		} 
		for(int i=0;i<ertmp.size();i++){
			elistID.erase(ertmp[i]);
		}
	}
	
	//Update CN and SEGL 
	SEGL.resize(0);
	CN.resize(0);
	
	SEGL.push_back(CN.size());
	CN.push_back(elist[trlist.front()].s);
	Vec pre_e = elist[trlist.front()].e;
	for(auto it=trlist.begin()+1;it!=trlist.end();it++){
		Vec cur_s = elist[*(it)].s;
		double dist = dist_p2p(cur_s,pre_e);
		if(dist<0.1*Width){
			CN.push_back(cur_s);
			pre_e = elist[*(it)].e;
		}
		else{
			CN.push_back(pre_e);
			SEGL.push_back(CN.size());
			SEGL.push_back(CN.size());
			CN.push_back(cur_s);
			pre_e = elist[*(it)].e;
		}
	}
	CN.push_back(pre_e);
	SEGL.push_back(CN.size());
	
	rfSEG(); 	
}

//Initialize polygon vertices 
void iniTotalP(){
	int id=0;
	for(int i=0;i<shOri.size();i++){
		totalP.push_back({shOri[i].x,shOri[i].y,id++});
	}
	for(int i=0;i<shOri.size();i++){
		totalP[i].pre = totalP[i].id-1;
		totalP[i].next= totalP[i].id+1;
	}
	totalP[0].pre = totalP.back().id;
	totalP.back().next = 0;

	//ini total_LineList 
	for(int i=0;i<totalP.size();i++){
		total_LineList.push_back({i,i+1});
	}
	total_LineList[totalP.size()-1].e=0;
} 

//Initialization double center point
void iniDcn(){
	int id=1;
	Vec cur = calNearCenterNode(0);
	dcn.push_back({cur.x,cur.y,0});
	double dist;
	double lvt=0.2*Width; 						
	Vec pre;
	for(int i=1;i<totalP.size();i++){
		cur = calNearCenterNode(i);
		pre.x = dcn.back().x;
		pre.y = dcn.back().y;
		dist = dist_p2p(cur,pre);
		if(dist>lvt){
			dcn.push_back({cur.x,cur.y,id++});
		}
	}
} 

//Initialize the center line segment  
void iniClist(){
	for(int i=0;i<dcn.size()-1;i++){
		clist.push_back({i,i+1,i});
		clistID.insert(i); 
	}
	int i=dcn.size()-1;
	clist.push_back({i,0,i});
	clistID.insert(i);
}

//Calculate the center point near a vertex 
Vec calNearCenterNode(int id){
	PP pp = calNodePP(id);
	Vec a=pp.s; Vec b=pp.e;
    Vec x3=middleNode(a,b);
	Vec x1,x2;
	double p = 0.01;		//precision
	int ID=0; 
	while(mod(sub(b,a))>p){
		ID++;
		x1 = add(a,nmult(0.382,sub(b,a)));
		x2 = add(a,nmult(0.618,sub(b,a)));
		if(-1*calNW(x1)>-1*calNW(x2)){
			a = x1; 
		}
		else{
			b = x2;
		}
	}
    x1=middleNode(a,b);
    for (int i=1;i<=1;i++) x3=middleNode(x1,x3);
	//cout<<"Iterator "<<ID<<endl;
	return x3;
}

//Calculate the angle bisector of a vertex within a polygon
PP calNodePP(int id){
	Vec cur={totalP[id].x,totalP[id].y};
	Vec pre={totalP[totalP[id].pre].x,totalP[totalP[id].pre].y};
	Vec next={totalP[totalP[id].next].x,totalP[totalP[id].next].y};
	pl isLeft = calpSide(next,pre,cur);
	Vec cp=uv(sub(pre,cur));
	Vec cn=uv(sub(next,cur));
	double ans = mult(cp,cn)/(mod(cp)*mod(cn));
	double angle = isLeft!=l? 2*PI-acos(ans):acos(ans);
	angle = angle/2.0;
	Vec dir = {cn.x*cos(-angle)+cn.y*sin(-angle),cn.y*cos(-angle)-cn.x*sin(-angle)};
	
	Vec ldir = add(cur,nmult(50.0*Width,dir));  	//以cur作为射线起点，以ldir作为射线终点，求射线与多边形的交点
	Vec tmpdes;										
	Vec des; 										//交点，一般情况下必然存在，若算法失效，使用1倍的估计通道宽度
	double mindist = MAX;
	bool secFlag=false;									//找到正确相交交点标志 
	for(int i=0;i<total_LineList.size();i++){
		Vec a = {totalP[total_LineList[i].s].x,totalP[total_LineList[i].s].y};
		Vec b = {totalP[total_LineList[i].e].x,totalP[total_LineList[i].e].y};
		if(SegIntersectSeg(cur,ldir,a,b,tmpdes)){
			double dist = dist_p2p(cur,tmpdes);
			if(dist<0.05*Width){
				continue;
			}
			else{
				secFlag = true;
				if(dist<mindist){
					mindist =  dist;
					des = tmpdes;
				}
			}
		} 
	} 
	if(secFlag){
		PP pp(cur.x,cur.y,des.x,des.y);
		return pp;
	}
	else{
		Vec des1 = add(cur,nmult(2.0*Width,dir));
		PP pp(cur.x,cur.y,des1.x,des1.y);
		return pp;
	}
} 

//Calculating angle bisector 
Vec calAngleBisctor(Vec pre,Vec cur,Vec next){
	pl isLeft = calpSide(next,pre,cur);
	Vec cp=uv(sub(pre,cur));
	Vec cn=uv(sub(next,cur));
	Vec cd=add(cn,cp);
	Vec dir;
	double l=mod(cd);
	if(l<0.02){							
		dir.x = -1.0*cn.y;
		dir.y = cn.x;
	}
	else{
		dir = (isLeft-l)<MIN? uv(cd):nmult(-1.0,uv(cd));
	} 
	return dir;
}

//Calculate the weight of a point
double calNW(Vec p){
	double mindist = MAX;
	double dist;
	for(int i=0;i<total_LineList.size();i++){
		Vec s = {totalP[total_LineList[i].s].x,totalP[total_LineList[i].s].y};
		Vec e = {totalP[total_LineList[i].e].x,totalP[total_LineList[i].e].y};
		dist = dist_p2Line(p,s,e);
		if(dist<mindist){
			mindist=dist;
		}
	} 
	return mindist;
} 

//Calculate the distance from the line segment to the line segment 
double dist_Line2Line(Vec src_s,Vec src_e,Vec des_s,Vec des_e){
	double d1=dist_p2Line(src_s,des_s,des_e);
	double d2=dist_p2Line(src_e,des_s,des_e);
	return min(d1,d2);
}

//Calculate the distance from the line segment to the line segment by index  
double dist_Line2LineByIdx(int src_l,int des_l){
	Vec src_s={dcn[clist[src_l].s].x,dcn[clist[src_l].s].y};
	Vec src_e={dcn[clist[src_l].e].x,dcn[clist[src_l].e].y};
	Vec des_s={dcn[clist[des_l].s].x,dcn[clist[des_l].s].y};
	Vec des_e={dcn[clist[des_l].e].x,dcn[clist[des_l].e].y};
	return dist_Line2Line(src_s,src_e,des_s,des_e); 
}

//Determine whether the line segment overlaps with the line segment 
bool isLineOvlpLine(Vec l1s,Vec l1e,Vec l2s,Vec l2e){
	double minl1,maxl1,minl2,maxl2;
	Vec l1se = sub(l1e,l1s);
	Vec l2se = sub(l2e,l2s);
	if(fabs(l1se.x)>fabs(l1se.y)){
		minl1 = min(l1s.x,l1e.x);
		maxl1 = max(l1s.x,l1e.x);
		minl2 = min(l2s.x,l2e.x);
		maxl2 = max(l2s.x,l2e.x);
	}
	else{
		minl1 = min(l1s.y,l1e.y);
		maxl1 = max(l1s.y,l1e.y);
		minl2 = min(l2s.y,l2e.y);
		maxl2 = max(l2s.y,l2e.y);
	}
	double m1,m2,m3,m4;
	m1 = minl1<minl2? minl1:minl2;
	m4 = maxl1>maxl2? maxl1:maxl2;
	m2 = min(minl1+minl2-m1,maxl1+maxl2-m4);
	m3 = max(minl1+minl2-m1,maxl1+maxl2-m4);
	double mid = (m2+m3)/2.0;
	if(mid>=minl1 and mid<=maxl1){
		double l = m3-m2;
		if(l>0.2*Width){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
} 

//Determine whether the line segment and the line segment are adjacent 
bool isLineNearLine(Vec src_s,Vec src_e,Vec cmp_s,Vec cmp_e){
	double lvt = 0.2*Width;
	double cmpL = dist_p2p(cmp_s,cmp_e);
	if(cmpL<lvt){
		double dist = dist_Line2Line(cmp_s,cmp_e,src_s,src_e);
		if(dist<0.2*Width){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		Vec usrc = uv(sub(src_e,src_s));
		Vec udes = uv(sub(cmp_e,cmp_s));
		double tmp = fabs(cmult(usrc,udes));
		if(tmp<0.3){
			double dist = dist_Line2Line(cmp_s,cmp_e,src_s,src_e);
			if(dist<0.2*Width){
				if(isLineOvlpLine(src_s,src_e,cmp_s,cmp_e)){ 
					return true;
				}
				else{
					return false;
				}
			}
			else{ 
				return false;
			}
		}
		else{ 
			return false;
		}
	}
}

//Print totalP (total ploygon vertex)
void show_totalP(){
	cout<<"totalP size:"<<totalP.size()<<endl;
	for(int i=0;i<totalP.size();i++){
		cout<<"totalP["<<i<<"]:"<<"id:"<<totalP[i].id<<" pos:"<<totalP[i].x<<","<<totalP[i].y<<" pre:"<<totalP[i].pre<<" next:"<<totalP[i].next<<endl; 
	}
	cout<<endl;
}

//Print dcn (center node)
void show_dcn(){
	cout<<"dcn size:"<<dcn.size()<<endl;
	for(int i=0;i<dcn.size();i++){
		cout<<"dcn["<<i<<"]:"<<" id:"<<dcn[i].id<<" pos:"<<dcn[i].x<<" "<<dcn[i].y<<endl;
	}
	cout<<endl;
}

//Print clist (center line list)
void show_clist(){
	cout<<"clist size:"<<clist.size()<<endl;
	int id=0;
	for(auto it=clist.begin();it!=clist.end();it++){
		cout<<"clist["<<id++<<"]: id:"<<(*it).id<<" "<<(*it).s<<"->"<<(*it).e<<endl;
	}
	cout<<endl;
}

//Print clist ID
void show_clistID(){
	cout<<"clistID size:"<<clistID.size()<<endl; 
	for(auto it=clistID.begin();it!=clistID.end();it++){
		cout<<(*it)<<" "; 
	}
	cout<<endl<<endl;
} 

//Print rlist (remained center line list)
void show_rlist(){
	cout<<"rlist size:"<<rlist.size()<<endl;
	for(int i=0;i<rlist.size();i++){
		cout<<"rlist["<<i<<"]: id:"<<rlist[i].id<<" "<<rlist[i].s<<"->"<<rlist[i].e<<endl;
	}
	cout<<endl;
}

