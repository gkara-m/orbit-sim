import fs from "fs";
import { createRequire } from "module";
import r from "raylib";

const confPath: string = "data/config.json";
const orbitSimPath: string = "../build/Release/orbit_sim.node";
const bgColour: r.Color = {r: 30, g: 30, b: 30, a: 255};

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
  // 0 = no gui, 1 = raylib gui
  gui: number;
}

interface Config {
  settings: Settings;
  bodies: Body[];
  uiSettings: UISettings;
}

function runRaylibGUI() {
  r.SetConfigFlags(r.FLAG_WINDOW_RESIZABLE);
  let width = r.GetScreenWidth();
  let height = r.GetScreenHeight();
  r.InitWindow(width/2, height/2, "Orbit Sim");
  r.MaximizeWindow();
  r.SetTargetFPS(60);
  while (r.WindowShouldClose() == false) {
    r.BeginDrawing();
    r.ClearBackground(bgColour);
    r.EndDrawing();
  };
}

const jsonString = fs.readFileSync(confPath, "utf8");
const data: Config = JSON.parse(jsonString);

if (data.uiSettings.gui == 1) {
  runRaylibGUI();
}

const require = createRequire(import.meta.url)
const orbitSim = require(orbitSimPath);
orbitSim.start(data);

if (data.uiSettings.gui == 1) {
  r.CloseWindow();
};
