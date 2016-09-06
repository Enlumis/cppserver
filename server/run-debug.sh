cmake . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug ; make clean ; make -j 8
echo "-----------------------------------  RUN  --------------------------------------"
cd Debug/
valgrind ./babel-server -track-origins=yes
cd ..
