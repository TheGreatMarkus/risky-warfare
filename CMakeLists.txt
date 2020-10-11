cmake_minimum_required(VERSION 3.13)

project("Map")
add_executable(Map
        src/map/Map.cpp
        src/map/MapDriver.cpp
        src/player/Player.cpp
        src/orders/Orders.cpp
        src/cards/Cards.cpp)

project(MapLoader)
add_executable(MapLoader
        src/map-loader/MapLoader.cpp
        src/map-loader/MapLoaderDriver.cpp
        src/map/Map.cpp)

project(Orders)
add_executable(Orders
        src/orders/Orders.cpp
        src/orders/OrdersDriver.cpp
        src/map/Map.cpp
        src/player/Player.cpp)

project(Cards)
add_executable(Cards
        src/cards/Cards.cpp
        src/cards/CardsDriver.cpp)