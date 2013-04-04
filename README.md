wMusic
==========

Streaming system using Spotify.

### Installation

To install wMusic on your RaspberryPi you have to :
 * Clone this repo
 * Execute Scripts/install_dependency.sh
 * Execute Scripts/launch_bridge.sh

wMusic is now working :)

### Using it

Now to use it , you have to clone the repo : https://github.com/wVibes/wRemote , it's the mobile application to use it.

### Sonos-Like system

wMusic is design to act like Sonos system ( Bridge and Connectors ). 
That's mean, you can plug a RaspberryPi behind your ISP Box provider, this RaspberryPi will act like a dispatcher, and 
dispatch your music onto the Connectors.

Connectors are simply RaspberryPi plug into your Hi-Fi systems.

For example if you have one Hi-Fi system in your livingroom and another in your bedroom, you will be able to listen
the same music on these 2 rooms ( the music will be synchro and at the same volume ).

### Debug

You can debug your application even if the application is running, simply open a netcat on port 1339
 * nc __ip__ 1339

This will open a console, which allow to use some advanced command ! ( current memory allocation, modify trace level ,
dump the playqueue )

To sum up it's for advanced users :).
