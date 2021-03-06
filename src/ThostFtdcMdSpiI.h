/////////////////////////////////////////////////////////////////////////
///@system ctp行情nodejs addon
///@company 慧网基金
///@file ThostFtdcMdApiSpi.h
///@brief ctp线程及回调
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __THOSTFTDCMDSPI_H__
#define __THOSTFTDCMDSPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <set>
#include <map>
#include <string>
#include <sys/time.h>
#include <uv.h>

// CTP
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"

using namespace std;

namespace md
{

class CThostFtdcMdSpiI;

//以api来划分结构体
struct taskdata
{
    taskdata(CThostFtdcMdSpiI* p){work.data = this; pmd = p;}
    CThostFtdcMdSpiI* pmd;
    string api;             //表示是那个api回调
    uv_work_t work;

    union _data 
    {
        int nReason;
        int nTimeLapse;
        CThostFtdcRspUserLoginField RspUserLogin;
        CThostFtdcUserLogoutField UserLogout;
        CThostFtdcSpecificInstrumentField SpecificInstrument;
        CThostFtdcDepthMarketDataField DepthMarketData;
        CThostFtdcForQuoteRspField ForQuoteRsp;
    }data;
    int nRequestID;
    bool bIsLast;
    CThostFtdcRspInfoField RspInfo; //公用返回
};

class CThostFtdcMdSpiI : public CThostFtdcMdSpi 
{

    public:
        CThostFtdcMdSpiI();
        ~CThostFtdcMdSpiI();

        //主线程回调js处理
        virtual void MainOnFrontConnected() = 0;
        virtual void MainOnFrontDisconnected(int nReason) = 0;
        virtual void MainOnHeartBeatWarning(int nTimeLapse) = 0;
        virtual void MainOnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
        virtual void MainOnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) = 0;
        virtual void MainOnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) = 0;
        

        virtual void OnFrontConnected();
        virtual void OnFrontDisconnected(int nReason);
        virtual void OnHeartBeatWarning(int nTimeLapse);
        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;
        virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;
        
        CThostFtdcMdApi* GetMdApi(){return m_pApi;}

    private:
        static void _on_completed(uv_work_t * work, int);
        static void _on_async_queue(uv_work_t * work);

        CThostFtdcMdApi* 	m_pApi; 		        //交易请求结构体
        uv_async_t async_t;
};
}

#endif






