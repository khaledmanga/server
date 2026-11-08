# HTTP Versions and Features

## HTTP/0.9 (1991) – The Prototype
* **Single method support:** Uses only the `GET` method to request data from the server.
* **HTML exclusive:** Responses are strictly limited to plain HTML text documents.
* **No HTTP headers:** Transmits no metadata, configuration, or content details alongside the request or response.
* **No status codes:** Returns a descriptive HTML file detailing the error instead of numeric codes if a failure occurs.

## HTTP/1.0 (1996) – Media Extension
* **Introduction of headers:** Allows transmission of metadata including browser details, accepted languages, and configuration settings.
* **Content-Type (MIME) support:** Enables transmission of rich media like images, videos, audio, and script files beyond plain HTML.
* **New methods added:** Introduces `POST` for sending data to the server and `HEAD` for requesting headers only.
* **Standardized status codes:** Implements response status categories such as 200 OK, 404 Not Found, and 500 Internal Server Error.
* **Short-lived connections:** Requires opening and closing a separate TCP connection for every single request/response pair.

## HTTP/1.1 (1997) – Standardization and Connection Optimization
* **Persistent connections:** Introduces `Keep-Alive` by default to reuse a single TCP connection for multiple resources, reducing handshake latency.
* **Pipelining mechanism:** Allows clients to send multiple requests concurrently without waiting for the previous response, though susceptible to Head-of-Line (HOL) blocking.
* **Host header support:** Enables Virtual Hosting, allowing multiple domains and websites to reside on a single IP address.
* **Chunked transfer encoding:** Permits servers to stream data in dynamic chunks without needing to declare the total file size upfront.
* **Advanced caching:** Introduces robust cache control headers including `Cache-Control` and `ETag` for precise browser storage management.

## HTTP/2 (2015) – Performance Enhancement via Binary
* **Binary protocol:** Replaces plain text format with binary framing, making data parsing faster, more compact, and less error-prone.
* **Multiplexing:** Allows hundreds of concurrent requests and responses over a single TCP connection, eliminating application-layer Head-of-Line blocking.
* **Header compression (HPACK):** Compresses header fields and eliminates redundant data between requests to save substantial bandwidth.
* **Server Push:** Empowers the server to proactively send critical resources (like CSS and JS files) to the browser before the client explicitly requests them.

## HTTP/3 (2022) – The QUIC and UDP Era
* **QUIC transport protocol:** Replaces the traditional TCP protocol with QUIC, which runs on top of UDP.
* **No transport-layer HOL blocking:** Prevents packet loss in one stream from stalling or interrupting other independent data streams on the same connection.
* **Zero-RTT handshake:** Minimizes connection setup latency by allowing clients to send encrypted data immediately during the initial handshake.
* **Connection migration:** Uses unique connection IDs instead of IP addresses, keeping active downloads uninterrupted when switching networks (e.g., from Wi-Fi to 5G).
* **Native TLS 1.3 encryption:** Integrates transport-layer security directly into the protocol core, making encryption mandatory and unskippable.
