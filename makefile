chip8-wasm:
	em++ -O2 \
	./chip8/src/main.cc \
	./chip8/src/chip8.cc \
	./chip8/src/emulator.cc \
	./chip8/src/window.cc \
	./chip8/src/interpreter.cc \
	--preload-file roms/INVADERS.ch8 \
	-s --std=c++17 \
	-s WASM=1 \
	-s USE_SDL=2 \
	-s EXPORTED_FUNCTIONS='['_main', '_load_game']' \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	-o ./dist/chip8.js
