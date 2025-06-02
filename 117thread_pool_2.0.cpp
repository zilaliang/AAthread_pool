
#include "xthread_pool.h"
#include"zthread.h"
#include <iostream>
using namespace std;
class MyTask :public XTask
{
public:
    int Run()
    {
        cout << "================================================" << endl;
        cout << this_thread::get_id()<<" Mytask " << name << endl;
        cout << "================================================" << endl;
        for (int i = 0; i < 10; i++)
        {
            if (is_exit())break;
            cout << "." << flush;
            this_thread::sleep_for(500ms);
        }
        return 0;
    }
    std::string name = "";
};

class newThread :public ZTask {
    int Run()override {
        cout << "<<<<<<<" << endl;
        return 301;
    }
};
class ZMyTask :public ZTask
{
public:
    int Run()
    {
        cout << "================================================" << endl;
        cout << this_thread::get_id() << " Mytask " << name << endl;
        cout << "================================================" << endl;
        for (int i = 0; i < 10; i++)
        {
            if (is_exit())break;//相当于调用了ZThreadpool::is_exit()
            cout << "." << flush;
            this_thread::sleep_for(500ms);
        }
        return 0;
    }
    std::string name = "";
};
bool GetFF()
{
    return false;
}
std::function<bool()>isGetFF = GetFF;
int main(int argc, char* argv[])
{
    auto ddd=make_shared<newThread>();
    ZThreadpool zdpool;
    zdpool.Init(16);
    zdpool.Start();
    zdpool.AddTask(ddd);

    auto re = ddd->GetReturn();
    cout << re << endl;
    getchar();
    //封装一个普通函数
    /*cout << isGetFF() << endl;

    auto ree = []() {return 321; };
    cout << ree() << endl;*/

    //ZThreadpool zpool;
    //zpool.Init(16);
    //zpool.Start();
    //ZMyTask ztask1;
    //ztask1.name = "test name 001";
    //zpool.AddTask(&ztask1);
    //this_thread::sleep_for(2s);
    //zpool.Stop();
    //getchar();
    std::cout << "============" << std::endl;
    ZThreadpool zpool;
    zpool.Init(16);
    zpool.Start();
    //MyTask task1;
    //task1.name = "test name 001";
    //pool.AddTask(&task1);
    //MyTask task2;
    //task2.name = "test name 002";
    //pool.AddTask(&task2);

    {
        auto task3 = make_shared<ZMyTask>();
        task3->name = "test shared 003";
        zpool.AddTask(task3);

        auto task4 = make_shared<ZMyTask>();
        task4->name = "test shared 004";
        zpool.AddTask(task4);
        auto re = task4->GetReturn();
        cout << "task4->GetReturn() = " << re << endl;
    }
    this_thread::sleep_for(100ms);
    cout << "task run  count = " << zpool.task_run_count() << endl;

    this_thread::sleep_for(1s);
    zpool.Stop();
    cout << "task run  count = " << zpool.task_run_count() << endl;

    getchar();
    return 0;
}