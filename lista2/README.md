# Arithmetic Adaptive Encoding
### encode.cpp contains encoder and decode.cpp decoder. Follow steps below to run the program:
1. make
2. ./encode ${file_to_encode} ${file_name_after_encode}
3. ./decode ${file_to_decode} ${file_name_after_decode}

WARNING:If program is not compiling due to line 5 or 160  that means that four compiler version does not support filesystem library. You can either comment those lines or try updating your compiler

Example:
./encode test1.bin coded_test1
./decode coded_test1 decoded_test1.bin
