# Cooking recipe, checkout Cook at https://github.com/serweryn617/cook

default_project = 'all'

projects = {}

projects['all'] = {
    'components': [
        'clean_tusb',
        'setup',
        'build',
    ],
}

projects['setup'] = (
    'docker image inspect pico_builder >/dev/null 2>&1 || docker build -t pico_builder .',
)

projects['build'] = (
    'mkdir -p build/generated/pio',

    'docker run --rm -u $(id -u):$(id -g) -v $(pwd)/firmware:/workspace/firmware -v $(pwd)/build:/workspace/build -w /workspace/build pico_builder /bin/bash -c "cmake ../firmware -GNinja && ninja"',

    'mkdir -p output',
    'cp build/main/pico_start.uf2 output/pico_start.uf2',

    'sed -i "s#/workspace#$(pwd)#g" build/compile_commands.json',
)

projects['clean'] = (
    'rm -r build',
    'rm -r output',
)

projects['clean_tusb'] = (
    'rm -r build/lib/tinyusb',
)
