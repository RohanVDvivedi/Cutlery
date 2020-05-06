cd ..
make clean all
cd test
gcc $1.c -I$CUTLERY_PATH/inc -L$CUTLERY_PATH/bin -lcutlery
./a.out