json_array:
	g++ -shared -o libjson_array_sort.so -I include -fPIC -std=c++17 json_array_sort.cc include/simdjson.cpp

list_at:
	g++ -shared -o libjson_array_sort.so -I include -fPIC -std=c++17 list_at.cc

testout:
	g++ -g -O0 -o func-test -I include -std=c++17 func-test.cc json_array_sort.cc include/simdjson.cpp list_at.cc

clean:
	rm -rvf lib*.so *.o func-test
