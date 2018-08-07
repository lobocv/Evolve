import sys
import zmq
import json

port = "12346"
    
# Socket to talk to server
context = zmq.Context()
socket = context.socket(zmq.SUB)

print ("Collecting updates from weather server...")
socket.connect ("tcp://0.0.0.0:%s" % port)

# Subscribe to zipcode, default is NYC, 10001
topicfilter = b""
socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

# Process 5 updates
total_value = 0
while 1:
    string = socket.recv()
    #topic, *messagedata = string.split()
    #messagedata = b''.join(messagedata).decode()
    messagedata = string.decode()
    try:
        messagedata = json.loads(messagedata)
    except Exception as e:
        print(e)
        #continue	
    print(messagedata)

print ("Average messagedata value for topic '%s' was %dF" % (topicfilter, total_value / update_nbr))
