#ifndef _HCI_INTF_H
#define _HCI_INTF_H

#include "hci_def.h"
#include "hci_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


//回调函数定义
typedef INT (*ai_analyse_callback)(UINT nCh, UINT nDestModuleType, INT nAnalyseType,  VOID* pAnalyseData, UINT nAnalyseLen);

typedef INT (*hcm_msg_callback)(LPHCM_MSG_HEAD_S lpMsgHead,const VOID* lpMsgData, UINT nMsgLen,VOID** pResultMsg, UINT *pResultLen, VOID* pContext);

typedef INT (*hcm_local_callback)(HCI_LOCAL_TYPE_E eLocalType,const VOID* lpMsgData, UINT nMsgLen, VOID* pContext);

typedef INT (*hcm_paramChange_callback)(HCI_CONFIG_TYPE_E eCfgType,const VOID* lpMsgData, UINT nMsgLen, VOID* pContext);

typedef INT (*hcm_upgrade_progress_callback)(UINT nProgress, VOID* pContext); //升级进度回调  0~100  100为升级完成

typedef INT (*hcm_Wiegand_ReadData_callback)(HCM_Wiegand_Data* nWriteBuff); //韦根数据读取回调

typedef INT (*hcm_IDcard_ReadData_callback)(HCI_IDCARD_DATA_S* nReadBuff);

//static INT Intf_GetParam(HCP_CONFIG_TYPE_E nCfgType, UINT nCh, void * lpParam, UINT iParamLen);
//static INT Intf_SetParam(HCP_CONFIG_TYPE_E nCfgType, UINT nCh, void * lpParam, UINT iParamLen);


