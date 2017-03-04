NAME = ./bin/renderer

LIBFOLDER = ./libs

LIBC = $(LIBFOLDER)/C_data_structures
LIBC_STATIC = $(LIBC)/libcstruct.a

LIBM = $(LIBFOLDER)/C_maths
LIBM_STATIC = $(LIBM)/libcmaths.a

LIBGL = $(LIBFOLDER)/glfw
LIBGL_STATIC = $(LIBGL)/build/src/libglfw3.a

LIBTHREAD = $(LIBFOLDER)/tinycthread
LIBTHREAD_STATIC = $(LIBFOLDER)/tinycthread/libtinycthread.a

LIB_STATIC = $(LIBC_STATIC) $(LIBM_STATIC) $(LIBGL_STATIC) $(LIBTHREAD_STATIC)

SRCS = $(wildcard ./srcs/*.c)
C_OBJ = $(SRCS:.c=.o)
FLAGS = -Wall -Werror -Wextra
INC = -I ./includes -I $(LIBC)/includes -I $(LIBM)/includes -I $(LIBGL)/include -I $(LIBTHREAD)/source


UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LIB_OTHER = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif

ifeq ($(OS),Windows_NT)
	LIB_OTHER = -lgdi32 -lopengl32 -luser32 -lkernel32 -lglew32
endif

all: $(LIB_STATIC) $(NAME) $(HEADERS)

$(LIB_STATIC):
	- git submodule init
	- git submodule update --remote --merge
	make -C $(LIBC)
	make -C $(LIBM)
	- cd $(LIBGL) ; mkdir build ; cd build ; cmake -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake ../ ; make
	- cd $(LIBTHREAD) ; cmake . ; make

$(NAME): $(C_OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(C_OBJ) $(LIB_STATIC) $(LIB_OTHER)

%.o: %.c ./includes/renderer.h
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
