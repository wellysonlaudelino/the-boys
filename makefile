# Definindo variáveis
SRCS	= src
OBJS	= obj
INCL	= includes
OUT	= theboys
CC	= gcc

# Flags de compilação e link
CFLAGS	= -g -c -Wall -Wextra -Werror -Wshadow -I$(INCL)
LFLAGS	= -lm

SRC_FILES = $(wildcard $(SRCS)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRCS)/%.c=$(OBJS)/%.o)

$(OUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LFLAGS) -o $(OUT)

$(OBJS)/%.o: $(SRCS)/%.c
	@mkdir -p $(OBJS) # Cria o diretório obj se não existir
	$(CC) $(CFLAGS) $< -o $@ # Compila os arquivos .c em .o

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(OBJS) $(OUT)

# Regra padrão para compilar tudo
all: $(OUT)
