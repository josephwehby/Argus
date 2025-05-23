- create https request to binance to get past candles for chart
- switch orderbook to binance
- send pong frame every 30ish seconds or disconnected

- move dataparser to App class and pass it to ws and hc
- will have to redo how dp shuts down now most likely as it was originall only dependent on ws
- also hc data does not have any fields so we need a different parsing method for it in dataparser. kind of odd 
