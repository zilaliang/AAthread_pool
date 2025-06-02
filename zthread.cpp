#include "zthread.h"
#include<iostream>
using namespace std;
void ZThreadpool::Init(int num) {
	unique_lock <mutex>lock(mux_);
	this->thread_num_ = num;
	cout << "Thread pool Init " << num << endl;
}
void ZThreadpool::Start() {
	unique_lock<mutex> lock(mux_);
	if (thread_num_ <= 0)
	{
		cerr << "Please Init XThreadPool" << endl;
		return;
	}
	if (!threads_.empty())
	{
		cerr << "Thread pool has start!" << endl;
		return;
	}

	for (int i = 0; i < thread_num_; i++) {
		//auto th = new thread(&ZThreadpool::Run, this);
		auto th = make_shared<thread>(&ZThreadpool::Run, this);
		threads_.push_back(th);
	}
}

void  ZThreadpool::Stop() {
	is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_) {
		th->join();
	}
	unique_lock<mutex>lock(mux_);
	threads_.clear();
}


void ZThreadpool::Run() {//��Ӧ��thread_num_��
	cout << "begin ZThreadpool Run " << this_thread::get_id() << endl;
	while (!is_exit())
	{
		auto task = GetTask();//������һ�������ȡ������̲߳������ж�Ӧ�ĺ���
		if (!task)continue;
		++task_run_count_;//ԭ�ӱ�����������Ӱ��
		try {
			auto re=task->Run();
			task->SetValue(re);//��¼�˷���ֵ
		}
		catch (...) {

		}
		--task_run_count_;
	}
	cout << "end ZThreadpool Run " << this_thread::get_id() << endl;
}


std::shared_ptr<ZTask> ZThreadpool::GetTask() {
	unique_lock<mutex>lock(mux_);
	if (tasks_.empty())
	{
		cv_.wait(lock);
	}
	if (is_exit())return nullptr;
	if (tasks_.empty())
	{
		return nullptr;
	}
	auto tmp_task = tasks_.front();
	tasks_.pop_front();
	return tmp_task;
}
void ZThreadpool::AddTask(std::shared_ptr<ZTask> ztask) {
	unique_lock<mutex>lock(mux_);
	tasks_.push_back(ztask);
	ztask->is_exit = [this] {return is_exit(); };
	lock.unlock();
	cv_.notify_one();
}