#ifndef _TRAIN_H
#define _TRAIN_H
#include<cstring>

class train{
public:
    char train_id[21],station[30][31],sale_date[2][6],starttime[6],type;
    int station_num,seat_num,price[30],travel_time[30],stop_over_time[30],pos,seat_pos[92],id_cnt;
    bool is_release;
    train(){
        is_release=false;
    }
};

class train_s{
public:
    int price,pos,stop_over_time,in,tim,id_cnt;
    char train_id[21],starttime[6],sale_date[2][6];
    bool operator <(const train_s &x)const{
        return strcmp(train_id,x.train_id)<0;
    }
    bool operator ==(const train_s &x)const{
        return strcmp(train_id,x.train_id)==0;
    }
};

#endif