#! /bin/bash

BASE_DIR=`pwd`
MAGNUM_INSTALL_FOLDER=${BASE_DIR}/install
BUILD_TYPE=RelWithDebInfo

echo ${MAGNUM_INSTALL_FOLDER}
set -x

rm -rf ${MAGNUM_INSTALL_FOLDER}

MAGNUM_VERSION="v2019.10"

# install Corrade
cd ${BASE_DIR}
rm -rf corrade
git clone --branch ${MAGNUM_VERSION} --depth 1 https://github.com/mosra/corrade.git
cd corrade
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DBUILD_DEPRECATED=OFF \
      -DBUILD_STATIC=ON \
      -DBUILD_STATIC_PIC=ON \
      -DCMAKE_INSTALL_PREFIX=${MAGNUM_INSTALL_FOLDER} \
      ..
make -j8 && make install

# magnum dependencies
cd ${BASE_DIR}
rm -rf glfw
git clone --branch 3.3 --depth 1 https://github.com/glfw/glfw.git
cd glfw
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_PREFIX_PATH=${MAGNUM_INSTALL_FOLDER} \
      -DGLFW_BUILD_DOCS=OFF \
      -DGLFW_BUILD_EXAMPLES=OFF \
      -DGLFW_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=${MAGNUM_INSTALL_FOLDER} \
      ..
make -j8 && make install

# install magnum
cd ${BASE_DIR}
rm -rf magnum
git clone --branch ${MAGNUM_VERSION} --depth 1 https://github.com/mosra/magnum.git
cd magnum
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_PREFIX_PATH=${MAGNUM_INSTALL_FOLDER} \
      -DBUILD_DEPRECATED=OFF \
      -DBUILD_STATIC=ON \
      -DBUILD_STATIC_PIC=ON \
      -DCMAKE_INSTALL_PREFIX=${MAGNUM_INSTALL_FOLDER} \
      -DWITH_GLFWAPPLICATION=ON \
      ..
make -j8 && make install

# imgui dependency
cd ${BASE_DIR}
rm -rf imgui
git clone --branch v1.74 --depth 1 https://github.com/ocornut/imgui.git
# imguizmo
rm -rf ImGuizmo
git clone --depth 1 https://github.com/CedricGuillemet/ImGuizmo.git
rm -rf build_imgui
mkdir build_imgui && cd build_imgui
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX=${MAGNUM_INSTALL_FOLDER} \
      ..
make -j8 && make install

# install magnum-integration
cd ${BASE_DIR}
rm -rf magnum-integration
git clone --branch ${MAGNUM_VERSION} --depth 1 https://github.com/mosra/magnum-integration.git
cd magnum-integration
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_PREFIX_PATH=${MAGNUM_INSTALL_FOLDER} \
      -DBUILD_STATIC=ON \
      -DBUILD_STATIC_PIC=ON \
      -DCMAKE_INSTALL_PREFIX=${MAGNUM_INSTALL_FOLDER} \
      -DWITH_IMGUI=ON \
      ..
make -j8 && make install

# cleanup intermediate folders
cd ${BASE_DIR}
rm -rf corrade magnum imgui ImGuizmo build_imgui magnum-integration glfw
