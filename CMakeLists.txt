cmake_minimum_required(VERSION 3.13)

project("Map")
add_executable(
        Map
        src/map/MapDriver.cpp
        src/map/Map.cpp
        src/utils/Utils.cpp
)

project(MapLoader)
add_executable(
        MapLoader
        src/map-loader/MapLoader.cpp
        src/map-loader/MapLoaderDriver.cpp
        src/map/Map.cpp
        src/utils/Utils.cpp
)

project(Player)
add_executable(
        Player
        src/player/PlayerDriver.cpp
        src/player/Player.cpp
        src/cards/Cards.cpp
        src/orders/Orders.cpp
        src/utils/Utils.cpp
        src/map/Map.cpp
)

project(Orders)
add_executable(
        Orders
        src/orders/Orders.cpp
        src/orders/OrdersDriver.cpp
        src/cards/Cards.cpp
        src/map/Map.cpp
        src/player/Player.cpp
        src/utils/Utils.cpp
)

project(Cards)
add_executable(
        Cards
        src/cards/Cards.cpp
        src/cards/CardsDriver.cpp
        src/utils/Utils.cpp
        src/orders/Orders.cpp
        src/map/Map.cpp
        src/player/Player.cpp
)


