#include <dot-5/dot-5.h>

int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) return 1;

    wchar_t path[514] = {0};
    memcpy(path, argv[0], 512);

    size_t len = wcslen(path) - 1;
    while (path[len] != L'\\') path[len--] = L'\0';
    swprintf(path, 512, L"%ls%ls", path, L"config.cfg");

    if (!d5_load_w(argv[1], path)) return 1;

    d5_run();

    d5_exit();

    return 0;
}