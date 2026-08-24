#pragma once

#include <string>



enum class Priority {Low, Medium, High};
enum class Status {Open, InProgress, Done};


class Task {
	private:
		std::string title;
		std::string category;
		std::string dueDate;
		Priority priority;
		Status status;

	public:
		Task(std::string title, std::string category, std::string dueDate, Priority priority, Status status);

		const std::string& getTitle() const;
		const std::string& getCategory() const;
		const std::string& getDueDate() const;
		Priority getPriority() const;
		Status getStatus() const;

		void setPriority(Priority prio);
		void setStatus(Status stat);

		void print() const;
};


Priority strToPrio(const std::string& inp);
Status strToStat(const std::string& inp);
std::string PrioToStr (const Priority& prio);
std::string StatToStr (const Status& stat);