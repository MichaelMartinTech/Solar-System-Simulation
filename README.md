# Space Explorer 2023
### Author: ### 
` Michael Nia `
### Date of Report: ###
` 12/22/2023 `

The following is a project for UNR's Graduate Level Computer Graphics for Fall 2023.

### Implementation – UML Class Diagram ###
![alt text](https://github.com/MichaelRNia/Solar-System-Simulation/blob/main/UML-1.png?raw=true)

## Mesh Generation and Loading
The `Mesh` class is responsible for handling mesh generation and loading from files using the Assimp library. Key functionalities include:

- Constructor Overloads: Initialize the mesh with different parameters.
- Update and Render Methods: Allow dynamic changes to the model matrix for animation.
- Buffer Initialization: Initialize vertex and index buffers for rendering.
- Mesh Loading: Utilize the Assimp library to load 3D models from file paths.

## Starship, Sun, Planets, Moons, Comets
The `Object` class serves as the base for celestial bodies, and the `Sphere` class represents spherical bodies like planets. Key features include:

- Texture Handling: Manage textures through the `Texture` class.
- Initialization and Rendering: Handle buffer initialization and rendering.
- Emissive Property: Indicate whether the object emits light.
- Spherical Geometry Setup: Set up spherical geometry, including vertices, indices, texture coordinates, and normals.

## Instancing for Asteroid Belts
Matrix Stack Utilization: The `SceneGraph` class efficiently manages transformations with a matrix stack, optimizing performance during hierarchical transformations. It orchestrates the rendering process, maintains a hierarchical structure, and supports instancing for rendering asteroid belts efficiently.

Rendering Coordination: Represents a node within the scene graph, encapsulating essential information such as position, rotation, scale, and an optional renderable object.

Representation of Scene Node: Manages the hierarchical structure of celestial bodies, allowing for efficient organization and rendering of the entire celestial system.

Instancing Support: Facilitates the implementation of instancing, particularly useful for rendering asteroid belts efficiently. This enables the scene graph to handle multiple instances of similar celestial bodies, such as asteroids in a belt, optimizing rendering performance.

## Texturing
### `Texture`
The `Texture` class is responsible for handling texture loading and management. Key functionalities include:

- Texture Loading: Load texture images from file paths using the SOIL2 library.
- Texture ID Access: Provide a method to retrieve the OpenGL texture ID.

## Lighting
The `Shader` class plays a pivotal role in managing shaders and shader programs within a graphics framework, specifically focusing on lighting and shading.

### Vertex Shader
Attributes: Take vertex attributes such as position, normal, tangent, bitangent, and texture coordinates.

Outputs: Compute and output information for lighting calculations and transformations.

Transformations: Perform model-view-projection transformations.

### Fragment Shader
Uniforms: Receive various uniforms such as texture, normal map, and boolean flags.

Lighting Calculations: Compute ambient, diffuse, and specular components.

Light and Material Properties: Consider light color, light power, specular power, and material properties.

Texture Sampling: Sample textures and normal maps.

Normal Mapping: Adjust the normal vector using the tangent, bitangent, normal matrix.

### Shader Initialization and Finalization
Shader Initialization (Initialize): Create a shader program.
Adding Shaders (AddShader): Compile and attach vertex and fragment shaders to the shader program.

Shader Linking and Validation (Finalize): Link the shader program, validate it, and delete intermediate shader objects.

Shader Activation and Uniform Location Retrieval
Shader Activation (Enable): Enable the shader program for use.

Uniform Location Retrieval (GetUniformLocation): Retrieve the location of a uniform variable.

Attribute Location Retrieval (GetAttribLocation): Retrieve the location of an attribute variable.

Destructor Shader Destruction (~Shader): Delete shader objects and the shader program upon destruction.

## User Interaction
The user interaction is implemented through the `ProcessInput` method in the `Engine` class, capturing user input from the keyboard and mouse to control spaceship and camera movement. Key interactions include:

- Spaceship Movement (Exploration Mode): W, S, A, D, Q, E, R, F, Left Shift.
- Camera Control (Exploration Mode): Mouse Movement, E, Q.
- Observation Mode Toggle: O.

## Exploration Game Mode Interactions
Navigate through space using the spaceship with controls allowing movement in six degrees of freedom.

## Observation Game Mode Interactions
Toggle between exploration and observation modes. In observation mode, orbit around selected planets, cycle through planets, and control camera rotation with the mouse.

## Additional Notes
- The code uses Microsoft Visual Studio 2022 for development.
- The code uses the GLFW library for window input.
- Camera position, target, and up vector update based on the spaceship or selected planet's position and orientation.
- Includes handling of scroll wheel events (`scroll_callback`) for zooming.

## Graphics Pipeline
### 1. Graphics Class (`graphics.cpp`):
- Manages the overall graphics system and OpenGL rendering.
- Creates and handles a scene graph (`SceneGraph`).
- Initializes shaders, camera, and enables depth testing.
- Provides methods for adding objects to the scene and rendering.

### 2. SceneNode Class (`scene_node.cpp`):
- Represents a node in the scene graph with transformation properties.
- Can have a renderable object associated with it.
- Handles local and world space transformations.
- Manages child nodes in a hierarchical structure.
- Renders itself and its children, setting up textures.

### 3. SceneGraph Class (`scene_graph.cpp`):
- Manages the scene graph structure.
- Keeps track of a matrix stack for hierarchical transformations.
- Renders the entire scene graph by invoking rendering on the root node.
- Provides methods for adding nodes to the scene graph.

### 4. Engine Class (`engine.cpp`):
- Manages the application's core functionalities, including window creation, input processing, and the main game loop.
- Interacts with the graphics system through the `Graphics` class, updating and rendering the scene.
- Handles user input for camera and player movement, allowing for exploration and planetary observation modes.

## Conclusion
The code creates a modular graphics system with a scene graph to organize and render objects. The `Graphics` class initializes the system, the `SceneNode` class represents nodes with transformations and optional renderable objects, and the `SceneGraph` class manages the overall structure and rendering of the scene graph. The development environment is Microsoft Visual Studio 2022, and the code utilizes the OpenGL graphics framework.

## Update Log ##
### 1/13/2024 ###
`4:33 PM PST` - Github created with readme

`4:40 PM PST` - Uploaded UML Diagram
