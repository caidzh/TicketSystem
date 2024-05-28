#include<bits/stdc++.h>
using namespace std;
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
    delete tmp;
}
int a[30];
int main(){
    int n=20;
    for(int i=0;i<n;i++)
        a[i]=i;
    random_shuffle(a,a+n);
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";
    cout<<endl;
    mergesort(a,0,n-1,[](const int &x,const int &y)->bool{
        return x<y;
    });
    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";cout<<endl;
    return 0;
}