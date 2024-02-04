#include <bits/stdc++.h>
#define Fisher_left 0
#define Base 1
#define Fisher_right 2
#define Fishpool_left 3
#define Fishpool_front 4
#define Fishpool_right 5
#define Fishpool_back 6

#define Bluefish 1
#define Greenfish 2
#define Yellowfish 3
#define Redfish 4
#define Feed 5

#define Fish_pool 1
#define Fish_pond 2
#define Base_Feed 3
using namespace std;
const int Node_siz=7,MAXN=105,INF=1e6,Trytimes=1000,Trydeep=100000;
const double End_time=420.0,Stage1end=120.0,Stage2end=300.0;
const double Speed=100.0,Catch_time=5.0,Put_time=5.0,Pause=3.0;
const int Maxroom=6;
double G[Node_siz][Node_siz]= {{0.0   , 57.0  , 114.0 , 300.0 , 195.0 , 290.0 , 485.0},
						       {57.0  , 0.0   , 57.0  , 247.0 , 188.0 , 247.0 , 427.0},
							   {114.0 , 57.0  , 0.0   , 290.0 , 195.0 , 300.0 , 485.0},
							   {300.0 , 247.0 , 290.0 , 0.0   , 167.0 , 340.0 , 185.0},
						 	   {195.0 , 188.0 , 195.0 , 167.0 , 0.0   , 167.0 , 370.0},
							   {290.0 , 247.0 , 300.0 , 340.0 , 167.0 , 0.0   , 185.0},
							   {485.0 , 427.0 , 485.0 , 185.0 , 370.0 , 185.0 , 0.0  }};
