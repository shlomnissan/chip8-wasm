document.addEventListener("DOMContentLoaded", function () {
    document.querySelector("#game-selection").onchange = async (event) => {
        const filename = event.target.value;
        const response = await fetch(`./roms/${filename}`);
        const arrayBuffer = await response.arrayBuffer();
        const gameData = new Uint8Array(arrayBuffer);

        const loadGame = Module.cwrap('init', 'null', ['array']);
        loadGame(gameData);
    }
});