//拷贝到模块的函数集
typedef struct _HCI_FUNC
{
	CHAR*		(*Intf_GetVersion)(VOID);
	INT			(*Intf_GetParam)(HCI_CONFIG_TYPE_E nCfgType, UINT nCh, VOID * lpParam, UINT iParamLen);
	INT			(*Intf_SetParam)(HCI_CONFIG_TYPE_E nCfgType, UINT nCh, VOID * lpParam, UINT iParamLen);
	INT 		(*Intf_GetDefaultParam)(HCI_CONFIG_TYPE_E nCfgType, UINT nCh, VOID * lpParam, UINT iParamLen);

	//模块间的消息传送， lpMsgHead 的 u32MsgType 的用户自定义类型, lpMsgData数据用户自定义
	//SendModuleMsg 在当前模块发送给指定模块消息
	INT			(*Intf_SendModuleMsg)(LPHCM_MSG_HEAD_S lpMsgHead,const VOID* lpMsgData, UINT nMsgLen,VOID** pResultMsg, UINT *pResultLen);
	//lpCallback 回调函数接收 SendModuleMsg 发送到的消息，并处理
	INT			(*Intf_SetModuleMsgCallback)(UINT nModuleType,hcm_msg_callback lpCallback,VOID * pContext);
	//移除回调函数模块
	INT			(*Intf_RemoveModuleMsg)(UINT nModuleType);

	//本地消息通知及其回传
	INT 		(*Intf_SetLocalMsgCallback)(UINT nModuleType,hcm_local_callback lpCallback,VOID * pContext);
	INT 		(*Intf_RemoveLocalMsg)(UINT nModuleType);

	//参数改变回调
	INT 		(*Intf_SetParamChangeCallback)(UINT nModuleType,hcm_paramChange_callback lpCallback,VOID * pContext);
	INT 		(*Intf_RemoveParamChange)(UINT nModuleType);

	//-----------------------//
	// set CPU Bind num
	INT  		(*Intf_SetBindCpu)(UINT nCpuNum);// nCpuNum support 0  or 1

	//yuv data deal
	INT 		(*Intf_GetYuvInfoList)(INT nRgbOrIr, HCI_CHNINFO_S **pChnInfo, UINT *pCount); //nRgbOrIr 0 rgb , 1 ir
	INT 		(*Intf_GetUsrYuvInfo)(INT nRgbOrIr, HCI_CHNINFO_S *pChnInfo);
	INT 		(*Intf_SetUsrYuvInfo)(INT nRgbOrIr, HCI_CHNINFO_S *pChnInfo);
	INT 		(*Intf_GetYuvData)(HCI_CHNHEAD_S *lpChnHead, HCI_VIDEO_FRAME_S *lpFrameInfo, UINT nTimeoutMilliSec);
	INT 		(*Intf_ReleaseYuvData)(HCI_CHNHEAD_S *lpChnHead, HCI_VIDEO_FRAME_S *lpFrameInfo);
	VOID* 		(*Intf_MmapCache)(UINT64 nPhyAddr, UINT nSize); //return vir addr
	INT	 		(*Intf_Munmap)(VOID *lpVirAddr, UINT nSize);
	INT  		(*Intf_EncodeYuvData)(HCI_VIDEO_FRAME_S *lpFrameYuvIn, HCI_VIDEO_ENCODE_S  *lpEncodeIn,  INT MaxFrameLenIn, CHAR *lpOutVencData, UINT *lpOutVencDataLen);

	//draw face
	INT  		(*Intf_DrawFace)(HCI_DRAW_S *pDrawInfo);

  	//io 控制 及其 状态获取
	INT 		(*Intf_LocalIoCtl)(HCI_IOCTL_TYPE_E nIoctlType, VOID *lpParam, UINT iParamLen);

	//audio
	INT 		(*Intf_AudioPlayFile)(UINT nCh, CHAR *pAudioFilePath); //音频文件播放


	//存储信息
	INT 		(*Intf_GetStorageInfo)(UINT nCh, HCI_STORAGE_S *pStorageInfo); //获取存储信息
	INT			(*Intf_FormatDisk)(HCI_DISK_TYPE_E dev_type, UCHAR cDiskGroup, UCHAR cDiskNo); //格式化

	//升级 传文件或者数据方式, 注意数据方式需要传0
	INT 		(*Intf_UpdateFile)(CHAR *pFileName, CHAR *pFileData, INT nFileLen);
	INT 		(*Intf_SetUpdateProgessCallback)(UINT nModuleType, hcm_upgrade_progress_callback lpCallback, VOID* pContext);

	// UART 控制/配置/读写
	INT			(*Intf_UartCreate)(INT nUartChn); // 串口创建 16dv300 最大串口号为4
	INT			(*Intf_UartDestroy)(INT nUartChn); // 销毁串口
	INT			(*Intf_UartConfig)(INT nUartChn,HCM_UART_INFO_S *UartConfig); // 配置串口
	INT			(*Intf_UartReadData)(INT nUartChn,UCHAR *uart_buf,INT buf_len,INT timeout_us); // 读取串口数据
	INT			(*Intf_UartWriteData)(INT nUartChn,const UCHAR *uart_buf,INT buf_len);  // 写串口数据
	// Wiegand 读写 注意，韦根读是回调
	// Wiegand 写
	INT 		(*Intf_WiegandWriteData)(HCM_Wiegand_Data *nWriteBuff);	// 写数据
	INT 		(*Intf_WiegandReadData_CallbackRegister)(hcm_Wiegand_ReadData_callback nWG_Callback);	//

	INT 		(*Intf_AudioPlayStream)(INT AdChn,UCHAR *pStream,UINT u32Len); //音频流播放
	INT 		(*Intf_AudioGetStream)(INT AdChn,UCHAR *pStream,UINT *u32Len); //音频流获取
	//video
	INT 		(*Intf_GetVideoFrame)(INT nCh,CHAR *pFrameBuf,UINT nBufSize,CHAR nFrameType,HCM_Frame_Info *pFrame_Info); //视频帧获取
	// GPS
	INT 		(*Intf_GNNS_GetInfo)(HCM_GNSS_MSG *pstGNNS_Info); //定位信息获取
	double 		(*Intf_GNNS_GetDirectDistance)(double srcLon, double srcLat, double destLon, double destLat); //估算两个坐标之间的直线距离
	//4G
	INT 		(*Intf_4G_GetInfo)(HCM_4G_INFO_S *pst4G_Info); //获取4G模块运行信息
	//EPTZ
	INT 		(*Intf_EptzSet)(HCM_EPTZ *pstEptz); //

	//syslog
	INT 		(*Intf_DoLogInfo)(M_DOLOG_TYPE_E nCmd, void *pBuf, INT nBufLen);

	// id card
	INT			(*Intf_IDcardReadData_CallbackRegister)(hcm_IDcard_ReadData_callback nIDcard_Callback);
//uart

//video

//audio

//snap


//update  data or path

//record

	//....

}HCI_FUNC;





#ifdef __cplusplus
}
#endif

#endif




