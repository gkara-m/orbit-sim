import fs from "fs";
import { createRequire } from "module";
import r from "raylib";

const confPath: string = "data/config.json";
const orbitSimPath: string = "../build/Release/orbit_sim.node";
const bgColour: r.Color = {r: 30, g: 30, b: 30, a: 255};
const mauve: r.Color = {r: 203, g: 166, b: 247, a:255};
const textColour: r.Color = {r: 205, g: 214, b: 244, a: 255};

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
  position: [number, number];
  velocity: [number, number];
  acceleration: [number, number];
}

interface Config {
  physics: Physics;
  bodies: Body[];
  settings: Settings;
}

function raylibInit(worldSize: [number, number]) {
  r.SetConfigFlags(r.FLAG_WINDOW_RESIZABLE);
  r.InitWindow(800, 600, "Orbit Sim");
  r.ToggleFullscreen();
  r.SetTargetFPS(60);

  const zoomX = r.GetScreenWidth() / worldSize[0];
  const zoomY = r.GetScreenHeight() / worldSize[1];
  const finalZoom = Math.min(zoomX, zoomY);

  const camera: r.Camera2D = {
    offset: { x: r.GetScreenWidth() / 2, y: r.GetScreenHeight() / 2 },
    target: { x: 0, y: 0 },
    rotation: 0,
    zoom: finalZoom
  }

  return camera;
}

function runRaylibGUI(worldSize: [number, number]) {
  const camera = raylibInit(worldSize);
  const numBodies = data.bodies.length;
  while (r.WindowShouldClose() == false) {
    r.BeginDrawing();
    r.ClearBackground(bgColour);
    
    r.BeginMode2D(camera);
    for (let i = 0; i < numBodies; i++) {
      r.DrawCircleV({x: views.positions[2*i], y: views.positions[2*i+1]}, 5000000, mauve);
    };
    r.EndMode2D();

    r.EndDrawing();
  };

  r.CloseWindow();
}

function getWorldSize(gui: number) {
  const size: [number, number] = [0,0];
  if (gui == 0) return size;
  for (const body of data.bodies) {
    if (Math.abs(body.position[0]) > size[0]) {
      size[0] = Math.abs(body.position[0]) * 2.1
    };
    if (Math.abs(body.position[1]) > size[1]) {
      size[1] = Math.abs(body.position[1]) * 2.1
    };
  };
  return size;
}

const jsonString = fs.readFileSync(confPath, "utf8");
const data: Config = JSON.parse(jsonString);

const worldSize = getWorldSize(data.settings.gui)

const require = createRequire(import.meta.url)
const orbitSim = require(orbitSimPath);
orbitSim.start(data);
// Available views: g, dt, positions, velocities, accelerations
const views = orbitSim.getBuffer();

if (data.settings.gui == 1) {
  runRaylibGUI(worldSize);
};
