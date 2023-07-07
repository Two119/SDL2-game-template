import subprocess
subprocess.run("g++ -o test.exe main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_Image -lSDL2_mixer -lSDL2_mixer.dll -lSDL2_ttf -lSDL2_ttf.dll -I src -std=c++20")
subprocess.run("test.exe")
