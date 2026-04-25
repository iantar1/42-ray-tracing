NAME = rt
MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = $(MLX_LIB) -lXext -lX11 -lm -lz
CXX = g++
CXXFLAGS = -O3 -march=native
DEBUG_FLAGS = -g3 -O0 -fsanitize=address
OBJ_DIR = obj

SRC = main.cpp \
	  ./srcs/Camera.cpp \
	  ./srcs/Scene.cpp \
	  ./srcs/Objects.cpp \
	  ./srcs/Sphere.cpp \
	  ./srcs/Plane.cpp \
	  ./srcs/Cylinder.cpp \
	  ./srcs/Cone.cpp \
	  ./srcs/Light.cpp \
	  ./srcs/Ray.cpp \
	  ./srcs/utils.cpp \
	  ./srcs/Vec3.cpp \

OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))
DEP = $(OBJ:.o=.d)

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/srcs

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) -MMD $(CXXFLAGS) -c -o $@ $<

$(NAME): $(OBJ) */**.hpp
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) $(MLX_FLAGS)
# -MMD: This tells the compiler to generate .d files that contain dependency rules
# 		(i.e., which .hpp files each .cpp depends on).

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: re

clean:
	rm -rf $(OBJ_DIR)
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
