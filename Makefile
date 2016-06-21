all:
	@gcc trab1.c -o trab -lglut -lGLU -lGL -lm -Wall

run:
	@./trab

clean:
	@rm trab