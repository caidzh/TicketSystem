#ifndef _ORDER_H
#define _ORDER_H

class order{
public:
    int order_id,status,price,num;
    char train_id[21],from[31],to[31],leaving[20],arriving[20];
    int in_stack_pos,pos;
    bool operator <(const order &x)const{
        return order_id<x.order_id;
    }
};

#endif