#include "task.hpp"
#include "taskmanager.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>



constexpr const char* EXIT_STR = "0";


std::string valiDATE() {
	const int MIN = 1;
    const int MONTHS = 12;
    const int LONG_MONTH_DAYS = 31;
    const int SHORT_MONTH_DAYS = 30;
    const int FEBRUARY_DAYS = 28;
    const int LEAP_FEBRUARY_DAYS = 29;

    const std::vector<int> LONG_MONTHS = {1,3,5,7,8,10,12};
    const std::vector<int> SHORT_MONTHS = {4,6,9,11};
    const int FEBRUARY_MONTH = 2;

    const int DATE_LENGTH = 10;

    const std::vector<char> SEPARATORS = {'-', '/', '.', ','};

    std::string date;
    bool date_valid = false;
    int day, month, year;
    int max_days = 0;

    do {
		std::cout << "-> ";
		std::getline(std::cin, date);

		if (date == EXIT_STR) {
			return date;
		}

        if (date.size() != DATE_LENGTH) {
            std::cout << "\033[31mInvalid date format.\033[0m" << std::endl;
            continue;
        }
        char sep1 = date[2];
        char sep2 = date[5];

        if ((std::find(SEPARATORS.begin(), SEPARATORS.end(), sep1) == SEPARATORS.end()) || 
        (std::find(SEPARATORS.begin(), SEPARATORS.end(), sep2) == SEPARATORS.end())) {
            std::cout << "\033[31mInvalid date format.\033[0m" << std::endl;
            continue;
        }
        try {
            day = std::stoi(date.substr(0,2));
            month = std::stoi(date.substr(3,2));
            year = std::stoi(date.substr(6,4));
        }
        catch (const std::exception&) {
            std::cout << "\033[31mInvalid date format.\033[0m" << std::endl;
            continue;
        }
        if (day >= MIN && day <= LONG_MONTH_DAYS && month >= MIN && month <= MONTHS && year >= MIN) {
            // Check february
            if (month == FEBRUARY_MONTH) {
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    max_days = LEAP_FEBRUARY_DAYS;
                }
                else {
                    max_days = FEBRUARY_DAYS;
                }
            }
            // Check long months
            else if (std::find(LONG_MONTHS.begin(), LONG_MONTHS.end(), month) != LONG_MONTHS.end()) {
                max_days = LONG_MONTH_DAYS;
            }
            // Check short months
            else if (std::find(SHORT_MONTHS.begin(), SHORT_MONTHS.end(), month) != SHORT_MONTHS.end()) {
                max_days = SHORT_MONTH_DAYS;
            }
            if (day <= max_days) {
                date_valid = true;
            }
            else {
                std::cout << "\033[31mInvalid date.\033[0m" << std::endl;
            }     
        }
        else {
            std::cout << "\033[31mInvalid date.\033[0m" << std::endl;
        } 
    } while (date_valid == false);

	date[2] = '-';
	date[5] = '-';
	return date;
}


std::optional<Task> findTaskPrompt(const TaskManager& taskmanager) {
	std::string title;
	std::optional<Task> foundTask = std::nullopt;
	std::cout << "Enter Task Title:" << std::endl;
	while (foundTask == std::nullopt && title != EXIT_STR) {
		std::cout << "[Enter 0 to exit.]" << std::endl;
		std::cout << "-> ";
		std::getline(std::cin, title);
		if (title == EXIT_STR) { break; }

		std::transform(title.begin(), title.end(), title.begin(), ::tolower);
		foundTask = taskmanager.findTask(title);
		if (foundTask == std::nullopt) {
			std::cout << "\n\033[31mNo Task with name '\033[0m" << title << "\033[31m' found.\033[0m" << std::endl;
		}
	}
	return foundTask;
}


