all: main
main:
	g++ -o thread_and_stack_management thread_and_stack_management.cpp -lpthread
	g++ -o thread_and_pipe thread_and_pipe.cpp -lpthread
	g++ -o thread_and_dependention thread_and_dependention.cpp -lpthread
	g++ -o block_and_resume block_and_resume.cpp -lpthread
clean:
	rm -rf thread_and_stack_management thread_and_pipe block_and_resume
	make -C criticalResource clean
	make -C hellothread clean
	make -C sched_priority clean
