#include "task.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <utility>



Task::Task(std::string title, std::string category, std::string dueDate, Priority priority, Status status)
    : title(std::move(title)), category(std::move(category)), dueDate(std::move(dueDate)),
    priority(priority), status(status)
{}

const std::string& Task::getTitle() const { return title; }
const std::string& Task::getCategory() const { return category; }
const std::string& Task::getDueDate() const { return dueDate; }
Priority Task::getPriority() const { return priority; }
Status Task::getStatus() const { return status; }

void Task::setPriority(Priority prio) { priority = prio; }
void Task::setStatus(Status stat) { status = stat; }

void Task::print() const {
    std::cout << "Title: " << title << ", Category: " << category << ", Due Date: " << dueDate;
    switch(priority) {
        case Priority::Low: std::cout << ", Priority: \033[32mLow\033[0m"; break;
        case Priority::Medium: std::cout << ", Priority: \033[33mMedium\033[0m"; break;
        case Priority::High: std::cout << ", Priority: \033[31mHigh\033[0m"; break;
    }		
    switch(status) {
        case Status::Open: std::cout << ", Status: Open" << std::endl; break;
        case Status::InProgress: std::cout << ", Status: In Progress" << std::endl; break;
        case Status::Done: std::cout << ", Status: Done" << std::endl; break;
    }
}



Priority strToPrio(const std::string& inp) {
	std::string lower = inp;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	if (lower == "low") {
		return Priority::Low;
	}
	if (lower == "medium") {
		return Priority::Medium;
	}
	if (lower == "high") {
		return Priority::High;
	}
	throw std::invalid_argument("\033[31mInvalid Priority:\033[0m " + inp);
}

Status strToStat(const std::string& inp) {
	std::string lower = inp;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	if (lower == "open") {
		return Status::Open;
	}
	if (lower == "inprogress" || lower == "in progress") {
		return Status::InProgress;
	}
	if (lower == "done") {
		return Status::Done;
	}
	throw std::invalid_argument("\033[31mInvalid Status:\033[0m " + inp);
}

std::string PrioToStr (const Priority& prio) {
	if (prio == Priority::Low) {
		return "Low";
	}
	if (prio == Priority::Medium) {
		return "Medium";
	}
	if (prio == Priority::High) {
		return "High";
	}
	throw std::invalid_argument("\033[31mInvalid Priority.\033[0m");
}

std::string StatToStr (const Status& stat) {
	if (stat == Status::Open) {
		return "Open";
	}
	if (stat == Status::InProgress) {
		return "InProgress";
	}
	if (stat == Status::Done) {
		return "Done";
	}
	throw std::invalid_argument("\033[31mInvalid Status.\033[0m");
}