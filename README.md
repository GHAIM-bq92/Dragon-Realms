# Dragon Realms

Dragon Realms is a UE5 open-world dragon-riding prototype focused on a massive flying dragon, large fantasy world, and scalable online gameplay foundations.

This repository provides an initial Unreal Engine 5 project scaffold and a playable prototype foundation for:
- a small exploration map
- a rideable dragon pawn
- keyboard/mouse flight controls
- camera follow behavior
- local multiplayer setup plus replication notes
- a project structure designed for expansion toward online co-op, dragon breeding, combat, and world systems

## Project status

This is a starter prototype architecture and C++ gameplay foundation. It is designed to be opened in Unreal Engine 5.3 and extended with world content, models, and online services.

Important: actual full game assets, shaders, terrain, dragon models, and online services still require additional setup, licensing, and sometimes hosting.

## Included in this prototype

- UE5.3 project file
- C++ module named `DragonRealms`
- `ADragonRealmsGameModeBase` setup
- `ADragonRealmsPawn` with:
  - third-person follow camera
  - flight mode toggle
  - forward/strafe/ascend controls
  - yaw/pitch/roll handling
  - boost behavior
  - mountable rideable dragon-style gameplay loop
- Default input mappings for keyboard controls
- Basic documentation for expansion and multiplayer setup

## Controls

- `W / S`: forward/backward flight
- `A / D`: strafe left/right
- `Q / E`: descend/ascend (or lift control)
- `Left Mouse`: maybe used for camera orbiting in future implementation
- `Space`: toggle flight mode / takeoff
- `Left Shift`: boost speed
- `Mouse`: camera yaw/pitch when attached to camera logic

## Required setup

1. Install Unreal Engine 5.3.
2. Install Visual Studio 2022 with C++ Desktop development and Windows 10/11 SDK.
3. Open `DragonRealms.uproject` with UE5.3.
4. Let Unreal generate project files.
5. Build the project in Development Editor configuration.

## Recommended directory structure

- `/Source/DragonRealms` — C++ gameplay code
- `/Config` — project settings and input mappings
- `/Content` — game assets, maps, materials, and UI later
- `/Plugins` — optional plugins, if added later

## Expansion roadmap

Phase 1: prototype
- playable flight prototype
- test map with mountain, lake, floating island, ruin
- basic dragon pawn and camera
- local multiplayer replication scaffold

Phase 2: core systems
- multiple dragon species (fire, ice, storm, shadow)
- dragon eggs, breeding, rare collection
- combat attacks, breath fire, claws, magical abilities

Phase 3: world systems
- day/night cycle
- dynamic weather
- Lumen lighting and atmosphere
- more biome variation and boss encounters

Phase 4: online play
- Epic Online Services (EOS) or Steam integration
- dedicated server setup
- co-op world sharing and persistence

Phase 5: polish
- quest system
- loot, inventory, levels, experience
- UI polish, save system, optimization

## Multiplayer notes

This starter project is intended to be expanded with a dedicated server model. For public online gameplay, you will need either:
- Epic Online Services (EOS) with UE5 native integration, or
- Steamworks + custom multiplayer flow, or
- a custom authoritative server stack

For local development, you can start by testing replication on LAN and then scale to public online sessions.

## Licensing and assets

This repository includes starter code, templates, and documentation only. Any production models, textures, animations, sounds, or terrain assets must be added using licensed content or assets you have rights to use.

## Useful references

- Unreal Engine 5 documentation
- UE5 C++ API docs
- Enhanced Input plugin documentation
- Unreal networking / replication docs

## Next development steps after this prototype

- add a minimal terrain map with a mountain biome and floating island
- add dragon model with skeletal mesh and animation assets
- hook up proper attack/combat VFX
- implement online session creation, lobby, and world replication
- add quest systems, dragon breeding, and progression

## Quick start for contributors

```bash
git clone https://github.com/GHAIM-bq92/Dragon-Realms.git
cd DragonRealms
# open DragonRealms.uproject in Unreal Editor 5.3
```

This repo is intentionally organized so it can grow into a larger UE5 game project without restructuring the entire codebase later.
