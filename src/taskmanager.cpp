#include "taskmanager.hpp"

#include <iostream>
#include <algorithm>
#include <sqlite3.h>
#include <stdexcept>



TaskManager::TaskManager() {
    if (sqlite3_open("./data/tasks_sql.db", &db) != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error("Failed to open database: " + errorMsg);
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        CREATE TABLE IF NOT EXISTS tasks (
            title 		TEXT		PRIMARY KEY,
            category	TEXT		NOT NULL,
            dueDate		TEXT		NOT NULL,
            priority	INTEGER		NOT NULL,
            status		INTEGER		NOT NULL
            );
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            sqlite3_close(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }
        
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

TaskManager::~TaskManager() {
    sqlite3_close(db);
}


std::vector<Task> TaskManager::getAllTasks() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<Task> allTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        allTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return allTasks;
}


std::vector<std::string> TaskManager::getAvailableCategories() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<std::string> availableCategories;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);
        if (std::find(availableCategories.begin(), availableCategories.end(), category) == availableCategories.end()) {
            availableCategories.push_back(category);
        }
    }

    sqlite3_finalize(stmt);
    return availableCategories;
}

std::vector<std::string> TaskManager::getAvailablePriorities() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<std::string> availablePriorities;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string priority = PrioToStr(static_cast<Priority>(sqlite3_column_int(stmt, 3)));
        if (std::find(availablePriorities.begin(), availablePriorities.end(), priority) == availablePriorities.end()) {
            availablePriorities.push_back(priority);
        }
    }

    sqlite3_finalize(stmt);
    return availablePriorities;
}

std::vector<std::string> TaskManager::getAvailableStatuses() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<std::string> availableStatuses;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string status = StatToStr(static_cast<Status>(sqlite3_column_int(stmt, 4)));
        if (std::find(availableStatuses.begin(), availableStatuses.end(), status) == availableStatuses.end()) {
            availableStatuses.push_back(status);
        }
    }

    sqlite3_finalize(stmt);
    return availableStatuses;
}


bool TaskManager::addTask(const Task& task, bool mongoUpload) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        INSERT INTO tasks (title, category, dueDate, priority, status) VALUES (?, ?, ?, ?, ?);
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, task.getCategory().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, task.getDueDate().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, static_cast<int>(task.getPriority()));
    sqlite3_bind_int(stmt, 5, static_cast<int>(task.getStatus()));

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_CONSTRAINT) {
        std::cout << "\n\033[31mTask '" << task.getTitle() << "' already exists.\033[0m" << std::endl;
        return false;
    }
    return result == SQLITE_DONE;

    // *************************MongoDB*********************************
    // db.taskmanager.InsertOne({
    // title: task.getTitle(),
    // category: task.getCategory(),
    // dueDate: new Date(task.getDueDate())
    // priority: PrioToStr(task.getPriority())
    // status: StatToStr(task.getStatus())
    // })
    if (mongoUpload) {}
}

bool TaskManager::removeTask(const std::string& title, bool mongoUpload) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        DELETE FROM tasks WHERE title = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE && sqlite3_changes(db) > 0;

    // *************************MongoDB*********************************
    // db.taskmanager.deleteOne({
    // title: title
    // })
    if (mongoUpload) {}
}


std::optional<Task> TaskManager::findTask(const std::string& title) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks WHERE title = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);

    std::optional<Task> foundTask = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        foundTask = Task(title, category, dueDate, priority, status);
    }

    sqlite3_finalize(stmt);

    return foundTask;
}


bool TaskManager::updatePriority(const std::string& title, const Priority& priority, bool mongoUpload) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        UPDATE tasks SET priority = ? WHERE title = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_int(stmt, 1, static_cast<int>(priority));
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE && sqlite3_changes(db) > 0;

    // *************************MongoDB*********************************
    // db.taskmanager.updateOne(
    // {title, title},
    // {priority: PrioToStr(priority)}
    // )
    if (mongoUpload) {}
}

bool TaskManager::updateStatus(const std::string& title, const Status& status, bool mongoUpload) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        UPDATE tasks SET status = ? WHERE title = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_int(stmt, 1, static_cast<int>(status));
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE && sqlite3_changes(db) > 0;

    // *************************MongoDB*********************************
    // db.taskmanager.updateOne(
    // {title, title},
    // {status: StatToStr(status)}
    // )
    if (mongoUpload) {}
}


std::vector<Task> TaskManager::filterByCategory(const std::string& cat) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks WHERE category = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_text(stmt, 1, cat.c_str(), -1, SQLITE_STATIC);

    std::vector<Task> filteredCategoryTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        filteredCategoryTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return filteredCategoryTasks;
}

std::vector<Task> TaskManager::filterByPriority(Priority prio) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks WHERE priority = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_int(stmt, 1, static_cast<int>(prio));

    std::vector<Task> filteredPriorityTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        filteredPriorityTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return filteredPriorityTasks;
}

std::vector<Task> TaskManager::filterByStatus(Status stat) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks WHERE status = ?;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    sqlite3_bind_int(stmt, 1, static_cast<int>(stat));

    std::vector<Task> filteredStatusTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        filteredStatusTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return filteredStatusTasks;
}


std::vector<Task> TaskManager::sortByTitle() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks ORDER BY title ASC;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<Task> orderedTitleTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        orderedTitleTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return orderedTitleTasks;
}

std::vector<Task> TaskManager::sortByCategory() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks ORDER BY category ASC;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<Task> orderedCategoryTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        orderedCategoryTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return orderedCategoryTasks;
}

std::vector<Task> TaskManager::sortByPriority() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks ORDER BY priority DESC;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<Task> orderedPriorityTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        orderedPriorityTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return orderedPriorityTasks;
}

std::vector<Task> TaskManager::sortByStatus() const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, R"(
        SELECT * FROM tasks ORDER BY status;
        )", -1, &stmt, nullptr) != SQLITE_OK) {
            std::string errorMsg = sqlite3_errmsg(db);
            throw std::runtime_error("SQL prepare failed:" + errorMsg);
        }

    std::vector<Task> orderedStatusTasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title 	 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Priority priority 	 = static_cast<Priority>(sqlite3_column_int(stmt, 3));
        Status status 		 = static_cast<Status>(sqlite3_column_int(stmt, 4));
        orderedStatusTasks.push_back(Task(title, category, dueDate, priority, status));
    }

    sqlite3_finalize(stmt);
    return orderedStatusTasks;
}