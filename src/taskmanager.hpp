#pragma once

#include "task.hpp"

#include <optional>
#include <string>
#include <vector>


struct sqlite3;


class TaskManager {
	private:
		sqlite3* db;

	public:
		TaskManager();
		~TaskManager();

		// Rule of Five
        TaskManager(const TaskManager&) 		   = delete; // No copy
        TaskManager& operator=(const TaskManager&) = delete; // No copy-assigning
        TaskManager(TaskManager&&) 				   = delete; // no moving
        TaskManager& operator=(TaskManager&&) 	   = delete; // no moving-assigning

		std::vector<Task> getAllTasks() const;
		std::vector<std::string> getAvailableCategories() const;
		std::vector<std::string> getAvailablePriorities() const;
		std::vector<std::string> getAvailableStatuses() const;

		bool addTask(const Task& task);
		bool removeTask(const std::string& title);
		std::optional<Task> findTask(const std::string& title) const;

		bool updatePriority(const std::string& title, const Priority& priority) const;
		bool updateStatus(const std::string& title, const Status& status) const;

		std::vector<Task> filterByCategory(const std::string& cat) const;
		std::vector<Task> filterByPriority(Priority prio) const;
		std::vector<Task> filterByStatus(Status stat) const;

		std::vector<Task> sortByTitle() const;
		std::vector<Task> sortByCategory() const;
		std::vector<Task> sortByPriority() const;
		std::vector<Task> sortByStatus() const;
};