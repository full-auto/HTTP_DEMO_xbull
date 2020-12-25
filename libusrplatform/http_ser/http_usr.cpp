#include "http_usr.h"


http_usr::http_usr(QObject *parent) : QObject(parent)
{
}

/*******************************************************************************
 * 函数功能：HTTP主处理类的初始化
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_usr::init(void)
{
    ser      = new QTcpServer(this);
    client   = new QTcpSocket(this);
    ser->setMaxPendingConnections(3);

    glo = Global_Class::GetGlobal_class();

    // 监听端口
    if(!ser->listen(QHostAddress("192.168.0.101"),8080)){
        //ui->label_state->setText("获取网络错误！");
        emit emit_err_string("获取网络错误！" + ser->errorString());
    }

    // 有新连接就触发槽函数
    connect(ser,SIGNAL(newConnection()),this,SLOT(new_client_connect()) );
}

http_usr::~http_usr()
{
    delete ser;
    delete client;
}


/*******************************************************************************
 * 函数功能：当有新TCP用户链接时，触发的槽函数。主要用来链接接收信息槽函数
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_usr::new_client_connect(void)
{
    client = ser->nextPendingConnection();
    emit emit_client(client->peerAddress().toString());

    // 有新信息过来就触发槽函数
    connect(client,SIGNAL(readyRead()),this, SLOT(new_client_message()) );
}



/*******************************************************************************
 * 函数功能：当链接对象发送信息过来时，触发的槽函数。分析是POST或是GET请求，图片数据则保存.达到对应
 *          文件大小就保存进文件，并塞入全局链表
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
int num = 1; // 记录错误次数
int  i = 1; // 记录图片数据传输的次数
// 函数功能：接受消息槽函数分析请求协议 支持get post
void http_usr::new_client_message(void)
{
    QByteArray msg_1    = client->readAll();
    QString msg         = msg_1;
    QStringList msg_buf = msg.split("\r\n");

    qDebug() << QString("-----NEW---i[%1] dowm_size[%2] file_size[%3] len[%4]------------------------").arg(i).arg(data.length()).arg(file_size).arg(msg_buf.size());

    if(msg_buf.size() > 3) // 分割后字段长度大于４，代表是有效数据请求
    {
        emit_err_string(msg);

        //　get 请求
        if(msg_buf.at(0).startsWith("GET"))
            Parsing_GET_request(msg_buf);

        // post 请求
        else if (msg_buf.at(0).startsWith("POST"))
            Parsing_POST_request(msg_buf);
    }
    else // 其他数据，可能是图片，当前使用场景下不作判断
    {

#ifdef BUG_ooo
        qDebug() << "\n======================pic_data?========================================";
#endif
        // 把读到的数据存入缓存区
        data.append(msg_1);

        // 下载完成,把缓冲区的数据写入文件，并发送信号让ui显示图片
        if(data.length() >= file_size)
        {
            //－－目前很大几率出现粘包－－
/*
本次图片还未下完，就被传来的新图片HTTP获取的新file_size(新旧图片大小肯定不一样)取代了旧file_size。
由于本程序逻辑是缓冲区大小 >= file_size就保存图片发送结束报文(摄像头停止发送)，固导致图片过早或过晚保存
*/
            // 盲目删除多余的部分
            data.remove(file_size,data.length()-file_size);

            QImage test;
            // 判断是否是正确图片，是就调整大小,写入文件，不是则清空
            if(test.loadFromData(data))
            {
                emit_err_string("接收完成！　有效图片！ 传输识别图片中*******");
                qDebug() << "\n*****************file_done!*********************************";
                qDebug() << "dowm_size :" << data.length() << "file_size :" << file_size<<"\n";

                // 调整大小
                uint x = test.width();
                uint y = test.height();
                QImage save_file = test.scaled(x-80,y-80,Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // 写入文件
                save_file.save(file_name,"JPG",100);

                // 给全局类去保存进链表，并发送信号提醒?　从尾部进入，头部出
                glo->pic_list.push_back(file_name.toLatin1());

                // 信号发送文件名，告诉识别类http_scan已经入列，
                //emit emit_scan_pic(file_name);

            }
            else // 图片数据是错误的(粘包情况)，目前只做记录　不做其他操作
            {
                qDebug() << err_func_show << "http服务器---这是个错误的图片!---";
                qDebug() << "dowm_size :" << data.length() << "file_size :" << file_size<<"\n";
                // 用system命令写入日志
                QByteArray temp = QString("echo '-[%1] is an err picture! [%2]' >> /home/libusrplatform/face_pic/log.txt").arg(file_name.remove(0,29)).arg(num++).toLatin1();
                system(temp.data());
            }

            i = 1;           // 清空计数值
            file_size = 0;   // 清空待下载文件大小
            data.clear();    // 清空缓冲区


            //　文件下载完成，发送响应报文
            QString http = "HTTP/1.1 200 OK\r\n";
            http += "Server: WHLtest_server 1.0\r\n";
            http += "Content-Type: text/html;charset=utf-8\r\n";
            http += "Connection: close\r\n";
            client->write(http.toUtf8());
            client->waitForBytesWritten(http.length());
            client->flush();
            client->disconnect();
            client->close();
        }
        else{
#ifdef BUG_ooo
            qDebug() << err_func_show << "keep going!! " << i <<"---------------";
#endif
            emit emit_err_string(QString("持续接收中! [%1] ").arg(i++));
        }
    }

}



