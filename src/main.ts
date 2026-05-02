import fs from "fs";
import { createRequire } from "module";
const require = createRequire(import.meta.url)

const filePath: string = "data/config.json";

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

const orbit_sim = require("../build/Release/orbit_sim.node");
orbit_sim.start(data);
