chip8-wasm:
	em++ -O2 \
	./src/chip8/src/main.cc \
	./src/chip8/src/chip8.cc \
	./src/chip8/src/emulator.cc \
	./src/chip8/src/window.cc \
	./src/chip8/src/interpreter.cc \
	-s --std=c++17 \
	-s WASM=1 \
	-s USE_SDL=2 \
	-s EXPORTED_FUNCTIONS='['_main', '_load_game']' \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	-o ./src/chip8/chip8.js
