rm ./a.out

if [ $1 = "testBase64" ]
then
	gcc $1.c -lcutlery
	echo "Rohan Vipulkumar Dvivedi" | ./a.out | base64 --decode
	echo "Rohan Vipulkumar Dvivedi" | base64 | ./a.out --decode
else
	gcc $1.c -lcutlery && sudo valgrind -v ./a.out
fi