rm ./a.out

if [ $1 = "testBase64" ]
then
	gcc $1.c -lcutlery
	echo -n "Rohan Vipulkumar Dvivedi" | ./a.out | base64 --decode -w 0
	echo ""
	echo -n "Rohan Vipulkumar Dvivedi" | base64 -w 0 | ./a.out --decode
	echo ""
else
	gcc $1.c -lcutlery && sudo valgrind -v ./a.out
fi