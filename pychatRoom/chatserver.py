import select
import socket
import sys
import signal
from communication import send, receive

BUFSIZ = 1024


class ChatServer(object):
    """ Simple chat server using select """

    def __init__(self, port=3490, backlog=5):
        self.clients = 0
        # Client map
        self.clientmap = {}
        # Output socket list
        self.outputs = []
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.bind(('',port))
        print 'Listening to port',port,'...'
        self.server.listen(backlog)

        # Trap keyboard interrupts
        signal.signal(signal.SIGINT, self.sighandler)

    def sighandler(self, signum, frame):
        # Close the server
        print 'Shutting down server...'
        # Close existing client sockets
        for o in self.outputs:
            o.close()
        self.server.close()

    def getname(self, client):

        # Return the printable name of the
        # client, given its socket...
        info = self.clientmap[client]
        host, name = info[0][0], info[1]
        return '@'.join((name, host))

    def serve(self):
        inputs = [self.server,sys.stdin]
        self.outputs = []
        running = 1
        while running: 
            try:
                inputready,outputready,exceptready = select.select(inputs, self.outputs, [])
            except select.error, e:
                break
            except socket.error, e:
                break

            for s in inputready:

                if s == self.server:
                    # handle the server socket
                    client, address = self.server.accept()
                    print 'chatserver: got connection %d from %s' % (client.fileno(), address)
                    # Read the login name
                    cname = receive(client).split('NAME: ')[1]
                    # Compute client name and send back
                    self.clients += 1
                    send(client, 'CLIENT: ' + str(address[0]))
                    inputs.append(client)
                    self.clientmap[client] = (address, cname)
                    # Send joining information to other clients
                    msg = '\n(Connected: New client (%d) from %s)' % (self.clients, self.getname(client))
                    for o in self.outputs:
                        # o.send(msg)
                        send(o, msg)
                    self.outputs.append(client)

                elif s == sys.stdin:
                    # handle standard input
                    junk = sys.stdin.readline()
                    running = 0
                else:
                    # handle all other sockets
                    try:
                        # data = s.recv(BUFSIZ)
                        data = receive(s)
                        if data:
                            # Send as new client's message...
                            msg = '\n#[' + self.getname(s) + ']>> ' + data
                            # Send data to all except ourselves
                            for o in self.outputs:
                                if o != s:
                                    # o.send(msg)
                                    send(o, msg)
                        else:
                            print 'chatserver: %d hung up' % s.fileno()
                            self.clients -= 1
                            s.close()
                            inputs.remove(s)
                            self.outputs.remove(s) 
                            # Send client leaving information to others
                            msg = '\n(Hung up: Client from %s)' % self.getname(s)
                            for o in self.outputs:
                                # o.send(msg)
                                send(o, msg)
                    except socket.error, e:
                        # Remove
                        inputs.remove(s)
                        self.outputs.remove(s)

        self.server.close()

if __name__ == "__main__":
    ChatServer().serve()

