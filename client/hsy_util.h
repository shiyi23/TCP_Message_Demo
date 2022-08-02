#pragma once

typedef unsigned char UCHAR8;
typedef unsigned short UINT16;
typedef long long INT64;
typedef char CHAR8;
typedef short INT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef double REAL8;
typedef int BOOL;

#define SCR_CODE_LEN    32

#pragma pack(push)
#pragma pack(8)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// 二进制存储的文件头信息
struct save_file_head
{
    int data_count;     // number of quote data items
    short data_type;    // 行情类型标识
    short data_length;  // length of one item (byte)
};

template<typename T>
struct save_file_record
{
    long long t_;           // 时间戳
    T data_;                // 数据对象 （8字节对齐）

    // 缺省构造
    save_file_record()
    {
        t_ = 0;
    }

    // 通过时间戳、和网络数据包构造
    save_file_record(long long t, const T &d)
        : t_(t), data_(d)
    {
    }
};

template <typename T, typename H>
struct my_shm_record
{
    long type;
    long flag;
    T data;
    H expand;
};

struct quote_expand_record
{
    long t0; //行情收到时间戳
    long t1; //行情发送时间戳
    long recv_time; //行情收到时间戳
    char reserve[56];
};

//股票level2数据
struct TDF_MARKET_DATA_MY
{
    char szWindCode[32];         //600001.SH
    char szCode[32];             //原始Code
    int nActionDay;             //业务发生日(自然日)
    int nTradingDay;            //交易日
    int nTime;					//时间(HHMMSSmmm)
    int nStatus;				//状态
    unsigned int nPreClose;				//前收盘价 * 10000
    unsigned int nOpen;					//开盘价 * 10000
    unsigned int nHigh;					//最高价 * 10000
    unsigned int nLow;					//最低价 * 10000
    unsigned int nMatch;				//最新价 * 10000
    unsigned int nAskPrice[10];			//申卖价 * 10000
    unsigned int nAskVol[10];			//申卖量
    unsigned int nBidPrice[10];			//申买价 * 10000
    unsigned int nBidVol[10];			//申买量
    unsigned int nNumTrades;			//成交笔数
    long long iVolume;				//成交总量
    long long iTurnover;				//成交总金额
    long long nTotalBidVol;			//委托买入总量
    long long nTotalAskVol;			//委托卖出总量
    unsigned int nWeightedAvgBidPrice;	//加权平均委买价格 * 10000
    unsigned int nWeightedAvgAskPrice;  //加权平均委卖价格 * 10000
    int nIOPV;					//IOPV净值估值  * 10000 （基金）
    int nYieldToMaturity;		//到期收益率	* 10000	（债券）
    unsigned int nHighLimited;			//涨停价 * 10000
    unsigned int nLowLimited;			//跌停价 * 10000
    char chPrefix[4];			//证券信息前缀
    int nSyl1;					//市盈率1	未使用（当前值为0）
    int nSyl2;					//市盈率2	未使用（当前值为0）
    int nSD2;					//升跌2（对比上一笔）	未使用（当前值为0）
};

//指数level2数据
struct TDF_INDEX_DATA_MY
{
    char szWindCode[32];         //600001.SH
    char szCode[32];             //原始Code
    int nActionDay;             //业务发生日(自然日)
    int nTradingDay;            //交易日
    int nTime;			//时间(HHMMSSmmm)
    int nOpenIndex;		//今开盘指数
    int nHighIndex;		//最高指数
    int nLowIndex;		//最低指数
    int nLastIndex;		//最新指数
    long long iTotalVolume;	//参与计算相应指数的交易数量
    long long iTurnover;		//参与计算相应指数的成交金额
    int nPreCloseIndex;	//前盘指数
};

// 实时逐笔成交
struct T_PerBargain
{
    int time;                     // 成交时间
    int market;
    char scr_code[32];
    long long bgn_id;             // 成交编号
    int bgn_price;                // 成交价格
    long long bgn_qty;            // 成交数量
    long long bgn_amt;            // 成交金额

    char bgn_flg[4];              // 成交类别(深圳填'0',上海不填)
    char nsr_txn_tp_code[4];      // 指令交易类型('B','S')
    char function_code[4];        // 成交代码(成交'0',撤销'C')
    int ask_order;	              // 叫卖方委托序号
    int bid_order;	              // 叫买方委托序号  
};

// 实时逐笔委托
struct T_PerEntrust
{
    INT32 market;
    CHAR8 scr_code[SCR_CODE_LEN];
    INT32 entrt_time;               // 委托时间
    INT32 entrt_price;              // 委托价格
    INT64 entrt_id;                 // 委托编号
    INT64 entrt_vol;                // 委托数量

    CHAR8 insr_txn_tp_code[4];      // 指令交易类型 'B','S'
    CHAR8 entrt_tp[4];              // 委托类别
};

// 逐笔成交
struct quote_trade_t
{
    uint16_t channel;           // 通道
    char security_id[9];        // 证券代码
    uint8_t market;             // 交易所: '0'(48)深圳; '1'(49)上海
    int32_t trade_time;         // 成交时间
    int32_t trade_price;        // 成交价格
    char bs_flag;               // 买卖方向: 'B'买;'S'卖
    char trade_type;            // 成交类别: '0'成交;'C'撤单
    int64_t trade_index;        // 成交序号
    int64_t trade_qty;          // 成交数量
    int64_t trade_amount;       // 成交金额
    int64_t ask_order_index;    // 卖方委托序号
    int64_t bid_order_index;    // 买方委托序号
    int64_t biz_index;          // 业务序列号
};

// 逐笔委托
struct quote_order_t
{
    char order_type;            // 订单类型: 深交所:'0'限价;'1'市价;'U'本方最优 上交所:'A':新增,'D'删除
    uint8_t market;             // 交易所: '0'(48)深圳; '1'(49)上海
    char bs_flag;               // 买卖方向: 'B'买;'S'卖
    char security_id[9];        // 证券代码
    int32_t order_time;         // 委托时间
    int64_t order_index;        // 委托序号
    int64_t order_qty;          // 委托数量
    int64_t ori_order_no;       // 原始订单号
    int64_t biz_index;          // 业务序列号
    int32_t order_price;        // 委托价格
    uint16_t channel;           // 通道
};


typedef my_shm_record<TDF_MARKET_DATA_MY, quote_expand_record> my_stock_record;
typedef my_shm_record<TDF_INDEX_DATA_MY, quote_expand_record> my_index_record;
typedef my_shm_record<T_PerEntrust, quote_expand_record> my_entrust_record;

typedef my_shm_record<quote_trade_t, quote_expand_record> my_trade_tick_record;
typedef my_shm_record<quote_order_t, quote_expand_record> my_order_tick_record;



template <typename T>
void print_shm(T* data);

template <>
void print_shm<my_stock_record>(my_stock_record* d);

template <>
void print_shm<my_index_record>(my_index_record* d);

template <>
void print_shm<my_entrust_record>(my_entrust_record* d);

