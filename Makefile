# Compilador
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -I./bib

# Arquivos fonte
SRC = main.c funcs/loja.c

# Arquivos objeto
OBJ = $(SRC:.c=.o)

# Nome do executável
TARGET = programa

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação dos .c para .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJ) $(TARGET)

# Recompila tudo
rebuild: clean all