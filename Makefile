#Do not edit the contents of this file.
warnings_BAD: *.c
	gcc -Werror -Wall -g -std=gnu99 -o Assignment_4 *.c -lrt
	
warnings_OK: *.c
	gcc -Wall -g -std=gnu99 -o Assignment_4 *.4 -lrt
	
clean: *.c
	rm Assignment_4
