FROM gcc:latest AS builder

WORKDIR /app

COPY . /app/

RUN apt-get update && apt-get install -y --no-install-recommends libsqlite3-dev && rm -rf /var/lib/apt/lists/*

RUN g++ -std=c++17 src/main.cpp src/task.cpp src/taskmanager.cpp -o src/main -lsqlite3

FROM debian:stable-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends libsqlite3-0 && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /app/data

COPY --from=builder /app/src/main ./src/main

CMD ["./src/main"]
