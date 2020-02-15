# chip8-wasm [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.com/shlomnissan/chip8-wasm.svg?branch=master)](https://travis-ci.com/shlomnissan/chip8-wasm)

Created in the '70s by Joseph Weisbecker, Chip-8 is a virtual machine that allowed games to be more easily programmed for early microcomputers. This emulator lets you load Chip-8 games in JavaScript and play them in the browser using WebAssembly.

## Demo:

https://shlomnissan.github.io/chip8-wasm/

![chip8](https://user-images.githubusercontent.com/3165988/61174034-d79e8380-a54f-11e9-9ee5-05351d67d9b5.gif)

## TODO's:

- [ ] Add system sound using JavaScript
- [x] Build a simple UI
- [ ] Fix memory issues, improve performance
- [ ] Apply an optional flicker fix

## Installation:

1. Install Emscripten: https://emscripten.org/docs/getting_started/downloads.html

2. Clone the repository: `git clone https://github.com/shlomnissan/chip8-wasm`

3. Run `make` to generate the WASM module using Emscripten

4. Run `npm run start` to start webpack

5. Chrome doesn't support file XHR requests so you need to open index.html from a web server.

## MIT license:	

 ```	
Copyright (c) 2020 Shlomi Nissan	
 Permission is hereby granted, free of charge, to any person obtaining a copy	
of this software and associated documentation files (the "Software"), to deal	
in the Software without restriction, including without limitation the rights	
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell	
copies of the Software, and to permit persons to whom the Software is	
furnished to do so, subject to the following conditions:	
 The above copyright notice and this permission notice shall be included in all	
copies or substantial portions of the Software.	
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,	
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE	
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER	
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE	
SOFTWARE.	
```
