import beacons from "~/shared/beacons";

// Typical RSSI signal strength ranges for Bluetooth beacons:
// -40 dBm: very close (~0.5 meters)
// -60 dBm: close (~2 meters)
// -70 dBm: medium (~5-10 meters)
// -80 dBm: far (~15-30 meters)
// -90 dBm: very far (>30 meters)

export default function(readings: WebsocketData[]){
  if(readings.length != 3)return null;
  // Get the xy locations of the device reading based on the distance to the beacons in rssi
  const coordinates: { x: number; y: number } = { x: 0, y: 0 };
  const distances: number[] = [];

  readings.forEach(reading => {
    const beacon = beacons.find(b => b.hwid === reading.hwid);
    if (beacon) {
      const distance = Math.pow(10, (27.55 - (20 * Math.log10(reading.rssi)) / 20));
      distances.push(distance);
      coordinates.x += beacon.x / distance;
      coordinates.y += beacon.y / distance;
    }
  });
  return coordinates;
}