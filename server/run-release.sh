cmake . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release ; make clean ; make -j 8
echo "-----------------------------------  RUN  --------------------------------------"

cd Release
./babel-server 
cd ..
