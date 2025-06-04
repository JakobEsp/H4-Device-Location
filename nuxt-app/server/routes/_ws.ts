import { ref } from "vue";
import checkReadings from "../utils/checkReadings";
import calculateXY from "../utils/calculateXY";

const subscribtion = "readings"

let numberOfRequests = 0;
const num = ref(0);

const DeviceIds: Record<string, string> = {
}

const devices: Device[] = [] 

const readings: Readings = {
  "64071ea7dbcc": [],
  "00555bf6b870": [],
  "8cf585c964ec": []
}

export default defineWebSocketHandler({

  open(peer) {
    console.log("[ws] open");
    peer.subscribe(peer.websocket.url ?? subscribtion);
  },

  message(peer, message) {
    console.log("[ws] message", message.text());
    // Handle message type - esp info or esp reading
    const reading: WebsocketData = JSON.parse(message.text());

    //TODO: add a time check to remove readings that are too old
    //if reading[hwid] already has a reading with reading.mac Address, remove the old reading
    if (readings[reading.hwid].some(r => r.macAddress === reading.macAddress)) {
      readings[reading.hwid] = readings[reading.hwid].filter(r => r.macAddress !== reading.macAddress);
    }
    // save data until there is messages all esp's with the same macAddress
    readings[reading.hwid].push(reading);

    const validReadings = checkReadings(readings, reading.macAddress);
    if(!validReadings) return;
    removeReadings(reading.macAddress);
    // use Trilateration to get the ca x,y of the device based on esp x,y and distance
    const coordinates = calculateXY(validReadings);
    if(!coordinates) return;
    // Transform the data to type Device
      try {
        const id = DeviceIds[reading.macAddress] ?? Object.keys(DeviceIds).length + 1;
        const device: Device = {
          id: id.toString(),
          x: coordinates.x,
          y: coordinates.y,
          timestamp: Date.now(),
        }
        
        devices.push(device);
        peer.publish(peer.websocket.url ?? subscribtion, JSON.stringify(device));
        peer.send(JSON.stringify(device));
      }catch(error){
        console.error("[ws] error publishing device", error);
      }
  },

  close(peer, event) {
    console.log("[ws] close", peer, event);
  },

  error(peer, error) {
    console.log("[ws] error", peer, error);
  },
});

function removeReadings(macAddress: WebsocketData["macAddress"]) {
  Object.keys(readings).forEach(hwid => {
    readings[hwid as Hwid] = readings[hwid as Hwid].filter(r => r.macAddress !== macAddress);
  });
}

// implement
function removeOldReadings() {
  const now = Date.now();
  Object.keys(readings).forEach(hwid => {
    readings[hwid as Hwid] = readings[hwid as Hwid].filter(r => now - r.timestamp < 60000); // Keep readings for 1 minute
  });
}