std::string checkInputPrompt(const std::vector<std::string>& allowedValues) {
	std::vector<std::string> allowedValuesLower;
	for (std::string elem : allowedValues) {
		std::transform(elem.begin(), elem.end(), elem.begin(), ::tolower);
		allowedValuesLower.push_back(elem);
	}
	
	std::string value;
	do {
		std::cout << "[Enter 0 to exit.]" << std::endl;
		std::cout << "-> ";
		std::getline(std::cin, value);
		if (value == EXIT_STR) { 
			return value;
		}

		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		if (std::find(allowedValuesLower.begin(), allowedValuesLower.end(), value) == allowedValuesLower.end()) {
			std::cout << "\n\033[31mNot a valid Input.\033[0m\nAvailable Inputs: ";
			for (std::string elem : allowedValues) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}
	} while (std::find(allowedValuesLower.begin(), allowedValuesLower.end(), value) == allowedValuesLower.end() && value != EXIT_STR);
	return value;
}


void printMany(const std::vector<Task>& tasks, const bool& filterBool, const std::string& CatPrioStat) {
	std::cout << "\n--------------------------------------------------------------------------" << std::endl;
	if (tasks.size() == 0 && filterBool) {
		std::cout << "\n\033[31mNo Tasks with '\033[0m" << CatPrioStat << "\033[31m' found.\033[0m" << std::endl;
	}
	else {
		if (filterBool) {
			std::cout << "Tasks filtered by - " << CatPrioStat << ":\n" << std::endl;
		}
		else {
			std::cout << "Tasks:\n" << std::endl;
		}	
		for (const Task& task : tasks) {
			task.print();
		}
		std::cout << "--------------------------------------------------------------------------\n" << std::endl;
	}
}


void createJSON(const TaskManager& taskmanager) {
	std::ofstream file("./data/tasks.json");
	if (!file.is_open()) {
        std::cerr << "\033[31mFailed to open tasks.json: \033[0m" << std::endl;
        return;
    }

	std::vector<Task> tasks = taskmanager.getAllTasks();

	file << "{\"tasks\": [\n";
	for (size_t i = 0; i < tasks.size(); i++) {
		file << "{\"title\": \"" << tasks[i].getTitle() << "\", ";
		file << "\"category\": \"" << tasks[i].getCategory() << "\", ";
		file << "\"dueDate\": \"" << tasks[i].getDueDate() << "\", ";
		file << "\"priority\": \"" << PrioToStr(tasks[i].getPriority()) << "\", ";
		file << "\"status\": \"" << StatToStr(tasks[i].getStatus()) << "\"}";
		if (i != tasks.size() - 1) {
			file << ",\n";
		}
	}
	file << "\n	]\n}";
	file.close();
}





