![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)
[![SQLite](https://img.shields.io/badge/SQLite-%2307405e.svg?logo=sqlite&logoColor=white)](https://sqlite.org/)
[![Docker](https://img.shields.io/badge/Docker-%230db7ed.svg?&logo=docker&logoColor=white)](https://hub.docker.com/r/luniphys/taskmanager)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

# Task Manager

A small console based task manager using a **SQLite** database.

The project was created to practice object-oriented programming, enums, user input validation, file separation and SQL database operations.

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
- [Requirements](#requirements)
- [Build & Run](#build--run)
- [Docker](#docker)
- [License](#license)

## Overview

The app is designed to manage tasks in a simple and structured way in a local SQLite database. The application allows users to create, view, find, update, remove, filter and sort tasks.

## Features

- **List tasks** – Display all tasks stored in the database.
- **Add task** – Create a task by entering a (unique) title, category, due date, priority & status.
- **Remove task** – Delete an existing task by title.
- **Find task** – Search for a task by title.
- **Change priority / status** – Update the priority (`Low` / `Medium` / `High`) or status (`Open` / `InProgress` / `Done`) of an existing task.
- **Filter by category** – Show only tasks that belong to a given category.
- **Filter by priority** – Show only tasks at a given priority level.
- **Filter by status** – Show only tasks with a given status.
- **Sort tasks** – Sort tasks by title, by category, by priority, or by status.
- **Date validation** – Due dates are validated including leap-year awareness.
- **Input cancellation** - Every prompt allows `0` to cancel and return to the main menu.
- **JSON export** – Export tasks to `data/tasks.json`.
- **SQLite database** - Stores tasks in `data/tasks_sql.db`.
- **Separated source files** - Separate task model, database manager and main loop.

## Project structure

```
taskmanager/
├── data/                   # auto-generated & updated database
│   ├── tasks.json          # JSON export of tasks
│   └── tasks_sql.db        # SQLite database
└── src/
    ├── task.hpp            # Declares the Task class, Priority, Status and conversion functions.
    ├── task.cpp            # Implements the Task class and conversion functions.
    ├── taskmanager.hpp     # Declares the TaskManager class.
    ├── taskmanager.hpp     # Implements SQLite database operations for TaskManager.
    └── main.cpp            # Contains the command-line interface, menu loop, input validation, and JSON export.
```

## Requirements

- C++ compiler with C++17 support
- SQLite development libraries
- SQLite runtime library

## Build & Run

On Debian/ Ubuntu, install the dependencies with:

```bash
sudo apt-get update
sudo apt-get install g++ libsqlite3-dev
```

Compile from root:

```bash
g++ -std=c++17 src/main.cpp src/task.cpp src/taskmanager.cpp -o src/main -lsqlite3
```

Run the file:

```bash
./src/main
```

## Docker

A Dockerfile is included to provide a reproducible runtime environment. The Dockerfile uses two stages:

1. A builder stage that installs the SQLite development package and compiles the program.
2. A runtime stage that installs the SQLite runtime library and runs the compiled executable.

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
- The volume mount keeps the database and JSON file in the local `data/` directory after the container stops.

## License

MIT © [luniphy](https://github.com/luniphy)
