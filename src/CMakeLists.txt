add_executable(
	server 
	main.cpp 
	./server/server.cpp 
	./context/connection.cpp 
	./http_parser/http_parser.cpp 
	./request/request.cpp
  ./response/response.cpp
	./utils/common.cpp
	./constant/common.cpp
)

