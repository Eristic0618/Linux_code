.PHONY:all
all:clientcal servercal

clientcal:CilentCal.cc
	g++ -o $@ $^ -std=c++11
servercal:ServerCal.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f clientcal servercal