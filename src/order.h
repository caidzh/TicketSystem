#ifndef _ORDER_H
#define _ORDER_H

class order{
public:
    int order_id,status,num,from_in,to_in,pos,price;
    char train_id[21],from[31],to[31],leaving_day[6],leaving_inday[6],arriving_day[6],arriving_inday[6],username[21];
    bool operator <(const order &x)const{
        return order_id<x.order_id;
    }
};

#endif