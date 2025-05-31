
// returns readings if there are 3  
export default function(readings: Readings, macAddress: string) {
  const matches = Object.values(readings)
    .flat()
    .filter(reading => reading.macAddress === macAddress);
    if(matches.length > 3){
      console.error("OVER 3!?! THis should not be possible get good fr fr ong")
    }
  return matches.length === 3 ? matches : null;
}
