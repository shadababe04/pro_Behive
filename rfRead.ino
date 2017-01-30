 if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      uint8_t len;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
	len = radio.getDynamicPayloadSize();
	radio.read( receive_payload, len );
          if (len >0)
            done = 1;

	// Put a zero at the end for easy printing
	receive_payload[len] = 0;

	// Spew it
	printf("Got payload size=%i value=%s\n\r",len,receive_payload);
      }

      // First, stop listening so we can talk
      radio.stopListening();
      //delay(1000);
      
      for (j = 0; j < 9; j++)
	    nodeAddr[j] = receive_payload[j]; //node_address_s.node_status[node_index];
	nodeAddr[9] = '\0';
      printf("nodeAddr ::%s\n\r",nodeAddr);
	
	if ((strncmp(nodeAddr, "BBA619550",9) == 0)) {
          for (j = 0; j < 11; j++)
		nodeStatus[j] = receive_payload[j + 9]; //node_address_s.node_status[node_index];
	nodeStatus[j] = '\0';