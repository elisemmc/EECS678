# Add the dp_asymmetric and dp_waiter targets to all as you implement
# them
#
STUDENT_ID = "2298962"
MY_LAST_NAME = "McEllhiney"

all: function_ptrs

function_ptrs: function_ptrs.c
	gcc -g function_ptrs.c -lpthread -lm -o function_ptrs

# Add the dp_asymmetric_test and dp_waiter_test targets to test as you implement
# them
#
test: function_ptrs_test

function_ptrs_test: function_ptrs
	./function_ptrs

clean:
	rm -f function_ptrs

tar: 
	make clean
	mkdir $(STUDENT_ID)-function_ptrs-lab
	cp Makefile function_ptrs.c process.txt $(STUDENT_ID)-function_ptrs-lab/
	tar cvzf $(STUDENT_ID)-function_ptrs-lab.tar.gz $(STUDENT_ID)-function_ptrs-lab
	rm -rf $(STUDENT_ID)-function_ptrs-lab

