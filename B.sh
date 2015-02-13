#! /bin/bash
rm bob
g++ Point.cpp CE.cpp client.cpp random.cpp -o bob -lgmpxx -lgmp
