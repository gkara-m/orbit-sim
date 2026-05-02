import fs from "fs";
import { createRequire } from "module";
import r from "raylib";

const confPath: string = "data/config.json";
const orbitSimPath: string = "../build/Release/orbit_sim.node";
const bgColour: r.Color = {r: 30, g: 30, b: 30, a: 255};

interface Physics {
  g: number;
  dt: number;
}

interface Settings {
  steps: number;
  gui: number; // 0 = CLI, 1 = GUI
}

interface Body {
  id: number;
  mass: number;
  position: number[];
  velocity: number[];
  acceleration: number[];
}

interface Config {
  physics: Physics;
  bodies: Body[];
  settings: Settings;
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

  r.CloseWindow();
}

const jsonString = fs.readFileSync(confPath, "utf8");
const data: Config = JSON.parse(jsonString);

const require = createRequire(import.meta.url)
const orbitSim = require(orbitSimPath);
orbitSim.start(data);

if (data.settings.gui == 1) {
  runRaylibGUI();
}
