for filename in *.vert.hlsl; do
    if [ -f "$filename" ]; then
        glslc -fshader-stage=vertex "$filename" -o "./SPIRV/${filename/.hlsl/.spv}"
    fi
done

for filename in *.frag.hlsl; do
    if [ -f "$filename" ]; then
        glslc -fshader-stage=fragment "$filename" -o "./SPIRV/${filename/.hlsl/.spv}"
    fi
done
