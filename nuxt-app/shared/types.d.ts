
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

declare interface Device extends Coordinates{
  id: string;
  timestamp: number;
}

declare interface Beacon extends Coordinates {
  hwid: Hwid;
}

declare interface Coordinates {
  x: number;
  y: number;
}

declare type Readings = Record<Hwid, WebsocketData[]>;
