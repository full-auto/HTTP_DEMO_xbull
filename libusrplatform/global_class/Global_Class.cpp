#include "Global_Class.h"

Global_Class* Global_Class::instance=NULL; // 先初始化

Global_Class::Global_Class()
{
    g_HciFunc = NULL;
}


Global_Class::~Global_Class()
{
    qDebug()<< "delete global********************************";
    if(instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

/*******************************************************************************
 * 函数功能：返回全局类的地址。若是第一次调用，会默认初始化
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
Global_Class* Global_Class::GetGlobal_class()
{
    QMutex g_mutex;
    g_mutex.lock();

    if(instance == NULL)
        //还没有这个指针，代表本静态指针是第一次调用，需要初始化，再返回
        instance = new Global_Class();
    else{}
       //已经有这个指针，代表本静态指针不是第一次调用，不用初始化，直接返回

    g_mutex.unlock();
    return instance;
}