int main() {

	try {

		TaskManager taskmanager;

		// Test examples
		/*
		Task interview("job interview", "work", "25-09-2026", Priority::High, Status::Open);
		Task haircut("haircut", "private", "17-10-2026", Priority::Medium, Status::Open);
		Task presents("christmas presents", "private", "23-12-2026", Priority::High, Status::InProgress);
		Task meeting("business meeting", "work", "07-05-2026", Priority::Low, Status::Done);
		taskmanager.addTask(interview);
		taskmanager.addTask(haircut);
		taskmanager.addTask(presents);
		taskmanager.addTask(meeting);
		*/


		int inpChoice;
		std::string title, category, dueDate, priorityStr, statusStr, inpMenu, inpChange, inpSort;
		std::string emptyStr = "";
		std::vector<std::string> PrioStrVec   = {"Low", "Medium", "High"};
		std::vector<std::string> StatStrVec   = {"Open", "InProgress", "In Progress", "Done"};
		std::vector<std::string> ChangeStrVec = {"1", "2"};
		std::vector<std::string> SortStrVec   = {"1", "2", "3", "4"};
		
		do {
			createJSON(taskmanager);

			std::cout << "\n**************************************************************************" << std::endl;
			std::cout << "Task Manager:\n1: List available Tasks" <<
						"\n2: Add Task\n3: Remove Task" <<
						"\n4: Find Task\n5: Change Priority/Status" <<
						"\n6: Filter by Category\n7: Filter by Priority" <<
						"\n8: Filter by Status\n9: Sort Tasks\n0: End\n-> ";

			std::getline(std::cin, inpMenu);
			
			if (inpMenu.empty() || !std::all_of(inpMenu.begin(), inpMenu.end(), ::isdigit)) {
				std::cout << "\n\033[31mInvalid Input.\033[0m" << std::endl;
				continue;
			}

			inpChoice = stoi(inpMenu);


			switch (inpChoice) {
				case 0: // Stop Loop
					std::cout << "\n\033[32mBye, bye!\033[0m :)\n" << std::endl;
					break;
				case 1: { // List All Tasks
					std::vector<Task> allTasks = taskmanager.getAllTasks();
					printMany(allTasks, false, emptyStr);
					break;
				}
				case 2: { // Add Task 
					std::cout << "\nAdd Task\nEnter Task Title:\n[Enter 0 to exit.]\n-> ";
					std::getline(std::cin, title);
					if (title == EXIT_STR) { break; }
					std::transform(title.begin(), title.end(), title.begin(), ::tolower);

					std::cout << "\nEnter Task Category:\n[Enter 0 to exit.]\n-> ";
					std::getline(std::cin, category);
					if (category == EXIT_STR) { break; }
					std::transform(category.begin(), category.end(), category.begin(), ::tolower);

					std::cout << "\nEnter Task Due Date (DD-MM-YYYY):\n[Enter 0 to exit.]" << std::endl;
					dueDate = valiDATE();
					if (dueDate == EXIT_STR) { break; }

					std::cout << "\nEnter Task Priority (Low/Medium/High):" << std::endl;
					priorityStr = checkInputPrompt(PrioStrVec);
					if (priorityStr == EXIT_STR) { break; }

					std::cout << "\nEnter Task Status (Open/InProgress/Done):" << std::endl;
					statusStr = checkInputPrompt(StatStrVec);
					if (statusStr == EXIT_STR) { break; }

					Task task(title, category, dueDate, strToPrio(priorityStr), strToStat(statusStr));
					if (taskmanager.addTask(task)) {
						std::cout << "\n\033[32mAdded '\033[0m" << title << "\033[32m'.\033[0m" << std::endl;
					}
					else {
						std::cout << "\n\033[31mCould not add '\033[0m" << title << "\033[31m'.\033[0m" << std::endl;
					}
					break;
				}
				case 3: { // Remove Task
					std::cout << "\nRemove Task" << std::endl;
					std::optional<Task> foundTask = findTaskPrompt(taskmanager);
					if (foundTask == std::nullopt) { break ;}
					
					if (taskmanager.removeTask(foundTask->getTitle())) {
						std::cout << "\n\033[32mRemoved '\033[0m" << foundTask->getTitle() << "\033[32m'.\033[0m" << std::endl;
					}
					else {
						std::cout << "\n\033[31mCould not remove '\033[0m " << foundTask->getTitle() << "\033[31m'.\033[0m" << std::endl;
					}
					break;
				}
				case 4: { // Find Task
					std::cout << "\nFind Task" << std::endl;
					std::optional<Task> foundTask = findTaskPrompt(taskmanager);
					if (foundTask == std::nullopt) { break ;}

					std::cout << "\n\033[32mFound Task:\033[0m" << std::endl;
					foundTask->print();
					break;
				}
				case 5: { // Change Status/Priority
					std::cout << "\nChange Priority/Status" << std::endl;
					std::optional<Task> foundTask = findTaskPrompt(taskmanager);
					if (foundTask == std::nullopt) { break ;}

					std::cout << std::endl;
					foundTask->print();
					
					std::cout << "\nChange Priority (1) / Status (2):\n";
					inpChange = checkInputPrompt(ChangeStrVec);
					if (inpChange == EXIT_STR) { break; }

					if (inpChange == "1") {
						std::cout << "\nEnter new Task Priority (Low/Medium/High):\n";
						priorityStr = checkInputPrompt(PrioStrVec);
						if (priorityStr == EXIT_STR) { break; }

						std::cout << "\n\033[32mChanged Priority of '\033[0m" << foundTask->getTitle() << "\033[32m' from '\033[0m" << PrioToStr(foundTask->getPriority());
						taskmanager.updatePriority(foundTask->getTitle(), strToPrio(priorityStr));

						std::optional<Task> changedTask = taskmanager.findTask(foundTask->getTitle());
						if (changedTask == std::nullopt) { break; }
						std::cout << "\033[32m' to '\033[0m" << PrioToStr(changedTask.value().getPriority()) << "\033[32m'.\033[0m" << std::endl;
					}
					
					else if (inpChange == "2") {
						std::cout << "\nEnter new Task Status (Open/InProgress/Done):\n";
						statusStr = checkInputPrompt(StatStrVec);
						if (statusStr == EXIT_STR) { break; }

						std::cout << "\n\033[32mChanged Status of '\033[0m" << foundTask->getTitle() << "\033[32m' from '\033[0m" << StatToStr(foundTask->getStatus());
						taskmanager.updateStatus(foundTask->getTitle(), strToStat(statusStr));

						std::optional<Task> changedTask = taskmanager.findTask(foundTask->getTitle());
						if (changedTask == std::nullopt) { break; }
						std::cout << "\033[32m' to '\033[0m" << StatToStr(changedTask.value().getStatus()) << "\033[32m'.\033[0m" << std::endl; 
					}
					break;
				}
				case 6: { // Filter by Category
					std::cout << "\nFilter by Category\nEnter Category name:" << std::endl;
					category = checkInputPrompt(taskmanager.getAvailableCategories());
					if (category == EXIT_STR || category.empty()) { break; }

					std::vector<Task> filteredTasks = taskmanager.filterByCategory(category);
					printMany(filteredTasks, true, category);
					break;
				}
				case 7: { // Filter by Priority
					std::cout << "\nFilter by Priority\nEnter Priority level (Low/Medium/High):" << std::endl;
					priorityStr = checkInputPrompt(taskmanager.getAvailablePriorities());
					if (priorityStr == EXIT_STR || priorityStr.empty()) { break; }
					
					std::vector<Task> filteredTasks = taskmanager.filterByPriority(strToPrio(priorityStr));
					printMany(filteredTasks, true, priorityStr);
					break;
				}
				case 8: { // Filter by Status
					std::cout << "\nFilter by Status\nEnter Status level (Open/InProgress/Done):" << std::endl;
					statusStr = checkInputPrompt(taskmanager.getAvailableStatuses());
					if (statusStr == EXIT_STR || statusStr.empty()) { break; }

					std::vector<Task> filteredTasks = taskmanager.filterByStatus(strToStat(statusStr));
					printMany(filteredTasks, true, statusStr);
					break;
				}
				case 9: // Sort alphabetically / by Priority
					std::cout << "\nSort Alphabetically (1) / By Category (2) / By Priority (3) / By Status (4):\n";
					inpSort = checkInputPrompt(SortStrVec);
					if (inpSort == EXIT_STR) { break; }

					if (inpSort == "1") {
						printMany(taskmanager.sortByTitle(), false, emptyStr);
					}
					else if (inpSort == "2") {
						printMany(taskmanager.sortByCategory(), false, emptyStr);
					}
					else if (inpSort == "3") {
						printMany(taskmanager.sortByPriority(), false, emptyStr);
						
					}
					else if (inpSort == "4") {
						printMany(taskmanager.sortByStatus(), false, emptyStr);
					}
					break;
				default:
					std::cout << "\n\033[31mInvalid Input.\033[0m" << std::endl;
			}
		} while (inpChoice != 0);

		return 0;
	}

	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}