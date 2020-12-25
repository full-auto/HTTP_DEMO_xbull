#include "httpselfstudyfacegateconfig.h"

HttpselfStudyFaceGateConfig::HttpselfStudyFaceGateConfig()
{
 //   qDebug()<<"selfStudyFaceGateConfig init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.selfStudyFaceGateConfig);
    devname_user="";
    school_user="";
    face_thr=80;
    wifi_id="";
    wifi_key="";
    dopenflag=0;
  //  qDebug()<<"selfStudyFaceGateConfig ok";
    glo=Global_class::GetInstance();
}

HttpselfStudyFaceGateConfig::~HttpselfStudyFaceGateConfig()
{

}

int HttpselfStudyFaceGateConfig::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;
    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("version",version);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);
    if(ptr=="")//超时.
        return -999;

    int ret = replay_return_func(ptr);
    return ret;
}

int HttpselfStudyFaceGateConfig::replay_return_func(QString ptr)
{
  //  qDebug()<<"go get set";
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    int ht=0;
    int minfacesize;
    float ht_thr;
    int kz=0;
    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(ptr.toStdString(),jv))
    {
        return 0;
    }
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mStudyFaceGateConfig");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mStudyFaceGateConfig"));

    //先删除之前的
    query.exec("DROP TABLE DeviceConfig");
    query.exec("DROP TABLE SchoolInfo");
    query.exec("DROP TABLE AccessRule");
    query.exec("DROP TABLE AccessRule2");

    /* 检测是否有设备配置表，没有则创建 */
    if(!db.tables().contains("DeviceConfig"))
    {
        int t=query.exec("create table DeviceConfig(id int,\
                                            name varchar,\
                                            device_type int,\
                                            recognized_threshold float,\
                                            live_threshold_switch int,\
                                            live_threshold float,\
                                            face_angle int,\
                                            limit_face int,\
                                            is_mute int,\
                                            gate_number int,\
                                            number int,\
                                            channel int,\
                                            is_speaker int,\
                                            broadcast_twice int,\
                                            non_broadcast varchar,\
                                            default_access_type_id int,\
                                            config_hash varchar,\
                                            is_apply_card int,\
                                            mask_status int,\
                                            temperature_status int,\
                                            fever_threshold float,\
                                            temperature_compensate_status int,\
                                            temperature_compensate float,\
                                            temperature_mode int,\
                                            open_door_signal int,\
                                            temperature_wait_time int,\
                                            temperature_time_slot varchar,\
                                            non_thermal_users varchar,\
                                            thermal_users varchar,\
                                            debug_switch int,\
                                            is_leave_absence int,\
                                            start_absence_at int,\
                                            end_absence_at int)");
                //qDebug()<<"创建设备表格结果"<<t;
    }
    /* 检测是否有学校消息表，没有则创建 */
    if(!db.tables().contains("SchoolInfo"))
    {
        query.exec("create table SchoolInfo(id int,\
                                            name varchar,\
                                            lang varchar,\
                                            ssid varchar,\
                                            ssid_password varchar)");
    }
    /* 检测是否有门禁规则表，没有则创建 */
    if(!db.tables().contains("AccessRule"))
    {
        query.exec("create table AccessRule(access_label varchar,\
                                            apply_type varchar,\
                                            apply_date varchar,\
                                            rule_items varchar,\
                                            is_open int)");
    }
    /* 检测是否有门禁规则2.0表，没有则创建 */
    if(!db.tables().contains("AccessRule2"))
    {
        query.exec("create table AccessRule2(id int,\
                                            name varchar,\
                                            start_at varchar,\
                                            end_at varchar,\
                                            is_eternal int,\
                                            invalid_at_items varchar,\
                                            student_tag_items varchar,\
                                            parent_tag_items varchar,\
                                            staff_tag_items varchar,\
                                            device_items varchar,\
                                            access_items varchar)");
    }

    db.transaction();

    /* 设备 */
    Json::Value jdevice=jv["data"]["device"];
    /* 设备ID */
    int d_id;
    if(jdevice["id"].isInt())
        d_id=jdevice["id"].asInt();
    else
        d_id=QString::fromStdString(jdevice["id"].asString()).toInt();
    QString dev_id=QString::number(d_id);//记录设备id，在判断门禁是否适应该设备时使用
    /* 设备名字 */
    QString d_name=QString::fromLocal8Bit(jdevice["name"].asString().data());
    devname_user=d_name;
    /* 设备类型 */
    int d_device_type;
    if(jdevice["device_type"].isInt())
        d_device_type=jdevice["device_type"].asInt();
    else
        d_device_type=QString::fromStdString(jdevice["device_type"].asString()).toInt();
    /* 识别阈值 */
    float d_recognized_threshold;
    if(jdevice["recognized_threshold"].isDouble())
        d_recognized_threshold=jdevice["recognized_threshold"].asFloat();
    else
        d_recognized_threshold=QString::fromStdString(jdevice["recognized_threshold"].asString()).toFloat();
    face_thr=d_recognized_threshold*100;
    /* 活体阀值开关，1 开启；0 关闭 */
    int d_live_threshold_switch;
    if(jdevice["live_threshold_switch"].isInt())
        d_live_threshold_switch=jdevice["live_threshold_switch"].asInt();
    else
        d_live_threshold_switch=QString::fromStdString(jdevice["live_threshold_switch"].asString()).toInt();
    ht=d_live_threshold_switch;
    /* 活体识别阀值 */
    float d_live_threshold;
    if(jdevice["live_threshold"].isDouble())
        d_live_threshold=jdevice["live_threshold"].asFloat();
    else
        d_live_threshold=QString::fromStdString(jdevice["live_threshold"].asString()).toFloat();
    ht_thr=d_live_threshold;
    /* 正脸角度 */
    int d_face_angle;
    if(jdevice["face_angle"].isInt())
        d_face_angle=jdevice["face_angle"].asInt();
    else
        d_face_angle=QString::fromStdString(jdevice["face_angle"].asString()).toInt();
    /* 最小脸部 */
    int d_limit_face;
    if(jdevice["limit_face"].isInt())
        d_limit_face=jdevice["limit_face"].asInt();
    else
        d_limit_face=QString::fromStdString(jdevice["limit_face"].asString()).toInt();
    minfacesize=d_limit_face;
    /* 是否静音，1 静音；0 发声 */
    int d_is_mute;
    if(jdevice["is_mute"].isInt())
        d_is_mute=jdevice["is_mute"].asInt();
    else
        d_is_mute=QString::fromStdString(jdevice["is_mute"].asString()).toInt();
    voice_f=d_is_mute;
    /* 门控编号，0表示无门控 */
    int d_gate_number;
    if(jdevice["gate_number"].isInt())
        d_gate_number=jdevice["gate_number"].asInt();
    else
        d_gate_number=QString::fromStdString(jdevice["gate_number"].asString()).toInt();
    /* 编号 */
    int d_number;
    if(jdevice["number"].isInt())
        d_number=jdevice["number"].asInt();
    else
        d_number=QString::fromStdString(jdevice["number"].asString()).toInt();
    hbox_number=d_number;
    /* 音箱频段 */
    int d_channel;
    if(jdevice["channel"].isInt())
        d_channel=jdevice["channel"].asInt();
    else
        d_channel=QString::fromStdString(jdevice["channel"].asString()).toInt();
    hbox_cha=d_channel;
    /* 人脸闸门音响播报开关，1 开启；0 关闭 */
    int d_is_speaker;
    if(jdevice["is_speaker"].isInt())
        d_is_speaker=jdevice["is_speaker"].asInt();
    else
        d_is_speaker=QString::fromStdString(jdevice["is_speaker"].asString()).toInt();
    hbox_open=d_is_speaker;
    /* 是否播报两次 */
    int d_broadcast_twice;
    if(jdevice["broadcast_twice"].isInt())
        d_broadcast_twice=jdevice["broadcast_twice"].asInt();
    else
        d_broadcast_twice=QString::fromStdString(jdevice["broadcast_twice"].asString()).toInt();
    hbox_angin=d_broadcast_twice;
    /* 不播报用户类型 */
    QString d_non_broadcast=QString::fromStdString(Json::FastWriter().write(jdevice["non_broadcast"]));
    hbox_notype=d_non_broadcast;
    //默认进出类型。0：允许进出；1：禁止进出
    int default_access_type_id;
    if(jdevice["default_access_type_id"].isInt())
        default_access_type_id=jdevice["default_access_type_id"].asInt();
    else
        default_access_type_id=QString::fromStdString(jdevice["default_access_type_id"].asString()).toInt();
    dopenflag=default_access_type_id;
    /* 配置hahs值，用于校验是否跟平台数据一致性 */
    QString d_config_hash=QString::fromStdString(jdevice["config_hash"].asString());
    /* 人脸闸门卡模块开关，1 开启；0 关闭 */
    int d_is_apply_card;
    if(jdevice["is_apply_card"].isInt())
        d_is_apply_card=jdevice["is_apply_card"].asInt();
    else
        d_is_apply_card=QString::fromStdString(jdevice["is_apply_card"].asString()).toInt();
    card_open=d_is_apply_card;
    /* 人脸闸门是否支持口罩识别，0 关闭，1 开启 */
    int d_mask_status;
    if(jdevice["mask_status"].isInt())
        d_mask_status=jdevice["mask_status"].asInt();
    else
        d_mask_status=QString::fromStdString(jdevice["mask_status"].asString()).toInt();
    kz=d_mask_status;
    /* 体温检测开关，0 关闭，1 开启 */
    int d_temperature_status;
    if(jdevice["temperature_status"].isInt())
        d_temperature_status=jdevice["temperature_status"].asInt();
    else
        d_temperature_status=QString::fromStdString(jdevice["temperature_status"].asString()).toInt();
    tem_flag=d_temperature_status;
    /* 发烧阈值 */
    float d_fever_threshold;
    if(jdevice["fever_threshold"].isDouble())
        d_fever_threshold=jdevice["fever_threshold"].asFloat();
    else
        d_fever_threshold=QString::fromStdString(jdevice["fever_threshold"].asString()).toFloat();
    tem_thr=d_fever_threshold;
    /* 测温补偿开关，0 关闭，1 开启 */
    int d_temperature_compensate_status;
    if(jdevice["temperature_compensate_status"].isInt())
        d_temperature_compensate_status=jdevice["temperature_compensate_status"].asInt();
    else
        d_temperature_compensate_status=QString::fromStdString(jdevice["temperature_compensate_status"].asString()).toInt();
    tem_pc_flag=d_temperature_compensate_status;
    /* 测温补偿 */
    float d_temperature_compensate=0;;
    if(jdevice["temperature_compensate"].isDouble())
        d_temperature_compensate=jdevice["temperature_compensate"].asFloat();
    else
        d_temperature_compensate=QString::fromStdString(jdevice["temperature_compensate"].asString()).toFloat();
    tem_pc=d_temperature_compensate;
    /* 测温模式。0：默认；1：低温也放行（小于34℃）；2：发烧也放行；3：低温也放行+发烧也放行；4：低温也放行+发烧也放行+发烧不推送 */
    int d_temperature_mode=0;
    if(jdevice.isMember("temperature_mode"))
    {
        if(jdevice["temperature_mode"].isInt())
            d_temperature_mode=jdevice["temperature_mode"].asInt();
        else
            d_temperature_mode=QString::fromStdString(jdevice["temperature_mode"].asString()).toInt();
    }
    tem_type=d_temperature_mode;
    /* 开门信号。0：默认；1：反置 */
    int d_open_door_signal=0;
    if(jdevice.isMember("open_door_signal"))
    {
        if(jdevice["open_door_signal"].isInt())
            d_open_door_signal=jdevice["open_door_signal"].asInt();
        else
            d_open_door_signal=QString::fromStdString(jdevice["open_door_signal"].asString()).toInt();
    }
    /* 测温等待时间 */
    int d_temperature_wait_time=0;
    if(jdevice.isMember("temperature_wait_time"))
    {
        if(jdevice["temperature_wait_time"].isInt())
            d_temperature_wait_time=jdevice["temperature_wait_time"].asInt();
        else
            d_temperature_wait_time=QString::fromStdString(jdevice["temperature_wait_time"].asString()).toInt();
    }
    tem_wait=d_temperature_wait_time;
    /* 不测温时段 */
    QString d_temperature_time_slot=QString::fromStdString(Json::FastWriter().write(jdevice["temperature_time_slot"]));
    tem_ptime=d_temperature_time_slot;
    /* 不测温用户类型 */
    QString d_non_thermal_users=QString::fromStdString(Json::FastWriter().write(jdevice["non_thermal_users"]));
    tem_nousertype=d_non_thermal_users;
    /* 超时可以通行用户 */
    QString d_thermal_users=QString::fromStdString(Json::FastWriter().write(jdevice["thermal_users"]));
    tem_usertype_timeout=d_thermal_users;
    /* 是否在ui显示调试框 */
    int d_debug_switch=0;
    if(jdevice.isMember("debug_switch"))
    {
        if(jdevice["debug_switch"].isInt())
            d_debug_switch=jdevice["debug_switch"].asInt();
        else
            d_debug_switch=QString::fromStdString(jdevice["debug_switch"].asString()).toInt();
    }
    debug_showui=d_debug_switch;
    /* 是否可以请假通行 */
    int d_is_leave_absence=0;
    if(jdevice.isMember("is_leave_absence"))
    {
        if(jdevice["is_leave_absence"].isInt())
            d_is_leave_absence=jdevice["is_leave_absence"].asInt();
        else
            d_is_leave_absence=QString::fromStdString(jdevice["is_leave_absence"].asString()).toInt();
    }
    leaceflag=d_is_leave_absence;
    /* 请假通行开始时间 */
    int d_start_absence_at=0;
    if(jdevice.isMember("start_absence_at"))
    {
        if(jdevice["start_absence_at"].isInt())
            d_start_absence_at=jdevice["start_absence_at"].asInt();
        else
            d_start_absence_at=QString::fromStdString(jdevice["start_absence_at"].asString()).toInt();
    }
    start_leave=d_start_absence_at;
    /* 请假通行结束时间 */
    int d_end_absence_at=0;
    if(jdevice.isMember("end_absence_at"))
    {
        if(jdevice["end_absence_at"].isInt())
            d_end_absence_at=jdevice["end_absence_at"].asInt();
        else
            d_end_absence_at=QString::fromStdString(jdevice["end_absence_at"].asString()).toInt();
    }
    end_leave=d_end_absence_at;

    query.prepare("INSERT INTO DeviceConfig(id,\
                                            name,\
                                            device_type,\
                                            recognized_threshold,\
                                            live_threshold_switch,\
                                            live_threshold,\
                                            face_angle,\
                                            limit_face,\
                                            is_mute,\
                                            gate_number,\
                                            number,\
                                            channel,\
                                            is_speaker,\
                                            broadcast_twice,\
                                            non_broadcast,\
                                            default_access_type_id,\
                                            config_hash,\
                                            is_apply_card,\
                                            mask_status,\
                                            temperature_status,\
                                            fever_threshold,\
                                            temperature_compensate_status,\
                                            temperature_compensate,\
                                            temperature_mode,\
                                            open_door_signal,\
                                            temperature_wait_time,\
                                            temperature_time_slot,\
                                            non_thermal_users,\
                                            thermal_users,\
                                            debug_switch,\
                                            is_leave_absence,\
                                            start_absence_at,\
                                            end_absence_at)\
                                VALUES(:id,\
                                        :name,\
                                        :device_type,\
                                        :recognized_threshold,\
                                        :live_threshold_switch,\
                                        :live_threshold,\
                                        :face_angle,\
                                        :limit_face,\
                                        :is_mute,\
                                        :gate_number,\
                                        :number,\
                                        :channel,\
                                        :is_speaker,\
                                        :broadcast_twice,\
                                        :non_broadcast,\
                                        :default_access_type_id,\
                                        :config_hash,\
                                        :is_apply_card,\
                                        :mask_status,\
                                        :temperature_status,\
                                        :fever_threshold,\
                                        :temperature_compensate_status,\
                                        :temperature_compensate,\
                                        :temperature_mode,\
                                        :open_door_signal,\
                                        :temperature_wait_time,\
                                        :temperature_time_slot,\
                                        :non_thermal_users,\
                                        :thermal_users,\
                                        :debug_switch,\
                                        :is_leave_absence,\
                                        :start_absence_at,\
                                        :end_absence_at)");
    query.bindValue(":id",d_id);
    query.bindValue(":name",d_name);
    query.bindValue(":device_type",d_device_type);
    query.bindValue(":recognized_threshold",d_recognized_threshold);
    query.bindValue(":live_threshold_switch",d_live_threshold_switch);
    query.bindValue(":live_threshold",d_live_threshold);
    query.bindValue(":face_angle",d_face_angle);
    query.bindValue(":limit_face",d_limit_face);
    query.bindValue(":is_mute",d_is_mute);
    query.bindValue(":gate_number",d_gate_number);
    query.bindValue(":number",d_number);
    query.bindValue(":channel",d_channel);
    query.bindValue(":is_speaker",d_is_speaker);
    query.bindValue(":broadcast_twice",d_broadcast_twice);
    query.bindValue(":non_broadcast",d_non_broadcast);
    query.bindValue(":default_access_type_id",default_access_type_id);
    query.bindValue(":config_hash",d_config_hash);
    query.bindValue(":is_apply_card",d_is_apply_card);
    query.bindValue(":mask_status",d_mask_status);
    query.bindValue(":temperature_status",d_temperature_status);
    query.bindValue(":fever_threshold",d_fever_threshold);
    query.bindValue(":temperature_compensate_status",d_temperature_compensate_status);
    query.bindValue(":temperature_compensate",d_temperature_compensate);
    query.bindValue(":temperature_mode",d_temperature_mode);
    query.bindValue(":open_door_signal",d_open_door_signal);
    query.bindValue(":temperature_wait_time",d_temperature_wait_time);
    query.bindValue(":temperature_time_slot",d_temperature_time_slot);
    query.bindValue(":thermal_users",d_thermal_users);
    query.bindValue(":thermal_users",d_thermal_users);
    query.bindValue(":debug_switch",d_debug_switch);
    query.bindValue(":is_leave_absence",d_is_leave_absence);
    query.bindValue(":start_absence_at",d_start_absence_at);
    query.bindValue(":end_absence_at",d_end_absence_at);
    query.exec();

    /* 学校 */
    Json::Value jschool=jv["data"]["school"];
    /* 学校ID */
    int s_id;
    if(jschool["id"].isInt())
        s_id=jschool["id"].asInt();
    else
        s_id=QString::fromStdString(jschool["id"].asString()).toInt();
    /* 学校名字 */
    QString s_name=QString::fromLocal8Bit(jschool["name"].asString().data());
    school_user=s_name;
    /* 学校语言 */
    QString s_lang=QString::fromStdString(jschool["lang"].asString());
    /* Wi-Fi名字 */
    QString s_ssid=QString::fromLocal8Bit(jschool["ssid"].asString().data());
    /* Wi-Fi密码 */
    QString s_ssid_password=QString::fromStdString(jschool["ssid_password"].asString());

    //转换wifi密码，给设置用
    wifi_id=s_ssid;
    wifi_key=s_ssid_password;

    query.prepare("INSERT INTO SchoolInfo (id,\
                                            name,\
                                            lang,\
                                            ssid,\
                                            ssid_password)\
                                    VALUES(:id,\
                                           :name,\
                                           :lang,\
                                           :ssid,\
                                           :ssid_password)");
    query.bindValue(":id",s_id);
    query.bindValue(":name",s_name);
    query.bindValue(":lang",s_lang);
    query.bindValue(":ssid",s_ssid);
    //qDebug()<<"保存时的ssid_password:"<<s_ssid_password;
    query.bindValue(":ssid_password",s_ssid_password);
    query.exec();

    /* 门禁规则 旧的不再处理*/
    /*
    Json::Value jaccess_rule=jv["data"]["access_rule"];
    for(int i=0;i<jaccess_rule.size();i++)
    {
        // 用户标签
        QString access_label;
        if(jaccess_rule[i]["access_label"].isArray())
        {
            access_label=QString::fromStdString(Json::FastWriter().write(jaccess_rule[i]["access_label"]));
        }
        // 日期类型，all 全部；weekday 星期一至五；weekend 星期六至天；date 自定义
        QString apply_type=QString::fromStdString(jaccess_rule[i]["apply_type"].asString());
        // 自定义日期
        QString apply_date;
        if(jaccess_rule[i]["apply_date"].isArray())
        {
            apply_date=QString::fromStdString(Json::FastWriter().write(jaccess_rule[i]["apply_date"]));
        }
        // 规则项
        QString rule_items;
        if(jaccess_rule[i]["rule_items"].isArray())
        {
            rule_items=QString::fromStdString(Json::FastWriter().write(jaccess_rule[i]["rule_items"]));
        }
        int is_open;
        if(jaccess_rule[i]["is_open"].isInt())
            is_open=jaccess_rule[i]["is_open"].asInt();
        else
            is_open=QString::fromStdString(jaccess_rule[i]["is_open"].asString()).toInt();

        query.prepare("INSERT INTO AccessRule (access_label,\
                                              apply_type,\
                                              apply_date,\
                                              rule_items,\
                                              is_open)\
                                        VALUES(:access_label,\
                                               :apply_type,\
                                               :apply_date,\
                                               :rule_items,\
                                               :is_open)");
        query.bindValue(":access_label",access_label);
        query.bindValue(":apply_type",apply_type);
        query.bindValue(":apply_date",apply_date);
        query.bindValue(":rule_items",rule_items);
        query.bindValue(":is_open",is_open);
        query.exec();
    }
    */
    /* 门禁规则2.0 */
    Json::Value jaccess_rule2=jv["data"]["access_rule_v2"];

    rule2_strlist.clear();
    for(int i=0;i<jaccess_rule2.size();i++)
    {
        //门禁规则id
        int id;
        if(jaccess_rule2[i]["id"].isInt())
            id=jaccess_rule2[i]["id"].asInt();
        else
            id=QString::fromStdString(jaccess_rule2[i]["id"].asString()).toInt();
        //门禁规则名称
        QString name=QString::fromStdString(jaccess_rule2[i]["name"].asString());
        //开始日期
        int start_at;
        if(jaccess_rule2[i]["start_at"].isInt())
            start_at=jaccess_rule2[i]["start_at"].asInt();
        else
            start_at=QString::fromStdString(jaccess_rule2[i]["start_at"].asString()).toInt();
        //结束日期
        int end_at;
        if(jaccess_rule2[i]["end_at"].isInt())
            end_at=jaccess_rule2[i]["end_at"].asInt();
        else
            end_at=QString::fromStdString(jaccess_rule2[i]["end_at"].asString()).toInt();
        //是否永久生效
        int is_eternal;
        if(jaccess_rule2[i]["is_eternal"].isInt())
            is_eternal=jaccess_rule2[i]["is_eternal"].asInt();
        else
            is_eternal=QString::fromStdString(jaccess_rule2[i]["is_eternal"].asString()).toInt();
        //不生效日期   //Json::FastWriter().write(jd[i]["card_items"])
        QString invalid_at_items=QString::fromStdString(Json::FastWriter().write(jaccess_rule2[i]["invalid_at_items"]));
        //针对学生
        QString student_tag_items="";
        for(int j=0;j<jaccess_rule2[i]["student_tag_items"].size();j++)
        {
            QString student_item="|"+QString::fromStdString(jaccess_rule2[i]["student_tag_items"][j].asString())+"|";
            student_tag_items=student_tag_items+student_item;
        }
        //针对家长
        QString parent_tag_items="";
        for(int j=0;j<jaccess_rule2[i]["parent_tag_items"].size();j++)
        {
            QString parent_item="|"+QString::fromStdString(jaccess_rule2[i]["parent_tag_items"][j].asString())+"|";
            parent_tag_items=parent_tag_items+parent_item;
        }
        //针对职工
        QString staff_tag_items="";
        for(int j=0;j<jaccess_rule2[i]["staff_tag_items"].size();j++)
        {
            QString staff_item="|"+QString::fromStdString(jaccess_rule2[i]["staff_tag_items"][j].asString())+"|";
            staff_tag_items=staff_tag_items+staff_item;
        }
        //生效设备
        QString device_items=QString::fromStdString(Json::FastWriter().write(jaccess_rule2[i]["device_items"]));
        //规则详情
        QString access_items=QString::fromStdString(Json::FastWriter().write(jaccess_rule2[i]["access_items"]));

        query.prepare("INSERT INTO AccessRule2(id,\
                                            name,\
                                            start_at,\
                                            end_at,\
                                            is_eternal,\
                                            invalid_at_items,\
                                            student_tag_items,\
                                            parent_tag_items,\
                                            staff_tag_items,\
                                            device_items,\
                                            access_items)\
                                        VALUES(:id,\
                                               :name,\
                                               :start_at,\
                                               :end_at,\
                                               :is_eternal,\
                                               :invalid_at_items,\
                                               :student_tag_items,\
                                               :parent_tag_items,\
                                               :staff_tag_items,\
                                               :device_items,\
                                               :access_items)");
        query.bindValue(":id",id);
        query.bindValue(":name",name);
        query.bindValue(":start_at",start_at);
        query.bindValue(":end_at",end_at);
        query.bindValue(":is_eternal",is_eternal);
        query.bindValue(":invalid_at_items",invalid_at_items);
        query.bindValue(":student_tag_items",student_tag_items);
        query.bindValue(":parent_tag_items",parent_tag_items);
        query.bindValue(":staff_tag_items",staff_tag_items);
        query.bindValue(":device_items",device_items);
        query.bindValue(":access_items",access_items);
        query.exec();
        //判断该设备是否适用这条门禁
        if(device_items.contains(dev_id))
        {
            QStringList new_rule;
            new_rule.append(QString::number(start_at));
            new_rule.append(QString::number(is_eternal));
            new_rule.append(QString::number(end_at));
            new_rule.append(invalid_at_items);
            new_rule.append(student_tag_items);
            new_rule.append(parent_tag_items);
            new_rule.append(staff_tag_items);
            new_rule.append(access_items);
            rule2_strlist.append(new_rule);
        }
    }

    db.commit();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mStudyFaceGateConfig");
    //如果名字为空
    if(devname_user=="")
       devname_user=net_MAC;
    if(school_user=="")
       school_user="无学校信息";
    if(wifi_id=="")
        wifi_id="xbull_wifi";
    if(wifi_key=="")
        wifi_key="a12345678b";

    //硬件配置
    int nRet = 0;
    HCM_FACE_DETECT_S stFaceDetect;
    memset( &stFaceDetect, 0, sizeof(stFaceDetect));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_FACEDETEC, 0, &stFaceDetect, sizeof(stFaceDetect) );
    if(nRet==0)
    {
        //活体
        if(ht==0)
            stFaceDetect.ucLivenessEnable = 0;
        else    
        {
            stFaceDetect.ucLivenessEnable = 1;
            //活体阈值-与平台刚好相反
            stFaceDetect.fLivescore=1-ht_thr;
        }
        //最小人脸
        stFaceDetect.iFaceMinSize=minfacesize*3+60;
        if(stFaceDetect.iFaceMinSize>300)
            stFaceDetect.iFaceMinSize=300;
        if(stFaceDetect.iFaceMinSize<30)
            stFaceDetect.iFaceMinSize=30;
        //口罩
        stFaceDetect.ucMaskCheckEnable=kz;
        nRet = glo->g_HciFunc->Intf_SetParam( HCI_CFG_FACEDETEC, 0, &stFaceDetect, sizeof(HCM_FACE_DETECT_S) );
    }
    else
    {
        //获取设备信息失败
    }

    //识别阈值
    AI_FACE_CTL_S staiface;
    memset( &staiface, 0, sizeof(staiface));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_FACECTL, 0, &staiface, sizeof(staiface) );
    if(nRet==0)
    {
        //把识别对比开关设为1，防止出现不识别的情况
        staiface.ucCmpEnable=1;
        //qDebug()<<"设置阈值----------------------------:"<<face_thr;
        staiface.fCmpSore=face_thr;
        nRet = glo->g_HciFunc->Intf_SetParam( HCI_CFG_FACECTL, 0, &staiface, sizeof(AI_FACE_CTL_S) );
    }
    else
    {
        //qDebug()<<"设置阈值失败!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        //获取设备信息失败
    }

    //wifi
    HCM_WIFI_S stwifi;
    memset( &stwifi, 0, sizeof(stwifi));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_WIFI, 0, &stwifi, sizeof(stwifi) );
    if(nRet==0)
    {
        stwifi.bEnable=1;
        stwifi.bDhcp=1;
        stwifi.byWifiMode=0;
        stwifi.byWifiType=2;
        stwifi.byEncryptType=1;
        memcpy(stwifi.bySSID,wifi_id.toStdString().c_str(),wifi_id.toLocal8Bit().length()+1);
        memcpy(stwifi.byPasswd,wifi_key.toStdString().c_str(),wifi_key.toLocal8Bit().length()+1);
        nRet=glo->g_HciFunc->Intf_SetParam( HCI_CFG_WIFI, 0, &stwifi, sizeof(HCM_WIFI_S));
    }
    else
    {
        //获取设备信息失败
    }

    return 0;
}

