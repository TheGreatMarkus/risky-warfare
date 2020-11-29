cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project("Map")
add_executable(
        Map
        src/map/MapDriver.cpp
        src/map/Map.cpp
        src/utils/Utils.cpp
        src/player/Player.cpp
        src/orders/Orders.cpp
        src/cards/Cards.cpp
)

project(MapLoader)
add_executable(
        MapLoader
        src/map-loader/MapLoaderDriver.cpp
        src/map-loader/MapLoader.cpp
        src/utils/Utils.cpp
        src/map/Map.cpp
        src/player/Player.cpp
        src/orders/Orders.cpp
        src/cards/Cards.cpp
)

project(Player)
add_executable(
        Player
        src/player/PlayerDriver.cpp
        src/player/Player.cpp
        src/utils/Utils.cpp
        src/orders/Orders.cpp
        src/map/Map.cpp
        src/cards/Cards.cpp
)

project(Orders)
add_executable(
        Orders
        src/orders/Orders.cpp
        src/orders/OrdersDriver.cpp
        src/utils/Utils.cpp
        src/cards/Cards.cpp
        src/map/Map.cpp
        src/player/Player.cpp
)

project(Cards)
add_executable(
        Cards
        src/cards/Cards.cpp
        src/cards/CardsDriver.cpp
        src/utils/Utils.cpp
        src/orders/Orders.cpp
        src/player/Player.cpp
        src/map/Map.cpp
)

project(GameEngine)
add_executable(
        GameEngine
        src/GameEngine.cpp
        src/utils/Utils.cpp
        src/map/Map.cpp
        src/map-loader/MapLoader.cpp
        src/player/Player.cpp
        src/orders/Orders.cpp
        src/cards/Cards.cpp
        src/observers/GameObservers.cpp
        src/player/PlayerStrategies.cpp
)