double price[5]={0.0,200.0,270.0,320.0,350.0};
double handletime[5]={0.0,20.0,30.0,40.0,50.0};
struct State{
	double time;
	double val;
	int fishpondleft[MAXN],fishpondleftcnt;
	int fishpondright[MAXN],fishpondrightcnt;
	int basefeed;
	int fishpoolleft,fishpoolright,fishpoolfront,fishpoolback;
	double fisherlefttime,fisherrighttime;
	int fisherleftdoing,fisherrightdoing;
	int feeded;
	int robot_address;
	int robot_fishroom[MAXN];
	int robot_fishcnt;
}Now;
bool cmp(int a,int b){
	return a>b;
}
struct Renew{
	State renew_fishpool(double starttime,double endtime,State x){
		if(starttime <= 60.0 && 60.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Redfish;
			x.fishpoolleft=x.fishpoolright=Redfish;
		}else if(starttime <= 120.0 && 120.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Redfish;
			x.fishpoolleft=x.fishpoolright=Yellowfish;
		}else if(starttime <= 180.0 && 180.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Yellowfish;
			x.fishpoolleft=x.fishpoolright=Yellowfish;
		}else if(starttime <= 240.0 && 240.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Greenfish;
			x.fishpoolleft=x.fishpoolright=Greenfish;
		}else if(starttime <= 300.0 && 300.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Greenfish;
			x.fishpoolleft=x.fishpoolright=Bluefish;
		}else if(starttime <= 360.0 && 360.0 <endtime){
			x.fishpoolback=x.fishpoolfront=Bluefish;
			x.fishpoolleft=x.fishpoolright=Bluefish;
		}
		return x;
	}
	State renew_fisher(double starttime,double endtime,State x){
		double dertatime=endtime-starttime;
		if(x.fisherlefttime>0.0 && 
		x.fisherlefttime-dertatime <= 0.0 && x.fisherleftdoing!=0){
			if(Stage1end <x.time && x.time+x.fisherlefttime < Stage2end && x.feeded==1){
				x.val+=price[x.fisherleftdoing]*1.2;
			}else if(Stage2end < x.time  && x.feeded==1){
				x.val+=price[x.fisherleftdoing]*1.5;
			}else{
				x.val+=price[x.fisherleftdoing];
			}
			x.fisherleftdoing=0;
		}
		if(x.fisherrighttime>0.0 && x.fisherrighttime-dertatime <= 0.0 && x.fisherrightdoing!=0){
			if(Stage1end <x.time && x.time+x.fisherrighttime < Stage2end && x.feeded==1){
				x.val+=price[x.fisherrightdoing]*1.2;
			}else if(Stage2end < x.time  && x.feeded==1){
				x.val+=price[x.fisherrightdoing]*1.5;
			}else{
				x.val+=price[x.fisherrightdoing];
			}
			x.fisherleftdoing=0;
		}
		x.fisherlefttime-=dertatime;
		x.fisherrighttime-=dertatime;
		return x;
	}
	State renew_feed(double starttime,double endtime,State x){
		if(starttime<=Stage1end && Stage1end<endtime){
			x.feeded=0;
			x.basefeed=5;
		}else if(starttime<=Stage2end && Stage2end<endtime){
			x.feeded=0;
			x.basefeed=5;
		}
		return x;
	}
}renew;
struct Oper{
	State Wait(double waittime,State x){
		if(waittime<0.0){
			exit(0);
		}
		x=renew.renew_fisher(x.time,x.time+waittime,x);
		x=renew.renew_fishpool(x.time,x.time+waittime,x);
		x=renew.renew_feed(x.time,x.time+waittime,x);
		x.time+=waittime;
		return x;
	}
	State Walk(int End,State x){
		if(End<0 || End>=Node_siz){
			exit(0);
		}
		double dertatime=G[x.robot_address][End]/Speed+Pause;
		x=renew.renew_fisher(x.time,x.time+dertatime,x);
		x=renew.renew_fishpool(x.time,x.time+dertatime,x);
		x=renew.renew_feed(x.time,x.time+dertatime,x);
		x.time+=dertatime;
		x.robot_address=End;
		return x; 
	}
	State Catch(int target,State x){
		if(!((x.robot_address==3 && target==Fish_pond && x.fishpondleftcnt>=1) ||
	  	    (x.robot_address==5 && target==Fish_pond && x.fishpondrightcnt>=1) ||
		    (x.robot_address==3 && target==Fish_pool && x.fishpoolleft!=0) ||
		    (x.robot_address==5 && target==Fish_pool && x.fishpoolright!=0) ||
		    (x.robot_address==4 && target==Fish_pool && x.fishpoolfront!=0) ||
		    (x.robot_address==6 && target==Fish_pool && x.fishpoolback!=0) ||
		    (x.robot_address==1 && target==Base_Feed && x.basefeed==5)
			) 
			
			|| x.robot_fishcnt==Maxroom){
			exit(0);
		}
		x=renew.renew_fisher(x.time,x.time+Catch_time,x);
		x=renew.renew_fishpool(x.time,x.time+Catch_time,x); 
		x=renew.renew_feed(x.time,x.time+Catch_time,x);
		x.time+=Catch_time; 
		if(target==Fish_pond){
			if(x.robot_address==3){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpondleft[x.fishpondleftcnt--];
			}else if(x.robot_address==5){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpondright[x.fishpondrightcnt--];
			}
		}else if(target==Fish_pool){
			if(x.robot_address==3){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpoolleft;
				x.fishpoolleft=0;
			}else if(x.robot_address==4){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpoolfront;
				x.fishpoolfront=0;
			}else if(x.robot_address==5){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpoolright;
				x.fishpoolright=0;
			}else if(x.robot_address==6){
				x.robot_fishroom[++x.robot_fishcnt]=x.fishpoolback;
				x.fishpoolback=0;
			}
		}else if(target==Base_Feed){
			x.robot_fishroom [++x.robot_fishcnt ]=x.basefeed ;
			x.basefeed =0;
		}
		sort(x.robot_fishroom+1,x.robot_fishroom+x.robot_fishcnt+1,cmp);
		return x;
	}
	State Put(int target,State x){
		if(x.robot_address==1){
			exit(0);
		}
		int id=0;
		for(int i=1;i<=x.robot_fishcnt;i++){
			if(x.robot_fishroom[i]==target){
				id=i;
				break;
			}
		}
		if(id==0||
			!((x.robot_address == 0 || x.robot_address ==2) && 1<= target && target<=4
			|| (x.robot_address ==3 || x.robot_address ==4 ||x.robot_address ==5||x.robot_address ==6)&&target==5)
		){
			exit(0);
		} 
		x=renew.renew_fisher(x.time,x.time+Put_time,x);
		x=renew.renew_fishpool(x.time,x.time+Put_time,x); 
		x=renew.renew_feed(x.time,x.time+Put_time,x);
		x.time+=Put_time;
		if(x.robot_address == 0 || x.robot_address ==2){
			if(x.robot_address == 0){
				if(x.fisherlefttime > 0.0){
					exit(0);
				}
				x.fisherleftdoing=x.robot_fishroom[id];
				x.fisherlefttime=handletime[x.robot_fishroom[id]];
			}else if(x.robot_address ==2){
				if(x.fisherrighttime > 0.0){
					exit(0);
				}
				x.fisherrightdoing=x.robot_fishroom[id];
				x.fisherrighttime=handletime[x.robot_fishroom[id]];
			}
		}else if(x.robot_address==3 || x.robot_address==4 || x.robot_address ==5 || x.robot_address==6){
			x.feeded=1;
		}
		for(int i=id;i<=x.robot_fishcnt;i++){
			x.robot_fishroom[i]=x.robot_fishroom[i+1];
		}
		x.robot_fishroom[x.robot_fishcnt--]=0;
		return x;
	}
}oper;
double Estimate(State x){
	double cnt=0.0;
	for(int trytime=1;trytime<=Trytimes;trytime++){
		State Now=x;
		for(int trydeep=1;trydeep<=Trydeep && Now.time <= End_time;trydeep++){
			int randx=rand()%120+1;
			if(randx<=20){
				//printf("fishpoolback\n");
				Now.fishpoolback=0;
			} else if(randx<=32){
				//printf("fishpoolleft\n");
				Now.fishpoolleft=0;
			} else if(randx<=44){
				//printf("fishpoolright\n");
				Now.fishpoolright=0;
			} else if(randx<=59){
				//printf("fishpondleft\n");
				if(Now.fishpondleftcnt > 0){
					Now.fishpondleft[Now.fishpondleftcnt--]=0;
				}
			} else if(randx<=64){
				//printf("fishpondright\n");
				if(Now.fishpondrightcnt > 0){
					Now.fishpondright[Now.fishpondrightcnt--]=0;
				}
			} else if(randx<=70){
				//printf("fishpoolfront\n");
				Now.fishpoolfront=0;
			}
			State Try[MAXN];
			double estimateval[MAXN];
			int trycnt=0;
			for(int i=0;i<Node_siz;i++){
				if(i!=Now.robot_address){
					Try[++trycnt]=oper.Walk(i,Now);
				}
			}
			if(Now.robot_address == 3 && Now.robot_fishcnt != Maxroom && Now.fishpondleftcnt>0){
				Try[++trycnt]=oper.Catch(2,Now); 
			}
			if(Now.robot_address == 5 && Now.robot_fishcnt != Maxroom && Now.fishpondrightcnt>0){
				Try[++trycnt]=oper.Catch(2,Now); 
			}
			if(Now.robot_address == 4 && Now.robot_fishcnt != Maxroom && Now.fishpoolfront!=0){
				Try[++trycnt]=oper.Catch(1,Now);
			}
			if(Now.robot_address == 6 && Now.robot_fishcnt != Maxroom && Now.fishpoolback!=0){
				Try[++trycnt]=oper.Catch(1,Now);
			}
			if(Now.robot_address == 3 && Now.robot_fishcnt != Maxroom && Now.fishpoolleft!=0){
				Try[++trycnt]=oper.Catch(1,Now);
			}
			if(Now.robot_address == 5 && Now.robot_fishcnt != Maxroom && Now.fishpoolright!=0){
				Try[++trycnt]=oper.Catch(1,Now);
			}
			if(Now.robot_address == 1 && Now.robot_fishcnt != Maxroom && Now.basefeed != 0){
				Try[++trycnt]=oper.Catch(3,Now); 
			}
			if(Now.robot_address==0 && Now.fisherlefttime <= 0.0){
				for(int i=1;i<=Now.robot_fishcnt;i++){
					if(1<=Now.robot_fishroom[i] && Now.robot_fishroom[i]<=4){
						Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
					}
				}
			}
			if(Now.robot_address==2 && Now.fisherrighttime <= 0.0){
				for(int i=1;i<=Now.robot_fishcnt;i++){
					if(1<=Now.robot_fishroom[i] && Now.robot_fishroom[i]<=4){
						Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
					}
				}
			}
			if(Now.robot_address ==3 || Now.robot_address ==4 || Now.robot_address ==5 || Now.robot_address ==6){
				for(int i=1;i<=Now.robot_fishcnt;i++){
					if(Now.robot_fishroom[i] == 5){
						Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
					}
				}
			}
			double waittime=1.0;
			for(int i=1;i<=3;i++){
				Try[++trycnt]=oper.Wait(waittime,Now);
				waittime=waittime*2.0; 
			}
			int id=rand()%trycnt+1;
			Now=Try[id];
		}
		double con=0;
		con+=Now.val;
		cnt+=con;
	}
	return cnt;
}
int solve(State Now){
	srand(time(0));
	State Try[MAXN];
	int ans[MAXN];
	double estimateval[MAXN];
	int trycnt=0;
	for(int i=0;i<Node_siz;i++){
		if(i!=Now.robot_address){
			Try[++trycnt]=oper.Walk(i,Now);
			ans[trycnt]=10+i;
		}
	}
	if(Now.robot_address == 3 && Now.robot_fishcnt != Maxroom && Now.fishpondleftcnt>0){
		Try[++trycnt]=oper.Catch(2,Now); 
		ans[trycnt]=32;
	}
	if(Now.robot_address == 5 && Now.robot_fishcnt != Maxroom && Now.fishpondrightcnt>0){
		Try[++trycnt]=oper.Catch(2,Now);
		ans[trycnt]=32;
	}
	if(Now.robot_address == 4 && Now.robot_fishcnt != Maxroom && Now.fishpoolfront!=0){
		Try[++trycnt]=oper.Catch(1,Now);
		ans[trycnt]=31;
	}
	if(Now.robot_address == 6 && Now.robot_fishcnt != Maxroom && Now.fishpoolback!=0){
		Try[++trycnt]=oper.Catch(1,Now);
		ans[trycnt]=31;
	}
	if(Now.robot_address == 3 && Now.robot_fishcnt != Maxroom && Now.fishpoolleft!=0){
		Try[++trycnt]=oper.Catch(1,Now);
		ans[trycnt]=31;
	}
	if(Now.robot_address == 5 && Now.robot_fishcnt != Maxroom && Now.fishpoolright!=0){
		Try[++trycnt]=oper.Catch(1,Now);
		ans[trycnt]=31;
	}
	if(Now.robot_address == 1 && Now.robot_fishcnt != Maxroom && Now.basefeed != 0){
		Try[++trycnt]=oper.Catch(3,Now); 
		ans[trycnt]=33;
	}
	if(Now.robot_address==0 && Now.fisherlefttime <= 0.0){
		for(int i=1;i<=Now.robot_fishcnt;i++){
			if(1<=Now.robot_fishroom[i] && Now.robot_fishroom[i]<=4){
				Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
				ans[trycnt]=40+Now.robot_fishroom[i];
			}
		}
	}
	if(Now.robot_address==2 && Now.fisherrighttime <= 0.0){
		for(int i=1;i<=Now.robot_fishcnt;i++){
			if(1<=Now.robot_fishroom[i] && Now.robot_fishroom[i]<=4){
				Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
				ans[trycnt]=40+Now.robot_fishroom[i];
			}
		}
	}
	if(Now.robot_address ==3 || Now.robot_address ==4 || Now.robot_address ==5 || Now.robot_address ==6){
		for(int i=1;i<=Now.robot_fishcnt;i++){
			if(Now.robot_fishroom[i] == 5){
				Try[++trycnt]=oper.Put(Now.robot_fishroom[i],Now); 
				ans[trycnt]=45;
			}
		}
	}
	double waittime=1.0;
	for(int i=1;i<=3;i++){
		Try[++trycnt]=oper.Wait(waittime,Now);
		ans[trycnt]=20+i;
		waittime=waittime*2.0; 
	}
	int maxid=1;
	for(int i=1;i<=trycnt;i++){
		estimateval[i]=Estimate(Try[i]);
		if(estimateval[i] > estimateval[maxid]){
			maxid=i;
		}
	}
	return ans[maxid];
} 
State readstate(){
	State Now;
	scanf("%lf",&Now.time);
	scanf("%lf",&Now.val);
	memset(Now.fishpondleft,0,sizeof Now.fishpondleft);
	scanf("%d",&Now.fishpondleftcnt);
	for(int i=1;i<=Now.fishpondleftcnt;i++){
		scanf("%d",&Now.fishpondleft[i]);
	}
	memset(Now.fishpondright,0,sizeof Now.fishpondright);
	scanf("%d",&Now.fishpondrightcnt);
	for(int i=1;i<=Now.fishpondrightcnt;i++){
		scanf("%d",&Now.fishpondright[i]);
	}
	scanf("%d",&Now.fishpoolleft);
	scanf("%d",&Now.fishpoolright);
	scanf("%d",&Now.fishpoolfront);
	scanf("%d",&Now.fishpoolback);
	scanf("%lf",&Now.fisherlefttime);
	scanf("%lf",&Now.fisherrighttime);
	scanf("%d",&Now.fisherleftdoing);
	scanf("%d",&Now.fisherrightdoing);
	scanf("%d",&Now.basefeed);
	scanf("%d",&Now.feeded);
	scanf("%d",&Now.robot_address);
	memset(Now.robot_fishroom,0,sizeof Now.robot_fishroom);
	scanf("%d",&Now.robot_fishcnt);
	for(int i=1;i<=Now.robot_fishcnt;i++){
		scanf("%d",&Now.robot_fishroom[i]);
	}
	return Now;
}
int main(){
	freopen("fishcore.in", "r", stdin);
	freopen("fishcore.out", "w", stdout);
	State x;
	x=readstate();
	printf("%d\n",solve(x)); 
	return 0;
} 
