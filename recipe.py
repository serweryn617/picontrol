# Cooking recipe, checkout Cook at https://github.com/serweryn617/cook

default_project = "all"

projects = {}

projects["all"] = {
    "components": [
        "autoformat",
        "setup",
        "build",
        "enter_bootsel",
        "upload",
    ],
}

projects["setup"] = ("docker image inspect pico_builder >/dev/null 2>&1 || docker build -t pico_builder .",)

projects["build"] = (
    "mkdir -p build/generated/pio",
    'docker run --rm -u $(id -u):$(id -g) -v $(pwd)/firmware:/workspace/firmware -v $(pwd)/build:/workspace/build -w /workspace/build pico_builder /bin/bash -c "cmake ../firmware -GNinja && ninja"',
    "mkdir -p output",
    "cp build/main/pico_start.uf2 output/pico_start.uf2",
    'sed -i "s#/workspace#$(pwd)#g" build/compile_commands.json',
)

projects["clean"] = (
    "rm -r build",
    "rm -r output",
)

projects["autoformat"] = (
    "isort .",
    "black .",
    "git ls-files *.h *.hpp *.c *.cpp | xargs clang-format -i -style=file",
)

projects["serial"] = ("tio /dev/ttyUSB0",)

projects["enter_bootsel"] = ("picontrol enter_bootsel",)

projects["upload"] = (
    "timeout 10 bash -c 'while [[ ! -e /dev/disk/by-label/RPI-RP2 ]]; do sleep 0.2; done'",
    "sudo mount /dev/disk/by-label/RPI-RP2 /mnt",
    "sudo cp output/pico_start.uf2 /mnt",
    "sudo umount /mnt",
)
