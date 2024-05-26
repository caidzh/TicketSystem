#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include "order.h"
#include "list.hpp"

class account{
public:
    char username[21],password[31],name[20],mailAddr[31];
    int privilege,order_cnt,pos;
    bool is_login;
    account(){
        privilege=-1;
        order_cnt=0;
        is_login=false;
    }
};

#endif