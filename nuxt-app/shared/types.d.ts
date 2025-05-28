
declare interface WebsocketData {
  macAddress: string;
  channel: number;
  rssi: number;
  rssi_type: string;
  sig_len: number;
  timestamp: number;
  time: string;
  hwid: string;
}

declare interface Device {
  id: string;
  x: number;
  y: number;
  timestamp: number;
}

declare interface Beacon {
  hwid: string;
  x: number;
  y: number;
}