void HttpselfStudyFaceGateConfig::cleardb()
{
    devname_user=net_MAC;
    school_user="无学校信息";
    debug_showui=0;
    face_thr=80;
    tem_flag=0;
#ifdef QT_DEBUG
    qDebug()<<"clear db * DeviceConfig SchoolInfo AccessRule ";
#endif

    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mStudyFaceGateConfig");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mStudyFaceGateConfig"));
    QString sqlstr;

    query.exec("DROP TABLE DeviceConfig");
    query.exec("DROP TABLE SchoolInfo");
    query.exec("DROP TABLE AccessRule");
    query.exec("DROP TABLE AccessRule2");

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mStudyFaceGateConfig");
}

void HttpselfStudyFaceGateConfig::getdecinfo()
{
#ifdef QT_DEBUG
    qDebug()<<"获取设备信息";
#endif
    leaceflag=0;
    start_leave=0;
    end_leave=0;
    debug_showui=0;
    voice_f=0;
    int ht=0;
    float ht_thr=0.8;
    int minfacesize=80;
    int kz=0;
    tem_flag=0;
    card_open=0;
    hbox_number=0;
    hbox_cha=0;
    hbox_open=0;
    QString dev_id;
    hbox_angin=0;
    hbox_notype="";
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mStudyFaceGateConfig");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mStudyFaceGateConfig"));

    //先添加未有的字段，避免旧版本没有，导致查找数据失败
    //其实不判断直接添加也行，如果已有会添加失败，不会影响之前的数据
    query.exec("select * from sqlite_master where name='DeviceConfig' and sql like '%broadcast_twice%'");
    if(!query.next())//broadcast_twice non_broadcast 是在同一个版本添加
    {
        query.exec("alter table DeviceConfig add column broadcast_twice int");
        query.exec("alter table DeviceConfig add column non_broadcast varchar");
    }

    //设备信息
    query.exec("SELECT name,\
                        recognized_threshold,\
                        live_threshold_switch,\
                        live_threshold,\
                        limit_face,\
                        debug_switch,\
                        default_access_type_id,\
                        is_leave_absence,\
                        start_absence_at,\
                        end_absence_at,\
                        mask_status,\
                        is_mute,\
                        id,\
                        temperature_status,\
                        fever_threshold,\
                        temperature_compensate_status,\
                        temperature_compensate,\
                        temperature_mode,\
                        temperature_wait_time,\
                        temperature_time_slot,\
                        non_thermal_users,\
                        thermal_users,\
                        is_apply_card,\
                        number,\
                        channel,\
                        is_speaker,\
                        broadcast_twice,\
                        non_broadcast from DeviceConfig ORDER BY id asc");
    while(query.next())
    {
        //qDebug()<<"进入设置基本参数";
        devname_user=query.value(0).toString();
        face_thr=query.value(1).toFloat()*100;
        ht=query.value(2).toInt();
        ht_thr=query.value(3).toFloat();
        minfacesize=query.value(4).toInt();
        debug_showui=query.value(5).toInt();
        dopenflag=query.value(6).toInt();
        leaceflag=query.value(7).toInt();
        start_leave=query.value(8).toInt();
        end_leave=query.value(9).toInt();
        kz=query.value(10).toInt();
        voice_f=query.value(11).toInt();
        //获得设备id，在判断门禁是否适应该设备时使用
        dev_id=QString::number(query.value(12).toInt());
        tem_flag=query.value(13).toInt();
        tem_thr=query.value(14).toFloat();
        tem_pc_flag=query.value(15).toInt();
        tem_pc=query.value(16).toFloat();
        tem_type=query.value(17).toInt();
        tem_wait=query.value(18).toInt();
        tem_ptime=query.value(19).toString();
        tem_nousertype=query.value(20).toString();
        tem_usertype_timeout=query.value(21).toString();
        card_open=query.value(22).toInt();
        hbox_number=query.value(23).toInt();
        hbox_cha=query.value(24).toInt();
        hbox_open=query.value(25).toInt();
        hbox_angin=query.value(26).toInt();
        hbox_notype=query.value(27).toString();
        break;
    }
    //学校信息
    query.exec("SELECT name,ssid,ssid_password from SchoolInfo ORDER BY id asc");
    while(query.next())
    {
        school_user=query.value(0).toString();
        wifi_id=query.value(1).toString();
        wifi_key=query.value(2).toString();
        break;
    }
    //门禁信息
    query.exec("SELECT start_at,\
                        is_eternal,\
                        end_at,\
                        invalid_at_items,\
                        student_tag_items,\
                        parent_tag_items,\
                        staff_tag_items,\
                        access_items,\
                        device_items from AccessRule2 ORDER BY id asc");
    rule2_strlist.clear();
    while(query.next())
    {
        QString start_at=query.value(0).toString();
        int is_eternal=query.value(1).toInt();
        QString end_at=query.value(2).toString();
        QString invalid_at_items=query.value(3).toString();
        QString student_tag_items=query.value(4).toString();
        QString parent_tag_items=query.value(5).toString();
        QString staff_tag_items=query.value(6).toString();
        QString access_items=query.value(7).toString();
        //获得门禁使用的设备id，判断该设备是否适用这条门禁
        QString device_items=query.value(8).toString();
        if(device_items.contains(dev_id))
        {
            QStringList new_rule;
            new_rule.append(start_at);
            new_rule.append(QString::number(is_eternal));
            new_rule.append(end_at);
            new_rule.append(invalid_at_items);
            new_rule.append(student_tag_items);
            new_rule.append(parent_tag_items);
            new_rule.append(staff_tag_items);
            new_rule.append(access_items);
            rule2_strlist.append(new_rule);
        }
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mStudyFaceGateConfig");
    //如果名字为空
    if(devname_user=="")
       devname_user=net_MAC;
    if(school_user=="")
       school_user="无学校信息";
    if(wifi_id=="")
        wifi_id="xbull_wifi";
    if(wifi_key=="")
        wifi_key="a12345678b";

    int nRet = 0;
    HCM_FACE_DETECT_S stFaceDetect;
    memset( &stFaceDetect, 0, sizeof(stFaceDetect));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_FACEDETEC, 0, &stFaceDetect, sizeof(stFaceDetect) );
    if(nRet==0)
    {
        //活体
        if(ht==0)
            stFaceDetect.ucLivenessEnable = 0;
        else
        {
            stFaceDetect.ucLivenessEnable = 1;
            //活体阈值-与平台刚好相反
            stFaceDetect.fLivescore=1-ht_thr;
        }
        //最小人脸
        stFaceDetect.iFaceMinSize=minfacesize*3+60;
        if(stFaceDetect.iFaceMinSize>300)
            stFaceDetect.iFaceMinSize=300;
        if(stFaceDetect.iFaceMinSize<30)
            stFaceDetect.iFaceMinSize=30;
        //口罩
        stFaceDetect.ucMaskCheckEnable=kz;
        nRet = glo->g_HciFunc->Intf_SetParam( HCI_CFG_FACEDETEC, 0, &stFaceDetect, sizeof(HCM_FACE_DETECT_S) );
    }
    else
    {
        //获取设备信息失败
    }

    //识别阈值
    AI_FACE_CTL_S staiface;
    memset( &staiface, 0, sizeof(staiface));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_FACECTL, 0, &staiface, sizeof(staiface) );
    if(nRet==0)
    {
        //把识别对比开关设为1，防止出现不识别的情况
        staiface.ucCmpEnable=1;
        //qDebug()<<"设置阈值----------------------------:"<<face_thr;
        staiface.fCmpSore=face_thr;
        nRet = glo->g_HciFunc->Intf_SetParam( HCI_CFG_FACECTL, 0, &staiface, sizeof(AI_FACE_CTL_S) );
    }
    else
    {
        //qDebug()<<"设置阈值失败!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        //获取设备信息失败
    }

    //wifi
    HCM_WIFI_S stwifi;
    memset( &stwifi, 0, sizeof(stwifi));
    nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_WIFI, 0, &stwifi, sizeof(stwifi) );
    if(nRet==0)
    {
        stwifi.bEnable=1;
        stwifi.bDhcp=1;
        stwifi.byWifiMode=0;
        stwifi.byWifiType=2;
        stwifi.byEncryptType=1;
        memcpy(stwifi.bySSID,wifi_id.toStdString().c_str(),wifi_id.toLocal8Bit().length()+1);
        memcpy(stwifi.byPasswd,wifi_key.toStdString().c_str(),wifi_key.toLocal8Bit().length()+1);
        nRet=glo->g_HciFunc->Intf_SetParam( HCI_CFG_WIFI, 0, &stwifi, sizeof(HCM_WIFI_S));

/*
        qDebug()<<"----------------------------------------";
        qDebug()<<"wifi_id:"<<wifi_id.toStdString().c_str()<<"length:"<<wifi_id.toLocal8Bit().length();
        qDebug()<<"wifi_key:"<<wifi_key<<"length:"<<wifi_key.toLocal8Bit().length();
        qDebug()<<"stwifi.bySSID:"<<QString::fromStdString((char*)stwifi.bySSID);
        qDebug()<<"stwifi.byPasswd:"<<QString::fromStdString((char*)stwifi.byPasswd);
        qDebug()<<"----------------------------------------";*/
    }
    else
    {
        //获取设备信息失败
    }
}

