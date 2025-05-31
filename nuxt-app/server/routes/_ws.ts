import { ref } from "vue";
import checkReadings from "../utils/checkReadings";
import calculateXY from "../utils/calculateXY";

let numberOfRequests = 0;
const num = ref(0);
const wsReadings = "readings";

const DeviceIds: Record<string, string> = {
}

const devices: Device[] = [] 

const readings: Readings = {
  beacon1: [],
  beacon2: [],
  beacon3: []
}

export default defineWebSocketHandler({
  open(peer) {
    // console.log("[ws] open", peer);
  },

  message(peer, message) {

    // Handle message type - esp info or esp reading
    const reading: WebsocketData = JSON.parse(message.text());
    
    //TODO: add a time check to remove readings that are too old
    //if reading[hwid] already has a reading with reading.mac Address, remove the old reading
    if (readings[reading.hwid].some(r => r.macAddress === reading.macAddress)) {
      readings[reading.hwid] = readings[reading.hwid].filter(r => r.macAddress !== reading.macAddress);
    }
    // save data until there is messages all esp's with the same id / mac in a reasonable timeframe 
    readings[reading.hwid].push(reading);
    console.log("[ws] readings", readings);
    const validReadings = checkReadings(readings, reading.macAddress);
    if(!validReadings) return;
    console.log("[ws] validReadings", validReadings);
    // use Trilateration to get the ca x,y of the device based on esp x,y and distance
    const coordinates = calculateXY(validReadings);
    if(!coordinates) return;
    console.log("[ws] coordinates", coordinates);

    // Transform the data to x, y and id

    const id = Object.keys(DeviceIds).length + 1;



    console.log("[ws] message", message);
    num.value++;
    console.log("[ws] num.value", num.value); 
    numberOfRequests++;
    console.log("[ws] numberOfRequests", numberOfRequests);

    // const data: WebsocketData = JSON.parse(message.text());

    // if (!readings[data.hwid]) {
    //   readings[data.hwid] = [];
    // }else {
    //   console.log("No hardware id get good")
    // }
    // peer.publish(wsReadings, num.value);
  },

  close(peer, event) {
    console.log("[ws] close", peer, event);
  },

  error(peer, error) {
    console.log("[ws] error", peer, error);
  },
});

