# Fuzzer

This fuzzer was created for the class of computer-security @ UCL.

It will generate tars, manipulate their headers and check if they crash an extractor.If the latter is true then it will rename the tars to successX.tar with X being the "position" in which the file was created (for some reason all the X's are odd numbers).

To change the extractor simply update the makefile to have your extractor's path or compile the fuzzer and add the path to your extractor as an argument.

## Compile
```
make
```
## Compile and Run
```
make run
```
## Clean
```
make clean
```
