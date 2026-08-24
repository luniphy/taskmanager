![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)
[![SQLite](https://img.shields.io/badge/SQLite-%2307405e.svg?logo=sqlite&logoColor=white)](https://sqlite.org/)
[![Docker](https://img.shields.io/badge/Docker-%230db7ed.svg?&logo=docker&logoColor=white)](https://hub.docker.com/r/luniphys/taskmanager)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

# Task Manager

A small project for learning **Object-Oriented Programming** & **SQL** better through a simple task manager console app based on a **SQLITE** database.

### Focus

- Classes and objects
- Encapsulation
- Enums
- User input handling
- Rule of Five
- SQL

### Example

```bash
--------------------------------------------------------------------------
Tasks:

Title: job interview, Category: work, Due Date: 25-09-2026, Priority: High, Status: Open
Title: haircut, Category: private, Due Date: 17-10-2026, Priority: Medium, Status: Open
Title: christmas presents, Category: private, Due Date: 23-12-2026, Priority: High, Status: In Progress
Title: business meeting, Category: work, Due Date: 07-05-2026, Priority: Low, Status: Done
--------------------------------------------------------------------------


**************************************************************************
Task Manager:
1: List available Tasks
2: Add Task
3: Remove Task
4: Find Task
5: Change Priority/Status
6: Filter by Category
7: Filter by Priority
8: Filter by Status
9: Sort Tasks
0: End
->
```

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Build & Run](#build--run)
- [Docker](#docker)
- [License](#license)

## Overview

The app is designed to manage tasks in a simple and structured way in a local SQLite database. Users can create tasks, update their details, mark them as completed and organize them by filtering/sorting.

## Features

- **List tasks** – Display all tasks stored in the database.
- **Add task** – Create a new task by entering title, category, due date, priority & status. Titles must be unique.
- **Remove task** – Delete a task by title.
- **Find task** – Look up a task by title.
- **Change priority / status** – Update the priority (`Low` / `Medium` / `High`) or status (`Open` / `InProgress` / `Done`) of an existing task.
- **Filter by category** – Show only tasks that belong to a given category.
- **Filter by priority** – Show only tasks at a given priority level.
- **Filter by status** – Show only tasks with a given status.
- **Sort tasks** – Order the task list alphabetically by title, by category, by priority (High → Low), or by status (Open → Done).
- **Input validation** – due dates are validated including leap-year awareness; priority and status inputs are checked against allowed values (case-insensitive); every prompt allows `0` to cancel and return to the main menu.
- **JSON export** – after every action the full task list is written to `data/tasks.json`.
- **SQLite database** - The database `data/tasks_sql.db` is created automatically on the first run.

## Project structure

```
taskmanager/
├── data/                    # auto-generated & updated .db & .json files
└── src/
    ├── taskmanager.cpp      # source code — Task & TaskManager classes, main loop
```

## Build & Run

Compile the file:

```bash
g++ -std=c++17 src/main.cpp src/task.cpp src/taskmanager.cpp -o src/main -lsqlite3
```

Run the file:

```bash
./src/main
```

## Docker

A Dockerfile is included to provide a reproducible runtime environment.

### Build the image

From the repository root, build the Docker image with:

```bash
docker build -t taskmanager .
```

### Pull from Docker Hub

A prebuilt image is also available on [Docker Hub](https://hub.docker.com/r/luniphys/taskmanager):

```bash
docker pull luniphys/taskmanager
```

### Run the container

```bash
docker run --rm -it -v $(pwd)/data:/app/data taskmanager
```

### Notes

- Run the container in interactive mode: `-it`
- The database will be saved in `data/` as a `.db` and `.json` file.

## License

MIT © [luniphy](https://github.com/luniphy)
