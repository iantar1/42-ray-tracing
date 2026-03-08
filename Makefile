NAME = rt
MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
CXX = g++
CXXFLAGS =  -fsanitize=address
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJ) */**.hpp
	$(CXX)  -MMD $(CXXFLAGS) -o $(NAME) $(OBJ) $(MLX_FLAGS)
# -MMD: This tells the compiler to generate .d files that contain dependency rules
# 		(i.e., which .hpp files each .cpp depends on).
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all


install:
	sudo apt update
	sudo apt install -y \
		libx11-dev \
		libxext-dev \
		libxrandr-dev \
		libxi-dev \
		libxinerama-dev \
		libxcursor-dev \
		libbsd-dev \
		zlib1g-dev
