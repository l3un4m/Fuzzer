# Fuzzer

This fuzzer was created for the class of computer-security @ UCL.

It will generate tars, manipulate their headers and check if they crash an extractor. If the latter is true then it will rename the tars to successX.tar

To change the extractor simply update the makefile to have your extractor's path or compile the fuzzer and add the path to your extractor as an argument

## Compile
```
make
```
## Compile and Run with our Extractor
```
make run
```
## Run with other Extractor
```
./fuzzer ./[PATH TO EXTRACTOR]
```
## Clean
```
make clean
```
