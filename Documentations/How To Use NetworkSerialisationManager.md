# NetworkSerialisationManager Guide
NetworkSerialisationManager contains all the information deserialised from network's received data. This document will contain the data for client and server. To use the functionalities, you must include the header file for this manager.

To access the data below, add a prefix of `NetworkSerialisationManager::GetInstance()->` to the names below. All of them are public names. For example, `NetworkSerialisationManager::GetInstance()->mPlayerID`.

<br>

---

<br>

## Data From Client To Server (C2S)

### mPlayAgainCount
if mPlayAgainCount = -1, one of the player doesn't want to play again.
if 0, no player replied yet.
if 1, one of the player want to play again.
if 2, all the players wants to play again.

### Player Inputs
There are 4 movement keys for each player and there are 2 players, so total of 8 variables. <br>
mP1InputW, mP1InputA, mP1InputS, mP1InputD for player 1. <br>
mP2InputW, mP2InputA, mP2InputS, mP2InputD for player 2. <br>
These variables will be available on **client side code** for the purpose of interpolation. It will be on server side for logic calculation.

<br>

---

<br>

## Data From Server To Client (S2C)

### mNumberOfClientConnected
Numbers of client connected to the server

### mPlayerID
Player's ID. It's determined by number of client connected. 













