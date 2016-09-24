NAME = ./bin/renderer

LIBFOLDER = ./libs

LIBC = $(LIBFOLDER)/C_data_structures
LIBC_STATIC = $(LIBC)/libcstruct.a

LIBM = $(LIBFOLDER)/C_maths
LIBM_STATIC = $(LIBM)/libcmaths.a

LIBGL = $(LIBFOLDER)/glfw
LIBGL_STATIC = $(LIBGL)/build/src/libglfw3.a

LIB_STATIC = $(LIBC_STATIC) $(LIBM_STATIC) $(LIBGL_STATIC)
LIB_OTHER = -lgdi32 -lopengl32 -luser32 -lkernel32 -lglew32

SRCS = $(wildcard ./srcs/*.c)
C_OBJ = $(SRCS:.c=.o)
CC = x86_64-w64-mingw32-gcc
FLAGS = -Wall -Werror -Wextra
INC = -I ./includes -I $(LIBC)/includes -I $(LIBM)/includes -I $(LIBGL)/include

all: $(LIB_STATIC) $(NAME)

$(LIB_STATIC):
	git submodule init
	git submodule update
	make -C $(LIBC)
	make -C $(LIBM)
	- cd $(LIBGL) ; mkdir build ; cd build ; cmake -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake ../ ; make

$(NAME): $(C_OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(C_OBJ) $(LIB_STATIC) $(LIB_OTHER)

%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $< $(INC)

clean:
	make -C $(LIBC) clean
	make -C $(LIBM) clean
	rm -rf $(C_OBJ)

fclean: clean
	make -C $(LIBC) fclean
	make -C $(LIBM) fclean
	rm -rf $(NAME)

re: fclean all
