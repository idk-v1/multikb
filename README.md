# MultiKB
## Simple windows functions to read seperate input from multiple keyboards

How this works:<br>
A hidden window is created that recieves input messages.<br>
This program registers for raw input messages, and will recieve them as long as any window owned by this program is active.<br>
<br>
The multiKB_Update function runs a message loop to get key changed messages, then sets the last key state to the current key state for comparing later.<br>

## Usage:
Create a KBManager struct that will store all of the keyboard data.<br>
Call multiKB_Setup() to setup internal things and allocate memory.<br><br>
Using toggle keys like capslock and numlock is on by default. <br>
Set the useToggle member in KBManager to false to disable this.<br>
If useToggle is false they will work like any other key and only be on while being held.<br><br>

Call multiKB_Update() to check for key state changes.<br><br>

### Helper functions
multiKB_Key() gets the current key state<br>
multiKB_KeyLast() gets the last key state<br>
multiKB_KeyPress() returns true if the current state is on and the last state was off<br>
multiKB_KeyRelease() returns true if the current state is off and the last state was on<br><br>
To read the key state without these:<br>
0th bit - Raw State<br>
1st bit - Current State<br>
2nd bit - Last State<br><br>

Call multiKB_Setup() to release allocated memory when you are done with the program.<br>
