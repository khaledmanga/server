# C++ HTTP Server

A multithreaded HTTP/1.1 server written in C++17 for Linux. It accepts connections with `epoll`, processes requests on a thread pool, and exposes an Express-style API.

## Overview

- **Event loop**: `epoll` + `eventfd`, non-blocking accept
- **Thread pool**: runs request handlers concurrently
- **HTTP parser**: parses the request line, headers and body
- **Router**: maps `GET`, `POST`, `PUT`and `PATCH` requests to handlers and support for dynamic path parameters such as `/users/:id`.
- **Request / Response**: JSON support via nlohmann/json
- **Logger**: thread-safe, with pluggable sinks
- **Graceful shutdown**: handles `SIGINT` and `SIGTERM`

## Tech stack

- C++17
- CMake (>= 3.17)
- [nlohmann/json](https://github.com/nlohmann/json)
- [GoogleTest](https://github.com/google/googletest)
- vcpkg (optional)
- clang-format
- GitHub Actions

## Project structure

```
src/
  main.cc          entry point and route definitions
  app/             wires router, thread pool and logger together
  server/          sockets, accept loop, shutdown
  event_loop/      epoll wrapper
  thread_pool/     worker threads and task queue
  context/         per-connection read/write handling
  http_parser/     HTTP request parser
  request/         Request, Header, Body
  response/        Response
  router/          route matching and path params
  logger/          logger and sinks
  constant/        enums and status codes
  utils/           helpers
test/              unit tests, mirroring src/
scripts/           build helper
diagram.puml       class diagram
```

## Setup

Requirements: Linux, a C++17 compiler, CMake and git.

```bash
sudo apt update
sudo apt install -y build-essential cmake git
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Or with vcpkg:

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --parallel
```

## Run

```bash
./build/src/server
```

The server listens on port 3000.

```bash
curl http://localhost:3000/

curl -X POST http://localhost:3000/ \
  -H "Content-Type: application/json" \
  -d '{"message": "hello"}'
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Format

```bash
make format
```