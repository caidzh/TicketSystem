#ifndef _TRAIN_H
#define _TRAIN_H

class train{
public:
    char train_id[21],station[100][31],sale_date[2][5],starttime[5],type;
    int station_num,seat_num,price[100],travel_time[100],stop_over_time[100],pos;
    bool is_release;
};

#endif