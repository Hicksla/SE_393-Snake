var udp = require('dgram');
const serverPort = 8005;
const MAX_CLIENTS = 12;

// stores active client 'connections'
var clients = [];

const server = udp.createSocket('udp4');

server.on('error', function() {
    console.log(`error`);
    removeDeadSockets();
    server.close();
});


// server receives data
server.on('message', function(buf, rinfo) {
    // if this is a new client connection
    if (!clients.some(client => client.includes(rinfo.address)&&client.includes(rinfo.port))) {
        // get a unique id for the client
        getRandomSocketId(MAX_CLIENTS, function(id) {
            console.log(`Client connected: ${rinfo.address}:${rinfo.port} - ${id}`);

            // add client to the record
            clients.push([rinfo.address, rinfo.port, id, Date.now()]);

            // send them their id
            const msgBuffer = Buffer.from(`${id}$`);
            server.send(msgBuffer,rinfo.port, rinfo.address);
        });

    }
    else { // client has been assigned an id
        // deal with their message
        parseMessage(buf, rinfo.address,rinfo.port);
    }
});

// on server creation
server.on('listening', function() {
    console.log(`Server on port: ${serverPort}`);
});

// create the server
server.bind(serverPort);

// function to manipulate the client message
// manipulation avoided, so most of the logic could be in Qt
function parseMessage(message, address, port) {
    // send the message to all the other connected clients
    broadcastMessage(`${message}`, address, port);
}

// function to get a unique identifier for a client
function getRandomSocketId(max, callback) {
    // should check length of sockets (number of connections)

    while (true) { // somewhat dangerous, but with multithreading it's not too bad
        // generate random id
        var id = Math.floor(Math.random() * max);
        // if the id isn't taken already
        if (!clients.some(client => client.includes(id))) {
            // break out of the loop
            break;
        }
    }
    // return the unique id
    return callback(id);
}

// function to clear up the client list
function removeDeadSockets() {
    var timeout = Date.now() - 30 * 1000; // timeout of 30 seconds

    clients.forEach(function(client, index) {
        // if the client hasn't sent a message within the timeout
        if (client[3] < timeout) {

            // remove the client
            clients.splice(index, 1);
            console.log(`Client disconnected: ${client[0]}:${client[1]} - ${client[2]}`);
            //                                   address  :    port     -      id
        }
    });
}

// function to send a message to all clients, except the originator
function broadcastMessage(message, address, port) {
    // for every client
    clients.forEach(function(client) {
        // if the client isn't the sender
        if (client[0] != address || client[1] != port) {
            // send the message
            const msgBuffer = Buffer.from(`${message}`);
            server.send(msgBuffer, client[1], client[0]);
        }
        else { // if the client sent the message
            // update their time since last contact
            client[3] = Date.now();
        }
    });
}

// every X seconds
setInterval(function() {
    // disconnect the inactive clients
    removeDeadSockets();
}, 5000); // every 5 seconds