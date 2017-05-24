
file(GLOB HEADERS ../include/*.h ../include/detail/*.h)
add_executable(hellow_world hellow_world/main.cpp ${HEADERS})