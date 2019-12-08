# magnum-bazel
## Bazel integration for the Magnum graphics engine

[![Build Status](https://github.com/dbacchet/magnum-bazel/workflows/build/badge.svg)](https://github.com/dbacchet/magnum-bazel/actions)

This repository contains scripts that fetch the dependencies, build and package the [Magnum engine](https://magnum.graphics), and makes it available as a [bazel](https://bazel.build) package.
The packages are built for linux x86_64 (Ubuntu 16.04 and 18.04) and MacOS 10.14+.

The final package contains the following libraries:
* _Corrade_ ([github link](https://github.com/mosra/corrade))
* _Magnum_ ([github link](https://github.com/mosra/magnum)) built with glfw support
* _Magnum_ Integration ([github link](https://github.com/mosra/magnum-integration)) built with imgui support
* _GLFW 3.3_ ([github link](https://github.com/glfw/glfw))
* _Imgui 1.74_ ([github link](https://github.com/ocornut/imgui))
* _ImGuizmo_ ([github link](https://github.com/CedricGuillemet/ImGuizmo))
