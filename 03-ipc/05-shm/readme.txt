To run this example:

1 - Run make to compile everything.
2 - Start worker in a terminal.
3 - Start reader in another terminal.
4 - Start writer in yet another terminal.
5 - Type messages in writer, and observe the output from reader.

Worker creates and initializes a shared memory region which contains:
- A message buffer.
- A mutex for safe access to the messsage buffer.
- 3 semaphores to indicate what the buffer contains.

Writer does:
- Reads a message.
- Waits for an empty buffer
- Take the buffer mutex
- Copies the message onto the buffer
- Releases the mutex
- Signals that the buffer now contains a (potentially) lowercase message.
- Repeat.

Worker does:
- Wait for the buffer to contain a lowercase message.
- Grabs the mutex for the buffer.
- Converts the message to uppercase.
- Releases the mutex.
- Signals that the buffer now contains an uppercase message.
- Repeat.

Reader does:
- Wait for the buffer to contain an uppercase message.
- Grab the mutex to the buffer.
- Print the message.
- Releases the mutex to the buffer.
- Signal that the buffer is now empty.
- Repeat.
