

export default function(readings: Readings, macAddress: string) {

  // check if every key on readings has at least one reading with the same macAddress
  return Object.values(readings).every((readingArray) => {
    return readingArray.some(reading => reading.macAddress === macAddress);
  });
}
