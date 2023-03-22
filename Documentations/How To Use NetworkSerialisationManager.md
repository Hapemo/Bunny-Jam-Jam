# NetworkSerialisationManager Guide
NetworkSerialisationManager contains all the information deserialised from network's received data. This document will contain the data for client and server. To use the functionalities, you must include the header file for this manager.


## Data From Server To Client (S2C)
To access the data below, add a prefix of `NetworkSerialisationManager::GetInstance()->` to the names below. All of them are public names. For example, `NetworkSerialisationManager::GetInstance()->mPlayerID`.

### mNumberOfClientConnected
Numbers of client connected to the server

### mPlayerID
Player's ID. It's determined by number of client connected. 













