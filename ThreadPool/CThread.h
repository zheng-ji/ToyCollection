#ifndef __CTHREAD_H__
#define __CTHREAD_H__
#include <vector>
#include <string>
#include <pthread.h>
using namespace std;

class CTask
{
	protected:
		string m_strTaskName;
		void *m_ptrData;
	public:
		CTask() {}
		CTask(string taskName) {
			this->m_strTaskName = taskName;
			m_ptrData = NULL;
		}
		virtual int Run() = 0;
		void SetData(void *data);
};

class CThreadPool
{
	private:
		vector<CTask*> m_vecTaskList;
		int m_iThreadNum;
		static vector<pthread_t> m_vecIdleThread;
		static vector<pthread_t> m_vecBusyThread;
		static pthread_mutex_t m_pthreadMutex;
		static pthread_cond_t m_pthreadCond;
	protected:
		static void* ThreadFunc(void *threadData);
		static int MovedToIdle(pthread_t tid);
		static int MoveToBusy(pthread_t tid);
		int Create();
	public:
		CThreadPool(int threadNum);
		int AddTask(CTask *task);
		int StopAll();
};

#endif
