CXX ?= clang++

test:
	$(CXX) -std=c++14 xaml.gen.example.cc -o xaml.gen.example.out -I..

install:
	cp -r . /usr/include/cis-workflow-gen

clean:
	rm -f xaml.gen.example.out

