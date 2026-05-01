import fs from "fs";

const filePath: string = "/home/user/dev/c++/orbit-sim/data/config.json";

const jsonString = fs.readFileSync(filePath, "utf8");

interface Settings {
  g: number;
  dt: number;
  steps: number;
}

interface Body {
  id: number;
  mass: number;
  position: number[];
  velocity: number[];
  acceleration: number[];
}

interface Config {
  settings: Settings;
  bodies: Body[];
}

const data: Config = JSON.parse(jsonString);
