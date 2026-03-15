# Minimal Renderer

This is a learning project to get a deeper understanding of the render pipeline in 3D Graphics.

The project follows the [Learn OpenGL](https://learnopengl.com/) online resource, but where it's appropriate, some of the functionality is abstracted into separate classes. This allows for a easier usage as new concepts are taught.

I've also created a 'Scene' system, that lets me build separate demos, for each new concept.

> [!NOTE]
> This is a learning project, the code base is not the most optimal. I'll plan to clean it up as i go.

## Progress:

<details>
  <summary><b>Click to expand gallery</b></summary>

## Basics shape drawing:

  <p align="center">
    <img src="screenshots\00_ClearColor.png" width="45%">
    <img src="screenshots\01_Hello_Triangle.png" width="45%">
    <img src="screenshots\03_Textures.png" width="45%">
    <img src="screenshots\04_Transform.png" width="45%">
  </p>

## 3D coordinates:

  <p align="center">
    <img src="screenshots\05_CoordinateSystem.png" width="45%">
    <img src="screenshots\06_Cube.png" width="45%">
    <img src="screenshots\07_MultipleCubes.png" width="45%">
  </p>

## Shading & Lights:

  <p align="center">
    <img src="screenshots\05_CoordinateSystem.png" width="45%">
    <img src="screenshots\06_Cube.png" width="45%">
    <img src="screenshots\07_MultipleCubes.png" width="45%">
    <img src="screenshots\08_PhongMaterial.png" width="45%">
    <img src="screenshots\10_SurfaceMaps.png" width="45%">
    <img src="screenshots\12_MultipleLights.png" width="45%">
  </p>

## Model Loading:

  <p align="center">
    <img src="screenshots\13_ModelLoading.png" width="45%">
  </p>

## Culling & Blending:

  <p align="center">
    <img src="screenshots\15_StencilTest.png" width="45%">
    <img src="screenshots\16_Blending.png" width="45%">
  </p>

## Cube map:

  <p align="center">
    <img src="screenshots\20_CubeMap_Skybox.png" width="45%">
    <img src="screenshots\20_CubeMap_Reflections.png" width="45%">
    <img src="screenshots\20_CubeMap_Refraction.png" width="45%">
  </p>
</details>

## 3rd Party libraries used:

- ImGui
- GLFW
- GLM
- glad
- stb_image
