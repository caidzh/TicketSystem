#include<bits/stdc++.h>
using namespace std;
void rev_calc(int t,string &day,string &inday){
    day="";
    inday="";
    int m,d;
    d=t/1440;
    if(d>=61){
        m=8;
        d-=60;
    }
    else if(d>=30){
        m=7;
        d-=29;
    }
    else{
        m=6;
        d+=1;
    }
    day="0";
    day+=(char)('0'+m);
    day+="-";
    day+=(char)(d/10+'0');
    day+=(char)(d%10+'0');
    t%=1440;
    int hour,min;
    hour=t/60;
    min=t%60;
    inday+=(char)('0'+hour/10);
    inday+=(char)('0'+hour%10);
    inday+=':';
    inday+=(char)('0'+min/10);
    inday+=(char)('0'+min%10);
}
void calc(const string &day,const string &inday,int &t){
    t=0;
    if(day[1]=='8')
        t+=61*1440;
    if(day[1]=='7')
        t+=30*1440;
    int d=(day[3]-'0')*10+day[4]-'0';
    t+=1440*(d-1);
    int h=(inday[0]-'0')*10+inday[1]-'0';
    t+=60*h;
    t+=(inday[3]-'0')*10+inday[4]-'0';
}
int main(){
    freopen("timer","w",stdout);
    string day,inday;
    for(int i=0;i<=50000;i++){
        rev_calc(i,day,inday);
        int j;
        calc(day,inday,j);
        if(i!=j)
            cout<<"wtf\n";
    }
    return 0;
}