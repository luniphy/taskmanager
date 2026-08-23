FROM gcc:latest AS builder

WORKDIR /app

COPY . /app/

RUN g++ src/taskmanager.cpp -o src/taskmanager -lsqlite3

FROM debian:stable-slim

WORKDIR /app

RUN mkdir -p /app/data

COPY --from=builder /app/src/taskmanager ./src/taskmanager

CMD ["./src/taskmanager"]
