y = 1
f = 0
gf = 5
pf = 5
df = 5
m1f = 5
m2f = 5

all:
	g++ -Wall -pedantic -std=c++11 ims.cpp -o ims -lsimlib
run:
	./ims -f $(f) -y $(y) -gf $(gf) -pf $(pf) -df $(df) -m1f $(m1f) -m2f $(m2f)
help:
	./ims --help