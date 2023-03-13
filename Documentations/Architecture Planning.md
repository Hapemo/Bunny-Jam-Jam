# Bunny Jam Jam Architecture Planning
The Original VI Engine architecture does not support Networking feature, so here is a breakdown of each architecture in attempt to merge them together
* [VI Engine Architecture](#vi-engine-architecture)
* [Networking Architecture](#networking-architecture)
* [VI Engine with Networking](#vi-engine-with-networking)
<br>

---
<br>

## VI Engine Architecture
* [<span style="color:yellow">App Init</span>](#app-init)
* [<span style="color:yellow">App Update Loop</span>](#app-update-loop)
    * [<span style="color:yellow">App First Update</span>](#app-first-update)
    * [<span style="color:yellow">App Main Update</span>](#app-main-update)
    * [<span style="color:yellow">App Second Update</span>](#app-second-update)
* [<span style="color:yellow">App Exit](#app-exit)



<br>

---
<br>


### App Init
Initialise all essential systems to run the applicaion. Some of the systems are as listed below:
* Config
* GLEW
* GLFW
* Input Manager
* Helper Manager
* ECS Manager
* GameState Manager
* <span style="color:yellow"> Network Manager </span>
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

### App Update Loop
An infinite looping while loop with condition to quit when glfw sense that it should close the window (This condition is determined by GameStateManager on runtime).
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

### App First Update
Things to update first in a game loop should be here. Things such as:
* GLFW poll events
* Reset fps calculation
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

### App Main Update
The standard update loop that updates all gamelogic and system. Things such as:
* Editor system
* All ECS system (Button, Logic, AI, Physics, Collision, Particle, Animation)
* Render Manager
* Audio Manager
* Update GameStateManager
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

### App Second Update
Things that update last in the game loop should be here. Things such as:
* Printing titlebar
* Checking gamestate exit
* Reset Input
* Calculate dt
* Limit fps
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

### App Exit
Things to unload when the application is going to quit. Things such as:
* GameStateManager
* Editor
* All ECS Systems
* ResourceManager
* Singleton
* GLFW
<br>[Back to VI Engine Architecture](#vi-engine-architecture)

<br>

---

<br>


## Networking Architecture
Network architecture for Client and Host is different, thus they will be separated in this instance.
<br>

## Host Network Architecture
* [<span style="color:cyan">Socket Init</span>](#socket-init)
* [<span style="color:cyan">Bind Socket</span>](#bind-socket)
* [<span style="color:cyan">Activate Listen Mode</span>](#activate-listen-mode)
* [<span style="color:cyan">Find Connection Thread</span>](#find-connection-thread)
* Infinite while loop
    * [<span style="color:cyan">Open Client Thread</span>](#open-client-thread)
* [<span style="color:cyan">Socket Exit</span>](#socket-exit)

<br>

## Client Network Architecture
* [<span style="color:cyan">Socket Init</span>](#socket-init)
* [<span style="color:cyan">Connect Socket</span>](#connect-socket)
* [<span style="color:cyan">Listening Thread</span>](#listening-thread)
* Infinite while loop
    * [<span style="color:cyan">Main Client Loop</span>](#main-game-loop)
* [<span style="color:cyan">Client Unloading</span>](#client-unloading)

<br>

---
<br>

### Socket Init
Initialise `SOCKET`, contain operations such as:
* Initalise WinSock2
* Set up <span style="color:yellow">UDP Socket</span>
* Error checking for these 2 set ups
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

### Bind Socket
Binding Socket to specified server IP address and port number. The operation includes:
* Creation of `struct sockaddr_in`
* Binding `SOCKET` to the `struct sockaddr_in` created using WinSock2's `bind`
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

### Activate Listen Mode
Activate Host's listen mode using WinSock2's `listen` on the socket.
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

### Find Connection Thread
Create a thread with infinite while loop for this is **blocking call** function. It will continuously look out for client's connection using WinSock2's `accept` and connect. When `accept` got signal to continue, it will break out of the blocking call.
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

### Open Client Thread
Create a new thread for the client with **an unique client info** and run a **blocking call** for the client. It is a blocking call because the function will contain a while loop with another **blocking call** inside, WinSock2's `recv`. <br>
The infinite while loop should contain but not limited to functionalities such as:
* WinSock's `recv` - **Blocking call** to receive a signal from the client
* Push back the signal to host's queue of running game logic
* Check if client quits
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

### Socket Exit
Exit the socket functions and join all the client threads. It should contain but not limited to functionalities such as:
* Closing host socket with WinSock's `closesocket`
* Ending WinSock's socket with `WSACleaup`
* Joining all the client's threads (Not too sure if we want to wait for all clients to close before host closes)
<br>[Back to Host Network Architecture](#host-network-architecture)

<br>

---

### Connect Socket
Connects the socket to a server, the host. Procedure is similar to [Bind Socket](#bind-socket), except it uses WinSock2's `connect` instead of `bind`.
<br>[Back to Client Network Architecture](#client-network-architecture)

<br>

### Main Game Loop
The main game loop runs here. Since some important components are calculated on host side, such as physics, game logic and scripts, this client game loop should not run them. Some things client game loop should run are:
* Graphics rendering
* Particle generation
* Input system
* Networking system to send data
<br>[Back to Client Network Architecture](#client-network-architecture)


<br>

### Listening Thread
The client should **launch a thread** that **asynchronously** listen for response from server. Once it received data, it should update all it's systems with those data.<br>
If no data is received, one method of error prevention is to <span style="color:yellow">interpolate the current data</span> for predicted future data. 
<br>[Back to Client Network Architecture](#client-network-architecture)

<br>

### Client Unloading
When the client wants to quit the application, they should unload all the systems they have for the systems they had activated, such as:
- Unloading textures
- Unloading inputs
- Unloading ECS
<br>[Back to Client Network Architecture](#client-network-architecture)

<br>

---
<br>

## VI Engine with Networking
With the individual architectures sorted out in [VI Engine Architecture](#vi-engine-architecture) and [Networking Architecture](#networking-architecture), here is their combined effort.<br>
The functionalities in both client and host's update loop will be lesser. Eg, host don't need to render or generate particles, clients don't need to calculate collision and run game logics.


<br>

### For Host
* [<span style="color:yellow">App Init</span>](#app-init) - [<span style="color:cyan">Socket Init</span>](#socket-init), [<span style="color:cyan">Bind Socket</span>](#bind-socket), [<span style="color:cyan">Activate Listen Mode</span>](#activate-listen-mode)
* New threads
    * [<span style="color:cyan">Find Connection Thread</span>](#find-connection-thread)
    * [<span style="color:cyan">Open Client Thread</span>](#open-client-thread)
* [<span style="color:yellow">App Update Loop</span>](#app-update-loop)
    * [<span style="color:yellow">App First Update</span>](#app-first-update)
    * [<span style="color:yellow">App Main Update</span>](#app-main-update)
    * [<span style="color:yellow">App Second Update</span>](#app-second-update)
* [<span style="color:yellow">App Exit](#app-exit) - [<span style="color:cyan">Socket Exit</span>](#socket-exit)

<br>

### For Client
* [<span style="color:yellow">App Init</span>](#app-init) - [<span style="color:cyan">Socket Init</span>](#socket-init), [<span style="color:cyan">Connect Socket</span>](#connect-socket)
* New threads
    * [<span style="color:cyan">Listening Thread</span>](#listening-thread)
* [<span style="color:yellow">App Update Loop</span>](#app-update-loop)
    * [<span style="color:yellow">App First Update</span>](#app-first-update)
    * [<span style="color:yellow">App Main Update</span>](#app-main-update)
    * [<span style="color:yellow">App Second Update</span>](#app-second-update)
* [<span style="color:yellow">App Exit](#app-exit) - [<span style="color:cyan">Client Unloading</span>](#client-unloading)


