#include "hsy_util.h"

template <typename T>
void print_shm(T* data)
{
    printf("this data not find print\n");
}

template <>
void print_shm<my_stock_record>(my_stock_record* d)
{
    printf("stock lv2 : %d|%s|%d|%d|\n", d->data.nTime, d->data.szCode, d->data.nBidPrice[0], d->data.nAskPrice[0]);
}

template <>
void print_shm<my_index_record>(my_index_record* d)
{
    printf("stock index : %d|%s|%d|\n", d->data.nTime, d->data.szWindCode, d->data.nLastIndex);
}

template <>
void print_shm<my_entrust_record>(my_entrust_record* d)
{
        printf("entrust : %d|%s|%d|%lld|%lld|%s|%s|\n", d->data.entrt_time, d->data.scr_code, d->data.entrt_price, 
      d->data.entrt_id, d->data.entrt_vol, d->data.insr_txn_tp_code, d->data.entrt_tp);
}


