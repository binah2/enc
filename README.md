# enc

## Build

This project uses the vcpkg manifest to fetch Botan 3.

```
git clone https://github.com/microsoft/vcpkg /tmp/vcpkg
/tmp/vcpkg/bootstrap-vcpkg.sh
/tmp/vcpkg/vcpkg install
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/tmp/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build build
./build/enc

> adjust `VCPKG_TARGET_TRIPLET` for your platform (e.g., x64-windows, x64-osx).
```