//浏览器请求:
//GET / HTTP/1.1
//Accept: text/html, application/xhtml+xml, image/jxr, */*
//Accept-Language: zh-CN
//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko
//Accept-Encoding: gzip, deflate
//Host: 192.168.123.190:5050
//Connection: Keep-Alive
/*******************************************************************************
 * 函数功能：处理分析GET请求，根据发来的对应请求头，返回对应响应报
 * 参数说明：
 *   QStringList &msg　：　传进来的分割后的字符串列表。用来逐行分析数据
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_usr::Parsing_GET_request(QStringList &msg)
{
    // 获取请求的内容
    QString temp_1 = msg.at(0);
    temp_1.remove("GET ").remove(" HTTP/1.1");
    qDebug() << err_func_show << "temp_1 :" << temp_1;

    // 是有内容的请求,根据内容返回对应语句
    if(temp_1.startsWith("/text"))
    {
        temp_1.remove("/text:").replace("%20"," ");

        if (temp_1 == "hello") choreme_view("yes! is me");
        if (temp_1 == "how are you") choreme_view("I am fine ! thanks!");
        if (temp_1 == "how old are you") choreme_view("I am 1 days old!");
        if (temp_1 == "what is you name") choreme_view("my name is 奥利给");
        else choreme_view("I AM stupid basics! they call me SB!\n");
    }
    else // 否则是无内容交流请求，只发送回报
    {
        qDebug() << err_func_show << temp_1.size();
        choreme_view("You Can Ask Me :[hello][how are you][how old are you][what is you name]");
    }
}


/*
POST / HTTP/1.1
Host: 192.168.0.101:5050
DeviceId: 10056666                           //设备ID
DeviceUUID: umetuv6bc39y                     //设备UUID
DeviceMac: 00:11:04:00:cf:83                 //设备mac地址
FaceID: 41                                   //人脸ID
Left: 510                                    //人脸区域离左边界的距离
Top: 73                                     //人脸区域离上边界的距离
Width: 832                                   //人脸区域的宽度
Height: 941                                  //人脸区域的高度
SnapTime: 2020-12-08 10:12:41                //抓拍时间 %Y-%m-%d %H:%M:%S
SendTime: 2020-12-08 10:17:01                //HTTP上传时间 %Y-%m-%d %H:%M:%S
nSendNum: 7823                               //发送成功计数
nLostNum: 7820                               //发送失败计数（图片丢失）
nSendCount: 0
ClearCount: 0
FacePiclen: 345895                           //人脸图片数据长度
Content-Type: image/jpeg                     //数据类型
Connection: Close
Content-Length: 345895                       //数据长度
*/
/*******************************************************************************
 * 函数功能：处理分析POST请求， 获取摄像头发来的对应识别信息
 * 参数说明：
 *   QStringList &msg　：　传进来的分割后的字符串列表。用来逐行分析数据
 * 返回值：
 * 　空
 * ******************************************************************************/
void http_usr::Parsing_POST_request(QStringList &msg)
{
    //ui->textBrowser->clear();
    //qDebug() << err_func_show << msg<< " msg.size :" << msg.length();
    QString face_id   = msg.at(5).mid(8);
    QString device_id = msg.at(2).mid(10);
    QString time      = msg.at(10).mid(21).replace(":","-");

    file_size = msg.at(19).mid(16).toInt();
    file_name = save_path + QString("/face_%1_%2_%3.jpg").arg(face_id).arg(device_id).arg(time);

    emit_err_string(" this requese is for POST");
    emit_info(device_id,time);
}


/*******************************************************************************
 * 函数功能：把传进来的字符串组合成有效响应主体
 * 参数说明：
 *   QString　msg　：　需要发送字符串数据
 * 返回值：
 * 　空
 * ******************************************************************************/
// 示例：http://192.168.123.190:5050/ 模拟青云客AI聊天
void http_usr::choreme_view(QString msg)
{
    emit_err_string("[this is a request for GET]");

    // 组成响应报文,因为有发送长度，所以客户端会自动断开链接
    QByteArray response = QString("<h1><center>This is my First HTTP Server! </center></h1>\r\n"
                                  "<h1><center>%1</center></h1>\r\n").arg(msg).toUtf8();
    QString http = "HTTP/1.1 200 OK\r\n";
    http += "Server: WHLtest_setver 1.0\r\n";
    http += "Content-Type: text/html;charset=utf-8\r\n";
    http += "Connection: keep-alive\r\n";
    http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(response.size()));

    // 发送响应后断开
    client->write(http.toUtf8());
    client->write(response);
    client->flush();
    client->waitForBytesWritten(http.size() + response.size());
    client->disconnect();
    client->close();
    //ui->label_state->setText("response to client done");
}

