
declare type Hwid = 'beacon1' | 'beacon2' | 'beacon3';

declare interface WebsocketData {
  macAddress: string;
  channel: number;
  rssi: number;
  rssi_type: string;
  sig_len: number;
  timestamp: number;
  time: string;
  hwid: Hwid;
}

declare interface Device {
  id: string;
  x: number;
  y: number;
  timestamp: number;
}

declare interface Beacon {
  hwid: Hwid;
  x: number;
  y: number;
}

declare type Readings = Record<Hwid, WebsocketData[]>;
