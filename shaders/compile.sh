for filename in *.vert.slang; do
    if [ -f "$filename" ]; then
        slangc -stage vertex -entry start -o "./SPIRV/${filename/.slang/.spv}"  "$filename"
        slangc -stage vertex -entry start -o "./MSL/${filename/.slang/.metal}"    "$filename" 
    fi
done

for filename in *.frag.slang; do
    if [ -f "$filename" ]; then
        slangc -stage fragment -entry start  -o "./SPIRV/${filename/.slang/.spv}" "$filename"
        slangc -stage fragment -entry start  -o "./MSL/${filename/.slang/.metal}" "$filename"
    fi
done
