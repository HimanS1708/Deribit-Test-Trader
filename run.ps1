if (Test-Path "build") {
    Remove-Item -Path "build" -Recurse -Force
}

mkdir build
cd build

cmake .. -G "MinGW Makefiles"
mingw32-make

Copy-Item "../api_keys.json" -Destination "."

./DeribitTestTrader

cd ..
