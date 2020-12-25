#include "httptastreturn.h"

httptastreturn::httptastreturn()
{

}

httptastreturn::~httptastreturn()
{

}

void httptastreturn::init()
{
    h_taskRelease=new HttptaskRelease();
    uptime=new QTimer();
    connect(uptime,SIGNAL(timeout()),this,SLOT(uptask_over()));
    uptime->start(500);
}

void httptastreturn::uninit()
{
    if(uptime->isActive())
        uptime->stop();
    delete uptime;
    uptime=NULL;

    delete h_taskRelease;
    h_taskRelease=NULL;
}

void httptastreturn::tast_over(int tastid, int over)
{
    { // 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","taskreturn");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("taskreturn"));
    if(!db.tables().contains("tasksheet"))
    {
        query.exec("create table tasksheet(id int,over int)");
    }
    query.prepare("INSERT INTO tasksheet(id,over)VALUES(:id,:over)");
    query.bindValue(":id",tastid);
    query.bindValue(":over",over);
    query.exec();

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("taskreturn");
}

void httptastreturn::uptask_over()
{
    int id=-1;
    int over=-1;
    { // 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","taskreturn");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("taskreturn"));
    if(db.tables().contains("tasksheet"))
    {
        query.exec("SELECT count(*) from tasksheet");
        query.next();
        if(query.value(0).toInt()!=0)//记录表有数据.
        {
            query.exec("SELECT * from tasksheet limit 1"); //获得第一条记录.
            query.seek(0);
            QSqlRecord rdata = query.record();

            id=rdata.value("id").toInt();
            over=rdata.value("over").toInt();

            h_taskRelease->id=id;
            h_taskRelease->is_success=over;
            h_taskRelease->http_timeout->setInterval(5000);
            if(h_taskRelease->http_replay_func()!=-999)
            {
                query.prepare("Delete from tasksheet where id=:id");
                query.bindValue(":id",id);
                query.exec();
            }
        }
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("taskreturn");
}
