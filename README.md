# HTTP Server From Scratch

This is an HTTP server made from scratch in C. It supports static content through GET and POST requests.

## Usage

1. Compile the project:
```
make
```

2. Start the HTTP server:
```
./build/http_server 127.0.0.1 8080 test
```
Here the server is listening on port 8080 on localhost, and serves files from the test directory.

3. Access `localhost:8080/index.html` from you browser.

Note: this project lacks many security features found in production-ready HTTP servers, so use it only in local.

## Code structure

1. `boostrap` contains code that initializes the HTTP server.
2. `server` contains the code that handles HTTP requests and sends HTTP responses.
3. `http_parser` is used by `server` to parse HTTP headers.
4. `error_html_pages` contains html files that are sent in case of 404 not found, 400 bad request, etc.
5. `test` can be used as a root directory to test the HTTP server.