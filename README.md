# MultiKB
## Simple windows functions to read seperate input from multiple keyboards

## Usage:
### Setup:
Call `mkb_init()` to setup internal things and allocate memory.<br>
Only one instance of this is allowed. I made that questionable design choice of allowing multiple instances of this last time.<br>
There is no need for this to even support multiple instances, because this manages ALL of the keyboards on the system.<br>
### Update Loop:
The `mkb_update()` function runs a message loop to get key changed messages, then sets `lastState` to `state` for each key (used to tell if a key was just changed).<br>
It returns:<br>
`mkb_DEVICE_NONE` (0) if no devices were changed<br>
`mkb_DEVICE_CONNECT` (1) if a new device was added<br>
`mkb_DEVICE_RECONNECT` (2) if the new device has the same name as a previous device<br>
`mkb_DEVICE_DISCONNECT` (3) if a device was removed<br>
The function `mkb_getLastEvent()` returns the same thing.<br>
<br>
Use `mkb_getLatestDevice()` to get the index of the last device changed.<br>
Use `mkb_deviceCount()` to get the total device count (including disconnected devices).<br>
Use `mkb_deviceConnectedCount()` to get the active device count (excluding disconnected devices).<br>
The device index does not shift over when a device is removed,<br>
because the project I am making this for uses keyboards like controllers, and shifting the player indexes would mess up many things.<br>
Use `mkb_getNthDevice()` to get the index of the nth device.<br>
Ex: `mkb_getNthDevice(0) -> 2` the index of the first device is 2.<br>
<br>
### Key States:
`mkb_key()` gets the current key state.<br>
`mkb_keyLast()` gets the last key state.<br>
`mkb_keyDown()` gets if the key was just pressed `(state && !lastState)`.<br>
`mkb_keyUp()` gets if the key was just released `(!state && lastState)`.<br>
### Shutdown:
Call `mkb_shutdown()` to free memory when you are done.<br>
To use this again, you have to call `mkb_init()` again.<br>
<br>
#### Extras:
The key codes are aligned to the ascii table, so you can use a character instead of the key name. (Uppercase)<br>
The array `mkb_keyNames` contains key names made for displaying to users.<br>
I used to allow capslock and numloc keys to toggle, but they are global states, so that didn't end up being helpful.<br>
## C++ Wrapper:
MultiKB class<br>
It is just a very thin wrapper.<br>
It just calls the C functions, nothing else, only one instance will work<br>
It doesn't do anything different, just some people are very unhappy about touching C at all :(<br>
<br>
<br>
```
         ___   ___   ___
        /   \ /   \ /   \
   /\/\|  |  |  |  |  |  |
  ('w' |  |     |  |  |  |__
   \_____/ \___/ \___/ \____)

           Inch Cat
```
