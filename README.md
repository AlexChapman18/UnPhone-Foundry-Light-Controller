# UnPhone Student Union Lighting Controller
By Kush Bharakhada (kbharakhada1@sheffield.ac.uk) and Alex Chapman (achapman5@sheffield.ac.uk)

## Project Description

The objective of this project has been to control the LED lighting strips throughout different rooms in the Student Union [REF], employing a variety of effects and colours, all managed by the UnPhone [REF]. Currently, the lighting system relies on dedicated software and a computer interface, which provides additional complexity for individuals without specialised knowledge in lighting engineering. With our approach, the user has the ability to use the UnPhone to select an architecture (group of lights) and control their colours and effects easily. While our project may offer less flexibility compared to the conventional lighting control, its simplicity provides users with a variety of features tailored for events and occasions.

## Light Control Permissions

Permission was requested and granted from (the lighting people) for access to its lighting system, allowing the project to undergo live testing and visualisation of its performance.

## Design

### Application Features

<center>

| Feature | Description |
| ------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Choosing an Architecture | Allow the user to choose which architecture (lighting group) they would like to change the colour for. The architectures must be pre-defined and the user must be able to easily press the architecture which they would like to control. |
| Default Colour Options | When an architecture is selected, the user must be provided an array of default colour options to choose from and set the lights to.|
| RGB Colour Faders | For more flexible colour options, the user must have the ability to set RGB colours using faders (sliders). There must be three faders. For user experience, if a default colour option is chosen, the RGB faders must automatically update and set to the corresponding RGB levels.|
| Switching Lights OFF | The user must be given the ability to turn the lights OFF, individually for an architecture.|
| Default Effects | Allow the user to visualise lighting animations, from a pre-defined, set of effects.|
| Intensity and Speed | Provide the user with the ability to utilise faders to control the speed of effects and the lighting intensity. |
| Global Colour View | For a global view of colours, the user must be able to visualise every single architecture and its currently set colour on one screen. 
| UnPhone Buttons (Triangle, Circle and Square) | UnPhone will be utilised for navigation. Therefore, there must be three main screens architecture selection, setting *effects and intensity, and global colour view. Setting a colour must be navigated to via the architecture screen. |

</center>

### Architectures

<center>

| Architecture | Number of Lights in Group |
| ------------ | ------------------------- |
| a            | i                         |
| b            | j                         |
| c            | k                         |
| d            | l                         |
| e            | m                         |
| f            | n                         |
| g            | o                         |
| h            | p                         |

</center>

### Default Colour Options

The default colours the user can select from have been decided from the 12 colour wheel, as illustrated below. This presents the user with a sufficient number of default colours as well as the ability to create custom colours using RGB faders.

<center>

|       | Red | Rose | Magenta | Violet | Blue | Azure | Cyan | Aquamarine | Green | Chartreuse | Yellow | Orange |
| ----- | --- | ---- | ------- | ------ | ---- | ----- | ---- | ---------- | ----- | ---------- | ------ | ------ |
| **R** | 255 | 255  | 255     | 127    | 0    | 0     | 0    | 0          | 0     | 127        | 255    | 255    |
| **G** | 0   | 0    | 0       | 0      | 0    | 127   | 255  | 255        | 255   | 255        | 255    | 127    |
| **B** | 0   | 127  | 255     | 255    | 255  | 255   | 255  | 127        | 0     | 0          | 0      | 0      |

</center>

### Lighting Effects

<center>

| Effect Name | Description |
| ----------- | ----------- |
| a           | i           |
| b           | j           |
| c           | k           |
| d           | l           |
| e           | m           |
| f           | n           |
| g           | o           |
| h           | p           |

</center>

### User Interface

The application will consist of four main screens:

<center>

| Screen | Navigation | Description | Interactive Screen |
| ---------------------------- | ------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------ |
| Architecture Screen | Physical Triangle Button | Presents the user with buttons to choose an architecture that they would like to change the colours for. | YES |
| Colour Screen | From Architecture Screen | Presents the user with default colour options as well as RGB faders for custom colours. | YES |
| Effects and Intensity Screen | Physical Circle Button | Presents the user with two faders; intensity of lighting and speed of effects. Furthermore, buttons will be presented for various lighting effects. | YES |
| Global Colour Screen | Physical Square Button | Presents the user with all architectures with its current lighting colour (black if the corresponding architecture is OFF). | NO|

</center>

Wireframes illustrating the designs of the screens:

<center>
<img src="images/wireframe-screens.png" alt="breadboard" width="700"/>
<img src="images/wireframe-navigation-buttons.png" alt="breadboard" width="250"/>
</center>

Diagram to illustrate the connection and navigation between different screens:

<center>
<img src="images/navigation-diagram.png" alt="breadboard" width="350"/>
</center>

### Networking and Backend

Temporary text.

## Implementation

### UnPhone to NuPhone

Temporary text.

### Libraries Used

Temporary text.

#### Testing

Temporary text.

