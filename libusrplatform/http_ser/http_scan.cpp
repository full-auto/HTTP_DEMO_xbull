#include "http_scan.h"




http_scan::http_scan()
{
}

http_scan::~http_scan()
{
}

/*******************************************************************************
 * 函数功能：初始化全局类地址，及初始化定时器
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_scan::init(void)
{
    // 获取全局类的地址
    glo = Global_Class::GetGlobal_class();

    // 该定时器70ms 触发一次分析图片
    QObject::connect(&tim_pic_scan,SIGNAL(timeout()),this,SLOT(pic_scaning()) );
    tim_pic_scan.start(70);
}

/*******************************************************************************
 * 函数功能：定时器触发的用来轮询缓冲图片列表的槽函数，有内容则传去识别并把结果发给ui类，没有则返回
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_scan::pic_scaning(void)
{
    //char szPath[128] = "/FACE_0_20201218171303896_1.jpg";
    // 如果长度为0，代表该链表没有数据，也就不用分析
    if(glo->pic_list.isEmpty()) return;

    QByteArray szPath = glo->pic_list.takeFirst(); // 返回头部元素,并删除它

    AI_COMPARE_REC_INFO pRecInfo;
    HCM_MSG_HEAD_S stMsgHead;
    uint nCount = 1;
    int nret = 999;

    // 比对
    memset(&stMsgHead, 0, sizeof(stMsgHead));
    memset(&pRecInfo, 0, sizeof(pRecInfo));
    stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
    stMsgHead.u32MsgType    = OUT_FACE_CMP_PIC;  //人脸比对图片17
    nret = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, szPath.data(), 0, (void **)&pRecInfo, &nCount);

#if 0
    // 获取下发的信息
    memset(&stMsgHead, 0, sizeof(stMsgHead));
    memset(&pRecInfo, 0, sizeof(pRecInfo));
    stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
    stMsgHead.u32MsgType    = IN_UPLOADE_IDFACE;  //下发人脸比对信息14
    nret = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, NULL, 0, (void **)&pRecInfo, &nCount);
#endif

    qDebug() << "************接收到图片！已传给AI识别!下面是结果！**************************************************************";
    qDebug() << "*    list size　: "<<glo->pic_list.size(); // 打印列表数量，以求证是否真正入列
    qDebug() << "*    pic_path = [" << szPath << "]";
    qDebug() << QString("*   nret [%1], pRecInfo_len = [%3], pRecInfo.name [%2], pRecInfo.result = [%4], pRecInfo.FirstCmptime = [%5]")
                             .arg(nret)
                             .arg(pRecInfo.name)
                             .arg(nCount)
                             .arg(pRecInfo.result)
                             .arg(pRecInfo.FirstCmptime);


    // 发送，让ui显示相关信息和识别图片
    emit emit_show_pic(szPath);
    emit emit_usr(pRecInfo.name);
}

