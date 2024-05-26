#include "bpt.hpp"
#include "account.h"
#include "train.h"
using std::cin;
using std::cout;
string s;
int account_cnt;
string token[20];
int len;
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
bpt<account>acc("acc");
bpt<order>ord("ord");
bpt<train>train_id("id"),train_start("start");
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
    if(acc.find(y,a)==false&&(account_cnt==0||(c.privilege>a.privilege&&c.is_login))){
        acc.insert(y,a);
        account_cnt++;
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
    if(strcmp(c.password,a.password)!=0||c.is_login){
        call_invalid();
        return;
    }
    c.is_login=true;
    acc.file_info.update(c,c.pos);
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
    if(a.is_login==false){
        call_invalid();
        return;
    }
    a.is_login=false;
    acc.file_info.update(a,a.pos);
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
    if(c.is_login&&(strcmp(a.username,c.username)==0||a.privilege<c.privilege))
        cout<<a.username<<" "<<a.name<<" "<<a.mailAddr<<" "<<a.privilege<<"\n";
    else
        call_invalid();
}
void modify_profile(){
    node x,y;
    account a,c,new_a;
    for(int i=2;i<len;i+=2){
        if(token[i]=="-c"){
            fsta(token[i+1],x.index);
            if(acc.find(x,c)==false){
                call_invalid();
                return;
            }
            if(c.is_login==false){
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
        if(token[i]=="-g")
            fstn(token[i+1],new_a.privilege);
    }
    if((strcmp(c.username,a.username)==0||c.privilege>a.privilege)&&c.privilege>new_a.privilege){
        acc.file_info.update(new_a,a.pos);
        cout<<new_a.username<<" "<<new_a.name<<" "<<new_a.mailAddr<<" "<<new_a.privilege<<"\n";
    }
    else
        call_invalid();
}
void add_train(){

}
void delete_train(){

}
void release_train(){

}
void query_train(){

}
void query_ticket(){

}
void query_transfer(){

}
void buy_ticket(){

}
void query_order(){

}
void refund_ticket(){

}
void clean(){

}
int main(){
    freopen("1.in","r",stdin);
    freopen("out","w",stdout);
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    acc.initialise("acc");
    ord.initialise("ord");
    train_id.initialise("id");
    train_start.initialise("start");
    getline(cin,s);
    get_token(s);
    while(token[1]!="exit"){
        if(token[1]=="add_user")
            add_user();
        if(token[1]=="login")
            login();
        if(token[1]=="logout")
            logout();
        if(token[1]=="query_profile")
            query_profile();
        if(token[1]=="modify_profile")
            modify_profile();
        if(token[1]=="add_train")
            add_train();
        if(token[1]=="delete_train")
            delete_train();
        if(token[1]=="release_train")
            release_train();
        if(token[1]=="query_train")
            query_train();
        if(token[1]=="query_ticket")
            query_ticket();
        if(token[1]=="query_transfer")
            query_transfer();
        if(token[1]=="buy_ticket")
            buy_ticket();
        if(token[1]=="query_order")
            query_order();
        if(token[1]=="refund_ticket")
            refund_ticket();
        if(token[1]=="clean")
            clean();
        getline(cin,s);
        get_token(s);
    }
    cout<<"bye\n";
    return 0;
}