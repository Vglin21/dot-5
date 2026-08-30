Module.preRun = Module.preRun || [];

function fetchInto(url, virtual_path) {
    Module.addRunDependency(virtual_path);
    return fetch(url)
        .then(r => r.arrayBuffer())
        .then(buf => {
            FS.writeFile(virtual_path, new Uint8Array(buf));
            Module.removeRunDependency(virtual_path);
        });
}

Module.preRun.push(function () {
    fetchInto("rom.d5", "rom.d5");
    fetchInto("config.cfg", "config.cfg");
});