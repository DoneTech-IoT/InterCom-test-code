# PubSubManager-test-code
##### How to use:
1- include SharedBus.h in any file you want to use it.   
2- It is initialized in Sevice Manager so you do not have to do it.   
3- while using SharedBus you are sending a Packet using a Queue So:    
  - Define a SharedBusPacket_t type structure and fill it with needed data    
  - Use the SharedBusSend function to send the packet on the SharedBus    
  - Use the SharedBusRecieve function to Receive the packet    

  #### interfaceID in SharedBusRecieve is the ID of the receiver component. You can find the right ID in the SharedBus.h file.
  #### Do not forget to handle unexpected ERRORs while using send and receive functions.
  
