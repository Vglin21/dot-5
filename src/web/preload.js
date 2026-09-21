Module.preRun = Module.preRun || [];

function fetch_into(url, virtual_path) {
    Module.addRunDependency(virtual_path);
    return fetch(url)
        .then(r => r.arrayBuffer())
        .then(buf => {
            FS.writeFile(virtual_path, new Uint8Array(buf));
            Module.removeRunDependency(virtual_path);
        });
}

Module.preRun.push(function () {
    fetch_into("rom.d5", "rom.d5");
    fetch_into("dot-5.cfg", "dot-5.cfg");
});

function resize_canvas() {
    const canvas = document.getElementById("canvas");
    canvas.width  = window.innerWidth;
    canvas.height = window.innerHeight;
    Module._display_resize_web(window.innerWidth, window.innerHeight);
}
window.addEventListener("resize", resize_canvas);