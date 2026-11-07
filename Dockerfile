FROM gcc:14 AS builder

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y --no-install-recommends cmake && rm -rf /var/lib/apt/lists/*
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --config Release --parallel

FROM debian:trixie-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /app/build/src/server_bin /app/tcp-server

ENV PORT=8080
EXPOSE 8080

CMD ["/app/tcp-server"]