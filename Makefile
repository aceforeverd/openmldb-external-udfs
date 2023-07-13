json_array:
	g++ -shared -o libjson_array_sort.so -I include -I . -fPIC -std=c++17 json_array_sort.cc simdjson.cpp

testout:
	g++ -g -o func-test -I include -I . -std=c++17 func-test.cc json_array_sort.cc simdjson.cpp

clean:
	rm -rf lib*.so *.o func-test
