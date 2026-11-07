# TCP server

## Run locally

From the `server` directory:

```sh
cmake -S . -B build
cmake --build build
./build/src/server_bin
```

The server listens on port `8080` by default. To choose another port:

```sh
PORT=9000 ./build/src/server_bin
```

## Build and run with Docker

Run these commands from the `server` directory, where the `Dockerfile` is:

```sh
docker build -t tcp-server:local .
docker run --rm --name tcp-server -e PORT=8080 -p 8080:8080 tcp-server:local
```

In another terminal, send a request:

```sh
curl -v http://localhost:8080/
```

`PORT` controls the port the server listens on inside the container. `-p 8080:8080` publishes that container port on the host. Change both values together if testing another port.

## Push to Docker Hub

```sh
docker login
docker tag tcp-server:local kyoani/tcp-server:latest
docker push kyoani/tcp-server:latest
```

## Deploy on Koyeb

Create a Koyeb service from the Docker image:

```text
kyoani/tcp-server:latest
```

Configure the service to listen on port `8080` (HTTP/TCP as appropriate for the service). The application reads Koyeb's `PORT` environment variable; if it is not set, it uses `8080`.
