import beacons from "~/shared/beacons";

// Typical RSSI signal strength ranges for Bluetooth beacons:
// -40 dBm: very close (~0.5 meters)
// -60 dBm: close (~2 meters)
// -70 dBm: medium (~5-10 meters)
// -80 dBm: far (~15-30 meters)   
// -90 dBm: very far (>30 meters)
//RSSI distance formula: Distance = 10^((TxPower - RSSI)/(10n))*

export default function(readings: WebsocketData[]){
  if(readings.length != 3)return null;
  // Get the xy locations of the device reading based on the distance to the beacons in rssi
  const coordinates: Coordinates = { x: 0, y: 0 };
  const distances = readings.map(reading => {
    const beacon = beacons.find(b => b.hwid === reading.hwid);
    if (!beacon) return null;

    // Convert RSSI to distance using a simplified model
    // Assuming TxPower is -59 dBm for the beacons
    // txPower is the estimated RSSI value at 1 meter distance
    const txPower = -10;
    const n = 4; // Path-loss exponent, typically between 2 and 4
    const distance = Math.pow(10, (txPower - reading.rssi) / (10 * n));
    
    return { beacon, distance };
  });

  if (distances.some(d => d === null)) return null;

  // Extract beacon positions and distances
  const [a, b, c] = distances as { beacon: {x: number, y: number}, distance: number }[];
  // x,y and radius of the circles
  const x1 = a.beacon.x, y1 = a.beacon.y, r1 = a.distance;
  const x2 = b.beacon.x, y2 = b.beacon.y, r2 = b.distance;
  const x3 = c.beacon.x, y3 = c.beacon.y, r3 = c.distance;

  // Calculate coefficients for the trilateration linear equations
  // These equations are derived from the intersection of three circles (beacons)
  // Each circle: (x - xi)^2 + (y - yi)^2 = ri^2
  // After subtracting pairs and simplifying, we get two linear equations:
  const A = 2 * (x2 - x1); // Difference in x between beacon 2 and 1, scaled
  const B = 2 * (y2 - y1); // Difference in y between beacon 2 and 1, scaled
  const C = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2; // Constant term for first equation
  const D = 2 * (x3 - x2); // Difference in x between beacon 3 and 2, scaled
  const E = 2 * (y3 - y2); // Difference in y between beacon 3 and 2, scaled
  const F = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3; // Constant term for second equation
  
  // Calculate the denominator for solving the linear system (should not be zero to avoid division by zero)
  const denominator = (A * E - B * D);
  if (denominator === 0) return null; // If denominator is zero, the beacons are collinear or too close, so no unique solution

  // Solve for x and y using Cramer's rule for linear equations
  const rawX = (C * E - F * B) / denominator;
  const rawY = (A * F - D * C) / denominator;

  // Scale the coordinates
  console.log("[ws] raw coordinates", { rawX, rawY });
  coordinates.x = (rawX - 299.5) * 600;
  coordinates.y = (rawY - 199.5) * 400;

  return coordinates;
}