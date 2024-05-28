#ifndef _TRAIN_H
#define _TRAIN_H

class train{
public:
    char train_id[21],station[100][31],sale_date[2][6],starttime[6],type;
    int station_num,seat_num,price[100],travel_time[100],stop_over_time[100],pos,seat_pos;
    bool is_release;
    train(){
        is_release=false;
    }
};

class train_s{
public:
    int price,pos,stop_over_time,in,tim;
    char starttime[6],sale_date[2][6];
    //leave time
};

#endif