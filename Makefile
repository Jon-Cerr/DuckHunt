MAIN=main.c
COMPILER=gcc
APP=main.exe
GRAPHICS_H=.\src\graficos.h
GRAPHICS_SRC=.\src\graficos.c
WIN_FLAGS=-mwindows
SERIAL=.\src\serial.c
SERIAL_H=.\src\serial.h
SCONTROLLER=.\src\simplecontroller.c
SCONTROLLER_H=.\src\simplecontroller.h
GAME_CODES= .\src\imagenes.c .\src\pato.c .\src\juego.c .\src\marcadores.c
WINMM_LIB=-lwinmm


run: $(APP)
	./$(APP)

$(APP): $(MAIN) $(GRAPHICS_H) $(GRAPHICS_LIB) $(SERIAL_H) $(SERIAL) $(SCONTROLLER) $(SCONTROLLER_H) $(GAME_CODES)
	$(COMPILER) -o $(APP) $(GRAPHICS_LIB) $(MAIN) $(WIN_FLAGS) $(SERIAL) $(SCONTROLLER) $(WINMM_LIB) $(GAME_CODES)

$(GRAPHICS_LIB): $(GRAPHICS_H) $(GRAPHICS_SRC)
	$(COMPILER) -c $(GRAPHICS_SRC) -o $(GRAPHICS_LIB)

clean: 
	del $(APP) $(GRAPHICS_LIB)