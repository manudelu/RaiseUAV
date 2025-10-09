# Unreal Engine Digital Twin Environment  

This Unreal Engine project serves as the **Digital Twin visualization layer** for both simulated (SITL) and real (HITL) drone operations. It connects to the **Control Room** implemented in this [repository](https://github.com/manudelu/ws_uspace_control_room.git), receives live MQTT telemetry via WebSocket, and renders fleet activity over Cesium 3D geospatial terrain using AirSim physics. And also uses Gaussian Splatting to enhance the rendering capabilities of the operational area in which the drone is moving.

---

## Required Plugins

| Plugin | Purpose | Installation Status | 
|--------|--------|--------|
| **[Cosys-AirSim](https://cosys-lab.github.io/Cosys-AirSim/)** | Core simulation backend | Already included in this repository |
| **[Cesium for Unreal](https://www.fab.com/listings/76c295fe-0dc6-4fd6-8319-e9833be427cd)** | Streaming real-world 3D terrain | Install manually via Fab Marketplace |
| **[XScene-UE (Gaussian Splatting)](https://github.com/xverse-engine/XScene-UEPlugin)** | Enables 3D Gaussian Splatting rendering for high-fidelity environment scenes | Requires manual install |

> After installing Cesium, enable it under *Edit → Plugins*, then restart Unreal.

---

## Content Structure Overview

| Folder | Purpose/Functionality |
|--------|------------|
| `Drone/DroneModel/` | Contains DJI S900 model, propellers, and `BP_MyPawn` - a modified version of AirSim's BP_FlyingPawn, including a cinematic third-person camera and drone mesh override |
| `Drone/LandingPad/` | Blueprint that spawns a temporary collision pad under each drone at startup. This ensures drones do not fall through Cesium tiles, which load without collision initially |
| `Drone/BP_SpectatorCamera` | Free-roam spectator camera that can be toggled using the `P` key. Allows the operator to detach from drones and explore the scene |
| `Drone/TelemetryWidget` | HUD UI widget that displays live telemetry (position, velocity, etc.) for the currently selected drone. Telemetry updates are driven through MQTT → WebSocket → Unreal |
| `Drone/BP_MqttTelemetry` | WebSocket listener blueprint that receives telemetry packets from the Control Room and triggers UI updates. Uses an event dispatcher bound in the Level Blueprint to update the widget state |
| `Sensor/BP_SensorDataReceiver` | Listens for sensor-level MQTT messages (e.g., temperature, gas detection) via WebSocket. Updates 3D floating labels (WB_SensorLabel) and spawns visual anomaly markers (e.g., smoke, fire) when thresholds are exceeded |

---

## Level Blueprint Logic Summary

The Level Blueprint orchestrates runtime logic:
* Automatically spawns Landing Pads at drone spawn positions.
* Binds event dispatchers to update the Telemetry Widget per active drone.
* Handles camera switching between:
  * Drone follow camera (`Enter` key — cycles between drones)
  * Free Spectator Camera (`P`)
* Toggles visibility of telemetry UI depending on which drone is currently selected.
---

## C++ Network Layer (WebSocket Receivers)

| Class | Role |
|-------|------|
| `BaseWebSocketReceiver` | Base implementation that manages WebSocket connection, reconnection, and disposal, with retry logic | 
| `MqttTelemetryReceiver` | Connects to Control Room WebSocket  and forwards drone telemetry to BP_MqttTelemetry |
| `SensorDataReceiver` | Receives raw sensor packets and forwards them to BP_SensorDataReceiver → triggers 3D scene annotations and widget updates |

---

