# Unreal-GAS-Multiplayer-Combat-System
🧠 Overview

A multiplayer combat system built in Unreal Engine using the Gameplay Ability System (GAS), supporting both melee and projectile-based abilities.

The project focuses on building a complete gameplay pipeline from input → ability execution → animation → hit/impact → network synchronization.

⚙️ Features
GAS-based ability system with full lifecycle control (Activate → Commit → End)
Supports both melee and projectile abilities
Animation-driven execution using montages
Data-driven skill configuration using DataAssets and GameplayTags
Server-authoritative gameplay with client-side responsiveness

🌐 Networking
Server-authoritative architecture
Local execution for responsive input
Ability validation performed on server via CommitAbility
Designed to handle multiplayer synchronization for combat interactions

🔑 Key Implementation
Custom ability base class controlling execution flow and animation callbacks
Projectile system integrated with ability execution
Clean separation between gameplay logic and animation-driven behavior

🚧 Challenges
Handling non-replicated actors (e.g. projectiles) in a multiplayer environment
Implementing client-side prediction for responsive ability execution
Managing prediction confirmation and rollback using FPredictionKey
Resolved an issue where simulated clients did not play animation montages correctly
Ensured animation execution is properly synchronized across network roles
