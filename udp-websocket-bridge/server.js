const WebSocket =  require('ws');
const dgram = require('dgram');
const express = require('express');
const parser = require('body-parser');
const bodyParser = require('body-parser');

const udpServer = dgram.createSocket('udp4');
const UDP_PORT = 8080;

//Websocket setup
const app = express();
app.use(bodyParser.text());

const server = app.listen(8081, ()=> {
    console.log('HTTP/WebSocket server started on port 8081');
});

const wss = new WebSocket.Server({server});

wss.on("connection", ws => {
    console.log("WebSocket client connected");
    ws.on("message", message => {
        console.log("message recieved from client: ",message)
    });
});
function broadcast(data){
    wss.clients.forEach(client => {
        if(client.readyState === WebSocket.OPEN){
            client.send(data);
        }
    })
}
udpServer.on('message',(msg,rinfo)=>{
    console.log("UDP message recieved: ",msg, " from ",rinfo.address,":",rinfo.port);
    broadcast(msg.toString());
});

udpServer.bind(UDP_PORT, ()=> {
    console.log("UDP server listening on port", UDP_PORT);
});






app.post("/udpdata", (req, res) => {
    const data = req.body;
    console.log("received data via HTTP POST: ", data);
    broadcast(data);
    res.sendStatus(200); 
});