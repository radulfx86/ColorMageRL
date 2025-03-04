/home/fre/local/emsdk/upstream/emscripten/emcc \
    -o web/colorMage.js \
    -sUSE_SDL=2 \
    -sFULL_ES3=1 \
    -sWASM=0 \
    -I src/scenes -I src -I src/ecs \
    $(find src -type f -name *.cpp) \
    --preload-file shaders \
    --preload-file assets/images/characters.png \
    --preload-file assets/images/tiles.png \
    --preload-file assets/images/font4_8.png \
    --preload-file assets/images/font8_12.png \
    --use-preload-plugins \
    -v
