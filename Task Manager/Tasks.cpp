#include "Tasks.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName)
{
	std::ofstream file(fileName, std::ios::in);
	file << tasks.size();

	for (const Task& task : tasks) {
		std::string description = task.description;
		//replace every space with an underscore
		std::replace(description.begin(), description.end(), ' ', '_');

		file << "\n" << description << ' ' << task.done;
	}

	file.close();
}

std::vector<Task> loadTasksFromFile(const std::string& fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open()) {
		return std::vector<Task>();
	}

	std::vector<Task> tasks;

	//number of tasks
	int n;
	file >> n;

	for (int i = 0; i < n; i++) {
		std::string description;
		bool done;

		file >> description >> done;
		std::replace(description.begin(), description.end(), ' ', '_');
		tasks.push_back(Task{ description, done });
	}

	return tasks;
}