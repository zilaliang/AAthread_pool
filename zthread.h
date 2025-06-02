#pragma once

#include<mutex>
#include<thread>
#include<vector>
#include<list>
#include<functional>
#include <atomic>
#include<iostream>
#include<future>
class ZTask {
public:
	ZTask() {
		std::cout << "构造函数" << std::endl;
	}
	~ZTask() {
		std::cout << "析构函数" << std::endl;
	}
	ZTask(const ZTask&)
	{
		std::cout << "拷贝构造函数" << std::endl;
	}
	virtual int Run() = 0;
	std::function<bool()>is_exit = nullptr;
	auto GetReturn()
	{
		//阻塞等待 set_value
		return p_.get_future().get();
	}
	void SetValue(int v) {
		p_.set_value(v);
	}
private:
	std::promise<int>p_;
};

class ZThreadpool {
public:
	void Init(int num);
	void Start();
	void Stop();
	std::shared_ptr<ZTask> GetTask();
	//void AddTask(XTask* task);
	void AddTask(std::shared_ptr<ZTask> ztask);
	bool is_exit(){
		return is_exit_;
	}
	int task_run_count() { return task_run_count_; }
private:
	void Run();
	int thread_num_ = 0;
	std::mutex mux_;
	//std::vector<std::thread*>threads_;
	std::vector<std::shared_ptr<std::thread>>threads_;
	//std::list<ZTask*>tasks_;
	std::list<std::shared_ptr<ZTask>>tasks_;

	std::condition_variable cv_;
	bool is_exit_ = false;

	std::atomic<int>task_run_count_ = { 0 };
};
