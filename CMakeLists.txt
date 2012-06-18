project(umisic)
cmake_minimum_required(VERSION 2.8)

# Standard install paths
include(GNUInstallDirs)

find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(KQOAUTH REQUIRED kqoauth)
pkg_check_modules(QJSON REQUIRED QJson)

add_subdirectory(src)
