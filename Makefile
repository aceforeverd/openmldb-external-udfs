CXXFLAGS ?= ''
# CXXFLAGS ?= '-g -O0'

json_array:
	g++ -shared -o libjson_array_sort.so -I include -fPIC -std=c++17 json_array_sort.cc include/simdjson.cpp $(CXXFLAGS) 

list_at:
	g++ -shared -o liblist_at.so -I include -fPIC -std=c++17 list_at.cc $(CXXFLAGS) 

testout:
	g++ -o func-test -I include -std=c++17 func-test.cc json_array_sort.cc include/simdjson.cpp list_at.cc $(CXXFLAGS) 

clean:
	rm -rvf lib*.so *.o func-test
