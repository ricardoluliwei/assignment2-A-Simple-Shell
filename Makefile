all:
	@gcc src/sh.c -o shell
	@echo Compile Completed, Start Running
	@./shell

clean:
	@rm shell
	@echo Clean Completed