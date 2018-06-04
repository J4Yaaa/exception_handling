//异常优点
//1.比返回错误码的方式更清晰
//2.很多c++的第三方库使用异常,使用异常能更好的用这些库
//3.引用异常能更好的测试代码,如测试框架使用异常

//异常缺点
//1.执行流乱跳,不方便跟踪调试代码
//2.异常安全问题 : 需要更好的使用RAII编写异常安全代码,如智能指针
//  如内存泄漏 -> 可以手动catch再throw

#include <iostream>
using namespace std;
#include <string>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

//实现一个可以只用一个catch捕捉异常的方法
//基类 实现对异常成员的定义
//子类 共有继承基类,对其成员函数和变量都继承下来
class Exception
{
public:
    Exception(const string& errmsg,int errid)
        :_errmsg(errmsg),_errid(errid)
    {}
    virtual string What()
    {
        return _errmsg;
    }
protected:
    string _errmsg;
    int _errid;
};

//第一种异常场景 : 数据库层异常
class SqlException : public Exception
{
public:
    SqlException(const string& errmsg,int errid)
        :Exception(errmsg,errid)
    {}
    virtual string What()
    {
        string err("1:");
        err += _errmsg;
        return err;
    }
};

//第二个异常场景 : 缓存层异常
class CacheException : public Exception
{
public:
    CacheException(const string& errmsg,int errid)
        :Exception(errmsg,errid)
    {}
    virtual string What()
    {
        string err("2:");
        err += _errmsg;
        return err;
    }
};

//...可构建多个异常场景

void ConnectSql()
{
    if(rand()%4 == 0)
    {
        throw SqlException("链接数据库失败",1);
    }
}

void InitAql()
{
    if(rand()%3 == 0)
    {
        throw CacheException("初始化缓存失败",2);
    }
}

class NetWorkServer
{
public:
    void StartSqlMgr()
    {//链接数据库需要调用数据库层
        ConnectSql();
    }

    void StartCacheMgr()
    {//初始化数据需要调用缓存层
        InitAql();
    }

    void start()
    {
        while(1)
        {
            try
            {
                //只在start里捕捉异常
                StartSqlMgr();
                StartCacheMgr();
                sleep(1);
            }
            catch(Exception& e)//因为都是继承基类,所以上面两个函数如果产生异常,那么类型都相同,这里直接捕捉Exception类型的异常
            {
                cout<<e.What()<<endl;//使用多态,调用自身对应的What函数
            }
            catch(...)//如果不是当前Exception的异常,证明是在我们意料之外的
            {
                cout<<"未知异常"<<endl;
            }
        }
    }
};

int main()
{
    srand(time(0));
    NetWorkServer s;
    s.start();
    return 0;
}

//void fun2()
//{
//    /* throw int(1); */
//}
//
//void fun1()
//{
//    try
//    {
//        fun2();
//    }
//    catch(exception e)
//    {
//        cout<<"main catch"<<endl;
//    }
//    throw char('q');
//}
//
//int main()
//{
//    try
//    {
//        fun1();
//    }
//    catch(int e)
//    {
//        cout<<"main catch"<<endl;
//    }
//    catch(...)
//    {
//        cout<<"位置错误"<<endl;
//    }
//    return 0;
//}
