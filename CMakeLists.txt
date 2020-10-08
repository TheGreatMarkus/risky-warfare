cmake_minimum_required(VERSION 3.13)

project("Map")
add_executable(Map
        src/map/Map.cpp
        src/map/MapDriver.cpp
        src/player/Player.cpp)

project(MapLoader)
add_executable(MapLoader
        src/map-loader/MapLoader.cpp
        src/map-loader/MapLoaderDriver.cpp
        src/map/Map.cpp)

project(Order)
add_executable(Order
        src/orders/Orders.cpp
        src/orders/OrdersDriver.cpp)