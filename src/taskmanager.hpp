#pragma once

#include "task.hpp"

#include <optional>
#include <string>
#include <vector>

#if __has_include(<mongocxx/client.hpp>)
	#define MONGO_ENABLED
	#include <mongocxx/client.hpp>
	#include <mongocxx/instance.hpp>
	#include <mongocxx/uri.hpp>
#endif


struct sqlite3;


class TaskManager {
	private:
		sqlite3* db;
		
#ifdef MONGO_ENABLED
		mongocxx::client mongoClient;
		mongocxx::collection mongoCollection;
#endif

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

		bool updatePriority(const std::string& title, const Priority& priority);
		bool updateStatus(const std::string& title, const Status& status);

		std::vector<Task> filterByCategory(const std::string& cat) const;
		std::vector<Task> filterByPriority(Priority prio) const;
		std::vector<Task> filterByStatus(Status stat) const;

		std::vector<Task> sortByTitle() const;
		std::vector<Task> sortByCategory() const;
		std::vector<Task> sortByPriority() const;
		std::vector<Task> sortByStatus() const;
};