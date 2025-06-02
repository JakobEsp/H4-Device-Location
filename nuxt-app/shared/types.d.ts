
declare type Hwid = "64071ea7dbcc" | "00555bf6b870" | "8cf585c964ec";

declare interface WebsocketData {
  macAddress: number;
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
