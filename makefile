chip8-wasm:
	em++ -O2 \
	./chip8/main.cc \
	./chip8/chip8.cc \
	./chip8/emulator.cc \
	./chip8/window.cc \
	./chip8/interpreter.cc \
	-s --std=c++17 \
	-s WASM=1 \
	-s USE_SDL=2 \
	-s EXPORTED_FUNCTIONS='['_main', '_load_game']' \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	-o ./dist/chip8.js
