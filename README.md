# Argus
- Cryptocurrency viewer
- Connects to the Binance exchange and displays various info such as the orderbook, trades, price chart, etc.
- Uses ImGui to draw eveything to the screen with a glfw and OpenGL backend
- Uses ImPlot for the chart widget
- Use Boost Beast websockets to connect to the Binance exchange
- Argus is a dog in Greek Mythology with 100 eyes (cause you're watching all the crypto prices. very creative)

![Main Screen](images/argus.png)

# Compiling
- This program is designed to run on windows
- ImGui and ImPlot are submodules so you need to clone recursively
- cpp-httplib is also a third party library but the header file is already included
- The remaining 3rd party libraries are boost-beast, openssl, and nlohmann-json
  - These packages can be installed with vcpkg like so `vcpkg install package-name`
- To download run `git clone --recursive git@github.com:josephwehby/Argus.git`
