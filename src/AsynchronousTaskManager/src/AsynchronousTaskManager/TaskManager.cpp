#include "TaskManager.h"
#include "Task.h"
#include "TaskSHELL.h"
#include "TaskCPP.h"
#include <vector>
#include <unordered_map>
#include <iostream>


void TaskManager::killAllTasks()
{
	// Kill all tasks
	for (auto& task : tasks)
	{
		delete task;
	}

	// empty task vector
	std::vector<Task*>().swap(tasks);

	// Empty hash map
	std::unordered_map<std::string, taskID_t>().swap(NamesIDsmap);
}

TaskManager::~TaskManager()
{
	killAllTasks();
}

taskID_t TaskManager::findTaskIDFromName(const std::string& taskName) const 
{
	const auto it = NamesIDsmap.find(taskName);
	if (it == NamesIDsmap.end())
	{
		throw std::string("Task with name '") + taskName + std::string("' could not be found. Are you sure you submitted a task with this name previously?\n");
	}

	// Sanity check. This check will be done a second time afterward but I wish to do it here too so that I can figure out where the issue come from shall I experience any issue
	taskID_t taskID = it->second;
	if (taskID >= (taskID_t)tasks.size())
	{
		std::cerr << "taskID " << taskID << " is unknown. This is a bug that was found in function 'findTaskIDFromName'\n";
		std::exit(1);
	}

	return taskID; // return taskID
}

void TaskManager::addTaskToMap(const std::string& taskName)
{
	if (NamesIDsmap.size() > 0 && NamesIDsmap.find(taskName) == NamesIDsmap.end())
		throw std::string("Attempt to create a process with an already existing name. All processes must have unique names.\n");
	else
		NamesIDsmap[taskName] = tasks.size();
		
}

void TaskManager::startSHELL(std::string& process, std::string& taskName)
{
	//std::cout << "RUN void TaskManager::startSHELL(std::string& process)\n";
	addTaskToMap(taskName);
	tasks.push_back(new TaskSHELL(process));
}

void TaskManager::startCPP(std::string& process, std::string& taskName)
{
	//std::cout << "RUN void TaskManager::startCPP(std::string& process)\n";
	addTaskToMap(taskName);
	tasks.push_back(new TaskCPP(process));
}


void TaskManager::pause(taskID_t taskID)
{
	checkTaskID(taskID);
	tasks[taskID]->pause();
}

void TaskManager::pause(std::string& taskName)
{
	pause(findTaskIDFromName(taskName));
}

void TaskManager::resume(taskID_t taskID)
{
	checkTaskID(taskID);
	tasks[taskID]->resume();
}

void TaskManager::resume(std::string& taskName)
{
	resume(findTaskIDFromName(taskName));
}

void TaskManager::stop(taskID_t taskID)
{
	checkTaskID(taskID);
	tasks[taskID]->stop();
}

void TaskManager::stop(std::string& taskName)
{
	stop(findTaskIDFromName(taskName));
}

int TaskManager::status(taskID_t taskID)
{
	checkTaskID(taskID);
	return tasks[taskID]->status();
}

int TaskManager::status(std::string& taskName)
{
	return status(findTaskIDFromName(taskName));
}


void TaskManager::checkTaskID(const taskID_t& taskID) const
{
	if (taskID >= (taskID_t)tasks.size())
		throw std::string("taskID ") + std::to_string(taskID) + std::string(" is unknown\n");
}
