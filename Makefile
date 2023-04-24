chat:md5.o sc.o rsa.o binaryHelpFunc.o
	g++ -static -g -o chat md5.o sc.o rsa.o binaryHelpFunc.o -lwsock32
sc.o:
	g++ -static -c sc.cpp
md5.o:
	g++ -static -c md5.cpp
rsa.o:
	g++ -static -c rsa.cpp
binaryHelpFunc.o:
	g++ -static -c BigInt\binaryHelpFunc.cpp
clean:
	del chat.exe
	del sc.o
	del md5.o
	del rsa.o
	del binaryHelpFunc.o
run:
	./chat