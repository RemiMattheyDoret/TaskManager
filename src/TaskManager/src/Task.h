#ifndef TASK_H
#define TASK_H

#include<mutex>
#include "TypeDefinitions.h"
#include <array>


/*
	Task have some methods that are pure virtual.
*/

class Task
{
	public:
		// Status defaultValue is only used for when we do not want to speceify a status when pasing it to a function
		enum TaskStatus {paused=0, running=1, stopped=2, completed=3, defaultValue = 4};



		// I think I need to declare desctructor virtual if I want the vtable to point to derived class default destructors
		virtual ~Task();
		Task(); // Constructor will be called from derived classes

		/*
			Simply use the constructor to start the Task. There is therefore no start method
			virtual start(std::string& process) = 0;
		*/


		virtual void pause();
		virtual void resume();
		virtual void stop();
		virtual double progress() = 0;
		virtual Task::TaskStatus status();

		static const std::array<std::string, 5> _TaskStatusNames;

	protected:		
		static unsigned long _currentTaskID; // This thing will be shared among derived class
		TaskStatus _status;
		const unsigned long _taskID;
		

		std::mutex _mu;
		std::unique_lock<std::mutex> lock();
};



#endif /* TASK_H */

