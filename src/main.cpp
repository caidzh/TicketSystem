#include "bpt.hpp"
#include "account.h"
#include "train.h"
using std::cin;
using std::cout;
string s;
int account_cnt;
string token[30];
int len;
class seat_ticket{
public:
    int seat[99];
};
MemoryRiver<seat_ticket>seat_manager("seat");
void get_token(string &s){
    string str="";
    len=0;
    for(int i=0;i<s.size();i++)
        if(s[i]==' '||s[i]=='\n'||s[i]=='\r'){
            token[len++]=str;
            str="";
        }
        else
            str+=s[i];
    if(str!="")
        token[len++]=str;
    cout<<token[0]<<" ";
}
bpt<account>acc;
bpt<order>ord,ref;
bpt<train>train_id;
bpt<train_s>train_start;
//acc username-(blank)
//ord username-(blank)
//train_id train_id-(blank)
//train_start startstation-train_id
void call_invalid(){
    cout<<"-1\n";
}
void call_valid(){
    cout<<"0\n";
}
bool vis[10010];
void add_user(){
    node x,y;
    account a;
    account c;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-c"){
            if(account_cnt==0)
                continue;
            fsta(token[i+1],c.username);
            fsta(token[i+1],x.index);
            if(acc.find(x,c)==false&&account_cnt!=0){
                call_invalid();
                return;
            }
        }
        if(token[i]=="-u"){
            fsta(token[i+1],a.username);
            fsta(token[i+1],y.index);
        }
        if(token[i]=="-p")
            fsta(token[i+1],a.password);
        if(token[i]=="-n")
            fsta(token[i+1],a.name);
        if(token[i]=="-m")
            fsta(token[i+1],a.mailAddr);
        if(token[i]=="-g")
            fstn(token[i+1],a.privilege);
    }
    if(account_cnt==0)
        a.privilege=10;
    if(acc.find(y,a)==false&&(account_cnt==0||(c.privilege>a.privilege&&vis[c.id]))){
        a.id=account_cnt++;
        acc.insert(y,a);
    }
    else{
        call_invalid();
        return;
    }
    call_valid();
}
void login(){
    node x;
    account a,c;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-u")
            fsta(token[i+1],x.index);
        if(token[i]=="-p")
            fsta(token[i+1],a.password);
    }
    if(acc.find(x,c)==false){
        call_invalid();
        return;
    }
    if(strcmp(c.password,a.password)!=0||vis[c.id]){
        call_invalid();
        return;
    }
    vis[c.id]=true;
    call_valid();
}
void logout(){
    node x;
    account a;
    fsta(token[3],x.index);
    if(acc.find(x,a)==false){
        call_invalid();
        return;
    }
    if(vis[a.id]==false){
        call_invalid();
        return;
    }
    vis[a.id]=false;
    call_valid();
}
void query_profile(){
    node x,y;
    account a,c;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-c"){
            fsta(token[i+1],x.index);
            if(acc.find(x,c)==false){
                call_invalid();
                return;
            }
        }
        if(token[i]=="-u"){
            fsta(token[i+1],y.index);
            if(acc.find(y,a)==false){
                call_invalid();
                return;
            }
        }
    }
    if(vis[c.id]&&(strcmp(a.username,c.username)==0||a.privilege<c.privilege))
        cout<<a.username<<" "<<a.name<<" "<<a.mailAddr<<" "<<a.privilege<<"\n";
    else
        call_invalid();
}
void modify_profile(){
    node x,y;
    account a,c,new_a;
    int next_privilege=0;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-c"){
            fsta(token[i+1],x.index);
            if(acc.find(x,c)==false){
                call_invalid();
                return;
            }
            if(vis[c.id]==false){
                call_invalid();
                return;
            }
        }
        if(token[i]=="-u"){
            fsta(token[i+1],y.index);
            if(acc.find(y,a)==false){
                call_invalid();
                return;
            }
            new_a=a;
        }
    }
    for(int i=2;i<len;i+=2){
        if(token[i]=="-p")
            fsta(token[i+1],new_a.password);
        if(token[i]=="-n")
            fsta(token[i+1],new_a.name);
        if(token[i]=="-m")
            fsta(token[i+1],new_a.mailAddr);
        if(token[i]=="-g"){
            fstn(token[i+1],new_a.privilege);
            next_privilege=new_a.privilege;
        }
    }
    if((strcmp(c.username,a.username)==0||c.privilege>a.privilege)&&c.privilege>next_privilege){
        acc.file_info.update(new_a,a.pos);
        cout<<new_a.username<<" "<<new_a.name<<" "<<new_a.mailAddr<<" "<<new_a.privilege<<"\n";
    }
    else
        call_invalid();
}
void calc(const string &day,const string &inday,int &t){
    t=0;
    if(day[1]=='9')
        t+=92*1440;
    else if(day[1]=='8')
        t+=61*1440;
    else if(day[1]=='7')
        t+=30*1440;
    int d=(day[3]-'0')*10+day[4]-'0';
    t+=1440*(d-1);
    int h=(inday[0]-'0')*10+inday[1]-'0';
    t+=60*h;
    t+=(inday[3]-'0')*10+inday[4]-'0';
}
void rev_calc(int t,string &day,string &inday){
    day="";
    inday="";
    int m,d;
    d=t/1440;
    if(d>=92){
        m=9;
        d-=91;
    }
    else if(d>=61){
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
void add_train(){
    node x;
    train a,b;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-i"){
            fsta(token[i+1],a.train_id);
            fsta(token[i+1],x.index);
            if(train_id.find(x,b)){
                call_invalid();
                return;
            }
        }
        if(token[i]=="-n")
            fstn(token[i+1],a.station_num);
        if(token[i]=="-m")
            fstn(token[i+1],a.seat_num);
        if(token[i]=="-s"){
            int cnt=0,l=0;
            for(int j=0;j<token[i+1].size();j++){
                if(token[i+1][j]=='|'){
                    a.station[cnt][l]='\0';
                    cnt++;
                    l=0;
                }
                else
                    a.station[cnt][l++]=token[i+1][j];
            }
            a.station[cnt][l]='\0';
        }
        if(token[i]=="-p"){
            int cnt=0,l=0;
            for(int j=0;j<token[i+1].size();j++){
                if(token[i+1][j]=='|'){
                    a.price[cnt]=l;
                    cnt++;
                    l=0;
                }
                else
                    l=l*10+token[i+1][j]-'0';
            }
            a.price[cnt]=l;
        }
        if(token[i]=="-x")
            fsta(token[i+1],a.starttime);
        if(token[i]=="-t"){
            int cnt=0,l=0;
            for(int j=0;j<token[i+1].size();j++){
                if(token[i+1][j]=='|'){
                    a.travel_time[cnt]=l;
                    cnt++;
                    l=0;
                }
                else
                    l=l*10+token[i+1][j]-'0';
            }
            a.travel_time[cnt]=l;
        }
        if(token[i]=="-o"){
            int cnt=0,l=0;
            for(int j=0;j<token[i+1].size();j++){
                if(token[i+1][j]=='|'){
                    a.stop_over_time[cnt]=l;
                    cnt++;
                    l=0;
                }
                else
                    l=l*10+token[i+1][j]-'0';
            }
            a.stop_over_time[cnt]=l;
        }
        if(token[i]=="-d"){
            int cnt=0,l=0;
            for(int j=0;j<token[i+1].size();j++){
                if(token[i+1][j]=='|'){
                    a.sale_date[cnt][l]='\0';
                    cnt++;
                    l=0;
                }
                else
                    a.sale_date[cnt][l++]=token[i+1][j];
            }
            a.sale_date[cnt][l]='\0';
        }
        if(token[i]=="-y")
            a.type=token[i+1][0];
    }
    for(int i=1;i<a.station_num-1;i++){
        a.price[i]+=a.price[i-1];
        a.travel_time[i]+=a.travel_time[i-1];
    }
    for(int i=1;i<a.station_num-2;i++)
        a.stop_over_time[i]+=a.stop_over_time[i-1];
    train_id.insert(x,a);
    call_valid();
}
void delete_train(){
    node x;
    train a;
    fsta(token[3],x.index);
    if(!train_id.find(x,a)){
        call_invalid();
        return;
    }
    if(a.is_release){
        call_invalid();
        return;
    }
    train_id.del(x);
    call_valid();
}
void release_train(){
    node x;
    train a;
    fsta(token[3],x.index);
    if(!train_id.find(x,a)){
        call_invalid();
        return;
    }
    if(a.is_release){
        call_invalid();
        return;
    }
    seat_ticket mp[92];
    a.is_release=true;
    train_s a_s;
    for(int i=0;i<6;i++)
        a_s.sale_date[0][i]=a.sale_date[0][i];
    for(int i=0;i<6;i++)
        a_s.sale_date[1][i]=a.sale_date[1][i];
    int L,R;
    calc(fats(a_s.sale_date[0]),"00:00",L);
    calc(fats(a_s.sale_date[1]),"00:00",R);
    int o=(R-L)/1440;
    for(int i=0;i<6;i++)
        a_s.starttime[i]=a.starttime[i];
    a_s.price=0;
    a_s.stop_over_time=0;
    a_s.in=0;
    a_s.tim=0;
    for(int i=0;i<a.station_num;i++){
        node y;
        char_array_assign(y.index,a.station[i]);
        char_array_assign(y.val,a.train_id);
        train_start.insert(y,a_s);
        if(i!=a.station_num-1){
            int t;
            train_s A_s;
            calc(fats(a_s.sale_date[0]),fats(a_s.starttime),t);
            mp[0].seat[i]=a.seat_num;
            for(int j=1;j<=o;j++)
                mp[j].seat[i]=a.seat_num;
            if(i!=a.station_num-2)
                t+=a.travel_time[i]-(i?a.travel_time[i-1]:0)+a.stop_over_time[i]-(i?a.stop_over_time[i-1]:0);
            else
                t+=a.travel_time[i]-(i?a.travel_time[i-1]:0);
            string day,inday;
            rev_calc(t,day,inday);
            fsta(day,A_s.sale_date[0]);
            fsta(inday,A_s.starttime);
            calc(fats(a_s.sale_date[1]),fats(a_s.starttime),t);
            if(i!=a.station_num-2)
                t+=a.travel_time[i]-(i?a.travel_time[i-1]:0)+a.stop_over_time[i]-(i?a.stop_over_time[i-1]:0);
            else
                t+=a.travel_time[i]-(i?a.travel_time[i-1]:0);
            rev_calc(t,day,inday);
            fsta(day,A_s.sale_date[1]);
            if(i!=a.station_num-2)
                A_s.stop_over_time=a.stop_over_time[i]-(i?a.stop_over_time[i-1]:0);
            else
                A_s.stop_over_time=0;
            A_s.price=a.price[i];
            A_s.in=a_s.in+1;
            if(i!=a.station_num-2)
                A_s.tim=a_s.tim+a.travel_time[i]-(i?a.travel_time[i-1]:0)+a.stop_over_time[i]-(i?a.stop_over_time[i-1]:0);
            else
                A_s.tim=a_s.tim+a.travel_time[i]-(i?a.travel_time[i-1]:0);
            a_s=A_s;
        }
    }
    for(int i=0;i<=o;i++){
        a.seat_pos[i]=seat_manager.get_lst();
        seat_manager.write(mp[i]);
    }
    train_id.file_info.update(a,a.pos);
    call_valid();
}
void query_train(){
    node x;
    string day;
    train a;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-i")
            fsta(token[i+1],x.index);
        if(token[i]=="-d")
            day=token[i+1];
    }
    int t;
    calc(day,"00:00",t);
    if(!train_id.find(x,a)){
        call_invalid();
        return;
    }
    if(day<fats(a.sale_date[0])||day>fats(a.sale_date[1])){
        call_invalid();
        return;
    }
    cout<<a.train_id<<" "<<a.type<<"\n";
    seat_ticket mp;
    int d1,d2;
    calc(day,"00:00",d1);
    calc(fats(a.sale_date[0]),"00:00",d2);
    int d=(d1-d2)/1440;
    if(a.is_release)
        seat_manager.read(mp,a.seat_pos[d]);
    string tim=fats(a.starttime);
    int T,T1;
    calc(day,tim,T);
    for(int i=0;i<a.station_num;i++){
        cout<<a.station[i]<<" ";
        if(i==0)
            cout<<"xx-xx xx:xx -> ";
        else{
            T1=T+a.travel_time[i-1];
            if(i>=2)
                T1+=a.stop_over_time[i-2];
            rev_calc(T1,day,tim);
            cout<<day<<" "<<tim<<" -> ";
        }
        if(i==0)
            cout<<day<<" "<<tim<<" ";
        else if(i==a.station_num-1)
            cout<<"xx-xx xx:xx ";
        else{
            T1=T+a.travel_time[i-1]+a.stop_over_time[i-1];
            rev_calc(T1,day,tim);
            cout<<day<<" "<<tim<<" ";
        }
        if(i==0)
            cout<<"0 ";
        else
            cout<<a.price[i-1]<<" ";
        if(i==a.station_num-1)
            cout<<"x\n";
        else{
            if(a.is_release)
                cout<<mp.seat[i]<<"\n";
            else
                cout<<a.seat_num<<"\n";
        }
    }
}
char start_train_id[10010][31],stop_train_id[10010][31],satisfy_train_id[10010][31];
train_s start_train[10010],stop_train[10010],satisfy_start_train[10010],satisfy_stop_train[10010];
int start_train_num,stop_train_num,id[10010],seat[10010];
void mergesort(int a[],int l,int r,bool (*cmp)(const int &x,const int &y)){
    if(l==r)
        return;
    int mid=(l+r)>>1;
    mergesort(a,l,mid,cmp);
    mergesort(a,mid+1,r,cmp);
    int pl=l,pr=mid+1,cnt=0;
    int *tmp=new int[r-l+1];
    while(pl<=mid&&pr<=r){
        if(cmp(a[pl],a[pr])){
            tmp[cnt]=a[pl];
            cnt++;pl++;
        }
        else{
            tmp[cnt]=a[pr];
            cnt++;pr++;
        }
    }
    while(pl<=mid){
        tmp[cnt]=a[pl];
        cnt++;pl++;
    }
    while(pr<=r){
        tmp[cnt]=a[pr];
        cnt++;pr++;
    }
    for(int i=l;i<=r;i++)
        a[i]=tmp[i-l];
    delete[] tmp;
}
int get_ticket(train &x,int t,int l,int r){
    if(l>=r)
        return 0;
    int mn=114514;
    seat_ticket mp;
    seat_manager.read(mp,x.seat_pos[t]);
    for(int i=l;i<r;i++)
        mn=std::min(mn,mp.seat[i]);
    return mn;
}
void del_ticket(train &x,int t,int l,int r,int d){
    if(l>=r)
        return;
    seat_ticket mp;
    seat_manager.read(mp,x.seat_pos[t]);
    for(int i=l;i<r;i++)
        mp.seat[i]-=d;
    seat_manager.update(mp,x.seat_pos[t]);
}
void query_ticket(){
    node x,y;
    start_train_num=stop_train_num=0;
    bool opt=false;
    string day;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-s")
            fsta(token[i+1],x.index);
        if(token[i]=="-t")
            fsta(token[i+1],y.index);
        if(token[i]=="-d")
            day=token[i+1];
        if(token[i]=="-p"){
            if(token[i+1]=="cost")
                opt=true;
        }
    }
    train_start.find(x,start_train_id,start_train,start_train_num);
    train_start.find(y,stop_train_id,stop_train,stop_train_num);
    int pl=0,pr=0,l=0;
    while(pl<start_train_num&&pr<stop_train_num){
        if(strcmp(start_train_id[pl],stop_train_id[pr])==0){
            if(day<fats(start_train[pl].sale_date[0])||day>fats(start_train[pl].sale_date[1])||start_train[pl].in>=stop_train[pr].in){
                pl++;
                pr++;
            }
            else{
                train b;
                node z;
                char_array_assign(z.index,start_train_id[pl]);
                train_id.find(z,b);
                int t1,t2;
                calc(day,start_train[pl].starttime,t1);
                if(start_train[pl].in)
                    t1-=(b.travel_time[start_train[pl].in-1]+b.stop_over_time[start_train[pl].in-1]);
                t1=t1-t1%1440;
                calc(fats(b.sale_date[0]),"00:00",t2);
                char_array_assign(satisfy_train_id[l],start_train_id[pl]);
                satisfy_start_train[l]=start_train[pl];
                satisfy_stop_train[l]=stop_train[pr];
                seat[l]=get_ticket(b,(t1-t2)/1440,start_train[pl].in,stop_train[pr].in);
                pl++;
                pr++;
                l++;
            }
        }
        else{
            if(strcmp(start_train_id[pl],stop_train_id[pr])<0)
                pl++;
            else
                pr++;
        }
    }
    if(l==0){
        cout<<"0\n";
        return;
    }
    for(int i=0;i<l;i++)
        id[i]=i;
    if(opt==1)
        mergesort(id,0,l-1,[](const int &x,const int &y)->bool{
            if(satisfy_stop_train[x].price-satisfy_start_train[x].price!=satisfy_stop_train[y].price-satisfy_start_train[y].price)
                return satisfy_stop_train[x].price-satisfy_start_train[x].price<satisfy_stop_train[y].price-satisfy_start_train[y].price;
            else
                return strcmp(satisfy_train_id[x],satisfy_train_id[y])<0;
        });
    else
        mergesort(id,0,l-1,[](const int &x,const int &y)->bool{
            if(satisfy_stop_train[x].tim-satisfy_start_train[x].tim-satisfy_stop_train[x].stop_over_time!=
            satisfy_stop_train[y].tim-satisfy_start_train[y].tim-satisfy_stop_train[y].stop_over_time)
                return satisfy_stop_train[x].tim-satisfy_start_train[x].tim-satisfy_stop_train[x].stop_over_time<
                satisfy_stop_train[y].tim-satisfy_start_train[y].tim-satisfy_stop_train[y].stop_over_time;
            else
                return strcmp(satisfy_train_id[x],satisfy_train_id[y])<0;
        });
    cout<<l<<'\n';
    for(int i=0;i<l;i++){
        cout<<satisfy_train_id[id[i]]<<" "<<x.index<<" "<<day<<" "<<satisfy_start_train[id[i]].starttime<<" -> "<<y.index<<" ";
        int t;
        calc(day,satisfy_start_train[id[i]].starttime,t);
        t+=satisfy_stop_train[id[i]].tim-satisfy_start_train[id[i]].tim-satisfy_stop_train[id[i]].stop_over_time;
        string new_day,new_inday;
        rev_calc(t,new_day,new_inday);
        cout<<new_day<<" "<<new_inday<<" "<<satisfy_stop_train[id[i]].price-satisfy_start_train[id[i]].price<<" "<<seat[id[i]]<<"\n";
    }
}
bool compare(int &best_time,int &best_cost,char best_train1[],char best_train2[],int &time,int &cost,char train1[],char train2[]){
    if(best_time!=time)
        return time<best_time;
    if(best_cost!=cost)
        return cost<best_cost;
    int k=strcmp(best_train1,train1);
    if(k!=0)
        return k>0;
    k=strcmp(best_train2,train2);
    return k>0;
}
void query_transfer(){
    node x,y;
    start_train_num=0;
    bool opt=false;
    string day;
    for(int i=2;i<len;i++){
        if(token[i]=="-s")
            fsta(token[i+1],x.index);
        if(token[i]=="-t")
            fsta(token[i+1],y.index);
        if(token[i]=="-d")
            day=token[i+1];
        if(token[i]=="-p"){
            if(token[i+1]=="cost")
                opt=true;
        }
    }
    train_start.find(x,start_train_id,start_train,start_train_num);
    bool can=false;
    int best_time=114514191,best_cost=114514191,seat,seat1,T1,T2,T3,c,c1;
    char best_train_id[2][31],transfer[31];
    for(int i=0;i<start_train_num;i++){
        if(day<fats(start_train[i].sale_date[0])||day>fats(start_train[i].sale_date[1]))
            continue;
        train a;
        node z;
        char_array_assign(z.index,start_train_id[i]);
        train_id.find(z,a);
        if(start_train[i].in==a.station_num-1)
            continue;
        int d;
        calc(day,fats(start_train[i].starttime),d);
        if(start_train[i].in)
            d-=(a.travel_time[start_train[i].in-1]+a.stop_over_time[start_train[i].in-1]);
        d=d-d%1440;
        int d1;
        calc(fats(a.sale_date[0]),"00:00",d1);
        seat_ticket mp;
        seat_manager.read(mp,a.seat_pos[(d-d1)/1440]);
        int T,cost=0,now_seat=114514;
        calc(day,fats(start_train[i].starttime),T);
        now_seat=std::min(now_seat,mp.seat[start_train[i].in]);
        for(int j=start_train[i].in+1;j<a.station_num;j++){
            int TT=T+(a.travel_time[j-1]-(start_train[i].in?a.travel_time[start_train[i].in-1]:0));
            if(j-start_train[i].in>=2)
                TT+=a.stop_over_time[j-2]-(start_train[i].in?a.stop_over_time[start_train[i].in-1]:0);
            cost=(a.price[j-1]-(start_train[i].in?a.price[start_train[i].in-1]:0));
            string new_day,new_inday;
            rev_calc(TT,new_day,new_inday);
            node xx;
            char_array_assign(xx.index,a.station[j]);
            stop_train_num=0;
            train_start.find(xx,stop_train_id,stop_train,stop_train_num);
            for(int k=0;k<stop_train_num;k++){
                if(strcmp(stop_train_id[k],start_train_id[i])==0)
                    continue;
                int TTT,now_seat1=114514;
                bool flg=0;
                string New_day=new_day;
                if(new_day>fats(stop_train[k].sale_date[1]))
                    continue;
                if(new_day==fats(stop_train[k].sale_date[1])&&new_inday>fats(stop_train[k].starttime))
                    continue;
                if(new_day<fats(stop_train[k].sale_date[0])){
                    New_day=fats(stop_train[k].sale_date[0]);
                    flg=1;
                }
                calc(New_day,fats(stop_train[k].starttime),TTT);
                if(new_inday>fats(stop_train[k].starttime)&&!flg)
                    TTT+=1440;
                train b;
                node zz;
                char_array_assign(zz.index,stop_train_id[k]);
                train_id.find(zz,b);
                if(stop_train[k].in==b.station_num-1)
                    continue;
                seat_ticket mp1;
                int D,D1;
                int T5=TTT;
                if(stop_train[k].in)
                    T5-=(b.travel_time[stop_train[k].in-1]+b.stop_over_time[stop_train[k].in-1]);
                D=T5-T5%1440;
                calc(fats(b.sale_date[0]),"00:00",D1);
                seat_manager.read(mp1,b.seat_pos[(D-D1)/1440]);
                now_seat1=std::min(now_seat1,mp1.seat[stop_train[k].in]);
                for(int kk=stop_train[k].in+1;kk<b.station_num;kk++){
                    if(strcmp(b.station[kk],y.index)==0){
                        int cost1=cost;
                        can=true;
                        int TTTT=TTT+(b.travel_time[kk-1]-(stop_train[k].in?b.travel_time[stop_train[k].in-1]:0));
                        if(kk-stop_train[k].in>=2)
                            TTTT+=b.stop_over_time[kk-2]-(stop_train[k].in?b.stop_over_time[stop_train[k].in-1]:0);
                        cost1+=(b.price[kk-1]-(stop_train[k].in?b.price[stop_train[k].in-1]:0));
                        TTTT-=T;
                        if(opt){
                            if(compare(best_cost,best_time,best_train_id[0],best_train_id[1],cost1,TTTT,a.train_id,b.train_id)){
                                best_cost=cost1;
                                best_time=TTTT;
                                char_array_assign(best_train_id[0],a.train_id);
                                char_array_assign(best_train_id[1],b.train_id);
                                char_array_assign(transfer,a.station[j]);
                                seat=now_seat;
                                seat1=now_seat1;
                                T1=TT;
                                T2=TTT;
                                T3=TTTT+T;
                                c=cost;
                                c1=cost1-cost;
                            }
                        }
                        else{
                            if(compare(best_time,best_cost,best_train_id[0],best_train_id[1],TTTT,cost1,a.train_id,b.train_id)){
                                best_cost=cost1;
                                best_time=TTTT;
                                char_array_assign(best_train_id[0],a.train_id);
                                char_array_assign(best_train_id[1],b.train_id);
                                char_array_assign(transfer,a.station[j]);
                                seat=now_seat;
                                seat1=now_seat1;
                                T1=TT;
                                T2=TTT;
                                T3=TTTT+T;
                                c=cost;
                                c1=cost1-cost;
                            }
                        }
                    }
                    if(kk!=b.station_num-1)
                        now_seat1=std::min(now_seat1,mp1.seat[kk]);
                }
            }
            if(j!=a.station_num-1)
                now_seat=std::min(now_seat,mp.seat[j]);
        }
    }
    if(!can){
        cout<<"0\n";
        return;
    }
    else{
        int T;
        train_s a,b;
        node z;
        char_array_assign(z.index,x.index);
        char_array_assign(z.val,best_train_id[0]);
        train_start.findone(z,a);
        char_array_assign(z.index,transfer);
        char_array_assign(z.val,best_train_id[1]);
        train_start.findone(z,b);
        cout<<best_train_id[0]<<" "<<x.index<<" ";
        cout<<day<<" "<<a.starttime<<" -> "<<transfer<<" ";
        string new_day,new_inday;
        rev_calc(T1,new_day,new_inday);
        cout<<new_day<<" "<<new_inday<<" "<<c<<" "<<seat<<'\n';
        cout<<best_train_id[1]<<" "<<transfer<<" ";
        rev_calc(T2,new_day,new_inday);
        cout<<new_day<<" "<<new_inday<<" -> "<<y.index<<" ";
        rev_calc(T3,new_day,new_inday);
        cout<<new_day<<" "<<new_inday<<" "<<c1<<" "<<seat1<<'\n';
    }
}
int order_cnt;
void fnta(char str[],int n){
    for(int i=6;i>=0;i--){
        str[i]=(char)(n%10+'0');
        n/=10;
    }
    str[7]='\0'; 
}
void buy_ticket(){
    order a;
    node x,y,z;
    string day;
    bool flg=false;
    a.order_id=order_cnt++;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-u"){
            fsta(token[i+1],x.index);
            fsta(token[i+1],a.username);
        }
        if(token[i]=="-i"){
            fsta(token[i+1],y.index);
            fsta(token[i+1],a.train_id);
        }
        if(token[i]=="-d")
            day=token[i+1];
        if(token[i]=="-n")
            fstn(token[i+1],a.num);
        if(token[i]=="-f")
            fsta(token[i+1],a.from);
        if(token[i]=="-t")
            fsta(token[i+1],a.to);
        if(token[i]=="-q"){
            if(token[i+1]=="true")
                flg=true;
        }
    }
    account ac;
    if(!acc.find(x,ac)){
        call_invalid();
        return;
    }
    if(!vis[ac.id]){
        call_invalid();
        return;
    }
    train b;
    train_id.find(y,b);
    if(!b.is_release){
        call_invalid();
        return;
    }
    a.from_in=a.to_in=-1;
    for(int i=0;i<b.station_num;i++){
        if(strcmp(b.station[i],a.from)==0)
            a.from_in=i;
        if(strcmp(b.station[i],a.to)==0)
            a.to_in=i;
    }
    if(a.from_in>a.to_in||a.from_in==-1||a.to_in==-1){
        call_invalid();
        return;
    }
    train_s c,d;
    char_array_assign(z.index,a.from);
    char_array_assign(z.val,b.train_id);
    train_start.findone(z,c);
    char_array_assign(z.index,a.to);
    train_start.findone(z,d);
    if(day<fats(c.sale_date[0])||day>fats(c.sale_date[1])){
        call_invalid();
        return;
    }
    int d1,d2;
    calc(fats(b.sale_date[0]),"00:00",d1);
    int t1;
    calc(day,fats(c.starttime),t1);
    int t3=t1;
    if(c.in!=0)
        t3-=(b.travel_time[c.in-1]+b.stop_over_time[c.in-1]);
    d2=t3-t3%1440;
    fsta(day,a.leaving_day);
    char_array_assign(a.leaving_inday,c.starttime);
    int can=get_ticket(b,(d2-d1)/1440,c.in,d.in);
    int t2=t1;
    t2+=b.travel_time[d.in-1]-(c.in?b.travel_time[c.in-1]:0);
    if(d.in-c.in>=2)
        t2+=b.stop_over_time[d.in-2]-(c.in>=1?b.stop_over_time[c.in-1]:0);
    string new_day,new_inday;
    rev_calc(t2,new_day,new_inday);
    fsta(new_day,a.arriving_day);
    fsta(new_inday,a.arriving_inday);
    if(can<a.num&&!flg){
        call_invalid();
        return;
    }
    else if(can<a.num&&flg){
        cout<<"queue\n";
        a.status=0;
        a.price=b.price[a.to_in-1]-(a.from_in?b.price[a.from_in-1]:0);
        node u;
        char_array_assign(u.index,x.index);
        fnta(u.val,a.order_id);
        ord.insert(u,a);
        char_array_assign(u.index,y.index);
        ref.insert(u,a);
    }
    else{
        del_ticket(b,(d2-d1)/1440,c.in,d.in,a.num);
        a.price=b.price[a.to_in-1]-(a.from_in?b.price[a.from_in-1]:0);
        cout<<1ll*a.price*a.num<<"\n";
        a.status=1;
        node u;
        char_array_assign(u.index,x.index);
        fnta(u.val,a.order_id);
        ord.insert(u,a);
    }
}
int ordlist[1000010];
int ord_cnt;
void query_order(){
    ord_cnt=0;
    node x;
    fsta(token[3],x.index);
    account a;
    if(!acc.find(x,a)){
        call_invalid();
        return;
    }
    if(!vis[a.id]){
        call_invalid();
        return;
    }
    ord.find(x,ordlist,ord_cnt);
    order b;
    cout<<ord_cnt<<'\n';
    for(int i=ord_cnt-1;i>=0;i--){
        ord.file_info.read(b,ordlist[i]);
        if(b.status==0)
            cout<<"[pending] ";
        if(b.status==1)
            cout<<"[success] ";
        if(b.status==2)
            cout<<"[refunded] ";
        cout<<b.train_id<<" "<<b.from<<" "<<b.leaving_day<<" "<<b.leaving_inday<<" -> "<<b.to<<" "<<b.arriving_day<<" "<<b.arriving_inday<<" "<<b.price<<" "<<b.num<<'\n';
    }
}
void refund_ticket(){
    int N=1;
    ord_cnt=0;
    node x;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-u")
            fsta(token[i+1],x.index);
        if(token[i]=="-n")
            fstn(token[i+1],N);
    }
    account a;
    if(!acc.find(x,a)){
        call_invalid();
        return;
    }
    if(!vis[a.id]){
        call_invalid();
        return;
    }
    ord.find(x,ordlist,ord_cnt);
    order b;
    if(ord_cnt<N){
        call_invalid();
        return;
    }
    ord.file_info.read(b,ordlist[ord_cnt-N]);
    if(b.status==2){
        call_invalid();
        return;
    }
    call_valid();
    if(b.status==1){
        b.status=2;
        ord.file_info.update(b,b.pos);
        int t;
        calc(fats(b.leaving_day),fats(b.leaving_inday),t);
        train c;
        node y;
        char_array_assign(y.index,b.train_id);
        train_id.find(y,c);
        int d1,d2;
        calc(fats(c.sale_date[0]),"00:00",d2);
        if(b.from_in!=0)
            t-=(c.travel_time[b.from_in-1]+c.stop_over_time[b.from_in-1]);
        d1=t-t%1440;
        del_ticket(c,(d1-d2)/1440,b.from_in,b.to_in,-b.num);
        char_array_assign(x.index,b.train_id);
        ord_cnt=0;
        ref.find(x,ordlist,ord_cnt);
        order d;
        for(int i=0;i<ord_cnt;i++){
            ref.file_info.read(d,ordlist[i]);
            calc(fats(d.leaving_day),fats(d.leaving_inday),t);
            int d1,d2;
            calc(fats(c.sale_date[0]),"00:00",d2);
            if(d.from_in!=0)
                t-=(c.travel_time[d.from_in-1]+c.stop_over_time[d.from_in-1]);
            d1=t-t%1440;
            int can=get_ticket(c,(d1-d2)/1440,d.from_in,d.to_in);
            if(can>=d.num){
                del_ticket(c,(d1-d2)/1440,d.from_in,d.to_in,d.num);
                fnta(x.val,d.order_id);
                ref.del(x);
                node z;
                char_array_assign(z.index,d.username);
                char_array_assign(z.val,x.val);
                order e;
                ord.findone(z,e);
                e.status=1;
                ord.file_info.update(e,e.pos);
            }
        }
    }
    else{
        b.status=2;
        char_array_assign(x.index,b.train_id);
        fnta(x.val,b.order_id);
        ref.del(x);
        ord.file_info.update(b,b.pos);
    }
}
void clean(){
    for(int i=0;i<account_cnt;i++)
        vis[i]=false;
    order_cnt=0;account_cnt=0;
    acc.clean();
    ord.clean();
    train_id.clean();
    train_start.clean();
    ref.clean();
    seat_manager.initialise("seat");
}
void init(){
    seat_manager.file.open("seat",std::ios::in|std::ios::out|std::ios::binary);
    if(!seat_manager.file.good())
        seat_manager.initialise("seat");
    else
        seat_manager.file.close();
    seat_manager.sizeofT=sizeof(seat_ticket);
}
int main(){
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    acc.initialise("acc");
    ord.initialise("ord");
    train_id.initialise("id");
    train_start.initialise("start");
    ref.initialise("ref");
    init();
    seat_manager.get_info(order_cnt,1);
    seat_manager.get_info(account_cnt,2);
    // if(order_cnt)
    //     F=1;
    getline(cin,s);
    get_token(s);
    while(token[1]!="exit"){
        if(token[1]=="add_user")
            add_user();
        else if(token[1]=="login")
            login();
        else if(token[1]=="logout")
            logout();
        else if(token[1]=="query_profile")
            query_profile();
        else if(token[1]=="modify_profile")
            modify_profile();
        else if(token[1]=="add_train")
            add_train();
        else if(token[1]=="delete_train")
            delete_train();
        else if(token[1]=="release_train")
            release_train();
        else if(token[1]=="query_train")
            query_train();
        else if(token[1]=="query_ticket")
            query_ticket();
        else if(token[1]=="query_transfer")
            query_transfer();
        else if(token[1]=="buy_ticket")
            buy_ticket();
        else if(token[1]=="query_order")
            query_order();
        else if(token[1]=="refund_ticket")
            refund_ticket();
        else if(token[1]=="clean")
            clean();
        getline(cin,s);
        get_token(s);
    }
    cout<<"bye\n";
    seat_manager.write_info(order_cnt,1);
    seat_manager.write_info(account_cnt,2);
    acc.file_bpt.write_info(acc.root,1);
    ref.file_bpt.write_info(ref.root,1);
    ord.file_bpt.write_info(ord.root,1);
    train_id.file_bpt.write_info(train_id.root,1);
    train_start.file_bpt.write_info(train_start.root,1);
    return 0;
}