CC 				  = gcc
CFLAGS			  = -Wall
CFLAGS_EXAMPLES	  = -I . Levenshtein_c.o

PATH_EXAMPLES	  = examples
TESTS 			  = code code1
EXTENSION		  = exe

examples: Levenshtein_c.o $(addprefix $(PATH_EXAMPLES)/, $(addsuffix .$(EXTENSION), $(TESTS)))
	@echo "generando tests... $^"

# Regla patrón: compila cada test a partir de su fuente .c
$(PATH_EXAMPLES)/%.$(EXTENSION): $(PATH_EXAMPLES)/%.c
	$(CC) $< $(CFLAGS_EXAMPLES) -o $@ 

Levenshtein_c.o: Levenshtein_c.c
	gcc -c $< -o $@