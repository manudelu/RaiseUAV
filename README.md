# 🛰 Unreal Engine Digital Twin Environment  

This Unreal Engine project serves as the **Digital Twin visualization layer** for both **simulated fleets (SITL)** and **real drone operations (HIL)**. It connects to the **Control Room** (https://github.com/manudelu/ws_uspace_control_room.git), receives **live MQTT telemetry via WebSocket**, and renders fleet activity over **Cesium 3D geospatial terrain** using **AirSim physics**.

---

## Required Plugins

| Plugin | Status | Notes |
|--------|--------|------|
| **[Cosys-AirSim](https://cosys-lab.github.io/Cosys-AirSim/)** | Already included in this repository |
| **[Cesium for Unreal](https://www.fab.com/listings/76c295fe-0dc6-4fd6-8319-e9833be427cd)** | Install from Fab Marketplace manually |

> After installing Cesium, enable it under **Edit → Plugins**, then restart Unreal.

---

## Project Structure Overview

| Folder | Description |
|--------|------------|
| `Content/DroneModel/` | DJI S900 model, propellers, and `BP_MyPawn` (AirSim pawn variant with cinematic third-person camera and custom mesh) |
| `Content/LandingPad/` | Spawns pads under drones to prevent falling through Cesium (no collision on load) |
| `Content/BP_SpectatorCamera` | Enables free-roam camera (`P` key toggle) |
| `Content/TelemetryWidget` | UI showing **live telemetry** from MQTT/WebSocket |
| `Content/BP_MqttTelemetry` | Receives messages from Control Room WebSocket → Updates widget |
| `Content/Sensor/` | Receivers and 3D label widgets for sensor payloads |

---

## Controls & Level Blueprint Logic

| Action | Key | Description |
|--------|-----|------------|
| Toggle Spectator Camera | `P` | Activates free camera, detached from drones |
| Cycle Drones (Camera Focus) | `Enter` | Switches between UAVs and updates telemetry UI source |
| Pad Placement | Auto | Spawns landing pads under each AirSim drone on simulation start |

---

## C++ Integration (WebSocket Data Streams)

| Class | Role |
|-------|------|
| `BaseWebSocketReceiver` | Handles connect → retry → disconnect | 
| `MqttTelemetryReceiver` | Connects to WebSocket from Control Room → Broadcasts live telemetry |
| `SensorDataReceiver` | Connects to WebSocket and updates in-world 3D sensor widgets |

These classes trigger **Blueprint Event Dispatchers**, which are bound in **Level Blueprint** to update UI and visual actors.

---

