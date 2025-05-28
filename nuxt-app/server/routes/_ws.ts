import { ref } from "vue";

let numberOfRequests = 0;
const num = ref(0);
const wsReadings = "readings";


const readings: Record<string, WebsocketData[]> = {}

export default defineWebSocketHandler({
  open(peer) {
    // console.log("[ws] open", peer);
  },

  message(peer, message) {

    // Handle message type - esp info or esp reading
    
    // save data until there is messages all esp's with the same id / mac in a reasonable timeframe 
    
    // use Trilateration to get the ca geolocation of the device based on esp geolocation
    // Transform the data to x, y and id

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

