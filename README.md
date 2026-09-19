[![CD](https://github.com/luniphy/taskmanager/actions/workflows/cd.yml/badge.svg)](https://github.com/luniphy/taskmanager/actions/workflows/cd.yml)
![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)
[![SQLite](https://img.shields.io/badge/SQLite-%2307405e.svg?logo=sqlite&logoColor=white)](https://sqlite.org/)
[![Docker](https://img.shields.io/badge/Docker-%230db7ed.svg?&logo=docker&logoColor=white)](https://hub.docker.com/r/luniphys/taskmanager)
[![GHCR](https://img.shields.io/badge/GHCR-181717?logo=github)](https://github.com/luniphy/taskmanager/pkgs/container/taskmanager)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

# Task Manager

A small console based task manager app using a **SQLite** database. This is a learning project to practice object-oriented programming, enums, file separation and SQL database operations.

### Example

```text
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

The app is designed to manage tasks in a simple and structured way in a local SQLite database. It allows users to create, view, find, update, remove, filter and sort tasks.

## Features

- **List tasks** – Display all tasks stored in the database.
- **Add task** – Create a task by entering a (unique) title, category, due date, priority & status.
- **Remove task** – Delete an existing task.
- **Find task** – Search for a task by title.
- **Change priority / status** – Update the priority (`Low` / `Medium` / `High`) or status (`Open` / `InProgress` / `Done`) of a task.
- **Filter by category** – Show only tasks that belong to a given category.
- **Filter by priority** – Show only tasks of a given priority level.
- **Filter by status** – Show only tasks of a given status.
- **Sort tasks** – Sort tasks by title, category, priority or status.
- **Date validation** – Due dates are validated including leap-years.
- **Input cancellation** - Every prompt allows to cancel and return to the main menu.
- **JSON export** – Export tasks to `data/tasks.json`.
- **SQLite database** - Stores tasks in `data/tasks_sql.db`.
- **Separated source files** - Separate task model, database manager and main loop.
- **Continuous Deployment (CD)**: Automated image pushes to Docker Hub / GHCR with vulnerability check

## Project structure

```
taskmanager/
├─ data/                    # Example files are given
│  ├─ tasks_sql.db          # SQLite database
│  └─ tasks.json            # JSON export
├─ src/
│  ├─ main.cpp              # Contains the CLI, menu loop, input validation and JSON export.
│  ├─ task.cpp              # Implements the Task class and the conversion functions.
│  ├─ task.hpp              # Declaration of the Task class.
│  ├─ taskmanager.cpp       # Implements SQLite operations for the Taskmanager class.
│  └─ taskmanager.hpp       # Declaration of the Taskmanager class.
├─ Dockerfile
├─ LICENSE
└─ README.md
```

## Build & Run

Compile from root:

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

### Pull from Docker Hub / GHCR

Prebuilt images are also available on [Docker Hub](https://hub.docker.com/r/luniphys/taskmanager):

```bash
docker pull luniphys/taskmanager:latest
```

And on the [GitHub Container Registry](https://github.com/luniphy/taskmanager/pkgs/container/taskmanager):

```bash
docker pull ghcr.io/luniphy/taskmanager:latest
```

### Run the container

```bash
docker run --rm -it -v $(pwd)/data:/app/data taskmanager
```

### Notes

- To run pulled images, use `luniphys/taskmanager` or `ghcr.io/luniphy/taskmanager` respectively.
- Run the container in interactive mode: `-it`
- The volume mount preserves the database and JSON file in the local `data/` directory after the container stops.

## License

MIT © [luniphy](https://github.com/luniphy)
