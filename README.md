order-book
#INSTRUCTION 
mkdir build
cd build
cmake ..
make


Run application:
binaries will be generated into bin directory

cd bin
./order_book
./order_book < "file_name"


Run unit test suite:
cd bin
./order_book_test
