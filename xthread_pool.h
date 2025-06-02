
#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <functional>
#include <atomic>
class XTask
{
public:
    virtual int Run() = 0;
    std::function<bool()> is_exit = nullptr;
};

class XThreadPool
{
public:
    //////////////////////////////////////////////
    /// ��ʼ���̳߳�
    /// @para num �߳�����
    void Init(int num);

    //////////////////////////////////////////////
    /// ���������̣߳������ȵ���Init
    void Start();

    //////////////////////////////////////////////
    /// �̳߳��˳�
    void Stop();

    //void AddTask(XTask* task);
    void AddTask(std::shared_ptr<XTask> task);

    std::shared_ptr<XTask> GetTask();

    //�̳߳��Ƿ��˳�
    bool is_exit() { return is_exit_; }

    int task_run_count() { return task_run_count_; }
private:
    //�̳߳��̵߳���ں���
    void Run() ;
    int thread_num_ = 0;//�߳�����
    std::mutex mux_;
    //std::vector<std::thread*> threads_;
    //�߳��б� ָ��ָ��汾
    std::vector< std::shared_ptr<std::thread> > threads_;
    std::list<std::shared_ptr<XTask> > tasks_;
    
    std::condition_variable cv_;
    bool is_exit_ = false; //�̳߳��˳�

    //�������е���������,�̰߳�ȫ
    std::atomic<int> task_run_count_ = {0};
};

