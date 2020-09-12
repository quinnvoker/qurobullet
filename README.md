
<a href='https://ko-fi.com/I2I51OMV9' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://cdn.ko-fi.com/cdn/kofi4.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
# qurobullet
A powerful 2D projectile system module for Godot!

[Check out the release demo video here!](https://www.youtube.com/watch?v=Pye_jmW4an0)

**qurobullet** provides a means of easily creating and processing thousands of configurable 2D projectiles in Godot. It handles the heavy-lifting of spawning, movement, and collision detection for large groups of bullets, and gives the user tools for designing bullet-hell-worthy patterns!

It consists of three main components: the `BulletServer` and `BulletSpawner` nodes, and the `BulletType` resource. 

 - A `BulletServer` creates a configurable pool of bullet objects (not nodes), controls their movement, and reports their collisions. The bullet pool does not grow, and recycles the oldest bullets if overloaded.
 - A `BulletSpawner` calculates various arrangements to spawn bullets in, and "spawns" bullets by sending a signal to a `BulletServer` containing the positions, directions, and `BulletType` desired. It features a preview drawing system which shows the positions and directions their bullets will travel, which simplifies the process of creating new patterns via `AnimationPlayer` .
 - A `BulletType` is a container for bullet data, and bullets' appearance and behaviour are determined by the type they hold. Contains a dictionary called `custom_data` which can be used for easy extension of the type (for example, you could give it an `AudioStream` to associate the type with custom hit sound).
 
More specific information about these components and their properties can be found in the built-in documentation.

## Installation
Because **qurobullet** is a module, it must be added to the modules folder of Godot's source and compiled into a custom build. If you are unfamiliar with the process of compiling software from source, [Godot's documentation can point you in the right direction for any platform](https://docs.godotengine.org/en/stable/development/compiling/index.html).

This module does not have dependencies beyond those required to build Godot.

The installation method can vary a bit, depending on your OS and whether you're cloning Godot's git repo or downloading a zipped version (and whether you want to keep the module up to date with this repository), but so long as all of the source files from this repository end up in `<your_godot_source_folder>/modules/qurobullet`, you're all set to compile the engine as normal, and **qurobullet** will be included!

If you wish to clone this module into your own Godot git repository, I would recommend adding it as a submodule. This will allow you to keep them up to date independently without interfering with each other. To do so, open a terminal and enter the following:

    cd <your_godot_source_folder>/modules
    git submodule add https://github.com/quinnvoker/qurobullet

This will clone this repo into your modules folder as a submodule, and you're ready to compile!

## How to use
Once you've added the module to your Godot build, using **qurobullet** in your project is straightforward. Add a `BulletServer` and `BulletSpawner` to your scene, define a `BulletType` for the spawner to use, and you're done!

No manual signal connections are required for these components, because they automatically connect themselves to the included `BulletServerRelay` singleton object on `ready`. Because of this, one server object can receive signals from all spawners in any given scene (including those within instanced scenes), without needing configuration! This can be disabled, and connections can also be made manually. This is handy if your project specifically requires multiple `BulletServer` nodes for different uses (for example, a separate server for the player's bullets so they can be recycled with high efficiency, or just to have separate visual layers of bullets).

Because bullets are objects rather than area or body nodes, their collision shapes are not monitorable, and they will not trigger emission of any `area_entered` or `body_entered` signals from `CollisionObject2D`s in your scene. Instead, when a bullet collides with something, the `BulletServer` running it will emit a `collision_detected` signal, with the bullet and an array containing the `CollisionObject2D`s it has contacted as arguments. You are free to process collisions in your game however you like using this signal. You could connect it to a function which reads the bullet's type data and uses it to apply the appropriate amount of damage to the colliders it hit that frame, or perhaps just read the bullet type's `custom_data` and play some `AudioStream` you have stored there, for example. 

## Modularity
Because all communication between components is done via signals, **qurobullet** is naturally modular. You can write your own custom spawners and connect them to the `BulletServerRelay` or a `BulletServer` seamlessly, so long as you provide the required data! 

`BulletServer` has two public methods for spawning bullets, `spawn_bullet`, and `spawn_volley`.

`spawn_bullet`  spawns a single bullet, and expects the following arguments:

 - The `BulletType` to use
 - The starting position of the bullet in world space, as a `Vector2`
 - The direction in which the bullet will travel, as a `Vector2` (must be normalised)

`spawn_volley` spawns any number of bullets at once, and expects the following arguments:

 - The `BulletType` to use
 - The origin of the spawner in world space, as a `Vector2`
 - An `Array` containing spawn info for each shot. This spawn info is stored as a `Dictionary` containing two entries, *"position"* (bullet spawn position relative to origin, as a `Vector2`), and *"direction"* (bullet travel direction, also a `Vector2`, and must be normalised)

Knowing this, you can spawn bullets any way you wish via code! You could even code your own `BulletServer` implementation, if all you needed was a configurable spawner and bullet data definition...
 
## Known Issues
 - Combining the use of `arc_offset` and `MANUAL` pattern mode in `BulletSpawner` is not recommended and results in undesired behaviour, because `arc_offset`'s shot wrapping (applied when a shot would be offset beyond the arc's range) modifies shot indices.
 - Bullets set to collide with Area2Ds will ignore their `monitorable` status and always see them.
 - `BulletSpawner` does its best to prevent "stacking" bullets by combining them into one if both their directions and spawn positions are approximately equal when spawned. However, this is not a guarantee, and spawners can still stack bullets when shots are scattered individually.

## Note from the author
*It's finally here! After 6 months of development and multiple rewrites, **qurobullet** 1.0 is complete, and the project is now open source!*
 
*It started off as a simple bullet spawning node written in GDScript, and began to immediately take off in complexity when I decided to try writing a visualiser for planning waves... Suddenly, with the shots appearing in front of me, all I could think was "okay, but what if I could try making it do THIS" and adding layer upon layer of configuration options. I got a lot of positive feedback from the Godot subreddit and wanted to try making a tool that others could benefit from.*
 
*After a few months, it showed signs that it could collapse under its own weight... The systems were getting more convoluted and harder to change, adding new features began to hurt performance noticeably, etc. It was time for a rewrite. While I could have stuck to GDScript and still improved peformance, I decided to try C++, as I'd always been somewhat intimidated to use it and this seemed like a good opportunity to conquer that fear.*
 
*I went with module development rather than GDNative C++ script, because I wanted to get more familiar with how Godot works and working directly with the source gave me a lot of closely related reference material to draw from as a new C++ user. The rewrite went smoothly, and now I've achieved kinds of functionality and better performance than I ever imagined early on!*
 
***qurobullet*** *has been a blast to work on, and it (as well as the support of the Godot community) has given me a lot more confidence in myself as a programmer... I've always sort of drifted along on minimum wage jobs doing whatever pays bills, but I think at this point I want to get serious and try and turn programming a living, somehow. Thank you all for everything!*
 
*So here it is. Do whatever you want with it! I hope you like it.*

-quinn
