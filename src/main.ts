import fs from "fs";
import { createRequire } from "module";
import r from "raylib";

const confPath: string = "data/config.json";
const orbitSimPath: string = "../build/Release/orbit_sim.node";

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

interface UISettings {
  gui: number;
}

interface Config {
  settings: Settings;
  bodies: Body[];
  uiSettings: UISettings;
}


const jsonString = fs.readFileSync(confPath, "utf8");
const data: Config = JSON.parse(jsonString);

if (data.uiSettings.gui == 0) {
  r.InitWindow(800, 800, "Orbit Sim");
}

const require = createRequire(import.meta.url)
const orbitSim = require(orbitSimPath);
orbitSim.start(data);
