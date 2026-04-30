./copy_to_cpp.ps1
cd ./build
cmake ./cpp
cmake --build .
echo ""
./Debug/Zmii.exe
