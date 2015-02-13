#! /bin/bash
rm alice
g++ Point.cpp CE.cpp server.cpp random.cpp -o alice -lgmpxx -lgmp
