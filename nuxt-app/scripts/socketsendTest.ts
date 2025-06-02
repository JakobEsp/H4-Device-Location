const beaconIds = ["6407Iea7dbcc", "00555bf66870", "8cf585c964ec"];

let message: WebsocketData = {
  macAddress: Math.floor(Math.random() * 1000), // Random macAddress for testing
  channel: 1,
  rssi: -70,
  rssi_type: "SCRIPT",
  sig_len: 8,
  timestamp: Date.now(),
  time: new Date().toISOString(),
  hwid: "00555bf66870"
}

const ws = new WebSocket('ws://192.168.0.102:3000/_ws');

ws.onopen = () => {
  console.log('WebSocket connection opened');
  ws.send(JSON.stringify(message));
  startSendingMessages();
};

ws.onmessage = (event) => {
  console.log('Received:', event.data);
};

ws.onerror = (error) => {
  console.error('WebSocket error:', error);
};

ws.onclose = () => {
  console.log('WebSocket connection closed');
};

function startSendingMessages() {

  while (true) {
    message.timestamp = Date.now();
    message.time = new Date().toISOString();
    // Set hwid to the hwid of a random beacon from index 1-3
    const randomRssi = Math.floor(Math.random() * 61) - 80; // Random RSSI between -80 and -20
    const randomIndex = Math.floor(Math.random() * beaconIds.length); // 1, 2, or 3
    message.hwid = beaconIds[randomIndex] as Hwid; // Cast to Hwid type
    message.rssi = randomRssi; // Set the random RSSI value
    ws.send(JSON.stringify(message));
    console.log('Sent:', message);
    // Wait for a second before sending the next message
    Atomics.wait(new Int32Array(new SharedArrayBuffer(4)), 0, 0, 1000);
  }

}