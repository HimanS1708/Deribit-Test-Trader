if (!(Test-Path "vcpkg/.git")) {
    git submodule update --init --recursive vcpkg
}

.\vcpkg\bootstrap-vcpkg.bat

$env:VCPKG_ROOT="$pwd/vcpkg"
$env:PATH="$env:VCPKG_ROOT;$env:PATH"

if (!(vcpkg list | Select-String "openssl")) {
    vcpkg install openssl:x64-mingw-static
}

if (!(vcpkg list | Select-String "boost-thread")) {
    vcpkg install boost-thread:x64-windows
}

if (!(vcpkg list | Select-String "nlohmann-json")) {
    vcpkg install nlohmann-json:x64-windows
}

if (!(vcpkg list | Select-String "websocketpp")) {
    vcpkg install nlohmann-json:x64-windows
}

if (Test-Path "build") {
    Remove-Item -Path "build" -Recurse -Force
}

mkdir build
cd build

cmake .. -G "MinGW Makefiles"
mingw32-make

Copy-Item "../api_keys.json" -Destination "."

Clear-Host

./DeribitTestTrader

cd ..
