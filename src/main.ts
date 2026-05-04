console.log("started orbit_sim")

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
  useSteps: boolean;
  steps: number;
  gui: number; // 0 = CLI (TODO), 1 = GUI 
  integrator: number; // 0 = Velocity Verlet, 1... to be added?
  algorithm: number; // 0 = Brute Force O(n^2), 1 = Barnes Hut O(nlogn) (TODO)
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

function raylibInit() {
  r.SetConfigFlags(r.FLAG_WINDOW_RESIZABLE);
  r.InitWindow(800, 600, "Orbit Sim");
  r.ToggleFullscreen();
  r.SetTargetFPS(60);

  let worldSize = getWorldSize();

  const zoomX = r.GetScreenWidth() / worldSize[0];
  const zoomY = r.GetScreenHeight() / worldSize[1];
  const finalZoom = Math.min(zoomX, zoomY);

  const camera: r.Camera2D = {
    offset: { x: r.GetScreenWidth(), y: r.GetScreenHeight()},
    target: { x: 0, y: 0 },
    rotation: 0,
    zoom: finalZoom
  }

  return camera;
}

function runRaylibGUI() {
  let camera = raylibInit();
  const numBodies = data.bodies.length;
  while (r.WindowShouldClose() == false) {
    camera.target.x = views.dimensions[2];
    camera.target.y = views.dimensions[3];
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

function getWorldSize() {
  const size: [number, number] = [views.dimensions[0] * 1.1, views.dimensions[0] * 1.1];
  return size;
}

const jsonString = fs.readFileSync(confPath, "utf8");
const data: Config = JSON.parse(jsonString);
console.log("parsed json")

const require = createRequire(import.meta.url)
const orbitSim = require(orbitSimPath);

console.log("starting...")
orbitSim.start(data);
// Available views: g, dt, positions, velocities, accelerations, dimensions
const views = orbitSim.getBuffer();
console.log("simulation started")

if (data.settings.gui == 1) {
  runRaylibGUI();
};

console.log("shutting down sim");
