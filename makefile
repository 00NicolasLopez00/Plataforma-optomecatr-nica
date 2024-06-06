# BIBLIOGRAFIA:
#    https://makefiletutorial.com/
#Tutorial 

# Define el origen de los datos
SESION  = 0406
SRC_DIR = ../
TYPE    = cpp

# Crear una lista con los archivos fuente
TGT_DIR   = ..\.build
TARGET    = $(TGT_DIR)\Salida.exe
OBJ_DIR   = $(TGT_DIR)\object
SOURCE   := $(wildcard $(SRC_DIR)$(SESION)*.$(TYPE))
OBJECT    = $(subst $(SRC_DIR),$(OBJ_DIR)\,$(SOURCE:.$(TYPE)=.o))

# Configuracion del compilador de recursos
_WINDRES  = --preprocessor=x86_64-w64-mingw32-g++.exe \
 --preprocessor-arg=-E \
 --preprocessor-arg=-xc-header \
 --preprocessor-arg=-DRC_INVOKED

# Lista de archivos auxiliares
CLEAN_LST = \
  $(wildcard $(subst \,/,$(TARGET))) \
  $(wildcard $(OBJ_DIR)/$(SESION)*.o)

# Crear ejecutable enlazando archivos objeto 
all: $(TARGET)

# Llama al enlazador 
$(TARGET) : $(OBJECT) $(OBJ_DIR)\\Resource.o
	g++ -g -mwindows $^ -o $(TARGET)

# Compilar archivo de recursos
$(OBJ_DIR)\\Resource.o: $(wildcard $(SRC_DIR)$(SESION)*.rc)
	windres $(_WINDRES) -i $< -o $@

# Crear archivos objeto -DUNICODE 
$(OBJ_DIR)\\%.o: $(SRC_DIR)%.$(TYPE)
	g++ -g -c -DUNICODE -D_UNICODE $^ -o $@

# Borrar archivos auxilares y el archivo fuente
clean: 
	del $(subst /,\,$(CLEAN_LST))

# Borrar archivos auxiliares
cleanAux:
	del $(subst /,\,$(wildcard $(OBJ_DIR)/*.o))
  