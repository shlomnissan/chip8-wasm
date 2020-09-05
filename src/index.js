require("./chip8/chip8");
require("./assets/styles/style.less");

document.addEventListener("DOMContentLoaded", function () {
    document.querySelector("#game-selection").onchange = async (event) => {
        const filename = event.target.value;
        const response = await fetch(`./roms/${filename}`);
        const arrayBuffer = await response.arrayBuffer();
        const gameData = new Uint8Array(arrayBuffer);

        const loadGame = Module.cwrap('load_game', 'null', ['array']);
        loadGame(gameData);
    }
});