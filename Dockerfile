FROM gcc:latest AS builder

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    libsqlite3-dev cmake pkg-config git libssl-dev && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 -b releases/stable https://github.com/mongodb/mongo-cxx-driver.git /tmp/mongo-cxx-driver \
    && cmake -S /tmp/mongo-cxx-driver -B /tmp/mongo-cxx-driver/build \
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DENABLE_SSL=OPENSSL \
    && cmake --build /tmp/mongo-cxx-driver/build -j"$(nproc)" \
    && cmake --install /tmp/mongo-cxx-driver/build \
    && rm -rf /tmp/mongo-cxx-driver

COPY . /app/

RUN g++ -std=c++17 src/main.cpp src/task.cpp src/taskmanager.cpp -o src/main \
    -lsqlite3 $(pkg-config --cflags --libs libmongocxx1 libbsoncxx1)

FROM debian:stable-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    libsqlite3-0 libssl3 ca-certificates && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /app/data

COPY --from=builder /usr/local /usr/local

RUN ldconfig

COPY --from=builder /app/src/main ./src/main

CMD ["./src/main"]
